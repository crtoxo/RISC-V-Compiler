#include "AST/return.hpp"

// TODO
ReturnNode::ReturnNode(const uint32_t line, const uint32_t col,
               Node ret_val )
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->ret_val = ret_val;
    }

// TODO: You may use code snippets in AstDumper.cpp
void ReturnNode::print() {
    std::cout << "return statement <line: " << line
        << ", col: " << col
        << ">" << std::endl;
}

void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(ret_val) ret_val->accept(p_visitor);
}
