/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <utility>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

VectorOperationExpr *create_operation_expression(VectorOperationExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  VectorOperationExpr *expr = new VectorOperationExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundAggregateExpr *create_aggregate_expression(const char *aggregate_name,
                                           Expression *child,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundSysFuncExpr *create_sysfunc_expression(const char *func_name,
                                           Expression *child,
                                           Expression *second_child,
                                           Expression *third_child,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundSysFuncExpr *expr = nullptr;
  if (third_child) {
    expr = new UnboundSysFuncExpr(func_name, child, second_child, third_child);
  } else if (second_child) {
    expr = new UnboundSysFuncExpr(func_name, child, second_child);
  } else {
    expr = new UnboundSysFuncExpr(func_name, child);
  }
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}


#line 156 "yacc_sql.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_BY = 4,                         /* BY  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_DROP = 6,                       /* DROP  */
  YYSYMBOL_GROUP = 7,                      /* GROUP  */
  YYSYMBOL_ORDER = 8,                      /* ORDER  */
  YYSYMBOL_HAVING = 9,                     /* HAVING  */
  YYSYMBOL_ASC = 10,                       /* ASC  */
  YYSYMBOL_DESC = 11,                      /* DESC  */
  YYSYMBOL_TABLE = 12,                     /* TABLE  */
  YYSYMBOL_VIEW = 13,                      /* VIEW  */
  YYSYMBOL_TABLES = 14,                    /* TABLES  */
  YYSYMBOL_INDEX = 15,                     /* INDEX  */
  YYSYMBOL_CALC = 16,                      /* CALC  */
  YYSYMBOL_SELECT = 17,                    /* SELECT  */
  YYSYMBOL_SHOW = 18,                      /* SHOW  */
  YYSYMBOL_SYNC = 19,                      /* SYNC  */
  YYSYMBOL_WITH = 20,                      /* WITH  */
  YYSYMBOL_INSERT = 21,                    /* INSERT  */
  YYSYMBOL_DELETE = 22,                    /* DELETE  */
  YYSYMBOL_UPDATE = 23,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 24,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 25,                    /* RBRACE  */
  YYSYMBOL_LMBRACE = 26,                   /* LMBRACE  */
  YYSYMBOL_RMBRACE = 27,                   /* RMBRACE  */
  YYSYMBOL_COMMA = 28,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 29,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 30,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 31,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 32,                     /* INT_T  */
  YYSYMBOL_STRING_T = 33,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 34,                   /* FLOAT_T  */
  YYSYMBOL_DATE_T = 35,                    /* DATE_T  */
  YYSYMBOL_VECTOR_T = 36,                  /* VECTOR_T  */
  YYSYMBOL_TEXT_T = 37,                    /* TEXT_T  */
  YYSYMBOL_HELP = 38,                      /* HELP  */
  YYSYMBOL_EXIT = 39,                      /* EXIT  */
  YYSYMBOL_DOT = 40,                       /* DOT  */
  YYSYMBOL_INTO = 41,                      /* INTO  */
  YYSYMBOL_VALUES = 42,                    /* VALUES  */
  YYSYMBOL_FROM = 43,                      /* FROM  */
  YYSYMBOL_WHERE = 44,                     /* WHERE  */
  YYSYMBOL_INNER = 45,                     /* INNER  */
  YYSYMBOL_JOIN = 46,                      /* JOIN  */
  YYSYMBOL_AND = 47,                       /* AND  */
  YYSYMBOL_OR = 48,                        /* OR  */
  YYSYMBOL_SET = 49,                       /* SET  */
  YYSYMBOL_ON = 50,                        /* ON  */
  YYSYMBOL_AS = 51,                        /* AS  */
  YYSYMBOL_LOAD = 52,                      /* LOAD  */
  YYSYMBOL_DATA = 53,                      /* DATA  */
  YYSYMBOL_LIKE = 54,                      /* LIKE  */
  YYSYMBOL_NULL_T = 55,                    /* NULL_T  */
  YYSYMBOL_EXISTS = 56,                    /* EXISTS  */
  YYSYMBOL_IN = 57,                        /* IN  */
  YYSYMBOL_IS = 58,                        /* IS  */
  YYSYMBOL_NOT = 59,                       /* NOT  */
  YYSYMBOL_INFILE = 60,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 61,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 62,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 63,                    /* FORMAT  */
  YYSYMBOL_LIMIT = 64,                     /* LIMIT  */
  YYSYMBOL_EQ = 65,                        /* EQ  */
  YYSYMBOL_LT = 66,                        /* LT  */
  YYSYMBOL_GT = 67,                        /* GT  */
  YYSYMBOL_LE = 68,                        /* LE  */
  YYSYMBOL_GE = 69,                        /* GE  */
  YYSYMBOL_NE = 70,                        /* NE  */
  YYSYMBOL_UNIQUE = 71,                    /* UNIQUE  */
  YYSYMBOL_UNION = 72,                     /* UNION  */
  YYSYMBOL_ALL = 73,                       /* ALL  */
  YYSYMBOL_ALTER = 74,                     /* ALTER  */
  YYSYMBOL_ADD = 75,                       /* ADD  */
  YYSYMBOL_CHANGE = 76,                    /* CHANGE  */
  YYSYMBOL_RENAME = 77,                    /* RENAME  */
  YYSYMBOL_TO = 78,                        /* TO  */
  YYSYMBOL_COLUMN = 79,                    /* COLUMN  */
  YYSYMBOL_L2_DISTANCE = 80,               /* L2_DISTANCE  */
  YYSYMBOL_COSINE_DISTANCE = 81,           /* COSINE_DISTANCE  */
  YYSYMBOL_INNER_PRODUCT = 82,             /* INNER_PRODUCT  */
  YYSYMBOL_DISTANCE = 83,                  /* DISTANCE  */
  YYSYMBOL_LISTS = 84,                     /* LISTS  */
  YYSYMBOL_TYPE = 85,                      /* TYPE  */
  YYSYMBOL_PROBES = 86,                    /* PROBES  */
  YYSYMBOL_IVFFLAT = 87,                   /* IVFFLAT  */
  YYSYMBOL_MATCH = 88,                     /* MATCH  */
  YYSYMBOL_AGAINST = 89,                   /* AGAINST  */
  YYSYMBOL_FULLTEXT = 90,                  /* FULLTEXT  */
  YYSYMBOL_NUMBER = 91,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 92,                     /* FLOAT  */
  YYSYMBOL_ID_KEY = 93,                    /* ID_KEY  */
  YYSYMBOL_SSS = 94,                       /* SSS  */
  YYSYMBOL_DATE_VALUE = 95,                /* DATE_VALUE  */
  YYSYMBOL_96_ = 96,                       /* '+'  */
  YYSYMBOL_97_ = 97,                       /* '-'  */
  YYSYMBOL_98_ = 98,                       /* '*'  */
  YYSYMBOL_99_ = 99,                       /* '/'  */
  YYSYMBOL_UMINUS = 100,                   /* UMINUS  */
  YYSYMBOL_YYACCEPT = 101,                 /* $accept  */
  YYSYMBOL_commands = 102,                 /* commands  */
  YYSYMBOL_command_wrapper = 103,          /* command_wrapper  */
  YYSYMBOL_exit_stmt = 104,                /* exit_stmt  */
  YYSYMBOL_help_stmt = 105,                /* help_stmt  */
  YYSYMBOL_sync_stmt = 106,                /* sync_stmt  */
  YYSYMBOL_begin_stmt = 107,               /* begin_stmt  */
  YYSYMBOL_commit_stmt = 108,              /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 109,            /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 110,          /* drop_table_stmt  */
  YYSYMBOL_show_index_stmt = 111,          /* show_index_stmt  */
  YYSYMBOL_show_tables_stmt = 112,         /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 113,          /* desc_table_stmt  */
  YYSYMBOL_alter_table_stmt = 114,         /* alter_table_stmt  */
  YYSYMBOL_create_index_stmt = 115,        /* create_index_stmt  */
  YYSYMBOL_vector_operation = 116,         /* vector_operation  */
  YYSYMBOL_vector_index_type = 117,        /* vector_index_type  */
  YYSYMBOL_idx_col_list = 118,             /* idx_col_list  */
  YYSYMBOL_col_list = 119,                 /* col_list  */
  YYSYMBOL_unique_option = 120,            /* unique_option  */
  YYSYMBOL_drop_index_stmt = 121,          /* drop_index_stmt  */
  YYSYMBOL_create_view_stmt = 122,         /* create_view_stmt  */
  YYSYMBOL_create_table_stmt = 123,        /* create_table_stmt  */
  YYSYMBOL_as_option = 124,                /* as_option  */
  YYSYMBOL_attr_def_list = 125,            /* attr_def_list  */
  YYSYMBOL_attr_def = 126,                 /* attr_def  */
  YYSYMBOL_nullable = 127,                 /* nullable  */
  YYSYMBOL_number = 128,                   /* number  */
  YYSYMBOL_type = 129,                     /* type  */
  YYSYMBOL_date_type = 130,                /* date_type  */
  YYSYMBOL_insert_stmt = 131,              /* insert_stmt  */
  YYSYMBOL_values_list = 132,              /* values_list  */
  YYSYMBOL_value_list = 133,               /* value_list  */
  YYSYMBOL_value = 134,                    /* value  */
  YYSYMBOL_storage_format = 135,           /* storage_format  */
  YYSYMBOL_delete_stmt = 136,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 137,              /* update_stmt  */
  YYSYMBOL_key_values = 138,               /* key_values  */
  YYSYMBOL_assign_value = 139,             /* assign_value  */
  YYSYMBOL_select_stmt = 140,              /* select_stmt  */
  YYSYMBOL_select_unit = 141,              /* select_unit  */
  YYSYMBOL_limit = 142,                    /* limit  */
  YYSYMBOL_calc_stmt = 143,                /* calc_stmt  */
  YYSYMBOL_function_args = 144,            /* function_args  */
  YYSYMBOL_expression_list = 145,          /* expression_list  */
  YYSYMBOL_expression = 146,               /* expression  */
  YYSYMBOL_ID = 147,                       /* ID  */
  YYSYMBOL_rel_attr = 148,                 /* rel_attr  */
  YYSYMBOL_relation = 149,                 /* relation  */
  YYSYMBOL_rel_list = 150,                 /* rel_list  */
  YYSYMBOL_join_list = 151,                /* join_list  */
  YYSYMBOL_alias = 152,                    /* alias  */
  YYSYMBOL_on = 153,                       /* on  */
  YYSYMBOL_where = 154,                    /* where  */
  YYSYMBOL_condition_list = 155,           /* condition_list  */
  YYSYMBOL_condition = 156,                /* condition  */
  YYSYMBOL_comp_op = 157,                  /* comp_op  */
  YYSYMBOL_unary_op = 158,                 /* unary_op  */
  YYSYMBOL_group_by = 159,                 /* group_by  */
  YYSYMBOL_having_node = 160,              /* having_node  */
  YYSYMBOL_having_list = 161,              /* having_list  */
  YYSYMBOL_having_unit = 162,              /* having_unit  */
  YYSYMBOL_order_by = 163,                 /* order_by  */
  YYSYMBOL_order_by_list = 164,            /* order_by_list  */
  YYSYMBOL_order_by_unit = 165,            /* order_by_unit  */
  YYSYMBOL_load_data_stmt = 166,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 167,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 168,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 169             /* opt_semicolon  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  88
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   517

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  101
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  69
/* YYNRULES -- Number of rules.  */
#define YYNRULES  178
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  370

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   351


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    98,    96,     2,    97,     2,    99,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,   100
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   308,   308,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   344,   350,   355,   361,
     367,   373,   379,   385,   393,   399,   407,   417,   427,   439,
     449,   465,   484,   503,   531,   535,   539,   546,   550,   553,
     567,   570,   583,   586,   591,   602,   609,   628,   649,   667,
     679,   682,   687,   690,   703,   712,   721,   734,   735,   736,
     740,   743,   744,   745,   746,   747,   750,   754,   776,   782,
     796,   806,   823,   827,   831,   838,   845,   848,   855,   858,
     865,   877,   895,   902,   917,   921,   925,   936,   950,   998,
    1001,  1007,  1016,  1021,  1033,  1042,  1057,  1060,  1063,  1066,
    1069,  1123,  1126,  1132,  1135,  1141,  1144,  1156,  1159,  1170,
    1183,  1184,  1189,  1194,  1202,  1207,  1214,  1223,  1228,  1244,
    1277,  1280,  1307,  1310,  1313,  1320,  1323,  1332,  1335,  1340,
    1345,  1350,  1358,  1365,  1378,  1388,  1389,  1390,  1391,  1392,
    1393,  1394,  1395,  1396,  1397,  1398,  1399,  1403,  1404,  1410,
    1413,  1421,  1424,  1431,  1436,  1442,  1450,  1460,  1463,  1470,
    1476,  1489,  1495,  1501,  1510,  1523,  1531,  1541,  1542
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "BY",
  "CREATE", "DROP", "GROUP", "ORDER", "HAVING", "ASC", "DESC", "TABLE",
  "VIEW", "TABLES", "INDEX", "CALC", "SELECT", "SHOW", "SYNC", "WITH",
  "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "LMBRACE", "RMBRACE",
  "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T",
  "FLOAT_T", "DATE_T", "VECTOR_T", "TEXT_T", "HELP", "EXIT", "DOT", "INTO",
  "VALUES", "FROM", "WHERE", "INNER", "JOIN", "AND", "OR", "SET", "ON",
  "AS", "LOAD", "DATA", "LIKE", "NULL_T", "EXISTS", "IN", "IS", "NOT",
  "INFILE", "EXPLAIN", "STORAGE", "FORMAT", "LIMIT", "EQ", "LT", "GT",
  "LE", "GE", "NE", "UNIQUE", "UNION", "ALL", "ALTER", "ADD", "CHANGE",
  "RENAME", "TO", "COLUMN", "L2_DISTANCE", "COSINE_DISTANCE",
  "INNER_PRODUCT", "DISTANCE", "LISTS", "TYPE", "PROBES", "IVFFLAT",
  "MATCH", "AGAINST", "FULLTEXT", "NUMBER", "FLOAT", "ID_KEY", "SSS",
  "DATE_VALUE", "'+'", "'-'", "'*'", "'/'", "UMINUS", "$accept",
  "commands", "command_wrapper", "exit_stmt", "help_stmt", "sync_stmt",
  "begin_stmt", "commit_stmt", "rollback_stmt", "drop_table_stmt",
  "show_index_stmt", "show_tables_stmt", "desc_table_stmt",
  "alter_table_stmt", "create_index_stmt", "vector_operation",
  "vector_index_type", "idx_col_list", "col_list", "unique_option",
  "drop_index_stmt", "create_view_stmt", "create_table_stmt", "as_option",
  "attr_def_list", "attr_def", "nullable", "number", "type", "date_type",
  "insert_stmt", "values_list", "value_list", "value", "storage_format",
  "delete_stmt", "update_stmt", "key_values", "assign_value",
  "select_stmt", "select_unit", "limit", "calc_stmt", "function_args",
  "expression_list", "expression", "ID", "rel_attr", "relation",
  "rel_list", "join_list", "alias", "on", "where", "condition_list",
  "condition", "comp_op", "unary_op", "group_by", "having_node",
  "having_list", "having_unit", "order_by", "order_by_list",
  "order_by_unit", "load_data_stmt", "explain_stmt", "set_variable_stmt",
  "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-300)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-144)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     443,     4,    15,    99,   349,   349,    37,  -300,   -26,    -6,
      99,  -300,  -300,  -300,  -300,  -300,    99,   -14,   443,    29,
      47,    83,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,     1,  -300,  -300,  -300,  -300,    99,    99,    87,  -300,
     100,   105,    99,    99,  -300,  -300,  -300,  -300,  -300,   221,
     349,  -300,  -300,  -300,  -300,    72,  -300,  -300,  -300,  -300,
     349,  -300,   119,  -300,  -300,   155,     2,  -300,    62,  -300,
     102,    99,    99,   107,    93,   115,  -300,    99,  -300,  -300,
    -300,   -13,    73,    90,    99,    99,    99,  -300,   109,   156,
     161,   160,    -9,    99,  -300,   349,    99,   349,   349,   349,
     349,  -300,   163,   315,   -39,    99,    99,   169,   150,    99,
     121,   113,    42,   186,  -300,    99,  -300,   186,    99,   186,
     159,   164,   172,    99,  -300,  -300,  -300,   349,   171,   198,
      39,  -300,   -22,   -22,  -300,  -300,   349,  -300,   199,    71,
    -300,  -300,  -300,    70,   150,  -300,    99,   183,   269,  -300,
     150,   162,  -300,   189,   152,   153,   154,    48,  -300,   207,
     249,  -300,   208,  -300,    99,    99,    99,  -300,  -300,   148,
     349,  -300,  -300,   349,   196,   235,   208,   220,  -300,   193,
     226,   320,  -300,    92,   349,  -300,   349,   243,    99,    99,
      99,    99,    99,    99,   231,  -300,  -300,  -300,  -300,  -300,
    -300,     7,   -23,    99,   232,   237,   238,   241,   246,   -16,
    -300,   225,   244,   271,   278,   253,   349,  -300,  -300,    99,
    -300,  -300,   229,   -19,  -300,  -300,  -300,  -300,  -300,  -300,
     349,   269,   269,    81,   261,    81,    99,  -300,  -300,    99,
    -300,   212,   207,    12,   200,  -300,   242,  -300,  -300,   208,
     247,    99,    99,    99,   205,  -300,    99,    99,   349,   349,
     284,  -300,   275,   279,  -300,  -300,  -300,    81,  -300,  -300,
      99,  -300,  -300,    99,  -300,   245,   186,  -300,  -300,   282,
    -300,  -300,   186,   208,   208,   208,   285,    70,  -300,  -300,
     320,  -300,   101,   307,   256,   289,   240,  -300,  -300,   262,
    -300,   -23,  -300,   296,   301,   305,  -300,   281,   349,   349,
     349,   349,   200,  -300,   220,   308,    99,  -300,   313,  -300,
    -300,   269,   196,    81,  -300,  -300,    11,  -300,   306,  -300,
    -300,   248,  -300,   311,  -300,  -300,  -300,  -300,   349,   312,
     251,  -300,   157,   273,   257,  -300,   317,   260,   283,   138,
     318,   263,   287,   264,   328,   272,   294,   200,   340,  -300
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    52,     0,     0,     0,     0,     0,    28,     0,     0,
       0,    29,    30,    31,    27,    26,     0,     0,     0,     0,
       0,   177,    25,    24,    17,    18,    19,    20,     9,    10,
      12,    13,    16,    14,    15,    11,     8,     5,     7,     6,
       4,    95,     3,    21,    22,    23,     0,     0,     0,    53,
       0,     0,     0,     0,   121,   122,   123,   120,    35,     0,
       0,    86,    44,    45,    46,     0,    82,    83,    84,    85,
       0,   115,     0,   112,   101,   132,   124,   114,     0,    34,
       0,     0,     0,     0,     0,     0,   175,     0,     1,   178,
       2,     0,    60,     0,     0,     0,     0,    32,     0,     0,
       0,     0,    80,     0,   113,     0,     0,     0,     0,     0,
       0,   133,   104,     0,     0,     0,     0,    50,   137,     0,
       0,     0,     0,     0,    96,     0,    61,     0,     0,     0,
       0,     0,     0,     0,   117,   118,    87,     0,   124,     0,
       0,   134,   106,   107,   108,   109,     0,   116,     0,   102,
     126,   125,   127,   132,   137,    33,     0,     0,     0,    90,
     137,     0,   176,     0,     0,     0,     0,     0,    97,    62,
       0,    59,    48,    55,     0,     0,     0,    54,    81,     0,
       0,   105,   110,     0,   130,   159,    48,     0,   157,     0,
       0,     0,   138,   139,     0,    91,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    71,    72,    73,    76,    74,
      75,    67,    67,     0,     0,     0,     0,     0,     0,     0,
     103,     0,   128,     0,   161,     0,     0,    77,   158,     0,
     145,   153,   155,     0,   147,   148,   149,   150,   151,   152,
       0,     0,     0,   144,    92,    94,     0,    37,    36,     0,
      39,     0,    62,    88,     0,    68,     0,    65,    66,    48,
       0,     0,     0,     0,     0,   111,     0,     0,     0,     0,
     167,    51,     0,     0,   156,   146,   154,   142,   140,   141,
       0,   174,    40,     0,    63,     0,     0,    57,    70,     0,
      69,    49,     0,    48,    48,    48,     0,   132,   129,   160,
       0,   162,   163,     0,    99,    78,     0,    93,    38,     0,
      58,    67,    56,     0,     0,     0,   119,   135,     0,     0,
       0,     0,     0,    98,     0,     0,     0,    64,     0,    42,
      41,     0,   130,   166,   165,   164,   171,   168,   169,   100,
      79,     0,    89,     0,   136,   131,   172,   173,     0,     0,
       0,   170,   119,     0,     0,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    43
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -300,  -300,   351,  -300,  -300,  -300,  -300,  -300,  -300,  -300,
    -300,  -300,  -300,  -300,  -300,    13,  -300,  -161,  -300,  -300,
    -300,  -300,  -300,   118,   124,  -179,  -207,  -299,  -300,  -300,
    -300,    56,  -131,   274,  -300,  -300,  -300,   103,  -300,   -58,
    -300,  -300,  -300,   201,     5,   -52,    -3,  -100,   116,   114,
      59,  -151,  -300,  -126,  -230,  -300,    98,  -300,  -300,  -300,
    -158,  -300,  -300,    44,  -300,  -300,  -300,  -300,  -300
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    72,   356,   214,   157,    51,
      34,    35,    36,   127,   204,   169,   257,   289,   211,   212,
      37,   227,   101,    73,   287,    38,    39,   160,   244,    40,
      41,   323,    42,   148,    74,    75,    76,    77,   153,   154,
     222,   112,   332,   159,   192,   193,   240,   194,   224,   270,
     301,   302,   304,   337,   338,    43,    44,    45,    90
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,    99,   184,   139,     5,   258,   178,    83,   102,   265,
      78,   278,   279,    84,    54,    81,    46,    47,   104,   137,
     248,   346,   347,   339,   252,   225,   113,    52,   185,   -60,
      53,   254,   255,   124,   195,   275,   256,    82,   276,    85,
      48,    87,   114,    92,    93,    55,    56,    88,   164,    97,
      98,    79,    80,   140,    57,   142,   143,   144,   145,   150,
     123,   149,   255,   126,   100,   168,   256,   180,   368,   171,
     282,   173,   111,    91,   285,    49,   109,   110,   117,   118,
     107,   108,   109,   110,   122,   102,    89,   107,   108,   109,
     110,   130,   131,   132,    50,   272,   103,   125,   291,   183,
     138,   344,    94,   141,   327,   115,   191,   107,   108,   109,
     110,   151,   152,   155,   128,    95,   161,   165,   166,   167,
      96,   106,   170,    54,   126,   172,   201,   202,   219,   273,
     177,   149,   313,   314,   315,   107,   108,   109,   110,   241,
     242,   129,   243,   105,   245,   116,   317,    60,   319,   320,
     111,   181,    54,   186,    55,    56,   119,  -143,   120,   133,
    -143,   334,   335,    57,  -143,  -143,  -143,   107,   108,   109,
     110,   215,   216,   217,   102,   121,    61,   107,   108,   109,
     110,   134,  -143,    55,    56,  -143,   135,   136,   277,   191,
     191,   146,    57,   156,   158,   247,   170,   249,   250,   251,
     170,  -143,  -143,     5,  -143,  -143,   106,   163,    54,   174,
     259,   114,    66,    67,   175,    68,    69,   300,    62,    63,
      64,  -143,   176,   179,   182,   187,   138,   196,   310,  -143,
     197,   198,   199,   200,   312,   203,   213,   218,     5,    55,
      56,   221,   223,   281,   226,    59,   170,    60,    57,   228,
     229,   107,   108,   109,   110,   246,   253,   260,   293,   294,
     295,   261,   262,   152,   152,   263,   333,   300,   300,   336,
     264,   266,   267,   299,    54,   268,    61,   161,   271,   191,
     308,   205,   206,   207,   208,   209,   210,   269,   274,   280,
     283,   288,   303,    59,   111,    60,   336,   290,   292,   296,
     305,    62,    63,    64,   306,    55,    56,   311,   309,    65,
     316,   321,    66,    67,    57,    68,    69,   324,    70,    71,
     322,   328,    54,   342,    61,   188,   329,   326,   189,   325,
     330,   331,   341,   343,   348,   350,   353,   352,   354,    59,
     147,    60,   349,   358,   355,   357,   361,   362,   359,    62,
      63,    64,   363,    55,    56,   364,   365,   190,   366,   367,
      66,    67,    57,    68,    69,   369,    70,    71,    54,    86,
      61,   286,   360,    59,   230,    60,   284,   231,   232,   233,
     340,   298,   297,   307,   220,   234,   235,   236,   237,   238,
     239,   345,   351,     0,   162,    62,    63,    64,   318,    55,
      56,     0,    54,    65,    61,     0,    66,    67,    57,    68,
      69,     0,    70,    71,     0,     0,   107,   108,   109,   110,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    62,
      63,    64,     0,    55,    56,     0,     0,    65,     0,     0,
      66,    67,    57,    68,    69,     0,    70,    71,     1,     2,
       0,     0,     0,     0,     3,     0,     0,     0,     0,     4,
       5,     6,     7,     0,     8,     9,    10,     0,     0,     0,
       0,     0,    11,    12,    13,     0,     0,     0,     0,     0,
       0,    14,    15,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    16,     0,     0,    17,     0,     0,     0,     0,
       0,     0,     0,     0,    18,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    19
};

static const yytype_int16 yycheck[] =
{
       3,    59,   153,   103,    17,   212,   137,    10,    60,    25,
       5,   241,   242,    16,    53,    41,    12,    13,    70,    28,
     199,    10,    11,   322,   203,   186,    24,    12,   154,    17,
      15,    24,    55,    91,   160,    54,    59,    43,    57,    53,
      36,    12,    40,    46,    47,    84,    85,     0,     6,    52,
      53,    14,    15,   105,    93,   107,   108,   109,   110,    98,
      73,   113,    55,    51,    59,   123,    59,    28,   367,   127,
     249,   129,    75,    72,    62,    71,    98,    99,    81,    82,
      96,    97,    98,    99,    87,   137,     3,    96,    97,    98,
      99,    94,    95,    96,    90,   226,    24,    24,   259,    28,
     103,   331,    15,   106,   311,    43,   158,    96,    97,    98,
      99,   114,   115,   116,    24,    15,   119,    75,    76,    77,
      15,    51,   125,    53,    51,   128,    78,    79,   180,   229,
     133,   183,   293,   294,   295,    96,    97,    98,    99,    47,
      48,    51,   194,    24,   196,    43,   297,    26,    47,    48,
     153,   146,    53,   156,    84,    85,    49,     0,    65,    50,
       3,   319,   320,    93,     7,     8,     9,    96,    97,    98,
      99,   174,   175,   176,   226,    60,    55,    96,    97,    98,
      99,    25,    25,    84,    85,    28,    25,    27,   240,   241,
     242,    28,    93,    24,    44,   198,   199,   200,   201,   202,
     203,    44,    45,    17,    47,    48,    51,    94,    53,    50,
     213,    40,    91,    92,    50,    94,    95,   269,    80,    81,
      82,    64,    50,    25,    25,    42,   229,    65,   286,    72,
      41,    79,    79,    79,   292,    28,    28,    89,    17,    84,
      85,    45,     7,   246,    24,    24,   249,    26,    93,    56,
      24,    96,    97,    98,    99,    12,    25,    25,   261,   262,
     263,    24,    24,   266,   267,    24,   318,   319,   320,   321,
      24,    46,    28,   268,    53,     4,    55,   280,    25,   331,
     283,    32,    33,    34,    35,    36,    37,     9,    59,    28,
      78,    91,     8,    24,   297,    26,   348,    55,    51,    94,
      25,    80,    81,    82,    25,    84,    85,    25,    63,    88,
      25,     4,    91,    92,    93,    94,    95,    28,    97,    98,
      64,    25,    53,   326,    55,    56,    25,    65,    59,    89,
      25,    50,    24,    20,    28,    24,    85,    25,    65,    24,
      25,    26,    94,    83,    87,    28,    28,    84,    65,    80,
      81,    82,    65,    84,    85,    91,    28,    88,    86,    65,
      91,    92,    93,    94,    95,    25,    97,    98,    53,    18,
      55,   253,   359,    24,    54,    26,   252,    57,    58,    59,
     324,   267,   266,   280,   183,    65,    66,    67,    68,    69,
      70,   332,   348,    -1,   120,    80,    81,    82,   300,    84,
      85,    -1,    53,    88,    55,    -1,    91,    92,    93,    94,
      95,    -1,    97,    98,    -1,    -1,    96,    97,    98,    99,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    80,
      81,    82,    -1,    84,    85,    -1,    -1,    88,    -1,    -1,
      91,    92,    93,    94,    95,    -1,    97,    98,     5,     6,
      -1,    -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    16,
      17,    18,    19,    -1,    21,    22,    23,    -1,    -1,    -1,
      -1,    -1,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,
      -1,    38,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    49,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    61,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    74
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     6,    11,    16,    17,    18,    19,    21,    22,
      23,    29,    30,    31,    38,    39,    49,    52,    61,    74,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   121,   122,   123,   131,   136,   137,
     140,   141,   143,   166,   167,   168,    12,    13,    36,    71,
      90,   120,    12,    15,    53,    84,    85,    93,   147,    24,
      26,    55,    80,    81,    82,    88,    91,    92,    94,    95,
      97,    98,   116,   134,   145,   146,   147,   148,   145,    14,
      15,    41,    43,   147,   147,    53,   103,    12,     0,     3,
     169,    72,   147,   147,    15,    15,    15,   147,   147,   140,
     145,   133,   146,    24,   146,    24,    51,    96,    97,    98,
      99,   147,   152,    24,    40,    43,    43,   147,   147,    49,
      65,    60,   147,    73,   140,    24,    51,   124,    24,    51,
     147,   147,   147,    50,    25,    25,    27,    28,   147,   148,
     146,   147,   146,   146,   146,   146,    28,    25,   144,   146,
      98,   147,   147,   149,   150,   147,    24,   119,    44,   154,
     138,   147,   134,    94,     6,    75,    76,    77,   140,   126,
     147,   140,   147,   140,    50,    50,    50,   147,   133,    25,
      28,   145,    25,    28,   152,   154,   147,    42,    56,    59,
      88,   146,   155,   156,   158,   154,    65,    41,    79,    79,
      79,    78,    79,    28,   125,    32,    33,    34,    35,    36,
      37,   129,   130,    28,   118,   147,   147,   147,    89,   146,
     144,    45,   151,     7,   159,   118,    24,   132,    56,    24,
      54,    57,    58,    59,    65,    66,    67,    68,    69,    70,
     157,    47,    48,   146,   139,   146,    12,   147,   126,   147,
     147,   147,   126,    25,    24,    55,    59,   127,   127,   147,
      25,    24,    24,    24,    24,    25,    46,    28,     4,     9,
     160,    25,   133,   148,    59,    54,    57,   146,   155,   155,
      28,   147,   126,    78,   125,    62,   124,   135,    91,   128,
      55,   118,    51,   147,   147,   147,    94,   149,   150,   145,
     146,   161,   162,     8,   163,    25,    25,   138,   147,    63,
     140,    25,   140,   118,   118,   118,    25,   152,   157,    47,
      48,     4,    64,   142,    28,    89,    65,   127,    25,    25,
      25,    50,   153,   146,   161,   161,   146,   164,   165,   128,
     132,    24,   147,    20,   155,   151,    10,    11,    28,    94,
      24,   164,    25,    85,    65,    87,   117,    28,    83,    65,
     116,    28,    84,    65,    91,    28,    86,    65,   128,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   101,   102,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   114,   114,   114,
     114,   115,   115,   115,   116,   116,   116,   117,   118,   118,
     119,   119,   120,   120,   121,   122,   122,   123,   123,   123,
     124,   124,   125,   125,   126,   126,   126,   127,   127,   127,
     128,   129,   129,   129,   129,   129,   130,   131,   132,   132,
     133,   133,   134,   134,   134,   134,   134,   134,   135,   135,
     136,   137,   138,   138,   139,   140,   140,   140,   141,   142,
     142,   143,   144,   144,   145,   145,   146,   146,   146,   146,
     146,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     147,   147,   147,   147,   148,   148,   148,   149,   150,   150,
     151,   151,   152,   152,   152,   153,   153,   154,   154,   155,
     155,   155,   156,   156,   156,   157,   157,   157,   157,   157,
     157,   157,   157,   157,   157,   157,   157,   158,   158,   159,
     159,   160,   160,   161,   161,   161,   162,   163,   163,   164,
     164,   165,   165,   165,   166,   167,   168,   169,   169
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     2,     2,     6,     6,     8,     6,
       7,    10,    10,    28,     1,     1,     1,     1,     0,     3,
       0,     4,     0,     1,     5,     5,     9,     8,     9,     5,
       0,     1,     0,     3,     6,     3,     3,     0,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     6,     3,     5,
       1,     3,     1,     1,     1,     1,     1,     3,     0,     4,
       4,     5,     3,     5,     1,     1,     3,     4,     9,     0,
       2,     2,     1,     3,     2,     4,     3,     3,     3,     3,
       4,     6,     1,     2,     1,     1,     3,     3,     3,     8,
       1,     1,     1,     1,     1,     3,     3,     1,     3,     5,
       0,     6,     0,     1,     2,     0,     2,     0,     2,     1,
       3,     3,     3,     8,     2,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     2,     0,
       3,     0,     2,     1,     3,     3,     3,     0,     3,     1,
       3,     1,     2,     2,     7,     2,     4,     0,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_ID_KEY: /* ID_KEY  */
#line 98 "yacc_sql.y"
            { free(((*yyvaluep).string)); }
#line 1714 "yacc_sql.cpp"
        break;

    case YYSYMBOL_SSS: /* SSS  */
#line 98 "yacc_sql.y"
            { free(((*yyvaluep).string)); }
#line 1720 "yacc_sql.cpp"
        break;

    case YYSYMBOL_DATE_VALUE: /* DATE_VALUE  */
#line 98 "yacc_sql.y"
            { free(((*yyvaluep).string)); }
#line 1726 "yacc_sql.cpp"
        break;

    case YYSYMBOL_storage_format: /* storage_format  */
#line 226 "yacc_sql.y"
            { free(((*yyvaluep).string)); }
#line 1732 "yacc_sql.cpp"
        break;

    case YYSYMBOL_ID: /* ID  */
#line 226 "yacc_sql.y"
            { free(((*yyvaluep).string)); }
#line 1738 "yacc_sql.cpp"
        break;

    case YYSYMBOL_relation: /* relation  */
#line 226 "yacc_sql.y"
            { free(((*yyvaluep).string)); }
#line 1744 "yacc_sql.cpp"
        break;

    case YYSYMBOL_alias: /* alias  */
#line 226 "yacc_sql.y"
            { free(((*yyvaluep).string)); }
#line 1750 "yacc_sql.cpp"
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, &yylloc, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 309 "yacc_sql.y"
  {
    if((yyvsp[-1].sql_node) != nullptr){
      std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
      sql_result->add_sql_node(std::move(sql_node));
    }
  }
#line 2061 "yacc_sql.cpp"
    break;

  case 26: /* exit_stmt: EXIT  */
#line 344 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 2070 "yacc_sql.cpp"
    break;

  case 27: /* help_stmt: HELP  */
#line 350 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 2078 "yacc_sql.cpp"
    break;

  case 28: /* sync_stmt: SYNC  */
#line 355 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 2086 "yacc_sql.cpp"
    break;

  case 29: /* begin_stmt: TRX_BEGIN  */
#line 361 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 2094 "yacc_sql.cpp"
    break;

  case 30: /* commit_stmt: TRX_COMMIT  */
#line 367 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 2102 "yacc_sql.cpp"
    break;

  case 31: /* rollback_stmt: TRX_ROLLBACK  */
#line 373 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 2110 "yacc_sql.cpp"
    break;

  case 32: /* drop_table_stmt: DROP TABLE ID  */
#line 379 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2120 "yacc_sql.cpp"
    break;

  case 33: /* show_index_stmt: SHOW INDEX FROM ID  */
#line 386 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_INDEX);
      (yyval.sql_node)->show_index.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2130 "yacc_sql.cpp"
    break;

  case 34: /* show_tables_stmt: SHOW TABLES  */
#line 393 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 2138 "yacc_sql.cpp"
    break;

  case 35: /* desc_table_stmt: DESC ID  */
#line 399 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2148 "yacc_sql.cpp"
    break;

  case 36: /* alter_table_stmt: ALTER TABLE ID ADD COLUMN attr_def  */
#line 408 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::ADD_COLUMN;
      alter_table.attr_info = *(yyvsp[0].attr_info);
      free((yyvsp[-3].string));
      delete (yyvsp[0].attr_info);
    }
#line 2162 "yacc_sql.cpp"
    break;

  case 37: /* alter_table_stmt: ALTER TABLE ID DROP COLUMN ID  */
#line 418 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::DROP_COLUMN;
      alter_table.old_name = (yyvsp[0].string);
      free((yyvsp[-3].string));
      free((yyvsp[0].string));
    }
