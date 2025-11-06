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
  YYSYMBOL_ALTER = 73,                     /* ALTER  */
  YYSYMBOL_ADD = 74,                       /* ADD  */
  YYSYMBOL_COLUMN = 75,                    /* COLUMN  */
  YYSYMBOL_RENAME = 76,                    /* RENAME  */
  YYSYMBOL_TO = 77,                        /* TO  */
  YYSYMBOL_FULLTEXT = 78,                  /* FULLTEXT  */
  YYSYMBOL_ALL = 79,                       /* ALL  */
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
  YYSYMBOL_NUMBER = 90,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 91,                     /* FLOAT  */
  YYSYMBOL_ID_KEY = 92,                    /* ID_KEY  */
  YYSYMBOL_SSS = 93,                       /* SSS  */
  YYSYMBOL_DATE_VALUE = 94,                /* DATE_VALUE  */
  YYSYMBOL_95_ = 95,                       /* '+'  */
  YYSYMBOL_96_ = 96,                       /* '-'  */
  YYSYMBOL_97_ = 97,                       /* '*'  */
  YYSYMBOL_98_ = 98,                       /* '/'  */
  YYSYMBOL_UMINUS = 99,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 100,                 /* $accept  */
  YYSYMBOL_commands = 101,                 /* commands  */
  YYSYMBOL_command_wrapper = 102,          /* command_wrapper  */
  YYSYMBOL_exit_stmt = 103,                /* exit_stmt  */
  YYSYMBOL_help_stmt = 104,                /* help_stmt  */
  YYSYMBOL_sync_stmt = 105,                /* sync_stmt  */
  YYSYMBOL_begin_stmt = 106,               /* begin_stmt  */
  YYSYMBOL_commit_stmt = 107,              /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 108,            /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 109,          /* drop_table_stmt  */
  YYSYMBOL_show_index_stmt = 110,          /* show_index_stmt  */
  YYSYMBOL_show_tables_stmt = 111,         /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 112,          /* desc_table_stmt  */
  YYSYMBOL_alter_table_stmt = 113,         /* alter_table_stmt  */
  YYSYMBOL_create_index_stmt = 114,        /* create_index_stmt  */
  YYSYMBOL_vector_operation = 115,         /* vector_operation  */
  YYSYMBOL_vector_index_type = 116,        /* vector_index_type  */
  YYSYMBOL_idx_col_list = 117,             /* idx_col_list  */
  YYSYMBOL_col_list = 118,                 /* col_list  */
  YYSYMBOL_unique_option = 119,            /* unique_option  */
  YYSYMBOL_drop_index_stmt = 120,          /* drop_index_stmt  */
  YYSYMBOL_create_view_stmt = 121,         /* create_view_stmt  */
  YYSYMBOL_create_table_stmt = 122,        /* create_table_stmt  */
  YYSYMBOL_as_option = 123,                /* as_option  */
  YYSYMBOL_attr_def_list = 124,            /* attr_def_list  */
  YYSYMBOL_attr_def = 125,                 /* attr_def  */
  YYSYMBOL_nullable = 126,                 /* nullable  */
  YYSYMBOL_number = 127,                   /* number  */
  YYSYMBOL_type = 128,                     /* type  */
  YYSYMBOL_date_type = 129,                /* date_type  */
  YYSYMBOL_insert_stmt = 130,              /* insert_stmt  */
  YYSYMBOL_values_list = 131,              /* values_list  */
  YYSYMBOL_value_list = 132,               /* value_list  */
  YYSYMBOL_value = 133,                    /* value  */
  YYSYMBOL_storage_format = 134,           /* storage_format  */
  YYSYMBOL_delete_stmt = 135,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 136,              /* update_stmt  */
  YYSYMBOL_key_values = 137,               /* key_values  */
  YYSYMBOL_assign_value = 138,             /* assign_value  */
  YYSYMBOL_select_stmt = 139,              /* select_stmt  */
  YYSYMBOL_select_unit = 140,              /* select_unit  */
  YYSYMBOL_limit = 141,                    /* limit  */
  YYSYMBOL_calc_stmt = 142,                /* calc_stmt  */
  YYSYMBOL_expression_list = 143,          /* expression_list  */
  YYSYMBOL_expression = 144,               /* expression  */
  YYSYMBOL_ID = 145,                       /* ID  */
  YYSYMBOL_rel_attr = 146,                 /* rel_attr  */
  YYSYMBOL_relation = 147,                 /* relation  */
  YYSYMBOL_rel_list = 148,                 /* rel_list  */
  YYSYMBOL_join_list = 149,                /* join_list  */
  YYSYMBOL_alias = 150,                    /* alias  */
  YYSYMBOL_on = 151,                       /* on  */
  YYSYMBOL_where = 152,                    /* where  */
  YYSYMBOL_condition_list = 153,           /* condition_list  */
  YYSYMBOL_condition = 154,                /* condition  */
  YYSYMBOL_comp_op = 155,                  /* comp_op  */
  YYSYMBOL_unary_op = 156,                 /* unary_op  */
  YYSYMBOL_group_by = 157,                 /* group_by  */
  YYSYMBOL_having_node = 158,              /* having_node  */
  YYSYMBOL_having_list = 159,              /* having_list  */
  YYSYMBOL_having_unit = 160,              /* having_unit  */
  YYSYMBOL_order_by = 161,                 /* order_by  */
  YYSYMBOL_order_by_list = 162,            /* order_by_list  */
  YYSYMBOL_order_by_unit = 163,            /* order_by_unit  */
  YYSYMBOL_load_data_stmt = 164,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 165,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 166,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 167             /* opt_semicolon  */
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
#define YYLAST   421

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  100
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  174
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  362

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   350


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
       2,     2,    97,    95,     2,    96,     2,    98,     2,     2,
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
      99
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   301,   301,   311,   312,   313,   314,   315,   316,   317,
     318,   319,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   333,   337,   343,   348,   354,
     360,   366,   372,   378,   386,   392,   400,   410,   420,   432,
     445,   464,   483,   511,   515,   519,   526,   530,   533,   547,
     550,   563,   566,   571,   582,   589,   608,   629,   647,   659,
     662,   667,   670,   683,   692,   701,   714,   715,   716,   720,
     723,   724,   725,   726,   727,   730,   734,   756,   762,   776,
     786,   803,   807,   811,   818,   825,   828,   835,   838,   845,
     857,   875,   882,   897,   901,   905,   916,   930,   978,   981,
     987,   996,  1005,  1020,  1023,  1026,  1029,  1032,  1035,  1044,
    1047,  1053,  1056,  1110,  1122,  1125,  1138,  1139,  1144,  1149,
    1157,  1162,  1169,  1178,  1183,  1199,  1232,  1235,  1262,  1265,
    1268,  1275,  1278,  1287,  1290,  1295,  1300,  1305,  1313,  1320,
    1333,  1343,  1344,  1345,  1346,  1347,  1348,  1349,  1350,  1351,
    1352,  1353,  1354,  1358,  1359,  1365,  1368,  1376,  1379,  1386,
    1391,  1397,  1405,  1415,  1418,  1425,  1431,  1444,  1450,  1456,
    1465,  1478,  1486,  1496,  1497
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
  "LE", "GE", "NE", "UNIQUE", "UNION", "ALTER", "ADD", "COLUMN", "RENAME",
  "TO", "FULLTEXT", "ALL", "L2_DISTANCE", "COSINE_DISTANCE",
  "INNER_PRODUCT", "DISTANCE", "LISTS", "TYPE", "PROBES", "IVFFLAT",
  "MATCH", "AGAINST", "NUMBER", "FLOAT", "ID_KEY", "SSS", "DATE_VALUE",
  "'+'", "'-'", "'*'", "'/'", "UMINUS", "$accept", "commands",
  "command_wrapper", "exit_stmt", "help_stmt", "sync_stmt", "begin_stmt",
  "commit_stmt", "rollback_stmt", "drop_table_stmt", "show_index_stmt",
  "show_tables_stmt", "desc_table_stmt", "alter_table_stmt",
  "create_index_stmt", "vector_operation", "vector_index_type",
  "idx_col_list", "col_list", "unique_option", "drop_index_stmt",
  "create_view_stmt", "create_table_stmt", "as_option", "attr_def_list",
  "attr_def", "nullable", "number", "type", "date_type", "insert_stmt",
  "values_list", "value_list", "value", "storage_format", "delete_stmt",
  "update_stmt", "key_values", "assign_value", "select_stmt",
  "select_unit", "limit", "calc_stmt", "expression_list", "expression",
  "ID", "rel_attr", "relation", "rel_list", "join_list", "alias", "on",
  "where", "condition_list", "condition", "comp_op", "unary_op",
  "group_by", "having_node", "having_list", "having_unit", "order_by",
  "order_by_list", "order_by_unit", "load_data_stmt", "explain_stmt",
  "set_variable_stmt", "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-305)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-140)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     348,     4,     6,   173,   213,   213,    78,  -305,   -10,    -2,
     173,  -305,  -305,  -305,  -305,  -305,   173,   -16,   348,    36,
      76,    93,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,    29,  -305,  -305,  -305,  -305,   173,   173,    91,  -305,
     102,   105,   173,   173,  -305,  -305,  -305,  -305,  -305,    66,
    -305,  -305,  -305,   107,   213,  -305,   110,  -305,   192,    -4,
    -305,    96,  -305,   118,   173,   173,   137,    80,   117,  -305,
     173,  -305,  -305,  -305,    -6,     5,    11,   173,   173,   173,
    -305,   157,   186,   189,   173,  -305,   213,   173,   213,   213,
     213,   213,  -305,   187,   150,   -38,   173,   173,   194,   175,
     173,    89,   129,    13,   206,  -305,   173,  -305,   206,   173,
     206,   178,   183,   190,   173,  -305,  -305,   199,   216,     2,
    -305,    45,    45,  -305,  -305,   213,  -305,   219,  -305,  -305,
    -305,    84,   175,  -305,   173,   207,   128,  -305,   175,   188,
     213,  -305,  -305,  -305,  -305,  -305,  -305,   218,   179,   185,
      55,  -305,   220,   161,  -305,   234,  -305,   173,   173,   173,
    -305,   174,   213,  -305,  -305,   222,   257,   234,   244,  -305,
     214,   247,   277,  -305,   109,   213,  -305,   213,   246,    30,
     262,   173,   173,   173,   173,   173,   250,  -305,  -305,  -305,
    -305,  -305,  -305,    15,   -21,   173,   258,   261,   268,   272,
     275,   -17,   236,   274,   296,   294,   281,   213,  -305,  -305,
     173,  -305,  -305,   255,   -31,  -305,  -305,  -305,  -305,  -305,
    -305,   213,   128,   128,    75,   288,    75,  -305,   213,   173,
    -305,  -305,   240,  -305,   220,   116,   228,  -305,   264,  -305,
    -305,   234,   269,   173,   173,   173,   229,  -305,   173,   173,
     213,   213,   313,  -305,   298,   299,  -305,  -305,  -305,    75,
    -305,  -305,   173,  -305,  -305,   173,  -305,   263,   206,  -305,
    -305,   300,  -305,  -305,   206,   234,   234,   234,   302,    84,
    -305,  -305,   277,  -305,   112,   324,   266,   305,   248,  -305,
    -305,   273,  -305,   -21,  -305,   314,   315,   316,  -305,   306,
     213,   213,   213,   213,   228,  -305,   244,   326,   173,  -305,
     335,  -305,  -305,   128,   222,    75,  -305,  -305,    14,  -305,
     329,  -305,  -305,   265,  -305,   336,  -305,  -305,  -305,  -305,
     213,   337,   276,  -305,   304,   317,   293,  -305,   353,   280,
     318,   -29,   356,   301,   323,   303,   361,   308,   325,   228,
     366,  -305
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    51,     0,     0,     0,     0,     0,    28,     0,     0,
       0,    29,    30,    31,    27,    26,     0,     0,     0,     0,
       0,   173,    25,    24,    17,    18,    19,    20,     9,    10,
      12,    13,    16,    14,    15,    11,     8,     5,     7,     6,
       4,    94,     3,    21,    22,    23,     0,     0,     0,    52,
       0,     0,     0,     0,   117,   118,   119,   116,    35,     0,
      43,    44,    45,     0,     0,   111,     0,   100,   128,   120,
     110,     0,    34,     0,     0,     0,     0,     0,     0,   171,
       0,     1,   174,     2,     0,    59,     0,     0,     0,     0,
      32,     0,     0,     0,     0,   109,     0,     0,     0,     0,
       0,     0,   129,   101,     0,     0,     0,     0,    49,   133,
       0,     0,     0,     0,     0,    95,     0,    60,     0,     0,
       0,     0,     0,     0,     0,   114,   108,   120,     0,     0,
     130,   103,   104,   105,   106,     0,   113,     0,   122,   121,
     123,   128,   133,    33,     0,     0,     0,    89,   133,     0,
       0,    85,    81,    82,    83,    84,   172,     0,     0,     0,
       0,    96,    61,     0,    58,    47,    54,     0,     0,     0,
      53,     0,     0,   102,   112,   126,   155,    47,     0,   153,
       0,     0,     0,   134,   135,     0,    90,     0,     0,    79,
       0,     0,     0,     0,     0,     0,     0,    70,    71,    72,
      75,    73,    74,    66,    66,     0,     0,     0,     0,     0,
       0,     0,     0,   124,     0,   157,     0,     0,    76,   154,
       0,   141,   149,   151,     0,   143,   144,   145,   146,   147,
     148,     0,     0,     0,   140,    91,    93,    86,     0,     0,
      37,    36,     0,    39,    61,    87,     0,    67,     0,    64,
      65,    47,     0,     0,     0,     0,     0,   107,     0,     0,
       0,     0,   163,    50,     0,     0,   152,   142,   150,   138,
     136,   137,     0,    80,   170,     0,    62,     0,     0,    56,
      69,     0,    68,    48,     0,    47,    47,    47,     0,   128,
     125,   156,     0,   158,   159,     0,    98,    77,     0,    92,
      38,     0,    57,    66,    55,     0,     0,     0,   115,   131,
       0,     0,     0,     0,     0,    97,     0,     0,     0,    63,
       0,    41,    40,     0,   126,   162,   161,   160,   167,   164,
     165,    99,    78,     0,    88,     0,   132,   127,   168,   169,
       0,     0,     0,   166,   115,     0,     0,    46,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    42
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -305,  -305,   374,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,    44,  -305,  -163,  -305,  -305,
    -305,  -305,  -305,   151,   154,  -150,  -195,  -304,  -305,  -305,
    -305,    83,  -205,  -305,  -305,  -305,  -305,   130,  -305,   -57,
    -305,  -305,  -305,     1,   -32,    -3,   -91,   143,   144,    81,
    -140,  -305,  -120,  -228,  -305,   114,  -305,  -305,  -305,  -107,
    -305,  -305,    64,  -305,  -305,  -305,  -305,  -305
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    66,   348,   206,   145,    51,
      34,    35,    36,   118,   196,   162,   249,   281,   203,   204,
      37,   218,   188,   156,   279,    38,    39,   148,   235,    40,
      41,   315,    42,    67,    68,    69,    70,   141,   142,   213,
     103,   324,   147,   183,   184,   231,   185,   215,   262,   293,
     294,   296,   329,   330,    43,    44,    45,    83
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      58,   175,    92,   128,   270,   271,    71,    76,   257,   250,
     331,     5,   264,    77,   216,    54,    46,    47,    52,   158,
     104,    53,   176,   267,   338,   339,   268,   115,   186,   116,
     172,    74,    95,   273,   247,   119,   105,    78,   248,   246,
      48,    75,   241,    85,    86,   244,    55,    56,    80,    90,
      91,    60,    61,    62,    57,   360,   117,   161,   238,   138,
      93,   164,   120,   166,   129,   102,   131,   132,   133,   134,
     247,   108,   109,   114,   248,    49,    81,   113,    98,    99,
     100,   101,    50,     5,   121,   122,   123,   159,   283,   160,
      59,   127,    72,    73,   130,   336,    82,    98,    99,   100,
     101,    84,   139,   140,   143,   137,    87,   149,   319,    98,
      99,   100,   101,   163,   182,   150,   165,    88,   189,    54,
      89,   170,   305,   306,   307,    98,    99,   100,   101,   265,
     193,    94,   194,   -59,    96,    97,   173,    54,   102,   106,
     211,   177,   100,   101,   151,   111,    60,    61,    62,   309,
      55,    56,    59,   234,    63,   236,   232,   233,    57,   311,
     312,   107,    64,    65,   207,   208,   209,   117,    55,    56,
      98,    99,   100,   101,    59,   136,    57,   112,   277,   152,
     153,    54,   154,   155,   179,   189,   110,   180,   240,   163,
     242,   243,   163,   197,   198,   199,   200,   201,   202,   269,
     182,   182,   251,    54,   326,   327,   189,   124,    60,    61,
      62,   125,    55,    56,   126,   135,   181,   127,   144,   146,
      57,   302,   157,     5,    64,    65,    54,   304,   167,   292,
      60,    61,    62,   168,    55,    56,   274,    59,    63,   105,
     169,   171,    57,    97,   174,    54,    64,    65,   195,   178,
     285,   286,   287,   187,   191,   140,   140,    55,    56,   190,
     192,   291,   205,   210,   214,    57,    54,   212,   217,   149,
     219,   220,   300,   237,   239,   245,    55,    56,   325,   292,
     292,   328,   258,   252,    57,   253,   102,    98,    99,   100,
     101,   182,   254,    60,    61,    62,   255,    55,    56,   256,
     260,    63,   259,   261,  -139,    57,   263,  -139,   328,    64,
      65,  -139,  -139,  -139,   266,   334,   272,   275,   280,   282,
     284,   295,   288,   297,   298,   303,   301,   308,   313,  -139,
     314,   221,  -139,   316,   222,   223,   224,   317,   318,   320,
     321,   322,   225,   226,   227,   228,   229,   230,  -139,  -139,
     333,  -139,  -139,     1,     2,   335,   323,   340,   341,     3,
     342,   345,   344,   350,     4,     5,     6,     7,  -139,     8,
       9,    10,    98,    99,   100,   101,  -139,    11,    12,    13,
     347,   349,   346,   351,   353,   354,    14,    15,   355,   357,
     359,   361,    79,   356,   358,   352,   278,    16,   276,   332,
      17,   289,   299,   290,   343,   337,   310,     0,     0,    18,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    19
};

