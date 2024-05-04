/* Definition section */
%{
    #include "compiler_common.h"
    #include "compiler_util.h"
    #include "main.h"

    // int yydebug = 1;

    int op_idx = 0;
    bool is_bool = false, is_float = false;
    op_t ops[1024];
%}

/* Variable or self-defined structure */
%union {
    ObjectType var_type;

    char *s_var;
    int i_var;
    float f_var;
    bool b_var;

    op_t op;

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
%token COUT NEWLINE
%token SHR SHL BAN BOR BNT BXO ADD SUB MUL DIV REM NOT GTR LES GEQ LEQ EQL NEQ LAN LOR
%token VAL_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN REM_ASSIGN BAN_ASSIGN BOR_ASSIGN BXO_ASSIGN SHR_ASSIGN SHL_ASSIGN INC_ASSIGN DEC_ASSIGN
%token IF ELSE FOR WHILE RETURN BREAK CONTINUE

/* Token with return, which need to sepcify type */
%token <var_type> VARIABLE_T
%token <s_var> IDENT
%token <object_val> INT_LIT
%token <s_car> STR_LIT
%token <object_val> BOOL_LIT
%token <object_val> FLOAT_LIT

/* Nonterminal with return, which need to sepcify type */
%type <object_val> Expression
%type <s_var> FunctionParameterStmtList
%type <object_val> PrimaryExpr
%type <object_val> UnaryExpr
%type <object_val> Literal
%type <object_val> Operand
%type <op> add_op mul_op unary_op

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
        Insert_Symbol ( $2, OBJECT_TYPE_FUNCTION, "([Ljava/lang/String;)I", yylineno );
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

        Insert_Symbol ( $<s_var>4, $3, "", yylineno - 1 );
    }
    | VARIABLE_T IDENT '[' ']' {
        $$ = ( char * ) malloc ( sizeof ( char ) * 1024 );
        $$[0] = '(';
        $$[1] = get_type ( $1 );
        $$[2] = '\0';
        Insert_Symbol ( $<s_var>2, $1, "", yylineno - 1 );

    }
    | VARIABLE_T IDENT {
        $$ = ( char * ) malloc ( sizeof ( char ) * 1024 );
        $$[0] = '(';
        $$[1] = get_type ( $1 );
        $$[2] = '\0';

        Insert_Symbol ( $<s_var>2, $1, "", yylineno - 1 );
    }
;

/* Scope */
StmtList 
    : Stmt StmtList
    | Stmt
;

Stmt
    : COUT { Reset_treap(); } CoutParmListStmt ';' {
        if ( op_idx ) printf ( "\then %d\n", op_idx );
        printf ( "cout" );
        Print_List();
    }
    | RETURN Expression ';' { printf("RETURN\n"); }
    | ';'
;

CoutParmListStmt
    : SHL Expression {
        if ( is_bool ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_BOOL ) );
            is_bool = false;
        }
        else if ( is_float ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_FLOAT ) );
            is_float = false;
        }
        else
            Insert_Cout ( get_type_name ( $2.type ) );
    } CoutParmListStmt
    | SHL Expression {
        if ( is_bool ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_BOOL ) );
            is_bool = false;
        }
        else if ( is_float ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_FLOAT ) );
            is_float = false;
        }
        else
            Insert_Cout ( get_type_name ( $2.type ) );
    }
;