#line 2176 "yacc_sql.cpp"
    break;

  case 38: /* alter_table_stmt: ALTER TABLE ID RENAME COLUMN ID TO ID  */
#line 428 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-5].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::RENAME_COLUMN;
      alter_table.old_name = (yyvsp[-2].string);
      alter_table.new_name = (yyvsp[0].string);
      free((yyvsp[-5].string));
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2192 "yacc_sql.cpp"
    break;

  case 39: /* alter_table_stmt: ALTER TABLE ID RENAME TO ID  */
#line 440 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::RENAME_TABLE;
      alter_table.new_name = (yyvsp[0].string);
      free((yyvsp[-3].string));
      free((yyvsp[0].string));
    }
#line 2206 "yacc_sql.cpp"
    break;

  case 40: /* alter_table_stmt: ALTER TABLE ID CHANGE COLUMN ID attr_def  */
#line 450 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-4].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::CHANGE_COLUMN;
      alter_table.old_name = (yyvsp[-1].string);
      alter_table.attr_info = *(yyvsp[0].attr_info);
      alter_table.new_name = alter_table.attr_info.name;
      free((yyvsp[-4].string));
      free((yyvsp[-1].string));
      delete (yyvsp[0].attr_info);
    }
#line 2223 "yacc_sql.cpp"
    break;

  case 41: /* create_index_stmt: CREATE unique_option INDEX ID ON ID LBRACE ID idx_col_list RBRACE  */
