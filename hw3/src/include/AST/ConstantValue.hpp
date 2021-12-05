#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "AST/expression.hpp"

class ConstantValueNode : public ExpressionNode {
  public:
    ConstantValueNode(const uint32_t line, 
                      const uint32_t col,
                      std::string const_val,
                      enumType type
                     );
    ~ConstantValueNode() = default;
    const char* get_val() const { return const_val.c_str(); }
    void accept(AstNodeVisitor &v) { v.visit(*this); }
    void print() override;
    std::string get_val(){return const_val; }
    enumType getType(){return type;}
  private:
    enumType type;
    int line, col;
    std::string const_val;
};

#endif
