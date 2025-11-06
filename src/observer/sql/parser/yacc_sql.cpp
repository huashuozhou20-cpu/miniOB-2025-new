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
#include <string>
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


#line 157 "yacc_sql.cpp"

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
  YYSYMBOL_VECTOR = 37,                    /* VECTOR  */
  YYSYMBOL_TEXT_T = 38,                    /* TEXT_T  */
  YYSYMBOL_HELP = 39,                      /* HELP  */
  YYSYMBOL_EXIT = 40,                      /* EXIT  */
  YYSYMBOL_DOT = 41,                       /* DOT  */
  YYSYMBOL_INTO = 42,                      /* INTO  */
  YYSYMBOL_VALUES = 43,                    /* VALUES  */
  YYSYMBOL_FROM = 44,                      /* FROM  */
  YYSYMBOL_WHERE = 45,                     /* WHERE  */
  YYSYMBOL_INNER = 46,                     /* INNER  */
  YYSYMBOL_JOIN = 47,                      /* JOIN  */
  YYSYMBOL_AND = 48,                       /* AND  */
  YYSYMBOL_OR = 49,                        /* OR  */
  YYSYMBOL_SET = 50,                       /* SET  */
  YYSYMBOL_ON = 51,                        /* ON  */
  YYSYMBOL_AS = 52,                        /* AS  */
  YYSYMBOL_LOAD = 53,                      /* LOAD  */
  YYSYMBOL_DATA = 54,                      /* DATA  */
  YYSYMBOL_LIKE = 55,                      /* LIKE  */
  YYSYMBOL_NULL_T = 56,                    /* NULL_T  */
  YYSYMBOL_EXISTS = 57,                    /* EXISTS  */
  YYSYMBOL_IN = 58,                        /* IN  */
  YYSYMBOL_IS = 59,                        /* IS  */
  YYSYMBOL_NOT = 60,                       /* NOT  */
  YYSYMBOL_INFILE = 61,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 62,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 63,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 64,                    /* FORMAT  */
  YYSYMBOL_LIMIT = 65,                     /* LIMIT  */
  YYSYMBOL_EQ = 66,                        /* EQ  */
  YYSYMBOL_LT = 67,                        /* LT  */
  YYSYMBOL_GT = 68,                        /* GT  */
  YYSYMBOL_LE = 69,                        /* LE  */
  YYSYMBOL_GE = 70,                        /* GE  */
  YYSYMBOL_NE = 71,                        /* NE  */
  YYSYMBOL_UNIQUE = 72,                    /* UNIQUE  */
  YYSYMBOL_UNION = 73,                     /* UNION  */
  YYSYMBOL_ALTER = 74,                     /* ALTER  */
  YYSYMBOL_ADD = 75,                       /* ADD  */
  YYSYMBOL_COLUMN = 76,                    /* COLUMN  */
  YYSYMBOL_RENAME = 77,                    /* RENAME  */
  YYSYMBOL_TO = 78,                        /* TO  */
  YYSYMBOL_FULLTEXT = 79,                  /* FULLTEXT  */
  YYSYMBOL_ALL = 80,                       /* ALL  */
  YYSYMBOL_L2_DISTANCE = 81,               /* L2_DISTANCE  */
  YYSYMBOL_COSINE_DISTANCE = 82,           /* COSINE_DISTANCE  */
  YYSYMBOL_INNER_PRODUCT = 83,             /* INNER_PRODUCT  */
  YYSYMBOL_DISTANCE = 84,                  /* DISTANCE  */
  YYSYMBOL_LISTS = 85,                     /* LISTS  */
  YYSYMBOL_TYPE = 86,                      /* TYPE  */
  YYSYMBOL_PROBES = 87,                    /* PROBES  */
  YYSYMBOL_IVFFLAT = 88,                   /* IVFFLAT  */
  YYSYMBOL_MATCH = 89,                     /* MATCH  */
  YYSYMBOL_AGAINST = 90,                   /* AGAINST  */
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
  YYSYMBOL_expression_list = 144,          /* expression_list  */
  YYSYMBOL_expression = 145,               /* expression  */
  YYSYMBOL_ID = 146,                       /* ID  */
  YYSYMBOL_rel_attr = 147,                 /* rel_attr  */
  YYSYMBOL_relation = 148,                 /* relation  */
  YYSYMBOL_rel_list = 149,                 /* rel_list  */
  YYSYMBOL_join_list = 150,                /* join_list  */
  YYSYMBOL_alias = 151,                    /* alias  */
  YYSYMBOL_on = 152,                       /* on  */
  YYSYMBOL_where = 153,                    /* where  */
  YYSYMBOL_condition_list = 154,           /* condition_list  */
  YYSYMBOL_condition = 155,                /* condition  */
  YYSYMBOL_comp_op = 156,                  /* comp_op  */
  YYSYMBOL_unary_op = 157,                 /* unary_op  */
  YYSYMBOL_group_by = 158,                 /* group_by  */
  YYSYMBOL_having_node = 159,              /* having_node  */
  YYSYMBOL_having_list = 160,              /* having_list  */
  YYSYMBOL_having_unit = 161,              /* having_unit  */
  YYSYMBOL_order_by = 162,                 /* order_by  */
  YYSYMBOL_order_by_list = 163,            /* order_by_list  */
  YYSYMBOL_order_by_unit = 164,            /* order_by_unit  */
  YYSYMBOL_load_data_stmt = 165,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 166,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 167,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 168             /* opt_semicolon  */
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
#define YYFINAL  81
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   433

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  101
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  175
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  363

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
       0,   302,   302,   312,   313,   314,   315,   316,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   326,   327,   328,
     329,   330,   331,   332,   333,   334,   338,   344,   349,   355,
     361,   367,   373,   379,   387,   393,   401,   411,   421,   433,
     446,   465,   484,   512,   516,   520,   527,   531,   534,   548,
     551,   564,   567,   572,   583,   590,   609,   630,   648,   660,
     663,   668,   671,   684,   693,   702,   715,   716,   717,   721,
     724,   725,   726,   727,   728,   729,   732,   736,   758,   764,
     778,   788,   805,   809,   813,   823,   833,   836,   843,   846,
     853,   865,   883,   890,   905,   909,   913,   924,   938,   986,
     989,   995,  1004,  1013,  1028,  1031,  1034,  1037,  1040,  1043,
    1052,  1055,  1061,  1064,  1118,  1130,  1133,  1146,  1147,  1152,
    1157,  1165,  1170,  1177,  1186,  1191,  1207,  1240,  1243,  1270,
    1273,  1276,  1283,  1286,  1295,  1298,  1303,  1308,  1313,  1321,
    1328,  1341,  1351,  1352,  1353,  1354,  1355,  1356,  1357,  1358,
    1359,  1360,  1361,  1362,  1366,  1367,  1373,  1376,  1384,  1387,
    1394,  1399,  1405,  1413,  1423,  1426,  1433,  1439,  1452,  1458,
    1464,  1473,  1490,  1498,  1508,  1509
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
  "FLOAT_T", "DATE_T", "VECTOR_T", "VECTOR", "TEXT_T", "HELP", "EXIT",
  "DOT", "INTO", "VALUES", "FROM", "WHERE", "INNER", "JOIN", "AND", "OR",
  "SET", "ON", "AS", "LOAD", "DATA", "LIKE", "NULL_T", "EXISTS", "IN",
  "IS", "NOT", "INFILE", "EXPLAIN", "STORAGE", "FORMAT", "LIMIT", "EQ",
  "LT", "GT", "LE", "GE", "NE", "UNIQUE", "UNION", "ALTER", "ADD",
  "COLUMN", "RENAME", "TO", "FULLTEXT", "ALL", "L2_DISTANCE",
  "COSINE_DISTANCE", "INNER_PRODUCT", "DISTANCE", "LISTS", "TYPE",
  "PROBES", "IVFFLAT", "MATCH", "AGAINST", "NUMBER", "FLOAT", "ID_KEY",
  "SSS", "DATE_VALUE", "'+'", "'-'", "'*'", "'/'", "UMINUS", "$accept",
  "commands", "command_wrapper", "exit_stmt", "help_stmt", "sync_stmt",
  "begin_stmt", "commit_stmt", "rollback_stmt", "drop_table_stmt",
  "show_index_stmt", "show_tables_stmt", "desc_table_stmt",
  "alter_table_stmt", "create_index_stmt", "vector_operation",
  "vector_index_type", "idx_col_list", "col_list", "unique_option",
  "drop_index_stmt", "create_view_stmt", "create_table_stmt", "as_option",
  "attr_def_list", "attr_def", "nullable", "number", "type", "date_type",
  "insert_stmt", "values_list", "value_list", "value", "storage_format",
  "delete_stmt", "update_stmt", "key_values", "assign_value",
  "select_stmt", "select_unit", "limit", "calc_stmt", "expression_list",
  "expression", "ID", "rel_attr", "relation", "rel_list", "join_list",
  "alias", "on", "where", "condition_list", "condition", "comp_op",
  "unary_op", "group_by", "having_node", "having_list", "having_unit",
  "order_by", "order_by_list", "order_by_unit", "load_data_stmt",
  "explain_stmt", "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-308)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-141)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     317,     3,    21,   173,   217,   217,    27,  -308,    -5,    -9,
     173,  -308,  -308,  -308,  -308,  -308,   173,    20,   317,    80,
     101,   103,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,    42,  -308,  -308,  -308,  -308,   173,   173,   107,  -308,
     120,   122,   173,   173,  -308,  -308,  -308,  -308,  -308,    63,
    -308,  -308,  -308,   115,   217,  -308,   127,  -308,   289,    10,
    -308,   110,  -308,   118,   173,   173,   113,   106,   112,  -308,
     173,  -308,  -308,  -308,     8,    -7,    -6,   173,   173,   173,
    -308,   124,   149,   151,   173,  -308,   217,   173,   217,   217,
     217,   217,  -308,   150,   157,   190,   173,   173,   155,   139,
     173,   141,    91,     6,   170,  -308,   173,  -308,   170,   173,
     170,   142,   143,   144,   173,  -308,  -308,   158,   171,    30,
    -308,   -51,   -51,  -308,  -308,   217,  -308,   185,  -308,  -308,
    -308,   -31,   139,  -308,   173,   172,   123,  -308,   139,   132,
     217,  -308,  -308,  -308,  -308,  -308,  -308,   175,   137,   138,
     -47,  -308,   191,   326,  -308,   195,  -308,   173,   173,   173,
    -308,   134,   217,  -308,  -308,   179,   219,   195,   205,  -308,
     174,   210,   334,  -308,    71,   217,  -308,   217,   218,    72,
     235,   173,   173,   173,   173,   173,   223,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,    -4,   -30,   173,   224,   236,   237,
     239,   240,    13,   220,   241,   261,   259,   247,   217,  -308,
    -308,   173,  -308,  -308,   214,    18,  -308,  -308,  -308,  -308,
    -308,  -308,   217,   123,   123,    35,   249,    35,  -308,   217,
     173,  -308,  -308,   200,  -308,   191,     7,   193,  -308,   229,
    -308,  -308,   195,   234,   173,   173,   173,   196,  -308,   173,
     173,   217,   217,   281,  -308,   266,   268,  -308,  -308,  -308,
      35,  -308,  -308,   173,  -308,  -308,   173,  -308,   230,   170,
    -308,  -308,   270,  -308,  -308,   170,   195,   195,   195,   271,
     -31,  -308,  -308,   334,  -308,    94,   293,   243,   273,   215,
    -308,  -308,   251,  -308,   -30,  -308,   294,   295,   296,  -308,
     267,   217,   217,   217,   217,   193,  -308,   205,   300,   173,
    -308,   305,  -308,  -308,   123,   179,    35,  -308,  -308,     0,
    -308,   298,  -308,  -308,   233,  -308,   306,  -308,  -308,  -308,
    -308,   217,   312,   245,  -308,   304,   276,   256,  -308,   323,
     282,   279,    76,   327,   269,   299,   277,   343,   285,   307,
     193,   351,  -308
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    51,     0,     0,     0,     0,     0,    28,     0,     0,
       0,    29,    30,    31,    27,    26,     0,     0,     0,     0,
       0,   174,    25,    24,    17,    18,    19,    20,     9,    10,
      12,    13,    16,    14,    15,    11,     8,     5,     7,     6,
       4,    95,     3,    21,    22,    23,     0,     0,     0,    52,
       0,     0,     0,     0,   118,   119,   120,   117,    35,     0,
      43,    44,    45,     0,     0,   112,     0,   101,   129,   121,
     111,     0,    34,     0,     0,     0,     0,     0,     0,   172,
       0,     1,   175,     2,     0,    59,     0,     0,     0,     0,
      32,     0,     0,     0,     0,   110,     0,     0,     0,     0,
       0,     0,   130,   102,     0,     0,     0,     0,    49,   134,
       0,     0,     0,     0,     0,    96,     0,    60,     0,     0,
       0,     0,     0,     0,     0,   115,   109,   121,     0,     0,
     131,   104,   105,   106,   107,     0,   114,     0,   123,   122,
     124,   129,   134,    33,     0,     0,     0,    90,   134,     0,
       0,    86,    82,    83,    84,    85,   173,     0,     0,     0,
       0,    97,    61,     0,    58,    47,    54,     0,     0,     0,
      53,     0,     0,   103,   113,   127,   156,    47,     0,   154,
       0,     0,     0,   135,   136,     0,    91,     0,     0,    80,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      76,    73,    74,    75,    66,    66,     0,     0,     0,     0,
       0,     0,     0,     0,   125,     0,   158,     0,     0,    77,
     155,     0,   142,   150,   152,     0,   144,   145,   146,   147,
     148,   149,     0,     0,     0,   141,    92,    94,    87,     0,
       0,    37,    36,     0,    39,    61,    88,     0,    67,     0,
      64,    65,    47,     0,     0,     0,     0,     0,   108,     0,
       0,     0,     0,   164,    50,     0,     0,   153,   143,   151,
     139,   137,   138,     0,    81,   171,     0,    62,     0,     0,
      56,    69,     0,    68,    48,     0,    47,    47,    47,     0,
     129,   126,   157,     0,   159,   160,     0,    99,    78,     0,
      93,    38,     0,    57,    66,    55,     0,     0,     0,   116,
     132,     0,     0,     0,     0,     0,    98,     0,     0,     0,
      63,     0,    41,    40,     0,   127,   163,   162,   161,   168,
     165,   166,   100,    79,     0,    89,     0,   133,   128,   169,
     170,     0,     0,     0,   167,   116,     0,     0,    46,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    42
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -308,  -308,   360,  -308,  -308,  -308,  -308,  -308,  -308,  -308,
    -308,  -308,  -308,  -308,  -308,    28,  -308,  -163,  -308,  -308,
    -308,  -308,  -308,   135,   145,  -102,  -196,  -307,  -308,  -308,
    -308,    66,  -199,  -308,  -308,  -308,  -308,   111,  -308,   -57,
    -308,  -308,  -308,     1,   -32,    -3,   -91,   136,   146,    73,
    -140,  -308,  -120,  -229,  -308,   104,  -308,  -308,  -308,  -234,
    -308,  -308,    55,  -308,  -308,  -308,  -308,  -308
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    66,   349,   207,   145,    51,
      34,    35,    36,   118,   196,   162,   250,   282,   204,   205,
      37,   219,   188,   156,   280,    38,    39,   148,   236,    40,
      41,   316,    42,    67,    68,    69,    70,   141,   142,   214,
     103,   325,   147,   183,   184,   232,   185,   216,   263,   294,
     295,   297,   330,   331,    43,    44,    45,    83
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   175,    92,   128,   271,   272,    71,    76,   332,   251,
     339,   340,   158,    77,   217,    46,    47,   116,   119,   265,
     247,    97,   176,    54,   -59,     5,   248,   115,   186,   193,
     249,   194,    95,    52,   104,    75,    53,    74,   258,    48,
     274,    72,    73,    85,    86,   117,   120,   100,   101,    90,
      91,   105,   248,   361,    55,    56,   249,   161,   172,   117,
      93,   164,    57,   166,   129,   102,   131,   132,   133,   134,
     278,   108,   109,   268,    78,    49,   269,   113,   327,   328,
       5,   159,    50,   160,   121,   122,   123,    59,   114,   284,
     242,   127,    80,   245,   130,   337,    98,    99,   100,   101,
     239,    81,   139,   140,   143,   137,    82,   149,   320,    98,
      99,   100,   101,   163,   182,    84,   165,    54,   189,   233,
     234,   170,    87,   306,   307,   308,    98,    99,   100,   101,
     266,    98,    99,   100,   101,    88,   173,    89,   102,    94,
     212,   177,   312,   313,    60,    61,    62,    59,    55,    56,
     310,    96,    63,   235,   106,   237,    57,    60,    61,    62,
      64,    65,   107,   110,   208,   209,   210,   150,    98,    99,
     100,   101,   111,   112,   125,   124,   126,    54,   135,   144,
     179,    59,   136,   180,   146,   157,   189,     5,   241,   163,
     243,   244,   163,   167,   168,   169,   171,   151,   187,   105,
     270,   182,   182,   252,    60,    61,    62,   189,    55,    56,
     174,    54,   181,   191,   192,   178,    57,   190,   127,   195,
      64,    65,   303,   206,   211,   213,   215,    54,   305,   218,
     293,   220,   152,   153,   221,   154,   155,   275,    60,    61,
      62,    59,    55,    56,    54,   238,    63,   240,   246,   253,
      57,   286,   287,   288,    64,    65,   140,   140,    55,    56,
     254,   255,   292,   256,   257,   261,    57,   259,   262,   260,
     149,    54,   264,   301,   267,    55,    56,   273,   276,   326,
     293,   293,   329,    57,   281,   283,   285,   102,   138,   296,
     289,   298,   182,   299,   302,   304,   309,   314,    60,    61,
      62,   317,    55,    56,  -140,   318,    63,  -140,   315,   329,
      57,  -140,  -140,  -140,    64,    65,   335,   319,   324,   321,
     322,   323,     1,     2,   334,   336,   341,   342,     3,  -140,
     343,   346,  -140,     4,     5,     6,     7,   345,     8,     9,
      10,    97,   347,    54,   348,   352,    11,    12,    13,  -140,
    -140,   350,  -140,  -140,   355,   354,    14,    15,   197,   198,
     199,   200,   201,   202,   203,   356,   351,    16,   357,  -140,
      17,   358,   359,   360,    55,    56,   362,  -140,    79,    18,
     353,   279,    57,   333,   300,    98,    99,   100,   101,   222,
     277,    19,   223,   224,   225,   290,   344,   311,   338,     0,
     226,   227,   228,   229,   230,   231,   291,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      98,    99,   100,   101
};

