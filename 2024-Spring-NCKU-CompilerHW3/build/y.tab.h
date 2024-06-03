/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     COUT = 258,
     NEWLINE = 259,
     AUTO = 260,
     SHR = 261,
     SHL = 262,
     BAN = 263,
     BOR = 264,
     BNT = 265,
     BXO = 266,
     ADD = 267,
     SUB = 268,
     MUL = 269,
     DIV = 270,
     REM = 271,
     NOT = 272,
     GTR = 273,
     LES = 274,
     GEQ = 275,
     LEQ = 276,
     EQL = 277,
     NEQ = 278,
     LAN = 279,
     LOR = 280,
     VAL_ASSIGN = 281,
     ADD_ASSIGN = 282,
     SUB_ASSIGN = 283,
     MUL_ASSIGN = 284,
     DIV_ASSIGN = 285,
     REM_ASSIGN = 286,
     BAN_ASSIGN = 287,
     BOR_ASSIGN = 288,
     BXO_ASSIGN = 289,
     SHR_ASSIGN = 290,
     SHL_ASSIGN = 291,
     INC_ASSIGN = 292,
     DEC_ASSIGN = 293,
     IF = 294,
     ELSE = 295,
     FOR = 296,
     WHILE = 297,
     RETURN = 298,
     BREAK = 299,
     CONTINUE = 300,
     VARIABLE_T = 301,
     IDENT = 302,
     INT_LIT = 303,
     STR_LIT = 304,
     CHAR_LIT = 305,
     BOOL_LIT = 306,
     FLOAT_LIT = 307,
     THEN = 308
   };
#endif
/* Tokens.  */
#define COUT 258
#define NEWLINE 259
#define AUTO 260
#define SHR 261
#define SHL 262
#define BAN 263
#define BOR 264
#define BNT 265
#define BXO 266
#define ADD 267
#define SUB 268
#define MUL 269
#define DIV 270
#define REM 271
#define NOT 272
#define GTR 273
#define LES 274
#define GEQ 275
#define LEQ 276
#define EQL 277
#define NEQ 278
#define LAN 279
#define LOR 280
#define VAL_ASSIGN 281
#define ADD_ASSIGN 282
#define SUB_ASSIGN 283
#define MUL_ASSIGN 284
#define DIV_ASSIGN 285
#define REM_ASSIGN 286
#define BAN_ASSIGN 287
#define BOR_ASSIGN 288
#define BXO_ASSIGN 289
#define SHR_ASSIGN 290
#define SHL_ASSIGN 291
#define INC_ASSIGN 292
#define DEC_ASSIGN 293
#define IF 294
#define ELSE 295
#define FOR 296
#define WHILE 297
#define RETURN 298
#define BREAK 299
#define CONTINUE 300
#define VARIABLE_T 301
#define IDENT 302
#define INT_LIT 303
#define STR_LIT 304
#define CHAR_LIT 305
#define BOOL_LIT 306
#define FLOAT_LIT 307
#define THEN 308




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 18 "./compiler.y"
{
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
        };

        char *name;
        ObjectType type;
    } object_val;


    Object obj_val;
}
/* Line 1529 of yacc.c.  */
#line 182 "./build/y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

