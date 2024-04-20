#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "compiler_common.h"


#define debug printf("%s:%d: ############### debug\n", __FILE__, __LINE__)

#define toupper(_char) (_char - (char) 32)
const char *objectTypeName[] = {
    [OBJECT_TYPE_UNDEFINED] = "undefined",
    [OBJECT_TYPE_VOID] = "void",
    [OBJECT_TYPE_INT] = "int",
    [OBJECT_TYPE_FLOAT] = "float",
    [OBJECT_TYPE_BOOL] = "bool",
    [OBJECT_TYPE_STR] = "string",
    [OBJECT_TYPE_FUNCTION] = "function",
};

char *yyInputFileName;
bool compileError;

int indent = 0;
int scopeLevel = -1;
int funcLineNo = 0;
int variableAddress = 0;
int addr;
ObjectType variableIdentType;

Table *symbol_table[1024];
bool table_list[1024];
Node *table_tmp[1024];
int table_tmp_idx[1024];

int max(int a, int b)
{
    return a > b ? a : b;
}

int Sz(Node *o)
{
    return o ? o->sz : 0;
}

void up(Node **_o)
{
    Node *o = *_o;
    o->sz = 1;
    if (o->l) {
        o->sz += o->l->sz;
    }
    if (o->r) {
        o->sz += o->r->sz;
    }
}

Node *merge(Node *a, Node *b)
{
    if (!a || !b)
        return a ? a : b;
    if (a->pri < b->pri) {
        a->r = merge(a->r, b);
        up(&a);
        return a;
    }
    b->l = merge(a, b->l);
    up(&b);
    return b;
}

Node *Create_Node(char *name, ObjectType type, char *func, int lineno)
{
    Node *res = (Node *) malloc(sizeof(Node));
    int idx = scopeLevel - 1;
    if (symbol_table[idx]) {
        symbol_table[idx] = (Table *) malloc(sizeof(Table));
        symbol_table[idx]->treap = NULL;
        symbol_table[idx]->cnt = 0;
    }
    res->idx = Sz(symbol_table[idx]->treap) + 1;

    res->type = type;
    strcpy(res->name, name);
    strcpy(res->func, func);
    if (strlen(func) != 0)
        res->addr = -1;
    else {
        res->addr = addr++;
        res->lineno++;
        strcpy(res->func, "-\0");
    }
    return res;
}

Node *Insert_Head(Node *o, Node *n)
{
    return merge(o, n);
}

Node *Insert_Tail(Node *o, Node *n)
{
    return merge(n, o);
}

void Insert_Node(Node *node)
{
    int idx = scopeLevel - 1;
    printf("> Insert `%s` (addr: %d) to scope level %d\n", node->name,
           node->addr, idx);
    symbol_table[idx]->treap = Insert_Tail(symbol_table[idx]->treap, node);
}

Table *New_Table(void)
{
    Table *res = (Table *) malloc(sizeof(Table));
    res->treap = NULL;
    res->cnt = 0;
    return res;
}

void Insert_Symbol(char *name, ObjectType type, char *func, int lineno)
{
    int idx = scopeLevel - 1;
    Node *tmp = NULL;
    if (!table_list[idx])
        table_tmp[table_tmp_idx[idx]++] = tmp;
    else
        Insert_Node(tmp);
}

void Create_Table()
{
    table_list[++scopeLevel] = true;
    printf("> Create symbol table (scope level %d)\n", scopeLevel);
    symbol_table[scopeLevel] = New_Table();
    if (table_tmp_idx[scopeLevel]) {
        for (int i = 0; i < table_tmp_idx[scopeLevel]; i++) {
            Insert_Node(table_tmp[i]);
        }
    }
}

void Dump_Table()
{
    int idx = scopeLevel - 1;
    Node *tmp = symbol_table[idx]->treap;
    printf(
        "\n> Dump symbol table (scope level: "
        "%d)\n%-10s%-10s%-10s%-10s%-10s%-10s\n",
        idx, "Index", "Name", "Type", "Addr", "Lineno", "Func_sig");
    dfs(&tmp);
    table_tmp_idx[--scopeLevel] = 0;
    table_list[scopeLevel] = false;
    symbol_table[idx] = (Table *) malloc(sizeof(Table));
    symbol_table[idx]->treap = NULL;
    symbol_table[idx]->cnt = 0;
}

void dfs(Node **_o)
{
    Node *o = *_o;
    if (o->l)
        dfs(&o->l);

    printf("%-10d%-10s%-10s%-10d%-10d%-10s\n", o->idx, o->name,
           get_type_name(o->type), o->addr, o->lineno, o->func);
    if (o->r)
        dfs(&o->r);
}

char *get_type_name(ObjectType type)
{
    switch (type) {
    case OBJECT_TYPE_UNDEFINED:
        return "int";
    case OBJECT_TYPE_AUTO:
        return "auto";
    case OBJECT_TYPE_VOID:
        return "void";
    case OBJECT_TYPE_CHAR:
        return "char";
    case OBJECT_TYPE_INT:
        return "int";
    case OBJECT_TYPE_LONG:
        return "long";
    case OBJECT_TYPE_FLOAT:
        return "float";
    case OBJECT_TYPE_DOUBLE:
        return "double";
    case OBJECT_TYPE_BOOL:
        return "bool";
    case OBJECT_TYPE_STR:
        return "string";
    case OBJECT_TYPE_FUNCTION:
        return "function";

    default:
        return "undefined";
    }
}

int main(int argc, char *argv[])
{
    srand(clock());
    if (argc == 2) {
        yyin = fopen(yyInputFileName = argv[1], "r");
    } else {
        yyin = stdin;
    }
    if (!yyin) {
        printf("file `%s` doesn't exists or cannot be opened\n",
               yyInputFileName);
        exit(1);
    }

    // Start parsing
    yyparse();
    printf("Total lines: %d\n", yylineno);
    fclose(yyin);

    yylex_destroy();
    return 0;
}
