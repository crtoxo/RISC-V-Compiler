#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include <string>
#include "AST/expression.hpp"

class VariableReferenceNode : public ExpressionNode {
  public:
    // normal reference
    VariableReferenceNode(const uint32_t line, const uint32_t col,
                          std::string var_name);
    // array reference
    VariableReferenceNode(const uint32_t line, 
                          const uint32_t col,
                          std::string var_name,
                          NodeList* expression_node_list );
    ~VariableReferenceNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void print() override;
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    std::string getName(){return var_name; }

  private:
    int line, col;
    std::string var_name;
    NodeList* expr_node_list; // indices
};

#endif