static const yytype_int16 yycheck[] =
{
       3,   141,    59,    94,   233,   234,     5,    10,   315,   205,
      10,    11,     6,    16,   177,    12,    13,    24,    24,   218,
      24,    52,   142,    54,    17,    17,    56,    84,   148,    76,
      60,    78,    64,    12,    24,    44,    15,    42,    25,    36,
     239,    14,    15,    46,    47,    52,    52,    98,    99,    52,
      53,    41,    56,   360,    85,    86,    60,   114,    28,    52,
      59,   118,    93,   120,    96,    68,    98,    99,   100,   101,
      63,    74,    75,    55,    54,    72,    58,    80,   312,   313,
      17,    75,    79,    77,    87,    88,    89,    24,    80,   252,
     192,    94,    12,   195,    97,   324,    96,    97,    98,    99,
      28,     0,   105,   106,   107,   104,     3,   110,   304,    96,
      97,    98,    99,   116,   146,    73,   119,    54,   150,    48,
      49,   124,    15,   286,   287,   288,    96,    97,    98,    99,
     221,    96,    97,    98,    99,    15,   135,    15,   141,    24,
     172,   144,    48,    49,    81,    82,    83,    24,    85,    86,
     290,    24,    89,   185,    44,   187,    93,    81,    82,    83,
      97,    98,    44,    50,   167,   168,   169,    26,    96,    97,
      98,    99,    66,    61,    25,    51,    25,    54,    28,    24,
      57,    24,    25,    60,    45,    94,   218,    17,   191,   192,
     193,   194,   195,    51,    51,    51,    25,    56,    66,    41,
     232,   233,   234,   206,    81,    82,    83,   239,    85,    86,
      25,    54,    89,    76,    76,    43,    93,    42,   221,    28,
      97,    98,   279,    28,    90,    46,     7,    54,   285,    24,
     262,    57,    91,    92,    24,    94,    95,   240,    81,    82,
      83,    24,    85,    86,    54,    27,    89,    12,    25,    25,
      93,   254,   255,   256,    97,    98,   259,   260,    85,    86,
      24,    24,   261,    24,    24,     4,    93,    47,     9,    28,
     273,    54,    25,   276,    60,    85,    86,    28,    78,   311,
     312,   313,   314,    93,    91,    56,    52,   290,    98,     8,
      94,    25,   324,    25,    64,    25,    25,     4,    81,    82,
      83,    28,    85,    86,     0,    90,    89,     3,    65,   341,
      93,     7,     8,     9,    97,    98,   319,    66,    51,    25,
      25,    25,     5,     6,    24,    20,    28,    94,    11,    25,
      24,    86,    28,    16,    17,    18,    19,    25,    21,    22,
      23,    52,    66,    54,    88,    66,    29,    30,    31,    45,
      46,    28,    48,    49,    85,    28,    39,    40,    32,    33,
      34,    35,    36,    37,    38,    66,    84,    50,    91,    65,
      53,    28,    87,    66,    85,    86,    25,    73,    18,    62,
     352,   246,    93,   317,   273,    96,    97,    98,    99,    55,
     245,    74,    58,    59,    60,   259,   341,   293,   325,    -1,
      66,    67,    68,    69,    70,    71,   260,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      96,    97,    98,    99
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     6,    11,    16,    17,    18,    19,    21,    22,
      23,    29,    30,    31,    39,    40,    50,    53,    62,    74,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   121,   122,   123,   131,   136,   137,
     140,   141,   143,   165,   166,   167,    12,    13,    36,    72,
      79,   120,    12,    15,    54,    85,    86,    93,   146,    24,
      81,    82,    83,    89,    97,    98,   116,   144,   145,   146,
     147,   144,    14,    15,    42,    44,   146,   146,    54,   103,
      12,     0,     3,   168,    73,   146,   146,    15,    15,    15,
     146,   146,   140,   144,    24,   145,    24,    52,    96,    97,
      98,    99,   146,   151,    24,    41,    44,    44,   146,   146,
      50,    66,    61,   146,    80,   140,    24,    52,   124,    24,
      52,   146,   146,   146,    51,    25,    25,   146,   147,   145,
     146,   145,   145,   145,   145,    28,    25,   144,    98,   146,
     146,   148,   149,   146,    24,   119,    45,   153,   138,   146,
      26,    56,    91,    92,    94,    95,   134,    94,     6,    75,
      77,   140,   126,   146,   140,   146,   140,    51,    51,    51,
     146,    25,    28,   144,    25,   151,   153,   146,    43,    57,
      60,    89,   145,   154,   155,   157,   153,    66,   133,   145,
      42,    76,    76,    76,    78,    28,   125,    32,    33,    34,
      35,    36,    37,    38,   129,   130,    28,   118,   146,   146,
     146,    90,   145,    46,   150,     7,   158,   118,    24,   132,
      57,    24,    55,    58,    59,    60,    66,    67,    68,    69,
      70,    71,   156,    48,    49,   145,   139,   145,    27,    28,
      12,   146,   126,   146,   146,   126,    25,    24,    56,    60,
     127,   127,   146,    25,    24,    24,    24,    24,    25,    47,
      28,     4,     9,   159,    25,   133,   147,    60,    55,    58,
     145,   154,   154,    28,   133,   146,    78,   125,    63,   124,
     135,    91,   128,    56,   118,    52,   146,   146,   146,    94,
     148,   149,   144,   145,   160,   161,     8,   162,    25,    25,
     138,   146,    64,   140,    25,   140,   118,   118,   118,    25,
     151,   156,    48,    49,     4,    65,   142,    28,    90,    66,
     127,    25,    25,    25,    51,   152,   145,   160,   160,   145,
     163,   164,   128,   132,    24,   146,    20,   154,   150,    10,
      11,    28,    94,    24,   163,    25,    86,    66,    88,   117,
      28,    84,    66,   116,    28,    85,    66,    91,    28,    87,
      66,   128,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   101,   102,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   103,   103,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   114,   114,   114,
     115,   115,   115,   116,   116,   116,   117,   118,   118,   119,
     119,   120,   120,   121,   122,   122,   123,   123,   123,   124,
     124,   125,   125,   126,   126,   126,   127,   127,   127,   128,
     129,   129,   129,   129,   129,   129,   130,   131,   132,   132,
     133,   133,   134,   134,   134,   134,   134,   134,   135,   135,
     136,   137,   138,   138,   139,   140,   140,   140,   141,   142,
     142,   143,   144,   144,   145,   145,   145,   145,   145,   145,
     145,   145,   145,   145,   145,   145,   145,   146,   146,   146,
     146,   147,   147,   147,   148,   149,   149,   150,   150,   151,
     151,   151,   152,   152,   153,   153,   154,   154,   154,   155,
     155,   155,   156,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   157,   157,   158,   158,   159,   159,
     160,   160,   160,   161,   162,   162,   163,   163,   164,   164,
     164,   165,   166,   167,   168,   168
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     4,     2,     2,     6,     6,     8,     6,
      10,    10,    28,     1,     1,     1,     1,     0,     3,     0,
       4,     0,     1,     5,     5,     9,     8,     9,     5,     0,
       1,     0,     3,     6,     3,     3,     0,     1,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     6,     3,     5,
       1,     3,     1,     1,     1,     1,     1,     3,     0,     4,
       4,     5,     3,     5,     1,     1,     3,     4,     9,     0,
       2,     2,     2,     4,     3,     3,     3,     3,     6,     3,
       2,     1,     1,     4,     3,     3,     8,     1,     1,     1,
       1,     1,     3,     3,     1,     3,     5,     0,     6,     0,
       1,     2,     0,     2,     0,     2,     1,     3,     3,     3,
       8,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     2,     1,     2,     1,     2,     0,     3,     0,     2,
       1,     3,     3,     3,     0,     3,     1,     3,     1,     2,
       2,     7,     2,     4,     0,     1
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
  YY_USE (yykind);
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
#line 303 "yacc_sql.y"
  {
    if((yyvsp[-1].sql_node) != nullptr){
      std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
      sql_result->add_sql_node(std::move(sql_node));
    }
  }
#line 1998 "yacc_sql.cpp"
    break;

  case 26: /* exit_stmt: EXIT  */
#line 338 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 2007 "yacc_sql.cpp"
    break;

  case 27: /* help_stmt: HELP  */
#line 344 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 2015 "yacc_sql.cpp"
    break;

  case 28: /* sync_stmt: SYNC  */
#line 349 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 2023 "yacc_sql.cpp"
    break;

  case 29: /* begin_stmt: TRX_BEGIN  */
#line 355 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 2031 "yacc_sql.cpp"
    break;

  case 30: /* commit_stmt: TRX_COMMIT  */
#line 361 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 2039 "yacc_sql.cpp"
    break;

  case 31: /* rollback_stmt: TRX_ROLLBACK  */
#line 367 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 2047 "yacc_sql.cpp"
    break;

  case 32: /* drop_table_stmt: DROP TABLE ID  */
#line 373 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2057 "yacc_sql.cpp"
    break;

  case 33: /* show_index_stmt: SHOW INDEX FROM ID  */
#line 380 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_INDEX);
      (yyval.sql_node)->show_index.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2067 "yacc_sql.cpp"
    break;

  case 34: /* show_tables_stmt: SHOW TABLES  */
#line 387 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 2075 "yacc_sql.cpp"
    break;

  case 35: /* desc_table_stmt: DESC ID  */
#line 393 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2085 "yacc_sql.cpp"
    break;

  case 36: /* alter_table_stmt: ALTER TABLE ID ADD COLUMN attr_def  */
#line 402 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::ADD_COLUMN;
      alter_table.attr_info = *(yyvsp[0].attr_info);
      free((yyvsp[-3].string));
      delete (yyvsp[0].attr_info);
    }
