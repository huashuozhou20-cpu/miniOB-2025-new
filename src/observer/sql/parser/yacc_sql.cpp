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
#define YYFINAL  88
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   503

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  101
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  176
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  364

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
    1052,  1055,  1061,  1064,  1118,  1130,  1133,  1142,  1152,  1153,
    1158,  1163,  1171,  1176,  1183,  1192,  1197,  1213,  1246,  1249,
    1276,  1279,  1282,  1289,  1292,  1301,  1304,  1309,  1314,  1319,
    1327,  1334,  1347,  1357,  1358,  1359,  1360,  1361,  1362,  1363,
    1364,  1365,  1366,  1367,  1368,  1372,  1373,  1379,  1382,  1390,
    1393,  1400,  1405,  1411,  1419,  1429,  1432,  1439,  1445,  1458,
    1464,  1470,  1479,  1496,  1504,  1514,  1515
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

#define YYPACT_NINF (-309)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-142)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     416,     9,    62,   -39,   322,   322,    13,  -309,   -24,    12,
     -39,  -309,  -309,  -309,  -309,  -309,   -39,    -6,   416,    58,
      76,    77,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,    17,  -309,  -309,  -309,  -309,   -39,   -39,    67,  -309,
      83,   108,   -39,   -39,  -309,  -309,  -309,  -309,  -309,   212,
     322,  -309,  -309,  -309,  -309,   102,  -309,  -309,  -309,  -309,
     322,  -309,   107,  -309,  -309,   237,    -5,  -309,    98,  -309,
     101,   -39,   -39,   105,    82,    90,  -309,   -39,  -309,  -309,
    -309,   -11,     0,    14,   -39,   -39,   -39,  -309,   122,   149,
     151,   157,    36,   -39,  -309,   322,   -39,   322,   322,   322,
     322,  -309,   161,   303,    61,   -39,   -39,   166,   140,   -39,
      88,    97,     8,   180,  -309,   -39,  -309,   180,   -39,   180,
     152,   154,   155,   -39,  -309,  -309,  -309,   322,   159,   179,
      40,  -309,    59,    59,  -309,  -309,   322,  -309,   185,  -309,
    -309,  -309,   123,   140,  -309,   -39,   168,   258,  -309,   140,
     146,  -309,   171,   139,   141,    51,  -309,   191,   208,  -309,
     192,  -309,   -39,   -39,   -39,  -309,  -309,   131,   322,  -309,
    -309,   177,   218,   192,   203,  -309,   174,   204,   404,  -309,
     115,   322,  -309,   322,   220,   -39,   -39,   -39,   -39,   -39,
     209,  -309,  -309,  -309,  -309,  -309,  -309,  -309,     5,   -30,
     -39,   210,   213,   223,   224,   225,    -2,   186,   222,   247,
     243,   231,   322,  -309,  -309,   -39,  -309,  -309,   197,   -21,
    -309,  -309,  -309,  -309,  -309,  -309,   322,   258,   258,   -57,
     232,   -57,   -39,  -309,  -309,   183,  -309,   191,    -1,   178,
    -309,   214,  -309,  -309,   192,   219,   -39,   -39,   -39,   182,
    -309,   -39,   -39,   322,   322,   265,  -309,   252,   253,  -309,
    -309,  -309,   -57,  -309,  -309,   -39,  -309,   -39,  -309,   215,
     180,  -309,  -309,   255,  -309,  -309,   180,   192,   192,   192,
     256,   123,  -309,  -309,   404,  -309,   117,   279,   221,   257,
     200,  -309,  -309,   230,  -309,   -30,  -309,   262,   274,   275,
    -309,   251,   322,   322,   322,   322,   178,  -309,   203,   284,
     -39,  -309,   291,  -309,  -309,   258,   177,   -57,  -309,  -309,
      22,  -309,   285,  -309,  -309,   226,  -309,   292,  -309,  -309,
    -309,  -309,   322,   294,   235,  -309,   153,   259,   236,  -309,
     298,   248,   271,    23,   310,   246,   276,   254,   326,   273,
     295,   178,   333,  -309
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    51,     0,     0,     0,     0,     0,    28,     0,     0,
       0,    29,    30,    31,    27,    26,     0,     0,     0,     0,
       0,   175,    25,    24,    17,    18,    19,    20,     9,    10,
      12,    13,    16,    14,    15,    11,     8,     5,     7,     6,
       4,    95,     3,    21,    22,    23,     0,     0,     0,    52,
       0,     0,     0,     0,   119,   120,   121,   118,    35,     0,
       0,    86,    43,    44,    45,     0,    82,    83,    84,    85,
       0,   112,     0,   117,   101,   130,   122,   111,     0,    34,
       0,     0,     0,     0,     0,     0,   173,     0,     1,   176,
       2,     0,    59,     0,     0,     0,     0,    32,     0,     0,
       0,     0,    80,     0,   110,     0,     0,     0,     0,     0,
       0,   131,   102,     0,     0,     0,     0,    49,   135,     0,
       0,     0,     0,     0,    96,     0,    60,     0,     0,     0,
       0,     0,     0,     0,   115,   109,    87,     0,   122,     0,
       0,   132,   104,   105,   106,   107,     0,   114,     0,   124,
     123,   125,   130,   135,    33,     0,     0,     0,    90,   135,
       0,   174,     0,     0,     0,     0,    97,    61,     0,    58,
      47,    54,     0,     0,     0,    53,    81,     0,     0,   103,
     113,   128,   157,    47,     0,   155,     0,     0,     0,   136,
     137,     0,    91,     0,     0,     0,     0,     0,     0,     0,
       0,    70,    71,    72,    76,    73,    74,    75,    66,    66,
       0,     0,     0,     0,     0,     0,     0,     0,   126,     0,
     159,     0,     0,    77,   156,     0,   143,   151,   153,     0,
     145,   146,   147,   148,   149,   150,     0,     0,     0,   142,
      92,    94,     0,    37,    36,     0,    39,    61,    88,     0,
      67,     0,    64,    65,    47,     0,     0,     0,     0,     0,
     108,     0,     0,     0,     0,   165,    50,     0,     0,   154,
     144,   152,   140,   138,   139,     0,   172,     0,    62,     0,
       0,    56,    69,     0,    68,    48,     0,    47,    47,    47,
       0,   130,   127,   158,     0,   160,   161,     0,    99,    78,
       0,    93,    38,     0,    57,    66,    55,     0,     0,     0,
     116,   133,     0,     0,     0,     0,     0,    98,     0,     0,
       0,    63,     0,    41,    40,     0,   128,   164,   163,   162,
     169,   166,   167,   100,    79,     0,    89,     0,   134,   129,
     170,   171,     0,     0,     0,   168,   116,     0,     0,    46,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    42
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -309,  -309,   344,  -309,  -309,  -309,  -309,  -309,  -309,  -309,
    -309,  -309,  -309,  -309,  -309,    10,  -309,  -179,  -309,  -309,
    -309,  -309,  -309,   116,   118,  -110,  -204,  -308,  -309,  -309,
    -309,    48,  -120,   249,  -309,  -309,  -309,    92,  -309,   -56,
    -309,  -309,  -309,     4,   -50,    -3,  -101,   109,   106,    45,
    -151,  -309,  -128,  -226,  -309,    78,  -309,  -309,  -309,  -146,
    -309,  -309,    31,  -309,  -309,  -309,  -309,  -309
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    72,   350,   211,   156,    51,
      34,    35,    36,   127,   200,   167,   252,   283,   208,   209,
      37,   223,   101,    73,   281,    38,    39,   159,   240,    40,
      41,   317,    42,    74,    75,    76,    77,   152,   153,   218,
     112,   326,   158,   189,   190,   236,   191,   220,   265,   295,
     296,   298,   331,   332,    43,    44,    45,    90
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   181,   139,    99,   221,   253,     5,    83,   333,    78,
     102,   273,   274,    84,   163,    54,   -59,   176,    81,   113,
     104,    46,    47,   260,   125,   182,   250,    79,    80,   249,
     251,   192,   340,   341,   270,   124,   114,   271,   128,   107,
     108,   109,   110,    92,    93,    48,    55,    56,    85,    97,
      98,   126,   126,   362,    57,   140,    82,   142,   143,   144,
     145,   250,   279,   100,   137,   251,   129,   166,   178,   123,
      87,   169,   111,   171,    52,   285,    88,    53,   117,   118,
      89,    49,    94,   164,   122,   165,   244,   102,    50,   247,
      91,   130,   131,   132,   107,   108,   109,   110,    95,   338,
     138,   321,   267,   141,    62,    63,    64,   188,   307,   308,
     309,   150,   151,   154,    60,    54,   160,   148,   107,   108,
     109,   110,   168,    96,   268,   170,   103,   197,   216,   198,
     175,   105,   107,   108,   109,   110,   107,   108,   109,   110,
     311,   239,   115,   241,    61,   116,    55,    56,   120,   111,
     179,   121,   183,  -141,    57,   119,  -141,   109,   110,   149,
    -141,  -141,  -141,   237,   238,   313,   314,   328,   329,   212,
     213,   214,   102,   133,   134,   106,   135,    54,  -141,    66,
      67,  -141,    68,    69,   136,   157,   272,   188,   188,   146,
     155,   162,   243,   168,   245,   246,   168,     5,  -141,  -141,
     114,  -141,  -141,   172,   177,   173,   174,   254,    55,    56,
     180,   184,   193,   194,   294,   195,    57,   196,  -141,   199,
     210,   215,   138,   217,   304,   219,  -141,   222,   225,     5,
     306,   224,   242,   261,   248,   255,    59,   256,    60,   276,
     201,   202,   203,   204,   205,   206,   207,   257,   258,   259,
     262,   263,   264,   287,   288,   289,   266,   269,   151,   151,
     275,   277,   327,   294,   294,   330,    54,   293,    61,   282,
     284,   286,   160,   297,   302,   188,   290,   299,   300,   303,
     305,   310,    59,   315,    60,   318,   316,   322,   111,   106,
     319,    54,   330,    62,    63,    64,   320,    55,    56,   323,
     324,    65,   325,    66,    67,    57,    68,    69,   335,    70,
      71,   337,    54,   342,    61,   185,   344,   336,   186,   346,
     343,   347,    55,    56,   349,   348,   351,    59,   147,    60,
      57,   356,   352,   107,   108,   109,   110,   353,   355,    62,
      63,    64,   357,    55,    56,   358,    59,   187,    60,    66,
      67,    57,    68,    69,   359,    70,    71,    54,   363,    61,
     360,   361,    86,   354,   280,   278,   334,   301,   292,   161,
     291,   339,   312,   345,     0,     0,    54,     0,    61,     0,
       0,     0,     0,     0,    62,    63,    64,     0,    55,    56,
       0,     0,    65,     0,    66,    67,    57,    68,    69,     0,
      70,    71,     0,    62,    63,    64,     0,    55,    56,     0,
       0,    65,     0,    66,    67,    57,    68,    69,     0,    70,
      71,     1,     2,     0,     0,     0,     0,     3,     0,     0,
       0,     0,     4,     5,     6,     7,     0,     8,     9,    10,
       0,     0,     0,     0,     0,    11,    12,    13,     0,     0,
       0,     0,     0,     0,     0,    14,    15,     0,     0,   226,
       0,     0,   227,   228,   229,     0,    16,     0,     0,    17,
     230,   231,   232,   233,   234,   235,     0,     0,    18,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     107,   108,   109,   110
};

