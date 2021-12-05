#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include <string>
#include "AST/ast.hpp"


class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, 
                 const uint32_t col,
                 std::string name,
                 VariableInfo *type,
                 Node const_val_node);
    ~VariableNode() = default;

    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void print() override;
    std::string getType();
    std::string getName(){ return name; }

  private:
    int line, col;
    std::string name;
    VariableInfo *type;
    Node const_val_node;
    std::string variable_type;
};

#endif
