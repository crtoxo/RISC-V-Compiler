#include "AST/BinaryOperator.hpp"

// TODO
BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col, enumOperator op,
                       Node left_operand,
                       Node right_operand)
    : ExpressionNode{line, col} {
        this->line = line;
        this->col = col;
        this->op = op;
        this->left_operand = left_operand;
        this->right_operand = right_operand;
    }

// TODO: You may use code snippets in AstDumper.cpp
void BinaryOperatorNode::print() {
    std::cout << "binary operator <line: " << line
        << ", col: " << col
        << "> ";
    
    // cout << operator
    switch(op){
        case OP_OR:               std::cout<<"or"; break;
        case OP_AND:              std::cout<<"and"; break;
        // case OP_NOT:              std::cout<<"not"; break;
        case OP_LESS:             std::cout<<"<"; break;
        case OP_LESS_OR_EQUAL:    std::cout<<"<="; break;
        case OP_EQUAL:            std::cout<<"="; break;
        case OP_GREATER:          std::cout<<">"; break;
        case OP_GREATER_OR_EQUAL: std::cout<<">="; break;
        case OP_NOT_EQUAL:        std::cout<<"<>"; break;
        case OP_PLUS:             std::cout<<"+"; break;
        case OP_MINUS:            std::cout<<"-"; break;
        case OP_MULTIPLY:         std::cout<<"*"; break;
        case OP_DIVIDE:           std::cout<<"/"; break;
        case OP_MOD:              std::cout<<"mod"; break;
        default:                  std::cout<<"unknown"; break;
    }
    // cout << operator END

    std::cout << std::endl;
}

char* BinaryOperatorNode::getOp() {
    switch(op){
        case OP_OR:               return "or";
        case OP_AND:              return "and";
        // case OP_NOT:              std::cout<<"not"; break;
        case OP_LESS:             return "<";
        case OP_LESS_OR_EQUAL:    return "<=";
        case OP_EQUAL:            return "=";
        case OP_GREATER:          return ">";
        case OP_GREATER_OR_EQUAL: return ">="; 
        case OP_NOT_EQUAL:        return "<>";
        case OP_PLUS:             return "+";
        case OP_MINUS:            return "-"; 
        case OP_MULTIPLY:         return "*";
        case OP_DIVIDE:           return "/";
        case OP_MOD:              return "mod";
        default:                  return "unknown";
    }
    // cout << operator END

    std::cout << std::endl;
}



void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    
    if(left_operand) left_operand->accept(p_visitor);
        
    if(right_operand) right_operand->accept(p_visitor);
      
}
