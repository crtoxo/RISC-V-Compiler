%{
#include "AST/ast.hpp"
#include "AST/program.hpp"
#include "AST/decl.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/function.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/print.hpp"
#include "AST/expression.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/read.hpp"
#include "AST/if.hpp"
#include "AST/while.hpp"
#include "AST/for.hpp"
#include "AST/return.hpp"
#include "AST/AstDumper.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#define YYLTYPE yyltype

typedef struct YYLTYPE {
    uint32_t first_line;
    uint32_t first_column;
    uint32_t last_line;
    uint32_t last_column;
} yyltype;

/* Declared by scanner.l */
extern uint32_t line_num;
extern char buffer[512];
extern FILE *yyin;
extern char *yytext;
/* End */

static AstNode *root;
AstDumper v;
//static AstNodeVisitor *v;


extern "C" int yylex(void);
static void yyerror(const char *msg);
extern int yylex_destroy(void);

struct id_info{
    std::string name;
    uint32_t line_number;
    uint32_t col_number;
};



%}



%code requires {
    class AstNode;
}


%code requires { #include "AST/ast.hpp" }
%code requires { #include "AST/program.hpp" }
%code requires { #include "AST/decl.hpp" }
%code requires { #include "AST/variable.hpp"}
%code requires {#include "AST/ConstantValue.hpp"}
%code requires {#include "AST/function.hpp"}
%code requires {#include "AST/CompoundStatement.hpp"}
%code requires {#include "AST/print.hpp"}
%code requires {#include "AST/expression.hpp"}
%code requires {#include "AST/BinaryOperator.hpp"}
%code requires {#include "AST/UnaryOperator.hpp"}
%code requires {#include "AST/FunctionInvocation.hpp"}
%code requires {#include "AST/VariableReference.hpp"}
%code requires {#include "AST/assignment.hpp"}
%code requires {#include "AST/read.hpp"}
%code requires {#include "AST/if.hpp"}
%code requires {#include "AST/while.hpp"}
%code requires {#include "AST/for.hpp"}
%code requires {#include "AST/return.hpp"}

    /* For yylval */
%union {
    /* basic semantic value */
    char *text;
    int    val;
    double dval;
    
    Node      node;
    NodeList* node_list_ptr;
    ConstantValueNode* constnode;
    
    enum enumOperator op_type;
    
    struct NodeWithType*           node_w_type_ptr;
    std::vector<struct NodeWithType*>*  node_w_type_list_ptr;

    std::vector<struct id_info>* id_list_ptr;
    VariableInfo*           variable_info_ptr;
    
};

%type <text> ProgramName ID

    /* Delimiter */
%token COMMA SEMICOLON COLON
%token L_PARENTHESIS R_PARENTHESIS
%token L_BRACKET R_BRACKET

    /* Operator */
%token <op_type> ASSIGN
%left <op_type> OR
%left <op_type> AND
%right <op_type> NOT
%left <op_type> LESS LESS_OR_EQUAL EQUAL GREATER GREATER_OR_EQUAL NOT_EQUAL
%left <op_type> PLUS MINUS
%left <op_type> MULTIPLY DIVIDE MOD
%right <op_type> UNARY_MINUS

    /* Keyword */
%token ARRAY BOOLEAN INTEGER REAL STRING
%token END BEGIN_ /* Use BEGIN_ since BEGIN is a keyword in lex */
%token DO ELSE FOR IF THEN WHILE
%token DEF OF TO RETURN VAR
%token <text> FALSE TRUE
%token PRINT READ

    /* Identifier */
%token ID

    /* Literal */
%token <text> INT_LITERAL
%token <text> REAL_LITERAL
%token <text> STRING_LITERAL



%type <text> FunctionName

%type <node> Declaration
%type <node_list_ptr> Declarations
%type <node_list_ptr> DeclarationList

%type <node> FunctionDeclaration
%type <node_list_ptr> Functions
%type <node_list_ptr> FunctionList

%type <node_w_type_ptr> FormalArg
%type <node_w_type_list_ptr> FormalArgs
%type <node_w_type_list_ptr> FormalArgList

%type <node> Statement
%type <node_list_ptr> Statements
%type <node_list_ptr> StatementList

%type <node> Expression
%type <node_list_ptr> Expressions
%type <node_list_ptr> ExpressionList

%type <node> VariableReference
%type <node_list_ptr> ArrForm

%type <node> Condition
%type <node> ElseOrNot

%type <node> While
%type <node> For
%type <node> Return
%type <node> FunctionInvocation
%type <node> FunctionCall

%type <node> Simple
%type <node> CompoundStatement

%type <id_list_ptr> IdList
%type <variable_info_ptr> ReturnType
%type <variable_info_ptr> Type
%type <variable_info_ptr> ScalarType
%type <variable_info_ptr> ArrType
%type <variable_info_ptr> ArrDecl
%type <variable_info_ptr> TypeOrConstant
%type <constnode> LiteralConstant

%type <node> Function
%type <node> FunctionDefinition
%type <text> NegOrNot
%type <constnode> StringAndBoolean
%type <constnode> IntegerAndReal
%type <node_list_ptr> ArrRefList
%type <node_list_ptr> ArrRefs





%%
    /*
       Program Units
                     */

Program:
    ProgramName SEMICOLON
    /* ProgramBody */
    DeclarationList FunctionList CompoundStatement
    /* End of ProgramBody */
    END {
        std::string ret_type = "void";
        root = new ProgramNode(@1.first_line, @1.first_column,
                               std::string($1),
                                ret_type,
                                $3,
                                $4,
                                $5);

    }
;

ProgramName:
    ID {
        $$ = strdup($1);

        // Memory_Free
        
    }
;

DeclarationList:
    Epsilon {
        $$ = nullptr;
    }
    |
    Declarations {
        $$ = $1;
    }
;

Declarations:
    Declaration{
        $$ = new NodeList();
        $$->push_back($1);
    }
    |
    Declarations Declaration{
        $1->push_back($2);
        $$ = $1;
    }
;

FunctionList:
    Epsilon{
        $$ = nullptr;
    }
    |
    Functions{
        $$ = $1;
    }
;

Functions:
    Function{
        $$ = new NodeList();
        $$->push_back($1);
    }
    |
    Functions Function{
        $1->push_back($2);
        $$ = $1;
    }
;

Function:
    FunctionDeclaration{
        $$ = $1;
    }
    |
    FunctionDefinition{
        $$ = $1;
    }
;

FunctionDeclaration:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType SEMICOLON{
        std::vector<VariableInfo*> prototype;
        if ($3!=nullptr){
            for(uint i=0; i<$3->size(); i++){
                for(uint j=0; j<(*$3)[i]->counter; j++){
                    VariableInfo* dupTemp = new VariableInfo();
                    dupTemp->type_set = (*$3)[i]->type->type_set;
                    dupTemp->type = (*$3)[i]->type->type;
                    dupTemp->array_size = (*$3)[i]->type->array_size;
                    
                    prototype.push_back(dupTemp);
                }   
            }
        }
        $$ = new FunctionNode(
            @1.first_line,
            @1.first_column,
            std::string($1),
            $3,
            std::string($5->text),
            nullptr,
            prototype
        );
        
        free($1);
    }
;

FunctionDefinition:
    FunctionName L_PARENTHESIS FormalArgList R_PARENTHESIS ReturnType
    CompoundStatement
    END{
        std::vector<VariableInfo*> prototype;
        if ($3!=nullptr){
            for(uint i=0; i<$3->size(); i++){
                for(uint j=0; j<(*$3)[i]->counter; j++){
                    VariableInfo* dupTemp = new VariableInfo();
                    dupTemp->type_set = (*$3)[i]->type->type_set;
                    dupTemp->type = (*$3)[i]->type->type;
                    dupTemp->array_size = (*$3)[i]->type->array_size;
                    
                    prototype.push_back(dupTemp);
                }   
            }
        }
        $$ = new FunctionNode(
            @1.first_line,
            @1.first_column,
            std::string($1),
            $3,
            std::string($5->text),
            $6,
            prototype
        );
        
        // Memory_Free
        free($1);
    }
;

FunctionName:
    ID{
        $$ = strdup($1);
        
        // Memory_Free
        free($1);
    }
;

FormalArgList:
    Epsilon {
        $$ = nullptr;
    }
    |
    FormalArgs {
        
        $$ = $1;
    }
;

FormalArgs:
    FormalArg{
        $$ = new std::vector<NodeWithType*>();
        $$->push_back($1);
    }
    |
    FormalArgs SEMICOLON FormalArg {
        $1->push_back($3);
        $$ = $1;
    }
;

FormalArg:
    IdList COLON Type{
        $$ = new NodeWithType();
        $$->type = $3;
        $$->counter = 0;
    
    
        NodeList* var_list = new NodeList();
        
        for(uint i=0; i<$1->size(); i++){
             
            VariableInfo* dupTemp = new VariableInfo();
            
            dupTemp->type_set = $3->type_set;
            dupTemp->type = $3->type;
            
            dupTemp->array_size = $3->array_size;
            
            VariableNode* variable_node = new VariableNode(
                    (*$1)[i].line_number,
                    (*$1)[i].col_number,
                    (*$1)[i].name,
                    dupTemp,
                    nullptr
            );
            //printf("var = %s\n", (*$1)[i].name.c_str());
            var_list->push_back(variable_node);
            $$->counter++;
        }
        
        $$->node = new DeclNode(
                    @1.first_line,
                    @1.first_column,
                    var_list
                    );
        
        // Memory_Free
        delete $1;
    }
;

IdList:
    ID{
        $$ = new std::vector<id_info>();
        $$->push_back(id_info{std::string($1), @1.first_line, @1.first_column});

        // Memory_Free
        free($1);
    }
    |
    IdList COMMA ID{
        $1->push_back(id_info{std::string($3), @3.first_line, @3.first_column});
        $$ = $1;

        // Memory_Free
        free($3);
    }
;

ReturnType:
    COLON ScalarType{
        $$ = $2;
    }
    |
    Epsilon{
        $$ = new VariableInfo();
        $$-> text = "void";
    }
;

    /*
       Data Types and Declarations
                                   */

Declaration:
    VAR IdList COLON Type SEMICOLON{
        
        NodeList* var_list = new NodeList();
        //printf("declare\n");
        for(uint i=0; i<$2->size(); i++){
             
            VariableInfo* dupTemp = new VariableInfo();
            
            dupTemp->type_set = $4->type_set;
            dupTemp->type = $4->type;
            
            dupTemp->array_size = $4->array_size;
            
            VariableNode* variable_node = new VariableNode(
                    (*$2)[i].line_number,
                    (*$2)[i].col_number,
                    (*$2)[i].name,
                    dupTemp,
                    nullptr
            );
            var_list->push_back(variable_node);
        }
        
        $$ = new DeclNode(
                    @1.first_line,
                    @1.first_column,
                    var_list
                    );

        // Memory_Free
        delete $2;
    }
    |
    VAR IdList COLON LiteralConstant SEMICOLON{
       NodeList* var_list = new NodeList();
        for(uint i=0; i<$2->size(); i++){
                // duplicate $4 (eliminate hierarchy problem)
                VariableInfo* dupTemp2 = new VariableInfo();
                dupTemp2->type_set = SET_CONSTANT_LITERAL;
                dupTemp2->type = $4->getType();
                //printf("%s\n", $4->get_val().c_str());
                VariableNode* variable_node = new VariableNode(
                    (*$2)[i].line_number,
                    (*$2)[i].col_number,
                    (*$2)[i].name,
                    dupTemp2,
                    $4
                );

                var_list->push_back(variable_node);
                
        }

        $$ = new DeclNode(
            @1.first_line,
            @1.first_column,
            var_list
        );

        // Memory_Free
        delete $2;
    }
;

Type:
    ScalarType{
        $$ = $1;
    }
    |
    ArrType{
        $$ = $1;
    }
;

ScalarType:
    INTEGER{
        $$ = new VariableInfo();
        $$->type_set = SET_SCALAR;
        $$->type = TYPE_INTEGER;
        $$->text = "integer";
    }
    |
    REAL{
        $$ = new VariableInfo();
        $$->type_set = SET_SCALAR;
        $$->type = TYPE_REAL;
        $$->text = "real";
    }
    |
    STRING{
        $$ = new VariableInfo();
        $$->type_set = SET_SCALAR;
        $$->type = TYPE_STRING;
        $$->text = "string";
    }
    |
    BOOLEAN{
        $$ = new VariableInfo();
        $$->type_set = SET_SCALAR;
        $$->type = TYPE_BOOLEAN;
        $$->text = "boolean";
    }
;

ArrType:
    ArrDecl ScalarType{
        $$ = new VariableInfo();
        $$->type_set = SET_ACCUMLATED;
        $$->type = $2->type;
        $$->array_size = $1->array_size;

        // Memory_Free
        delete $1;
        delete $2;
    }
;

ArrDecl:
    ARRAY INT_LITERAL OF {
        $$ = new VariableInfo();
        $$->type_set = SET_ACCUMLATED;
        $$->type = UNKNOWN_TYPE;
        $$->array_size.push_back(atoi($2));
    }
    |
    ArrDecl ARRAY INT_LITERAL OF{
        $1->array_size.push_back(atoi($3));
        $$ = $1;
    }
;

LiteralConstant:
    NegOrNot INT_LITERAL{
        $$ = new ConstantValueNode(
            $1 == "" ? @2.first_line : @1.first_line,
            $1 == "" ? @2.first_column : @1.first_column,
            std::string($1)+std::string($2),
            TYPE_INTEGER
        );
    }
    |
    NegOrNot REAL_LITERAL{
        char buf[50];
        sprintf(buf, "%.6f", atof(std::string($2).c_str()));
        $$ = new ConstantValueNode(
            $1 == "" ? @2.first_line : @1.first_line,
            $1 == "" ? @2.first_column : @1.first_column,
            std::string($1)+std::string(buf),
            TYPE_REAL
        );
    }
    |
    StringAndBoolean{
        $$ = $1;
    }
;

NegOrNot:
    Epsilon{
        $$ = "";
    }
    |
    MINUS %prec UNARY_MINUS{
        $$ = "-";
    }
;

StringAndBoolean:
    STRING_LITERAL{
        $$ = new ConstantValueNode(
            @1.first_line,
            @1.first_column,
            std::string($1),
            TYPE_STRING
        );
    }
    |
    TRUE{
        $$ = new ConstantValueNode(
            @1.first_line,
            @1.first_column,
            std::string($1),
            TYPE_BOOLEAN
        );
    }
    |
    FALSE{
        $$ = new ConstantValueNode(
            @1.first_line,
            @1.first_column,
            std::string($1),
            TYPE_BOOLEAN
        );
    }
;

IntegerAndReal:
    INT_LITERAL{
        $$ = new ConstantValueNode(
            @1.first_line,
            @1.first_column,
            std::string($1),
            TYPE_INTEGER
        );
    }
    |
    REAL_LITERAL{
        char buf[50];
        sprintf(buf, "%.6f", atof(std::string($1).c_str()));
        $$ = new ConstantValueNode(
            @1.first_line,
            @1.first_column,
            std::string(buf),
            TYPE_REAL
        );
    }
;

    /*
       Statements
                  */

Statement:
    CompoundStatement{
        $$ = $1;
    }
    |
    Simple{
        $$ = $1;
    }
    |
    Condition{
        $$ = $1;
    }
    |
    While{
        $$ = $1;
    }
    |
    For{
        $$ = $1;
    }
    |
    Return{
        $$ = $1;
    }
    |
    FunctionCall{
        $$ = $1;
    }
;

CompoundStatement:
    BEGIN_
    DeclarationList
    StatementList
    END{
        $$ = new CompoundStatementNode(
            @1.first_line, 
            @1.first_column, 
            $2,
            $3
        );
    }
;

Simple:
    VariableReference ASSIGN Expression SEMICOLON{
        $$ = new AssignmentNode(
            @2.first_line,
            @2.first_column,
            $1,
            $3
        );
    }
    |
    PRINT Expression SEMICOLON{
        $$ = new PrintNode(
            @1.first_line,
            @1.first_column,
            $2
        );
    }
    |
    READ VariableReference SEMICOLON{
        $$ = new ReadNode(
            @1.first_line,
            @1.first_column,
            $2  
        );
    }
;

VariableReference:
    ID ArrRefList{
        $$ = new VariableReferenceNode(
            @1.first_line,
            @1.first_column,
            std::string($1),
            $2
        );

        // Memory_Free
        free($1);
    }
;

ArrRefList:
    Epsilon{
        $$ = nullptr;
    }
    |
    ArrRefs{
        $$ = $1;
    }
;

ArrRefs:
    L_BRACKET Expression R_BRACKET{
        $$ = new NodeList();
        $$->push_back($2);
    }
    |
    ArrRefs L_BRACKET Expression R_BRACKET{
        $1->push_back($3);
        $$ = $1;
    }
;

Condition:
    IF Expression THEN
    CompoundStatement
    ElseOrNot
    END IF{
        $$ = new IfNode(
            @1.first_line,
            @1.first_column,
            $2,
            $4,
            $5
        );
    }
;

ElseOrNot:
    ELSE
    CompoundStatement{
        $$ = $2;
    }
    |
    Epsilon{
        $$ = nullptr;
    }
;

While:
    WHILE Expression DO
    CompoundStatement
    END DO{
       $$ = new WhileNode(
            @1.first_line,
            @1.first_column,
            $2,
            $4
        );
    }
;

For:
    FOR ID ASSIGN INT_LITERAL TO INT_LITERAL DO
    CompoundStatement
    END DO{
        VariableInfo* var_info = new VariableInfo();
        var_info->type_set = SET_SCALAR;
        var_info->type = TYPE_INTEGER;
        VariableNode* variable_node = new VariableNode(
            @2.first_line,
            @2.first_column,
            std::string($2),
            var_info,
            nullptr
        );

        NodeList* var_list = new NodeList();
        var_list->push_back(variable_node);
        DeclNode* declaration_node = new DeclNode(
            @2.first_line,
            @2.first_column,
            var_list
        );

        // initial: an assignment node
        VariableReferenceNode* variable_reference_node = new VariableReferenceNode(
            @2.first_line,
            @2.first_column,
            std::string($2),
            nullptr
        );

        ConstantValueNode* constant_value_node = new ConstantValueNode(
            @4.first_line,
            @4.first_column,
            std::string($4),
            TYPE_INTEGER
        );

        AssignmentNode* assignment_node = new AssignmentNode(
            @3.first_line,
            @3.first_column,
            variable_reference_node,
            constant_value_node // expression node
        );

        
        ConstantValueNode* constant_value_node2 = new ConstantValueNode(
            @6.first_line,
            @6.first_column,
            std::string($6),
            TYPE_INTEGER
        );

        // For Node
        $$ = new ForNode(
            @1.first_line,
            @1.first_column,
            declaration_node,
            assignment_node,
            constant_value_node2, // expression node
            $8
        );

        // Memory_Free
        free($2);
    }
    
;

Return:
    RETURN Expression SEMICOLON{
        $$ = new ReturnNode(
            @1.first_line,
            @1.first_column,
            $2
        );
    }
;

FunctionCall:
    FunctionInvocation SEMICOLON{
        $$ = $1;
    }
;

FunctionInvocation:
    ID L_PARENTHESIS ExpressionList R_PARENTHESIS{
        $$ = new FunctionInvocationNode(
            @1.first_line,
            @1.first_column,
            std::string($1),
            $3
        );

        // Memory_Free
        free($1);
    }
;

ExpressionList:
    Epsilon{
        $$ = nullptr;
    }
    |
    Expressions{
        $$ = $1;
    }
;

Expressions:
    Expression{
        $$ = new NodeList();
        $$->push_back($1);
    }
    |
    Expressions COMMA Expression{
        $1->push_back($3);
        $$=$1;
    }
;

StatementList:
    Epsilon{
        $$ = nullptr;
    }
    |
    Statements{
        $$ = $1;
    }
;

Statements:
    Statement{
        $$ = new NodeList();
        $$->push_back($1);
    }
    |
    Statements Statement{
        $1->push_back($2);
        $$=$1;
    }
;

Expression:
    L_PARENTHESIS Expression R_PARENTHESIS{
        $$ = $2;
    }
    |
    MINUS Expression %prec UNARY_MINUS{
        // Unary Operator Node
        $$ = new UnaryOperatorNode(
            @1.first_line,
            @1.first_column,
            $1,
            $2
        );
    }
    |
    Expression MULTIPLY Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression DIVIDE Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression MOD Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression PLUS Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression MINUS Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression LESS Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression LESS_OR_EQUAL Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression GREATER Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression GREATER_OR_EQUAL Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression EQUAL Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression NOT_EQUAL Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    NOT Expression{
        // Unary Operator Node
        $$ = new UnaryOperatorNode(
            @1.first_line,
            @1.first_column,
            $1,
            $2
        );
    }
    |
    Expression AND Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    Expression OR Expression{
        // Binary Operator Node
        $$ = new BinaryOperatorNode(
            @2.first_line,
            @2.first_column,
            $2,
            $1,
            $3
        );
    }
    |
    IntegerAndReal{
        $$ = $1;
    }
    |
    StringAndBoolean{
        $$ = $1;
    }
    |
    VariableReference{
        // Variable Reference Node
        $$ = $1;
    }
    |
    FunctionInvocation{
        // Function Call Node
        $$ = $1;
    }
;

    /*
       misc
            */
Epsilon:
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
    if (argc < 2) {
        fprintf(stderr, "Usage: ./parser <filename> [--dump-ast]\n");
        exit(-1);
    }

    
    yyin = fopen(argv[1], "r");
    assert(yyin != NULL && "fopen() fails.");

    yyparse();
    
    //v = new AstNodeVisitor();
    
    if (argc >= 3 && strcmp(argv[2], "--dump-ast") == 0) {
        //printf("START\n");
        
        root->accept(v);
        
    }

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");

    delete root;
    fclose(yyin);
    yylex_destroy();
    return 0;
}