#line 466 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.unique = (yyvsp[-8].boolean);
      create_index.fulltext = false;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);
      std::vector<std::string> *idx_cols = (yyvsp[-1].relation_list);
      if (nullptr != idx_cols) {
        create_index.attr_names.swap(*idx_cols);
        delete (yyvsp[-1].relation_list);
      }
      create_index.attr_names.emplace_back((yyvsp[-2].string));
      std::reverse(create_index.attr_names.begin(), create_index.attr_names.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2246 "yacc_sql.cpp"
    break;

  case 42: /* create_index_stmt: CREATE FULLTEXT INDEX ID ON ID LBRACE ID idx_col_list RBRACE  */
#line 485 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.unique = false;
      create_index.fulltext = true;
      create_index.index_name = (yyvsp[-6].string);
      create_index.relation_name = (yyvsp[-4].string);
      std::vector<std::string> *idx_cols = (yyvsp[-1].relation_list);
      if (nullptr != idx_cols) {
        create_index.attr_names.swap(*idx_cols);
        delete (yyvsp[-1].relation_list);
      }
      create_index.attr_names.emplace_back((yyvsp[-2].string));
      std::reverse(create_index.attr_names.begin(), create_index.attr_names.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
      free((yyvsp[-2].string));
    }
#line 2269 "yacc_sql.cpp"
    break;

  case 43: /* create_index_stmt: CREATE VECTOR_T INDEX ID ON ID LBRACE ID idx_col_list RBRACE WITH LBRACE TYPE EQ vector_index_type COMMA DISTANCE EQ vector_operation COMMA LISTS EQ NUMBER COMMA PROBES EQ number RBRACE  */
#line 506 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.fulltext = false;
      create_index.index_name = (yyvsp[-24].string);
      create_index.relation_name = (yyvsp[-22].string);
      std::vector<std::string> *idx_cols = (yyvsp[-19].relation_list);
      if (nullptr != idx_cols) {
        create_index.attr_names.swap(*idx_cols);
        delete (yyvsp[-19].relation_list);
      }
      create_index.attr_names.emplace_back((yyvsp[-20].string));
      std::reverse(create_index.attr_names.begin(), create_index.attr_names.end());
      free((yyvsp[-24].string));
      free((yyvsp[-22].string));
      free((yyvsp[-20].string));

      create_index.vector_index.distance = (yyvsp[-9].number);    
      create_index.vector_index.type = (yyvsp[-13].vector_index_type);
      create_index.vector_index.lists = (yyvsp[-5].number);
      create_index.vector_index.probes = (yyvsp[-1].number); 
    }
