#include "codegen/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <vector>
#include <string.h>

bool print_gvar_header = false;
bool print_gconst_header = false;
bool print_main_header = false;
bool print_func_header = false;
bool is_assign = false;
int param_stack_idx = 0;
bool tmp_reg_stat[6];
int b_cnt = 0;
std::vector<int> b_target;


int get_avail_idx(){
    for(int i = 0; i < 6; i++){
        if(!tmp_reg_stat[i]){
            tmp_reg_stat[i] = true;
            return i;
        }
    }
}


CodeGenerator::CodeGenerator(const char *in_file_name, const char *out_file_name, SymbolManager *symbol_manager) 
    : in_file_name(in_file_name), symbol_manager(symbol_manager) {
    this->out_fp = fopen(out_file_name, "w");
    assert(this->out_fp != NULL && "fopen() fails");
}

CodeGenerator::~CodeGenerator() {
    fclose(this->out_fp);
}

void CodeGenerator::dumpInstrs(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(this->out_fp, format, args);
    va_end(args);
}

void CodeGenerator::visit(ProgramNode &p_program) {
    // Reconstruct the hash table for looking up the symbol entry
    // Hint: Use symbol_manager->lookup(symbol_name) to get the symbol entry.
    symbol_manager->reconstructHashTableFromSymbolTable(p_program.getSymbolTable());

    // Generate RISC-V instructions for program header
    dumpInstrs("%s%s%s%s", 
        "   .file \"", this->in_file_name, "\"\n",
        "   .option nopic\n"
    );
    
    

    p_program.visitChildNodes(*this);
    
    

    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_program.getSymbolTable());
}

void CodeGenerator::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

void CodeGenerator::visit(VariableNode &p_variable) {
    const SymbolEntry *cur_entr = symbol_manager->lookup(std::string(p_variable.getNameCString()));
    //if(!print_gvar_header)
    if(cur_entr->getLevel() == 0 && cur_entr->getKind() == SymbolEntry::KindEnum::kVariableKind){
        if(!print_gvar_header){
            dumpInstrs(".section   .bss\n");
            print_gvar_header = true;
        }
        dumpInstrs("%s:\n", p_variable.getNameCString());
        dumpInstrs("    .word 0\n");
        
    }else if(cur_entr->getLevel() == 0 && cur_entr->getKind() == SymbolEntry::KindEnum::kConstantKind){
        if(!print_gconst_header){
            dumpInstrs(".section   .rodata\n");
            print_gconst_header = true;
        }
        dumpInstrs("%s:\n", p_variable.getNameCString());
        dumpInstrs("    .word %s\n", cur_entr->getAttribute().constant()->getConstantValueCString());
    }else if(cur_entr->getKind() == SymbolEntry::KindEnum::kParameterKind){
        if(param_stack_idx < 8){
            dumpInstrs("    sw %c%d, %d%s", 'a', param_stack_idx, param_stack_idx*(-4)-12, "(s0)\n");
            //cur_entr->setLocationOffset(param_stack_idx*(-4)-12);
            param_stack_idx++;
        }else{
            int use_idx = get_avail_idx();
            dumpInstrs("    sw %c%d, %d%s", 't', param_stack_idx - 8, param_stack_idx*(-4)-12, "(s0)\n"); 
            //cur_entr->setLocationOffset(param_stack_idx*(-4)-12);
            param_stack_idx++;
        }
        
    }else{
        /*int use_idx = get_avail_idx();
        dumpInstrs("    addi %c%d, s0, %d\n", 't', use_idx, param_stack_idx*(-4)-12);
        //cur_entr->setLocationOffset(param_stack_idx*(-4)-12);
        dumpInstrs("    addi sp, sp, 4\n");
        dumpInstrs("    sw %c%d, 0%s", 't', use_idx, "(sp)\n"); */
        if(cur_entr->getKind() == SymbolEntry::KindEnum::kConstantKind){
            dumpInstrs("    addi t0, zero, %s\n", cur_entr->getAttribute().constant()->getConstantValueCString());
            dumpInstrs("    sw t0, %d(s0)\n", cur_entr->getLocationOffset());
        }
        param_stack_idx++;
    }
    
}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {
    if(!strcmp(p_constant_value.getConstantValueCString(), "true")){
        dumpInstrs("    li t0, 1\n");
    }else if(!strcmp(p_constant_value.getConstantValueCString(), "false")){
        dumpInstrs("    li t0, 0\n");
    }else{
        dumpInstrs("    li t0, %s\n", p_constant_value.getConstantValueCString());
    }
    
    dumpInstrs("    addi sp, sp, -4\n");
    dumpInstrs("    sw t0, 0(sp)\n");
}

