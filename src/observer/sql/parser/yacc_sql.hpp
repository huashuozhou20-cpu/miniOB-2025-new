/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_YACC_SQL_HPP_INCLUDED
# define YY_YY_YACC_SQL_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SEMICOLON = 258,               /* SEMICOLON  */
    BY = 259,                      /* BY  */
    CREATE = 260,                  /* CREATE  */
    DROP = 261,                    /* DROP  */
    GROUP = 262,                   /* GROUP  */
    ORDER = 263,                   /* ORDER  */
    HAVING = 264,                  /* HAVING  */
    ASC = 265,                     /* ASC  */
    DESC = 266,                    /* DESC  */
    TABLE = 267,                   /* TABLE  */
    VIEW = 268,                    /* VIEW  */
    TABLES = 269,                  /* TABLES  */
    INDEX = 270,                   /* INDEX  */
    CALC = 271,                    /* CALC  */
    SELECT = 272,                  /* SELECT  */
    SHOW = 273,                    /* SHOW  */
    SYNC = 274,                    /* SYNC  */
    WITH = 275,                    /* WITH  */
    INSERT = 276,                  /* INSERT  */
    DELETE = 277,                  /* DELETE  */
    UPDATE = 278,                  /* UPDATE  */
    LBRACE = 279,                  /* LBRACE  */
    RBRACE = 280,                  /* RBRACE  */
    LMBRACE = 281,                 /* LMBRACE  */
    RMBRACE = 282,                 /* RMBRACE  */
    COMMA = 283,                   /* COMMA  */
    TRX_BEGIN = 284,               /* TRX_BEGIN  */
    TRX_COMMIT = 285,              /* TRX_COMMIT  */
    TRX_ROLLBACK = 286,            /* TRX_ROLLBACK  */
    INT_T = 287,                   /* INT_T  */
    STRING_T = 288,                /* STRING_T  */
    FLOAT_T = 289,                 /* FLOAT_T  */
    DATE_T = 290,                  /* DATE_T  */
    VECTOR_T = 291,                /* VECTOR_T  */
    VECTOR = 292,                  /* VECTOR  */
    TEXT_T = 293,                  /* TEXT_T  */
    HELP = 294,                    /* HELP  */
    EXIT = 295,                    /* EXIT  */
    DOT = 296,                     /* DOT  */
    INTO = 297,                    /* INTO  */
    VALUES = 298,                  /* VALUES  */
    FROM = 299,                    /* FROM  */
    WHERE = 300,                   /* WHERE  */
    INNER = 301,                   /* INNER  */
    JOIN = 302,                    /* JOIN  */
    AND = 303,                     /* AND  */
    OR = 304,                      /* OR  */
    SET = 305,                     /* SET  */
    ON = 306,                      /* ON  */
    AS = 307,                      /* AS  */
    LOAD = 308,                    /* LOAD  */
    DATA = 309,                    /* DATA  */
    LIKE = 310,                    /* LIKE  */
    NULL_T = 311,                  /* NULL_T  */
    EXISTS = 312,                  /* EXISTS  */
    IN = 313,                      /* IN  */
    IS = 314,                      /* IS  */
    NOT = 315,                     /* NOT  */
    INFILE = 316,                  /* INFILE  */
    EXPLAIN = 317,                 /* EXPLAIN  */
    STORAGE = 318,                 /* STORAGE  */
    FORMAT = 319,                  /* FORMAT  */
    LIMIT = 320,                   /* LIMIT  */
    EQ = 321,                      /* EQ  */
    LT = 322,                      /* LT  */
    GT = 323,                      /* GT  */
    LE = 324,                      /* LE  */
    GE = 325,                      /* GE  */
    NE = 326,                      /* NE  */
    UNIQUE = 327,                  /* UNIQUE  */
    UNION = 328,                   /* UNION  */
    ALTER = 329,                   /* ALTER  */
    ADD = 330,                     /* ADD  */
    COLUMN = 331,                  /* COLUMN  */
    RENAME = 332,                  /* RENAME  */
    TO = 333,                      /* TO  */
    FULLTEXT = 334,                /* FULLTEXT  */
    ALL = 335,                     /* ALL  */
    L2_DISTANCE = 336,             /* L2_DISTANCE  */
    COSINE_DISTANCE = 337,         /* COSINE_DISTANCE  */
    INNER_PRODUCT = 338,           /* INNER_PRODUCT  */
    DISTANCE = 339,                /* DISTANCE  */
    LISTS = 340,                   /* LISTS  */
    TYPE = 341,                    /* TYPE  */
    PROBES = 342,                  /* PROBES  */
    IVFFLAT = 343,                 /* IVFFLAT  */
    MATCH = 344,                   /* MATCH  */
    AGAINST = 345,                 /* AGAINST  */
    NUMBER = 346,                  /* NUMBER  */
    FLOAT = 347,                   /* FLOAT  */
    ID_KEY = 348,                  /* ID_KEY  */
    SSS = 349,                     /* SSS  */
    DATE_VALUE = 350,              /* DATE_VALUE  */
    UMINUS = 351                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 190 "yacc_sql.y"

  ParsedSqlNode *                            sql_node;
  ConditionSqlNode *                         condition;
  HavingNode *                               having_list;
  Value *                                    value;
  enum CompOp                                comp;
  RelAttrSqlNode *                           rel_attr;
  std::vector<AttrInfoSqlNode> *             attr_infos;
  AttrInfoSqlNode *                          attr_info;
  Expression *                               expression;
  std::vector<std::unique_ptr<Expression>> * expression_list;
  std::vector<OrderByNode> *                 order_by_list;
  OrderByNode *                              order_by_unit;
  std::vector<Value> *                       value_list;
  std::vector<std::vector<Value>> *          values_list;
  Conditions *                               condition_list;
  std::vector<RelAttrSqlNode> *              rel_attr_list;
  std::vector<std::string> *                 relation_list;
  char *                                     string;
  int                                        number;
  float                                      floats;
  bool                                       boolean;
  Joins *                                    join_list;
  Key_values *                               key_values;
  enum VectorIndexType                       vector_index_type;

#line 187 "yacc_sql.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif




int yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner);


#endif /* !YY_YY_YACC_SQL_HPP_INCLUDED  */
