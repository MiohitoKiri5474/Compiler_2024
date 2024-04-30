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

typedef struct _treap {
    struct _treap *l, *r;
    char val[1024], sz, pri;
} treap;

typedef union _stack_val {
    op_t ops;
    struct {
        union {
            char *s_var;
            bool b_var;
            int i_var;
            float f_var;
        };
        ObjectType type;
    } val;
    enum { op, data } type;
} Stack_Val;

typedef struct _stack {
    struct _stack *l, *r;
    int sz, pri;
    Stack_Val value;
} Stack;

int max(int, int);
char *get_op_name(op_t);
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
char *get_type_name(ObjectType);
char get_type(ObjectType);
void ScopeAddOne(void);
void ScopeMinusOne(void);
Node *Query_Symbol(char *);
Node *dfs(Node **, char *);

treap *New_treap(char *);
int Sz_treap(treap *);
void up_treap(treap **);
treap *merge_treap(treap *, treap *);
void Print_Cout(treap **);
void Print_List(void);
void Insert_Cout(char *);
void Reset_treap(void);

Stack *New_Stack(Stack_Val);
int Sz_Stack(Stack *);
void up_Stack(Stack **);
Stack *merge_Stack(Stack *, Stack *);
void Print_Stack(void);
void __print_Stack(Stack **);
void Stack_Push(Stack_Val);
void Reset_Stack(void);

#endif
