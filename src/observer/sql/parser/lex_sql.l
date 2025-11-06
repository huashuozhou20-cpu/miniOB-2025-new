%top{
/*
这里的代码会被复制到lex_sql.cpp的最开始位置
定义yy_size_t的原因是因为flex生成的代码，会使用yy_size_t与其他类型的数字
做比较，导致编译报警
*/
#define YY_TYPEDEF_YY_SIZE_T
typedef int yy_size_t;

/* 参考生成的lex_sql.cpp代码，这个宏定义会放在每次运行yylex()最开始的地方 */
#define YY_USER_INIT                                         \
  yycolumn = 0;

/* 参考生成的lex_sql.cpp代码，这个宏定义会放在解析一个token之后，也可以在网上找到大量的参考资料 */
/* 我们在这里设置当前解析的token的位置信息，这样在yacc中就可以使用这些信息了 */
#define YY_USER_ACTION                                       \
  do {                                                       \
    yylloc->first_line   = yylineno + 1;                     \
    yylloc->first_column = yycolumn + 1;                     \
    yylloc->last_line    = yylineno + 1;                     \
    yylloc->last_column  = yycolumn + yyleng;                \
    yycolumn += yyleng;                                      \
  } while (0);

}

%{
#include<string.h>
#include<stdio.h>

/**
 * flex 代码包含三个部分，使用 %% 分隔
 * 第一个部分的代码是C代码，flex会原样复制到目标文件中
 * 第二个部分是规则部分，使用正则表达式定义一系列规则
 * 第三个部分还是C代码，flex 会复制此段代码
 */

#include "sql/parser/parse_defs.h"
#include "common/log/log.h"
#include "yacc_sql.hpp"

#ifndef register
#define register
#endif // register

extern int atoi();
extern double atof();

#define RETURN_TOKEN(token) LOG_DEBUG("%s", #token);return token
%}

/* Prevent the need for linking with -lfl */
%option noyywrap
%option nounput
%option noinput
%option bison-bridge
%option reentrant
/* 不区分大小写 */
%option case-insensitive
%option bison-locations

WHITE_SPACE      [ \t\r\f]+
NEWLINE          \n
DIGIT            [0-9]
INT              {DIGIT}+
ID_KEY           [A-Za-z_][A-Za-z0-9_]*
DOT              \.
%x STR COMMENT

/* 规则匹配的优先级：*/
/* 1. 匹配的规则长的优先 */
/* 2. 写在最前面的优先 */

/* yylval 就可以认为是 yacc 中 %union 定义的结构体(union 结构) */
%%

{WHITE_SPACE}                  { /* skip spaces */ }
{NEWLINE}                      { yycolumn = 0; /* keep yylineno */ }
--[^\n]*                       { /* SQL single-line comment */ }

"/*"                           { BEGIN(COMMENT); }
<COMMENT>"*/"                  { BEGIN(INITIAL); }
<COMMENT>\n                    { yycolumn = 0; }
<COMMENT>.                     { /* skip */ }

