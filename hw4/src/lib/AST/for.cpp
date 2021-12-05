#include "AST/for.hpp"
#include "visitor/AstNodeVisitor.hpp"

ForNode::ForNode(const uint32_t line, const uint32_t col, DeclNode *p_var_decl,
                 AssignmentNode *p_initial_statement,
                 ConstantValueNode *p_condition, CompoundStatementNode *p_body)
    : AstNode{line, col}, var_decl(p_var_decl),
      initial_statement(p_initial_statement), condition(p_condition),
      body(p_body) {}

void ForNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(var_decl) var_decl->accept(p_visitor);
    if(initial_statement) initial_statement->accept(p_visitor);
    if(condition) condition->accept(p_visitor);
    if(body) body->accept(p_visitor);
}

std::pair<int, int> ForNode::getLocationCString() const {
    return std::make_pair(this->location.line, this->location.col);
}
