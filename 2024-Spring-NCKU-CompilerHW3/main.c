#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler_common.h"

#define WJCL_LINKED_LIST_IMPLEMENTATION
#include "main.h"
#define WJCL_HASH_MAP_IMPLEMENTATION

#define debug printf("%s:%d: ############### debug\n", __FILE__, __LINE__)

char *yyInputFileName;
bool compileError;

#define toupper(_char) (_char - (char) 32)

char *yyInputFileName;
bool compileError;

int indent = 0;
int scopeLevel = 0;
int funcLineNo = 0;
int variableAddress = 0;
int addr;
int arg_idx;
int c_exp;
int if_status;
ObjectType variableIdentType;

Table *symbol_table[1024];
bool table_list[1024];
Node *table_tmp[1024];
int table_tmp_idx[1024];
treap *root;
IDENT_List *IDENT_head = NULL, *IDENT_tail = NULL;

void reset_addr(void)
{
    arg_idx = 0;
}

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
    root = NULL;
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
        up(&o->l);
    }
    if (o->r) {
        o->sz += o->r->sz;
        up(&o->r);
    }
}

Node *dfs(Node **_o, char *name)
{
    Node *o = *_o;
    if (!o)
        return NULL;
    if (!strcmp(o->name, name))
        return o;
    Node *tmp = dfs(&o->l, name);
    if (tmp)
        return tmp;
    tmp = dfs(&o->r, name);
    if (tmp)
        return tmp;
    return NULL;
}

void Update_Symbol(char *name, char *value)
{
    int idx = 0;
    update(&symbol_table[idx]->treap, name, value);
}

void Update_Symbol_Value(char *name, int value)
{
    Query_Symbol(name)->value = value;
}

