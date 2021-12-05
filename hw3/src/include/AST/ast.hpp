#ifndef __AST_H
#define __AST_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>


#include "visitor/AstNodeVisitor.hpp"


typedef std::vector< class AstNode* > NodeList;
typedef class AstNode*           Node;

enum enumOperator{
    OP_ASSIGN = 500,
    OP_OR,
    OP_AND,
    OP_NOT,
    OP_LESS,
    OP_LESS_OR_EQUAL,
    OP_EQUAL,
    OP_GREATER,
    OP_GREATER_OR_EQUAL,
    OP_NOT_EQUAL,
    OP_PLUS,
    OP_MINUS,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_MOD,
    UNKNOWN_OP
};

enum enumTypeSet{
    SET_SCALAR = 300,
    SET_ACCUMLATED,
    SET_CONSTANT_LITERAL,
    UNKNOWN_SET
};

enum enumType{
    TYPE_INTEGER = 400,
    TYPE_REAL,
    TYPE_STRING,
    TYPE_BOOLEAN,
    TYPE_VOID,
    UNKNOWN_TYPE
};




typedef struct __VariableInfo{
    enumTypeSet type_set;
    enumType type;
    std::vector<int> array_size;
    std::string text;
} VariableInfo ;

struct NodeWithType{
    Node node;
    VariableInfo* type;
    uint counter;
};

struct Location {
    Location(const uint32_t line, const uint32_t col) : line(line), col(col) {}

    uint32_t line;
    uint32_t col;
};

class AstNode {
  public:
    AstNode(const uint32_t line, const uint32_t col);
    virtual ~AstNode() = 0;

    const Location &getLocation() const;
    virtual void print() = 0;

    virtual void accept(class AstNodeVisitor &v) = 0;

  protected:
    const Location location;
};

#endif