static const yytype_int16 yycheck[] =
{
       3,   152,   103,    59,   183,   209,    17,    10,   316,     5,
      60,   237,   238,    16,     6,    54,    17,   137,    42,    24,
      70,    12,    13,    25,    24,   153,    56,    14,    15,    24,
      60,   159,    10,    11,    55,    91,    41,    58,    24,    96,
      97,    98,    99,    46,    47,    36,    85,    86,    54,    52,
      53,    52,    52,   361,    93,   105,    44,   107,   108,   109,
     110,    56,    63,    59,    28,    60,    52,   123,    28,    80,
      12,   127,    75,   129,    12,   254,     0,    15,    81,    82,
       3,    72,    15,    75,    87,    77,   196,   137,    79,   199,
      73,    94,    95,    96,    96,    97,    98,    99,    15,   325,
     103,   305,   222,   106,    81,    82,    83,   157,   287,   288,
     289,   114,   115,   116,    26,    54,   119,   113,    96,    97,
      98,    99,   125,    15,   225,   128,    24,    76,   178,    78,
     133,    24,    96,    97,    98,    99,    96,    97,    98,    99,
     291,   191,    44,   193,    56,    44,    85,    86,    66,   152,
     146,    61,   155,     0,    93,    50,     3,    98,    99,    98,
       7,     8,     9,    48,    49,    48,    49,   313,   314,   172,
     173,   174,   222,    51,    25,    52,    25,    54,    25,    91,
      92,    28,    94,    95,    27,    45,   236,   237,   238,    28,
      24,    94,   195,   196,   197,   198,   199,    17,    45,    46,
      41,    48,    49,    51,    25,    51,    51,   210,    85,    86,
      25,    43,    66,    42,   264,    76,    93,    76,    65,    28,
      28,    90,   225,    46,   280,     7,    73,    24,    24,    17,
     286,    57,    12,    47,    25,    25,    24,    24,    26,   242,
      32,    33,    34,    35,    36,    37,    38,    24,    24,    24,
      28,     4,     9,   256,   257,   258,    25,    60,   261,   262,
      28,    78,   312,   313,   314,   315,    54,   263,    56,    91,
      56,    52,   275,     8,   277,   325,    94,    25,    25,    64,
      25,    25,    24,     4,    26,    28,    65,    25,   291,    52,
      90,    54,   342,    81,    82,    83,    66,    85,    86,    25,
      25,    89,    51,    91,    92,    93,    94,    95,    24,    97,
      98,    20,    54,    28,    56,    57,    24,   320,    60,    25,
      94,    86,    85,    86,    88,    66,    28,    24,    25,    26,
      93,    85,    84,    96,    97,    98,    99,    66,    28,    81,
      82,    83,    66,    85,    86,    91,    24,    89,    26,    91,
      92,    93,    94,    95,    28,    97,    98,    54,    25,    56,
      87,    66,    18,   353,   248,   247,   318,   275,   262,   120,
     261,   326,   294,   342,    -1,    -1,    54,    -1,    56,    -1,
      -1,    -1,    -1,    -1,    81,    82,    83,    -1,    85,    86,
      -1,    -1,    89,    -1,    91,    92,    93,    94,    95,    -1,
      97,    98,    -1,    81,    82,    83,    -1,    85,    86,    -1,
      -1,    89,    -1,    91,    92,    93,    94,    95,    -1,    97,
      98,     5,     6,    -1,    -1,    -1,    -1,    11,    -1,    -1,
      -1,    -1,    16,    17,    18,    19,    -1,    21,    22,    23,
      -1,    -1,    -1,    -1,    -1,    29,    30,    31,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    39,    40,    -1,    -1,    55,
      -1,    -1,    58,    59,    60,    -1,    50,    -1,    -1,    53,
      66,    67,    68,    69,    70,    71,    -1,    -1,    62,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      74,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
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
      26,    56,    81,    82,    83,    89,    91,    92,    94,    95,
      97,    98,   116,   134,   144,   145,   146,   147,   144,    14,
      15,    42,    44,   146,   146,    54,   103,    12,     0,     3,
     168,    73,   146,   146,    15,    15,    15,   146,   146,   140,
     144,   133,   145,    24,   145,    24,    52,    96,    97,    98,
      99,   146,   151,    24,    41,    44,    44,   146,   146,    50,
      66,    61,   146,    80,   140,    24,    52,   124,    24,    52,
     146,   146,   146,    51,    25,    25,    27,    28,   146,   147,
     145,   146,   145,   145,   145,   145,    28,    25,   144,    98,
     146,   146,   148,   149,   146,    24,   119,    45,   153,   138,
     146,   134,    94,     6,    75,    77,   140,   126,   146,   140,
     146,   140,    51,    51,    51,   146,   133,    25,    28,   144,
      25,   151,   153,   146,    43,    57,    60,    89,   145,   154,
     155,   157,   153,    66,    42,    76,    76,    76,    78,    28,
     125,    32,    33,    34,    35,    36,    37,    38,   129,   130,
      28,   118,   146,   146,   146,    90,   145,    46,   150,     7,
     158,   118,    24,   132,    57,    24,    55,    58,    59,    60,
      66,    67,    68,    69,    70,    71,   156,    48,    49,   145,
     139,   145,    12,   146,   126,   146,   146,   126,    25,    24,
      56,    60,   127,   127,   146,    25,    24,    24,    24,    24,
      25,    47,    28,     4,     9,   159,    25,   133,   147,    60,
      55,    58,   145,   154,   154,    28,   146,    78,   125,    63,
     124,   135,    91,   128,    56,   118,    52,   146,   146,   146,
      94,   148,   149,   144,   145,   160,   161,     8,   162,    25,
      25,   138,   146,    64,   140,    25,   140,   118,   118,   118,
      25,   151,   156,    48,    49,     4,    65,   142,    28,    90,
      66,   127,    25,    25,    25,    51,   152,   145,   160,   160,
     145,   163,   164,   128,   132,    24,   146,    20,   154,   150,
      10,    11,    28,    94,    24,   163,    25,    86,    66,    88,
     117,    28,    84,    66,   116,    28,    85,    66,    91,    28,
      87,    66,   128,    25
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
     145,   145,   145,   145,   145,   145,   145,   145,   146,   146,
     146,   146,   147,   147,   147,   148,   149,   149,   150,   150,
     151,   151,   151,   152,   152,   153,   153,   154,   154,   154,
     155,   155,   155,   156,   156,   156,   156,   156,   156,   156,
     156,   156,   156,   156,   156,   157,   157,   158,   158,   159,
     159,   160,   160,   160,   161,   162,   162,   163,   163,   164,
     164,   164,   165,   166,   167,   168,   168
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
       1,     1,     1,     3,     3,     1,     3,     5,     0,     6,
       0,     1,     2,     0,     2,     0,     2,     1,     3,     3,
       3,     8,     2,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     2,     1,     2,     1,     2,     0,     3,     0,
       2,     1,     3,     3,     3,     0,     3,     1,     3,     1,
       2,     2,     7,     2,     4,     0,     1
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
#line 2012 "yacc_sql.cpp"
    break;

  case 26: /* exit_stmt: EXIT  */
#line 338 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 2021 "yacc_sql.cpp"
    break;

  case 27: /* help_stmt: HELP  */
#line 344 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 2029 "yacc_sql.cpp"
    break;

  case 28: /* sync_stmt: SYNC  */
#line 349 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 2037 "yacc_sql.cpp"
    break;

  case 29: /* begin_stmt: TRX_BEGIN  */
#line 355 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 2045 "yacc_sql.cpp"
    break;

  case 30: /* commit_stmt: TRX_COMMIT  */
#line 361 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 2053 "yacc_sql.cpp"
    break;

  case 31: /* rollback_stmt: TRX_ROLLBACK  */
#line 367 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 2061 "yacc_sql.cpp"
    break;

  case 32: /* drop_table_stmt: DROP TABLE ID  */
#line 373 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2071 "yacc_sql.cpp"
    break;

  case 33: /* show_index_stmt: SHOW INDEX FROM ID  */
#line 380 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_INDEX);
      (yyval.sql_node)->show_index.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2081 "yacc_sql.cpp"
    break;

  case 34: /* show_tables_stmt: SHOW TABLES  */
#line 387 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 2089 "yacc_sql.cpp"
    break;

  case 35: /* desc_table_stmt: DESC ID  */
#line 393 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2099 "yacc_sql.cpp"
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
#line 2113 "yacc_sql.cpp"
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
#line 2127 "yacc_sql.cpp"
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
#line 2143 "yacc_sql.cpp"
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
#line 2157 "yacc_sql.cpp"
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
#line 2180 "yacc_sql.cpp"
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
#line 2203 "yacc_sql.cpp"
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
#line 2230 "yacc_sql.cpp"
    break;

  case 43: /* vector_operation: L2_DISTANCE  */
#line 513 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::L2_DISTANCE;
    }
