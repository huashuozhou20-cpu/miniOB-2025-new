#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
全面功能验证脚本 - 检查所有功能是否真的实现
"""

import os
import re
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SRC_DIR = os.path.join(SCRIPT_DIR, "src/observer")

def check_file_exists(filepath):
    """检查文件是否存在"""
    full_path = os.path.join(SCRIPT_DIR, filepath)
    return os.path.exists(full_path)

def grep_in_files(filepaths, pattern):
    """在多个文件中搜索模式"""
    results = []
    for filepath in filepaths:
        full_path = os.path.join(SCRIPT_DIR, filepath)
        if not os.path.exists(full_path):
            continue
        try:
            with open(full_path, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                if re.search(pattern, content, re.IGNORECASE):
                    results.append(filepath)
        except:
            pass
    return results

def check_implementation(name, files, keywords, syntax_check=None):
    """检查功能实现"""
    print(f"\n{'='*70}")
    print(f"检查: {name}")
    print(f"{'='*70}")
    
    file_status = {}
    for filepath in files:
        exists = check_file_exists(filepath)
        file_status[filepath] = exists
        status = "✓" if exists else "✗"
        print(f"  {status} {filepath}")
    
    keyword_status = {}
    all_files_exist = all(file_status.values())
    
    if all_files_exist and keywords:
        print(f"\n  检查关键字:")
        for keyword in keywords:
            matches = grep_in_files(files, keyword)
            found = len(matches) > 0
            keyword_status[keyword] = found
            status = "✓" if found else "✗"
            print(f"    {status} {keyword}: {len(matches)} 处匹配")
    
    # 语法检查
    if syntax_check:
        yacc_file = os.path.join(SRC_DIR, "sql/parser/yacc_sql.y")
        if os.path.exists(yacc_file):
            with open(yacc_file, 'r', encoding='utf-8', errors='ignore') as f:
                content = f.read()
                syntax_found = bool(re.search(syntax_check, content, re.IGNORECASE))
                status = "✓" if syntax_found else "✗"
                print(f"  {status} 语法规则: {syntax_check}")
                keyword_status['syntax'] = syntax_found
    
    # 总结
    all_good = all_files_exist and all(keyword_status.values()) if keyword_status else all_files_exist
    
    if all_good:
        print(f"\n  总结: ✓ 功能已实现")
        return True
    else:
        print(f"\n  总结: ✗ 功能未完全实现")
        return False

def main():
    print("="*70)
    print("MiniOB 全面功能验证")
    print("="*70)
    
    # 功能检查列表
    checks = [
        # 基础功能
        ("drop-table", 
         ["src/observer/sql/stmt/drop_table_stmt.cpp", 
          "src/observer/sql/executor/drop_table_executor.cpp"],
         ["drop_table", "DROP TABLE"],
         r"drop_table_stmt\s*:"),
        
        ("update",
         ["src/observer/sql/stmt/update_stmt.cpp",
          "src/observer/sql/operator/update_physical_operator.cpp"],
         ["update_record", "UPDATE"],
         r"update_stmt\s*:"),
        
        ("date",
         ["src/observer/common/type/date_type.cpp"],
         ["Date", "DATE_T"],
         r"DATE_T|date_type\s*:"),
        
        ("join-tables",
         ["src/observer/sql/optimizer/logical_plan_generator.cpp"],
         ["JoinLogicalOperator", "joined_tables", "INNER JOIN"],
         r"INNER.*JOIN|join_list"),
        
        ("expression",
         ["src/observer/sql/expr/arithmetic_expr.cpp"],
         ["ArithmeticExpr", "add|sub|mul|div"],
         r"expression\s*:|arithmetic"),
        
        ("function",
         ["src/observer/sql/expr/sysfunc_expr.cpp"],
         ["eval_length", "eval_round", "eval_date_format", "LENGTH", "ROUND", "DATE_FORMAT"],
         r"LENGTH|ROUND|DATE_FORMAT"),
        
        ("multi-index",
         ["src/observer/storage/index/bplus_tree.cpp"],
         ["idx_col_list", "multi.*field"],
         r"idx_col_list"),
        
        ("unique",
         ["src/observer/storage/index/bplus_tree.cpp",
          "src/observer/sql/stmt/create_index_stmt.cpp"],
         ["unique", "UNIQUE", "is_unique"],
         r"UNIQUE|unique_option"),
        
        ("group-by",
         ["src/observer/sql/optimizer/logical_plan_generator.cpp"],
         ["GroupBy", "HAVING", "aggregate"],
         r"GROUP BY|group_by"),
        
        ("union",
         ["src/observer/sql/operator/union_physical_operator.cpp",
          "src/observer/sql/operator/union_logical_operator.h"],
         ["UnionPhysicalOperator", "union_all", "UNION"],
         r"UNION|union_select"),
        
        ("vector-basic",
         ["src/observer/common/type/vector_type.cpp",
          "src/observer/sql/expr/sysfunc_expr.cpp"],
         ["VectorType", "DISTANCE", "VECTOR_TO_STRING", "STRING_TO_VECTOR"],
         r"VECTOR|DISTANCE"),
        
        ("alter",
         ["src/observer/sql/stmt/alter_table_stmt.cpp",
          "src/observer/storage/table/table_alter.cpp"],
         ["ADD COLUMN", "DROP COLUMN", "RENAME", "alter_table"],
         r"ALTER TABLE|alter_table_stmt"),
        
        ("create-view",
         ["src/observer/storage/table/view.cpp",
          "src/observer/sql/stmt/create_view_stmt.cpp"],
         ["View", "create_view", "CREATE VIEW"],
         r"CREATE VIEW|create_view_stmt"),
        
        ("full-text-index",
         ["src/observer/storage/index/fulltext_index.cpp"],
         ["FullTextIndex", "MATCH", "AGAINST", "TOKENIZE", "BM25"],
         r"FULLTEXT|MATCH.*AGAINST"),
        
        ("big-order-by",
         ["src/observer/sql/operator/order_by_physical_operator.cpp"],
         ["external_sort", "MEMORY_THRESHOLD", "merge.*chunk"],
         None),
    ]
    
    results = {}
    for name, files, keywords, syntax in checks:
        result = check_implementation(name, files, keywords, syntax)
        results[name] = result
    
    # 生成报告
    print("\n" + "="*70)
    print("验证总结")
    print("="*70)
    
    implemented = [name for name, status in results.items() if status]
    not_implemented = [name for name, status in results.items() if not status]
    
    print(f"\n✓ 已实现 ({len(implemented)}):")
    for name in sorted(implemented):
        print(f"  - {name}")
    
    print(f"\n✗ 未实现 ({len(not_implemented)}):")
    for name in sorted(not_implemented):
        print(f"  - {name}")
    
    print(f"\n总计: {len(results)} 项功能")
    print(f"已实现: {len(implemented)} ({len(implemented)*100//len(results)}%)")
    print(f"未实现: {len(not_implemented)} ({len(not_implemented)*100//len(results)}%)")

if __name__ == "__main__":
    main()

