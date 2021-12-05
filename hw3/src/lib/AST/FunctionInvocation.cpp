#include "AST/FunctionInvocation.hpp"

// TODO
FunctionInvocationNode::FunctionInvocationNode(const uint32_t line,
                                               const uint32_t col, std::string function_name,
                           NodeList* arguments )
    : ExpressionNode{line, col} {
        this->line = line;
        this->col = col;
        this->function_name = function_name;
        this->arguments = arguments;
    }

// TODO: You may use code snippets in AstDumper.cpp
void FunctionInvocationNode::print() {
    std::cout << "function invocation <line: " << line
        << ", col: " << col
        << "> " << function_name 
        << std::endl;
}

void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(arguments){
        for (auto &arg : *arguments) {
            
            //if (decl == nullptr) break;
            
            arg->accept(p_visitor);
        }
    }
}
