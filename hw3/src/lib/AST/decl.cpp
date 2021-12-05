#include "AST/decl.hpp"

// TODO
DeclNode::DeclNode(const uint32_t line, const uint32_t col, NodeList *var_list)
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->var_list = var_list;
    }

// TODO
//DeclNode::DeclNode(const uint32_t line, const uint32_t col)
//    : AstNode{line, col} {}

// TODO: You may use code snippets in AstDumper.cpp
void DeclNode::print() {
    std::cout << "function invocation <line: " << line
        << ", col: " << col
        << "> " 
        << std::endl;
}
        
void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(var_list){
        for (auto &var : *var_list) {
            //if (var == nullptr) break;
            var->accept(p_visitor);
        }
    }
    
}
