#include "sema/SymbolTable.hpp"
#include <iomanip>
#include <cstdio>

extern int32_t opt_dmp;
 
 void dumpDemarcation(const char chr) {
    for (size_t i = 0; i < 110; ++i) {
        std::cout << chr;
    }
    std::cout << '\n';
}

void dumpSymbol(SymbolTable * symbol_table) {
    if(opt_dmp){
        dumpDemarcation('=');
        printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type", "Attribute");
        dumpDemarcation('-');
        for(uint i = 0; i < symbol_table->size(); ++i) {
            std::cout << std::left << std::setw(33) << (*symbol_table)[i].name;
            
            switch((*symbol_table)[i].kind) {
                case KIND_PROG: 
                    std::cout << std::left << std::setw(11) << "program"; break;
                case KIND_FUNC: 
                    std::cout << std::left << std::setw(11) << "function"; break;
                case KIND_PARAM: 
                    std::cout << std::left << std::setw(11) << "parameter"; break;
                case KIND_VAR: 
                    std::cout << std::left << std::setw(11) << "variable"; break;
                case KIND_LP_VAR:
                    std::cout << std::left << std::setw(11) << "loop_var"; break;
                case KIND_CONST: 
                    std::cout << std::left << std::setw(11) << "constant"; break;
            }
            
            std::string tmp = std::to_string((*symbol_table)[i].level);
            if((*symbol_table)[i].level > 0) {
                tmp += "(local)";
                std::cout << std::left << std::setw(11) << tmp;
            } else {
                tmp += "(global)";
                std::cout << std::left << std::setw(11) << tmp;
            }
            
            std::cout << std::left << std::setw(17) << (*symbol_table)[i].type;
            
            std::cout << std::left << std::setw(11) << (*symbol_table)[i].attr;
            
            std::cout << '\n';
        }
        dumpDemarcation('-');
    }
    
}
