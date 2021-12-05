#include "AST/VariableReference.hpp"

// TODO
VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             std::string var_name
                                            )
    : ExpressionNode{line, col} {
        this->line = line;
        this->col = col;
        this->var_name = var_name;
    }
    
VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             std::string var_name,
                                             NodeList* expr_node_list
                                            )
    : ExpressionNode{line, col} {
        this->line = line;
        this->col = col;
        this->var_name = var_name;
        this->expr_node_list = expr_node_list;
    }

// TODO
// VariableReferenceNode::VariableReferenceNode(const uint32_t line,
//                                              const uint32_t col)
//     : ExpressionNode{line, col} {}

// TODO: You may use code snippets in AstDumper.cpp
void VariableReferenceNode::print() {
    std::cout << "variable reference <line: " << line
        << ", col: " << col
        << "> " << var_name 
        << std::endl;
}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    
    if(expr_node_list){
        for (auto &expr : *expr_node_list) {
            if (expr == nullptr) break;
            
            expr->accept(p_visitor);
        }
    }
}
