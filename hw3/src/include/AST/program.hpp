#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include "AST/ast.hpp"

//#include "visitor/AstNodeVisitor.hpp"

class ProgramNode : public AstNode {
  public:
    ProgramNode(const uint32_t line, 
                const uint32_t col,
                std::string p_name,
                std::string ret_type,
                NodeList *dec,
                NodeList *func,
                Node comp_stat );
    
    ~ProgramNode() = default;

    const char *getNameCString() const; 
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    void visitChildNodes(AstNodeVisitor &p_visitor);

    void print() override;

  private:
    int line, col;
    std::string name; 
    std::string ret_type;
    NodeList *dec;
    NodeList *func;
    Node comp_stat;
};

#endif
