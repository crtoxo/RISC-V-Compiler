#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include <vector>
#include "AST/ast.hpp"
#include "AST/variable.hpp"

class FunctionNode : public AstNode {
  public:
    FunctionNode(const uint32_t line, 
                 const uint32_t col,
                 std::string name,
                 std::vector<NodeWithType*> *dec_list,
                 std::string ret_type,
                 Node body,
                 std::vector<VariableInfo*> prototype);
    ~FunctionNode() = default;
    void visitChildNodes(AstNodeVisitor &p_visitor);
    void accept(AstNodeVisitor &v) {  v.visit(*this);}
    std::string getName(){ return name; }
    std::string getRetType(){ return ret_type; }
    std::string getPrototype(){ 
        std::string tmp;
        int cnt = 0;
        tmp += "(";
        for(int i=0; i<this->type_list.size(); i++){
            if(i != 0) tmp += ", ";
            switch(this->type_list[i]->type_set){
                case SET_SCALAR:
                case SET_CONSTANT_LITERAL:
                    switch(this->type_list[i]->type){
                        case TYPE_INTEGER: tmp += "integer"; break;
                        case TYPE_REAL:    tmp += "real"; break;
                        case TYPE_STRING:  tmp += "string"; break;
                        case TYPE_BOOLEAN: tmp += "boolean"; break;
                        default:           tmp += "unknown"; break;
                    }
                    break;
                case SET_ACCUMLATED:
                    switch(this->type_list[i]->type){
                        case TYPE_INTEGER: tmp += "integer "; break;
                        case TYPE_REAL:    tmp += "real "; break;
                        case TYPE_STRING:  tmp += "string "; break;
                        case TYPE_BOOLEAN: tmp += "boolean "; break;
                        default:           tmp += "unknown "; break;
                    }

                    for(int j=0; j < this->type_list[i]->array_size.size(); j++){
                        tmp += "[";
                        tmp += std::to_string(this->type_list[i]->array_size[j]);
                        tmp += "]";
                    }
                    break;
                default: std::cout<<"unknown"; break;
            }
        }
        tmp += ")";
        return tmp;
    }
    void print() override;

  private:
    // TODO: name, declarations, return type, compound statement
    int line, col;
    std::string name;
    std::vector<NodeWithType*> *dec_list;
    std::string ret_type;
    Node body;
    std::vector<VariableInfo*> type_list;
};

#endif
