#include "AST/for.hpp"

// TODO
ForNode::ForNode(const uint32_t line, const uint32_t col, Node loop_var_dec,
            Node init_stat,
            Node cond,
            Node body )
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->loop_var_dec = loop_var_dec;
        this->init_stat = init_stat;
        this->cond = cond;
        this->body = body;
    }

// TODO: You may use code snippets in AstDumper.cpp
void ForNode::print() {
    std::cout << "for statement <line: " << line
        << ", col: " << col
        << ">" << std::endl;
}

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(loop_var_dec) loop_var_dec->accept(p_visitor);
    if(init_stat) init_stat->accept(p_visitor);
    if(cond) cond->accept(p_visitor);
    if(body) body->accept(p_visitor);
}
