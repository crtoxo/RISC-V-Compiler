#include "AST/VariableReference.hpp"
#include "visitor/AstNodeVisitor.hpp"

VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *p_name)
    : ExpressionNode{line, col}, name(p_name) {
        redecl = 0;
    }

VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *p_name,
                                             Exprs *p_indices)
    : ExpressionNode{line, col}, name(p_name), indices(std::move(*p_indices)) {
        redecl = 0;
    }

const char *VariableReferenceNode::getNameCString() const {
    return name.c_str();
}

const char *VariableReferenceNode::getTypeCString() const {
    std::string tmp = "";
    for(uint i = 0; i < indices.size(); i++){
        if(indices[i]) {
            tmp += std::string(indices[i]->type);
            std::cout << std::string(indices[i]->type) << "ooooooooooo\n";
        }
        
    }
    return tmp.c_str();
}

std::pair<int, int> VariableReferenceNode::getLocationCString() const {
    return std::make_pair(this->location.line, this->location.col);
}

void VariableReferenceNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &index : indices) {
        index->accept(p_visitor);
    }
}