#line 2099 "yacc_sql.cpp"
    break;

  case 37: /* alter_table_stmt: ALTER TABLE ID DROP COLUMN ID  */
#line 412 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::DROP_COLUMN;
      alter_table.old_name = (yyvsp[0].string);
      free((yyvsp[-3].string));
      free((yyvsp[0].string));
    }
#line 2113 "yacc_sql.cpp"
    break;

  case 38: /* alter_table_stmt: ALTER TABLE ID RENAME COLUMN ID TO ID  */
#line 422 "yacc_sql.y"
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
#line 2129 "yacc_sql.cpp"
    break;

  case 39: /* alter_table_stmt: ALTER TABLE ID RENAME TO ID  */
#line 434 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::RENAME_TABLE;
      alter_table.new_name = (yyvsp[0].string);
      free((yyvsp[-3].string));
      free((yyvsp[0].string));
    }
#line 2143 "yacc_sql.cpp"
    break;

  case 40: /* create_index_stmt: CREATE unique_option INDEX ID ON ID LBRACE ID idx_col_list RBRACE  */
#line 447 "yacc_sql.y"
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
#line 2166 "yacc_sql.cpp"
    break;

  case 41: /* create_index_stmt: CREATE FULLTEXT INDEX ID ON ID LBRACE ID idx_col_list RBRACE  */
