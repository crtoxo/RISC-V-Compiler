#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/ast.hpp"

class AssignmentNode : public AstNode {
  public:
    AssignmentNode(const uint32_t line, 
                   const uint32_t col,
                   Node var_ref_node, // Lvalue
                   Node expr_node );
    ~AssignmentNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void print() override;

  private:
    int line, col;
    Node var_ref_node; // Lvalue
    Node expr_node; // Expression
};

#endif
