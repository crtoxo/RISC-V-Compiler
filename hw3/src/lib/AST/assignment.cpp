#include "AST/assignment.hpp"

// TODO
AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col, Node var_ref_node,
                   Node expr_node)
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->var_ref_node = var_ref_node;
        this->expr_node = expr_node;
    }

// TODO: You may use code snippets in AstDumper.cpp
void AssignmentNode::print() {
    std::cout << "assignment statement <line: " << line
        << ", col: " << col
        << "> " << std::endl;
}

void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(var_ref_node) var_ref_node->accept(p_visitor);
    if(expr_node) expr_node->accept(p_visitor);
    
}
