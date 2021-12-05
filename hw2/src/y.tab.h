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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    ID = 258,
    MOD = 259,
    ASSIGN = 260,
    LT = 261,
    LE = 262,
    NE = 263,
    GE = 264,
    GT = 265,
    EQ = 266,
    AND = 267,
    OR = 268,
    NOT = 269,
    ARRAY = 270,
    BEG = 271,
    DEF = 272,
    DO = 273,
    ELSE = 274,
    END = 275,
    FALSE = 276,
    FOR = 277,
    IF = 278,
    INT = 279,
    OF = 280,
    PRINT = 281,
    READ = 282,
    THEN = 283,
    TO = 284,
    TRUE = 285,
    TYPE = 286,
    RETURN = 287,
    VAR = 288,
    WHILE = 289,
    DEC = 290,
    OCT = 291,
    FLOAT = 292,
    STR = 293,
    SCI = 294
  };
#endif
/* Tokens.  */
#define ID 258
#define MOD 259
#define ASSIGN 260
#define LT 261
#define LE 262
#define NE 263
#define GE 264
#define GT 265
#define EQ 266
#define AND 267
#define OR 268
#define NOT 269
#define ARRAY 270
#define BEG 271
#define DEF 272
#define DO 273
#define ELSE 274
#define END 275
#define FALSE 276
#define FOR 277
#define IF 278
#define INT 279
#define OF 280
#define PRINT 281
#define READ 282
#define THEN 283
#define TO 284
#define TRUE 285
#define TYPE 286
#define RETURN 287
#define VAR 288
#define WHILE 289
#define DEC 290
#define OCT 291
#define FLOAT 292
#define STR 293
#define SCI 294

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