void CodeGenerator::visit(FunctionNode &p_function) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_function.getSymbolTable());
    dumpInstrs(".section   .text\n");
    dumpInstrs("    .align 2\n");
    dumpInstrs("    .global %s\n", p_function.getNameCString());
    dumpInstrs("    .type %s, @function\n", p_function.getNameCString());
    dumpInstrs("%s:\n", p_function.getNameCString());
    
    print_func_header = true;
    
    dumpInstrs("    addi sp, sp, -128\n");
    dumpInstrs("    sw ra, 124(sp)\n");
    dumpInstrs("    sw s0, 120(sp)\n");
    dumpInstrs("    addi s0, sp, 128\n");
    
    p_function.visitChildNodes(*this);

    dumpInstrs("    lw ra, 124(sp)\n");
    dumpInstrs("    lw s0, 120(sp)\n");
    dumpInstrs("    addi sp, sp, 128\n");
    dumpInstrs("    jr ra\n");
    
    // Remove the entries in the hash table
    print_func_header = false;
    param_stack_idx = 0;
    symbol_manager->removeSymbolsFromHashTable(p_function.getSymbolTable());
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_compound_statement.getSymbolTable());
    if(!print_func_header && !print_main_header){
        print_main_header = true;
        dumpInstrs(".section   .text\n");
        dumpInstrs("    .align 2\n");
        dumpInstrs("    .global %s\n", "main");
        dumpInstrs("    .type %s, @function\n", "main");
        dumpInstrs("%s:\n", "main");
        dumpInstrs("    addi sp, sp, -128\n");
        dumpInstrs("    sw ra, 124(sp)\n");
        dumpInstrs("    sw s0, 120(sp)\n");
        dumpInstrs("    addi s0, sp, 128\n");
        
        p_compound_statement.visitChildNodes(*this);
        //recover
        dumpInstrs("    lw ra, 124(sp)\n");
        dumpInstrs("    lw s0, 120(sp)\n");
        dumpInstrs("    addi sp, sp, 128\n");
        dumpInstrs("    jr ra\n");
    }else{
        p_compound_statement.visitChildNodes(*this);
    }
    
    
    
    
    
    // Remove the entries in the hash table
    symbol_manager->removeSymbolsFromHashTable(p_compound_statement.getSymbolTable());
}