static const yytype_int16 yycheck[] =
{
       3,   141,    59,    94,   232,   233,     5,    10,    25,   204,
     314,    17,   217,    16,   177,    53,    12,    13,    12,     6,
      24,    15,   142,    54,    10,    11,    57,    84,   148,    24,
      28,    41,    64,   238,    55,    24,    40,    53,    59,    24,
      36,    43,   192,    46,    47,   195,    84,    85,    12,    52,
      53,    80,    81,    82,    92,   359,    51,   114,    28,    97,
      59,   118,    51,   120,    96,    68,    98,    99,   100,   101,
      55,    74,    75,    79,    59,    71,     0,    80,    95,    96,
      97,    98,    78,    17,    87,    88,    89,    74,   251,    76,
      24,    94,    14,    15,    97,   323,     3,    95,    96,    97,
      98,    72,   105,   106,   107,   104,    15,   110,   303,    95,
      96,    97,    98,   116,   146,    26,   119,    15,   150,    53,
      15,   124,   285,   286,   287,    95,    96,    97,    98,   220,
      75,    24,    77,    17,    24,    51,   135,    53,   141,    43,
     172,   144,    97,    98,    55,    65,    80,    81,    82,   289,
      84,    85,    24,   185,    88,   187,    47,    48,    92,    47,
      48,    43,    96,    97,   167,   168,   169,    51,    84,    85,
      95,    96,    97,    98,    24,    25,    92,    60,    62,    90,
      91,    53,    93,    94,    56,   217,    49,    59,   191,   192,
     193,   194,   195,    32,    33,    34,    35,    36,    37,   231,
     232,   233,   205,    53,   311,   312,   238,    50,    80,    81,
      82,    25,    84,    85,    25,    28,    88,   220,    24,    44,
      92,   278,    93,    17,    96,    97,    53,   284,    50,   261,
      80,    81,    82,    50,    84,    85,   239,    24,    88,    40,
      50,    25,    92,    51,    25,    53,    96,    97,    28,    42,
     253,   254,   255,    65,    75,   258,   259,    84,    85,    41,
      75,   260,    28,    89,     7,    92,    53,    45,    24,   272,
      56,    24,   275,    27,    12,    25,    84,    85,   310,   311,
     312,   313,    46,    25,    92,    24,   289,    95,    96,    97,
      98,   323,    24,    80,    81,    82,    24,    84,    85,    24,
       4,    88,    28,     9,     0,    92,    25,     3,   340,    96,
      97,     7,     8,     9,    59,   318,    28,    77,    90,    55,
      51,     8,    93,    25,    25,    25,    63,    25,     4,    25,
      64,    54,    28,    28,    57,    58,    59,    89,    65,    25,
      25,    25,    65,    66,    67,    68,    69,    70,    44,    45,
      24,    47,    48,     5,     6,    20,    50,    28,    93,    11,
      24,    85,    25,    83,    16,    17,    18,    19,    64,    21,
      22,    23,    95,    96,    97,    98,    72,    29,    30,    31,
      87,    28,    65,    65,    28,    84,    38,    39,    65,    28,
      65,    25,    18,    90,    86,   351,   245,    49,   244,   316,
      52,   258,   272,   259,   340,   324,   292,    -1,    -1,    61,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    73
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     6,    11,    16,    17,    18,    19,    21,    22,
      23,    29,    30,    31,    38,    39,    49,    52,    61,    73,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   120,   121,   122,   130,   135,   136,
     139,   140,   142,   164,   165,   166,    12,    13,    36,    71,
      78,   119,    12,    15,    53,    84,    85,    92,   145,    24,
      80,    81,    82,    88,    96,    97,   115,   143,   144,   145,
     146,   143,    14,    15,    41,    43,   145,   145,    53,   102,
      12,     0,     3,   167,    72,   145,   145,    15,    15,    15,
     145,   145,   139,   143,    24,   144,    24,    51,    95,    96,
      97,    98,   145,   150,    24,    40,    43,    43,   145,   145,
      49,    65,    60,   145,    79,   139,    24,    51,   123,    24,
      51,   145,   145,   145,    50,    25,    25,   145,   146,   144,
     145,   144,   144,   144,   144,    28,    25,   143,    97,   145,
     145,   147,   148,   145,    24,   118,    44,   152,   137,   145,
      26,    55,    90,    91,    93,    94,   133,    93,     6,    74,
      76,   139,   125,   145,   139,   145,   139,    50,    50,    50,
     145,    25,    28,   143,    25,   150,   152,   145,    42,    56,
      59,    88,   144,   153,   154,   156,   152,    65,   132,   144,
      41,    75,    75,    75,    77,    28,   124,    32,    33,    34,
      35,    36,    37,   128,   129,    28,   117,   145,   145,   145,
      89,   144,    45,   149,     7,   157,   117,    24,   131,    56,
      24,    54,    57,    58,    59,    65,    66,    67,    68,    69,
      70,   155,    47,    48,   144,   138,   144,    27,    28,    12,
     145,   125,   145,   145,   125,    25,    24,    55,    59,   126,
     126,   145,    25,    24,    24,    24,    24,    25,    46,    28,
       4,     9,   158,    25,   132,   146,    59,    54,    57,   144,
     153,   153,    28,   132,   145,    77,   124,    62,   123,   134,
      90,   127,    55,   117,    51,   145,   145,   145,    93,   147,
     148,   143,   144,   159,   160,     8,   161,    25,    25,   137,
     145,    63,   139,    25,   139,   117,   117,   117,    25,   150,
     155,    47,    48,     4,    64,   141,    28,    89,    65,   126,
      25,    25,    25,    50,   151,   144,   159,   159,   144,   162,
     163,   127,   131,    24,   145,    20,   153,   149,    10,    11,
      28,    93,    24,   162,    25,    85,    65,    87,   116,    28,
      83,    65,   115,    28,    84,    65,    90,    28,    86,    65,
     127,    25
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,   100,   101,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   113,   113,   113,
     114,   114,   114,   115,   115,   115,   116,   117,   117,   118,
     118,   119,   119,   120,   121,   121,   122,   122,   122,   123,
     123,   124,   124,   125,   125,   125,   126,   126,   126,   127,
     128,   128,   128,   128,   128,   129,   130,   131,   131,   132,
     132,   133,   133,   133,   133,   133,   133,   134,   134,   135,
     136,   137,   137,   138,   139,   139,   139,   140,   141,   141,
     142,   143,   143,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   145,   145,   145,   145,
     146,   146,   146,   147,   148,   148,   149,   149,   150,   150,
     150,   151,   151,   152,   152,   153,   153,   153,   154,   154,
     154,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   156,   156,   157,   157,   158,   158,   159,
     159,   159,   160,   161,   161,   162,   162,   163,   163,   163,
     164,   165,   166,   167,   167
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
       1,     1,     1,     1,     1,     1,     6,     3,     5,     1,
       3,     1,     1,     1,     1,     1,     3,     0,     4,     4,
       5,     3,     5,     1,     1,     3,     4,     9,     0,     2,
       2,     2,     4,     3,     3,     3,     3,     6,     3,     2,
       1,     1,     4,     3,     3,     8,     1,     1,     1,     1,
       1,     3,     3,     1,     3,     5,     0,     6,     0,     1,
       2,     0,     2,     0,     2,     1,     3,     3,     3,     8,
       2,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     2,     1,     2,     0,     3,     0,     2,     1,
       3,     3,     3,     0,     3,     1,     3,     1,     2,     2,
       7,     2,     4,     0,     1
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
#line 302 "yacc_sql.y"
  {
    if((yyvsp[-1].sql_node) != nullptr){
      std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
      sql_result->add_sql_node(std::move(sql_node));
    }
  }
#line 1995 "yacc_sql.cpp"
    break;

  case 26: /* exit_stmt: EXIT  */
#line 337 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 2004 "yacc_sql.cpp"
    break;

  case 27: /* help_stmt: HELP  */
#line 343 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 2012 "yacc_sql.cpp"
    break;

  case 28: /* sync_stmt: SYNC  */
#line 348 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 2020 "yacc_sql.cpp"
    break;

  case 29: /* begin_stmt: TRX_BEGIN  */
#line 354 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 2028 "yacc_sql.cpp"
    break;

  case 30: /* commit_stmt: TRX_COMMIT  */
#line 360 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 2036 "yacc_sql.cpp"
    break;

  case 31: /* rollback_stmt: TRX_ROLLBACK  */
#line 366 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 2044 "yacc_sql.cpp"
    break;

  case 32: /* drop_table_stmt: DROP TABLE ID  */
#line 372 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2054 "yacc_sql.cpp"
    break;

  case 33: /* show_index_stmt: SHOW INDEX FROM ID  */
#line 379 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_INDEX);
      (yyval.sql_node)->show_index.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2064 "yacc_sql.cpp"
    break;

  case 34: /* show_tables_stmt: SHOW TABLES  */
#line 386 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 2072 "yacc_sql.cpp"
    break;

  case 35: /* desc_table_stmt: DESC ID  */
#line 392 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2082 "yacc_sql.cpp"
    break;

  case 36: /* alter_table_stmt: ALTER TABLE ID ADD COLUMN attr_def  */
#line 401 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::ADD_COLUMN;
      alter_table.attr_info = *(yyvsp[0].attr_info);
      free((yyvsp[-3].string));
      delete (yyvsp[0].attr_info);
    }
