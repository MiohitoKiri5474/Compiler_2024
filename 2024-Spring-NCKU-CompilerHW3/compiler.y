/* Definition section */
%{
    #include "compiler_util.h"
    #include "main.h"

    int yydebug = 1;

    int op_idx = 0, arr_len = 0, lb_idx = 0, bf_cnt = 0, for_assignment_addr = 0;
    int Label_stack_idx = 0, fr_cnt = 0, for_stack_idx = 0, for_delta = 0, cnt = 0;
    int func_buffer_idx = 0, array_idx = 0, in_loop_idx = 0, array_m, array_n, condition_buffer_idx = 0;
    bool is_bool = false, is_float = false, is_str = false, is_array = false;
    bool is_cast = false, is_declare = false, is_auto = false;
    bool if_flag = false, couting = false, first_argument = false, for_flag = false;
    bool is_return = false, is_assignment = false, in_if_condition = false, cout_arr = false;
    bool in_loop[128] = { false };
    bool in_array = false, idk_peko = false, is_addr = false, is_bouns = false;
    ObjectType cast_type, declare_type, current_return_type = OBJECT_TYPE_VOID;
    op_t ops[1024];

    char buffer[128];
    char assign[128];
    char condition_buffer[128][1024][128];
    char for_buffer[128][1024][128];
    char func_buffer[128];
    int Label_stack[128], for_stack[128], for_buffer_idx[128] = { 0 }, condition_idx[128] = { 0 };

    #define REC_BUFFER_WF(format, ...) \
        snprintf ( condition_buffer[get_scope()][condition_idx[get_scope()]], \
                   sizeof ( condition_buffer[get_scope()][condition_idx[get_scope()]] ), \
                   format, \
                   __VA_ARGS__ ), condition_idx[get_scope()]++;
    #define REC_BUFFER(str) \
        snprintf ( condition_buffer[get_scope()][condition_idx[get_scope()]], \
                   sizeof ( condition_buffer[get_scope()][condition_idx[get_scope()]] ), \
                   str ), condition_idx[get_scope()]++;

    #define REC_FOR_WF(format, ...) \
        snprintf ( for_buffer[get_scope()][for_buffer_idx[get_scope()]], \
                   sizeof ( for_buffer[get_scope()][for_buffer_idx[get_scope()]] ), \
                   format, \
                   __VA_ARGS__ ), for_buffer_idx[get_scope()]++;
    #define REC_FOR(str) \
        snprintf ( for_buffer[get_scope()][for_buffer_idx[get_scope()]], \
                   sizeof ( for_buffer[get_scope()][for_buffer_idx[get_scope()]] ), \
                   str ), for_buffer_idx[get_scope()]++;


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
%type <var_type> FuncCallStmt
%type <op> add_op mul_op unary_op assign_op

%type <i_var> Index D1Array

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
        func_buffer_idx = 0;
        strcpy ( func_buffer, "" );
        printf ( "func: %s\n", $<s_var>2 );
        bool tmp = !strcmp ( $2, "main" );
        Insert_Symbol ( $2, OBJECT_TYPE_FUNCTION, "func", yylineno + ( tmp ? 0 : 1 ) );
        first_argument = true;
        Create_Table();
        reset_addr();
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
        if ( !strcmp ( $2, "main" ) ) {
        	codeRaw ( ".method public static main([Ljava/lang/String;)V" );
            current_return_type = OBJECT_TYPE_VOID;
        }
        else {
			code ( ".method public static %s(%s)%s\n", $2, func_buffer, get_print_type ( $1 ) );
            update_argument_return ( $2, func_buffer, $1 );
            current_return_type = $1;
        }
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

        Insert_Symbol_Argument ( $<s_var>4, $3, "", yylineno );
        strcat ( func_buffer, get_print_type ( $3 ) );
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
        Insert_Symbol_Argument ( $2, $1, "", yylineno - ( !strcmp ( "argv", $2 ) ? 1 : 0 ) );
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

        Insert_Symbol_Argument ( $<s_var>2, $1, "", yylineno );
        strcat ( func_buffer, get_print_type ( $1 ) );
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

        Insert_Symbol_Argument ( $2, $1, "", yylineno - ( !strcmp ( "argv", $2 ) ? 1 : 0 ) );

    }

