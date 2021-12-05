#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/ast.hpp"

class ReadNode : public AstNode {
  public:
    ReadNode(const uint32_t line, const uint32_t col,
             Node var_ref_node );
    ~ReadNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void print() override;

  private:
      int line, col;
    Node var_ref_node;
};

#endif