#line 2096 "yacc_sql.cpp"
    break;

  case 37: /* alter_table_stmt: ALTER TABLE ID DROP COLUMN ID  */
#line 411 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::DROP_COLUMN;
      alter_table.old_name = (yyvsp[0].string);
      free((yyvsp[-3].string));
      free((yyvsp[0].string));
    }
#line 2110 "yacc_sql.cpp"
    break;

  case 38: /* alter_table_stmt: ALTER TABLE ID RENAME COLUMN ID TO ID  */
#line 421 "yacc_sql.y"
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
#line 2126 "yacc_sql.cpp"
    break;

  case 39: /* alter_table_stmt: ALTER TABLE ID RENAME TO ID  */
#line 433 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ALTER_TABLE);
      AlterTableSqlNode &alter_table = (yyval.sql_node)->alter_table;
      alter_table.relation_name = (yyvsp[-3].string);
      alter_table.alter_type = AlterTableSqlNode::AlterType::RENAME_TABLE;
      alter_table.new_name = (yyvsp[0].string);
      free((yyvsp[-3].string));
      free((yyvsp[0].string));
    }
#line 2140 "yacc_sql.cpp"
    break;

  case 40: /* create_index_stmt: CREATE unique_option INDEX ID ON ID LBRACE ID idx_col_list RBRACE  */