void CodeGenerator::visit(PrintNode &p_print) {
    p_print.visitChildNodes(*this);
    dumpInstrs("    lw a0, 0(sp)\n");
    dumpInstrs("    addi sp, sp, 4\n");
    dumpInstrs("    jal %s, %s\n", "ra", "printInt");
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    p_bin_op.visitChildNodes(*this);
    int use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, 0(sp)\n", 't', use_idx);
    dumpInstrs("    addi sp, sp, 4\n");
    int new_use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, 0(sp)\n", 't', new_use_idx);
    dumpInstrs("    addi sp, sp, 4\n");
    switch (p_bin_op.getOp()) {
    case Operator::kPlusOp:
        dumpInstrs("    add t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kMinusOp:
        dumpInstrs("    sub t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kMultiplyOp:
        dumpInstrs("    mul t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kDivideOp:
        dumpInstrs("    div t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kModOp:
        dumpInstrs("    rem t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kAndOp:
        dumpInstrs("    and t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kOrOp:
        dumpInstrs("    or t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kLessOp:
        dumpInstrs("    slt t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kLessOrEqualOp:
        dumpInstrs("    slt t0, %c%d, %c%d\n", 't', use_idx, 't', new_use_idx);
        dumpInstrs("    xori t0, t0, 1\n");
        break;
    case Operator::kEqualOp:
        dumpInstrs("    xor t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        dumpInstrs("    seqz t0, t0\n");
        break;
    case Operator::kGreaterOp:
        dumpInstrs("    slt t0, %c%d, %c%d\n", 't', use_idx, 't', new_use_idx);
        break;
    case Operator::kGreaterOrEqualOp:
        dumpInstrs("    slt t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        dumpInstrs("    xori t0, t0, 1\n");
        break;
    case Operator::kNotEqualOp:
        dumpInstrs("    xor t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        dumpInstrs("    snez t0, t0\n");
        break;
    default:
        break;
    }
    dumpInstrs("    addi sp, sp, -4\n");
    dumpInstrs("    sw t0, 0(sp)\n");
    tmp_reg_stat[use_idx] = false;
    tmp_reg_stat[new_use_idx] = false;
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {
    p_un_op.visitChildNodes(*this);
    int use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, 0(sp)\n", 't', use_idx);
    dumpInstrs("    addi sp, sp, 4\n");
    int new_use_idx = get_avail_idx();
    
    switch (p_un_op.getOp()) {
    case Operator::kNegOp:
        dumpInstrs("    mv %c%d, zero\n", 't', new_use_idx);
        dumpInstrs("    sub t0, %c%d, %c%d\n", 't', new_use_idx, 't', use_idx);
        break;
    case Operator::kNotOp:
        dumpInstrs("    xori t0, %c%d, 1\n", 't', use_idx);
        break;
    default:
        break;
    }
    dumpInstrs("    addi sp, sp, -4\n");
    dumpInstrs("    sw t0, 0(sp)\n");
    tmp_reg_stat[use_idx] = false;
    tmp_reg_stat[new_use_idx] = false;
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {
    p_func_invocation.visitChildNodes(*this);
    
    for(int i = p_func_invocation.getArguments().size() - 1; i >= 0; i--){
        if(i < 8){
            dumpInstrs("    lw %c%d, 0(sp)\n", 'a', i);
            dumpInstrs("    addi sp, sp, 4\n");
        }else{
            dumpInstrs("    lw %c%d, 0(sp)\n", 't', i-8);
            dumpInstrs("    addi sp, sp, 4\n");
        }
    }
    dumpInstrs("    jal %s, %s\n", "ra", p_func_invocation.getNameCString());
    int use_idx = get_avail_idx();
    dumpInstrs("    mv %c%d, a0\n", 't', use_idx); //TODO delete tmp reg, const val node when assignment
    dumpInstrs("    addi sp, sp, -4\n");
    dumpInstrs("    sw %c%d, 0(sp)\n", 't', use_idx);
    tmp_reg_stat[use_idx] = false;
}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    const SymbolEntry *cur_entr = symbol_manager->lookup(std::string(p_variable_ref.getNameCString()));
    if(!is_assign){
        if(cur_entr == nullptr || cur_entr->getLevel() == 0){
            int use_idx = get_avail_idx();
            dumpInstrs("    %s %c%d, %s\n", "la", 't', use_idx, p_variable_ref.getNameCString());
            int new_use_idx = get_avail_idx();
            dumpInstrs("    %s %c%d, 0(t%d)\n", "lw", 't', new_use_idx, use_idx);
            dumpInstrs("    %s %c%d, %c%d\n", "mv", 't', use_idx, 't', new_use_idx);
            dumpInstrs("    addi sp, sp, -4\n");
            dumpInstrs("    %s %c%d, %d%s\n", "sw", 't', use_idx, 0, "(sp)");
            tmp_reg_stat[use_idx] = false;
            tmp_reg_stat[new_use_idx] = false;
        }else if(cur_entr->getLevel() > 1){
            int use_idx = get_avail_idx();
            dumpInstrs("    %s %c%d, %d%s\n", "lw", 't', use_idx, param_stack_idx*(-4)-12+4*(cur_entr->getLevel() - 1), "(s0)");
            //param_stack_idx++;
            dumpInstrs("    addi sp, sp, -4\n");
            dumpInstrs("    %s %c%d, %d%s\n", "sw", 't', use_idx, 0, "(sp)");
            tmp_reg_stat[use_idx] = false;
        }else{
            int use_idx = get_avail_idx();
            dumpInstrs("    %s %c%d, %d%s\n", "lw", 't', use_idx, cur_entr->getLocationOffset(), "(s0)");
            dumpInstrs("    addi sp, sp, -4\n");
            dumpInstrs("    %s %c%d, %d%s\n", "sw", 't', use_idx, 0, "(sp)");
            tmp_reg_stat[use_idx] = false;
        }
    }else{
        is_assign = false;
    }
    
    
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    int use_idx = get_avail_idx();
    const SymbolEntry *cur_entr = symbol_manager->lookup(std::string(p_assignment.getLvalue()->getNameCString()));
    is_assign = true;
    if(cur_entr == nullptr || cur_entr->getLevel() == 0){ //TODO
        dumpInstrs("    la %c%d, %s\n", 't', use_idx, p_assignment.getLvalue()->getNameCString());
    }else if(cur_entr->getLevel() > 1){
        dumpInstrs("    addi %c%d, s0, %d\n", 't', use_idx, param_stack_idx*(-4)-12);
    }else{
        dumpInstrs("    addi %c%d, s0, %d\n", 't', use_idx, cur_entr->getLocationOffset());
    }
    
    dumpInstrs("    addi sp, sp, -4\n");
    dumpInstrs("    sw %c%d, 0(sp)\n", 't', use_idx);
    tmp_reg_stat[use_idx] = false;
    
    p_assignment.visitChildNodes(*this);
    
    use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, %d%s\n", 't', use_idx, 0, "(sp)");
    dumpInstrs("    addi sp, sp, 4\n");
    int new_use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, %d%s\n", 't', new_use_idx, 0, "(sp)");
    dumpInstrs("    addi sp, sp, 4\n");
    dumpInstrs("    sw %c%d, 0(t%d)\n", 't', use_idx, new_use_idx);
    tmp_reg_stat[use_idx] = false;
    tmp_reg_stat[new_use_idx] = false;
}

void CodeGenerator::visit(ReadNode &p_read) {
    //p_read.visitChildNodes(*this);
    const SymbolEntry *cur_entr = symbol_manager->lookup(std::string(p_read.target->getNameCString()));
    int use_idx = get_avail_idx();
    if(cur_entr == nullptr || cur_entr->getLevel() == 0){
        dumpInstrs("    %s %c%d, %s\n", "la", 't', use_idx, p_read.target->getNameCString());
        
        dumpInstrs("    addi sp, sp, -4\n");
        dumpInstrs("    %s %c%d, %d%s\n", "sw", 't', use_idx, 0, "(sp)");
    }else{
        dumpInstrs("    %s %c%d, %d%s\n", "lw", 't', use_idx, cur_entr->getLocationOffset(), "(s0)");
        dumpInstrs("    addi sp, sp, -4\n");
        dumpInstrs("    %s %c%d, %d%s\n", "sw", 't', use_idx, 0, "(sp)");
    }
    tmp_reg_stat[use_idx] = false;
    
    dumpInstrs("    jal %s, %s\n", "ra", "readInt");
    use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, 0(sp)\n", 't', use_idx);
    dumpInstrs("    addi sp, sp, 4\n");
    dumpInstrs("    sw %c%d, 0(t%d)\n", 'a', 0, use_idx);
    tmp_reg_stat[use_idx] = false;
    
}

void CodeGenerator::visit(IfNode &p_if) {
    p_if.condition->accept(*this);
    int use_idx = 1;
    dumpInstrs("    li t%d, 1\n", use_idx);
    if(p_if.check_exist_else()){
        dumpInstrs("    lw t0, 0(sp)\n");
        dumpInstrs("    addi sp, sp, 4\n");
        dumpInstrs("    bne t%d, t0, L%d\n", use_idx, b_cnt+1);
        dumpInstrs("L%d:\n", b_cnt);
        p_if.body->accept(*this);
        dumpInstrs("    j L%d\n", b_cnt+2);
        dumpInstrs("L%d:\n", b_cnt+1);
        p_if.else_body->accept(*this);
        dumpInstrs("L%d:\n", b_cnt+2);
        b_cnt += 3;
    }else{
        dumpInstrs("    lw t0, 0(sp)\n");
        dumpInstrs("    addi sp, sp, 4\n");
        dumpInstrs("    bne t%d, t0, L%d\n", use_idx, b_cnt+1);
        dumpInstrs("L%d:\n", b_cnt);
        p_if.body->accept(*this);
        dumpInstrs("L%d:\n", b_cnt+1);
        b_cnt += 2;
    }
}

void CodeGenerator::visit(WhileNode &p_while) {
    dumpInstrs("L%d:\n", b_cnt);
    p_while.condition->accept(*this);
    dumpInstrs("    li t1, 1\n");
    dumpInstrs("    lw t0, 0(sp)\n");
    dumpInstrs("    addi sp, sp, 4\n");
    dumpInstrs("    bne t1, t0, L%d\n", b_cnt+2);
    dumpInstrs("L%d:\n", b_cnt+1);
    p_while.body->accept(*this);
    dumpInstrs("    j L%d\n", b_cnt);
    dumpInstrs("L%d:\n", b_cnt+2);
    b_cnt += 3;
}

void CodeGenerator::visit(ForNode &p_for) {
    // Reconstruct the hash table for looking up the symbol entry
    symbol_manager->reconstructHashTableFromSymbolTable(p_for.getSymbolTable());
    //const SymbolEntry *cur_entr = symbol_manager->lookup(std::string(p_assignment.getLvalue()->getNameCString()));
    int i_loc = param_stack_idx*(-4)-12;
        
    p_for.initial_statement->accept(*this);
    dumpInstrs("L%d:\n", b_cnt);
    b_target.push_back(b_cnt);
    param_stack_idx++;
    int use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, %d(s0)\n", 't', use_idx, i_loc);
    dumpInstrs("    addi sp, sp, -4\n");
    dumpInstrs("    sw %c%d, 0(sp)\n", 't', use_idx);
    dumpInstrs("    li %c%d, %s\n", 't', use_idx, p_for.condition->getConstantValueCString());
    dumpInstrs("    addi sp, sp, -4\n");
    dumpInstrs("    sw %c%d, 0(sp)\n", 't', use_idx);
    dumpInstrs("    lw %c%d, 0(sp)\n", 't', use_idx);
    dumpInstrs("    addi sp, sp, 4\n");
    int new_use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, 0(sp)\n", 't', new_use_idx);
    dumpInstrs("    addi sp, sp, 4\n");
    dumpInstrs("    bge t%d, t%d, L%d\n", new_use_idx, use_idx, b_cnt+2);
    dumpInstrs("L%d:\n", b_cnt+1);
    tmp_reg_stat[use_idx] = false;
    tmp_reg_stat[new_use_idx] = false;
    b_cnt += 3;
    p_for.body->accept(*this);
    int ori_b_cnt = b_target.back();
    b_target.pop_back();
    use_idx = get_avail_idx();
    dumpInstrs("    lw %c%d, %d(s0)\n", 't', use_idx, i_loc);
    dumpInstrs("    addi %c%d, %c%d, 1\n", 't', use_idx, 't', use_idx);
    new_use_idx = get_avail_idx();
    dumpInstrs("    addi %c%d, s0, %d\n", 't', new_use_idx, i_loc);
    dumpInstrs("    sw %c%d, 0(%c%d)\n", 't', use_idx, 't', new_use_idx);
    dumpInstrs("    j L%d\n", ori_b_cnt);
    dumpInstrs("L%d:\n", ori_b_cnt+2);
    //b_cnt += 3;
    // Remove the entries in the hash table
    param_stack_idx--;
    symbol_manager->removeSymbolsFromHashTable(p_for.getSymbolTable());
}

void CodeGenerator::visit(ReturnNode &p_return) {
    p_return.visitChildNodes(*this);
    dumpInstrs("    lw t0, 0(sp)\n");
    dumpInstrs("    addi sp, sp, 4\n");
    dumpInstrs("    mv a0, t0\n");
}