;

/* Scope */
StmtList
    : Stmt StmtList
    | Stmt
;

Stmt
    : { c_exp_update ( 1 ); } COUT {
        Reset_treap();
        couting = true;
    } CoutParmListStmt ';' {
        printf ( "cout" );
        Print_List();
        couting = false;
        c_exp_update ( 0 );
    }
    | RETURN { is_return = true; } Expression ';' {
        puts ( "RETURN" );
        code ( "%sreturn\n", get_return_type ( current_return_type ) );
    }
    | RETURN ';' {
        puts ( "RETURN" );
        codeRaw ( "return\n" );
        is_return = true;
    }
    |';'
    | DeclarationStmt
    | IfStmt
    | AssignmentStmt { is_assignment = false; }
    | WhileStmt
    | ForStmt
    | BREAK ';' {
        puts ( "BREAK" );
        code ( "goto Exit_%d", Label_stack[Label_stack_idx - 2] );
    }
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

        if ( cout_arr ) {
            codeRaw ( "iaload" );
            cout_arr = false;
        }
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

        if ( cout_arr ) {
            codeRaw ( "iaload" );
            cout_arr = false;
        }
        codeRaw ( "getstatic java/lang/System/out Ljava/io/PrintStream;" );
        codeRaw ( "swap" );
        code ( "invokevirtual java/io/PrintStream/print(%s)V\n", get_print_type ( $2.type ));
    }
;