#line 2238 "yacc_sql.cpp"
    break;

  case 44: /* vector_operation: COSINE_DISTANCE  */
#line 517 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::COSINE_DISTANCE;
    }
#line 2246 "yacc_sql.cpp"
    break;

  case 45: /* vector_operation: INNER_PRODUCT  */
#line 521 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::INNER_PRODUCT;
    }
#line 2254 "yacc_sql.cpp"
    break;

  case 46: /* vector_index_type: IVFFLAT  */
#line 527 "yacc_sql.y"
            { (yyval.vector_index_type) = VectorIndexType::IVFFLAT; }
#line 2260 "yacc_sql.cpp"
    break;

  case 47: /* idx_col_list: %empty  */
#line 531 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2268 "yacc_sql.cpp"
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
#line 2282 "yacc_sql.cpp"
    break;

  case 49: /* col_list: %empty  */
#line 548 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2290 "yacc_sql.cpp"
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
#line 2304 "yacc_sql.cpp"
    break;

  case 51: /* unique_option: %empty  */
#line 564 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2312 "yacc_sql.cpp"
    break;

  case 52: /* unique_option: UNIQUE  */
#line 568 "yacc_sql.y"
    {
      (yyval.boolean) = true;
    }
#line 2320 "yacc_sql.cpp"
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
#line 2332 "yacc_sql.cpp"
    break;

  case 54: /* create_view_stmt: CREATE VIEW ID AS select_stmt  */