#line 466 "yacc_sql.y"
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
#line 2189 "yacc_sql.cpp"
    break;

  case 42: /* create_index_stmt: CREATE VECTOR_T INDEX ID ON ID LBRACE ID idx_col_list RBRACE WITH LBRACE TYPE EQ vector_index_type COMMA DISTANCE EQ vector_operation COMMA LISTS EQ NUMBER COMMA PROBES EQ number RBRACE  */
#line 487 "yacc_sql.y"
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
#line 2216 "yacc_sql.cpp"
    break;

  case 43: /* vector_operation: L2_DISTANCE  */
#line 513 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::L2_DISTANCE;
    }
#line 2224 "yacc_sql.cpp"
    break;

  case 44: /* vector_operation: COSINE_DISTANCE  */
#line 517 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::COSINE_DISTANCE;
    }
#line 2232 "yacc_sql.cpp"
    break;

  case 45: /* vector_operation: INNER_PRODUCT  */
#line 521 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::INNER_PRODUCT;
    }
#line 2240 "yacc_sql.cpp"
    break;

  case 46: /* vector_index_type: IVFFLAT  */
#line 527 "yacc_sql.y"
            { (yyval.vector_index_type) = VectorIndexType::IVFFLAT; }
#line 2246 "yacc_sql.cpp"
    break;

  case 47: /* idx_col_list: %empty  */