#line 446 "yacc_sql.y"
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
#line 2163 "yacc_sql.cpp"
    break;

  case 41: /* create_index_stmt: CREATE FULLTEXT INDEX ID ON ID LBRACE ID idx_col_list RBRACE  */
#line 465 "yacc_sql.y"
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
#line 2186 "yacc_sql.cpp"
    break;

  case 42: /* create_index_stmt: CREATE VECTOR_T INDEX ID ON ID LBRACE ID idx_col_list RBRACE WITH LBRACE TYPE EQ vector_index_type COMMA DISTANCE EQ vector_operation COMMA LISTS EQ NUMBER COMMA PROBES EQ number RBRACE  */
#line 486 "yacc_sql.y"
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
#line 2213 "yacc_sql.cpp"
    break;

  case 43: /* vector_operation: L2_DISTANCE  */
#line 512 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::L2_DISTANCE;
    }
#line 2221 "yacc_sql.cpp"
    break;

  case 44: /* vector_operation: COSINE_DISTANCE  */
#line 516 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::COSINE_DISTANCE;
    }
#line 2229 "yacc_sql.cpp"
    break;

  case 45: /* vector_operation: INNER_PRODUCT  */
#line 520 "yacc_sql.y"
    {
      (yyval.number) = (int)VectorOperationExpr::Type::INNER_PRODUCT;
    }