#line 2296 "yacc_sql.cpp"
    break;

  case 44: /* vector_operation: L2_DISTANCE  */
#line 532 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::L2_DISTANCE;
    }
#line 2304 "yacc_sql.cpp"
    break;

  case 45: /* vector_operation: COSINE_DISTANCE  */
#line 536 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::COSINE_DISTANCE;
    }
#line 2312 "yacc_sql.cpp"
    break;

  case 46: /* vector_operation: INNER_PRODUCT  */
#line 540 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::INNER_PRODUCT;
    }
#line 2320 "yacc_sql.cpp"
    break;

  case 47: /* vector_index_type: IVFFLAT  */
#line 546 "yacc_sql.y"
            { (yyval.vector_index_type) = VectorIndexType::IVFFLAT; }
#line 2326 "yacc_sql.cpp"
    break;

  case 48: /* idx_col_list: %empty  */
#line 550 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2334 "yacc_sql.cpp"
    break;

  case 49: /* idx_col_list: COMMA ID idx_col_list  */
#line 554 "yacc_sql.y"
    {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }
      (yyval.relation_list)->emplace_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 2348 "yacc_sql.cpp"
    break;

  case 50: /* col_list: %empty  */
#line 567 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2356 "yacc_sql.cpp"
    break;

  case 51: /* col_list: LBRACE ID idx_col_list RBRACE  */
