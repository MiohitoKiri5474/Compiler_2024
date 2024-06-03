/* Definition section */
%{
    #include "compiler_util.h"
    #include "main.h"

    int yydebug = 1;

    int op_idx = 0, arr_len = 0;
    bool is_bool = false, is_float = false, is_str = false;
    bool is_cast = false, is_declare = false, is_auto = false;
    bool if_flag = false, couting = false, first_argument;
    bool is_return = false;
    ObjectType cast_type, declare_type;
    op_t ops[1024];
%}

/* Variable or self-defined structure */
%union {
    ObjectType var_type;

    char *s_var;
    int i_var;
    float f_var;
    bool b_var;
    char c_var;

    op_t op;

    struct {
        union {
            char *s_var;
            bool b_var;
            int i_var;
            float f_var;
            char c_var;
        };

        char *name;
        ObjectType type;
    } object_val;


    Object obj_val;
}

/* Token without return */
%token COUT NEWLINE AUTO
%token SHR SHL BAN BOR BNT BXO ADD SUB MUL DIV REM NOT GTR LES GEQ LEQ EQL NEQ LAN LOR
%token VAL_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN REM_ASSIGN BAN_ASSIGN BOR_ASSIGN BXO_ASSIGN SHR_ASSIGN SHL_ASSIGN INC_ASSIGN DEC_ASSIGN
%token IF ELSE FOR WHILE RETURN BREAK CONTINUE

/* Token with return, which need to sepcify type */
%token <var_type> VARIABLE_T
%token <s_var> IDENT
%token <object_val> INT_LIT
%token <s_var> STR_LIT
%token <c_var> CHAR_LIT
%token <object_val> BOOL_LIT
%token <object_val> FLOAT_LIT

/* Nonterminal with return, which need to sepcify type */
%type <s_var> FunctionParameterStmtList
%type <object_val> Expression
%type <object_val> PrimaryExpr
%type <object_val> UnaryExpr
%type <object_val> Literal
%type <object_val> Operand
%type <op> add_op mul_op unary_op

%left ADD SUB
%left MUL DIV REM

%nonassoc THEN
%nonassoc ELSE

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
        bool tmp = !strcmp ( $2, "main" );
        Insert_Symbol ( $2, OBJECT_TYPE_FUNCTION, "func", yylineno + ( tmp ? 0 : 1 ) );
        first_argument = true;
        Create_Table();
    } '(' FunctionParameterStmtList ')' {
        char tmp[2];
        tmp[0] = get_type ( $1 ), tmp[1] = '\0';
        strcat ( $5, ")" );
        if ( tmp[0] == 'S' )
            strcat ( $5, "Ljava/lang/String;" );
        else
            strcat ( $5, tmp );
        Update_Symbol ( $2, $5 );

        ScopeMinusOne();
        if ( !strcmp ( $2, "main" ) )
        	codeRaw ( ".method public static main([Ljava/lang/String;)V\n" );
        else
			code ( ".method public statis %s()V\n", $1 );
        ScopeAddOne();
        codeRaw ( ".limit stack 128" );
        codeRaw ( ".limit locals 128\n" );
    } '{' StmtList '}' {
        if ( !is_return )
            codeRaw ( "return" );
        is_return = false;
        Dump_Table();
        codeRaw ( ".end method" );
    }
;

FunctionParameterStmtList
    : FunctionParameterStmtList ',' VARIABLE_T IDENT {
        char tmp[2];
        tmp[0] = get_type ( $3 ), tmp[1] = '\0';
        if ( first_argument ) {
            $$ = ( char * ) malloc ( sizeof ( char ) * 1024 );
            $$[0] = '(';
            first_argument = false;
        }
        if ( tmp[0] == 'S' )
            strcat ( $$, "Ljava/lang/String;" );
        else
            strcat ( $$, tmp );

        Insert_Symbol ( $<s_var>4, $3, "", yylineno );
    }
    | VARIABLE_T IDENT '[' ']' {
        char tmp[2];
        tmp[0] = get_type ( $1 );
        tmp[1] = '\0';
        if ( first_argument ) {
            $$ = ( char * ) malloc ( sizeof ( char ) * 1024 );
            $$[0] = '(';
            first_argument = false;
        }
        if ( tmp[0] == 'S' )
            strcat ( $$, "[Ljava/lang/String;" );
        else
            strcat ( $$, tmp );
        Insert_Symbol ( $2, $1, "", yylineno - ( !strcmp ( "argv", $2 ) ? 1 : 0 ) );

    }
    | VARIABLE_T IDENT {
        char tmp[2];
        tmp[0] = get_type ( $1 ), tmp[1] = '\0';
        if ( first_argument ) {
            $$ = ( char * ) malloc ( sizeof ( char ) * 1024 );
            $$[0] = '(';
            first_argument = false;
        }
        if ( tmp[0] == 'S' )
            strcat ( $$, "Ljava/lang/String;" );
        else
            strcat ( $$, tmp );

        Insert_Symbol ( $<s_var>2, $1, "", yylineno );
    }
    | VARIABLE_T IDENT '[' INT_LIT { printf ( "INT_LIT %d\n", $<i_var>4 ); } ']' {
        char tmp[2];
        tmp[0] = get_type ( $1 ), tmp[1] = '\0';
        if ( first_argument ) {
            $$ = ( char * ) malloc ( sizeof ( char ) * 1024 );
            $$[0] = '(';
            first_argument = false;
        }
        if ( tmp[0] == 'S' )
            strcat ( $$, "Ljava/lang/String;" );
        else
            strcat ( $$, tmp );

        Insert_Symbol ( $2, $1, "", yylineno - ( !strcmp ( "argv", $2 ) ? 1 : 0 ) );

    }

