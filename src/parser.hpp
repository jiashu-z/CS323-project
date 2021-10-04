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

#ifndef YY_YY_USERS_BYTEDANCE_CS323_PROJECT1_SRC_PARSER_HPP_INCLUDED
# define YY_YY_USERS_BYTEDANCE_CS323_PROJECT1_SRC_PARSER_HPP_INCLUDED
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
    LOWER_THAN_ELSE = 258,         /* LOWER_THAN_ELSE  */
    ELSE = 259,                    /* ELSE  */
    TYPE = 260,                    /* TYPE  */
    STRUCT = 261,                  /* STRUCT  */
    IF = 262,                      /* IF  */
    WHILE = 263,                   /* WHILE  */
    RETURN = 264,                  /* RETURN  */
    CHAR = 265,                    /* CHAR  */
    INT = 266,                     /* INT  */
    FLOAT = 267,                   /* FLOAT  */
    ID = 268,                      /* ID  */
    ASSIGN = 269,                  /* ASSIGN  */
    OR = 270,                      /* OR  */
    AND = 271,                     /* AND  */
    NE = 272,                      /* NE  */
    EQ = 273,                      /* EQ  */
    GE = 274,                      /* GE  */
    GT = 275,                      /* GT  */
    LE = 276,                      /* LE  */
    LT = 277,                      /* LT  */
    MINUS = 278,                   /* MINUS  */
    PLUS = 279,                    /* PLUS  */
    MUL = 280,                     /* MUL  */
    DIV = 281,                     /* DIV  */
    NOT = 282,                     /* NOT  */
    LP = 283,                      /* LP  */
    RP = 284,                      /* RP  */
    LB = 285,                      /* LB  */
    RB = 286,                      /* RB  */
    DOT = 287,                     /* DOT  */
    SEMI = 288,                    /* SEMI  */
    COMMA = 289,                   /* COMMA  */
    LC = 290,                      /* LC  */
    RC = 291                       /* RC  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "src/parser.y"

    SyntaxTreeNode* node_type;

#line 104 "/Users/bytedance/CS323-project1/src/parser.hpp"

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


#endif /* !YY_YY_USERS_BYTEDANCE_CS323_PROJECT1_SRC_PARSER_HPP_INCLUDED  */