#line 584 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_VIEW;
      (yyval.sql_node)->create_view.view_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2343 "yacc_sql.cpp"
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
#line 2363 "yacc_sql.cpp"
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
#line 2388 "yacc_sql.cpp"
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
#line 2410 "yacc_sql.cpp"
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
#line 2422 "yacc_sql.cpp"
    break;

  case 59: /* as_option: %empty  */
#line 660 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2430 "yacc_sql.cpp"
    break;

  case 60: /* as_option: AS  */
#line 663 "yacc_sql.y"
         { (yyval.boolean) = true; }
#line 2436 "yacc_sql.cpp"
    break;

  case 61: /* attr_def_list: %empty  */
#line 668 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2444 "yacc_sql.cpp"
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
#line 2458 "yacc_sql.cpp"
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
#line 2471 "yacc_sql.cpp"
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
#line 2484 "yacc_sql.cpp"
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
#line 2497 "yacc_sql.cpp"
    break;

  case 66: /* nullable: %empty  */
#line 715 "yacc_sql.y"
    { (yyval.boolean) = true; }
#line 2503 "yacc_sql.cpp"
    break;

  case 67: /* nullable: NULL_T  */
#line 716 "yacc_sql.y"
             {(yyval.boolean) = true;}
#line 2509 "yacc_sql.cpp"
    break;

  case 68: /* nullable: NOT NULL_T  */
