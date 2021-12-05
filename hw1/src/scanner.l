%{
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define LIST                strCat(yytext)
#define TOKEN(t)            { LIST; if (opt_tok) printf("<%s>\n", t); } // #t
#define TOKEN_CHAR(t)       { LIST; if (opt_tok) printf("<%c>\n", (t)); }
#define TOKEN_STRING(t, s)  { LIST; if (opt_tok) printf("<%s: %s>\n", #t, (s)); }
#define TOKEN_KW(t)         { LIST; if (opt_tok) printf("<KW%s>\n", t); }
#define TOKEN_SPACE()       { LIST; }
#define MAX_LINE_LENG       512

uint32_t line_num = 1;
char buffer[MAX_LINE_LENG];

static int32_t opt_src = 1;
static int32_t opt_tok = 1;
static char *buffer_ptr = buffer;

static void strCat(const char *Text);

%}


space       [ \t]+
digit       [0-9]
letter      [a-zA-Z]
delimiter   (","|";"|":"|"("|")"|"["|"]")
keyword     ("array"|"begin"|"boolean"|"def"|"do"|"else"|"end"|"false"|"for"|"integer"|"if"|"of"|"print"|"read"|"real"|"string"|"then"|"to"|"true"|"return"|"var"|"while")
integer     (0|[1-9]{digit}*)
oct_integer [0][0-7]+
float       ([0]|[1-9]{digit}*)\.([0]|{digit}*[1-9])
scientific  ([1-9]{digit}*|{float})[eE][\+\-]?([0]|[1-9]{digit}*)
operator    ("+"|"-"|"*"|"/"|"mod"|":="|"<>"|"<"|"<="|"="|">="|">"|"and"|"or"|"not")
identifier  {letter}({digit}|{letter})*
string      \"([^\"]|([\"][\"]))*\"

%Start comment_line
%Start comment_all
%Start sflag
%Start tflag

%%

<INITIAL>"//&S+" {
        LIST;
        opt_src = 1;
        BEGIN sflag;
}

<INITIAL>"//&S-" {
        LIST;
        opt_src = 0;
        BEGIN sflag;
}

<INITIAL>"//&T+" {
        LIST;
        opt_tok = 1;
        BEGIN tflag;
}

<INITIAL>"//&T-" {
        LIST;
        opt_tok = 0;
        BEGIN tflag;
}

<INITIAL>"//"  {
                LIST;
                BEGIN comment_line;
               }

<INITIAL>"/*"  {
                LIST;
                BEGIN comment_all;
               }
               
<sflag,tflag>"/*"  {
                LIST;
                BEGIN comment_all;
}
               
<sflag,tflag>"\n" {
        if (opt_src) {
            printf("%d: %s\n", line_num, buffer);
            
        }
        ++line_num;
        buffer[0] = '\0';
        buffer_ptr = buffer;
        BEGIN 0;
}
<sflag,tflag>. {LIST; }

<sflag,tflag>"//" {
        LIST;
        BEGIN comment_line;
}

<comment_line>. {
    LIST;
}

<comment_line>"\n" {
    //LIST;
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
        
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
    BEGIN 0;
}

<comment_all>. {
    LIST;
}

<comment_all>"*/" {
    LIST;
    BEGIN 0;
}



<INITIAL>{space}       {TOKEN_SPACE(); }
<INITIAL>{delimiter}   {TOKEN(yytext); }
<INITIAL>{keyword}     {TOKEN_KW(yytext); }
<INITIAL>{integer}     {TOKEN_STRING(integer, yytext); }
<INITIAL>{oct_integer} {TOKEN_STRING(oct_integer, yytext); }
<INITIAL>{float}       {TOKEN_STRING(float, yytext); }
<INITIAL>{scientific}  {TOKEN_STRING(scientific, yytext); }
<INITIAL>{operator}    {TOKEN(yytext); }
<INITIAL>{identifier}  {TOKEN_STRING(id, yytext); }
<INITIAL>{string}      {
                            char *wd = malloc(yyleng*sizeof(char));
                            for(int i = 0, j = 1; j < yyleng; j++){
                                if(j >= yyleng - 1) {
                                    wd[i] = '\0';
                                    break;
                                }else{
                                    if(yytext[j] == '\"' && yytext[j+1] == '\"'){
                                        wd[i++] = '\"';
                                        j += 1;
                                    }else{
                                        wd[i++] = yytext[j];
                                    }
                                }
                                
                            }
                            TOKEN_STRING(string, wd); 
                       }






\n {
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
        
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}

    /* Catch the character which is not accepted by rules above */
<INITIAL>. {
    printf("Error at line %d: bad character \"%s\"\n", line_num, yytext);
    exit(-1);
}

%%

static void strCat(const char *Text) {
    while (*Text) {
        *buffer_ptr++ = *Text++;
    }
    *buffer_ptr = '\0';
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./scanner [filename]\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    return 0;
}
