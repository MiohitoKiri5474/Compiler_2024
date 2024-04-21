/* Definition section */
%{
    #include "compiler_common.h"
    #include "compiler_util.h"
    #include "main.h"

    int yydebug = 1;
%}

/* Variable or self-defined structure */
%union {
    ObjectType var_type;

    bool b_var;
    int i_var;
    float f_var;
    char *s_var;

    struct {
        union {
            char *s_var;
            bool b_var;
            int i_var;
            float f_var;
        };
        ObjectType type;
    } object_val;
}

/* Token without return */
%token COUT
%token SHR SHL BAN BOR BNT BXO ADD SUB MUL DIV REM NOT GTR LES GEQ LEQ EQL NEQ LAN LOR
%token VAL_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN REM_ASSIGN BAN_ASSIGN BOR_ASSIGN BXO_ASSIGN SHR_ASSIGN SHL_ASSIGN INC_ASSIGN DEC_ASSIGN
%token IF ELSE FOR WHILE RETURN BREAK CONTINUE

/* Token with return, which need to sepcify type */
%token <var_type> VARIABLE_T
%token <s_var> IDENT
%token <i_var> INT_LIT
%token <s_var> STR_LIT
%token <b_var> BOOL_LIT
%token <f_var> FLOAT_LIT

/* Nonterminal with return, which need to sepcify type */
%type <object_val> Expression
%type <s_var> FunctionParameterStmtList
%type <object_val> PrimaryExpr
%type <object_val> UnaryExpr
%type <object_val> Literal
%type <object_val> Operand

%left ADD SUB
%left MUL DIV REM

/* Yacc will start at this nonterminal */
%start Program

%%
/* Grammar section */

Program
    : { Create_Table(); } GlobalStmtList
    | /* Empty file */
;

GlobalStmtList
    : GlobalStmtList GlobalStmt
    | GlobalStmt
;

GlobalStmt
    : DefineVariableStmt
    | FunctionDefStmt
;

DefineVariableStmt
    : VARIABLE_T IDENT VAL_ASSIGN Expression ';'
;

/* Function */
FunctionDefStmt
    : VARIABLE_T IDENT {
        printf ( "func: %s\n", $<s_var>2 );
        Insert_Symbol ( $2, OBJECT_TYPE_FUNCTION, "([Ljava/lang/String;)I", yylineno + 1 );
        Create_Table();
    } '(' FunctionParameterStmtList ')' {
        char tmp[4];
        tmp[0] = ')', tmp[1] = get_type ( $1 ), tmp[2] = '\0';
        strcat ( $5, tmp );
    } '{' StmtList '}' { Dump_Table(); }
;

FunctionParameterStmtList
    : FunctionParameterStmtList ',' VARIABLE_T IDENT {
        char tmp[2];
        tmp[0] = get_type ( $3 ), tmp[1] = '\0';
        strcat ( $$, tmp );

        Insert_Symbol ( $<s_var>4, $3, "", yylineno + 1 );
    }
    | VARIABLE_T IDENT '[' ']' {
        $$ = ( char * ) malloc ( sizeof ( char ) * 1024 );
        $$[0] = '(';
        $$[1] = get_type ( $1 );
        $$[2] = '\0';

        Insert_Symbol ( $<s_var>2, $1, "", yylineno + 1 );
    }
    | VARIABLE_T IDENT {
        $$ = ( char * ) malloc ( sizeof ( char ) * 1024 );
        $$[0] = '(';
        $$[1] = get_type ( $1 );
        $$[2] = '\0';

        Insert_Symbol ( $<s_var>2, $1, "", yylineno + 1 );
    }
;

/* Scope */
StmtList 
    : Stmt StmtList
    | Stmt
;

Stmt
    : COUT { Reset_treap(); } CoutParmListStmt ';' {
        printf ( "cout" );
        Print_List();
    }
    | RETURN Expression ';' { printf("RETURN\n"); }
    | ';'
;

CoutParmListStmt
    : SHL Expression { Insert_Cout ( get_type_name ( $2.type ) ); } CoutParmListStmt
    | SHL Expression { Insert_Cout ( get_type_name ( $2.type ) ); }
;

Expression
    : Expression binary_op Expression
    | UnaryExpr
;

UnaryExpr
    : PrimaryExpr { $$.type = $1.type; }
;

PrimaryExpr
    : Operand
;

Operand
    : Literal
    | IDENT {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        if ( tmp ) {
            $$.type = tmp -> type;
            printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        }
        else if ( !strcmp ( "endl", $<s_var>1 ) ) {
            $$.type = OBJECT_TYPE_STR;
            puts ( "IDENT (name=endl, address=-1)" );
        }
    }
;

Literal
    : {} INT_LIT {
        $$.type = OBJECT_TYPE_INT;
        $$.i_var = atoi ( $<s_var>1 );
        printf ( "INT_LIT %d\n", $$.i_var );
    }
    | FLOAT_LIT {
        $$.type = OBJECT_TYPE_FLOAT;
        $$.f_var = atof ( $<s_var>1 );
        printf ( "FLOAT_LIT %f\n", $$.f_var );
    }
    | BOOL_LIT {
        $$.type = OBJECT_TYPE_BOOL;
        $$.b_var = !strcmp ( $<s_var>1, "true" ) ? true : false;
        printf ( "BOOL_LIT %s\n", $$.b_var ? "TRUE" : "FALSE" );
    }
    | STR_LIT {
        $$.type = OBJECT_TYPE_STR;
        $$.s_var = $<s_var>1;
        printf ( "STR_LIT \"%s\"\n", $$.s_var );
    }
;

binary_op
    : GTR
;

%%
/* C code section */
