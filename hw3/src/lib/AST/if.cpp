#include "AST/if.hpp"

// TODO
IfNode::IfNode(const uint32_t line, const uint32_t col, Node cond,
           Node body,
           Node body_of_else )
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->cond = cond;
        this->body = body;
        this->body_of_else = body_of_else;
    }

// TODO: You may use code snippets in AstDumper.cpp
void IfNode::print() {
    std::cout << "if statement <line: " << line
        << ", col: " << col
        << ">" << std::endl;
}

void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(cond) cond->accept(p_visitor);
    if(body) body->accept(p_visitor);
    if(body_of_else) body_of_else->accept(p_visitor);
}
