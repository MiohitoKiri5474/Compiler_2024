#ifndef MAIN_H
#define MAIN_H
#include "compiler_common.h"

#include <stdio.h>

extern FILE* yyin;
extern bool compileError;
int yyparse();
int yylex();
int yylex_destroy();

#define VAR_FLAG_DEFAULT 0
#define VAR_FLAG_ARRAY 0b00000001
#define VAR_FLAG_POINTER 0b00000010

void pushScope();
void dumpScope();

void pushFunParm(ObjectType variableType, char* variableName, int parmFlag);
void createFunction(ObjectType variableType, char* funcName);
void pushFunInParm(Object* b);

Object* findVariable(char* variableName);
Object* createVariable(ObjectType variableType, char* variableName, int variableFlag);
bool objectExpression(char op, Object* a, Object* b, Object* out);
bool objectExpBinary(char op, Object* a, Object* b, Object* out);
bool objectExpBoolean(char op, Object* a, Object* b, Object* out);
bool objectExpAssign(char op, Object* dest, Object* val, Object* out);
bool objectValueAssign(Object* dest, Object* val, Object* out);
bool objectNotBinaryExpression(Object* dest, Object* out);
bool objectNotExpression(Object* dest, Object* out);
bool objectNegExpression(Object* dest, Object* out);
bool objectIncAssign(Object* a, Object* out);
bool objectDecAssign(Object* a, Object* out);
bool objectCast(ObjectType variableType, Object* dest, Object* out);

void stdoutPrint();

typedef struct _node {
    struct _node *l, *r;
    int idx, addr, lineno;
    int sz, pri;
    char name[32], func[32];

    ObjectType type;
} Node;

typedef struct _table {
    Node *treap;
    int cnt;
} Table;

int max(int, int);
char *get_op_name(op_t);
char *get_type_name(ObjectType);
int get_op_priority(op_t);
char get_type(ObjectType);
int Get_Level(void);
void ScopeAddOne(void);
void ScopeMinusOne(void);

Node *New_Node(int, int);
void up(Node **);
Node *merge(Node *, Node *);
Node *Insert_Head(Node *, Node *);
Node *Insert_Tail(Node *, Node *);
void Insert_Symbol(char *, ObjectType, char *, int);
void Insert_Node(Node *);
Table *New_Table(void);
void Create_Table();
void Dump_Table();
void Print_Treap(Node **o);
Node *Query_Symbol(char *);
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
