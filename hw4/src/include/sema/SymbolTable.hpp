#include <vector>
#include <iostream>
#include <string>

enum KType{
    KIND_PROG = 100,
    KIND_FUNC,
    KIND_PARAM,
    KIND_VAR,
    KIND_LP_VAR,
    KIND_CONST
};


struct SymbolEntry {
    SymbolEntry(std::string __name,
                KType __kind,
                uint __level,
                std::string __type,
                std::string __attr ){
        name = __name;
        kind = __kind;
        level = __level;
        type = __type;
        attr = __attr;
        valid = 0;
        redecl = 0;
    }
    std::string name;
    KType kind;
    uint level;
    std::string type;
    std::string attr;
    uint valid;
    uint redecl;
} ;

typedef std::vector<SymbolEntry> SymbolTable;
typedef std::vector <SymbolTable *> SymbolManager;

void dumpDemarcation(const char chr);
void dumpSymbol(SymbolTable* symbol_table);

