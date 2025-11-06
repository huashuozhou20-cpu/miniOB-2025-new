#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
SQL 解析器潜在问题检查脚本
"""

import re
import os

def check_lex_keywords():
    """检查 lex 文件中的关键字定义"""
    print("=" * 60)
    print("1. 检查 lex_sql.l 关键字定义")
    print("=" * 60)
    
    lex_file = "src/observer/sql/parser/lex_sql.l"
    if not os.path.exists(lex_file):
        print(f"✗ 文件不存在: {lex_file}")
        return False
    
    with open(lex_file, 'r') as f:
        content = f.read()
    
    # 提取所有关键字规则
    keyword_pattern = r'^([A-Z_]+)\s+RETURN_TOKEN\(([A-Z_]+)\);'
    keywords = re.findall(keyword_pattern, content, re.MULTILINE)
    
    print(f"找到 {len(keywords)} 个关键字定义")
    
    # 检查重复定义
    keyword_names = [k[0] for k in keywords]
    duplicates = [k for k, v in __import__('collections').Counter(keyword_names).items() if v > 1]
    if duplicates:
        print(f"✗ 发现重复的关键字: {duplicates}")
        return False
    else:
        print("✓ 没有重复关键字")
    
    # 检查 VECTOR 相关
    vector_keywords = [k for k in keywords if 'VECTOR' in k[0]]
    print(f"\nVECTOR 相关关键字:")
    for kw, token in vector_keywords:
        print(f"  {kw} -> {token}")
    
    # 检查关键字顺序（关键字应该在 ID_KEY 之前）
    id_key_line = content.find('{ID_KEY}')
    if id_key_line > 0:
        keyword_section = content[:id_key_line]
        if 'VECTOR' in keyword_section and 'VECTOR_T' in keyword_section:
            print("✓ VECTOR 关键字在 ID_KEY 之前定义")
        else:
            print("⚠ VECTOR 关键字位置需要检查")
    
    return True

def check_yacc_tokens():
    """检查 yacc 文件中的 token 声明"""
    print("\n" + "=" * 60)
    print("2. 检查 yacc_sql.y token 声明")
    print("=" * 60)
    
    yacc_file = "src/observer/sql/parser/yacc_sql.y"
    if not os.path.exists(yacc_file):
        print(f"✗ 文件不存在: {yacc_file}")
        return False
    
    with open(yacc_file, 'r') as f:
        lines = f.readlines()
    
    # 查找 %token 区域
    in_tokens = False
    tokens = []
    for line in lines:
        if '%token' in line and 'SEMICOLON' in line:
            in_tokens = True
            continue
        if in_tokens:
            if line.strip() and not line.strip().startswith('//'):
                parts = line.split()
                for part in parts:
                    if part and part not in ['%token', 'SEMICOLON'] and not part.startswith('//'):
                        tokens.append(part)
            elif line.strip() == '' and tokens and len(tokens) > 10:
                break
    
    print(f"找到 {len(set(tokens))} 个 unique tokens")
    
    # 检查 VECTOR 相关 tokens
    vector_tokens = [t for t in tokens if 'VECTOR' in t]
    print(f"\nVECTOR 相关 tokens:")
    for token in set(vector_tokens):
        print(f"  {token}")
    
    if 'VECTOR' in tokens and 'VECTOR_T' in tokens:
        print("✓ VECTOR 和 VECTOR_T 都已声明")
    else:
        print("✗ VECTOR 或 VECTOR_T token 声明缺失")
        return False
    
    return True

def check_type_rule():
    """检查 type 规则是否支持 VECTOR"""
    print("\n" + "=" * 60)
    print("3. 检查 type 语法规则")
    print("=" * 60)
    
    yacc_file = "src/observer/sql/parser/yacc_sql.y"
    with open(yacc_file, 'r') as f:
        content = f.read()
    
    # 查找 type 规则
    type_pattern = r'type:\s*((?:[^;]|;)*?)date_type:'
    match = re.search(type_pattern, content, re.DOTALL)
    if match:
        type_section = match.group(1)
        print("type 规则内容:")
        lines = type_section.strip().split('\n')
        for line in lines:
            if line.strip():
                print(f"  {line.strip()}")
        
        if 'VECTOR_T' in type_section and 'VECTOR' in type_section:
            print("\n✓ type 规则同时支持 VECTOR_T 和 VECTOR")
        elif 'VECTOR_T' in type_section:
            print("\n⚠ type 规则只支持 VECTOR_T")
        else:
            print("\n✗ type 规则不支持 VECTOR")
            return False
    else:
        print("✗ 未找到 type 规则")
        return False
    
    return True

def check_attr_def_rules():
    """检查 attr_def 规则"""
    print("\n" + "=" * 60)
    print("4. 检查 attr_def 语法规则")
    print("=" * 60)
    
    yacc_file = "src/observer/sql/parser/yacc_sql.y"
    with open(yacc_file, 'r') as f:
        content = f.read()
    
    # 查找 attr_def 规则
    attr_def_pattern = r'attr_def:\s*((?:[^;]|;)*?);'
    match = re.search(attr_def_pattern, content, re.DOTALL)
    if match:
        attr_def_section = match.group(1)
        print("attr_def 规则:")
        lines = attr_def_section.strip().split('\n')
        for i, line in enumerate(lines[:15]):  # 只显示前15行
            if line.strip():
                print(f"  {line.strip()}")
        
        # 检查是否有支持带长度的类型（如 VECTOR(3)）
        if 'type LBRACE number RBRACE' in attr_def_section:
            print("\n✓ 支持带长度的类型定义（如 VECTOR(3)）")
        else:
            print("\n✗ 不支持带长度的类型定义")
            return False
    else:
        print("✗ 未找到 attr_def 规则")
        return False
    
    return True

def check_common_issues():
    """检查常见问题"""
    print("\n" + "=" * 60)
    print("5. 检查常见潜在问题")
    print("=" * 60)
    
    issues = []
    
    # 检查是否有大小写敏感问题
    lex_file = "src/observer/sql/parser/lex_sql.l"
    if os.path.exists(lex_file):
        with open(lex_file, 'r') as f:
            lex_content = f.read()
            if 'case-insensitive' in lex_content:
                print("✓ SQL 关键字大小写不敏感")
            else:
                issues.append("SQL 关键字可能是大小写敏感的")
    
    # 检查 TEXT vs TEXT_T
    yacc_file = "src/observer/sql/parser/yacc_sql.y"
    if os.path.exists(yacc_file):
        with open(yacc_file, 'r') as f:
            yacc_content = f.read()
            if 'TEXT_T' in yacc_content and 'TEXT' in yacc_content:
                # 检查 TEXT 是否也返回 TEXT_T
                lex_content = open(lex_file).read()
                if 'TEXT.*RETURN_TOKEN.*TEXT_T' in lex_content or 'TEXT                                    RETURN_TOKEN(TEXT_T)' in lex_content:
                    print("✓ TEXT 关键字正确映射到 TEXT_T")
                else:
                    issues.append("TEXT 关键字可能未正确映射")
    
    if issues:
        print("\n⚠ 发现潜在问题:")
        for issue in issues:
            print(f"  - {issue}")
    else:
        print("✓ 未发现常见问题")
    
    return len(issues) == 0

def main():
    print("SQL 解析器潜在问题检查")
    print("=" * 60)
    
    os.chdir(os.path.dirname(os.path.abspath(__file__)) or '.')
    
    results = []
    results.append(check_lex_keywords())
    results.append(check_yacc_tokens())
    results.append(check_type_rule())
    results.append(check_attr_def_rules())
    results.append(check_common_issues())
    
    print("\n" + "=" * 60)
    print("检查总结")
    print("=" * 60)
    passed = sum(results)
    total = len(results)
    print(f"通过: {passed}/{total}")
    
    if passed == total:
        print("\n✓ 所有检查通过！")
        return 0
    else:
        print(f"\n✗ 有 {total - passed} 项检查失败")
        return 1

if __name__ == "__main__":
    exit(main())

