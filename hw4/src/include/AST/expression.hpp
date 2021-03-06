#ifndef __AST_EXPRESSION_NODE_H
#define __AST_EXPRESSION_NODE_H

#include "AST/PType.hpp"
#include "AST/ast.hpp"

class ExpressionNode : public AstNode {
  public:
    ExpressionNode(const uint32_t line, const uint32_t col);
    ~ExpressionNode() = default;
    void setType(std::string p_type);
    int getPos(){return this->location.col;}
  //protected:
    // for carrying type of result of an expression
    std::string type;
    int valid;
};

#endif
