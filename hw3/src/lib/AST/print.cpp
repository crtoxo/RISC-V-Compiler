#include "AST/print.hpp"

// TODO
PrintNode::PrintNode(const uint32_t line, const uint32_t col, Node expression_node)
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->expression_node = expression_node;
    }

// TODO: You may use code snippets in AstDumper.cpp
void PrintNode::print() {
    std::cout << "print statement <line: " << line
        << ", col: " << col
        << "> " << std::endl;
}

void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(expression_node) expression_node->accept(p_visitor);
}
