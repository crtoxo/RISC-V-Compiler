#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/ast.hpp"

class ForNode : public AstNode {
  public:
    ForNode(const uint32_t line, 
            const uint32_t col,
            Node loop_var_dec,
            Node init_stat,
            Node cond,
            Node body );
    ~ForNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void print() override;

  private:
    int line, col;
    Node loop_var_dec; // a declaration node
    Node init_stat; // an assignment node
    Node cond; // an expression node
    Node body;
};

#endif
