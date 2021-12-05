#include "sema/SemanticAnalyzer.hpp"
#include "sema/SymbolTable.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <string.h>


SymbolManager *symbol_manager;
std::vector<std::string> err_msg;
uint err_loc[512 + 1];

uint level_cnt = 0;
uint current_idx = 0;
uint is_func = 0;
uint is_for = 0;
uint is_param = 0;
uint from_var = 0;
std::string cur_func_type;

extern char *line_code[512 + 1];
extern uint err_flag;
extern int32_t opt_dmp;

void pointErrLoc(int idx, std::string &s){
    idx += 4;
    while(--idx) s += " ";
    s += "^\n";
}

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    memset(err_loc, 0, sizeof(uint) * 513);
    SymbolEntry entry = SymbolEntry(std::string(p_program.getNameCString()), KIND_PROG, 0, std::string(p_program.getTypeCString()), std::string(""));
    SymbolTable* symbol_table = new SymbolTable();
    symbol_manager = new SymbolManager();
    
    symbol_table->push_back(entry);
    symbol_manager->push_back(symbol_table);
    
    
    p_program.visitChildNodes(*this);
    
    /*for(uint i = 0; i < p_program.var_decls->size(); ++i) {
        (*(p_program.var_decls))[i]->accept(*this);
    }
    
    for(uint i = 0; i < p_program.funcs->size(); ++i) {
        (*(p_program.funcs))[i]->accept(*this);
    }
    if(p_program.)*/
    
    /*TODO 4.  */
    
    dumpSymbol(symbol_table);
    symbol_manager->pop_back();
    
    for(uint i = 0; i < err_msg.size(); i++){
        std::cout << err_msg[i];
    }
    //std::cout << err_msg[0];
}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    
    //std::cout << p_variable.getNameCString() << "xxxxxxxxxxxx\n";
    KType cur_kind;
    from_var = 1;
    if(is_for){
        cur_kind = KIND_LP_VAR;
        //SymbolEntry entry = SymbolEntry(std::string(p_variable.getNameCString()), KIND_LP_VAR, level_cnt, std::string(p_variable.getTypeCString()), std::string(""));
        //symbol_manager->back()->push_back(entry);
        
    }
    else if(is_param){
        cur_kind = KIND_PARAM;
        //SymbolEntry entry = SymbolEntry(std::string(p_variable.getNameCString()), KIND_PARAM, level_cnt, std::string(p_variable.getTypeCString()), std::string(""));
        //symbol_manager->back()->push_back(entry);
    }
    else{
        cur_kind = KIND_VAR;
        //SymbolEntry entry = SymbolEntry(std::string(p_variable.getNameCString()), KIND_VAR, level_cnt, std::string(p_variable.getTypeCString()), std::string(""));
        //symbol_manager->back()->push_back(entry);
    }
    //std::cout << p_variable.getNameCString() << "\t" << cur_kind << "aaaaaaaaaaaaaaaaaaa\n";
    
    SymbolEntry entry = SymbolEntry(std::string(p_variable.getNameCString()), cur_kind, level_cnt, std::string(p_variable.getTypeCString()), std::string(""));
    symbol_manager->back()->push_back(entry);
    
    p_variable.visitChildNodes(*this);
    
    std::string getDim = std::string(p_variable.getTypeCString());
    std::string tmpVal = "";
    int flg = 0;
    for(uint i = 0; i < getDim.length(); i++){
        if(getDim[i] == '['){
            //tmpVal = getDim.substr(i+1, 1);
            flg = 1;
            continue;
        }
        else if(getDim[i] == ']'){
            flg = 0;
            if(atoi(tmpVal.c_str()) <= 0){
                flg = 2;
                break;
            }
            tmpVal = "";
            continue;
        }
        else if(flg){
            tmpVal += getDim[i];
        }
        
    }
    
    std::string log;
    uint err_found = 0;
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            //std::cout << (*((*symbol_manager)[i]))[j].name << "ooooooooooooooooo\n";
            if((*((*symbol_manager)[i]))[j].name == std::string(p_variable.getNameCString()) && 
               ((*((*symbol_manager)[i]))[j].kind == KIND_LP_VAR && level_cnt != (*((*symbol_manager)[i]))[j].level ||
               i + j < symbol_manager->size() + (*symbol_manager)[i]->size() - 2 && level_cnt == i)
            ){
                //std::cout << p_variable.getNameCString() << "  " << level_cnt << " is aaaa\n";
                (*((*symbol_manager)[i]))[j].redecl = 1;
                err_found = 1;
                
            }
        }
    }
    
    
    
    if(err_found){
        log += "<Error> Found in line " + std::to_string(p_variable.getLocationCString().first) + ", column " + std::to_string(p_variable.getLocationCString().second) + ": symbol '" + std::string(p_variable.getNameCString()) + "' is redeclared\n" + "    " + std::string(line_code[p_variable.getLocationCString().first]) + "\n";
        pointErrLoc(p_variable.getLocationCString().second, log);
        err_flag = 1;
        err_msg.push_back(log);
        symbol_manager->back()->pop_back();
    }
    else if(flg == 2){
        log += "<Error> Found in line " + std::to_string(p_variable.getLocationCString().first) + ", column " + std::to_string(p_variable.getLocationCString().second) + ": '" + std::string(p_variable.getNameCString()) + "' declared as an array with an index that is not greater than 0\n" + "    " + std::string(line_code[p_variable.getLocationCString().first]) + "\n";
        pointErrLoc(p_variable.getLocationCString().second, log);
        err_flag = 1;
        err_msg.push_back(log);
    }else{
        symbol_manager->back()->back().valid = 1;
    }
    
    
    
    
    is_for = 0;
    current_idx++;
    from_var = 0;
    
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    //printf("cons ------ \n");
    //printf("for = %d  var_name = %s \n", is_for, symbol_manager->back()->back().name.c_str());
    if(from_var == 1){
        //std::cout << symbol_manager->back()->back().name << "iiiiiiiiiiiiiiiiii\n";
        symbol_manager->back()->back().kind = KIND_CONST;
        symbol_manager->back()->back().attr = std::string(p_constant_value.getConstantValueCString());
    }
    
    
    
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    SymbolEntry entry = SymbolEntry(std::string(p_function.getNameCString()), KIND_FUNC, 0, std::string(p_function.getTypeCString()), std::string(p_function.getPrototypeCString()));
    SymbolTable* symbol_table = new SymbolTable();
    entry.valid = 1;
    
    uint err_found = 0;
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            //std::cout << (*((*symbol_manager)[i]))[j].name << "ooooooooooooooooo\n";
            if((*((*symbol_manager)[i]))[j].name == std::string(p_function.getNameCString())){
                //std::cout << p_variable.getNameCString() << "  " << level_cnt << " is aaaa\n";
                //(*((*symbol_manager)[i]))[j].redecl = 1;
                err_found = 1;
                
            }
        }
    }
    
    std::string log;
    if(err_found){
        log += "<Error> Found in line " + std::to_string(p_function.getLocationCString().first) + ", column " + std::to_string(p_function.getLocationCString().second) + ": symbol '" + std::string(p_function.getNameCString()) + "' is redeclared\n" + "    " + std::string(line_code[p_function.getLocationCString().first]) + "\n";
        pointErrLoc(p_function.getLocationCString().second, log);
        err_flag = 1;
        err_msg.push_back(log);
    }
    else{
        symbol_manager->back()->push_back(entry);
    }
    
    symbol_manager->push_back(symbol_table);
    cur_func_type = std::string(p_function.getTypeCString());
    level_cnt++;
    is_func = 1;
    is_param = 1;
    
    p_function.visitChildNodes(*this);
    
    is_func = 0;
    dumpSymbol(symbol_manager->back());
    symbol_manager->pop_back();
    
    cur_func_type = "void";
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    
    if(is_func == 0){
        //printf("go to com\n");
        level_cnt++;
        SymbolTable* symbol_table = new SymbolTable();
    
        //symbol_table->push_back(entry);
        symbol_manager->push_back(symbol_table);
        current_idx = 0;
        p_compound_statement.visitChildNodes(*this);
        dumpSymbol(symbol_table);
        symbol_manager->pop_back();
    }
    else{
        is_param = 0;
        is_func = 0;
        p_compound_statement.visitChildNodes(*this);
        
    }
    

    level_cnt--;
    //current_idx = 0;
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_print.visitChildNodes(*this);
    
    if(!p_print.target->valid) return;
    std::string log;
    if(p_print.target->type.find('[') != -1 || p_print.target->type == "void"){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_print.getLocationCString().first) + ", column " + std::to_string(p_print.target->getPos()) + ": expression of print statement must be scalar type\n" + 
        "    " + std::string(line_code[p_print.getLocationCString().first]) + "\n";
        pointErrLoc(p_print.target->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_print.getLocationCString().first] = 1;
        return;
    }
    
    
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_bin_op.visitChildNodes(*this);
    
    if(!p_bin_op.left->valid || !p_bin_op.right->valid) return;
    //std::cout << p_bin_op.left->type << " " << p_bin_op.getOpCString() << " " << p_bin_op.right->type << "\n";
    
    int err_found = 0;
    if(std::string(p_bin_op.getOpCString()) == "+" ||
       std::string(p_bin_op.getOpCString()) == "-" ||
       std::string(p_bin_op.getOpCString()) == "*" ||
       std::string(p_bin_op.getOpCString()) == "/"
    ){
        //printf("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddd\n");
        if((p_bin_op.left->type == "integer" || p_bin_op.left->type == "real" || p_bin_op.left->type == "") && (p_bin_op.right->type == "integer" || p_bin_op.right->type == "real")){
            //
            if(p_bin_op.left->type == p_bin_op.right->type) p_bin_op.setType(p_bin_op.right->type);
            else p_bin_op.setType("real");
        }
        else if(p_bin_op.left->type == "string" && p_bin_op.right->type == "string" && std::string(p_bin_op.getOpCString()) == "+"){
            p_bin_op.setType("string");
        }
        else{
            p_bin_op.valid = 0;
            err_found = 1;
        }
    }
    else if(std::string(p_bin_op.getOpCString()) == "mod"){
        if(p_bin_op.left->type == "integer" && p_bin_op.right->type == "integer") p_bin_op.setType("integer");
        else {
            p_bin_op.valid = 0;
            err_found = 1;
        }
    }
    else if(std::string(p_bin_op.getOpCString()) == "not" ||
            std::string(p_bin_op.getOpCString()) == "and" ||
            std::string(p_bin_op.getOpCString()) == "or"
    ){
        //std::cout << line_code[p_bin_op.getLocationCString().first] << " " << p_bin_op.right->type << "\n";
        if(p_bin_op.left->type == "boolean" && p_bin_op.right->type == "boolean") p_bin_op.setType("boolean");
        else {
            p_bin_op.valid = 0;
            err_found = 1;
        }
    }
    else if(std::string(p_bin_op.getOpCString()) == ">" ||
            std::string(p_bin_op.getOpCString()) == "=" ||
            std::string(p_bin_op.getOpCString()) == "<" ||
            std::string(p_bin_op.getOpCString()) == ">=" ||
            std::string(p_bin_op.getOpCString()) == "<=" ||
            std::string(p_bin_op.getOpCString()) == "<>"
    ){
        if((p_bin_op.left->type == "integer" || p_bin_op.left->type == "real" || p_bin_op.left->type == "") && (p_bin_op.right->type == "integer" || p_bin_op.right->type == "real")){
            //
            p_bin_op.setType("boolean");
        }
        else{
            p_bin_op.valid = 0;
            err_found = 1;
        }
    }
    std::string log = "";
    if(err_found == 1){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_bin_op.getLocationCString().first) + ", column " + std::to_string(p_bin_op.getLocationCString().second) + ": " + "invalid operands to binary operator '" + std::string(p_bin_op.getOpCString()) + "' ('" + p_bin_op.left->type + "' and '" + p_bin_op.right->type + "')\n" + 
        "    " + std::string(line_code[p_bin_op.getLocationCString().first]) + "\n";
        pointErrLoc(p_bin_op.getLocationCString().second, log);
        err_msg.push_back(log);
        err_loc[p_bin_op.getLocationCString().first] = 1;
        return;
    }
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_un_op.visitChildNodes(*this);
    
    if(!p_un_op.operand->valid) return;
    //std::cout << p_bin_op.left->type << " " << p_bin_op.getOpCString() << " " << p_bin_op.right->type << "\n";
    
    int err_found = 0;
    if(std::string(p_un_op.getOpCString()) == "-"){
        //printf("dddddddddddddddddddddddddddddddddddddddddddddddddddddddddd\n");
        if(p_un_op.operand->type == "integer" || p_un_op.operand->type == "real"){
            p_un_op.setType(p_un_op.operand->type);
        }
        else{
            p_un_op.valid = 0;
            err_found = 1;
        }
    }
    else if(std::string(p_un_op.getOpCString()) == "not"){
        //std::cout << line_code[p_bin_op.getLocationCString().first] << " " << p_bin_op.right->type << "\n";
        if(p_un_op.operand->type == "boolean") p_un_op.setType("boolean");
        else {
            p_un_op.valid = 0;
            err_found = 1;
        }
    }
    std::string log = "";
    if(err_found == 1){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_un_op.getLocationCString().first) + ", column " + std::to_string(p_un_op.getLocationCString().second) + ": " + "invalid operand to unary operator '" + std::string(p_un_op.getOpCString()) + "' ('" + p_un_op.operand->type + "')\n" + 
        "    " + std::string(line_code[p_un_op.getLocationCString().first]) + "\n";
        pointErrLoc(p_un_op.getLocationCString().second, log);
        err_msg.push_back(log);
        err_loc[p_un_op.getLocationCString().first] = 1;
        return;
    }
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_func_invocation.visitChildNodes(*this);
    
    int err_found = 1;
    std::string param_check;
    
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            //std::cout << (*((*symbol_manager)[i]))[j].name << "ooooooooooooooooo\n";
            if((*((*symbol_manager)[i]))[j].name == std::string(p_func_invocation.getNameCString())){
                //std::cout << (*((*symbol_manager)[i]))[j].attr <<" is attr\n";
                param_check = (*((*symbol_manager)[i]))[j].attr;
                p_func_invocation.setType((*((*symbol_manager)[i]))[j].type);
                err_found = 0;
                
            }
        }
    }
    //printf("yes\n");
    std::string log;
    
    if(err_found){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_func_invocation.getLocationCString().first) + ", column " + std::to_string(p_func_invocation.getLocationCString().second) + ": use of undeclared symbol '" + std::string(p_func_invocation.getNameCString()) + "'\n" + "    " + std::string(line_code[p_func_invocation.getLocationCString().first]) + "\n";
        pointErrLoc(p_func_invocation.getLocationCString().second, log);
        err_msg.push_back(log);
        p_func_invocation.valid = 0;
        return;
    }
    
    
    
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            if((*((*symbol_manager)[i]))[j].name == std::string(p_func_invocation.getNameCString()) && (*((*symbol_manager)[i]))[j].kind != KIND_FUNC){
                //std::cout << (*((*symbol_manager)[i]))[j].kind << "ooooooooooooooooo\n";
                err_found = 1;
            }
        }
    }
    
    if(err_found){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_func_invocation.getLocationCString().first) + ", column " + std::to_string(p_func_invocation.getLocationCString().second) + ": call of non-function symbol '" + p_func_invocation.getNameCString() + "'\n"
        + "    " + std::string(line_code[p_func_invocation.getLocationCString().first]) + "\n";
        pointErrLoc(p_func_invocation.getLocationCString().second, log);
        err_msg.push_back(log);
        p_func_invocation.valid = 0;
        return;
    }
    
    //printf("yes\n");
    
    //std::string name = std::string(p_func_invocation.getNameCString());
    if(((param_check == "") ? 0 : std::count(param_check.begin(), param_check.end(), ',') + 1) != p_func_invocation.arguments.size()){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_func_invocation.getLocationCString().first) + ", column " + std::to_string(p_func_invocation.getLocationCString().second) + ": too few/much arguments provided for function '" + p_func_invocation.getNameCString() + "'\n"
        + "    " + std::string(line_code[p_func_invocation.getLocationCString().first]) + "\n";
        pointErrLoc(p_func_invocation.getLocationCString().second, log);
        err_msg.push_back(log);
        p_func_invocation.valid = 0;
        return;
    }
    
    if(param_check == "") return;
    
    uint idx = 0;
    std::string tmp_check;
    for(uint i = 0; i < param_check.length()+1; i++){
        
        if(param_check[i] == ',' || i == param_check.length()){
            //std::cout << p_func_invocation.arguments[idx]->type << " A    A " <<tmp_check+"  " <<  p_func_invocation.getNameCString()  <<"  is size\n";
            if(tmp_check != p_func_invocation.arguments[idx]->type ){
                err_flag = 1;
                log += "<Error> Found in line " + std::to_string(p_func_invocation.getLocationCString().first) + ", column " + std::to_string(p_func_invocation.arguments[idx]->getPos()) + ": incompatible type passing '" + p_func_invocation.arguments[idx]->type + "' to parameter of type '"  + tmp_check + "'\n"
                + "    " + std::string(line_code[p_func_invocation.getLocationCString().first]) + "\n";
                pointErrLoc(p_func_invocation.arguments[idx]->getPos(), log);
                err_msg.push_back(log);
                p_func_invocation.valid = 0;
                break;
            }
            tmp_check = "";
            i += 1;
            idx++;
            
        }else {
            tmp_check += param_check[i];
        }
    }
    
    
}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_variable_ref.visitChildNodes(*this);
    
    
    
    
    
    int err_found = 1;
    int err_found2 = 0;
    int check_valid = 1;
    std::string tmp_type = "";
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            //std::cout << (*((*symbol_manager)[i]))[j].name << "    is    name\n";
            if((*((*symbol_manager)[i]))[j].name == std::string(p_variable_ref.getNameCString())){
                //std::cout << (*((*symbol_manager)[i]))[j].name << (*((*symbol_manager)[i]))[j].valid << "    is    name\n";
                if((*((*symbol_manager)[i]))[j].valid == 0 || (*((*symbol_manager)[i]))[j].redecl == 1){
                    (*((*symbol_manager)[i]))[j].redecl = 0;
                    //std::cout << p_variable_ref.getNameCString() << "is redecl\n";
                    p_variable_ref.valid = 0;
                    check_valid = 0;
                }
                tmp_type = (*((*symbol_manager)[i]))[j].type;
                for(uint k = 0; k < p_variable_ref.indices.size(); k++){
                    if(p_variable_ref.indices[k]->valid == 0) check_valid = 0;
                    int pos = tmp_type.find("[");
                    int pos2 = tmp_type.find("]");
                    if(tmp_type.find("[") == -1) err_found2 = 1;
                    else{
                        tmp_type.erase(tmp_type.begin()+pos, tmp_type.begin()+pos2+1);
                    }
                }
                if(tmp_type.find("[") == -1 && tmp_type != (*((*symbol_manager)[i]))[j].type) tmp_type.erase(tmp_type.length() - 1);
                p_variable_ref.setType(tmp_type);
                err_found = 0;
                
            }
        }
    }
    
    //std::cout << check_valid << "check valid\n";
    if(!check_valid) return;
    //printf("yes\n");
    //std::cout << p_variable_ref.getNameCString() << "    is    name\n";
    std::string log;
    
    if(err_found == 1){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_variable_ref.getLocationCString().first) + ", column " + std::to_string(p_variable_ref.getLocationCString().second) + ": use of undeclared symbol '" + p_variable_ref.getNameCString() + "'\n"
        + "    " + std::string(line_code[p_variable_ref.getLocationCString().first]) + "\n";
        pointErrLoc(p_variable_ref.getLocationCString().second, log);
        err_msg.push_back(log);
        p_variable_ref.valid = 0;
        return;
    }
    //err_found = 0;
    
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            //std::cout << (*((*symbol_manager)[i]))[j].kind << " " << (*((*symbol_manager)[i]))[j].name << "    " << p_variable_ref.getNameCString() << "ooooooooooooooooo\n";
            if((*((*symbol_manager)[i]))[j].name == std::string(p_variable_ref.getNameCString()) && 
               ((*((*symbol_manager)[i]))[j].kind == KIND_FUNC ||
                (*((*symbol_manager)[i]))[j].kind == KIND_PROG)
            ){
                //std::cout << (*((*symbol_manager)[i]))[j].kind << " " << (*((*symbol_manager)[i]))[j].name << "ooooooooooooooooo\n";
                err_found = 1;
            }
        }
    }
    
    if(err_found == 1){
        err_flag = 1;
        //std::cout << (*((*symbol_manager)[i]))[j].kind << "ooooooooooooooooo\n";
        log += "<Error> Found in line " + std::to_string(p_variable_ref.getLocationCString().first) + ", column " + std::to_string(p_variable_ref.getLocationCString().second) + ": use of non-variable symbol '" + p_variable_ref.getNameCString() + "'\n"
        + "    " + std::string(line_code[p_variable_ref.getLocationCString().first]) + "\n";
        pointErrLoc(p_variable_ref.getLocationCString().second, log);
        err_msg.push_back(log);
        p_variable_ref.valid = 0;
        return;
    }
    
    //std::cout << "is  " << std::string(p_variable_ref.getTypeCString());
    uint flg = 0; 
    uint brack_cnt = 0;
    std::string check = std::string(line_code[p_variable_ref.getLocationCString().first]);
    for(uint i = 0; i < check.length(); i++){
        if(check[i] == '['){
            flg = 1;
            brack_cnt++;
        }
        else if(check[i] == ']') flg = 0;
        else if(flg && !isdigit(check[i])){
            err_found = 1;
        }
    }
    //std::stringstream ss;
    
    if(err_found == 1){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_variable_ref.getLocationCString().first) + ", column " + std::to_string(p_variable_ref.getLocationCString().second + strlen(p_variable_ref.getNameCString()) + 1) + ": index of array reference must be an integer\n"
        + "    " + std::string(line_code[p_variable_ref.getLocationCString().first]) + "\n";
        pointErrLoc(p_variable_ref.getLocationCString().second + strlen(p_variable_ref.getNameCString()) + 1, log);
        err_msg.push_back(log);
    }
    else if(err_found2){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_variable_ref.getLocationCString().first) + ", column " + std::to_string(p_variable_ref.getLocationCString().second) + ": there is an over array subscript on '" + p_variable_ref.getNameCString() + "'\n"
        + "    " + std::string(line_code[p_variable_ref.getLocationCString().first]) + "\n";
        pointErrLoc(p_variable_ref.getLocationCString().second, log);
        err_msg.push_back(log);
    }
    
    //err_msg.push_back(ss);
    
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_assignment.visitChildNodes(*this);
    
    if(!p_assignment.lvalue->valid) return;
    
    std::string log;
    uint err_found = 0;
    if(p_assignment.lvalue->type.find('[') != -1){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_assignment.getLocationCString().first) + ", column " + std::to_string(p_assignment.lvalue->getPos()) + ": array assignment is not allowed\n" + 
        "    " + std::string(line_code[p_assignment.getLocationCString().first]) + "\n";
        pointErrLoc(p_assignment.lvalue->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_assignment.getLocationCString().first] = 1;
        return;
    }
    //std::cout << p_assignment.lvalue->type << "  is type\n";
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            if((*((*symbol_manager)[i]))[j].name == std::string(p_assignment.lvalue->getNameCString()) && 
               (*((*symbol_manager)[i]))[j].kind == KIND_CONST){
                //std::cout << (*((*symbol_manager)[i]))[j].kind << "ooooooooooooooooo\n";
                err_found = 1;
            }
        }
    }
    if(err_found){
        err_flag = 1;
        
        log += "<Error> Found in line " + std::to_string(p_assignment.getLocationCString().first) + ", column " + std::to_string(p_assignment.lvalue->getPos()) + ": cannot assign to variable '" + p_assignment.lvalue->getNameCString() + "' which is a constant\n" + 
        "    " + std::string(line_code[p_assignment.getLocationCString().first]) + "\n";
        pointErrLoc(p_assignment.lvalue->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_assignment.getLocationCString().first] = 1;
        return;
    }
    
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            if((*((*symbol_manager)[i]))[j].name == std::string(p_assignment.lvalue->getNameCString()) && 
               (*((*symbol_manager)[i]))[j].kind == KIND_LP_VAR && 
               (*((*symbol_manager)[i]))[j].level < level_cnt){
                //std::cout << (*((*symbol_manager)[i]))[j].kind << "ooooooooooooooooo\n";
                err_found = 1;
            }
        }
    }
    
    if(err_found){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_assignment.getLocationCString().first) + ", column " + std::to_string(p_assignment.lvalue->getPos()) + ": the value of loop variable cannot be modified inside the loop body\n" + 
        "    " + std::string(line_code[p_assignment.getLocationCString().first]) + "\n";
        pointErrLoc(p_assignment.lvalue->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_assignment.getLocationCString().first] = 1;
    }
    
    if(p_assignment.expr->type.find('[') != -1){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_assignment.getLocationCString().first) + ", column " + std::to_string(p_assignment.expr->getPos()) + ": array assignment is not allowed\n" + 
        "    " + std::string(line_code[p_assignment.getLocationCString().first]) + "\n";
        pointErrLoc(p_assignment.expr->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_assignment.getLocationCString().first] = 1;
        return;
    }
    
    if(p_assignment.expr->type != p_assignment.lvalue->type && p_assignment.expr->type + p_assignment.lvalue->type != "realinteger" && p_assignment.expr->type + p_assignment.lvalue->type != "integerreal"){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_assignment.getLocationCString().first) + ", column " + std::to_string((p_assignment.expr->getPos()-3)) + ": assigning to '" + p_assignment.lvalue->type + "' from incompatible type '" + p_assignment.expr->type + "'\n" + 
        "    " + std::string(line_code[p_assignment.getLocationCString().first]) + "\n";
        pointErrLoc(p_assignment.expr->getPos()-3, log);
        err_msg.push_back(log);
        err_loc[p_assignment.getLocationCString().first] = 1;
    }
    
    
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_read.visitChildNodes(*this);
    
    std::string log;
    uint err_found = 0;
    if(p_read.target->type.find('[') != -1){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_read.getLocationCString().first) + ", column " + std::to_string(p_read.target->getPos()) + ": variable reference of read statement must be scalar type\n" + 
        "    " + std::string(line_code[p_read.getLocationCString().first]) + "\n";
        pointErrLoc(p_read.target->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_read.getLocationCString().first] = 1;
        return;
    }
    
    for(uint i = 0; i < symbol_manager->size(); i++){
        for(uint j = 0; j < (*symbol_manager)[i]->size(); j++){
            if((*((*symbol_manager)[i]))[j].name == std::string(p_read.target->getNameCString()) && ((*((*symbol_manager)[i]))[j].kind == KIND_LP_VAR || (*((*symbol_manager)[i]))[j].kind == KIND_CONST)){
                //std::cout << (*((*symbol_manager)[i]))[j].kind << "ooooooooooooooooo\n";
                err_found = 1;
            }
        }
    }
    if(err_found){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_read.getLocationCString().first) + ", column " + std::to_string(p_read.target->getPos()) + ": variable reference of read statement cannot be a constant or loop variable\n" + 
        "    " + std::string(line_code[p_read.getLocationCString().first]) + "\n";
        pointErrLoc(p_read.target->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_read.getLocationCString().first] = 1;
    }
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_if.visitChildNodes(*this);
    if(!p_if.condition->valid) return;
    std::string log;
    if(p_if.condition->type != "boolean"){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_if.getLocationCString().first) + ", column " + std::to_string(p_if.condition->getPos()) + ": " + "the expression of condition must be boolean type\n" + 
        "    " + std::string(line_code[p_if.getLocationCString().first]) + "\n";
        pointErrLoc(p_if.condition->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_if.getLocationCString().first] = 1;
    }
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_while.visitChildNodes(*this);
    std::string log;
    if(p_while.condition->type != "boolean"){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_while.getLocationCString().first) + ", column " + std::to_string(p_while.condition->getPos()) + ": " + "the expression of condition must be boolean type\n" + 
        "    " + std::string(line_code[p_while.getLocationCString().first]) + "\n";
        pointErrLoc(p_while.condition->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_while.getLocationCString().first] = 1;
    }
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    is_for = 1;
    SymbolTable* symbol_table = new SymbolTable();
    
        //symbol_table->push_back(entry);
    symbol_manager->push_back(symbol_table);
    level_cnt++;
    p_for.visitChildNodes(*this);
    int flg = 0, x[2];
    std::string incre_check = std::string(line_code[p_for.getLocationCString().first]);
    std::string tmp;
    std::string log;

    for(uint i = 0; i < incre_check.length(); i++){
        if(incre_check[i] == '='){
            flg = 1;
        }
        //if(incre_check[i] == '1') printf("yes  ");
        if(isdigit(incre_check[i]) && flg){
            tmp += incre_check[i];
        }
        else if(!isdigit(incre_check[i]) && flg && !tmp.empty()){
            
            x[flg-1] = atoi(tmp.c_str());
            tmp = "";
            flg++;
            if(flg > 2)break;
        }
        
    }
    
    if(x[0] > x[1]){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_for.getLocationCString().first) + ", column " + std::to_string(p_for.getLocationCString().second) + ": the lower bound and upper bound of iteration count must be in the incremental order\n" + 
        "    " + std::string(line_code[p_for.getLocationCString().first]) + "\n";
        pointErrLoc(p_for.getLocationCString().second, log);
        err_msg.push_back(log);
        err_loc[p_for.getLocationCString().first] = 1;
    }
    
    dumpSymbol(symbol_manager->back());
    symbol_manager->pop_back();
    level_cnt--;
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    /*
     * TODO:
     *
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node.
     * 5. Pop the symbol table pushed at the 1st step.
     */
    p_return.visitChildNodes(*this);
    std::string log;
    if(cur_func_type == "void"){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_return.getLocationCString().first) + ", column " + std::to_string(p_return.getLocationCString().second) + ": program/procedure should not return a value\n" + 
        "    " + std::string(line_code[p_return.getLocationCString().first]) + "\n";
        pointErrLoc(p_return.getLocationCString().second, log);
        err_msg.push_back(log);
        err_loc[p_return.getLocationCString().first] = 1;
        return;
    }
    
    if(p_return.retval->type != cur_func_type){
        err_flag = 1;
        log += "<Error> Found in line " + std::to_string(p_return.getLocationCString().first) + ", column " + std::to_string(p_return.retval->getPos()) + ": return '" + p_return.retval->type + "' from a function with return type '" + cur_func_type + "'\n" + 
        "    " + std::string(line_code[p_return.getLocationCString().first]) + "\n";
        pointErrLoc(p_return.retval->getPos(), log);
        err_msg.push_back(log);
        err_loc[p_return.getLocationCString().first] = 1;
        return;
    }
}
