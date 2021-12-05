%{
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int32_t line_num;   /* declared in scanner.l */
extern char buffer[512];  /* declared in scanner.l */
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */

extern int yylex(void); 
static void yyerror(const char *msg);
%}

%token ID 
%token MOD ASSIGN LT LE NE GE GT EQ AND OR NOT
%token ARRAY BEG DEF DO ELSE END FALSE FOR IF INT OF PRINT READ THEN TO TRUE TYPE RETURN VAR WHILE
%token DEC OCT FLOAT STR SCI

%left PARENTHESES

%left '+' '-'
%left '*' '/'
%left '>' '<'
%left NOT
%left AND
%left OR

%%

program: ID ';' multi_declaration multi_function compound_stat END
;

multi_function: %empty
              | multi_function1
;

multi_function1: function
               | multi_function1 function
;

function: basic_func function_end
;

basic_func: ID '(' multi_formal_arg ')'
;

function_end: ':' TYPE compound_stat END
            | ':' TYPE ';'
            | compound_stat END
            | ';'
;

multi_formal_arg: %empty
                | multi_formal_arg1
;                

multi_formal_arg1: formal_arg
                 | multi_formal_arg1 ',' formal_arg
;

formal_arg: id_list ':' TYPE 
;

id_list: ID | id_list ',' ID
;

declaration: VAR id_list ':' declare_end ';'
;

declare_end: ARRAY num OF declare_end 
           | STR
           | sign num
           | sign TYPE
           | FALSE
           | TRUE
;

num: DEC | OCT | FLOAT | SCI
;

constint: DEC | OCT
;

sign: '-'
    | %empty
;

compound_stat: BEG multi_declaration multi_stat END
;

multi_declaration: %empty
                 | multi_declaration1
;

multi_declaration1: declaration
                  | multi_declaration1  declaration
;

multi_stat: %empty
          | multi_stat1 
;

multi_stat1: stat
           | multi_stat  stat
;

stat: compound_stat
    | cond
    | while_loop
    | for_loop
    | return_val ';'
    | procedure_call ';'
    | var_ref ASSIGN expr ';'
    | PRINT var_ref ';'
    | PRINT expr ';'
    | READ var_ref ';'
;

var_ref: ID multi_index
;

multi_index: %empty
           | multi_index1
;

multi_index1: '[' expr ']'
            | multi_index1 '[' expr ']'
;

cond: IF expr THEN compound_stat cond_end
;

cond_end: ELSE compound_stat END IF
        | END IF
;

while_loop: WHILE expr DO compound_stat END DO
;

for_loop: FOR ID ASSIGN constint TO constint DO compound_stat END DO
;

return_val: RETURN expr 
;

procedure_call: ID '(' multi_expr ')' 
;

multi_expr: %empty
          | multi_expr1
;

multi_expr1: expr
           | multi_expr1 ',' expr
;

expr: '-' expr %prec '*'
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr reop expr
    | expr MOD expr %prec '*'
    | '(' expr ')' %prec '*'
    | NOT expr
    | num
    | ID
    | function_invoc
    | STR
    | var_ref
;

reop: LT | LE | NE | GE | GT | EQ | AND | OR |NOT
;

function_invoc:  ID '(' multi_expr ')'
;


%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, buffer, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./parser <filename>\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    assert(yyin != NULL && "fopen() fails.");

    yyparse();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}
