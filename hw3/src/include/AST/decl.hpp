#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.hpp"
//#include "visitor/AstNodeVisitor.hpp"

class DeclNode : public AstNode {
  public:
    // variable declaration
    DeclNode(const uint32_t line, 
             const uint32_t col,
             NodeList *var_list);

    // constant variable declaration
    /*DeclNode(const uint32_t, 
             const uint32_t col,
             NodeList *var_list);*/

    ~DeclNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void print() override;

  private:
    int line, col;
    NodeList *var_list;
    NodeList *const_list; // ??
};

#endif