#line 531 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2254 "yacc_sql.cpp"
    break;

  case 48: /* idx_col_list: COMMA ID idx_col_list  */
#line 535 "yacc_sql.y"
    {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }
      (yyval.relation_list)->emplace_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 2268 "yacc_sql.cpp"
    break;

  case 49: /* col_list: %empty  */
#line 548 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2276 "yacc_sql.cpp"
    break;

  case 50: /* col_list: LBRACE ID idx_col_list RBRACE  */
#line 552 "yacc_sql.y"
    {
      if ((yyvsp[-1].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[-1].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }
      (yyval.relation_list)->emplace_back((yyvsp[-2].string));
      free((yyvsp[-2].string));      
    }
#line 2290 "yacc_sql.cpp"
    break;

  case 51: /* unique_option: %empty  */
#line 564 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2298 "yacc_sql.cpp"
    break;

  case 52: /* unique_option: UNIQUE  */
#line 568 "yacc_sql.y"
    {
      (yyval.boolean) = true;
    }
#line 2306 "yacc_sql.cpp"
    break;

  case 53: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 573 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2318 "yacc_sql.cpp"
    break;

  case 54: /* create_view_stmt: CREATE VIEW ID AS select_stmt  */
#line 584 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_VIEW;
      (yyval.sql_node)->create_view.view_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2329 "yacc_sql.cpp"
    break;

  case 55: /* create_view_stmt: CREATE VIEW ID LBRACE ID idx_col_list RBRACE AS select_stmt  */
#line 591 "yacc_sql.y"
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
#line 2349 "yacc_sql.cpp"
    break;

  case 56: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 610 "yacc_sql.y"
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
#line 2374 "yacc_sql.cpp"
    break;

  case 57: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE as_option select_stmt  */
#line 631 "yacc_sql.y"
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
#line 2396 "yacc_sql.cpp"
    break;

  case 58: /* create_table_stmt: CREATE TABLE ID as_option select_stmt  */
#line 649 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2408 "yacc_sql.cpp"
    break;

  case 59: /* as_option: %empty  */
#line 660 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2416 "yacc_sql.cpp"
    break;

  case 60: /* as_option: AS  */
#line 663 "yacc_sql.y"
         { (yyval.boolean) = true; }
#line 2422 "yacc_sql.cpp"
    break;

  case 61: /* attr_def_list: %empty  */
#line 668 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2430 "yacc_sql.cpp"
    break;

  case 62: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 672 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(move(*(yyvsp[-1].attr_info)));
      delete (yyvsp[-1].attr_info);
    }
#line 2444 "yacc_sql.cpp"
    break;

  case 63: /* attr_def: ID type LBRACE number RBRACE nullable  */
#line 685 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-5].string));
    }
#line 2457 "yacc_sql.cpp"
    break;

  case 64: /* attr_def: ID type nullable  */
#line 694 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2470 "yacc_sql.cpp"
    break;

  case 65: /* attr_def: ID date_type nullable  */
#line 703 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 10;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2483 "yacc_sql.cpp"
    break;

  case 66: /* nullable: %empty  */
#line 715 "yacc_sql.y"
    { (yyval.boolean) = true; }
#line 2489 "yacc_sql.cpp"
    break;

  case 67: /* nullable: NULL_T  */
#line 716 "yacc_sql.y"
             {(yyval.boolean) = true;}
#line 2495 "yacc_sql.cpp"
    break;

  case 68: /* nullable: NOT NULL_T  */
#line 717 "yacc_sql.y"
                 {(yyval.boolean) = false;}
#line 2501 "yacc_sql.cpp"
    break;

  case 69: /* number: NUMBER  */
#line 721 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2507 "yacc_sql.cpp"
    break;

  case 70: /* type: INT_T  */
#line 724 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2513 "yacc_sql.cpp"
    break;

  case 71: /* type: STRING_T  */
#line 725 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2519 "yacc_sql.cpp"
    break;

  case 72: /* type: FLOAT_T  */
#line 726 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2525 "yacc_sql.cpp"
    break;

  case 73: /* type: VECTOR_T  */
#line 727 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2531 "yacc_sql.cpp"
    break;

  case 74: /* type: VECTOR  */
#line 728 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2537 "yacc_sql.cpp"
    break;

  case 75: /* type: TEXT_T  */
#line 729 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::TEXTS);}
#line 2543 "yacc_sql.cpp"
    break;

  case 76: /* date_type: DATE_T  */
#line 732 "yacc_sql.y"
             {(yyval.number) = static_cast<int>(AttrType::DATES);}
#line 2549 "yacc_sql.cpp"
    break;

  case 77: /* insert_stmt: INSERT INTO ID col_list VALUES values_list  */
#line 737 "yacc_sql.y"
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
#line 2572 "yacc_sql.cpp"
    break;

  case 78: /* values_list: LBRACE value_list RBRACE  */
#line 759 "yacc_sql.y"
    {
      (yyval.values_list) = new std::vector<std::vector<Value>>;
      (yyval.values_list)->emplace_back(move(*(yyvsp[-1].value_list)));
      delete (yyvsp[-1].value_list);
    }
#line 2582 "yacc_sql.cpp"
    break;

  case 79: /* values_list: LBRACE value_list RBRACE COMMA values_list  */