bool update(Node **_o, char *name, char *value)
{
    Node *o = *_o;
    if (!o)
        return false;
    if (!strcmp(o->name, name)) {
        strcpy(o->func, value);
        return true;
    }
    return update(&o->l, name, value) || update(&o->r, name, value);
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

int Query_Symbol_Value(char *name)
{
    return Query_Symbol(name)->value;
}

Node *Query_Symbol(char *name)
{
    int idx = scopeLevel - 1;
    while (idx >= 0) {
        Node *tmp;
        tmp = dfs(&symbol_table[idx]->treap, name);
        if (tmp)
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
    res->return_type = OBJECT_TYPE_VOID;
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

void update_argument_return(char *name, char *argument, ObjectType type)
{
    Node *tmp = Query_Symbol(name);
    strcpy(tmp->argument, argument);
    tmp->return_type = type;
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

Node *Insert_Symbol(char *name, ObjectType type, char *func, int lineno)
{
    int idx = scopeLevel - 1;
    Node *tmp = Create_Node(
        name, type, func,
        lineno - (!strcmp(name, "argv") || !strcmp(name, "main") ? 0 : 1));
    if (!table_list[idx])
        table_tmp[table_tmp_idx[idx]++] = tmp;
    else
        Insert_Node(tmp);
    return tmp;
}

Node *Insert_Symbol_Argument(char *name,
                             ObjectType type,
                             char *func,
                             int lineno)
{
    int idx = scopeLevel - 1;
    Node *tmp = Create_Node(
        name, type, func,
        lineno - (!strcmp(name, "argv") || !strcmp(name, "main") ? 0 : 1));
    tmp->addr = arg_idx++;
    if (!table_list[idx])
        table_tmp[table_tmp_idx[idx]++] = tmp;
    else
        Insert_Node(tmp);
    return tmp;
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

int Get_Level(void)
{
    return scopeLevel;
}

void if_status_update(int _stt)
{
    if_status = _stt;
}

void Print_Treap(Node **_o)
{
    Node *o = *_o;
    if (!o)
        return;
    Print_Treap(&o->l);

    printf("%-10d%-20s%-10s%-10d%-10d%-10s\n", o->idx, o->name,
           get_type_name(o->type), o->addr, o->lineno, o->func);
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

char *get_return_type(ObjectType type)
{
    switch (type) {
    case OBJECT_TYPE_INT:
    case OBJECT_TYPE_LONG:
    case OBJECT_TYPE_BOOL:
        return "i";
    case OBJECT_TYPE_DOUBLE:
    case OBJECT_TYPE_FLOAT:
        return "f";
    case OBJECT_TYPE_VOID:
        return "";
    default:
        return "ERROR";
    }
}

char *get_print_type(ObjectType type)
{
    switch (type) {
    case OBJECT_TYPE_INT:
    case OBJECT_TYPE_LONG:
        return "I";
    case OBJECT_TYPE_DOUBLE:
    case OBJECT_TYPE_FLOAT:
        return "F";
    case OBJECT_TYPE_BOOL:
        return "Z";
    case OBJECT_TYPE_STR:
        return "Ljava/lang/String;";
    case OBJECT_TYPE_FUNCTION:
        return "func";
    case OBJECT_TYPE_CHAR:
        return "Ljava/lang/String;";
    case OBJECT_TYPE_UNDEFINED:
        return "Z";
    case OBJECT_TYPE_VOID:
        return "V";
    default:
        return "ERROR";
    }
}

char get_type(ObjectType type)
{
    return (char) (get_type_name(type)[0] - 'a' + 'A');
}

void get_op_inst(char *buf, ObjectType type, op_t op)
{
    char tmp[16];
    int idx = 0;
    buf[0] = get_type_name(type)[0], idx = 1;
    if (type == -1 || op == OP_EQL || op == OP_NEQ || op == OP_LES ||
        op == OP_LEQ || op == OP_GTR || op == OP_NEQ || op == OP_LOR ||
        op == OP_LAN || op == OP_GEQ) {
        idx = 0;
    }

    switch (op) {
    case OP_DIV:
    case OP_DIV_ASSIGN:
        strcpy(tmp, "DIV");
        break;
    case OP_LOR:
        strcpy(tmp, "IOR");
        break;
    case OP_LAN:
        strcpy(tmp, "IAND");
        break;
    case OP_EQL:
        strcpy(tmp, "IF_ICMPEQ");
        // strcpy(tmp, (if_status ? "IF_ICMLE" : "icmle"));
        break;
    case OP_NEQ:
        strcpy(tmp, "IF_ICMPNE");
        // strcpy(tmp, (if_status ? "IF_ICMNE" : "icmne"));
        break;
    case OP_LES:
        strcpy(tmp, (type == OBJECT_TYPE_FLOAT ? "fcmpl" : "IF_ICMPLT"));
        // strcpy(tmp, (if_status ? "IF_ICMLT" : "icmlt"));
        break;
    case OP_LEQ:
        strcpy(tmp, "IF_ICMPLE");
        // strcpy(tmp, (if_status ? "IF_ICMLE" : "icmle"));
        break;
    case OP_GTR:
        strcpy(tmp, (type == OBJECT_TYPE_FLOAT ? "fcmpg" : "IF_ICMPGT"));
        // strcpy(tmp, (if_status ? "IF_ICMGT" : "icmpg"));
        break;
    case OP_GEQ:
        strcpy(tmp, "IF_ICMPGE");
        // strcpy(tmp, (if_status ? "IF_ICMGE" : "icmge"));
        break;
    case OP_NOT:
        strcpy(tmp, "iconst_m1");
        idx = 0;
        break;
    default:
        strcpy(tmp, get_op_name(op));
    }

    for (int i = 0; i < strlen(tmp); i++)
        buf[idx + i] =
            ('A' <= tmp[i] && tmp[i] <= 'Z' ? (char) tmp[i] + 32 : tmp[i]);
    buf[strlen(tmp) + idx] = '\0';
}

void c_exp_update(int n)
{
    c_exp = n;
}

void ScopeAddOne(void)
{
    scopeLevel++;
}

void ScopeMinusOne(void)
{
    scopeLevel--;
}

int get_scope(void)
{
    return scopeLevel;
}

char *get_op_name(op_t op)
{
    if (op == OP_LBRA)
        return "LBRA";
    switch (op) {
    case OP_ADD:
        return "ADD";
    case OP_SUB:
        return "SUB";
    case OP_MUL:
        return "MUL";
    case OP_DIV:
        return "DIV";
    case OP_REM:
        return "REM";
    case OP_EQL:
        return "EQL";
    case OP_NEQ:
        return "NEQ";
    case OP_LES:
        return "LES";
    case OP_LEQ:
        return "LEQ";
    case OP_GTR:
        return "GTR";
    case OP_GEQ:
        return "GEQ";
    case OP_POS:
        return "POS";
    case OP_NEG:
        return "NEG";
    case OP_NOT:
        return "NOT";
    case OP_LOR:
        return "LOR";
    case OP_LAN:
        return "LAN";
    case OP_BOR:
        return "OR";
    case OP_BAN:
        return "AND";
    case OP_BNT:
        return "const_m1\n\tixor";
    case OP_BXO:
        return "XOR";
    case OP_VAL_ASSIGN:
        return "EQL_ASSIGN";
    case OP_ADD_ASSIGN:
        return "ADD";
    case OP_SUB_ASSIGN:
        return "SUB";
    case OP_MUL_ASSIGN:
        return "MUL";
    case OP_DIV_ASSIGN:
        return "DIV";
    case OP_REM_ASSIGN:
        return "REM";
    case OP_BAN_ASSIGN:
        return "AND";
    case OP_BOR_ASSIGN:
        return "OR";
    case OP_BXO_ASSIGN:
        return "BXO";
    case OP_SHL_ASSIGN:
        return "SHL";
    case OP_SHR_ASSIGN:
        return "SHR";
    case OP_INC_ASSIGN:
        return "INC";
    case OP_DEC_ASSIGN:
        return "INC";
    case OP_LSHIFT:
        return "SHL";
    case OP_RSHIFT:
        return "SHR";
    default:
        return "";
    }
}

char *get_ls_name(ObjectType type, int inst)
{
    switch (type) {
    case OBJECT_TYPE_INT:
        return !inst ? "iload" : "istore";
    case OBJECT_TYPE_DOUBLE:
    case OBJECT_TYPE_FLOAT:
        return !inst ? "fload" : "fstore";
    case OBJECT_TYPE_BOOL:
        return !inst ? "iload" : "istore";
    case OBJECT_TYPE_STR:
        return !inst ? "aload" : "astore";
    case OBJECT_TYPE_FUNCTION:
        return "func";
    default:
        return "ERROR";
    }
}

int get_c_exp(void)
{
    return c_exp;
}

int get_op_priority(op_t op)
{
    switch (op) {
    case OP_NOT:
        return 6;
    case OP_MUL:
    case OP_DIV:
    case OP_REM:
        return 5;
    case OP_ADD:
    case OP_SUB:
        return 4;
    case OP_GTR:
    case OP_LES:
    case OP_LEQ:
    case OP_GEQ:
    case OP_NEQ:
    case OP_EQL:
        return 3;
    case OP_LAN:
    case OP_LOR:
        return 2;
    case OP_VAL_ASSIGN:
    case OP_ADD_ASSIGN:
    case OP_SUB_ASSIGN:
    case OP_MUL_ASSIGN:
    case OP_DIV_ASSIGN:
    case OP_REM_ASSIGN:
    case OP_BAN_ASSIGN:
    case OP_BOR_ASSIGN:
    case OP_BXO_ASSIGN:
    case OP_SHL_ASSIGN:
    case OP_SHR_ASSIGN:
    case OP_INC_ASSIGN:
    case OP_DEC_ASSIGN:
        return 1;
    default:
        return -1;
    }
}

IDENT_List *New_IDENT(char *name)
{
    IDENT_List *res = (IDENT_List *) malloc(sizeof(IDENT_List));
    strcpy(res->name, name);
    return res;
}

void Reset_IDENT(void)
{
    IDENT_head = (IDENT_List *) malloc(sizeof(IDENT_List));
    IDENT_tail = IDENT_head;
}

void IDENT_Push(char *name)
{
    if (!IDENT_head)
        Reset_IDENT();
    IDENT_List *ins = New_IDENT(name);
    IDENT_tail->next = ins;
    ins->prev = IDENT_tail;
    IDENT_tail = ins;
}

void IDENT_Pop(void)
{
    IDENT_head->next->prev = NULL;
    IDENT_head = IDENT_head->next;
}

char *IDENT_front(void)
{
    if (!IDENT_head->next)
        return "";
    return IDENT_head->next->name;
}

bool IDENT_Empty(void)
{
    return IDENT_head == IDENT_tail;
}

int main(int argc, char *argv[])
{
    char *outputFileName = NULL;
    if (argc == 3) {
        yyin = fopen(yyInputFileName = argv[1], "r");
        yyout = fopen(outputFileName = argv[2], "w");
    } else if (argc == 2) {
        yyin = fopen(yyInputFileName = argv[1], "r");
        yyout = stdout;
    } else {
        printf("require input file");
        exit(1);
    }
    if (!yyin) {
        printf("file `%s` doesn't exists or cannot be opened\n",
               yyInputFileName);
        exit(1);
    }
    if (!yyout) {
        printf("file `%s` doesn't exists or cannot be opened\n",
               outputFileName);
        exit(1);
    }

    ScopeAddOne();
    codeRaw(".source hw3.j");
    codeRaw(".class public Main");
    codeRaw(".super java/lang/Object\n");
    ScopeMinusOne();

    yyparse();
    printf("Total lines: %d\n", yylineno);
    fclose(yyin);

    while (scopeLevel)
        Dump_Table();
    yylex_destroy();
    return 0;
}
