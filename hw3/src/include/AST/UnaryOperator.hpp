#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.hpp"

class UnaryOperatorNode : public ExpressionNode {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col,
                      enumOperator op,
                      Node operand );
    ~UnaryOperatorNode() = default;
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void print() override;
    char* getOp();

  private:
    int line, col;
    enumOperator op;
    Node operand;
};

#endif
