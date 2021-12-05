#include "AST/read.hpp"

// TODO
ReadNode::ReadNode(const uint32_t line, const uint32_t col, Node var_ref_node )
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->var_ref_node = var_ref_node ;
    }

// TODO: You may use code snippets in AstDumper.cpp
void ReadNode::print() {
    std::cout << "read statement <line: " << line
        << ", col: " << col
        << "> " << std::endl;
}

void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(var_ref_node) var_ref_node->accept(p_visitor);
}