;

/* Scope */
StmtList
    : Stmt StmtList
    | Stmt
;

Stmt
    : { c_exp_update ( 1 ); }COUT { Reset_treap(); couting = true; } CoutParmListStmt ';' {
        printf ( "cout" );
        Print_List();
        couting = false;
        c_exp_update ( 0 );
    }
    | RETURN { is_return = true; } Expression ';' {
        puts ( "RETURN" );
        codeRaw ( "return\n" );
    }
    | RETURN ';' {
        puts ( "RETURN" );
        codeRaw ( "return\n" );
        is_return = true;
    }
    |';'
    | DeclarationStmt
    | IfStmt
    | AssignmentStmt
    | WhileStmt
    | ForStmt
    | BREAK ';' { puts ( "BREAK" ); }
    | FuncCallStmt
;

CoutParmListStmt
    : SHL Expression {
        if ( is_str ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_STR ) );
            is_str = false;
        }
        else if ( is_bool ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_BOOL ) );
            is_bool = false;
        }
        else if ( is_float ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_FLOAT ) );
            is_float = false;
        }
        else
            Insert_Cout ( get_type_name ( $2.type ) );
        codeRaw ( "getstatic java/lang/System/out Ljava/io/PrintStream;" );
        codeRaw ( "swap" );
        code ( "invokevirtual java/io/PrintStream/print(%s)V", get_print_type ( $2.type ));
    } CoutParmListStmt
    | SHL Expression {
        if ( is_str ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_STR ) );
            is_str = false;
        }
        else if ( is_bool ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_BOOL ) );
            is_bool = false;
        }
        else if ( is_float ) {
            Insert_Cout ( get_type_name ( OBJECT_TYPE_FLOAT ) );
            is_float = false;
        }
        else
            Insert_Cout ( get_type_name ( $2.type ) );
        codeRaw ( "getstatic java/lang/System/out Ljava/io/PrintStream;" );
        codeRaw ( "swap" );
        code ( "invokevirtual java/io/PrintStream/print(%s)V", get_print_type ( $2.type ));
    }
;