#line 717 "yacc_sql.y"
                 {(yyval.boolean) = false;}
#line 2515 "yacc_sql.cpp"
    break;

  case 69: /* number: NUMBER  */
#line 721 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2521 "yacc_sql.cpp"
    break;

  case 70: /* type: INT_T  */
#line 724 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2527 "yacc_sql.cpp"
    break;

  case 71: /* type: STRING_T  */
#line 725 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2533 "yacc_sql.cpp"
    break;

  case 72: /* type: FLOAT_T  */
#line 726 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2539 "yacc_sql.cpp"
    break;

  case 73: /* type: VECTOR_T  */
#line 727 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2545 "yacc_sql.cpp"
    break;

  case 74: /* type: VECTOR  */
#line 728 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2551 "yacc_sql.cpp"
    break;

  case 75: /* type: TEXT_T  */
#line 729 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::TEXTS);}
#line 2557 "yacc_sql.cpp"
    break;

  case 76: /* date_type: DATE_T  */
#line 732 "yacc_sql.y"
             {(yyval.number) = static_cast<int>(AttrType::DATES);}
#line 2563 "yacc_sql.cpp"
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
#line 2586 "yacc_sql.cpp"
    break;

  case 78: /* values_list: LBRACE value_list RBRACE  */
#line 759 "yacc_sql.y"
    {
      (yyval.values_list) = new std::vector<std::vector<Value>>;
      (yyval.values_list)->emplace_back(move(*(yyvsp[-1].value_list)));
      delete (yyvsp[-1].value_list);
    }
#line 2596 "yacc_sql.cpp"
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
#line 2611 "yacc_sql.cpp"
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
#line 2626 "yacc_sql.cpp"
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
#line 2646 "yacc_sql.cpp"
    break;

  case 82: /* value: NUMBER  */
