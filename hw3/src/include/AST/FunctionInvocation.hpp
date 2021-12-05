#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include <string>
#include "AST/expression.hpp"

class FunctionInvocationNode : public ExpressionNode {
  public:
    FunctionInvocationNode(const uint32_t line, 
                           const uint32_t col,
                           std::string function_name,
                           NodeList* arguments );
    ~FunctionInvocationNode() = default;
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void visitChildNodes(AstNodeVisitor &p_visitor);
    const char *getNameCString() const { return function_name.c_str(); }

    void print() override;

  private:
    int line, col;
    std::string function_name;
    NodeList* arguments;
};

#endif
