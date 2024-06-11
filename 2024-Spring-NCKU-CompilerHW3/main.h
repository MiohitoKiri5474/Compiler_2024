#ifndef MAIN_H
#define MAIN_H
#include "compiler_common.h"

#include <stdio.h>

#define code(format, ...) \
    fprintf(yyout, "%*s" format "\n", ( scopeLevel - 1 ) << 2, "", __VA_ARGS__)
#define codeRaw(code) \
    fprintf(yyout, "%*s" code "\n", ( scopeLevel - 1 ) << 2, "")

extern FILE* yyout;
extern FILE* yyin;
extern bool compileError;
extern int scopeLevel;
int yyparse();
int yylex();
int yylex_destroy();

#define VAR_FLAG_DEFAULT 0
#define VAR_FLAG_ARRAY 0b00000001
#define VAR_FLAG_POINTER 0b00000010

typedef struct _node {
    struct _node *l, *r;
    int idx, addr, lineno;
    int sz, pri;
    char name[1024], func[1024], argument[1024];
    int value;

    ObjectType type, return_type;
} Node;

typedef struct _table {
    Node *treap;
    int cnt;
} Table;

int max(int, int);
Node *Create_Node(char *, ObjectType, char *, int);
void update_argument_return(char *, char *, ObjectType);
char *get_op_name(op_t);
char *get_type_name(ObjectType);
char *get_return_type(ObjectType);
char *get_print_type(ObjectType);
int get_op_priority(op_t);
char get_type(ObjectType);
int Get_Level(void);
void ScopeAddOne(void);
void ScopeMinusOne(void);
int get_scope(void);
void c_exp_update(int);
void if_status_update(int);
void get_op_inst(char *, ObjectType, op_t);
int get_c_exp(void);
char *get_ls_name(ObjectType, int);

Node *New_Node(int, int);
void up(Node **);
Node *merge(Node *, Node *);
Node *Insert_Head(Node *, Node *);
Node *Insert_Tail(Node *, Node *);
Node *Insert_Symbol(char *, ObjectType, char *, int);
void Update_Symbol(char *, char *);
void Update_Symbol_Value(char *, int);
bool update(Node **, char *, char *);
void Insert_Node(Node *);
Table *New_Table(void);
void Create_Table();
void Dump_Table();
void Print_Treap(Node **o);
Node *Query_Symbol(char *);
int Query_Symbol_Value(char *);
Node *dfs(Node **, char *);

typedef struct _treap {
    struct _treap *l, *r;
    char val[32];
    int sz, pri;
} treap;

treap *New_treap(char *);
int Sz_treap(treap *);
void up_treap(treap **);
treap *merge_treap(treap *, treap *);
void Print_Cout(treap **);
void Print_List(void);
void Insert_Cout(char *);
void Reset_treap(void);

typedef struct _ident_list {
    struct _ident_list *next, *prev;
    char name[32];
} IDENT_List;

IDENT_List *New_IDENT(char *);
void Reset_IDENT(void);
void IDENT_Push(char *);
void IDENT_Pop(void);
char *IDENT_front(void);
bool IDENT_Empty(void);

#endif
