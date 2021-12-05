#include "AST/CompoundStatement.hpp"

// TODO
CompoundStatementNode::CompoundStatementNode(const uint32_t line,
                                             const uint32_t col,
                                             NodeList* dec_node_list,
                                             NodeList* stat_node_list )
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->dec_node_list = dec_node_list;
        this->stat_node_list = stat_node_list;
    }

// TODO: You may use code snippets in AstDumper.cpp
void CompoundStatementNode::print() {
    std::cout << "compound statement <line: " << line
        << ", col: " << col
        << "> " << std::endl;
}

void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    // TODO
    if(dec_node_list){
        for (auto &decl : *dec_node_list) {
            //if (decl == nullptr) break;
            decl->accept(p_visitor);
        }
    }
    if(stat_node_list){
        for (auto &stat : *stat_node_list) {
            //if (decl == nullptr) break;
            stat->accept(p_visitor);
        }
    }
    
}