#line 765 "yacc_sql.y"
    {
      if ((yyvsp[0].values_list) != nullptr) {
        (yyval.values_list) = (yyvsp[0].values_list);
      } else {
        (yyval.values_list) = new std::vector<std::vector<Value>>;
      }

      (yyval.values_list)->emplace_back(move(*(yyvsp[-3].value_list)));
      delete (yyvsp[-3].value_list);
    }
#line 2597 "yacc_sql.cpp"
    break;

  case 80: /* value_list: expression  */
#line 778 "yacc_sql.y"
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
#line 2612 "yacc_sql.cpp"
    break;

  case 81: /* value_list: expression COMMA value_list  */
#line 788 "yacc_sql.y"
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
#line 2632 "yacc_sql.cpp"
    break;

  case 82: /* value: NUMBER  */
#line 805 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2641 "yacc_sql.cpp"
    break;

  case 83: /* value: FLOAT  */
#line 809 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2650 "yacc_sql.cpp"
    break;

  case 84: /* value: SSS  */
#line 813 "yacc_sql.y"
         {
      int len = strlen((yyvsp[0].string))-2;
      char *tmp = (char*)malloc(len+1);
      memcpy(tmp, (yyvsp[0].string)+1, len);
      tmp[len] = '\0';
      (yyval.value) = new Value(tmp, len);
      free(tmp);
      free((yyvsp[0].string));
      (yyloc) = (yylsp[0]);
    }
#line 2665 "yacc_sql.cpp"
    break;

  case 85: /* value: DATE_VALUE  */
#line 823 "yacc_sql.y"
                {
      int len = strlen((yyvsp[0].string))-2;
      char *tmp = (char*)malloc(len+1);
      memcpy(tmp, (yyvsp[0].string)+1, len);
      tmp[len] = '\0';
      (yyval.value) = new Value((Date*)tmp, len);
      free(tmp);
      free((yyvsp[0].string));
      (yyloc) = (yylsp[0]);
    }
#line 2680 "yacc_sql.cpp"
    break;

  case 86: /* value: NULL_T  */
#line 833 "yacc_sql.y"
            {
      (yyval.value) = new Value((void*)nullptr);
    }
#line 2688 "yacc_sql.cpp"
    break;

  case 87: /* value: LMBRACE value_list RMBRACE  */
#line 836 "yacc_sql.y"
                                 {
      (yyval.value) = new Value((yyvsp[-1].value_list));
      delete (yyvsp[-1].value_list);
    }
#line 2697 "yacc_sql.cpp"
    break;

  case 88: /* storage_format: %empty  */
#line 843 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2705 "yacc_sql.cpp"
    break;

  case 89: /* storage_format: STORAGE FORMAT EQ ID  */
#line 847 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2713 "yacc_sql.cpp"
    break;

  case 90: /* delete_stmt: DELETE FROM ID where  */
#line 854 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        swap((yyval.sql_node)->deletion.conditions, *(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2727 "yacc_sql.cpp"
    break;

  case 91: /* update_stmt: UPDATE ID SET key_values where  */
#line 866 "yacc_sql.y"
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
#line 2746 "yacc_sql.cpp"
    break;

  case 92: /* key_values: ID EQ assign_value  */
#line 884 "yacc_sql.y"
    {
      (yyval.key_values) = new Key_values;
      (yyval.key_values)->relation_list.emplace_back(std::string((yyvsp[-2].string)));
      free((yyvsp[-2].string));
      (yyval.key_values)->value_list.emplace_back(unique_ptr<Expression>((yyvsp[0].expression)));
    }
#line 2757 "yacc_sql.cpp"
    break;

  case 93: /* key_values: ID EQ assign_value COMMA key_values  */
#line 891 "yacc_sql.y"
    {
      if ((yyvsp[0].key_values) != nullptr) {
        (yyval.key_values) = (yyvsp[0].key_values);
      } else {
        (yyval.key_values) = new Key_values;
      }

      (yyval.key_values)->relation_list.emplace_back(std::string((yyvsp[-4].string)));
      free((yyvsp[-4].string));
      (yyval.key_values)->value_list.emplace_back(unique_ptr<Expression>((yyvsp[-2].expression)));
    }
#line 2773 "yacc_sql.cpp"
    break;

  case 94: /* assign_value: expression  */
#line 905 "yacc_sql.y"
               { (yyval.expression) = (yyvsp[0].expression); }
#line 2779 "yacc_sql.cpp"
    break;

  case 95: /* select_stmt: select_unit  */
#line 910 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
    }
#line 2787 "yacc_sql.cpp"
    break;

  case 96: /* select_stmt: select_unit UNION select_stmt  */
#line 914 "yacc_sql.y"
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
#line 2802 "yacc_sql.cpp"
    break;

  case 97: /* select_stmt: select_unit UNION ALL select_stmt  */
#line 925 "yacc_sql.y"
    {
      // 左结合：将当前 SELECT 和右边的 UNION ALL SELECT 合并
      if ((yyvsp[-3].sql_node) != nullptr && (yyvsp[0].sql_node) != nullptr) {
        (yyvsp[-3].sql_node)->selection.union_select = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
        (yyvsp[-3].sql_node)->selection.union_all = true;
        (yyval.sql_node) = (yyvsp[-3].sql_node);
      } else {
        (yyval.sql_node) = nullptr;
      }
    }
#line 2817 "yacc_sql.cpp"
    break;

  case 98: /* select_unit: SELECT expression_list FROM rel_list where group_by having_node order_by limit  */
#line 939 "yacc_sql.y"
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
#line 2865 "yacc_sql.cpp"
    break;

  case 99: /* limit: %empty  */
#line 986 "yacc_sql.y"
    {
      (yyval.number) = -1;
    }
#line 2873 "yacc_sql.cpp"
    break;

  case 100: /* limit: LIMIT number  */
#line 990 "yacc_sql.y"
    {
      (yyval.number) = (yyvsp[0].number);
    }
#line 2881 "yacc_sql.cpp"
    break;

  case 101: /* calc_stmt: CALC expression_list  */
#line 996 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2891 "yacc_sql.cpp"
    break;

  case 102: /* expression_list: expression alias  */
#line 1005 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      if((yyvsp[0].string) != nullptr){
        (yyvsp[-1].expression)->set_alias((yyvsp[0].string));
        free((yyvsp[0].string));
      }
      (yyval.expression_list)->emplace_back(move((yyvsp[-1].expression)));
    }
#line 2904 "yacc_sql.cpp"
    break;

  case 103: /* expression_list: expression alias COMMA expression_list  */
#line 1014 "yacc_sql.y"
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
#line 2921 "yacc_sql.cpp"
    break;

  case 104: /* expression: expression '+' expression  */
#line 1028 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2929 "yacc_sql.cpp"
    break;

  case 105: /* expression: expression '-' expression  */
#line 1031 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2937 "yacc_sql.cpp"
    break;

  case 106: /* expression: expression '*' expression  */
#line 1034 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2945 "yacc_sql.cpp"
    break;

  case 107: /* expression: expression '/' expression  */
#line 1037 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2953 "yacc_sql.cpp"
    break;

  case 108: /* expression: vector_operation LBRACE expression COMMA expression RBRACE  */