#line 571 "yacc_sql.y"
    {
      if ((yyvsp[-1].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[-1].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }
      (yyval.relation_list)->emplace_back((yyvsp[-2].string));
      free((yyvsp[-2].string));      
    }
#line 2370 "yacc_sql.cpp"
    break;

  case 52: /* unique_option: %empty  */
#line 583 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2378 "yacc_sql.cpp"
    break;

  case 53: /* unique_option: UNIQUE  */
#line 587 "yacc_sql.y"
    {
      (yyval.boolean) = true;
    }
#line 2386 "yacc_sql.cpp"
    break;

  case 54: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 592 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2398 "yacc_sql.cpp"
    break;

  case 55: /* create_view_stmt: CREATE VIEW ID AS select_stmt  */
#line 603 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_VIEW;
      (yyval.sql_node)->create_view.view_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2409 "yacc_sql.cpp"
    break;

  case 56: /* create_view_stmt: CREATE VIEW ID LBRACE ID idx_col_list RBRACE AS select_stmt  */
#line 610 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_VIEW;
      (yyval.sql_node)->create_view.view_name = (yyvsp[-6].string);

      std::vector<std::string> &col_names = (yyval.sql_node)->create_view.col_names;
      if (nullptr != (yyvsp[-3].relation_list)) {
        col_names.swap(*(yyvsp[-3].relation_list));
        delete (yyvsp[-3].relation_list);
      }
      col_names.emplace_back((yyvsp[-4].string));
      std::reverse(col_names.begin(), col_names.end());
      free((yyvsp[-6].string));
      free((yyvsp[-4].string));
    }
#line 2429 "yacc_sql.cpp"
    break;

  case 57: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 629 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-2].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(move(*(yyvsp[-3].attr_info)));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);
      if ((yyvsp[0].string) != nullptr) {
        create_table.storage_format = (yyvsp[0].string);
        free((yyvsp[0].string));
      }
    }
#line 2454 "yacc_sql.cpp"
    break;

  case 58: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE as_option select_stmt  */
#line 650 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-6].string);
      free((yyvsp[-6].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-3].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(move(*(yyvsp[-4].attr_info)));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-4].attr_info);
    }
#line 2476 "yacc_sql.cpp"
    break;

  case 59: /* create_table_stmt: CREATE TABLE ID as_option select_stmt  */
#line 668 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2488 "yacc_sql.cpp"
    break;

  case 60: /* as_option: %empty  */
#line 679 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2496 "yacc_sql.cpp"
    break;

  case 61: /* as_option: AS  */
#line 682 "yacc_sql.y"
         { (yyval.boolean) = true; }
#line 2502 "yacc_sql.cpp"
    break;

  case 62: /* attr_def_list: %empty  */
#line 687 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2510 "yacc_sql.cpp"
    break;

  case 63: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 691 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(move(*(yyvsp[-1].attr_info)));
      delete (yyvsp[-1].attr_info);
    }
#line 2524 "yacc_sql.cpp"
    break;

  case 64: /* attr_def: ID type LBRACE number RBRACE nullable  */
#line 704 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-5].string));
    }
#line 2537 "yacc_sql.cpp"
    break;

  case 65: /* attr_def: ID type nullable  */
#line 713 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2550 "yacc_sql.cpp"
    break;

  case 66: /* attr_def: ID date_type nullable  */
#line 722 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 10;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2563 "yacc_sql.cpp"
    break;

  case 67: /* nullable: %empty  */
#line 734 "yacc_sql.y"
    { (yyval.boolean) = true; }
#line 2569 "yacc_sql.cpp"
    break;

  case 68: /* nullable: NULL_T  */
#line 735 "yacc_sql.y"
             {(yyval.boolean) = true;}
#line 2575 "yacc_sql.cpp"
    break;

  case 69: /* nullable: NOT NULL_T  */
#line 736 "yacc_sql.y"
                 {(yyval.boolean) = false;}
#line 2581 "yacc_sql.cpp"
    break;

  case 70: /* number: NUMBER  */
#line 740 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2587 "yacc_sql.cpp"
    break;

  case 71: /* type: INT_T  */
#line 743 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2593 "yacc_sql.cpp"
    break;

  case 72: /* type: STRING_T  */
#line 744 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2599 "yacc_sql.cpp"
    break;

  case 73: /* type: FLOAT_T  */
#line 745 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2605 "yacc_sql.cpp"
    break;

  case 74: /* type: VECTOR_T  */
#line 746 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2611 "yacc_sql.cpp"
    break;

  case 75: /* type: TEXT_T  */
#line 747 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::TEXTS);}
#line 2617 "yacc_sql.cpp"
    break;

  case 76: /* date_type: DATE_T  */
#line 750 "yacc_sql.y"
             {(yyval.number) = static_cast<int>(AttrType::DATES);}
#line 2623 "yacc_sql.cpp"
    break;

  case 77: /* insert_stmt: INSERT INTO ID col_list VALUES values_list  */
#line 755 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-3].string);

      if ((yyvsp[0].values_list) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[0].values_list));
        std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
        delete (yyvsp[0].values_list);
      }

      if (nullptr != (yyvsp[-2].relation_list)) {
        (yyval.sql_node)->insertion.attrs_name.swap(*(yyvsp[-2].relation_list));
        std::reverse((yyval.sql_node)->insertion.attrs_name.begin(), (yyval.sql_node)->insertion.attrs_name.end());
        delete (yyvsp[-2].relation_list);
      }

      free((yyvsp[-3].string));
    }
#line 2646 "yacc_sql.cpp"
    break;

  case 78: /* values_list: LBRACE value_list RBRACE  */
#line 777 "yacc_sql.y"
    {
      (yyval.values_list) = new std::vector<std::vector<Value>>;
      (yyval.values_list)->emplace_back(move(*(yyvsp[-1].value_list)));
      delete (yyvsp[-1].value_list);
    }
#line 2656 "yacc_sql.cpp"
    break;

  case 79: /* values_list: LBRACE value_list RBRACE COMMA values_list  */
#line 783 "yacc_sql.y"
    {
      if ((yyvsp[0].values_list) != nullptr) {
        (yyval.values_list) = (yyvsp[0].values_list);
      } else {
        (yyval.values_list) = new std::vector<std::vector<Value>>;
      }

      (yyval.values_list)->emplace_back(move(*(yyvsp[-3].value_list)));
      delete (yyvsp[-3].value_list);
    }
#line 2671 "yacc_sql.cpp"
    break;

  case 80: /* value_list: expression  */
#line 796 "yacc_sql.y"
               {
      (yyval.value_list) = new std::vector<Value>;
      Value temp;
      if(OB_FAIL((yyvsp[0].expression)->try_get_value(temp))){
        yyerror(&(yyloc), sql_string, sql_result, scanner, "error");
        YYERROR;
      }
      (yyval.value_list)->emplace_back(temp);
      delete (yyvsp[0].expression);
    }
