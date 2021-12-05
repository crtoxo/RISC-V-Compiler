#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include "AST/ast.hpp"
#include "visitor/AstNodeVisitor.hpp"

class CompoundStatementNode : public AstNode {
  public:
    CompoundStatementNode(const uint32_t line, 
                          const uint32_t col,
                          NodeList* dec_node_list,
                          NodeList* stat_node_list );
    ~CompoundStatementNode() = default;
    
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void print() override;

  private:
    int line, col;
     NodeList* dec_node_list; // Local variable and constant declarations (zero or more)
     NodeList* stat_node_list; // Statements (zero or more)
};

#endif
