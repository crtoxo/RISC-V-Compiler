#include "AST/program.hpp"


// TODO
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                         std::string p_name,
                         std::string ret_type,
                         NodeList *dec,
                         NodeList *func,
                         Node comp_stat)
                         : AstNode{line, col}, name(p_name) {
                             
                             this->line = line;
                             this->col = col;
                             this->name = p_name;
                             this->ret_type = ret_type;
                             this->dec = dec;
                             this->func = func;
                             this->comp_stat = comp_stat;
                        }

const char *ProgramNode::getNameCString() const { return name.c_str(); }

void ProgramNode::print() {
    // TODO
    //outputIndentationSpace();

    std::printf("program <line: %u, col: %u> %s %s\n",
                line, col,
                name.c_str(), "void");

    // TODO
    //incrementIndentation();
    //visitChildNodes();
    //decrementIndentation();
}




void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) { // visitor pattern version
    if(dec){
        for (auto &decl : *dec) {
            
            //if (decl == nullptr) break;
            
            decl->accept(p_visitor);
        }
    }
    
    if(func){
        for (auto &f : *func) {
            if (f == nullptr) break;
            //printf("fname = %s\n", name.c_str());
            f->accept(p_visitor);
        }
    }
    
    if(comp_stat) comp_stat->accept(p_visitor);
}
