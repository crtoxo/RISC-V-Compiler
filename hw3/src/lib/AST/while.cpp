#include "AST/while.hpp"

// TODO
WhileNode::WhileNode(const uint32_t line, const uint32_t col, Node cond, // an expression node
              Node body )
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->cond = cond;
        this->body = body;
    }

// TODO: You may use code snippets in AstDumper.cpp
void WhileNode::print() {
    std::cout << "while statement <line: " << line
        << ", col: " << col
        << ">" << std::endl;
}

void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(cond) cond->accept(p_visitor);
    if(body) body->accept(p_visitor);
}