#line 2686 "yacc_sql.cpp"
    break;

  case 81: /* value_list: expression COMMA value_list  */
#line 806 "yacc_sql.y"
                                   {
      Value temp;
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      if(OB_FAIL((yyvsp[-2].expression)->try_get_value(temp))){
        yyerror(&(yyloc), sql_string, sql_result, scanner, "error");
        YYERROR;
      }

      (yyval.value_list)->emplace((yyval.value_list)->begin(), move(temp));
      delete (yyvsp[-2].expression);
    }
#line 2706 "yacc_sql.cpp"
    break;

  case 82: /* value: NUMBER  */
#line 823 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2715 "yacc_sql.cpp"
    break;

  case 83: /* value: FLOAT  */
#line 827 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2724 "yacc_sql.cpp"
    break;

  case 84: /* value: SSS  */
#line 831 "yacc_sql.y"
         {
      int len = strlen((yyvsp[0].string))-2;
      char *tmp = common::substr((yyvsp[0].string),1,len);
      (yyval.value) = new Value(tmp, len);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2736 "yacc_sql.cpp"
    break;

  case 85: /* value: DATE_VALUE  */
#line 838 "yacc_sql.y"
                {
      int len = strlen((yyvsp[0].string))-2;
      char *tmp = common::substr((yyvsp[0].string),1,len);
      (yyval.value) = new Value((Date*)tmp, len);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2748 "yacc_sql.cpp"
    break;

  case 86: /* value: NULL_T  */
#line 845 "yacc_sql.y"
            {
      (yyval.value) = new Value((void*)nullptr);
    }
#line 2756 "yacc_sql.cpp"
    break;

  case 87: /* value: LMBRACE value_list RMBRACE  */
#line 848 "yacc_sql.y"
                                 {
      (yyval.value) = new Value((yyvsp[-1].value_list));
      delete (yyvsp[-1].value_list);
    }
#line 2765 "yacc_sql.cpp"
    break;

  case 88: /* storage_format: %empty  */
#line 855 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2773 "yacc_sql.cpp"
    break;

  case 89: /* storage_format: STORAGE FORMAT EQ ID  */
#line 859 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2781 "yacc_sql.cpp"
    break;

  case 90: /* delete_stmt: DELETE FROM ID where  */
#line 866 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        swap((yyval.sql_node)->deletion.conditions, *(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2795 "yacc_sql.cpp"
    break;

  case 91: /* update_stmt: UPDATE ID SET key_values where  */
#line 878 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-3].string);
      (yyval.sql_node)->update.attribute_names.swap((yyvsp[-1].key_values)->relation_list);
      (yyval.sql_node)->update.values.swap((yyvsp[-1].key_values)->value_list);

      delete (yyvsp[-1].key_values);
      free((yyvsp[-3].string));

      if((yyvsp[0].condition_list) != nullptr){
        swap((yyval.sql_node)->update.conditions, *(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
    }
#line 2814 "yacc_sql.cpp"
    break;

  case 92: /* key_values: ID EQ assign_value  */
#line 896 "yacc_sql.y"
    {
      (yyval.key_values) = new Key_values;
      (yyval.key_values)->relation_list.emplace_back(move((yyvsp[-2].string)));
      free((yyvsp[-2].string));
      (yyval.key_values)->value_list.emplace_back(unique_ptr<Expression>((yyvsp[0].expression)));
    }
#line 2825 "yacc_sql.cpp"
    break;

  case 93: /* key_values: ID EQ assign_value COMMA key_values  */
#line 903 "yacc_sql.y"
    {
      if ((yyvsp[0].key_values) != nullptr) {
        (yyval.key_values) = (yyvsp[0].key_values);
      } else {
        (yyval.key_values) = new Key_values;
      }

      (yyval.key_values)->relation_list.emplace_back(move((yyvsp[-4].string)));
      free((yyvsp[-4].string));
      (yyval.key_values)->value_list.emplace_back(unique_ptr<Expression>((yyvsp[-2].expression)));
    }
#line 2841 "yacc_sql.cpp"
    break;

  case 94: /* assign_value: expression  */
#line 917 "yacc_sql.y"
               { (yyval.expression) = (yyvsp[0].expression); }
#line 2847 "yacc_sql.cpp"
    break;

  case 95: /* select_stmt: select_unit  */
#line 922 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
    }
#line 2855 "yacc_sql.cpp"
    break;

  case 96: /* select_stmt: select_unit UNION select_stmt  */
#line 926 "yacc_sql.y"
    {
      // 左结合：将当前 SELECT 和右边的 UNION SELECT 合并
      if ((yyvsp[-2].sql_node) != nullptr && (yyvsp[0].sql_node) != nullptr) {
        (yyvsp[-2].sql_node)->selection.union_select = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
        (yyvsp[-2].sql_node)->selection.union_all = false;
        (yyval.sql_node) = (yyvsp[-2].sql_node);
      } else {
        (yyval.sql_node) = nullptr;
      }
    }
#line 2870 "yacc_sql.cpp"
    break;

  case 97: /* select_stmt: select_unit UNION ALL select_stmt  */
#line 937 "yacc_sql.y"
    {
      // 左结合：将当前 SELECT 和右边的 UNION SELECT 合并
      if ((yyvsp[-3].sql_node) != nullptr && (yyvsp[0].sql_node) != nullptr) {
        (yyvsp[-3].sql_node)->selection.union_select = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
        (yyvsp[-3].sql_node)->selection.union_all = true;
        (yyval.sql_node) = (yyvsp[-3].sql_node);
      } else {
        (yyval.sql_node) = nullptr;
      }
    }
#line 2885 "yacc_sql.cpp"
    break;

  case 98: /* select_unit: SELECT expression_list FROM rel_list where group_by having_node order_by limit  */
#line 951 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-7].expression_list) != nullptr) {
        (yyval.sql_node)->selection.expressions.swap(*(yyvsp[-7].expression_list));
        delete (yyvsp[-7].expression_list);
      }

      if ((yyvsp[-5].join_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap((yyvsp[-5].join_list)->relation_list);
        (yyval.sql_node)->selection.alias.swap((yyvsp[-5].join_list)->alias_list);
        swap((yyval.sql_node)->selection.conditions, (yyvsp[-5].join_list)->condition_list);
        delete (yyvsp[-5].join_list);
      }

      if ((yyvsp[-4].condition_list) != nullptr) {
        auto& conditions = (yyval.sql_node)->selection.conditions;
        if(conditions.conditions.size()){
          conditions.conditions.insert(conditions.conditions.begin(),
            std::make_move_iterator((yyvsp[-4].condition_list)->conditions.begin()), 
            std::make_move_iterator((yyvsp[-4].condition_list)->conditions.end()));
          conditions.and_or = (yyvsp[-4].condition_list)->and_or;
        }
        else swap(conditions, *(yyvsp[-4].condition_list));
        delete (yyvsp[-4].condition_list);
      }

      if ((yyvsp[-3].expression_list) != nullptr) {
        (yyval.sql_node)->selection.group_by.swap(*(yyvsp[-3].expression_list));
        delete (yyvsp[-3].expression_list);
      }

      if ((yyvsp[-2].having_list) != nullptr) {
        swap((yyval.sql_node)->selection.having_list, *(yyvsp[-2].having_list));
        delete (yyvsp[-2].having_list);
      }

      if((yyvsp[-1].order_by_list) != nullptr){
        (yyval.sql_node)->selection.order_by.swap(*(yyvsp[-1].order_by_list));
        delete (yyvsp[-1].order_by_list);
      }

      (yyval.sql_node)->selection.limit = (yyvsp[0].number);
    }
#line 2933 "yacc_sql.cpp"
    break;

  case 99: /* limit: %empty  */
#line 998 "yacc_sql.y"
    {
      (yyval.number) = -1;
    }
#line 2941 "yacc_sql.cpp"
    break;

  case 100: /* limit: LIMIT number  */
#line 1002 "yacc_sql.y"
    {
      (yyval.number) = (yyvsp[0].number);
    }
#line 2949 "yacc_sql.cpp"
    break;

  case 101: /* calc_stmt: CALC expression_list  */
#line 1008 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2959 "yacc_sql.cpp"
    break;

  case 102: /* function_args: expression  */
#line 1017 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      (yyval.expression_list)->emplace_back(move((yyvsp[0].expression)));
    }
#line 2968 "yacc_sql.cpp"
    break;

  case 103: /* function_args: expression COMMA function_args  */
#line 1022 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), move((yyvsp[-2].expression)));
    }
#line 2981 "yacc_sql.cpp"
    break;

  case 104: /* expression_list: expression alias  */
#line 1034 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      if((yyvsp[0].string) != nullptr){
        (yyvsp[-1].expression)->set_alias((yyvsp[0].string));
        free((yyvsp[0].string));
      }
      (yyval.expression_list)->emplace_back(move((yyvsp[-1].expression)));
    }
#line 2994 "yacc_sql.cpp"
    break;

  case 105: /* expression_list: expression alias COMMA expression_list  */
#line 1043 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      }
      if((yyvsp[-2].string) != nullptr){
        (yyvsp[-3].expression)->set_alias((yyvsp[-2].string));
        free((yyvsp[-2].string));
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), move((yyvsp[-3].expression)));
    }
#line 3011 "yacc_sql.cpp"
    break;

  case 106: /* expression: expression '+' expression  */
#line 1057 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 3019 "yacc_sql.cpp"
    break;

  case 107: /* expression: expression '-' expression  */
#line 1060 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 3027 "yacc_sql.cpp"
    break;

  case 108: /* expression: expression '*' expression  */
#line 1063 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 3035 "yacc_sql.cpp"
    break;

  case 109: /* expression: expression '/' expression  */
#line 1066 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 3043 "yacc_sql.cpp"
    break;

  case 110: /* expression: ID LBRACE function_args RBRACE  */
