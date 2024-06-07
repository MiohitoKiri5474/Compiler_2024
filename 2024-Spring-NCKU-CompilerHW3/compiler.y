/* Definition section */
%{
    #include "compiler_util.h"
    #include "main.h"

    int yydebug = 1;

    int op_idx = 0, arr_len = 0, lb_idx = 0, bf_cnt = 0;
    bool is_bool = false, is_float = false, is_str = false;
    bool is_cast = false, is_declare = false, is_auto = false, in_if_condition = false;
    bool if_flag = false, couting = false, first_argument;
    bool is_return = false, is_assignment = false;
    ObjectType cast_type, declare_type;
    op_t ops[1024];

    char buffer[128];
    char assign[128];
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
%token VAL_ASSIGN ADD_ASSIGN SUB_ASSIGN MUL_ASSIGN DIV_ASSIGN REM_ASSIGN
%token BAN_ASSIGN BOR_ASSIGN BXO_ASSIGN SHR_ASSIGN SHL_ASSIGN INC_ASSIGN DEC_ASSIGN
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
%type <op> add_op mul_op unary_op assign_op

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
        	codeRaw ( ".method public static main([Ljava/lang/String;)V" );
        else
			code ( ".method public statis %s()V\n", $2 );
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
        code ( "invokevirtual java/io/PrintStream/print(%s)V\n", get_print_type ( $2.type ));
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
        code ( "invokevirtual java/io/PrintStream/print(%s)V\n", get_print_type ( $2.type ));
    }
;

