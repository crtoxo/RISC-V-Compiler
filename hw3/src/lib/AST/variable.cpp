#include "AST/variable.hpp"

// TODO
VariableNode::VariableNode(const uint32_t line, const uint32_t col, std::string name,
                 VariableInfo *type,
                 Node const_val_node)
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->name = name;
        this->type = type;
        this->const_val_node = const_val_node;
    }

// TODO: You may use code snippets in AstDumper.cpp
void VariableNode::print() {
    std::cout << "variable <line: " << line
        << ", col: " << col
        << "> " << name
        << " " << this->getType()
        << std::endl;
}

std::string VariableNode::getType() {
    switch(this->type->type_set){
        case SET_SCALAR:
        case SET_CONSTANT_LITERAL:
            switch(this->type->type){
                case TYPE_INTEGER: this->variable_type = "integer"; break;
                case TYPE_REAL:    this->variable_type = "real"; break;
                case TYPE_STRING:  this->variable_type = "string"; break;
                case TYPE_BOOLEAN: this->variable_type = "boolean"; break;
                default:           this->variable_type = "unknown"; break;
            }
            break;
        case SET_ACCUMLATED:
            switch(this->type->type){
                case TYPE_INTEGER: this->variable_type = "integer "; break;
                case TYPE_REAL:    this->variable_type = "real "; break;
                case TYPE_STRING:  this->variable_type = "string "; break;
                case TYPE_BOOLEAN: this->variable_type = "boolean "; break;
                default:           this->variable_type = "unknown "; break;
            }

            for(uint i=0; i<this->type->array_size.size(); i++){
                this->variable_type += "[";
                this->variable_type += std::to_string(this->type->array_size[i]);
                this->variable_type += "]";
            }
            break;
        default:
            this->variable_type = "unknown";
            break;
    }
    return this->variable_type;
}

void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    
    if(const_val_node) {
        const_val_node->accept(p_visitor);
        
    }
}
