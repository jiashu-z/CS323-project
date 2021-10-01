/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_HOME_MAOZUNYAO_CLIONPROJECTS_CS323_PROJECT1_SRC_PARSER_HPP_INCLUDED
# define YY_YY_HOME_MAOZUNYAO_CLIONPROJECTS_CS323_PROJECT1_SRC_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LOWER_THAN_ELSE = 258,
    ELSE = 259,
    TYPE = 260,
    STRUCT = 261,
    IF = 262,
    WHILE = 263,
    RETURN = 264,
    CHAR = 265,
    INT = 266,
    FLOAT = 267,
    ID = 268,
    ASSIGN = 269,
    OR = 270,
    AND = 271,
    NE = 272,
    EQ = 273,
    GE = 274,
    GT = 275,
    LE = 276,
    LT = 277,
    MINUS = 278,
    PLUS = 279,
    MUL = 280,
    DIV = 281,
    NOT = 282,
    LP = 283,
    RP = 284,
    LB = 285,
    RB = 286,
    DOT = 287,
    SEMI = 288,
    COMMA = 289,
    LC = 290,
    RC = 291
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "src/parser.y" /* yacc.c:1909  */

    SyntaxTreeNode* node_type;

#line 95 "/home/maozunyao/CLionProjects/CS323-project1/src/parser.hpp" /* yacc.c:1909  */
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


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_HOME_MAOZUNYAO_CLIONPROJECTS_CS323_PROJECT1_SRC_PARSER_HPP_INCLUDED  */