Expression
    : UnaryExpr {
        $$.type = $1.type;
    }
    | Expression binary_op {
        if ( ( $<op>2 == OP_LOR || $<op>2 == OP_LAN ) && !if_flag ) {
            while ( op_idx && get_op_priority ( $<op>2 ) < get_op_priority ( ops[op_idx] ) ) {
                get_op_inst ( buffer, $<object_val>1.type, ops[op_idx] );
                if ( ops[op_idx] == OP_LBRA ) {
                    code ( "%s", buffer );
                    op_idx--;
                    break;
                }
                if ( $1.type == OBJECT_TYPE_FLOAT )
                    code ( "%s", buffer );
                else if ( ops[op_idx] == OP_EQL || ops[op_idx] == OP_NEQ ||
                          ops[op_idx] == OP_LES || ops[op_idx] == OP_LEQ ||
                          ops[op_idx] == OP_GTR || ops[op_idx] == OP_GEQ ) {
                    code("%s L_cmp_%d", buffer, bf_cnt++);
                    codeRaw("ldc 1");
                    code("goto L_cmp_%d", bf_cnt++);
                    ScopeMinusOne();
                    code("L_cmp_%d:", bf_cnt - 2);
                    ScopeAddOne();
                    codeRaw("ldc 0");
                    ScopeMinusOne();
                    code("L_cmp_%d:", bf_cnt - 1);
                    ScopeAddOne();

                }
                else
                    code ( "%s", buffer );
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        else {
            while ( op_idx && get_op_priority ( $<op>2 ) <= get_op_priority ( ops[op_idx] ) ) {
                get_op_inst ( buffer, $<object_val>1.type, ops[op_idx] );
                if ( $1.type == OBJECT_TYPE_FLOAT )
                    code ( "%s", buffer );
                else if ( ops[op_idx] == OP_EQL || ops[op_idx] == OP_NEQ ||
                          ops[op_idx] == OP_LES || ops[op_idx] == OP_LEQ ||
                          ops[op_idx] == OP_GTR || ops[op_idx] == OP_GEQ ) {
                    code("%s L_cmp_%d", buffer, bf_cnt++);
                    codeRaw("ldc 1");
                    code("goto L_cmp_%d", bf_cnt++);
                    ScopeMinusOne();
                    code("L_cmp_%d:", bf_cnt - 2);
                    ScopeAddOne();
                    codeRaw("ldc 0");
                    ScopeMinusOne();
                    code("L_cmp_%d:", bf_cnt - 1);
                    ScopeAddOne();

                }
                else
                    code ( "%s", buffer );

                if ( ops[op_idx] == OP_LBRA ) {
                    op_idx--;
                    break;
                }
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        ops[++op_idx] = $<op>2;
    } Expression {
        $$.type = $4.type;
        bool pass = false;
        while ( op_idx ) {
            if ( ops[op_idx] == OP_LBRA )
                break;
            if ( ops[op_idx] == OP_REM && ( $1.type == OBJECT_TYPE_FLOAT || $4.type == OBJECT_TYPE_FLOAT ) ) {
                // has error, skip
            }
            else if ( ( $1.type == OBJECT_TYPE_INT || $1.type == OBJECT_TYPE_FLOAT ) &&
                      ( $4.type == OBJECT_TYPE_INT || $4.type == OBJECT_TYPE_FLOAT ) &&
                        $1.type != $4.type ) {
                pass = true;
                $$.type = $1.type;
            }
            else if ( ( ops[op_idx] == OP_LAN || ops[op_idx] == OP_LOR ) &&
                      ( $1.type == OBJECT_TYPE_BOOL || $4.type != OBJECT_TYPE_BOOL ) ) {
                // has error, skip
            }

            if ( ops[op_idx] != OP_VAL_ASSIGN ) {
                get_op_inst ( buffer, $$.type, ops[op_idx] );
                if ( ops[op_idx] == OP_EQL || ops[op_idx] == OP_NEQ ||
                     ops[op_idx] == OP_LES || ops[op_idx] == OP_LEQ ||
                     ops[op_idx] == OP_GTR || ops[op_idx] == OP_GEQ ) {
                    if ( !get_c_exp() )
                        code ( "%s Label_%d", buffer, lb_idx );
                }
            }
            else {
                Node *tmp = Query_Symbol ( assign );
                code ( "%s %d", get_ls_name ( $1.type, 1 ), tmp -> addr );
            }
            if ( !pass ) {
                if ( ops[op_idx] == OP_EQL || ops[op_idx] == OP_NEQ ||
                     ops[op_idx] == OP_LES || ops[op_idx] == OP_LEQ ||
                     ops[op_idx] == OP_GTR || ops[op_idx] == OP_GEQ )
                    $$.type = OBJECT_TYPE_BOOL;

                if ( $1.type == OBJECT_TYPE_BOOL || $4.type == OBJECT_TYPE_BOOL ) {
                    switch ( ops[op_idx] ) {
                    case OP_LES:
                    case OP_LEQ:
                    case OP_GEQ:
                    case OP_GTR:
                    case OP_NEQ:
                    case OP_EQL:
                    case OP_LOR:
                    case OP_LAN:
                        $$.type = OBJECT_TYPE_BOOL;
                        break;
                    default:
                        $$.type = ( $1.type == OBJECT_TYPE_BOOL ? $4.type : $1.type );
                    }
                }
                else if ( $1.type == OBJECT_TYPE_FLOAT || $4.type == OBJECT_TYPE_FLOAT )
                    $$.type = OBJECT_TYPE_FLOAT;
                else
                    $$.type = OBJECT_TYPE_INT;
            }
            if ( !in_if_condition ) {
                get_op_inst ( buffer, $$.type, ops[op_idx] );
                if ( $1.type == OBJECT_TYPE_FLOAT )
                    code ( "%s", buffer );
                else if ( ops[op_idx] == OP_EQL || ops[op_idx] == OP_NEQ ||
                            ops[op_idx] == OP_LES || ops[op_idx] == OP_LEQ ||
                            ops[op_idx] == OP_GTR || ops[op_idx] == OP_GEQ ) {
                    code("%s L_cmp_%d", buffer, bf_cnt++);
                    codeRaw("ldc 1");
                    code("goto L_cmp_%d", bf_cnt++);
                    ScopeMinusOne();
                    code("L_cmp_%d:", bf_cnt - 2);
                    ScopeAddOne();
                    codeRaw("ldc 0");
                    ScopeMinusOne();
                    code("L_cmp_%d:", bf_cnt - 1);
                    ScopeAddOne();

                }
                else
                    code ( "%s", buffer );
            }
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }

        if ( couting ) {
            if ( $1.type == OBJECT_TYPE_BOOL || $4.type == OBJECT_TYPE_BOOL )
                $$.type = OBJECT_TYPE_BOOL, is_bool = true, is_float = false;
            else if ( $1.type == OBJECT_TYPE_FLOAT || $4.type == OBJECT_TYPE_FLOAT )
                $$.type = OBJECT_TYPE_FLOAT, is_float = true;
        }
        if ( $$.type == OBJECT_TYPE_UNDEFINED )
            $$.type = OBJECT_TYPE_INT;
    }
    | '(' VARIABLE_T {
        is_cast = true;
        cast_type = $<var_type>2;
    } ')' UnaryExpr {
        $<s_var>$ = $<s_var>5;
        $$.type = $<var_type>2;
        is_float = is_str = is_bool = false;
        if ( cast_type == OBJECT_TYPE_INT && $5.type == OBJECT_TYPE_FLOAT )
            codeRaw ( "f2i" );
        else if ( cast_type == OBJECT_TYPE_FLOAT && $5.type == OBJECT_TYPE_INT )
            codeRaw ( "i2f" );
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
        get_op_inst ( buffer, $<object_val>2.type, $<op>1 );
        code ( "%s", buffer );
        printf ( "%s\n", get_op_name ( $<op>1 ) );
    }
;

PrimaryExpr
    : Operand {
        if ( is_cast ) {
            printf ( "Cast to %s\n", get_type_name ( cast_type ) );
            is_cast = false;
        }
        $$.type = $1.type;
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
                $$.name = malloc ( sizeof ( char ) * strlen ( tmp -> name ) );
                strcpy ( $$.name, tmp -> name );
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
                if ( !is_assignment ) {
                    code ( "%s %d", get_ls_name ( $$.type, 0 ), tmp -> addr );
                    is_assignment = false;
                }
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
            get_op_inst ( buffer, $<object_val>1.type, ops[op_idx] );
            code ( "%s", buffer );
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }
        $$.type = $3.type;
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
        Node *tmp = Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
        if ( declare_type == OBJECT_TYPE_INT )
            codeRaw ( "ldc 0" );
        else if ( declare_type == OBJECT_TYPE_FLOAT || declare_type == OBJECT_TYPE_DOUBLE )
            codeRaw ( "ldc 0.0" );
        else if ( declare_type == OBJECT_TYPE_STR )
            codeRaw ( "ldc \"\"" );
        code ( "%s %d", get_ls_name ( declare_type, 1 ), tmp -> addr );
    }
    | IDENT VAL_ASSIGN Expression {
        Node *tmp = Insert_Symbol ( $<s_var>1, ( is_auto ? $3.type : declare_type ), "", yylineno );
        code ( "%s %d", get_ls_name ( declare_type, 1 ), tmp -> addr );
    }
    | IDENT D2Array {
        // TODO Array
        Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
    }
    | IDENT D1Array { arr_len = 0; } VAL_ASSIGN '{' ListOfArray '}' {
        // TODO Array
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
        strcpy ( assign, $1.name );
        Node *tmp = Query_Symbol ( $1.name );
        while ( op_idx ) {
            printf ( "%s\n", get_op_name ( ops[op_idx] ) );

            if ( $2 != OP_VAL_ASSIGN ) {
                get_op_inst ( buffer, $1.type, ops[op_idx] );
                code ( "%s", buffer );
            }
            if ( $1.type == OBJECT_TYPE_FLOAT && $3.type == OBJECT_TYPE_INT )
                codeRaw ( "i2f" );
            else if ( $1.type == OBJECT_TYPE_INT && $3.type == OBJECT_TYPE_FLOAT )
                codeRaw ( "f2i" );
            code ( "%s %d", get_ls_name ( $1.type, 1 ), tmp -> addr );

            op_idx--;
        }

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
    : { if_flag = true; } IF '(' {
        in_if_condition = true;
    } Condition {
        in_if_condition = false;
    } ')' { puts ( "IF" ); if_flag = false; }
;

Condition
    : Expression {
        code ( "goto Exit_%d", lb_idx );
    }
;

Block
    : '{' {
        Create_Table();
        ScopeMinusOne();
        code ( "Label_%d:", lb_idx );
        ScopeAddOne();
    } StmtList '}' {
        ScopeMinusOne();
        code ( "Exit_%d:", lb_idx++ );
        ScopeAddOne();
        Dump_Table();
    }
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
        while ( op_idx ) {
            get_op_inst ( buffer, $<object_val>1.type, ops[op_idx] );
            code ( "%s", buffer );
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }
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