#line 1069 "yacc_sql.y"
                                                  {
      // Check if it's an aggregate function
      bool is_aggregate = (0 == strcasecmp((yyvsp[-3].string), "count") || 
                           0 == strcasecmp((yyvsp[-3].string), "sum") ||
                           0 == strcasecmp((yyvsp[-3].string), "max") ||
                           0 == strcasecmp((yyvsp[-3].string), "min") ||
                           0 == strcasecmp((yyvsp[-3].string), "avg"));
      
      if (is_aggregate) {
        if((yyvsp[-1].expression_list)->size() != 1)(yyval.expression) = create_aggregate_expression("", nullptr, sql_string, &(yyloc));
        else (yyval.expression) = create_aggregate_expression((yyvsp[-3].string), (yyvsp[-1].expression_list)->at(0).release(), sql_string, &(yyloc));
      } else {
        // System function
        if (0 == strcasecmp((yyvsp[-3].string), "date_format")) {
          // DATE_FORMAT requires 2 arguments
          if((yyvsp[-1].expression_list)->size() != 2) {
            yyerror(&(yyloc), sql_string, sql_result, scanner, "DATE_FORMAT requires 2 arguments");
            free((yyvsp[-3].string));
            delete (yyvsp[-1].expression_list);
            YYERROR;
          }
          (yyval.expression) = create_sysfunc_expression((yyvsp[-3].string), (yyvsp[-1].expression_list)->at(0).release(), (yyvsp[-1].expression_list)->at(1).release(), nullptr, sql_string, &(yyloc));
        } else if (0 == strcasecmp((yyvsp[-3].string), "distance")) {
          // DISTANCE requires 3 arguments
          if((yyvsp[-1].expression_list)->size() != 3) {
            yyerror(&(yyloc), sql_string, sql_result, scanner, "DISTANCE requires 3 arguments");
            free((yyvsp[-3].string));
            delete (yyvsp[-1].expression_list);
            YYERROR;
          }
          (yyval.expression) = create_sysfunc_expression((yyvsp[-3].string), (yyvsp[-1].expression_list)->at(0).release(), (yyvsp[-1].expression_list)->at(1).release(), (yyvsp[-1].expression_list)->at(2).release(), sql_string, &(yyloc));
        } else if (0 == strcasecmp((yyvsp[-3].string), "tokenize")) {
          // TOKENIZE requires 2 arguments
          if((yyvsp[-1].expression_list)->size() != 2) {
            yyerror(&(yyloc), sql_string, sql_result, scanner, "TOKENIZE requires 2 arguments");
            free((yyvsp[-3].string));
            delete (yyvsp[-1].expression_list);
            YYERROR;
          }
          (yyval.expression) = create_sysfunc_expression((yyvsp[-3].string), (yyvsp[-1].expression_list)->at(0).release(), (yyvsp[-1].expression_list)->at(1).release(), nullptr, sql_string, &(yyloc));
        } else {
          // LENGTH, ROUND, VECTOR_TO_STRING, STRING_TO_VECTOR require 1 argument
          if((yyvsp[-1].expression_list)->size() != 1) {
            yyerror(&(yyloc), sql_string, sql_result, scanner, "Function requires 1 argument");
            free((yyvsp[-3].string));
            delete (yyvsp[-1].expression_list);
            YYERROR;
          }
          (yyval.expression) = create_sysfunc_expression((yyvsp[-3].string), (yyvsp[-1].expression_list)->at(0).release(), nullptr, nullptr, sql_string, &(yyloc));
        }
      }
      free((yyvsp[-3].string));
      delete (yyvsp[-1].expression_list);
    }
#line 3102 "yacc_sql.cpp"
    break;

  case 111: /* expression: vector_operation LBRACE expression COMMA expression RBRACE  */
#line 1123 "yacc_sql.y"
                                                                {
      (yyval.expression) = create_operation_expression((VectorOperationExpr::Type)(yyvsp[-5].number), (yyvsp[-3].expression), (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 3110 "yacc_sql.cpp"
    break;

  case 112: /* expression: value  */
#line 1126 "yacc_sql.y"
            {
      // Allow value literals (strings, numbers, etc.) as expressions
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 3121 "yacc_sql.cpp"
    break;

  case 113: /* expression: '-' expression  */
#line 1132 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 3129 "yacc_sql.cpp"
    break;

  case 114: /* expression: rel_attr  */
#line 1135 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 3140 "yacc_sql.cpp"
    break;

  case 115: /* expression: '*'  */
#line 1141 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 3148 "yacc_sql.cpp"
    break;

  case 116: /* expression: ID LBRACE RBRACE  */
#line 1144 "yacc_sql.y"
                       {
      // Check if it's an aggregate function (COUNT can have no args)
      bool is_aggregate = (0 == strcasecmp((yyvsp[-2].string), "count"));
      if (is_aggregate) {
        (yyval.expression) = create_aggregate_expression("", nullptr, sql_string, &(yyloc));
      } else {
        yyerror(&(yyloc), sql_string, sql_result, scanner, "Function requires arguments");
        free((yyvsp[-2].string));
        YYERROR;
      }
      free((yyvsp[-2].string));
    }
#line 3165 "yacc_sql.cpp"
    break;

  case 117: /* expression: LBRACE select_stmt RBRACE  */
#line 1156 "yacc_sql.y"
                                {
      (yyval.expression) = new SelectExpr((yyvsp[-1].sql_node));
    }
#line 3173 "yacc_sql.cpp"
    break;

  case 118: /* expression: LBRACE expression_list RBRACE  */
#line 1159 "yacc_sql.y"
                                                 {
      // Parenthesized expression list (for grouping expressions)
      // Use %prec UMINUS to give lower precedence than function calls
      if ((yyvsp[-1].expression_list)->size() == 1) {
        (yyval.expression) = ((yyvsp[-1].expression_list)->front()).release();
      } else {
        (yyval.expression) = new ValueListExpr(*(yyvsp[-1].expression_list));
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[-1].expression_list);
    }
#line 3189 "yacc_sql.cpp"
    break;

  case 119: /* expression: MATCH LBRACE rel_attr RBRACE AGAINST LBRACE SSS RBRACE  */
#line 1171 "yacc_sql.y"
    {
      // MATCH(field) AGAINST('query') as expression (for ORDER BY and SELECT)
      Expression *field_expr = new UnboundFieldExpr((yyvsp[-5].rel_attr)->relation_name, (yyvsp[-5].rel_attr)->attribute_name);
      Expression *query_expr = new ValueExpr(Value((yyvsp[-1].string)));
      (yyval.expression) = create_sysfunc_expression("match_against", field_expr, query_expr, nullptr, sql_string, &(yyloc));
      delete (yyvsp[-5].rel_attr);
      free((yyvsp[-1].string));
    }
#line 3202 "yacc_sql.cpp"
    break;

  case 120: /* ID: ID_KEY  */
#line 1183 "yacc_sql.y"
           { (yyval.string) = (yyvsp[0].string); }
#line 3208 "yacc_sql.cpp"
    break;

  case 121: /* ID: DATA  */
#line 1185 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 5);
      memcpy((yyval.string), "data", sizeof(char) * 5);
    }
#line 3217 "yacc_sql.cpp"
    break;

  case 122: /* ID: LISTS  */
#line 1190 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 6);
      memcpy((yyval.string), "lists", sizeof(char) * 6);
    }
#line 3226 "yacc_sql.cpp"
    break;

  case 123: /* ID: TYPE  */
#line 1195 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 5);
      memcpy((yyval.string), "type", sizeof(char) * 5);
    }
#line 3235 "yacc_sql.cpp"
    break;

  case 124: /* rel_attr: ID  */
#line 1202 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 3245 "yacc_sql.cpp"
    break;

  case 125: /* rel_attr: ID DOT ID  */
#line 1207 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3257 "yacc_sql.cpp"
    break;

  case 126: /* rel_attr: ID DOT '*'  */
#line 1214 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = '*';
      free((yyvsp[-2].string));
    }
#line 3268 "yacc_sql.cpp"
    break;

  case 127: /* relation: ID  */
#line 1223 "yacc_sql.y"
       {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3276 "yacc_sql.cpp"
    break;

  case 128: /* rel_list: relation alias join_list  */
#line 1228 "yacc_sql.y"
                             {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new Joins;
      }

      (yyval.join_list)->relation_list.emplace((yyval.join_list)->relation_list.begin(), (yyvsp[-2].string));
      free((yyvsp[-2].string));
      if((yyvsp[-1].string) != nullptr){
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), (yyvsp[-1].string));
        free((yyvsp[-1].string));
      } else {
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), string());
      }
    }
#line 3297 "yacc_sql.cpp"
    break;

  case 129: /* rel_list: relation alias join_list COMMA rel_list  */
#line 1244 "yacc_sql.y"
                                             {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new Joins;
      }

      if((yyvsp[-2].join_list) != nullptr){
        (yyval.join_list)->relation_list.insert((yyval.join_list)->relation_list.begin(), 
          (yyvsp[-2].join_list)->relation_list.begin(), (yyvsp[-2].join_list)->relation_list.end());
        auto& conditions = (yyval.join_list)->condition_list.conditions;
        conditions.insert(conditions.begin(), 
          std::make_move_iterator((yyvsp[-2].join_list)->condition_list.conditions.begin()), 
          std::make_move_iterator((yyvsp[-2].join_list)->condition_list.conditions.end()));
        (yyval.join_list)->alias_list.insert((yyval.join_list)->alias_list.begin(),
          (yyvsp[-2].join_list)->alias_list.begin(), (yyvsp[-2].join_list)->alias_list.end());
      }

      (yyval.join_list)->relation_list.emplace((yyval.join_list)->relation_list.begin(), (yyvsp[-4].string));
      free((yyvsp[-4].string));
      if((yyvsp[-3].string) != nullptr){
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), string((yyvsp[-3].string)));
        free((yyvsp[-3].string));
      } else {
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), string());
      }

      delete (yyvsp[-2].join_list);
    }
#line 3331 "yacc_sql.cpp"
    break;

  case 130: /* join_list: %empty  */
