#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include "AST/ast.hpp"

class PrintNode : public AstNode {
  public:
    PrintNode(const uint32_t line, 
              const uint32_t col,
              Node expression_node );
    ~PrintNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void print() override;

  private:
      int line, col;
    Node expression_node;
};

#endif
