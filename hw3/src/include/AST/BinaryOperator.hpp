#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"

#include <memory>

class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(const uint32_t line, 
                       const uint32_t col,
                       enumOperator op,
                       Node left_operand,
                       Node right_operand );
    ~BinaryOperatorNode() = default;
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void print() override;
    char* getOp();

  private:
    // TODO: operator, expressions
    int line, col;
    enumOperator op;
    Node left_operand;
    Node right_operand;
};

#endif
