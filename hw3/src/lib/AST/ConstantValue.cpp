#include "AST/ConstantValue.hpp"

// TODO
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, std::string const_val, enumType type)
    : ExpressionNode{line, col} {
        this->line = line;
        this->col = col;
        this->const_val = const_val;
        this->type = type;
}
    
    


void ConstantValueNode::print() {
    /*std::cout << "constant <line: " << line
        << ", col: " << col
        << "> ";

    // cout << literal_value
    switch(this->const_val->type){
        case TYPE_INTEGER: std::cout << this->const_val->int_literal; break;
        case TYPE_REAL:    std::cout << fixed << setprecision(6) << this->const_val->real_literal; break;
        case TYPE_STRING:  std::cout << this->const_val->string_literal; break;
        case TYPE_BOOLEAN:
            switch(this->const_val->boolean_literal){
                case Boolean_TRUE:  std::cout << "true"; break;
                case Boolean_FALSE: std::cout << "false"; break;
                default: std::cout << "unknown"; break;
            } 
            break;
        default: std::cout << "unknown"; break;
    }
    // cout << literal_value

    std::cout << std::endl;*/
}

// TODO: You may use code snippets in AstDumper.cpp