#line 1277 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 3339 "yacc_sql.cpp"
    break;

  case 131: /* join_list: INNER JOIN relation alias on join_list  */
#line 1281 "yacc_sql.y"
    {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new Joins;
      }

      (yyval.join_list)->relation_list.emplace((yyval.join_list)->relation_list.begin(), (yyvsp[-3].string));
      free((yyvsp[-3].string));
      if((yyvsp[-2].string) != nullptr){
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), (yyvsp[-2].string));
        free((yyvsp[-2].string));
      } else {
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), string());
      }

      if((yyvsp[-1].condition_list) != nullptr){
        auto& conditions = (yyval.join_list)->condition_list.conditions;
        conditions.insert(conditions.end(), std::make_move_iterator((yyvsp[-1].condition_list)->conditions.begin()), 
          std::make_move_iterator((yyvsp[-1].condition_list)->conditions.end()));
        delete (yyvsp[-1].condition_list);
      }
    }
#line 3367 "yacc_sql.cpp"
    break;

  case 132: /* alias: %empty  */
#line 1307 "yacc_sql.y"
                {
      (yyval.string) = nullptr;
    }
#line 3375 "yacc_sql.cpp"
    break;

  case 133: /* alias: ID  */
#line 1310 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3383 "yacc_sql.cpp"
    break;

  case 134: /* alias: AS ID  */
#line 1313 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3391 "yacc_sql.cpp"
    break;

  case 135: /* on: %empty  */
#line 1320 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3399 "yacc_sql.cpp"
    break;

  case 136: /* on: ON condition_list  */
#line 1324 "yacc_sql.y"
    {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3407 "yacc_sql.cpp"
    break;

  case 137: /* where: %empty  */
#line 1332 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3415 "yacc_sql.cpp"
    break;

  case 138: /* where: WHERE condition_list  */
#line 1335 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3423 "yacc_sql.cpp"
    break;

  case 139: /* condition_list: condition  */
#line 1340 "yacc_sql.y"
              {
      (yyval.condition_list) = new Conditions;
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[0].condition)));
      delete (yyvsp[0].condition);
    }
#line 3433 "yacc_sql.cpp"
    break;

  case 140: /* condition_list: condition AND condition_list  */
#line 1345 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[-2].condition)));
      delete (yyvsp[-2].condition);
    }
#line 3443 "yacc_sql.cpp"
    break;

  case 141: /* condition_list: condition OR condition_list  */
#line 1350 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->and_or = true;
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[-2].condition)));
      delete (yyvsp[-2].condition);
    }
#line 3454 "yacc_sql.cpp"
    break;

  case 142: /* condition: expression comp_op expression  */
#line 1359 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = unique_ptr<Expression>((yyvsp[-2].expression));
      (yyval.condition)->right_expr = unique_ptr<Expression>((yyvsp[0].expression));
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3465 "yacc_sql.cpp"
    break;

  case 143: /* condition: MATCH LBRACE rel_attr RBRACE AGAINST LBRACE SSS RBRACE  */
#line 1366 "yacc_sql.y"
    {
      // MATCH(field) AGAINST('query') -> match_against(field, 'query')
      (yyval.condition) = new ConditionSqlNode;
      Expression *field_expr = new UnboundFieldExpr((yyvsp[-5].rel_attr)->relation_name, (yyvsp[-5].rel_attr)->attribute_name);
      Expression *query_expr = new ValueExpr(Value((yyvsp[-1].string)));
      Expression *match_expr = create_sysfunc_expression("match_against", field_expr, query_expr, nullptr, sql_string, &(yyloc));
      (yyval.condition)->left_expr = unique_ptr<Expression>(match_expr);
      (yyval.condition)->right_expr = unique_ptr<Expression>(new ValueExpr(Value(0)));
      (yyval.condition)->comp = GREAT_THAN;
      delete (yyvsp[-5].rel_attr);
      free((yyvsp[-1].string));
    }
#line 3482 "yacc_sql.cpp"
    break;

  case 144: /* condition: unary_op expression  */
#line 1379 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = unique_ptr<Expression>(new ValueExpr(Value((void*)nullptr)));
      (yyval.condition)->right_expr = unique_ptr<Expression>((yyvsp[0].expression));
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3493 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: LIKE  */
#line 1388 "yacc_sql.y"
         { (yyval.comp) = LIKE_OP; }
#line 3499 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: NOT LIKE  */
#line 1389 "yacc_sql.y"
               { (yyval.comp) = NOT_LIKE; }
#line 3505 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: EQ  */
#line 1390 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3511 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: LT  */
#line 1391 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3517 "yacc_sql.cpp"
    break;

  case 149: /* comp_op: GT  */
#line 1392 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3523 "yacc_sql.cpp"
    break;

  case 150: /* comp_op: LE  */
#line 1393 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3529 "yacc_sql.cpp"
    break;

  case 151: /* comp_op: GE  */
#line 1394 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3535 "yacc_sql.cpp"
    break;

  case 152: /* comp_op: NE  */
#line 1395 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3541 "yacc_sql.cpp"
    break;

  case 153: /* comp_op: IN  */
#line 1396 "yacc_sql.y"
         { (yyval.comp) = IN_OP; }
#line 3547 "yacc_sql.cpp"
    break;

  case 154: /* comp_op: NOT IN  */
#line 1397 "yacc_sql.y"
             { (yyval.comp) = NOT_IN; }
#line 3553 "yacc_sql.cpp"
    break;

  case 155: /* comp_op: IS  */
#line 1398 "yacc_sql.y"
         { (yyval.comp) = IS_NULL; }
#line 3559 "yacc_sql.cpp"
    break;

  case 156: /* comp_op: IS NOT  */
#line 1399 "yacc_sql.y"
             { (yyval.comp) = IS_NOT_NULL; }
#line 3565 "yacc_sql.cpp"
    break;

  case 157: /* unary_op: EXISTS  */
#line 1403 "yacc_sql.y"
           { (yyval.comp) = EXISTS_OP; }
#line 3571 "yacc_sql.cpp"
    break;

  case 158: /* unary_op: NOT EXISTS  */
#line 1404 "yacc_sql.y"
                 { (yyval.comp) = NOT_EXISTS; }
#line 3577 "yacc_sql.cpp"
    break;

  case 159: /* group_by: %empty  */
#line 1410 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 3585 "yacc_sql.cpp"
    break;

  case 160: /* group_by: GROUP BY expression_list  */
#line 1414 "yacc_sql.y"
    {
      (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 3593 "yacc_sql.cpp"
    break;

  case 161: /* having_node: %empty  */
#line 1421 "yacc_sql.y"
    {
      (yyval.having_list) = nullptr;
    }
#line 3601 "yacc_sql.cpp"
    break;

  case 162: /* having_node: HAVING having_list  */
#line 1425 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
    }
#line 3609 "yacc_sql.cpp"
    break;

  case 163: /* having_list: having_unit  */
#line 1432 "yacc_sql.y"
    {
      (yyval.having_list) = new HavingNode;
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[0].expression)));
    }
#line 3618 "yacc_sql.cpp"
    break;

  case 164: /* having_list: having_unit OR having_list  */
#line 1437 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[-2].expression)));
      (yyval.having_list)->and_or = true;
    }
#line 3628 "yacc_sql.cpp"
    break;

  case 165: /* having_list: having_unit AND having_list  */
#line 1443 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[-2].expression)));
    }
#line 3637 "yacc_sql.cpp"
    break;

  case 166: /* having_unit: expression comp_op expression  */
#line 1451 "yacc_sql.y"
    {
      unique_ptr<Expression> left((yyvsp[-2].expression));
      unique_ptr<Expression> right((yyvsp[0].expression));
      (yyval.expression) = new ComparisonExpr((yyvsp[-1].comp), std::move(left), std::move(right));
    }
#line 3647 "yacc_sql.cpp"
    break;

  case 167: /* order_by: %empty  */
#line 1460 "yacc_sql.y"
    {
      (yyval.order_by_list) = nullptr;
    }
#line 3655 "yacc_sql.cpp"
    break;

  case 168: /* order_by: ORDER BY order_by_list  */
#line 1464 "yacc_sql.y"
    {
      (yyval.order_by_list) = (yyvsp[0].order_by_list);
    }
#line 3663 "yacc_sql.cpp"
    break;

  case 169: /* order_by_list: order_by_unit  */
#line 1471 "yacc_sql.y"
    {
      (yyval.order_by_list) = new std::vector<OrderByNode>;
      (yyval.order_by_list)->emplace_back(move(*(yyvsp[0].order_by_unit)));
      delete (yyvsp[0].order_by_unit);
    }
#line 3673 "yacc_sql.cpp"
    break;

  case 170: /* order_by_list: order_by_unit COMMA order_by_list  */
#line 1477 "yacc_sql.y"
    {
      if((yyvsp[0].order_by_list) != nullptr){
        (yyval.order_by_list) = (yyvsp[0].order_by_list);
      } else {
        (yyval.order_by_list) = new std::vector<OrderByNode>;
      }
      (yyval.order_by_list)->emplace((yyval.order_by_list)->begin(), move(*(yyvsp[-2].order_by_unit)));
      delete (yyvsp[-2].order_by_unit);
    }
#line 3687 "yacc_sql.cpp"
    break;

  case 171: /* order_by_unit: expression  */
#line 1490 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = true;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[0].expression));
    }
#line 3697 "yacc_sql.cpp"
    break;

  case 172: /* order_by_unit: expression ASC  */
#line 1496 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = true;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[-1].expression));
    }
#line 3707 "yacc_sql.cpp"
    break;

  case 173: /* order_by_unit: expression DESC  */
#line 1502 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = false;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[-1].expression));
    }
#line 3717 "yacc_sql.cpp"
    break;

  case 174: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1511 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);

      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3731 "yacc_sql.cpp"
    break;

  case 175: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1524 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3740 "yacc_sql.cpp"
    break;

  case 176: /* set_variable_stmt: SET ID EQ value  */
#line 1532 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3752 "yacc_sql.cpp"
    break;


#line 3756 "yacc_sql.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 1544 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