ADD                                     RETURN_TOKEN(ADD);
AGAINST                                 RETURN_TOKEN(AGAINST);
ALL                                     RETURN_TOKEN(ALL);
ALTER                                   RETURN_TOKEN(ALTER);
AND                                     RETURN_TOKEN(AND);
AS                                      RETURN_TOKEN(AS);
ASC                                     RETURN_TOKEN(ASC);
BY                                      RETURN_TOKEN(BY);
CALC                                    RETURN_TOKEN(CALC);
COLUMN                                  RETURN_TOKEN(COLUMN);
COSINE_DISTANCE                         RETURN_TOKEN(COSINE_DISTANCE);
CREATE                                  RETURN_TOKEN(CREATE);
DATA                                    RETURN_TOKEN(DATA);
DATE_T                                  RETURN_TOKEN(DATE_T);
DELETE                                  RETURN_TOKEN(DELETE);
DESC                                    RETURN_TOKEN(DESC);
DISTANCE                                RETURN_TOKEN(DISTANCE);
DROP                                    RETURN_TOKEN(DROP);
EXISTS                                  RETURN_TOKEN(EXISTS);
EXIT                                    RETURN_TOKEN(EXIT);
EXPLAIN                                 RETURN_TOKEN(EXPLAIN);
FLOAT_T                                 RETURN_TOKEN(FLOAT_T);
FORMAT                                  RETURN_TOKEN(FORMAT);
FROM                                    RETURN_TOKEN(FROM);
FULLTEXT                                RETURN_TOKEN(FULLTEXT);
GROUP                                   RETURN_TOKEN(GROUP);
HAVING                                  RETURN_TOKEN(HAVING);
HELP                                    RETURN_TOKEN(HELP);
IN                                      RETURN_TOKEN(IN);
INDEX                                   RETURN_TOKEN(INDEX);
INFILE                                  RETURN_TOKEN(INFILE);
INNER                                   RETURN_TOKEN(INNER);
INNER_PRODUCT                           RETURN_TOKEN(INNER_PRODUCT);
INSERT                                  RETURN_TOKEN(INSERT);
INTO                                    RETURN_TOKEN(INTO);
INT_T                                   RETURN_TOKEN(INT_T);
IS                                      RETURN_TOKEN(IS);
IVFFLAT                                 RETURN_TOKEN(IVFFLAT);
JOIN                                    RETURN_TOKEN(JOIN);
L2_DISTANCE                             RETURN_TOKEN(L2_DISTANCE);
LIMIT                                   RETURN_TOKEN(LIMIT);
LIKE                                    RETURN_TOKEN(LIKE);
LISTS                                   RETURN_TOKEN(LISTS);
LOAD                                    RETURN_TOKEN(LOAD);
MATCH                                   RETURN_TOKEN(MATCH);
NOT                                     RETURN_TOKEN(NOT);
NULL_T                                  RETURN_TOKEN(NULL_T);
ON                                      RETURN_TOKEN(ON);
OR                                      RETURN_TOKEN(OR);
ORDER                                   RETURN_TOKEN(ORDER);
PROBES                                  RETURN_TOKEN(PROBES);
RENAME                                  RETURN_TOKEN(RENAME);
SELECT                                  RETURN_TOKEN(SELECT);
SET                                     RETURN_TOKEN(SET);
SHOW                                    RETURN_TOKEN(SHOW);
STRING_T                                RETURN_TOKEN(STRING_T);
STORAGE                                 RETURN_TOKEN(STORAGE);
SYNC                                    RETURN_TOKEN(SYNC);
TABLE                                   RETURN_TOKEN(TABLE);
TABLES                                  RETURN_TOKEN(TABLES);
TEXT_T                                  RETURN_TOKEN(TEXT_T);
TO                                      RETURN_TOKEN(TO);
BEGIN                                   RETURN_TOKEN(TRX_BEGIN);
COMMIT                                  RETURN_TOKEN(TRX_COMMIT);
ROLLBACK                                RETURN_TOKEN(TRX_ROLLBACK);
TYPE                                    RETURN_TOKEN(TYPE);
UNION                                   RETURN_TOKEN(UNION);
UNIQUE                                  RETURN_TOKEN(UNIQUE);
UPDATE                                  RETURN_TOKEN(UPDATE);
VALUES                                  RETURN_TOKEN(VALUES);
VECTOR_T                                RETURN_TOKEN(VECTOR_T);
VECTOR                                  RETURN_TOKEN(VECTOR_T);
VIEW                                    RETURN_TOKEN(VIEW);
WHERE                                   RETURN_TOKEN(WHERE);
WITH                                    RETURN_TOKEN(WITH);
TEXT                                    RETURN_TOKEN(TEXT_T);

{ID_KEY}                                yylval->string=strdup(yytext); RETURN_TOKEN(ID_KEY);
"("                                     RETURN_TOKEN(LBRACE);
")"                                     RETURN_TOKEN(RBRACE);
"["                                     RETURN_TOKEN(LMBRACE);
"]"                                     RETURN_TOKEN(RMBRACE);
","                                     RETURN_TOKEN(COMMA);
";"                                     RETURN_TOKEN(SEMICOLON);
"."                                     RETURN_TOKEN(DOT);

"<="                                    RETURN_TOKEN(LE);
"<>"                                    RETURN_TOKEN(NE);
"!="                                    RETURN_TOKEN(NE);
">="                                    RETURN_TOKEN(GE);
"="                                     RETURN_TOKEN(EQ);
"<"                                     RETURN_TOKEN(LT);
">"                                     RETURN_TOKEN(GT);

"+" |
"-" |
"*" |
"/"                                     { return yytext[0]; }

({INT}\.{INT}*|{INT}*\.{INT})([eE][+-]?{INT})?   { yylval->floats=atof(yytext); RETURN_TOKEN(FLOAT); }
{INT}                                     { yylval->number=atoi(yytext); RETURN_TOKEN(NUMBER); }

\"[0-9]{4}\-[0-9]{1,2}\-[0-9]{1,2}\"    yylval->string = strdup(yytext); RETURN_TOKEN(DATE_VALUE);
\'[0-9]{4}\-[0-9]{1,2}\-[0-9]{1,2}\'    yylval->string = strdup(yytext); RETURN_TOKEN(DATE_VALUE);

\"([^\\\n"]|\\.)*\"                     yylval->string = strdup(yytext); RETURN_TOKEN(SSS);
\'([^\\\n']|\\.)*\'                     yylval->string = strdup(yytext); RETURN_TOKEN(SSS);

.                                        LOG_DEBUG("Unknown character [%c]",yytext[0]); return yytext[0];
%%

void scan_string(const char *str, yyscan_t scanner) {
  yy_switch_to_buffer(yy_scan_string(str, scanner), scanner);
}