Expression
    : UnaryExpr { $$.type = $1.type; }
    | Expression binary_op {
        // printf ( "\t%s\n", get_op_name ( $<op>2 ) );
        if ( $<op>2 == OP_LOR || $<op>2 == OP_LAN ) {
            while ( op_idx > 0 && get_op_priority ( $<op>2 ) < get_op_priority ( ops[op_idx] ) ) {
                if ( ops[op_idx] == OP_LBRA ) {
                    op_idx--;
                    continue;
                }
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        else {
            while ( op_idx > 0 && get_op_priority ( $<op>2 ) <= get_op_priority ( ops[op_idx] ) ) {
                if ( ops[op_idx] == OP_LBRA ) {
                    op_idx--;
                    continue;
                }
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        ops[++op_idx] = $<op>2;
    } Expression {
        while ( op_idx > 0 ) {
            if ( ops[op_idx] == OP_LBRA ) {
                op_idx--;
                continue;
            }
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }

        if ( $1.type == OBJECT_TYPE_BOOL || $4.type == OBJECT_TYPE_BOOL )
            $$.type = OBJECT_TYPE_BOOL, is_bool = true, is_float = false;
        else if ( $1.type == OBJECT_TYPE_FLOAT || $4.type == OBJECT_TYPE_FLOAT )
            $$.type = OBJECT_TYPE_FLOAT, is_float = true;
    }
;

UnaryExpr
    : PrimaryExpr { $$.type = $1.type; }
    | unary_op UnaryExpr {
        $$.type = $2.type;
        if ( $$.type == OBJECT_TYPE_INT )
			$$.i_var = ( $<op>1 == OP_POS ? $<object_val>2.i_var : -$<object_val>2.i_var );
        else if ( $$.type == OBJECT_TYPE_FLOAT )
			$$.f_var = ( $<op>1 == OP_POS ? $<object_val>2.f_var : -$<object_val>2.f_var );
        printf ( "%s\n", get_op_name ( $<op>1 ) );
    }
;

PrimaryExpr
    : Operand
;

Operand
    : Literal
    | IDENT {
        if ( !strcmp ( "endl", $<s_var>1 ) ) {
            $$.type = OBJECT_TYPE_STR;
            puts ( "IDENT (name=endl, address=-1)" );
        }
        else {
            Node *tmp = Query_Symbol ( $<s_var>1 );
            if ( tmp ) {
                $$.type = tmp -> type;
                printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
            }
        }
    }
    | '(' {
        ops[++op_idx] = OP_LBRA;
    } Expression ')' {
        while ( op_idx ) {
            if ( ops[op_idx] == OP_LBRA ) {
                op_idx--;
                break;
            }
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }
    }
;

Literal
    : INT_LIT {
        $$.type = OBJECT_TYPE_INT;
        $$.i_var = $<i_var>1;
        printf ( "INT_LIT %d\n", $$.i_var );
    }
    | FLOAT_LIT {
        $$.type = OBJECT_TYPE_FLOAT;
        $$.f_var = $<f_var>1;
        printf ( "FLOAT_LIT %f\n", $$.f_var );
    }
    | BOOL_LIT {
        $$.type = OBJECT_TYPE_BOOL;
        $$.b_var = $<b_var>1;
        printf ( "BOOL_LIT %s\n", $$.b_var ? "TRUE" : "FALSE" );
    }
    | STR_LIT {
        $$.type = OBJECT_TYPE_STR;
        $$.s_var = $<s_var>1;
        printf ( "STR_LIT \"%s\"\n", $$.s_var );
    }
;

binary_op
    : LOR {
        // while ( op_idx > 0 && ops[op_idx] != OP_LOR && ops[op_idx] != OP_LAN )
			// printf ( "5: %s\n", get_op_name(ops[op_idx--] ) );
        $<op>$ = OP_LOR;
    }
    | LAN {
        // while ( op_idx > 0 && ops[op_idx] != OP_LOR && ops[op_idx] != OP_LAN )
			// printf ( "6: %s\n", get_op_name ( ops[op_idx--] ) );
        $<op>$ = OP_LAN;
    }
    | cmp_op
    | add_op
    | mul_op
;

cmp_op
    : EQL { $<op>$ = OP_EQL; }
    | NEQ { $<op>$ = OP_NEQ; }
    | LES { $<op>$ = OP_LES; }
    | LEQ { $<op>$ = OP_LEQ; }
    | GTR { $<op>$ = OP_GTR; }
    | GEQ { $<op>$ = OP_GEQ; }
;

add_op
    : ADD   { $<op>$ = OP_ADD; }
    | SUB   { $<op>$ = OP_SUB; }
	| BAN   { $<op>$ = OP_BAN; }
	| BOR   { $<op>$ = OP_BOR; }
;

mul_op
    : MUL   { $<op>$ = OP_MUL; }
    | DIV   { $<op>$ = OP_DIV; }
    | REM   { $<op>$ = OP_REM; }
	| SHR   { $<op>$ = OP_RSHIFT; }
;

unary_op
    : ADD   { $<op>$ = OP_POS; }
    | SUB   { $<op>$ = OP_NEG; }
    | NOT   { $<op>$ = OP_NOT; }
;
%%
/* C code section */
