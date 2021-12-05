#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include "AST/ast.hpp"

class ReturnNode : public AstNode {
  public:
    ReturnNode(const uint32_t line, 
               const uint32_t col,
               Node ret_val );
    ~ReturnNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void print() override;

  private:
      int line, col;
    Node ret_val;
};

#endif