#line 805 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2655 "yacc_sql.cpp"
    break;

  case 83: /* value: FLOAT  */
#line 809 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2664 "yacc_sql.cpp"
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
#line 2679 "yacc_sql.cpp"
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
#line 2694 "yacc_sql.cpp"
    break;

  case 86: /* value: NULL_T  */
#line 833 "yacc_sql.y"
            {
      (yyval.value) = new Value((void*)nullptr);
    }
#line 2702 "yacc_sql.cpp"
    break;

  case 87: /* value: LMBRACE value_list RMBRACE  */
#line 836 "yacc_sql.y"
                                 {
      (yyval.value) = new Value((yyvsp[-1].value_list));
      delete (yyvsp[-1].value_list);
    }
#line 2711 "yacc_sql.cpp"
    break;

  case 88: /* storage_format: %empty  */
#line 843 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2719 "yacc_sql.cpp"
    break;

  case 89: /* storage_format: STORAGE FORMAT EQ ID  */
#line 847 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2727 "yacc_sql.cpp"
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
#line 2741 "yacc_sql.cpp"
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
#line 2760 "yacc_sql.cpp"
    break;

  case 92: /* key_values: ID EQ assign_value  */
#line 884 "yacc_sql.y"
    {
      (yyval.key_values) = new Key_values;
      (yyval.key_values)->relation_list.emplace_back(std::string((yyvsp[-2].string)));
      free((yyvsp[-2].string));
      (yyval.key_values)->value_list.emplace_back(unique_ptr<Expression>((yyvsp[0].expression)));
    }
#line 2771 "yacc_sql.cpp"
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
#line 2787 "yacc_sql.cpp"
    break;

  case 94: /* assign_value: expression  */
#line 905 "yacc_sql.y"
               { (yyval.expression) = (yyvsp[0].expression); }
#line 2793 "yacc_sql.cpp"
    break;

  case 95: /* select_stmt: select_unit  */
#line 910 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
    }
#line 2801 "yacc_sql.cpp"
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
#line 2816 "yacc_sql.cpp"
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
#line 2831 "yacc_sql.cpp"
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
#line 2879 "yacc_sql.cpp"
    break;

  case 99: /* limit: %empty  */
#line 986 "yacc_sql.y"
    {
      (yyval.number) = -1;
    }
#line 2887 "yacc_sql.cpp"
    break;

  case 100: /* limit: LIMIT number  */
#line 990 "yacc_sql.y"
    {
      (yyval.number) = (yyvsp[0].number);
    }
#line 2895 "yacc_sql.cpp"
    break;

  case 101: /* calc_stmt: CALC expression_list  */
#line 996 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2905 "yacc_sql.cpp"
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
#line 2918 "yacc_sql.cpp"
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
#line 2935 "yacc_sql.cpp"
    break;

  case 104: /* expression: expression '+' expression  */
#line 1028 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2943 "yacc_sql.cpp"
    break;

  case 105: /* expression: expression '-' expression  */
#line 1031 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2951 "yacc_sql.cpp"
    break;

  case 106: /* expression: expression '*' expression  */
#line 1034 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2959 "yacc_sql.cpp"
    break;

  case 107: /* expression: expression '/' expression  */
#line 1037 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2967 "yacc_sql.cpp"
    break;

  case 108: /* expression: vector_operation LBRACE expression COMMA expression RBRACE  */
#line 1040 "yacc_sql.y"
                                                                {
      (yyval.expression) = create_operation_expression((VectorOperationExpr::Type)(yyvsp[-5].number), (yyvsp[-3].expression), (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2975 "yacc_sql.cpp"
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
#line 2989 "yacc_sql.cpp"
    break;

  case 110: /* expression: '-' expression  */
#line 1052 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2997 "yacc_sql.cpp"
    break;

  case 111: /* expression: rel_attr  */
#line 1055 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 3008 "yacc_sql.cpp"
    break;

  case 112: /* expression: '*'  */
#line 1061 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 3016 "yacc_sql.cpp"
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
#line 3075 "yacc_sql.cpp"
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
#line 3092 "yacc_sql.cpp"
    break;

  case 115: /* expression: LBRACE select_stmt RBRACE  */
#line 1130 "yacc_sql.y"
                                {
      (yyval.expression) = new SelectExpr((yyvsp[-1].sql_node));
    }
#line 3100 "yacc_sql.cpp"
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
#line 3113 "yacc_sql.cpp"
    break;

  case 117: /* expression: value  */
#line 1142 "yacc_sql.y"
            {
      // Allow value to be used as expression (for WHERE conditions, etc.)
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 3124 "yacc_sql.cpp"
    break;

  case 118: /* ID: ID_KEY  */
#line 1152 "yacc_sql.y"
           { (yyval.string) = (yyvsp[0].string); }
#line 3130 "yacc_sql.cpp"
    break;

  case 119: /* ID: DATA  */
#line 1154 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 5);
      memcpy((yyval.string), "data", sizeof(char) * 5);
    }
#line 3139 "yacc_sql.cpp"
    break;

  case 120: /* ID: LISTS  */
#line 1159 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 6);
      memcpy((yyval.string), "lists", sizeof(char) * 6);
    }
#line 3148 "yacc_sql.cpp"
    break;

  case 121: /* ID: TYPE  */
#line 1164 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 5);
      memcpy((yyval.string), "type", sizeof(char) * 5);
    }
#line 3157 "yacc_sql.cpp"
    break;

  case 122: /* rel_attr: ID  */
#line 1171 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 3167 "yacc_sql.cpp"
    break;

  case 123: /* rel_attr: ID DOT ID  */
#line 1176 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3179 "yacc_sql.cpp"
    break;

  case 124: /* rel_attr: ID DOT '*'  */
#line 1183 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = '*';
      free((yyvsp[-2].string));
    }