#line 2237 "yacc_sql.cpp"
    break;

  case 46: /* vector_index_type: IVFFLAT  */
#line 526 "yacc_sql.y"
            { (yyval.vector_index_type) = VectorIndexType::IVFFLAT; }
#line 2243 "yacc_sql.cpp"
    break;

  case 47: /* idx_col_list: %empty  */
#line 530 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2251 "yacc_sql.cpp"
    break;

  case 48: /* idx_col_list: COMMA ID idx_col_list  */
#line 534 "yacc_sql.y"
    {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }
      (yyval.relation_list)->emplace_back((yyvsp[-1].string));
      free((yyvsp[-1].string));
    }
#line 2265 "yacc_sql.cpp"
    break;

  case 49: /* col_list: %empty  */
#line 547 "yacc_sql.y"
    {
      (yyval.relation_list) = nullptr;
    }
#line 2273 "yacc_sql.cpp"
    break;

  case 50: /* col_list: LBRACE ID idx_col_list RBRACE  */
#line 551 "yacc_sql.y"
    {
      if ((yyvsp[-1].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[-1].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }
      (yyval.relation_list)->emplace_back((yyvsp[-2].string));
      free((yyvsp[-2].string));      
    }
#line 2287 "yacc_sql.cpp"
    break;

  case 51: /* unique_option: %empty  */
#line 563 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2295 "yacc_sql.cpp"
    break;

  case 52: /* unique_option: UNIQUE  */
#line 567 "yacc_sql.y"
    {
      (yyval.boolean) = true;
    }
#line 2303 "yacc_sql.cpp"
    break;

  case 53: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 572 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2315 "yacc_sql.cpp"
    break;

  case 54: /* create_view_stmt: CREATE VIEW ID AS select_stmt  */
#line 583 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_VIEW;
      (yyval.sql_node)->create_view.view_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2326 "yacc_sql.cpp"
    break;

  case 55: /* create_view_stmt: CREATE VIEW ID LBRACE ID idx_col_list RBRACE AS select_stmt  */
#line 590 "yacc_sql.y"
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
#line 2346 "yacc_sql.cpp"
    break;

  case 56: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 609 "yacc_sql.y"
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
#line 2371 "yacc_sql.cpp"
    break;

  case 57: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE as_option select_stmt  */
#line 630 "yacc_sql.y"
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
#line 2393 "yacc_sql.cpp"
    break;

  case 58: /* create_table_stmt: CREATE TABLE ID as_option select_stmt  */
#line 648 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
      (yyval.sql_node)->flag = SCF_CREATE_TABLE;
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-2].string);
      free((yyvsp[-2].string));
    }
#line 2405 "yacc_sql.cpp"
    break;

  case 59: /* as_option: %empty  */
#line 659 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2413 "yacc_sql.cpp"
    break;

  case 60: /* as_option: AS  */
#line 662 "yacc_sql.y"
         { (yyval.boolean) = true; }
#line 2419 "yacc_sql.cpp"
    break;

  case 61: /* attr_def_list: %empty  */
#line 667 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2427 "yacc_sql.cpp"
    break;

  case 62: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 671 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(move(*(yyvsp[-1].attr_info)));
      delete (yyvsp[-1].attr_info);
    }
#line 2441 "yacc_sql.cpp"
    break;

  case 63: /* attr_def: ID type LBRACE number RBRACE nullable  */
#line 684 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-5].string));
    }
#line 2454 "yacc_sql.cpp"
    break;

  case 64: /* attr_def: ID type nullable  */
#line 693 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2467 "yacc_sql.cpp"
    break;

  case 65: /* attr_def: ID date_type nullable  */
#line 702 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 10;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2480 "yacc_sql.cpp"
    break;

  case 66: /* nullable: %empty  */
#line 714 "yacc_sql.y"
    { (yyval.boolean) = true; }
#line 2486 "yacc_sql.cpp"
    break;

  case 67: /* nullable: NULL_T  */
#line 715 "yacc_sql.y"
             {(yyval.boolean) = true;}
#line 2492 "yacc_sql.cpp"
    break;

  case 68: /* nullable: NOT NULL_T  */
#line 716 "yacc_sql.y"
                 {(yyval.boolean) = false;}
#line 2498 "yacc_sql.cpp"
    break;

  case 69: /* number: NUMBER  */
#line 720 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2504 "yacc_sql.cpp"
    break;

  case 70: /* type: INT_T  */
#line 723 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2510 "yacc_sql.cpp"
    break;

  case 71: /* type: STRING_T  */
#line 724 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2516 "yacc_sql.cpp"
    break;

  case 72: /* type: FLOAT_T  */
#line 725 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2522 "yacc_sql.cpp"
    break;

  case 73: /* type: VECTOR_T  */
#line 726 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2528 "yacc_sql.cpp"
    break;

  case 74: /* type: TEXT_T  */
#line 727 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::TEXTS);}
#line 2534 "yacc_sql.cpp"
    break;

  case 75: /* date_type: DATE_T  */