Expression
    : UnaryExpr { $$.type = $1.type; }
    | Expression binary_op {
        if ( ( $<op>2 == OP_LOR || $<op>2 == OP_LAN ) && !if_flag ) {
            while ( op_idx && get_op_priority ( $<op>2 ) < get_op_priority ( ops[op_idx] ) ) {
                if ( ops[op_idx] == OP_LBRA ) {
                    op_idx--;
                    break;
                }
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        else {
            while ( op_idx && get_op_priority ( $<op>2 ) <= get_op_priority ( ops[op_idx] ) ) {
                if ( ops[op_idx] == OP_LBRA ) {
                    op_idx--;
                    break;
                }
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        ops[++op_idx] = $<op>2;
    } Expression {
        while ( op_idx ) {
            if ( ops[op_idx] == OP_LBRA ) {
                break;
            }
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }

        if ( couting ) {
            if ( $1.type == OBJECT_TYPE_BOOL || $4.type == OBJECT_TYPE_BOOL )
                $$.type = OBJECT_TYPE_BOOL, is_bool = true, is_float = false;
            else if ( $1.type == OBJECT_TYPE_FLOAT || $4.type == OBJECT_TYPE_FLOAT )
                $$.type = OBJECT_TYPE_FLOAT, is_float = true;
        }
    }
    | '(' VARIABLE_T {
        is_cast = true;
        cast_type = $<var_type>2;
    } ')' Expression {
        $<s_var>$ = $<s_var>5;
        $$.type = $<var_type>2;
        is_float = is_str = is_bool = false;
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
    : Operand {
        if ( is_cast ) {
            printf ( "Cast to %s\n", get_type_name ( cast_type ) );
            is_cast = false;
        }
    }
    | FuncCallStmt
;

Operand
    : Literal
    | IDENT {
        if ( !strcmp ( "endl", $<s_var>1 ) ) {
            $$.type = OBJECT_TYPE_STR;
            puts ( "IDENT (name=endl, address=-1)" );
            codeRaw ( "ldc \"\\n\"" );
        }
        else {
            Node *tmp = Query_Symbol ( $<s_var>1 );
            if ( tmp ) {
                $$.type = tmp -> type;
                if ( couting ) {
                    if ( tmp -> type == OBJECT_TYPE_STR )
                        is_str = true, is_bool = is_float = false;
                    else if ( tmp -> type == OBJECT_TYPE_BOOL )
                        is_bool = true, is_float = false;
                    else if ( tmp -> type == OBJECT_TYPE_FLOAT )
                        is_float = true;
                }
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
    | IDENT D1Array {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        if ( tmp ) {
            $$.type = tmp -> type;
            if ( couting ) {
                if ( tmp -> type == OBJECT_TYPE_STR )
                    is_str = true, is_bool = is_float = false;
                else if ( tmp -> type == OBJECT_TYPE_BOOL )
                    is_bool = true, is_float = false;
                else if ( tmp -> type == OBJECT_TYPE_FLOAT )
                    is_float = true;
            }
            printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        }
    }
    | IDENT D2Array {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        if ( tmp ) {
            $$.type = tmp -> type;
            if ( couting ) {
                if ( tmp -> type == OBJECT_TYPE_STR )
                    is_str = true, is_bool = is_float = false;
                else if ( tmp -> type == OBJECT_TYPE_BOOL )
                    is_bool = true, is_float = false;
                else if ( tmp -> type == OBJECT_TYPE_FLOAT )
                    is_float = true;
            }
            printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        }
    }
;

Literal
    : INT_LIT {
        $$.type = OBJECT_TYPE_INT;
        $$.i_var = $<i_var>1;
        printf ( "INT_LIT %d\n", $$.i_var );
        if ( !is_return ) {
            code ( "ldc %d", $$.i_var );
        }
    }
    | FLOAT_LIT {
        $$.type = OBJECT_TYPE_FLOAT;
        $$.f_var = $<f_var>1;
        printf ( "FLOAT_LIT %f\n", $$.f_var );
        if ( !is_return ) {
            code ( "ldc %f", $$.f_var );
        }
    }
    | BOOL_LIT {
        $$.type = OBJECT_TYPE_BOOL;
        $$.b_var = $<b_var>1;
        printf ( "BOOL_LIT %s\n", $$.b_var ? "TRUE" : "FALSE" );
        if ( !is_return ) {
            code ( "ldc %d", $$.b_var );
        }
    }
    | STR_LIT {
        $$.type = OBJECT_TYPE_STR;
        $$.s_var = $<s_var>1;
        printf ( "STR_LIT \"%s\"\n", $$.s_var );
        if ( !is_return ) {
            code ( "ldc \"%s\"", $$.s_var );
        }
    }
    | CHAR_LIT {
        $$.type = OBJECT_TYPE_CHAR;
        $$.c_var = $<c_var>1;
        printf ( "CHAR_LIT %c\n", $$.c_var );
        if ( !is_return ) {
            code ( "ldc \"%c\"", $$.c_var );
        }
    }
;

binary_op
    : LOR { $<op>$ = OP_LOR; }
    | LAN { $<op>$ = OP_LAN; }
    | cmp_op
    | add_op
    | mul_op
;

cmp_op
    : EQL   { $<op>$ = OP_EQL; }
    | NEQ   { $<op>$ = OP_NEQ; }
    | LES   { $<op>$ = OP_LES; }
    | LEQ   { $<op>$ = OP_LEQ; }
    | GTR   { $<op>$ = OP_GTR; }
    | GEQ   { $<op>$ = OP_GEQ; }
;

add_op
    : ADD   { $<op>$ = OP_ADD; }
    | SUB   { $<op>$ = OP_SUB; }
	| BAN   { $<op>$ = OP_BAN; }
	| BOR   { $<op>$ = OP_BOR; }
    | BXO   { $<op>$ = OP_BXO; }
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
    | BNT   { $<op>$ = OP_BNT; }
;

DeclarationStmt
    : VARIABLE_T {
        is_declare = true;
        declare_type = $1;
        if ( $1 == OBJECT_TYPE_AUTO )
            is_auto = true;
    } DeclarationList ';' { is_auto = is_declare = false; }
    | AUTO {
        is_auto = true;
    } DeclarationList ';' { is_auto = is_declare = false; }
;

DeclarationList
    : DeclarationIDENT ',' DeclarationList
    | DeclarationIDENT
;

DeclarationIDENT
    : IDENT {
        Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
    }
    | IDENT VAL_ASSIGN Expression {
        Insert_Symbol ( $<s_var>1, ( is_auto ? $3.type : declare_type ), "", yylineno );
    }
    | IDENT D2Array {
        Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
    }
    | IDENT D1Array { arr_len = 0; } VAL_ASSIGN '{' ListOfArray '}' {
        printf ( "create array: %d\n", arr_len );
        Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
    }
;

D2Array
    : D1Array D1Array
;

D1Array
    : '[' Index ']'
;

ListOfArray
    : Expression { arr_len++; }
    | Expression ',' ListOfArray { arr_len++; }
    | 
;

AssignmentStmt
    : Operand assign_op Expression {
        ops[++op_idx] = $<op>2;
        while ( op_idx )
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
    }
    | Operand INC_ASSIGN { ops[++op_idx] = OP_INC_ASSIGN; }
    | Operand DEC_ASSIGN { ops[++op_idx] = OP_DEC_ASSIGN; }
;

assign_op
    : VAL_ASSIGN    { $<op>$ = OP_VAL_ASSIGN; }
    | ADD_ASSIGN    { $<op>$ = OP_ADD_ASSIGN; }
    | SUB_ASSIGN    { $<op>$ = OP_SUB_ASSIGN; }
    | MUL_ASSIGN    { $<op>$ = OP_MUL_ASSIGN; }
    | DIV_ASSIGN    { $<op>$ = OP_DIV_ASSIGN; }
    | REM_ASSIGN    { $<op>$ = OP_REM_ASSIGN; }
    | BOR_ASSIGN    { $<op>$ = OP_BOR_ASSIGN; }
    | BAN_ASSIGN    { $<op>$ = OP_BAN_ASSIGN; }
    | BXO_ASSIGN    { $<op>$ = OP_BXO_ASSIGN; }
    | SHR_ASSIGN    { $<op>$ = OP_SHR_ASSIGN; }
    | SHL_ASSIGN    { $<op>$ = OP_SHL_ASSIGN; }
;

IfStmt
    : IfCondition Block
    | IfCondition Stmt
    | IfCondition Block ELSE { puts ( "ELSE" ); } Block
    | IfCondition Block ELSE { puts ( "ELSE" ); } IfStmt
;

IfCondition
    : { if_flag = true; } IF '(' Condition ')' { puts ( "IF" ); if_flag = false; }
;

Condition
    : Expression
;

Block
    : '{' { Create_Table(); } StmtList '}' { Dump_Table(); }
;

WhileStmt
    : WHILE { if_flag = true; puts ( "WHILE" ); } Condition { if_flag = false; } Block
;

ForStmt
    : FOR {
        puts ( "FOR" );
        Create_Table();
    } ForIn '{' StmtList '}' { Dump_Table(); }
;

ForIn
    : '(' AUTO IDENT ':' IDENT {
        Node *tmp = Query_Symbol ( $<s_var>5 );
        Insert_Symbol ( $<s_var>3, tmp -> type, "", yylineno );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
    } ')'
    | '(' ForDeclare { if_flag = true; } Condition { if_flag = false; } ';' AssignmentStmt {
        while ( op_idx )
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
    } ')'
;

ForDeclare
    :  DeclarationStmt
    | ';'
;


FuncCallStmt
    : IDENT '(' ')' {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        printf ( "call: %s%s\n", tmp -> name, tmp -> func );
    }
    | IDENT '(' ArgumentList ')' {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        printf ( "call: %s%s\n", tmp -> name, tmp -> func );
    }
;

ArgumentList
    : STR_LIT
    | Expression
    | Expression ',' ArgumentList
;

Index
    : INT_LIT {
        printf ( "INT_LIT %d\n", $<i_var>1 );
    }
    | IDENT {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        if ( tmp )
            printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
    }
;

%%
/* C code section */