#line 3190 "yacc_sql.cpp"
    break;

  case 125: /* relation: ID  */
#line 1192 "yacc_sql.y"
       {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3198 "yacc_sql.cpp"
    break;

  case 126: /* rel_list: relation alias join_list  */
#line 1197 "yacc_sql.y"
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
#line 3219 "yacc_sql.cpp"
    break;

  case 127: /* rel_list: relation alias join_list COMMA rel_list  */
#line 1213 "yacc_sql.y"
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
#line 3253 "yacc_sql.cpp"
    break;

  case 128: /* join_list: %empty  */
#line 1246 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 3261 "yacc_sql.cpp"
    break;

  case 129: /* join_list: INNER JOIN relation alias on join_list  */
#line 1250 "yacc_sql.y"
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
#line 3289 "yacc_sql.cpp"
    break;

  case 130: /* alias: %empty  */
#line 1276 "yacc_sql.y"
                {
      (yyval.string) = nullptr;
    }
#line 3297 "yacc_sql.cpp"
    break;

  case 131: /* alias: ID  */
#line 1279 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3305 "yacc_sql.cpp"
    break;

  case 132: /* alias: AS ID  */
#line 1282 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3313 "yacc_sql.cpp"
    break;

  case 133: /* on: %empty  */
#line 1289 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3321 "yacc_sql.cpp"
    break;

  case 134: /* on: ON condition_list  */
#line 1293 "yacc_sql.y"
    {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3329 "yacc_sql.cpp"
    break;

  case 135: /* where: %empty  */
#line 1301 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3337 "yacc_sql.cpp"
    break;

  case 136: /* where: WHERE condition_list  */
#line 1304 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3345 "yacc_sql.cpp"
    break;

  case 137: /* condition_list: condition  */
#line 1309 "yacc_sql.y"
              {
      (yyval.condition_list) = new Conditions;
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[0].condition)));
      delete (yyvsp[0].condition);
    }
#line 3355 "yacc_sql.cpp"
    break;

  case 138: /* condition_list: condition AND condition_list  */
#line 1314 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[-2].condition)));
      delete (yyvsp[-2].condition);
    }
#line 3365 "yacc_sql.cpp"
    break;

  case 139: /* condition_list: condition OR condition_list  */
#line 1319 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->and_or = true;
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[-2].condition)));
      delete (yyvsp[-2].condition);
    }
#line 3376 "yacc_sql.cpp"
    break;

  case 140: /* condition: expression comp_op expression  */
#line 1328 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = unique_ptr<Expression>((yyvsp[-2].expression));
      (yyval.condition)->right_expr = unique_ptr<Expression>((yyvsp[0].expression));
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3387 "yacc_sql.cpp"
    break;

  case 141: /* condition: MATCH LBRACE rel_attr RBRACE AGAINST LBRACE SSS RBRACE  */
#line 1335 "yacc_sql.y"
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
#line 3404 "yacc_sql.cpp"
    break;

  case 142: /* condition: unary_op expression  */
#line 1348 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = unique_ptr<Expression>(new ValueExpr(Value((void*)nullptr)));
      (yyval.condition)->right_expr = unique_ptr<Expression>((yyvsp[0].expression));
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3415 "yacc_sql.cpp"
    break;

  case 143: /* comp_op: LIKE  */
#line 1357 "yacc_sql.y"
         { (yyval.comp) = LIKE_OP; }
#line 3421 "yacc_sql.cpp"
    break;

  case 144: /* comp_op: NOT LIKE  */
#line 1358 "yacc_sql.y"
               { (yyval.comp) = NOT_LIKE; }
#line 3427 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: EQ  */
#line 1359 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3433 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: LT  */
#line 1360 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3439 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: GT  */
#line 1361 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3445 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: LE  */
#line 1362 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3451 "yacc_sql.cpp"
    break;

  case 149: /* comp_op: GE  */
#line 1363 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3457 "yacc_sql.cpp"
    break;

  case 150: /* comp_op: NE  */
#line 1364 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3463 "yacc_sql.cpp"
    break;

  case 151: /* comp_op: IN  */
#line 1365 "yacc_sql.y"
         { (yyval.comp) = IN_OP; }