#line 730 "yacc_sql.y"
             {(yyval.number) = static_cast<int>(AttrType::DATES);}
#line 2540 "yacc_sql.cpp"
    break;

  case 76: /* insert_stmt: INSERT INTO ID col_list VALUES values_list  */
#line 735 "yacc_sql.y"
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
#line 2563 "yacc_sql.cpp"
    break;

  case 77: /* values_list: LBRACE value_list RBRACE  */
#line 757 "yacc_sql.y"
    {
      (yyval.values_list) = new std::vector<std::vector<Value>>;
      (yyval.values_list)->emplace_back(move(*(yyvsp[-1].value_list)));
      delete (yyvsp[-1].value_list);
    }
#line 2573 "yacc_sql.cpp"
    break;

  case 78: /* values_list: LBRACE value_list RBRACE COMMA values_list  */
#line 763 "yacc_sql.y"
    {
      if ((yyvsp[0].values_list) != nullptr) {
        (yyval.values_list) = (yyvsp[0].values_list);
      } else {
        (yyval.values_list) = new std::vector<std::vector<Value>>;
      }

      (yyval.values_list)->emplace_back(move(*(yyvsp[-3].value_list)));
      delete (yyvsp[-3].value_list);
    }
#line 2588 "yacc_sql.cpp"
    break;

  case 79: /* value_list: expression  */
#line 776 "yacc_sql.y"
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
#line 2603 "yacc_sql.cpp"
    break;

  case 80: /* value_list: expression COMMA value_list  */
#line 786 "yacc_sql.y"
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
#line 2623 "yacc_sql.cpp"
    break;

  case 81: /* value: NUMBER  */
#line 803 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2632 "yacc_sql.cpp"
    break;

  case 82: /* value: FLOAT  */
#line 807 "yacc_sql.y"
           {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2641 "yacc_sql.cpp"
    break;

  case 83: /* value: SSS  */
#line 811 "yacc_sql.y"
         {
      int len = strlen((yyvsp[0].string))-2;
      char *tmp = common::substr((yyvsp[0].string),1,len);
      (yyval.value) = new Value(tmp, len);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2653 "yacc_sql.cpp"
    break;

  case 84: /* value: DATE_VALUE  */
#line 818 "yacc_sql.y"
                {
      int len = strlen((yyvsp[0].string))-2;
      char *tmp = common::substr((yyvsp[0].string),1,len);
      (yyval.value) = new Value((Date*)tmp, len);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2665 "yacc_sql.cpp"
    break;

  case 85: /* value: NULL_T  */
#line 825 "yacc_sql.y"
            {
      (yyval.value) = new Value((void*)nullptr);
    }
#line 2673 "yacc_sql.cpp"
    break;

  case 86: /* value: LMBRACE value_list RMBRACE  */
#line 828 "yacc_sql.y"
                                 {
      (yyval.value) = new Value((yyvsp[-1].value_list));
      delete (yyvsp[-1].value_list);
    }
#line 2682 "yacc_sql.cpp"
    break;

  case 87: /* storage_format: %empty  */
#line 835 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2690 "yacc_sql.cpp"
    break;

  case 88: /* storage_format: STORAGE FORMAT EQ ID  */
#line 839 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2698 "yacc_sql.cpp"
    break;

  case 89: /* delete_stmt: DELETE FROM ID where  */
#line 846 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition_list) != nullptr) {
        swap((yyval.sql_node)->deletion.conditions, *(yyvsp[0].condition_list));
        delete (yyvsp[0].condition_list);
      }
      free((yyvsp[-1].string));
    }
#line 2712 "yacc_sql.cpp"
    break;

  case 90: /* update_stmt: UPDATE ID SET key_values where  */
#line 858 "yacc_sql.y"
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
#line 2731 "yacc_sql.cpp"
    break;

  case 91: /* key_values: ID EQ assign_value  */
#line 876 "yacc_sql.y"
    {
      (yyval.key_values) = new Key_values;
      (yyval.key_values)->relation_list.emplace_back(std::string((yyvsp[-2].string)));
      free((yyvsp[-2].string));
      (yyval.key_values)->value_list.emplace_back(unique_ptr<Expression>((yyvsp[0].expression)));
    }
#line 2742 "yacc_sql.cpp"
    break;

  case 92: /* key_values: ID EQ assign_value COMMA key_values  */
#line 883 "yacc_sql.y"
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
#line 2758 "yacc_sql.cpp"
    break;

  case 93: /* assign_value: expression  */
#line 897 "yacc_sql.y"
               { (yyval.expression) = (yyvsp[0].expression); }
#line 2764 "yacc_sql.cpp"
    break;

  case 94: /* select_stmt: select_unit  */
#line 902 "yacc_sql.y"
    {
      (yyval.sql_node) = (yyvsp[0].sql_node);
    }
#line 2772 "yacc_sql.cpp"
    break;

  case 95: /* select_stmt: select_unit UNION select_stmt  */
#line 906 "yacc_sql.y"
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
#line 2787 "yacc_sql.cpp"
    break;

  case 96: /* select_stmt: select_unit UNION ALL select_stmt  */
#line 917 "yacc_sql.y"
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
#line 2802 "yacc_sql.cpp"
    break;

  case 97: /* select_unit: SELECT expression_list FROM rel_list where group_by having_node order_by limit  */
#line 931 "yacc_sql.y"
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
#line 2850 "yacc_sql.cpp"
    break;

  case 98: /* limit: %empty  */
#line 978 "yacc_sql.y"
    {
      (yyval.number) = -1;
    }
#line 2858 "yacc_sql.cpp"
    break;

  case 99: /* limit: LIMIT number  */
#line 982 "yacc_sql.y"
    {
      (yyval.number) = (yyvsp[0].number);
    }
#line 2866 "yacc_sql.cpp"
    break;

  case 100: /* calc_stmt: CALC expression_list  */
#line 988 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2876 "yacc_sql.cpp"
    break;

  case 101: /* expression_list: expression alias  */
#line 997 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      if((yyvsp[0].string) != nullptr){
        (yyvsp[-1].expression)->set_alias((yyvsp[0].string));
        free((yyvsp[0].string));
      }
      (yyval.expression_list)->emplace_back(move((yyvsp[-1].expression)));
    }
#line 2889 "yacc_sql.cpp"
    break;

  case 102: /* expression_list: expression alias COMMA expression_list  */
#line 1006 "yacc_sql.y"
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
#line 2906 "yacc_sql.cpp"
    break;

  case 103: /* expression: expression '+' expression  */
#line 1020 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2914 "yacc_sql.cpp"
    break;

  case 104: /* expression: expression '-' expression  */
#line 1023 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2922 "yacc_sql.cpp"
    break;

  case 105: /* expression: expression '*' expression  */
#line 1026 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2930 "yacc_sql.cpp"
    break;

  case 106: /* expression: expression '/' expression  */
#line 1029 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2938 "yacc_sql.cpp"
    break;

  case 107: /* expression: vector_operation LBRACE expression COMMA expression RBRACE  */
