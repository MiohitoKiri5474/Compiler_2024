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
int scopeLevel = 0;
int funcLineNo = 0;
int variableAddress = 0;
int addr;
ObjectType variableIdentType;

Table *symbol_table[1024];
bool table_list[1024];
Node *table_tmp[1024];
int table_tmp_idx[1024];
treap *root;

// treap for cout
treap *New_treap(char *name)
{
    treap *res = (treap *) malloc(sizeof(treap));
    strcpy(res->val, name);
    res->sz = 1, res->pri = rand();
    return res;
}

int Sz_treap(treap *o)
{
    return o ? o->sz : 0;
}

void up_treap(treap **_o)
{
    treap *o = *_o;
    o->sz = 1 + Sz_treap(o->l) + Sz_treap(o->r);
}

treap *merge_treap(treap *a, treap *b)
{
    if (!a || !b)
        return a ? a : b;
    if (a->pri < b->pri) {
        a->r = merge_treap(a->r, b);
        up_treap(&a);
        return a;
    }
    b->l = merge_treap(a, b->l);
    up_treap(&b);
    return b;
}

void Print_List(void)
{
    Print_Cout(&root);
    putchar('\n');
}

void Print_Cout(treap **_o)
{
    treap *o = *_o;
    if (!o)
        return;
    Print_Cout(&o->l);
    printf(" %s", o->val);
    Print_Cout(&o->r);
}

void Insert_Cout(char *name)
{
    root = merge_treap(root, New_treap(name));
}

void Reset_treap(void)
{
    Free_treap(&root);
    root = NULL;
}

void Free_treap(treap **_o)
{
    treap *o = *_o;
    if (!o)
        return;
    free(o->val);
    Free_treap(&o->l);
    Free_treap(&o->r);
    free(o);
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

Node *dfs(Node **_o, char *name)
{
    Node *o = *_o;
    if (!o)
        return NULL;
    if (!strcmp(o->name, name))
        return o;
    Node *tmp;
    tmp = dfs(&o->l, name);
    if (tmp)
        return tmp;
    tmp = dfs(&o->r, name);
    if (tmp)
        return tmp;
    return NULL;
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

Node *Query_Symbol(char *name)
{
    int idx = scopeLevel - 1;
    while (idx >= 0) {
        Node *tmp = symbol_table[idx]->treap;
        tmp = dfs(&tmp, name);
        if (!tmp)
            return tmp;
        idx--;
    }
    return NULL;
}

Node *Create_Node(char *name, ObjectType type, char *func, int lineno)
{
    Node *res = (Node *) malloc(sizeof(Node));
    int idx = scopeLevel - 1;
    if (!symbol_table[idx]) {
        symbol_table[idx] = (Table *) malloc(sizeof(Table));
        symbol_table[idx]->treap = NULL;
        symbol_table[idx]->cnt = 0;
    }
    res->idx = Sz(symbol_table[idx]->treap);
    res->sz = 1;
    res->pri = rand();

    res->type = type;
    strcpy(res->name, name);
    strcpy(res->func, func);
    res->lineno = lineno;
    if (strlen(func) != 0)
        res->addr = -1;
    else {
        res->addr = addr++;
        res->lineno++;
        strcpy(res->func, "-\0");
    }
    return res;
}

Node *Insert_Tail(Node *o, Node *n)
{
    return merge(o, n);
}

Node *Insert_Head(Node *o, Node *n)
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
    Node *tmp = Create_Node(name, type, func, lineno);
    if (!table_list[idx])
        table_tmp[table_tmp_idx[idx]++] = tmp;
    else
        Insert_Node(tmp);
}

void Create_Table()
{
    table_list[scopeLevel] = true;
    printf("> Create symbol table (scope level %d)\n", scopeLevel);
    symbol_table[scopeLevel] = New_Table();
    if (table_tmp_idx[scopeLevel]) {
        for (int i = 0; i < table_tmp_idx[scopeLevel]; i++) {
            Insert_Node(table_tmp[i]);
        }
    }
    scopeLevel++;
}

void Dump_Table()
{
    int idx = scopeLevel - 1;
    Node *tmp = symbol_table[idx]->treap;
    printf(
        "\n> Dump symbol table (scope level: "
        "%d)\n%-10s%-20s%-10s%-10s%-10s%-10s\n",
        idx, "Index", "Name", "Type", "Addr", "Lineno", "Func_sig");
    Print_Treap(&tmp);
    table_tmp_idx[--scopeLevel] = 0;
    table_list[scopeLevel] = false;
    symbol_table[idx] = (Table *) malloc(sizeof(Table));
    symbol_table[idx]->treap = NULL;
    symbol_table[idx]->cnt = 0;
}

void Print_Treap(Node **_o)
{
    Node *o = *_o;
    if (o->l)
        Print_Treap(&o->l);

    printf("%-10d%-20s%-10s%-10d%-10d%-10s\n", o->idx, o->name,
           get_type_name(o->type), o->addr, o->lineno, o->func);
    if (o->r)
        Print_Treap(&o->r);
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

char get_type(ObjectType type)
{
    return (char) (get_type_name(type)[0] - 'a' + 'A');
}

void ScopeAddOne(void)
{
    scopeLevel++;
}

void ScopeMinusOne(void)
{
    scopeLevel--;
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

    while (scopeLevel)
        Dump_Table();

    printf("Total lines: %d\n", yylineno);
    fclose(yyin);

    yylex_destroy();
    return 0;
}
