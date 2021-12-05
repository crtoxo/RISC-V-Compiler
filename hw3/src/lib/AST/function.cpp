#include "AST/function.hpp"

// TODO
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col, std::string name, 
    std::vector<NodeWithType*> *dec_list, 
    std::string ret_type, 
    Node body,
    std::vector<VariableInfo*> prototype )
    : AstNode{line, col} {
        this->line = line;
        this->col = col;
        this->name = name;
        this->dec_list = dec_list;
        this->ret_type = ret_type;
        this->body = body;
        this->type_list = prototype;
    }

// TODO: You may use code snippets in AstDumper.cpp
void FunctionNode::print() {
    std::cout << "function declaration <line: " << line
        << ", col: " << col
        << "> " << name
        << " " ;
    
       /* 
        
    // cout << return_type (scalar_type or void)
    switch(this->ret_type->type_set){
        case SET_SCALAR:
            switch(this->ret_type->type){
                case TYPE_INTEGER: std::cout<<"integer"; break;
                case TYPE_REAL:    std::cout<<"real"; break;
                case TYPE_STRING:  std::cout<<"string"; break;
                case TYPE_BOOLEAN: std::cout<<"boolean"; break;
                default:           std::cout<<"unknown"; break;
            }
            break;
        case UNKNOWN_SET:
            switch(this->ret_type->type){
                case TYPE_VOID: std::cout<<"void"; break;
                default:        std::cout<<"unknown"; break;
            }
            break;
        default: std::cout<<"unknown"; break; break;
    }
    // cout << return_type END

    // cout << parameter_prototype
    std::cout<<" (";
        for(uint i=0; i<this->prototype.size(); i++){
            if(i != 0) std::cout<<", ";
            switch(this->prototype[i]->type_set){
                case SET_SCALAR:
                case SET_CONSTANT_LITERAL:
                    switch(this->prototype[i]->type){
                        case TYPE_INTEGER: std::cout<<"integer"; break;
                        case TYPE_REAL:    std::cout<<"real"; break;
                        case TYPE_STRING:  std::cout<<"string"; break;
                        case TYPE_BOOLEAN: std::cout<<"boolean"; break;
                        default:           std::cout<<"unknown"; break;
                    }
                    break;
                case SET_ACCUMLATED:
                    switch(this->prototype[i]->type){
                        case TYPE_INTEGER: std::cout<<"integer"; break;
                        case TYPE_REAL:    std::cout<<"real"; break;
                        case TYPE_STRING:  std::cout<<"string"; break;
                        case TYPE_BOOLEAN: std::cout<<"boolean"; break;
                        default:           std::cout<<"unknown"; break;
                    }

                    for(uint j=0; j<this->prototype[i]->array_range.size(); j++){
                        std::cout<<"[";
                        std::cout<<to_string(this->prototype[i]->array_range[j].end-this->prototype[i]->array_range[j].start);
                        std::cout<<"]";
                    }
                    break;
                default: std::cout<<"unknown"; break;
            }
        }
    std::cout<<")";
    // cout << parameter_prototype END
    std::cout<<std::endl;
    
    */
       
       
       
}

void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if(dec_list){
        for (auto &decl : *dec_list) {
            if (decl == nullptr) break;
            decl->node->accept(p_visitor);
        }
    }
    
    if (body) body->accept(p_visitor);
}
