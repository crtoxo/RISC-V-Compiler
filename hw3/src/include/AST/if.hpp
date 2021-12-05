#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/ast.hpp"

class IfNode : public AstNode {
  public:
    IfNode(const uint32_t line, 
           const uint32_t col,
           Node cond,
           Node body,
           Node body_of_else );
    ~IfNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void print() override;

  private:
      int line, col;
    Node cond;
    Node body;
    Node body_of_else;
};

#endif