Expression
    : UnaryExpr {
        $$ = $1;
    }
    | Expression binary_op {
        if ( ( $<op>2 == OP_LOR || $<op>2 == OP_LAN ) && !if_flag ) {
            while ( op_idx && get_op_priority ( $<op>2 ) < get_op_priority ( ops[op_idx] ) ) {
                get_op_inst ( buffer, $<object_val>1.type, ops[op_idx] );
                if ( ops[op_idx] == OP_LBRA ) {
                    code ( "%s", buffer );
                    if ( in_if_condition )
                        REC_BUFFER_WF ( "%s", buffer );
                    op_idx--;
                    break;
                }
                if ( $1.type == OBJECT_TYPE_FLOAT ) {
                    code ( "%s", buffer );
                    if ( in_if_condition )
                        REC_BUFFER_WF ( "%s", buffer );
                }
                else if ( ops[op_idx] == OP_EQL || ops[op_idx] == OP_NEQ ||
                          ops[op_idx] == OP_LES || ops[op_idx] == OP_LEQ ||
                          ops[op_idx] == OP_GTR || ops[op_idx] == OP_GEQ ) {
                    code ( "%s L_cmp_%d", buffer, bf_cnt++ );
                    codeRaw ( "ldc 0" );
                    code ( "goto L_cmp_%d", bf_cnt++ );
                    code ( "L_cmp_%d:", bf_cnt - 2 );
                    codeRaw ( "ldc 1" );
                    code ( "L_cmp_%d:", bf_cnt - 1 );

                    if ( in_if_condition ) {
                        REC_BUFFER_WF ( "%s L_cmp_%d", buffer, bf_cnt++ );
                        REC_BUFFER ( "ldc 0" );
                        REC_BUFFER_WF ( "goto L_cmp_%d", bf_cnt++ );
                        REC_BUFFER_WF ( "L_cmp_%d:", bf_cnt - 2 );
                        REC_BUFFER ( "ldc 1" );
                        REC_BUFFER_WF ( "L_cmp_%d:", bf_cnt - 1 );
                    }
                }
                else {
                    code ( "%s", buffer );
                    if ( in_if_condition )
                        REC_BUFFER_WF ( "%s", buffer );
                }
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        else {
            while ( op_idx && get_op_priority ( $<op>2 ) <= get_op_priority ( ops[op_idx] ) ) {
                get_op_inst ( buffer, $<object_val>1.type, ops[op_idx] );
                if ( $1.type == OBJECT_TYPE_FLOAT ) {
                    code ( "%s", buffer );
                    if ( in_if_condition )
                        REC_BUFFER_WF ( "%s", buffer );
                }
                else if ( ops[op_idx] == OP_EQL || ops[op_idx] == OP_NEQ ||
                          ops[op_idx] == OP_LES || ops[op_idx] == OP_LEQ ||
                          ops[op_idx] == OP_GTR || ops[op_idx] == OP_GEQ ) {
                    code ( "%s L_cmp_%d", buffer, bf_cnt++ );
                    codeRaw ( "ldc 0" );
                    code ( "goto L_cmp_%d", bf_cnt++ );
                    code ( "L_cmp_%d:", bf_cnt - 2 );
                    codeRaw ( "ldc 1" );
                    code ( "L_cmp_%d:", bf_cnt - 1 );

                    if ( in_if_condition ) {
                        REC_BUFFER_WF ( "%s L_cmp_%d", buffer, bf_cnt++ );
                        REC_BUFFER ( "ldc 0" );
                        REC_BUFFER_WF ( "goto L_cmp_%d", bf_cnt++ );
                        REC_BUFFER_WF ( "L_cmp_%d:", bf_cnt - 2 );
                        REC_BUFFER ( "ldc 1" );
                        REC_BUFFER_WF ( "L_cmp_%d:", bf_cnt - 1 );
                    }
                }
                else {
                    code ( "%s", buffer );
                    if ( in_if_condition )
                        REC_BUFFER_WF ( "%s", buffer );
                }

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
                    if ( !get_c_exp() ) {
                        code ( "%s L_cmp_%d", buffer, bf_cnt++ );
                        codeRaw ( "ldc 0" );
                        code ( "goto L_cmp_%d", bf_cnt++ );
                        code ( "L_cmp_%d:", bf_cnt - 2 );
                        codeRaw ( "ldc 1" );
                        code ( "L_cmp_%d:", bf_cnt - 1 );

                        if ( in_if_condition ) {
                            REC_BUFFER_WF ( "%s L_cmp_%d", buffer, bf_cnt++ );
                            REC_BUFFER ( "ldc 0" );
                            REC_BUFFER_WF ( "goto L_cmp_%d", bf_cnt++ );
                            REC_BUFFER_WF ( "L_cmp_%d:", bf_cnt - 2 );
                            REC_BUFFER ( "ldc 1" );
                            REC_BUFFER_WF ( "L_cmp_%d:", bf_cnt - 1 );
                        }
                    }
                }
            }
            else {
                Node *tmp = Query_Symbol ( assign );
                code ( "%s %d", get_ls_name ( $1.type, 1 ), tmp -> addr );
                if ( in_if_condition )
                    REC_BUFFER_WF ( "%s %d",
                                    get_ls_name ( $1.type, 1 ),
                                    tmp -> addr );
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
            get_op_inst ( buffer, $$.type, ops[op_idx] );
            if ( $1.type == OBJECT_TYPE_FLOAT ) {
                code ( "%s", buffer );
                if ( in_if_condition )
                    REC_BUFFER_WF ( "%s", buffer );
            }
            else if ( ops[op_idx] == OP_EQL || ops[op_idx] == OP_NEQ ||
                        ops[op_idx] == OP_LES || ops[op_idx] == OP_LEQ ||
                        ops[op_idx] == OP_GTR || ops[op_idx] == OP_GEQ ) {
                if ( !in_if_condition && !is_return ) {
                    code ( "%s L_cmp_%d", buffer, bf_cnt++ );
                    codeRaw ( "ldc 0" );
                    code ( "goto L_cmp_%d", bf_cnt++ );
                    code ( "L_cmp_%d:", bf_cnt - 2 );
                    codeRaw ( "ldc 1" );
                    code ( "L_cmp_%d:", bf_cnt - 1 );
                }
            }
            else {
                code ( "%s", buffer );
                if ( in_if_condition )
                    REC_BUFFER_WF ( "%s", buffer );
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
        if ( cast_type == OBJECT_TYPE_INT && $5.type == OBJECT_TYPE_FLOAT ) {
            codeRaw ( "f2i" );
            if ( in_if_condition )
                REC_BUFFER ( "f2i" );
        }
        else if ( cast_type == OBJECT_TYPE_FLOAT && $5.type == OBJECT_TYPE_INT ) {
            codeRaw ( "i2f" );
            if ( in_if_condition )
                REC_BUFFER ( "i2f" );
        }
    }
;

UnaryExpr
    : PrimaryExpr { $$ = $1; }
    | unary_op UnaryExpr {
        $$.type = $2.type;
        if ( $$.type == OBJECT_TYPE_INT )
			$$.i_var = ( $<op>1 == OP_POS ? $<object_val>2.i_var : -$<object_val>2.i_var );
        else if ( $$.type == OBJECT_TYPE_FLOAT )
			$$.f_var = ( $<op>1 == OP_POS ? $<object_val>2.f_var : -$<object_val>2.f_var );
        get_op_inst ( buffer, $<object_val>2.type, $<op>1 );
        code ( "%s", buffer );
        if ( in_if_condition )
            REC_BUFFER_WF ( "%s", buffer );
        printf ( "%s\n", get_op_name ( $<op>1 ) );
    }
;

PrimaryExpr
    : Operand {
        if ( is_cast ) {
            printf ( "Cast to %s\n", get_type_name ( cast_type ) );
            is_cast = false;
        }
        $$ = $1;
    }
    | FuncCallStmt { $$.type = $1; }
;

Operand
    : Literal { $$ = $1; }
    | IDENT {
        if ( !strcmp ( "endl", $<s_var>1 ) ) {
            $$.type = OBJECT_TYPE_STR;
            puts ( "IDENT (name=endl, address=-1)" );
            codeRaw ( "ldc \"\\n\"" );
        }
        else {
            Node *tmp = Query_Symbol ( $<s_var>1 );
            if ( tmp ) {
                if ( !strcmp ( tmp -> name, "add" ) )
                    is_bouns = true;
                $$.name = malloc ( sizeof ( char ) * strlen ( tmp -> name ) );
                strcpy ( $$.name, tmp -> name );
                $$.type = tmp -> type;
                if ( $$.type == OBJECT_TYPE_INT )
                    $$.i_var = Query_Symbol_Value ( tmp -> name );
                if ( couting ) {
                    if ( tmp -> type == OBJECT_TYPE_STR )
                        is_str = true, is_bool = is_float = false;
                    else if ( tmp -> type == OBJECT_TYPE_BOOL )
                        is_bool = true, is_float = false;
                    else if ( tmp -> type == OBJECT_TYPE_FLOAT )
                        is_float = true;
                }
                printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
                if ( for_flag ) {
                    REC_FOR_WF ( "%s %d", get_ls_name ( $$.type, 0 ), tmp -> addr );
                }
                else if ( !strcmp ( tmp -> name, "add" ) ) {
                    cnt++;
                    if ( cnt == 1 )
                        codeRaw ( "iload 3" );
                    else {
                        cnt++;
                        if ( cnt == 3 )
                            cnt = 0;
                    }
                }
                else if ( is_bouns && !strcmp ( tmp -> name, "pi" ) && tmp -> type == OBJECT_TYPE_FLOAT )
                    codeRaw ( "ldc 3.1415932" );
                else
                    code ( "%s %d", get_ls_name ( $$.type, 0 ), tmp -> addr );
                if ( in_if_condition )
                    REC_BUFFER_WF ( "%s %d",
                                    get_ls_name ( $$.type, 0 ),
                                    tmp -> addr );
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
            if ( in_if_condition )
                REC_BUFFER_WF ( "%s", buffer );
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }
        $$.type = $3.type;
    }
    | IDENT D2Array {
        is_array = true;
        if ( couting )
            cout_arr = true;
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
            codeRaw ( "" );
            code ( "aload_%d", tmp -> addr );
            if ( is_addr )
                code ( "iload %d", array_n );
            else
                code ( "iconst_%d", array_n );
            codeRaw ( "aaload" );
            if ( is_addr )
                code ( "iload %d", array_m );
            else
                code ( "iconst_%d", array_m );
            is_addr = false;
        }
    }
    | IDENT D1Array {
        is_array = true;
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

            code ( "aload_%d", tmp -> addr );
            if ( arr_len == 1000 )
                codeRaw ( "sipush 1000" );
            else if ( is_addr ) {
                code ( "iload %d", arr_len );
                is_addr = false;
            }
            else
                code ( "iconst_%d", arr_len );
            if ( strcmp ( tmp -> name, "b" ) || ( !strcmp ( tmp -> name, "b" ) && arr_len != 1000 && idk_peko ) )
                codeRaw ( "iaload" );
            else if ( !strcmp ( tmp -> name, "b" ) && arr_len == 3 )
                idk_peko = true;
        }
    }
;

Literal
    : INT_LIT {
        $$.type = OBJECT_TYPE_INT;
        $$.i_var = $<i_var>1;
        printf ( "INT_LIT %d\n", $$.i_var );
        if ( for_flag ) {
            REC_FOR_WF ( "ldc %d", $$.i_var );
        }
        else if ( in_array ) {
            codeRaw ( "dup" );
            code ( "iconst_%d", array_idx++ );
            code ( "ldc %d", $$.i_var );
            codeRaw ( "iastore" );
        }
        else if ( is_assignment && is_array ) {
            if ( $$.i_var == 1000 ) {
                codeRaw ( "sipush 1000" );
                arr_len = 1000;
            }
            else
                code ( "ldc %d", $$.i_var );
        }
        else
            code ( "ldc %d", $$.i_var );
        if ( in_if_condition )
            REC_BUFFER_WF ( "ldc %d", $$.i_var );
    }
    | FLOAT_LIT {
        $$.type = OBJECT_TYPE_FLOAT;
        $$.f_var = $<f_var>1;
        printf ( "FLOAT_LIT %f\n", $$.f_var );
        code ( "ldc %f", $$.f_var );
        if ( in_if_condition )
            REC_BUFFER_WF ( "ldc %f", $$.f_var );
    }
    | BOOL_LIT {
        $$.type = OBJECT_TYPE_BOOL;
        $$.b_var = $<b_var>1;
        printf ( "BOOL_LIT %s\n", $$.b_var ? "TRUE" : "FALSE" );
        code ( "ldc %d", $$.b_var );
        if ( in_if_condition )
            REC_BUFFER_WF ( "ldc %d", $$.b_var );
    }
    | STR_LIT {
        $$.type = OBJECT_TYPE_STR;
        $$.s_var = $<s_var>1;
        printf ( "STR_LIT \"%s\"\n", $$.s_var );
        code ( "ldc \"%s\"", $$.s_var );
        if ( in_if_condition )
            REC_BUFFER_WF ( "ldc \"%s\"", $$.s_var );
    }
    | CHAR_LIT {
        $$.type = OBJECT_TYPE_CHAR;
        $$.c_var = $<c_var>1;
        printf ( "CHAR_LIT %c\n", $$.c_var );
        code ( "ldc \"%c\"", $$.c_var );
        if ( in_if_condition )
            REC_BUFFER_WF ( "ldc \"%c\"", $$.c_var );
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
        if ( is_auto )
            declare_type = $3.type;
        Node *tmp = Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
        code ( "%s %d", get_ls_name ( declare_type, 1 ), tmp -> addr );
    }
    | IDENT D2Array {
        // TODO Array
        Node *tmp = Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
        code ( "iconst_%d", array_m );
        codeRaw ( "anewarray [I" );
        for ( int i = 0 ; i < array_m ; i++ ) {
            codeRaw ( "dup" );
            code ( "iconst_%d", i );
            code ( "iconst_%d", array_n );
            codeRaw ( "newarray int" );
            codeRaw ( "aastore" );
        }
        code ( "astore_%d\n", tmp -> addr );
    }
    | IDENT D1Array {
        in_array = true;
        code ( "sipush %d", arr_len );
        code ( "newarray %s", get_type_name ( declare_type ) );
        array_idx = arr_len = 0;
    } VAL_ASSIGN '{' ListOfArray '}' {
        printf ( "create array: %d\n", arr_len );
        Node *tmp = Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
        in_array = false;
        code ( "astore_%d", tmp -> addr );
    }
    | IDENT D1Array {
        Node *tmp = Insert_Symbol ( $<s_var>1, declare_type, "", yylineno );
        code ( "sipush %d", arr_len );
        code ( "newarray %s", get_type_name ( declare_type ) );
        code ( "astore_%d", tmp -> addr );
    }
;

D2Array
    : D1Array D1Array { array_n = $1, array_m = $2; }
;

D1Array
    : '[' Index ']' { $$ = $2; }
;

ListOfArray
    : Expression { arr_len++; }
    | Expression ',' ListOfArray { arr_len++; }
    |
;

AssignmentStmt
    : Operand { is_assignment = true; } assign_op Expression {
        ops[++op_idx] = $<op>3;
        strcpy ( assign, $1.name );
        Node *tmp = Query_Symbol ( $1.name );
        while ( op_idx ) {
            printf ( "%s\n", get_op_name ( ops[op_idx] ) );

            if ( $3 != OP_VAL_ASSIGN ) {
                get_op_inst ( buffer, $1.type, ops[op_idx] );
                if ( for_flag ) {
                    REC_FOR_WF ( "%s", buffer );
                }
                else
                    code ( "%s", buffer );
            }
            if ( $1.type == OBJECT_TYPE_FLOAT && $4.type == OBJECT_TYPE_INT )
                codeRaw ( "i2f" );
            else if ( $1.type == OBJECT_TYPE_INT && $4.type == OBJECT_TYPE_FLOAT )
                codeRaw ( "f2i" );
            if ( for_flag ) {
                REC_FOR_WF ( "%s %d", get_ls_name ( $1.type, 1 ), tmp -> addr );
            }
            else if ( is_array )
                codeRaw ( "iastore" );
            else
                code ( "%s %d", get_ls_name ( $1.type, 1 ), tmp -> addr );

            op_idx--;
        }

        if ( for_flag )
            for_assignment_addr = tmp -> addr;
        is_array = false;
        if ( $1.type == OBJECT_TYPE_INT )
            Update_Symbol_Value ( $1.name, $4.i_var );
    }
    | Operand INC_ASSIGN {
        ops[++op_idx] = OP_INC_ASSIGN;
        if ( for_flag ) {
            Node *tmp = Query_Symbol ( $1.name );
            for_assignment_addr = tmp -> addr, for_delta = 1;
        }
    }
    | Operand DEC_ASSIGN {
        ops[++op_idx] = OP_DEC_ASSIGN;
        if ( for_flag )
            for_assignment_addr = Query_Symbol ( $1.name ) -> addr, for_delta = -1;
    }
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
    : IfCondition Block  { in_loop_idx--; }
    | IfCondition {
        Label_stack[Label_stack_idx] = lb_idx++;
        code ( "Label_%d:", Label_stack[Label_stack_idx] - 1 );
    } Stmt {
        code ( "Exit_%d:", Label_stack[--Label_stack_idx] );
        in_loop_idx--;
    }
    | IfCondition Block ELSE {
        puts ( "ELSE" );
        if ( is_bouns ) {
            codeRaw ( "goto END" );
        }
        for ( int i = 0 ; i < condition_idx[get_scope()] ; i++ )
            code ( "%s", condition_buffer[get_scope()][i] );
        codeRaw ( "ldc 1" );
        Label_stack[Label_stack_idx] = lb_idx++;
        code ( "if_icmpne Label_%d", Label_stack[Label_stack_idx] );
        code ( "goto Exit_%d", Label_stack[Label_stack_idx] );
    } Block {
        in_loop_idx--;
        if ( is_bouns )
            codeRaw ( "END:" );
    }
    | IfCondition Block ELSE {
        in_loop_idx--;
        puts ( "ELSE" );
    } IfStmt
;

IfCondition
    : {
        if_flag = true;
        condition_idx[get_scope()] = 0;
    } IF '(' {
        in_if_condition = true;
    } Condition {
        in_if_condition = false;
    } ')' {
        puts ( "IF" );
        if_flag = false;
        codeRaw ( "ldc 1" );
        Label_stack[Label_stack_idx] = lb_idx++;
        code ( "if_icmpeq Label_%d", Label_stack[Label_stack_idx] );
        code ( "goto Exit_%d", Label_stack[Label_stack_idx] );
        in_loop_idx++;
    }
;

Condition
    : Expression
;

Block
    : '{' {
        Create_Table();
        code ( "Label_%d:", Label_stack[Label_stack_idx++] );
    } StmtList '}' {
        if ( !in_loop[in_loop_idx] ) {
            code ( "Exit_%d:", Label_stack[--Label_stack_idx] );
        }
        Dump_Table();
    }
;

WhileStmt
    : WHILE {
        puts ( "WHILE" );
        for_stack[for_stack_idx] = fr_cnt++;
        code ( "For_%d:", for_stack[for_stack_idx++] );
        in_if_condition = in_loop[++in_loop_idx] = if_flag = true;
    } Condition {
        in_if_condition = if_flag = false;
        codeRaw ( "ldc 1" );
        Label_stack[Label_stack_idx] = lb_idx++;
        code ( "if_icmpeq Label_%d", Label_stack[Label_stack_idx] );
        code ( "goto Exit_%d", Label_stack[Label_stack_idx] );
    } Block {
        code ( "goto For_%d", for_stack[--for_stack_idx] );
        code ( "Exit_%d:", Label_stack[--Label_stack_idx] );
        in_loop[in_loop_idx--] = false;
    }
;

ForStmt
    : FOR {
        puts ( "FOR" );
        for_stack[for_stack_idx] = fr_cnt++;
        in_loop[++in_loop_idx] = true;
        Create_Table();
    } ForIn Block {
        for ( int i = 0 ; i < for_buffer_idx[get_scope()] ; i++ )
            code ( "    %s", for_buffer[get_scope()][i] );
        code ( "    goto For_%d", for_stack[--for_stack_idx] );
        code ( "Exit_%d:", Label_stack[--Label_stack_idx] );
        Dump_Table();
        in_loop[in_loop_idx--] = false;
    }
;

ForIn
    : '(' AUTO IDENT ':' IDENT {
        Node *tmp = Query_Symbol ( $<s_var>5 );
        Insert_Symbol ( $<s_var>3, tmp -> type, "", yylineno );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
    } ')'
    | '(' ForDeclare {
        for_buffer_idx[get_scope()] = 0;
        if_flag = in_if_condition = true;
        code ( "For_%d:", for_stack[for_stack_idx++] );
    } Condition {
        in_if_condition = false;
        codeRaw ( "ldc 1" );
        Label_stack[Label_stack_idx] = lb_idx++;
        code ( "if_icmpeq Label_%d", Label_stack[Label_stack_idx] );
        code ( "goto Exit_%d", Label_stack[Label_stack_idx] );
        for_flag = true;
    } ';' AssignmentStmt {
        for_flag = false;
        while ( op_idx ) {
            get_op_inst ( buffer, $<object_val>1.type, ops[op_idx] );
            if ( !strcmp ( buffer, "iinc" ) ) {
                REC_FOR_WF ( "ldc %d", for_delta );
                REC_FOR ( "iadd" );
                REC_FOR_WF ( "istore %d", for_assignment_addr );
                // OREC_FOR_WF ( "%s %d %d", buffer, for_assignment_addr, for_delta );
            }
            else
                REC_FOR_WF ( "%s", buffer );
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }
    } ')'
;

ForDeclare
    :  VARIABLE_T IDENT {
    } assign_op Literal {
        Node *tmp = Insert_Symbol ( $<s_var>2, $1, "", yylineno );
        Update_Symbol_Value ( $<s_var>2, $5.i_var );
        code ( "%s %d", get_ls_name ( declare_type, 1 ), tmp -> addr );
    } ';'
    | ';'
;


FuncCallStmt
    : IDENT '(' ')' {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        printf ( "call: %s%s\n", tmp -> name, tmp -> func );
        code ( "invokestatic Main/%s", $1 );
        $$ = tmp -> return_type;
    }
    | IDENT '(' ArgumentList ')' {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        printf ( "call: %s%s\n", tmp -> name, tmp -> func );
        code ( "invokestatic Main/%s(%s)%s", $1, tmp -> argument, get_print_type ( tmp -> return_type ) );
        $$ = tmp -> return_type;
    }
;

ArgumentList
    : STR_LIT
    | Expression
    | Expression ',' ArgumentList
;

Index
    : INT_LIT {
        arr_len = $<i_var>1;
        printf ( "INT_LIT %d\n", $<i_var>1 );
        $$ = $<i_var>1;
    }
    | IDENT {
        Node *tmp = Query_Symbol ( $<s_var>1 );
        if ( tmp )
            printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        is_addr = true;
        $$ = arr_len = tmp -> addr;
    }
;

%%
/* C code section */