#line 1040 "yacc_sql.y"
                                                                {
      (yyval.expression) = create_operation_expression((VectorOperationExpr::Type)(yyvsp[-5].number), (yyvsp[-3].expression), (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2961 "yacc_sql.cpp"
    break;

  case 109: /* expression: LBRACE expression_list RBRACE  */
#line 1043 "yacc_sql.y"
                                    {
      if ((yyvsp[-1].expression_list)->size() == 1) {
        (yyval.expression) = ((yyvsp[-1].expression_list)->front()).release();
      } else {
        (yyval.expression) = new ValueListExpr(*(yyvsp[-1].expression_list));
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[-1].expression_list);
    }
#line 2975 "yacc_sql.cpp"
    break;

  case 110: /* expression: '-' expression  */
#line 1052 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2983 "yacc_sql.cpp"
    break;

  case 111: /* expression: rel_attr  */
#line 1055 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2994 "yacc_sql.cpp"
    break;

  case 112: /* expression: '*'  */
#line 1061 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 3002 "yacc_sql.cpp"
    break;

  case 113: /* expression: ID LBRACE expression_list RBRACE  */
#line 1064 "yacc_sql.y"
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
#line 3061 "yacc_sql.cpp"
    break;

  case 114: /* expression: ID LBRACE RBRACE  */
#line 1118 "yacc_sql.y"
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
#line 3078 "yacc_sql.cpp"
    break;

  case 115: /* expression: LBRACE select_stmt RBRACE  */
#line 1130 "yacc_sql.y"
                                {
      (yyval.expression) = new SelectExpr((yyvsp[-1].sql_node));
    }
#line 3086 "yacc_sql.cpp"
    break;

  case 116: /* expression: MATCH LBRACE rel_attr RBRACE AGAINST LBRACE SSS RBRACE  */
#line 1134 "yacc_sql.y"
    {
      // MATCH(field) AGAINST('query') as expression (for ORDER BY and SELECT)
      Expression *field_expr = new UnboundFieldExpr((yyvsp[-5].rel_attr)->relation_name, (yyvsp[-5].rel_attr)->attribute_name);
      Expression *query_expr = new ValueExpr(Value((yyvsp[-1].string)));
      (yyval.expression) = create_sysfunc_expression("match_against", field_expr, query_expr, nullptr, sql_string, &(yyloc));
      delete (yyvsp[-5].rel_attr);
      free((yyvsp[-1].string));
    }
#line 3099 "yacc_sql.cpp"
    break;

  case 117: /* ID: ID_KEY  */
#line 1146 "yacc_sql.y"
           { (yyval.string) = (yyvsp[0].string); }
#line 3105 "yacc_sql.cpp"
    break;

  case 118: /* ID: DATA  */
#line 1148 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 5);
      memcpy((yyval.string), "data", sizeof(char) * 5);
    }
#line 3114 "yacc_sql.cpp"
    break;

  case 119: /* ID: LISTS  */
#line 1153 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 6);
      memcpy((yyval.string), "lists", sizeof(char) * 6);
    }
#line 3123 "yacc_sql.cpp"
    break;

  case 120: /* ID: TYPE  */
#line 1158 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 5);
      memcpy((yyval.string), "type", sizeof(char) * 5);
    }
#line 3132 "yacc_sql.cpp"
    break;

  case 121: /* rel_attr: ID  */
#line 1165 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 3142 "yacc_sql.cpp"
    break;

  case 122: /* rel_attr: ID DOT ID  */
#line 1170 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3154 "yacc_sql.cpp"
    break;

  case 123: /* rel_attr: ID DOT '*'  */
#line 1177 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = '*';
      free((yyvsp[-2].string));
    }
#line 3165 "yacc_sql.cpp"
    break;

  case 124: /* relation: ID  */
#line 1186 "yacc_sql.y"
       {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3173 "yacc_sql.cpp"
    break;

  case 125: /* rel_list: relation alias join_list  */
#line 1191 "yacc_sql.y"
                             {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new Joins;
      }

      (yyval.join_list)->relation_list.emplace((yyval.join_list)->relation_list.begin(), std::string((yyvsp[-2].string)));
      free((yyvsp[-2].string));
      if((yyvsp[-1].string) != nullptr){
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), std::string((yyvsp[-1].string)));
        free((yyvsp[-1].string));
      } else {
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), std::string());
      }
    }
#line 3194 "yacc_sql.cpp"
    break;

  case 126: /* rel_list: relation alias join_list COMMA rel_list  */
#line 1207 "yacc_sql.y"
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

      (yyval.join_list)->relation_list.emplace((yyval.join_list)->relation_list.begin(), std::string((yyvsp[-4].string)));
      free((yyvsp[-4].string));
      if((yyvsp[-3].string) != nullptr){
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), std::string((yyvsp[-3].string)));
        free((yyvsp[-3].string));
      } else {
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), std::string());
      }

      delete (yyvsp[-2].join_list);
    }
#line 3228 "yacc_sql.cpp"
    break;

  case 127: /* join_list: %empty  */
#line 1240 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 3236 "yacc_sql.cpp"
    break;

  case 128: /* join_list: INNER JOIN relation alias on join_list  */
#line 1244 "yacc_sql.y"
    {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new Joins;
      }

      (yyval.join_list)->relation_list.emplace((yyval.join_list)->relation_list.begin(), std::string((yyvsp[-3].string)));
      free((yyvsp[-3].string));
      if((yyvsp[-2].string) != nullptr){
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), std::string((yyvsp[-2].string)));
        free((yyvsp[-2].string));
      } else {
        (yyval.join_list)->alias_list.emplace((yyval.join_list)->alias_list.begin(), std::string());
      }

      if((yyvsp[-1].condition_list) != nullptr){
        auto& conditions = (yyval.join_list)->condition_list.conditions;
        conditions.insert(conditions.end(), std::make_move_iterator((yyvsp[-1].condition_list)->conditions.begin()), 
          std::make_move_iterator((yyvsp[-1].condition_list)->conditions.end()));
        delete (yyvsp[-1].condition_list);
      }
    }
#line 3264 "yacc_sql.cpp"
    break;

  case 129: /* alias: %empty  */
#line 1270 "yacc_sql.y"
                {
      (yyval.string) = nullptr;
    }
#line 3272 "yacc_sql.cpp"
    break;

  case 130: /* alias: ID  */
#line 1273 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3280 "yacc_sql.cpp"
    break;

  case 131: /* alias: AS ID  */
#line 1276 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3288 "yacc_sql.cpp"
    break;

  case 132: /* on: %empty  */
#line 1283 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3296 "yacc_sql.cpp"
    break;

  case 133: /* on: ON condition_list  */
#line 1287 "yacc_sql.y"
    {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3304 "yacc_sql.cpp"
    break;

  case 134: /* where: %empty  */
#line 1295 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3312 "yacc_sql.cpp"
    break;

  case 135: /* where: WHERE condition_list  */
#line 1298 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3320 "yacc_sql.cpp"
    break;

  case 136: /* condition_list: condition  */
#line 1303 "yacc_sql.y"
              {
      (yyval.condition_list) = new Conditions;
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[0].condition)));
      delete (yyvsp[0].condition);
    }
#line 3330 "yacc_sql.cpp"
    break;

  case 137: /* condition_list: condition AND condition_list  */
#line 1308 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[-2].condition)));
      delete (yyvsp[-2].condition);
    }