#line 1032 "yacc_sql.y"
                                                                {
      (yyval.expression) = create_operation_expression((VectorOperationExpr::Type)(yyvsp[-5].number), (yyvsp[-3].expression), (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2946 "yacc_sql.cpp"
    break;

  case 108: /* expression: LBRACE expression_list RBRACE  */
#line 1035 "yacc_sql.y"
                                    {
      if ((yyvsp[-1].expression_list)->size() == 1) {
        (yyval.expression) = ((yyvsp[-1].expression_list)->front()).release();
      } else {
        (yyval.expression) = new ValueListExpr(*(yyvsp[-1].expression_list));
      }
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[-1].expression_list);
    }
#line 2960 "yacc_sql.cpp"
    break;

  case 109: /* expression: '-' expression  */
#line 1044 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2968 "yacc_sql.cpp"
    break;

  case 110: /* expression: rel_attr  */
#line 1047 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2979 "yacc_sql.cpp"
    break;

  case 111: /* expression: '*'  */
#line 1053 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 2987 "yacc_sql.cpp"
    break;

  case 112: /* expression: ID LBRACE expression_list RBRACE  */
#line 1056 "yacc_sql.y"
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
#line 3046 "yacc_sql.cpp"
    break;

  case 113: /* expression: ID LBRACE RBRACE  */
#line 1110 "yacc_sql.y"
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
#line 3063 "yacc_sql.cpp"
    break;

  case 114: /* expression: LBRACE select_stmt RBRACE  */
#line 1122 "yacc_sql.y"
                                {
      (yyval.expression) = new SelectExpr((yyvsp[-1].sql_node));
    }
#line 3071 "yacc_sql.cpp"
    break;

  case 115: /* expression: MATCH LBRACE rel_attr RBRACE AGAINST LBRACE SSS RBRACE  */
#line 1126 "yacc_sql.y"
    {
      // MATCH(field) AGAINST('query') as expression (for ORDER BY and SELECT)
      Expression *field_expr = new UnboundFieldExpr((yyvsp[-5].rel_attr)->relation_name, (yyvsp[-5].rel_attr)->attribute_name);
      Expression *query_expr = new ValueExpr(Value((yyvsp[-1].string)));
      (yyval.expression) = create_sysfunc_expression("match_against", field_expr, query_expr, nullptr, sql_string, &(yyloc));
      delete (yyvsp[-5].rel_attr);
      free((yyvsp[-1].string));
    }
#line 3084 "yacc_sql.cpp"
    break;

  case 116: /* ID: ID_KEY  */
#line 1138 "yacc_sql.y"
           { (yyval.string) = (yyvsp[0].string); }
#line 3090 "yacc_sql.cpp"
    break;

  case 117: /* ID: DATA  */
#line 1140 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 5);
      memcpy((yyval.string), "data", sizeof(char) * 5);
    }
#line 3099 "yacc_sql.cpp"
    break;

  case 118: /* ID: LISTS  */
#line 1145 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 6);
      memcpy((yyval.string), "lists", sizeof(char) * 6);
    }
#line 3108 "yacc_sql.cpp"
    break;

  case 119: /* ID: TYPE  */
#line 1150 "yacc_sql.y"
    {
      (yyval.string) = (char *)malloc(sizeof(char) * 5);
      memcpy((yyval.string), "type", sizeof(char) * 5);
    }
#line 3117 "yacc_sql.cpp"
    break;

  case 120: /* rel_attr: ID  */
#line 1157 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 3127 "yacc_sql.cpp"
    break;

  case 121: /* rel_attr: ID DOT ID  */
#line 1162 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 3139 "yacc_sql.cpp"
    break;

  case 122: /* rel_attr: ID DOT '*'  */
#line 1169 "yacc_sql.y"
                 {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = '*';
      free((yyvsp[-2].string));
    }
#line 3150 "yacc_sql.cpp"
    break;

  case 123: /* relation: ID  */
#line 1178 "yacc_sql.y"
       {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3158 "yacc_sql.cpp"
    break;

  case 124: /* rel_list: relation alias join_list  */
#line 1183 "yacc_sql.y"
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
#line 3179 "yacc_sql.cpp"
    break;

  case 125: /* rel_list: relation alias join_list COMMA rel_list  */
#line 1199 "yacc_sql.y"
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
#line 3213 "yacc_sql.cpp"
    break;

  case 126: /* join_list: %empty  */
#line 1232 "yacc_sql.y"
    {
      (yyval.join_list) = nullptr;
    }
#line 3221 "yacc_sql.cpp"
    break;

  case 127: /* join_list: INNER JOIN relation alias on join_list  */
#line 1236 "yacc_sql.y"
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
#line 3249 "yacc_sql.cpp"
    break;

  case 128: /* alias: %empty  */
#line 1262 "yacc_sql.y"
                {
      (yyval.string) = nullptr;
    }
#line 3257 "yacc_sql.cpp"
    break;

  case 129: /* alias: ID  */
#line 1265 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3265 "yacc_sql.cpp"
    break;

  case 130: /* alias: AS ID  */
#line 1268 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 3273 "yacc_sql.cpp"
    break;

  case 131: /* on: %empty  */
#line 1275 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3281 "yacc_sql.cpp"
    break;

  case 132: /* on: ON condition_list  */
#line 1279 "yacc_sql.y"
    {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3289 "yacc_sql.cpp"
    break;

  case 133: /* where: %empty  */
#line 1287 "yacc_sql.y"
    {
      (yyval.condition_list) = nullptr;
    }
#line 3297 "yacc_sql.cpp"
    break;

  case 134: /* where: WHERE condition_list  */
#line 1290 "yacc_sql.y"
                           {
      (yyval.condition_list) = (yyvsp[0].condition_list);
    }
#line 3305 "yacc_sql.cpp"
    break;

  case 135: /* condition_list: condition  */
#line 1295 "yacc_sql.y"
              {
      (yyval.condition_list) = new Conditions;
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[0].condition)));
      delete (yyvsp[0].condition);
    }
#line 3315 "yacc_sql.cpp"
    break;

  case 136: /* condition_list: condition AND condition_list  */
#line 1300 "yacc_sql.y"
                                   {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[-2].condition)));
      delete (yyvsp[-2].condition);
    }
#line 3325 "yacc_sql.cpp"
    break;

  case 137: /* condition_list: condition OR condition_list  */
#line 1305 "yacc_sql.y"
                                  {
      (yyval.condition_list) = (yyvsp[0].condition_list);
      (yyval.condition_list)->and_or = true;
      (yyval.condition_list)->conditions.emplace_back(move(*(yyvsp[-2].condition)));
      delete (yyvsp[-2].condition);
    }
#line 3336 "yacc_sql.cpp"
    break;

  case 138: /* condition: expression comp_op expression  */
#line 1314 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = unique_ptr<Expression>((yyvsp[-2].expression));
      (yyval.condition)->right_expr = unique_ptr<Expression>((yyvsp[0].expression));
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3347 "yacc_sql.cpp"
    break;

  case 139: /* condition: MATCH LBRACE rel_attr RBRACE AGAINST LBRACE SSS RBRACE  */
#line 1321 "yacc_sql.y"
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
#line 3364 "yacc_sql.cpp"
    break;

  case 140: /* condition: unary_op expression  */
