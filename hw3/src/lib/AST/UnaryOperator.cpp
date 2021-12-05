#include "AST/UnaryOperator.hpp"

// TODO
UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col,
                      enumOperator op,
                      Node operand )
    : ExpressionNode{line, col} {
        this->line = line;
        this->col = col;
        this->op = op;
        this->operand = operand;
    }

// TODO: You may use code snippets in AstDumper.cpp
void UnaryOperatorNode::print() {
    std::cout << "unary operator <line: " << line
        << ", col: " << col
        << "> ";
    
    // cout << operator
    switch(op){
        case OP_NOT:              std::cout<<"not"; break;
        case OP_MINUS:            std::cout<<"neg"; break;
        default:                  std::cout<<"unknown"; break;
    }
    // cout << operator END

    std::cout << std::endl;
}

char* UnaryOperatorNode::getOp() {
    switch(op){
        case OP_NOT:               return "not";
        case OP_MINUS:              return "neg";
        default:                  return "unknown";
    }
    // cout << operator END

    std::cout << std::endl;
}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(operand) operand->accept(p_visitor);
}