#line 3340 "yacc_sql.cpp"
    break;

  case 138: /* condition_list: condition OR condition_list  */
#line 1313 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->and_or = true;
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[-2].condition)));
      delete (yyvsp[-2].condition);
    }
#line 3351 "yacc_sql.cpp"
    break;

  case 139: /* condition: expression comp_op expression  */
#line 1322 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = unique_ptr<Expression>((yyvsp[-2].expression));
      (yyval.condition)->right_expr = unique_ptr<Expression>((yyvsp[0].expression));
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3362 "yacc_sql.cpp"
    break;

  case 140: /* condition: MATCH LBRACE rel_attr RBRACE AGAINST LBRACE SSS RBRACE  */
#line 1329 "yacc_sql.y"
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
#line 3379 "yacc_sql.cpp"
    break;

  case 141: /* condition: unary_op expression  */
#line 1342 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = unique_ptr<Expression>(new ValueExpr(Value((void*)nullptr)));
      (yyval.condition)->right_expr = unique_ptr<Expression>((yyvsp[0].expression));
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3390 "yacc_sql.cpp"
    break;

  case 142: /* comp_op: LIKE  */
#line 1351 "yacc_sql.y"
         { (yyval.comp) = LIKE_OP; }
#line 3396 "yacc_sql.cpp"
    break;

  case 143: /* comp_op: NOT LIKE  */
#line 1352 "yacc_sql.y"
               { (yyval.comp) = NOT_LIKE; }
#line 3402 "yacc_sql.cpp"
    break;

  case 144: /* comp_op: EQ  */
#line 1353 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3408 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: LT  */
#line 1354 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3414 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: GT  */
#line 1355 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3420 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: LE  */
#line 1356 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3426 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: GE  */
#line 1357 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3432 "yacc_sql.cpp"
    break;

  case 149: /* comp_op: NE  */
#line 1358 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3438 "yacc_sql.cpp"
    break;

  case 150: /* comp_op: IN  */
#line 1359 "yacc_sql.y"
         { (yyval.comp) = IN_OP; }
#line 3444 "yacc_sql.cpp"
    break;

  case 151: /* comp_op: NOT IN  */
#line 1360 "yacc_sql.y"
             { (yyval.comp) = NOT_IN; }
#line 3450 "yacc_sql.cpp"
    break;

  case 152: /* comp_op: IS  */
#line 1361 "yacc_sql.y"
         { (yyval.comp) = IS_NULL; }
#line 3456 "yacc_sql.cpp"
    break;

  case 153: /* comp_op: IS NOT  */
#line 1362 "yacc_sql.y"
             { (yyval.comp) = IS_NOT_NULL; }
#line 3462 "yacc_sql.cpp"
    break;

  case 154: /* unary_op: EXISTS  */
#line 1366 "yacc_sql.y"
           { (yyval.comp) = EXISTS_OP; }
#line 3468 "yacc_sql.cpp"
    break;

  case 155: /* unary_op: NOT EXISTS  */
#line 1367 "yacc_sql.y"
                 { (yyval.comp) = NOT_EXISTS; }
#line 3474 "yacc_sql.cpp"
    break;

  case 156: /* group_by: %empty  */
#line 1373 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 3482 "yacc_sql.cpp"
    break;

  case 157: /* group_by: GROUP BY expression_list  */
#line 1377 "yacc_sql.y"
    {
      (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 3490 "yacc_sql.cpp"
    break;

  case 158: /* having_node: %empty  */
#line 1384 "yacc_sql.y"
    {
      (yyval.having_list) = nullptr;
    }
#line 3498 "yacc_sql.cpp"
    break;

  case 159: /* having_node: HAVING having_list  */
#line 1388 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
    }
#line 3506 "yacc_sql.cpp"
    break;

  case 160: /* having_list: having_unit  */
#line 1395 "yacc_sql.y"
    {
      (yyval.having_list) = new HavingNode;
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[0].expression)));
    }
#line 3515 "yacc_sql.cpp"
    break;

  case 161: /* having_list: having_unit OR having_list  */
#line 1400 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[-2].expression)));
      (yyval.having_list)->and_or = true;
    }
#line 3525 "yacc_sql.cpp"
    break;

  case 162: /* having_list: having_unit AND having_list  */
#line 1406 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[-2].expression)));
    }
#line 3534 "yacc_sql.cpp"
    break;

  case 163: /* having_unit: expression comp_op expression  */
#line 1414 "yacc_sql.y"
    {
      unique_ptr<Expression> left((yyvsp[-2].expression));
      unique_ptr<Expression> right((yyvsp[0].expression));
      (yyval.expression) = new ComparisonExpr((yyvsp[-1].comp), std::move(left), std::move(right));
    }
#line 3544 "yacc_sql.cpp"
    break;

  case 164: /* order_by: %empty  */
#line 1423 "yacc_sql.y"
    {
      (yyval.order_by_list) = nullptr;
    }
#line 3552 "yacc_sql.cpp"
    break;

  case 165: /* order_by: ORDER BY order_by_list  */
#line 1427 "yacc_sql.y"
    {
      (yyval.order_by_list) = (yyvsp[0].order_by_list);
    }
#line 3560 "yacc_sql.cpp"
    break;

  case 166: /* order_by_list: order_by_unit  */
#line 1434 "yacc_sql.y"
    {
      (yyval.order_by_list) = new std::vector<OrderByNode>;
      (yyval.order_by_list)->emplace_back(move(*(yyvsp[0].order_by_unit)));
      delete (yyvsp[0].order_by_unit);
    }
#line 3570 "yacc_sql.cpp"
    break;

  case 167: /* order_by_list: order_by_unit COMMA order_by_list  */
#line 1440 "yacc_sql.y"
    {
      if((yyvsp[0].order_by_list) != nullptr){
        (yyval.order_by_list) = (yyvsp[0].order_by_list);
      } else {
        (yyval.order_by_list) = new std::vector<OrderByNode>;
      }
      (yyval.order_by_list)->emplace((yyval.order_by_list)->begin(), move(*(yyvsp[-2].order_by_unit)));
      delete (yyvsp[-2].order_by_unit);
    }
#line 3584 "yacc_sql.cpp"
    break;

  case 168: /* order_by_unit: expression  */
#line 1453 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = true;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[0].expression));
    }
#line 3594 "yacc_sql.cpp"
    break;

  case 169: /* order_by_unit: expression ASC  */
#line 1459 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = true;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[-1].expression));
    }
#line 3604 "yacc_sql.cpp"
    break;

  case 170: /* order_by_unit: expression DESC  */
#line 1465 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = false;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[-1].expression));
    }
#line 3614 "yacc_sql.cpp"
    break;

  case 171: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1474 "yacc_sql.y"
    {
      int len = strlen((yyvsp[-3].string)) - 2;
      char *tmp_file_name = (char*)malloc(len+1);
      memcpy(tmp_file_name, (yyvsp[-3].string)+1, len);
      tmp_file_name[len] = '\0';

      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
      (yyloc) = (yylsp[-6]);
    }
#line 3632 "yacc_sql.cpp"
    break;

  case 172: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1491 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3641 "yacc_sql.cpp"
    break;

  case 173: /* set_variable_stmt: SET ID EQ value  */
#line 1499 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3653 "yacc_sql.cpp"
    break;


#line 3657 "yacc_sql.cpp"

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

#line 1511 "yacc_sql.y"

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