#line 1334 "yacc_sql.y"
    {
      (yyval.condition) = new ConditionSqlNode;
      (yyval.condition)->left_expr = unique_ptr<Expression>(new ValueExpr(Value((void*)nullptr)));
      (yyval.condition)->right_expr = unique_ptr<Expression>((yyvsp[0].expression));
      (yyval.condition)->comp = (yyvsp[-1].comp);
    }
#line 3375 "yacc_sql.cpp"
    break;

  case 141: /* comp_op: LIKE  */
#line 1343 "yacc_sql.y"
         { (yyval.comp) = LIKE_OP; }
#line 3381 "yacc_sql.cpp"
    break;

  case 142: /* comp_op: NOT LIKE  */
#line 1344 "yacc_sql.y"
               { (yyval.comp) = NOT_LIKE; }
#line 3387 "yacc_sql.cpp"
    break;

  case 143: /* comp_op: EQ  */
#line 1345 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 3393 "yacc_sql.cpp"
    break;

  case 144: /* comp_op: LT  */
#line 1346 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 3399 "yacc_sql.cpp"
    break;

  case 145: /* comp_op: GT  */
#line 1347 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 3405 "yacc_sql.cpp"
    break;

  case 146: /* comp_op: LE  */
#line 1348 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 3411 "yacc_sql.cpp"
    break;

  case 147: /* comp_op: GE  */
#line 1349 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 3417 "yacc_sql.cpp"
    break;

  case 148: /* comp_op: NE  */
#line 1350 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 3423 "yacc_sql.cpp"
    break;

  case 149: /* comp_op: IN  */
#line 1351 "yacc_sql.y"
         { (yyval.comp) = IN_OP; }
#line 3429 "yacc_sql.cpp"
    break;

  case 150: /* comp_op: NOT IN  */
#line 1352 "yacc_sql.y"
             { (yyval.comp) = NOT_IN; }
#line 3435 "yacc_sql.cpp"
    break;

  case 151: /* comp_op: IS  */
#line 1353 "yacc_sql.y"
         { (yyval.comp) = IS_NULL; }
#line 3441 "yacc_sql.cpp"
    break;

  case 152: /* comp_op: IS NOT  */
#line 1354 "yacc_sql.y"
             { (yyval.comp) = IS_NOT_NULL; }
#line 3447 "yacc_sql.cpp"
    break;

  case 153: /* unary_op: EXISTS  */
#line 1358 "yacc_sql.y"
           { (yyval.comp) = EXISTS_OP; }
#line 3453 "yacc_sql.cpp"
    break;

  case 154: /* unary_op: NOT EXISTS  */
#line 1359 "yacc_sql.y"
                 { (yyval.comp) = NOT_EXISTS; }
#line 3459 "yacc_sql.cpp"
    break;

  case 155: /* group_by: %empty  */
#line 1365 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 3467 "yacc_sql.cpp"
    break;

  case 156: /* group_by: GROUP BY expression_list  */
#line 1369 "yacc_sql.y"
    {
      (yyval.expression_list) = (yyvsp[0].expression_list);
    }
#line 3475 "yacc_sql.cpp"
    break;

  case 157: /* having_node: %empty  */
#line 1376 "yacc_sql.y"
    {
      (yyval.having_list) = nullptr;
    }
#line 3483 "yacc_sql.cpp"
    break;

  case 158: /* having_node: HAVING having_list  */
#line 1380 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
    }
#line 3491 "yacc_sql.cpp"
    break;

  case 159: /* having_list: having_unit  */
#line 1387 "yacc_sql.y"
    {
      (yyval.having_list) = new HavingNode;
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[0].expression)));
    }
#line 3500 "yacc_sql.cpp"
    break;

  case 160: /* having_list: having_unit OR having_list  */
#line 1392 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[-2].expression)));
      (yyval.having_list)->and_or = true;
    }
#line 3510 "yacc_sql.cpp"
    break;

  case 161: /* having_list: having_unit AND having_list  */
#line 1398 "yacc_sql.y"
    {
      (yyval.having_list) = (yyvsp[0].having_list);
      (yyval.having_list)->having_list.emplace_back(move((yyvsp[-2].expression)));
    }
#line 3519 "yacc_sql.cpp"
    break;

  case 162: /* having_unit: expression comp_op expression  */
#line 1406 "yacc_sql.y"
    {
      unique_ptr<Expression> left((yyvsp[-2].expression));
      unique_ptr<Expression> right((yyvsp[0].expression));
      (yyval.expression) = new ComparisonExpr((yyvsp[-1].comp), std::move(left), std::move(right));
    }
#line 3529 "yacc_sql.cpp"
    break;

  case 163: /* order_by: %empty  */
#line 1415 "yacc_sql.y"
    {
      (yyval.order_by_list) = nullptr;
    }
#line 3537 "yacc_sql.cpp"
    break;

  case 164: /* order_by: ORDER BY order_by_list  */
#line 1419 "yacc_sql.y"
    {
      (yyval.order_by_list) = (yyvsp[0].order_by_list);
    }
#line 3545 "yacc_sql.cpp"
    break;

  case 165: /* order_by_list: order_by_unit  */
#line 1426 "yacc_sql.y"
    {
      (yyval.order_by_list) = new std::vector<OrderByNode>;
      (yyval.order_by_list)->emplace_back(move(*(yyvsp[0].order_by_unit)));
      delete (yyvsp[0].order_by_unit);
    }
#line 3555 "yacc_sql.cpp"
    break;

  case 166: /* order_by_list: order_by_unit COMMA order_by_list  */
#line 1432 "yacc_sql.y"
    {
      if((yyvsp[0].order_by_list) != nullptr){
        (yyval.order_by_list) = (yyvsp[0].order_by_list);
      } else {
        (yyval.order_by_list) = new std::vector<OrderByNode>;
      }
      (yyval.order_by_list)->emplace((yyval.order_by_list)->begin(), move(*(yyvsp[-2].order_by_unit)));
      delete (yyvsp[-2].order_by_unit);
    }
#line 3569 "yacc_sql.cpp"
    break;

  case 167: /* order_by_unit: expression  */
#line 1445 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = true;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[0].expression));
    }
#line 3579 "yacc_sql.cpp"
    break;

  case 168: /* order_by_unit: expression ASC  */
#line 1451 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = true;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[-1].expression));
    }
#line 3589 "yacc_sql.cpp"
    break;

  case 169: /* order_by_unit: expression DESC  */
#line 1457 "yacc_sql.y"
    {
      (yyval.order_by_unit) = new OrderByNode;
      (yyval.order_by_unit)->is_asc = false;
      (yyval.order_by_unit)->expression = unique_ptr<Expression>((yyvsp[-1].expression));
    }
#line 3599 "yacc_sql.cpp"
    break;

  case 170: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 1466 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);

      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 3613 "yacc_sql.cpp"
    break;

  case 171: /* explain_stmt: EXPLAIN command_wrapper  */
#line 1479 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 3622 "yacc_sql.cpp"
    break;

  case 172: /* set_variable_stmt: SET ID EQ value  */
#line 1487 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 3634 "yacc_sql.cpp"
    break;


#line 3638 "yacc_sql.cpp"

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

#line 1499 "yacc_sql.y"

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