#line 3469 "yacc_sql.cpp"
    break;

  case 152: /* comp_op: NOT IN  */
#line 1366 "yacc_sql.y"
             { (yyval.comp) = NOT_IN; }
#line 3475 "yacc_sql.cpp"
    break;

  case 153: /* comp_op: IS  */
#line 1367 "yacc_sql.y"
         { (yyval.comp) = IS_NULL; }
#line 3481 "yacc_sql.cpp"
    break;

  case 154: /* comp_op: IS NOT  */
#line 1368 "yacc_sql.y"
             { (yyval.comp) = IS_NOT_NULL; }
#line 3487 "yacc_sql.cpp"
    break;

  case 155: /* unary_op: EXISTS  */
#line 1372 "yacc_sql.y"
           { (yyval.comp) = EXISTS_OP; }
#line 3493 "yacc_sql.cpp"
    break;

  case 156: /* unary_op: NOT EXISTS  */
#line 1373 "yacc_sql.y"
                 { (yyval.comp) = NOT_EXISTS; }
#line 3499 "yacc_sql.cpp"
    break;

  case 157: /* group_by: %empty  */
#line 1379 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 3507 "yacc_sql.cpp"
    break;

  case 158: /* group_by: GROUP BY expression_list  */
#line 1383 "yacc_sql.y"
    {
      (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 3515 "yacc_sql.cpp"
    break;

  case 159: /* having_node: %empty  */
#line 1390 "yacc_sql.y"
    {
      (yyval.having_list) = nullptr;
    }
#line 3523 "yacc_sql.cpp"
    break;

  case 160: /* having_node: HAVING having_list  */
#line 1394 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
    }
#line 3531 "yacc_sql.cpp"
    break;

  case 161: /* having_list: having_unit  */
#line 1401 "yacc_sql.y"
    {
      (yyval.having_list) = new HavingNode;
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[0].expression)));
    }
#line 3540 "yacc_sql.cpp"
    break;

  case 162: /* having_list: having_unit OR having_list  */
#line 1406 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[-2].expression)));
      (yyval.having_list)->and_or = true;
    }
#line 3550 "yacc_sql.cpp"
    break;

  case 163: /* having_list: having_unit AND having_list  */
#line 1412 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[-2].expression)));
    }
#line 3559 "yacc_sql.cpp"
    break;

  case 164: /* having_unit: expression comp_op expression  */
#line 1420 "yacc_sql.y"
    {
      unique_ptr<Expression> left((yyvsp[-2].expression));
      unique_ptr<Expression> right((yyvsp[0].expression));
      (yyval.expression) = new ComparisonExpr((yyvsp[-1].comp), std::move(left), std::move(right));
    }
#line 3569 "yacc_sql.cpp"
    break;

  case 165: /* order_by: %empty  */
#line 1429 "yacc_sql.y"
    {
      (yyval.order_by_list) = nullptr;
    }
#line 3577 "yacc_sql.cpp"
    break;

  case 166: /* order_by: ORDER BY order_by_list  */
#line 1433 "yacc_sql.y"
    {
      (yyval.order_by_list) = (yyvsp[0].order_by_list);
    }
#line 3585 "yacc_sql.cpp"
    break;

  case 167: /* order_by_list: order_by_unit  */
#line 1440 "yacc_sql.y"
    {
      (yyval.order_by_list) = new std::vector<OrderByNode>;
      (yyval.order_by_list)->emplace_back(move(*(yyvsp[0].order_by_unit)));
      delete (yyvsp[0].order_by_unit);
    }
#line 3595 "yacc_sql.cpp"
    break;

  case 168: /* order_by_list: order_by_unit COMMA order_by_list  */
#line 1446 "yacc_sql.y"
    {
      if((yyvsp[0].order_by_list) != nullptr){
        (yyval.order_by_list) = (yyvsp[0].order_by_list);
      } else {
        (yyval.order_by_list) = new std::vector<OrderByNode>;
      }
      (yyval.order_by_list)->emplace((yyval.order_by_list)->begin(), move(*(yyvsp[-2].order_by_unit)));
      delete (yyvsp[-2].order_by_unit);
    }
#line 3609 "yacc_sql.cpp"
    break;

  case 169: /* order_by_unit: expression  */
#line 1459 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = true;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[0].expression));
    }
#line 3619 "yacc_sql.cpp"
    break;

  case 170: /* order_by_unit: expression ASC  */
#line 1465 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = true;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[-1].expression));
    }
#line 3629 "yacc_sql.cpp"
    break;

  case 171: /* order_by_unit: expression DESC  */
#line 1471 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = false;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[-1].expression));
    }
#line 3639 "yacc_sql.cpp"
    break;

  case 172: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1480 "yacc_sql.y"
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
#line 3657 "yacc_sql.cpp"
    break;

  case 173: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1497 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3666 "yacc_sql.cpp"
    break;

  case 174: /* set_variable_stmt: SET ID EQ value  */
#line 1505 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3678 "yacc_sql.cpp"
    break;


#line 3682 "yacc_sql.cpp"

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

#line 1517 "yacc_sql.y"

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
