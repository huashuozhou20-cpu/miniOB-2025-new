#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
MiniOB 功能完整性验证脚本
检查所有任务是否真的实现
"""

import os
import re
import subprocess

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
SRC_DIR = os.path.join(SCRIPT_DIR, "src/observer")

def check_file_exists(filepath):
    """检查文件是否存在"""
    return os.path.exists(filepath)

def grep_in_file(filepath, pattern, context=0):
    """在文件中搜索模式"""
    if not os.path.exists(filepath):
        return []
    
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
            
        results = []
        for i, line in enumerate(lines):
            if re.search(pattern, line, re.IGNORECASE):
                start = max(0, i - context)
                end = min(len(lines), i + context + 1)
                results.append((i+1, line.strip()))
        return results
    except:
        return []

def check_yacc_rule(filepath, rule_name):
    """检查 yacc 文件中是否有特定规则"""
    return grep_in_file(filepath, rf"{rule_name}\s*:", 0)

def check_cpp_implementation(filepath, symbols):
    """检查 C++ 文件中是否有特定符号实现"""
    if not os.path.exists(filepath):
        return False
    
    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
        content = f.read()
    
    for symbol in symbols:
        if symbol not in content:
            return False
    return True

def verify_functionality(name, status, checks):
    """验证功能实现"""
    print(f"\n{'='*70}")
    print(f"验证: {name} (标记: {'已过' if status == 'passed' else '未过'})")
    print(f"{'='*70}")
    
    all_passed = True
    results = []
    
    for check_name, check_func, expected in checks:
        result = check_func()
        passed = result == expected
        
        status_mark = "✓" if passed else "✗"
        print(f"  {status_mark} {check_name}: {'通过' if passed else '失败'}")
        
        if not passed:
            all_passed = False
            if isinstance(result, list) and result:
                print(f"     找到: {len(result)} 处匹配")
            elif isinstance(result, bool):
                print(f"     期望: {expected}, 实际: {result}")
        
        results.append((check_name, passed))
    
    if all_passed:
        print(f"  总结: ✓ 功能已实现")
        return True, 'implemented'
    else:
        print(f"  总结: ✗ 功能未完全实现")
        return False, 'not_implemented' if status == 'passed' else 'not_implemented'

def main():
    print("="*70)
    print("MiniOB 功能完整性验证")
    print("="*70)
    
    yacc_file = os.path.join(SRC_DIR, "sql/parser/yacc_sql.y")
    
    # 功能验证列表
    functionalities = []
    
    # 1. drop-table
    functionalities.append((
        "drop-table",
        "passed",
        [
            ("DROP TABLE 语法规则", lambda: check_yacc_rule(yacc_file, "drop_table_stmt"), True),
            ("DropTableStmt 实现", lambda: check_file_exists(os.path.join(SRC_DIR, "sql/stmt/drop_table_stmt.cpp")), True),
        ]
    ))
    
    # 2. update
    functionalities.append((
        "update",
        "passed",
        [
            ("UPDATE 语法规则", lambda: check_yacc_rule(yacc_file, "update_stmt"), True),
            ("UpdateStmt 实现", lambda: check_file_exists(os.path.join(SRC_DIR, "sql/stmt/update_stmt.cpp")), True),
        ]
    ))
    
    # 3. date
    functionalities.append((
        "date",
        "passed",
        [
            ("DATE 类型定义", lambda: grep_in_file(yacc_file, "DATE_T|date_type"), True),
            ("Date 类型实现", lambda: check_file_exists(os.path.join(SRC_DIR, "common/type/date_type.cpp")), True),
        ]
    ))
    
    # 4. join-tables
    functionalities.append((
        "join-tables",
        "passed",
        [
            ("INNER JOIN 语法", lambda: grep_in_file(yacc_file, "INNER.*JOIN|join_list"), True),
            ("JoinLogicalOperator", lambda: grep_in_file(os.path.join(SRC_DIR, "sql/optimizer/logical_plan_generator.cpp"), "JoinLogicalOperator|joined_tables"), True),
        ]
    ))
    
    # 5. expression
    functionalities.append((
        "expression",
        "passed",
        [
            ("算术表达式", lambda: grep_in_file(yacc_file, "expression|arithmetic"), True),
            ("ArithmeticExpr", lambda: check_file_exists(os.path.join(SRC_DIR, "sql/expr/arithmetic_expr.cpp")), True),
        ]
    ))
    
    # 6. function
    functionalities.append((
        "function",
        "failed",
        [
            ("LENGTH 函数", lambda: grep_in_file(yacc_file, "\\bLENGTH\\b"), True),
            ("ROUND 函数", lambda: grep_in_file(yacc_file, "\\bROUND\\b"), True),
            ("DATE_FORMAT 函数", lambda: grep_in_file(yacc_file, "DATE_FORMAT"), True),
            ("SysFuncExpr 实现", lambda: check_file_exists(os.path.join(SRC_DIR, "sql/expr/sysfunc_expr.cpp")), True),
        ]
    ))
    
    # 7. multi-index
    functionalities.append((
        "multi-index",
        "passed",
        [
            ("多字段索引语法", lambda: grep_in_file(yacc_file, "idx_col_list"), True),
            ("索引创建", lambda: grep_in_file(yacc_file, "create_index_stmt"), True),
        ]
    ))
    
    # 8. unique
    functionalities.append((
        "unique",
        "failed",
        [
            ("UNIQUE 关键字", lambda: grep_in_file(yacc_file, "\\bUNIQUE\\b"), True),
            ("unique_option", lambda: grep_in_file(yacc_file, "unique_option"), True),
            ("唯一索引检查", lambda: grep_in_file(os.path.join(SRC_DIR, "storage/index/bplus_tree.cpp"), "unique|duplicate"), True),
        ]
    ))
    
    # 9. group-by
    functionalities.append((
        "group-by",
        "passed",
        [
            ("GROUP BY 语法", lambda: grep_in_file(yacc_file, "GROUP BY|group_by"), True),
            ("聚合函数", lambda: grep_in_file(yacc_file, "COUNT|SUM|AVG|MAX|MIN|aggregate"), True),
            ("HAVING", lambda: grep_in_file(yacc_file, "HAVING|having"), True),
        ]
    ))
    
    # 10. union
    functionalities.append((
        "union",
        "failed",
        [
            ("UNION 语法", lambda: grep_in_file(yacc_file, "UNION|union"), True),
            ("UNION ALL", lambda: grep_in_file(yacc_file, "UNION.*ALL"), True),
            ("UnionExecutor", lambda: check_file_exists(os.path.join(SRC_DIR, "sql/executor/union_executor.cpp")), True),
        ]
    ))
    
    # 11. vector-basic
    functionalities.append((
        "vector-basic",
        "failed",
        [
            ("VECTOR 类型", lambda: grep_in_file(yacc_file, "VECTOR|VECTOR_T"), True),
            ("DISTANCE 函数", lambda: grep_in_file(yacc_file, "DISTANCE|distance"), True),
            ("VECTOR_TO_STRING", lambda: grep_in_file(yacc_file, "VECTOR_TO_STRING"), True),
            ("STRING_TO_VECTOR", lambda: grep_in_file(yacc_file, "STRING_TO_VECTOR"), True),
            ("vector_type 实现", lambda: check_file_exists(os.path.join(SRC_DIR, "common/type/vector_type.cpp")), True),
        ]
    ))
    
    # 12. alter
    functionalities.append((
        "alter",
        "failed",
        [
            ("ALTER TABLE 语法", lambda: grep_in_file(yacc_file, "ALTER TABLE|alter_table_stmt"), True),
            ("ADD COLUMN", lambda: grep_in_file(yacc_file, "ADD COLUMN"), True),
            ("DROP COLUMN", lambda: grep_in_file(yacc_file, "DROP COLUMN"), True),
            ("CHANGE COLUMN", lambda: grep_in_file(yacc_file, "CHANGE COLUMN"), True),
            ("RENAME TO", lambda: grep_in_file(yacc_file, "RENAME TO"), True),
        ]
    ))
    
    # 13. create-view
    functionalities.append((
        "create-view",
        "failed",
        [
            ("CREATE VIEW 语法", lambda: grep_in_file(yacc_file, "CREATE VIEW|create_view_stmt"), True),
            ("View 实现", lambda: check_file_exists(os.path.join(SRC_DIR, "storage/table/view.cpp")), True),
        ]
    ))
    
    # 14. full-text-index
    functionalities.append((
        "full-text-index",
        "failed",
        [
            ("FULLTEXT INDEX", lambda: grep_in_file(yacc_file, "FULLTEXT"), True),
            ("MATCH AGAINST", lambda: grep_in_file(yacc_file, "MATCH.*AGAINST"), True),
            ("TOKENIZE", lambda: grep_in_file(yacc_file, "TOKENIZE"), True),
        ]
    ))
    
    # 验证所有功能
    results = {}
    for name, status, checks in functionalities:
        is_implemented, actual_status = verify_functionality(name, status, checks)
        results[name] = {
            'marked': status,
            'actual': actual_status,
            'implemented': is_implemented
        }
    
    # 生成总结报告
    print("\n" + "="*70)
    print("验证总结报告")
    print("="*70)
    
    print("\n标记为'已过'但可能未实现:")
    for name, info in results.items():
        if info['marked'] == 'passed' and not info['implemented']:
            print(f"  ✗ {name}: 标记为已过但验证未通过")
    
    print("\n标记为'未过'但可能已实现:")
    for name, info in results.items():
        if info['marked'] == 'failed' and info['implemented']:
            print(f"  ✓ {name}: 标记为未过但验证通过")
    
    print("\n所有功能验证结果:")
    for name, info in sorted(results.items()):
        status_icon = "✓" if info['implemented'] else "✗"
        marked = "已过" if info['marked'] == 'passed' else "未过"
        actual = "已实现" if info['implemented'] else "未实现"
        print(f"  {status_icon} {name:20s} 标记:{marked:4s} 实际:{actual}")

if __name__ == "__main__":
    main()

