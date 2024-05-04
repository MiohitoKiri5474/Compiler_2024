#ifndef COMPILER_COMMON_H
#define COMPILER_COMMON_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum _objectType {
    OBJECT_TYPE_UNDEFINED,
    OBJECT_TYPE_AUTO,
    OBJECT_TYPE_VOID,
    OBJECT_TYPE_CHAR,
    OBJECT_TYPE_INT,
    OBJECT_TYPE_LONG,
    OBJECT_TYPE_FLOAT,
    OBJECT_TYPE_DOUBLE,
    OBJECT_TYPE_BOOL,
    OBJECT_TYPE_STR,
    OBJECT_TYPE_FUNCTION,
} ObjectType;

typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_REM,
    OP_EQL,
    OP_NEQ,
    OP_LES,
    OP_LEQ,
    OP_GTR,
    OP_GEQ,
    OP_POS,
    OP_NEG,
    OP_NOT,
    OP_LOR,
    OP_LAN,
	OP_BOR,
	OP_BAN,
    OP_VAL_ASSIGN,
    OP_ADD_ASSIGN,
    OP_SUB_ASSIGN,
    OP_MUL_ASSIGN,
    OP_DIV_ASSIGN,
    OP_REM_ASSIGN,
    OP_BAN_ASSIGN,
    OP_BOR_ASSIGN,
    OP_BXO_ASSIGN,
    OP_SHL_ASSIGN,
    OP_SHR_ASSIGN,
    OP_INC_ASSIGN,
    OP_DEC_ASSIGN,
	OP_LSHIFT,
	OP_RSHIFT,
    OP_LBRA,
    OP_RBRA,
} op_t;

typedef struct _symbolData {
    char* name;
    int32_t index;
    int64_t addr;
    int32_t lineno;
    char* func_sig;
    uint8_t func_var;
} SymbolData;

typedef struct _object {
    ObjectType type;
    uint64_t value;
    SymbolData* symbol;
} Object;

extern int yylineno;
extern int funcLineNo;

#endif /* COMPILER_COMMON_H */
