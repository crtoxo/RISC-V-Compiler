#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"

#include <memory>
#include <utility>

class PrintNode : public AstNode {
  public:
    PrintNode(const uint32_t line, const uint32_t col, ExpressionNode *p_expr);
    ~PrintNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;
    
    std::pair<int, int> getLocationCString() const;

  //private:
    std::unique_ptr<ExpressionNode> target;
};

#endif
