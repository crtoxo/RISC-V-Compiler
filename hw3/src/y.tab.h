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
/* "%code requires" blocks.  */
#line 69 "parser.y" /* yacc.c:1909  */

    class AstNode;
#line 74 "parser.y" /* yacc.c:1909  */
 #include "AST/ast.hpp" 
#line 75 "parser.y" /* yacc.c:1909  */
 #include "AST/program.hpp" 
#line 76 "parser.y" /* yacc.c:1909  */
 #include "AST/decl.hpp" 
#line 77 "parser.y" /* yacc.c:1909  */
 #include "AST/variable.hpp"
#line 78 "parser.y" /* yacc.c:1909  */
#include "AST/ConstantValue.hpp"
#line 79 "parser.y" /* yacc.c:1909  */
#include "AST/function.hpp"
#line 80 "parser.y" /* yacc.c:1909  */
#include "AST/CompoundStatement.hpp"
#line 81 "parser.y" /* yacc.c:1909  */
#include "AST/print.hpp"
#line 82 "parser.y" /* yacc.c:1909  */
#include "AST/expression.hpp"
#line 83 "parser.y" /* yacc.c:1909  */
#include "AST/BinaryOperator.hpp"
#line 84 "parser.y" /* yacc.c:1909  */
#include "AST/UnaryOperator.hpp"
#line 85 "parser.y" /* yacc.c:1909  */
#include "AST/FunctionInvocation.hpp"
#line 86 "parser.y" /* yacc.c:1909  */
#include "AST/VariableReference.hpp"
#line 87 "parser.y" /* yacc.c:1909  */
#include "AST/assignment.hpp"
#line 88 "parser.y" /* yacc.c:1909  */
#include "AST/read.hpp"
#line 89 "parser.y" /* yacc.c:1909  */
#include "AST/if.hpp"
#line 90 "parser.y" /* yacc.c:1909  */
#include "AST/while.hpp"
#line 91 "parser.y" /* yacc.c:1909  */
#include "AST/for.hpp"
#line 92 "parser.y" /* yacc.c:1909  */
#include "AST/return.hpp"

#line 86 "y.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    COMMA = 258,
    SEMICOLON = 259,
    COLON = 260,
    L_PARENTHESIS = 261,
    R_PARENTHESIS = 262,
    L_BRACKET = 263,
    R_BRACKET = 264,
    ASSIGN = 265,
    OR = 266,
    AND = 267,
    NOT = 268,
    LESS = 269,
    LESS_OR_EQUAL = 270,
    EQUAL = 271,
    GREATER = 272,
    GREATER_OR_EQUAL = 273,
    NOT_EQUAL = 274,
    PLUS = 275,
    MINUS = 276,
    MULTIPLY = 277,
    DIVIDE = 278,
    MOD = 279,
    UNARY_MINUS = 280,
    ARRAY = 281,
    BOOLEAN = 282,
    INTEGER = 283,
    REAL = 284,
    STRING = 285,
    END = 286,
    BEGIN_ = 287,
    DO = 288,
    ELSE = 289,
    FOR = 290,
    IF = 291,
    THEN = 292,
    WHILE = 293,
    DEF = 294,
    OF = 295,
    TO = 296,
    RETURN = 297,
    VAR = 298,
    FALSE = 299,
    TRUE = 300,
    PRINT = 301,
    READ = 302,
    ID = 303,
    INT_LITERAL = 304,
    REAL_LITERAL = 305,
    STRING_LITERAL = 306
  };
#endif
/* Tokens.  */
#define COMMA 258
#define SEMICOLON 259
#define COLON 260
#define L_PARENTHESIS 261
#define R_PARENTHESIS 262
#define L_BRACKET 263
#define R_BRACKET 264
#define ASSIGN 265
#define OR 266
#define AND 267
#define NOT 268
#define LESS 269
#define LESS_OR_EQUAL 270
#define EQUAL 271
#define GREATER 272
#define GREATER_OR_EQUAL 273
#define NOT_EQUAL 274
#define PLUS 275
#define MINUS 276
#define MULTIPLY 277
#define DIVIDE 278
#define MOD 279
#define UNARY_MINUS 280
#define ARRAY 281
#define BOOLEAN 282
#define INTEGER 283
#define REAL 284
#define STRING 285
#define END 286
#define BEGIN_ 287
#define DO 288
#define ELSE 289
#define FOR 290
#define IF 291
#define THEN 292
#define WHILE 293
#define DEF 294
#define OF 295
#define TO 296
#define RETURN 297
#define VAR 298
#define FALSE 299
#define TRUE 300
#define PRINT 301
#define READ 302
#define ID 303
#define INT_LITERAL 304
#define REAL_LITERAL 305
#define STRING_LITERAL 306

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 95 "parser.y" /* yacc.c:1909  */

    /* basic semantic value */
    char *text;
    int    val;
    double dval;
    std::string const_type;
    
    Node      node;
    NodeList* node_list_ptr;
    
    enum enumOperator op_type;
    
    struct NodeWithTypeList*           node_w_type_list_ptr;
    std::vector<struct NodeWithTypeList*>*  node_w_type_list_ptr_list_ptr;

    std::vector<struct id_info>* id_list_ptr;
    VariableInfo*           variable_info_ptr;
    

#line 220 "y.tab.h" /* yacc.c:1909  */
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

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
