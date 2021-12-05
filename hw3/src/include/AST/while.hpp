#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/ast.hpp"

class WhileNode : public AstNode {
  public:
    WhileNode(const uint32_t line, const uint32_t col,
              Node cond, // an expression node
              Node body );
    ~WhileNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void print() override;
    void accept(AstNodeVisitor &v) { v.visit(*this); }

  private:
    int line, col;
    Node cond; // an expression node
    Node body;
};

#endif
