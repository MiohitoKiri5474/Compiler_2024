/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 2 "./compiler.y"

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


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

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
/* Line 193 of yacc.c.  */
#line 244 "./build/y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 257 "./build/y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   370

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  63
/* YYNRULES -- Number of rules.  */
#define YYNRULES  139
/* YYNRULES -- Number of states.  */
#define YYNSTATES  207

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   308

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      55,    56,     2,     2,    59,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    62,    54,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    60,     2,    61,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    57,     2,    58,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     8,    11,    13,    15,    17,
      23,    24,    25,    36,    41,    46,    49,    50,    57,    60,
      62,    63,    68,    69,    74,    77,    79,    81,    83,    85,
      87,    89,    92,    94,    95,   100,   103,   105,   106,   111,
     112,   118,   120,   123,   125,   127,   129,   131,   132,   137,
     140,   143,   145,   147,   149,   151,   153,   155,   157,   159,
     161,   163,   165,   167,   169,   171,   173,   175,   177,   179,
     181,   183,   185,   187,   189,   191,   193,   195,   197,   199,
     200,   205,   206,   211,   215,   217,   219,   223,   226,   227,
     235,   238,   242,   244,   248,   249,   253,   256,   259,   261,
     263,   265,   267,   269,   271,   273,   275,   277,   279,   281,
     284,   287,   288,   294,   295,   301,   302,   308,   310,   311,
     316,   317,   318,   324,   325,   332,   333,   341,   342,   343,
     344,   354,   356,   358,   362,   367,   369,   371,   375,   377
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      64,     0,    -1,    -1,    65,    66,    -1,    -1,    66,    67,
      -1,    67,    -1,    68,    -1,    69,    -1,    46,    47,    26,
      80,    54,    -1,    -1,    -1,    46,    47,    70,    55,    72,
      56,    71,    57,    74,    58,    -1,    72,    59,    46,    47,
      -1,    46,    47,    60,    61,    -1,    46,    47,    -1,    -1,
      46,    47,    60,    48,    73,    61,    -1,    75,    74,    -1,
      75,    -1,    -1,     3,    76,    78,    54,    -1,    -1,    43,
      77,    80,    54,    -1,    43,    54,    -1,    54,    -1,    93,
      -1,   104,    -1,   102,    -1,   112,    -1,   115,    -1,    44,
      54,    -1,   123,    -1,    -1,     7,    80,    79,    78,    -1,
       7,    80,    -1,    83,    -1,    -1,    80,    88,    81,    80,
      -1,    -1,    55,    46,    82,    56,    80,    -1,    84,    -1,
      92,    83,    -1,    85,    -1,   123,    -1,    87,    -1,    47,
      -1,    -1,    55,    86,    80,    56,    -1,    47,   100,    -1,
      47,    99,    -1,    48,    -1,    52,    -1,    51,    -1,    49,
      -1,    25,    -1,    24,    -1,    89,    -1,    90,    -1,    91,
      -1,    22,    -1,    23,    -1,    19,    -1,    21,    -1,    18,
      -1,    20,    -1,    12,    -1,    13,    -1,     8,    -1,     9,
      -1,    11,    -1,    14,    -1,    15,    -1,    16,    -1,     6,
      -1,    12,    -1,    13,    -1,    17,    -1,    10,    -1,    -1,
      46,    94,    96,    54,    -1,    -1,     5,    95,    96,    54,
      -1,    97,    59,    96,    -1,    97,    -1,    47,    -1,    47,
      26,    80,    -1,    47,    99,    -1,    -1,    47,   100,    98,
      26,    57,   101,    58,    -1,   100,   100,    -1,    60,   125,
      61,    -1,    80,    -1,    80,    59,   101,    -1,    -1,    85,
     103,    80,    -1,    85,    37,    -1,    85,    38,    -1,    26,
      -1,    27,    -1,    28,    -1,    29,    -1,    30,    -1,    31,
      -1,    33,    -1,    32,    -1,    34,    -1,    35,    -1,    36,
      -1,   107,   110,    -1,   107,    75,    -1,    -1,   107,   110,
      40,   105,   110,    -1,    -1,   107,   110,    40,   106,   104,
      -1,    -1,   108,    39,    55,   109,    56,    -1,    80,    -1,
      -1,    57,   111,    74,    58,    -1,    -1,    -1,    42,   113,
     109,   114,   110,    -1,    -1,    41,   116,   117,    57,    74,
      58,    -1,    -1,    55,     5,    47,    62,    47,   118,    56,
      -1,    -1,    -1,    -1,    55,   122,   119,   109,   120,    54,
     102,   121,    56,    -1,    93,    -1,    54,    -1,    47,    55,
      56,    -1,    47,    55,   124,    56,    -1,    49,    -1,    80,
      -1,    80,    59,   124,    -1,    48,    -1,    47,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    82,    82,    82,    83,    87,    88,    92,    93,    97,
     102,   108,   102,   130,   145,   161,   176,   176,   197,   198,
     202,   202,   207,   207,   212,   216,   217,   218,   219,   220,
     221,   222,   223,   227,   227,   246,   268,   269,   269,   304,
     304,   315,   316,   327,   333,   337,   338,   360,   360,   371,
     386,   404,   412,   420,   428,   439,   440,   441,   442,   443,
     447,   448,   449,   450,   451,   452,   456,   457,   458,   459,
     460,   464,   465,   466,   467,   471,   472,   473,   474,   478,
     478,   484,   484,   490,   491,   495,   498,   501,   504,   504,
     511,   515,   519,   520,   521,   525,   530,   531,   535,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   549,
     550,   551,   551,   552,   552,   556,   556,   560,   564,   564,
     568,   568,   568,   572,   572,   579,   579,   584,   584,   584,
     584,   591,   592,   597,   602,   610,   611,   612,   616,   619
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "COUT", "NEWLINE", "AUTO", "SHR", "SHL",
  "BAN", "BOR", "BNT", "BXO", "ADD", "SUB", "MUL", "DIV", "REM", "NOT",
  "GTR", "LES", "GEQ", "LEQ", "EQL", "NEQ", "LAN", "LOR", "VAL_ASSIGN",
  "ADD_ASSIGN", "SUB_ASSIGN", "MUL_ASSIGN", "DIV_ASSIGN", "REM_ASSIGN",
  "BAN_ASSIGN", "BOR_ASSIGN", "BXO_ASSIGN", "SHR_ASSIGN", "SHL_ASSIGN",
  "INC_ASSIGN", "DEC_ASSIGN", "IF", "ELSE", "FOR", "WHILE", "RETURN",
  "BREAK", "CONTINUE", "VARIABLE_T", "IDENT", "INT_LIT", "STR_LIT",
  "CHAR_LIT", "BOOL_LIT", "FLOAT_LIT", "THEN", "';'", "'('", "')'", "'{'",
  "'}'", "','", "'['", "']'", "':'", "$accept", "Program", "@1",
  "GlobalStmtList", "GlobalStmt", "DefineVariableStmt", "FunctionDefStmt",
  "@2", "@3", "FunctionParameterStmtList", "@4", "StmtList", "Stmt", "@5",
  "@6", "CoutParmListStmt", "@7", "Expression", "@8", "@9", "UnaryExpr",
  "PrimaryExpr", "Operand", "@10", "Literal", "binary_op", "cmp_op",
  "add_op", "mul_op", "unary_op", "DeclarationStmt", "@11", "@12",
  "DeclarationList", "DeclarationIDENT", "@13", "D2Array", "D1Array",
  "ListOfArray", "AssignmentStmt", "assign_op", "IfStmt", "@14", "@15",
  "IfCondition", "@16", "Condition", "Block", "@17", "WhileStmt", "@18",
  "@19", "ForStmt", "@20", "ForIn", "@21", "@22", "@23", "@24",
  "ForDeclare", "FuncCallStmt", "ArgumentList", "Index", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,    59,    40,    41,   123,   125,    44,
      91,    93,    58
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    63,    65,    64,    64,    66,    66,    67,    67,    68,
      70,    71,    69,    72,    72,    72,    73,    72,    74,    74,
      76,    75,    77,    75,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    79,    78,    78,    80,    81,    80,    82,
      80,    83,    83,    84,    84,    85,    85,    86,    85,    85,
      85,    87,    87,    87,    87,    88,    88,    88,    88,    88,
      89,    89,    89,    89,    89,    89,    90,    90,    90,    90,
      90,    91,    91,    91,    91,    92,    92,    92,    92,    94,
      93,    95,    93,    96,    96,    97,    97,    97,    98,    97,
      99,   100,   101,   101,   101,   102,   102,   102,   103,   103,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   104,
     104,   105,   104,   106,   104,   108,   107,   109,   111,   110,
     113,   114,   112,   116,   115,   118,   117,   119,   120,   121,
     117,   122,   122,   123,   123,   124,   124,   124,   125,   125
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     2,     1,     1,     1,     5,
       0,     0,    10,     4,     4,     2,     0,     6,     2,     1,
       0,     4,     0,     4,     2,     1,     1,     1,     1,     1,
       1,     2,     1,     0,     4,     2,     1,     0,     4,     0,
       5,     1,     2,     1,     1,     1,     1,     0,     4,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       4,     0,     4,     3,     1,     1,     3,     2,     0,     7,
       2,     3,     1,     3,     0,     3,     2,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     0,     5,     0,     5,     0,     5,     1,     0,     4,
       0,     0,     5,     0,     6,     0,     7,     0,     0,     0,
       9,     1,     1,     3,     4,     1,     1,     3,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     1,     0,     3,     6,     7,     8,    10,
       5,     0,     0,    78,    75,    76,    77,    46,    51,    54,
      53,    52,    47,     0,    36,    41,    43,    45,     0,    44,
       0,     0,     0,    50,    49,    39,     0,    74,    68,    69,
      70,    66,    67,    71,    72,    73,    64,    62,    65,    63,
      60,    61,    56,    55,     9,    37,    57,    58,    59,    47,
      42,     0,     0,    54,   133,   136,     0,   139,   138,     0,
      90,     0,     0,     0,    15,    11,     0,     0,   134,    91,
       0,    48,    38,     0,     0,     0,   137,    40,    16,    14,
     115,    13,     0,    20,    81,   123,   120,    22,     0,    79,
      25,     0,    19,     0,    26,    28,    27,   115,     0,    29,
      30,    32,    17,     0,     0,     0,     0,    24,     0,    31,
       0,    12,    18,    98,    99,   100,   101,   102,   103,   105,
     104,   106,   107,   108,    96,    97,     0,   118,   110,   109,
       0,     0,     0,    85,     0,    84,     0,     0,   117,   121,
       0,     0,    95,   115,   111,     0,    33,    21,     0,    87,
      88,    82,     0,     0,   132,   131,   127,   115,     0,    23,
      80,     0,     0,   115,     0,     0,    86,     0,    83,     0,
       0,     0,   122,   119,   112,   114,   116,    34,     0,     0,
     128,   124,    94,   125,     0,    92,     0,     0,     0,    94,
      89,   126,    46,   129,    93,     0,   130
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     5,     6,     7,     8,    12,    84,    62,
      92,   101,   102,   113,   118,   142,   175,   148,    73,    71,
      24,    25,    26,    36,    27,    55,    56,    57,    58,    28,
     104,   120,   114,   144,   145,   177,    33,    34,   196,   105,
     136,   106,   172,   173,   107,   108,   149,   139,   153,   109,
     116,   168,   110,   115,   147,   197,   180,   194,   205,   166,
      29,    66,    69
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -156
static const yytype_int16 yypact[] =
{
       5,    28,   -23,  -156,    26,   -23,  -156,  -156,  -156,    58,
    -156,   265,    36,  -156,  -156,  -156,  -156,   -49,  -156,  -156,
    -156,  -156,    57,   191,  -156,  -156,  -156,  -156,   274,  -156,
      59,   255,    16,  -156,    44,  -156,   265,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,    61,   -44,  -156,  -156,    18,    50,  -156,  -156,    48,
    -156,    55,   171,   265,    53,  -156,    68,   283,  -156,  -156,
     265,  -156,   331,   -26,    62,    69,  -156,   331,  -156,  -156,
     112,  -156,    60,  -156,  -156,  -156,  -156,    64,    66,  -156,
    -156,    65,    97,   332,  -156,  -156,  -156,     4,    83,  -156,
    -156,  -156,  -156,   117,    79,    73,   265,  -156,   265,  -156,
      79,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,   265,  -156,  -156,    89,
      76,   265,    78,   -16,    80,    91,     3,    85,   331,  -156,
     217,    81,   331,   112,    98,   265,   238,  -156,   265,  -156,
      44,  -156,    79,   110,  -156,  -156,  -156,   112,   105,  -156,
    -156,   107,   105,  -156,   113,   117,   331,   142,  -156,   108,
     265,   114,  -156,  -156,  -156,  -156,  -156,  -156,   116,   124,
    -156,  -156,   265,  -156,   120,    74,   118,   119,    23,   265,
    -156,  -156,    44,  -156,  -156,   122,  -156
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -156,  -156,  -156,  -156,   193,  -156,  -156,  -156,  -156,  -156,
    -156,   -99,    94,  -156,  -156,    33,  -156,   -11,  -156,  -156,
     189,  -156,   -86,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
      72,  -156,  -156,  -102,  -156,  -156,    77,   -33,    20,    24,
    -156,    51,  -156,  -156,  -156,  -156,   -79,  -155,  -156,  -156,
    -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,  -156,
     -88,   144,  -156
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -116
static const yytype_int16 yytable[] =
{
      23,    70,   111,   122,   103,    -4,    31,    93,   163,    94,
     158,    32,    75,   182,   111,    76,   103,   184,   151,   111,
      65,   103,    88,     4,    37,    72,    38,    39,     3,    40,
      41,    42,    43,    44,    45,    89,    46,    47,    48,    49,
      50,    51,    52,    53,    32,    95,    96,    97,    98,    99,
      99,    17,    18,    19,   171,    20,    21,   164,   100,    59,
     178,   137,    82,    67,    68,   111,    65,   103,   181,    87,
     202,    18,    19,     9,    20,    21,   174,    77,    59,   111,
      37,   103,    38,    39,    11,    40,    41,    42,    43,    44,
      45,    30,    46,    47,    48,    49,    50,    51,    52,    53,
      93,   190,    94,    35,    32,    61,    78,   150,    74,    79,
     160,    80,   103,    83,    85,    93,    91,    94,   117,    90,
     119,   112,   140,   121,   141,   152,   143,    70,   146,   154,
     156,   155,   157,   199,   161,   170,  -115,  -113,    95,    96,
      97,    98,   167,    99,    17,    18,    19,   176,    20,    21,
     162,   100,    59,    95,    96,    97,    98,   179,    99,    17,
      18,    19,   137,    20,    21,   183,   100,    59,   188,   186,
     189,   193,   191,   192,   198,   201,   200,    37,   206,    38,
      39,   195,    40,    41,    42,    43,    44,    45,   195,    46,
      47,    48,    49,    50,    51,    52,    53,    37,    10,    38,
      39,   138,    40,    41,    42,    43,    44,    45,   187,    46,
      47,    48,    49,    50,    51,    52,    53,    60,   165,   204,
     159,    86,   203,    37,   185,    38,    39,    81,    40,    41,
      42,    43,    44,    45,     0,    46,    47,    48,    49,    50,
      51,    52,    53,     0,    37,    54,    38,    39,     0,    40,
      41,    42,    43,    44,    45,     0,    46,    47,    48,    49,
      50,    51,    52,    53,     0,    13,     0,    14,    15,     0,
       0,   169,    16,     0,     0,    13,     0,    14,    15,     0,
       0,     0,    16,     0,    13,     0,    14,    15,     0,     0,
       0,    16,   -35,    13,     0,    14,    15,     0,     0,     0,
      16,     0,    17,    18,    63,     0,    20,    21,     0,     0,
      22,    64,    17,    18,    19,     0,    20,    21,     0,     0,
      22,    17,    18,    19,     0,    20,    21,     0,     0,    59,
      17,    18,    63,     0,    20,    21,     0,    37,    22,    38,
      39,     0,    40,    41,    42,    43,    44,    45,     0,    46,
      47,    48,    49,    50,    51,    52,    53,     0,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135
};

static const yytype_int16 yycheck[] =
{
      11,    34,    90,   102,    90,     0,    55,     3,     5,     5,
      26,    60,    56,   168,   102,    59,   102,   172,   120,   107,
      31,   107,    48,    46,     6,    36,     8,     9,     0,    11,
      12,    13,    14,    15,    16,    61,    18,    19,    20,    21,
      22,    23,    24,    25,    60,    41,    42,    43,    44,    46,
      46,    47,    48,    49,   153,    51,    52,    54,    54,    55,
     162,    57,    73,    47,    48,   153,    77,   153,   167,    80,
      47,    48,    49,    47,    51,    52,   155,    59,    55,   167,
       6,   167,     8,     9,    26,    11,    12,    13,    14,    15,
      16,    55,    18,    19,    20,    21,    22,    23,    24,    25,
       3,   180,     5,    46,    60,    46,    56,   118,    47,    61,
     143,    56,   198,    60,    46,     3,    47,     5,    54,    57,
      54,    61,    39,    58,     7,   136,    47,   160,    55,    40,
     141,    55,    54,    59,    54,    54,    39,    39,    41,    42,
      43,    44,    57,    46,    47,    48,    49,   158,    51,    52,
      59,    54,    55,    41,    42,    43,    44,    47,    46,    47,
      48,    49,    57,    51,    52,    58,    54,    55,    26,    56,
      62,    47,    58,    57,    54,    56,    58,     6,    56,     8,
       9,   192,    11,    12,    13,    14,    15,    16,   199,    18,
      19,    20,    21,    22,    23,    24,    25,     6,     5,     8,
       9,   107,    11,    12,    13,    14,    15,    16,   175,    18,
      19,    20,    21,    22,    23,    24,    25,    28,   146,   199,
     143,    77,   198,     6,   173,     8,     9,    56,    11,    12,
      13,    14,    15,    16,    -1,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,     6,    54,     8,     9,    -1,    11,
      12,    13,    14,    15,    16,    -1,    18,    19,    20,    21,
      22,    23,    24,    25,    -1,    10,    -1,    12,    13,    -1,
      -1,    54,    17,    -1,    -1,    10,    -1,    12,    13,    -1,
      -1,    -1,    17,    -1,    10,    -1,    12,    13,    -1,    -1,
      -1,    17,    54,    10,    -1,    12,    13,    -1,    -1,    -1,
      17,    -1,    47,    48,    49,    -1,    51,    52,    -1,    -1,
      55,    56,    47,    48,    49,    -1,    51,    52,    -1,    -1,
      55,    47,    48,    49,    -1,    51,    52,    -1,    -1,    55,
      47,    48,    49,    -1,    51,    52,    -1,     6,    55,     8,
       9,    -1,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    64,    65,     0,    46,    66,    67,    68,    69,    47,
      67,    26,    70,    10,    12,    13,    17,    47,    48,    49,
      51,    52,    55,    80,    83,    84,    85,    87,    92,   123,
      55,    55,    60,    99,   100,    46,    86,     6,     8,     9,
      11,    12,    13,    14,    15,    16,    18,    19,    20,    21,
      22,    23,    24,    25,    54,    88,    89,    90,    91,    55,
      83,    46,    72,    49,    56,    80,   124,    47,    48,   125,
     100,    82,    80,    81,    47,    56,    59,    59,    56,    61,
      56,    56,    80,    60,    71,    46,   124,    80,    48,    61,
      57,    47,    73,     3,     5,    41,    42,    43,    44,    46,
      54,    74,    75,    85,    93,   102,   104,   107,   108,   112,
     115,   123,    61,    76,    95,   116,   113,    54,    77,    54,
      94,    58,    74,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,   103,    57,    75,   110,
      39,     7,    78,    47,    96,    97,    55,   117,    80,   109,
      80,    96,    80,   111,    40,    55,    80,    54,    26,    99,
     100,    54,    59,     5,    54,    93,   122,    57,   114,    54,
      54,    74,   105,   106,   109,    79,    80,    98,    96,    47,
     119,    74,   110,    58,   110,   104,    56,    78,    26,    62,
     109,    58,    57,    47,   120,    80,   101,   118,    54,    59,
      58,    56,    47,   102,   101,   121,    56
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 82 "./compiler.y"
    { Create_Table(); }
    break;

  case 10:
#line 102 "./compiler.y"
    {
        printf ( "func: %s\n", (yyvsp[(2) - (2)].s_var) );
        bool tmp = !strcmp ( (yyvsp[(2) - (2)].s_var), "main" );
        Insert_Symbol ( (yyvsp[(2) - (2)].s_var), OBJECT_TYPE_FUNCTION, "func", yylineno + ( tmp ? 0 : 1 ) );
        first_argument = true;
        Create_Table();
    }
    break;

  case 11:
#line 108 "./compiler.y"
    {
        char tmp[2];
        tmp[0] = get_type ( (yyvsp[(1) - (6)].var_type) ), tmp[1] = '\0';
        strcat ( (yyvsp[(5) - (6)].s_var), ")" );
        if ( tmp[0] == 'S' )
            strcat ( (yyvsp[(5) - (6)].s_var), "Ljava/lang/String;" );
        else
            strcat ( (yyvsp[(5) - (6)].s_var), tmp );
        Update_Symbol ( (yyvsp[(2) - (6)].s_var), (yyvsp[(5) - (6)].s_var) );

        ScopeMinusOne();
        if ( !strcmp ( (yyvsp[(2) - (6)].s_var), "main" ) )
        	codeRaw ( ".method public static main([Ljava/lang/String;)V\n" );
        else
			code ( ".method public statis %s()V\n", (yyvsp[(1) - (6)].var_type) );
        ScopeAddOne();
        codeRaw ( ".limit stack 128" );
        codeRaw ( ".limit locals 128\n" );
    }
    break;

  case 12:
#line 126 "./compiler.y"
    { Dump_Table(); }
    break;

  case 13:
#line 130 "./compiler.y"
    {
        char tmp[2];
        tmp[0] = get_type ( (yyvsp[(3) - (4)].var_type) ), tmp[1] = '\0';
        if ( first_argument ) {
            (yyval.s_var) = ( char * ) malloc ( sizeof ( char ) * 1024 );
            (yyval.s_var)[0] = '(';
            first_argument = false;
        }
        if ( tmp[0] == 'S' )
            strcat ( (yyval.s_var), "Ljava/lang/String;" );
        else
            strcat ( (yyval.s_var), tmp );

        Insert_Symbol ( (yyvsp[(4) - (4)].s_var), (yyvsp[(3) - (4)].var_type), "", yylineno );
    }
    break;

  case 14:
#line 145 "./compiler.y"
    {
        char tmp[2];
        tmp[0] = get_type ( (yyvsp[(1) - (4)].var_type) );
        tmp[1] = '\0';
        if ( first_argument ) {
            (yyval.s_var) = ( char * ) malloc ( sizeof ( char ) * 1024 );
            (yyval.s_var)[0] = '(';
            first_argument = false;
        }
        if ( tmp[0] == 'S' )
            strcat ( (yyval.s_var), "[Ljava/lang/String;" );
        else
            strcat ( (yyval.s_var), tmp );
        Insert_Symbol ( (yyvsp[(2) - (4)].s_var), (yyvsp[(1) - (4)].var_type), "", yylineno - ( !strcmp ( "argv", (yyvsp[(2) - (4)].s_var) ) ? 1 : 0 ) );

    }
    break;

  case 15:
#line 161 "./compiler.y"
    {
        char tmp[2];
        tmp[0] = get_type ( (yyvsp[(1) - (2)].var_type) ), tmp[1] = '\0';
        if ( first_argument ) {
            (yyval.s_var) = ( char * ) malloc ( sizeof ( char ) * 1024 );
            (yyval.s_var)[0] = '(';
            first_argument = false;
        }
        if ( tmp[0] == 'S' )
            strcat ( (yyval.s_var), "Ljava/lang/String;" );
        else
            strcat ( (yyval.s_var), tmp );

        Insert_Symbol ( (yyvsp[(2) - (2)].s_var), (yyvsp[(1) - (2)].var_type), "", yylineno );
    }
    break;

  case 16:
#line 176 "./compiler.y"
    { printf ( "INT_LIT %d\n", (yyvsp[(4) - (4)].i_var) ); }
    break;

  case 17:
#line 176 "./compiler.y"
    {
        char tmp[2];
        tmp[0] = get_type ( (yyvsp[(1) - (6)].var_type) ), tmp[1] = '\0';
        if ( first_argument ) {
            (yyval.s_var) = ( char * ) malloc ( sizeof ( char ) * 1024 );
            (yyval.s_var)[0] = '(';
            first_argument = false;
        }
        if ( tmp[0] == 'S' )
            strcat ( (yyval.s_var), "Ljava/lang/String;" );
        else
            strcat ( (yyval.s_var), tmp );

        Insert_Symbol ( (yyvsp[(2) - (6)].s_var), (yyvsp[(1) - (6)].var_type), "", yylineno - ( !strcmp ( "argv", (yyvsp[(2) - (6)].s_var) ) ? 1 : 0 ) );

    }
    break;

  case 20:
#line 202 "./compiler.y"
    { Reset_treap(); couting = true; }
    break;

  case 21:
#line 202 "./compiler.y"
    {
        printf ( "cout" );
        Print_List();
        couting = false;
    }
    break;

  case 22:
#line 207 "./compiler.y"
    { is_return = true; }
    break;

  case 23:
#line 207 "./compiler.y"
    {
        puts ( "RETURN" );
        codeRaw ( "return\n.end method" );
        is_return = false;
    }
    break;

  case 24:
#line 212 "./compiler.y"
    {
        puts ( "RETURN" );
        codeRaw ( "return\n.end method" );
    }
    break;

  case 31:
#line 222 "./compiler.y"
    { puts ( "BREAK" ); }
    break;

  case 33:
#line 227 "./compiler.y"
    {
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
            Insert_Cout ( get_type_name ( (yyvsp[(2) - (2)].object_val).type ) );
        codeRaw ( "getstatic java/lang/System/out Ljava/io/PrintStream;" );
        codeRaw ( "swap" );
        code ( "invokevirtual java/io/PrintStream/print(%s)V", get_print_type ( (yyvsp[(2) - (2)].object_val).type ));
    }
    break;

  case 35:
#line 246 "./compiler.y"
    {
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
            Insert_Cout ( get_type_name ( (yyvsp[(2) - (2)].object_val).type ) );
        codeRaw ( "getstatic java/lang/System/out Ljava/io/PrintStream;" );
        codeRaw ( "swap" );
        code ( "invokevirtual java/io/PrintStream/print(%s)V", get_print_type ( (yyvsp[(2) - (2)].object_val).type ));
    }
    break;

  case 36:
#line 268 "./compiler.y"
    { (yyval.object_val).type = (yyvsp[(1) - (1)].object_val).type; }
    break;

  case 37:
#line 269 "./compiler.y"
    {
        if ( ( (yyvsp[(2) - (2)].op) == OP_LOR || (yyvsp[(2) - (2)].op) == OP_LAN ) && !if_flag ) {
            while ( op_idx && get_op_priority ( (yyvsp[(2) - (2)].op) ) < get_op_priority ( ops[op_idx] ) ) {
                if ( ops[op_idx] == OP_LBRA ) {
                    op_idx--;
                    break;
                }
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        else {
            while ( op_idx && get_op_priority ( (yyvsp[(2) - (2)].op) ) <= get_op_priority ( ops[op_idx] ) ) {
                if ( ops[op_idx] == OP_LBRA ) {
                    op_idx--;
                    break;
                }
                printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
            }
        }
        ops[++op_idx] = (yyvsp[(2) - (2)].op);
    }
    break;

  case 38:
#line 289 "./compiler.y"
    {
        while ( op_idx ) {
            if ( ops[op_idx] == OP_LBRA ) {
                break;
            }
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }

        if ( couting ) {
            if ( (yyvsp[(1) - (4)].object_val).type == OBJECT_TYPE_BOOL || (yyvsp[(4) - (4)].object_val).type == OBJECT_TYPE_BOOL )
                (yyval.object_val).type = OBJECT_TYPE_BOOL, is_bool = true, is_float = false;
            else if ( (yyvsp[(1) - (4)].object_val).type == OBJECT_TYPE_FLOAT || (yyvsp[(4) - (4)].object_val).type == OBJECT_TYPE_FLOAT )
                (yyval.object_val).type = OBJECT_TYPE_FLOAT, is_float = true;
        }
    }
    break;

  case 39:
#line 304 "./compiler.y"
    {
        is_cast = true;
        cast_type = (yyvsp[(2) - (2)].var_type);
    }
    break;

  case 40:
#line 307 "./compiler.y"
    {
        (yyval.s_var) = (yyvsp[(5) - (5)].s_var);
        (yyval.object_val).type = (yyvsp[(2) - (5)].var_type);
        is_float = is_str = is_bool = false;
    }
    break;

  case 41:
#line 315 "./compiler.y"
    { (yyval.object_val).type = (yyvsp[(1) - (1)].object_val).type; }
    break;

  case 42:
#line 316 "./compiler.y"
    {
        (yyval.object_val).type = (yyvsp[(2) - (2)].object_val).type;
        if ( (yyval.object_val).type == OBJECT_TYPE_INT )
			(yyval.object_val).i_var = ( (yyvsp[(1) - (2)].op) == OP_POS ? (yyvsp[(2) - (2)].object_val).i_var : -(yyvsp[(2) - (2)].object_val).i_var );
        else if ( (yyval.object_val).type == OBJECT_TYPE_FLOAT )
			(yyval.object_val).f_var = ( (yyvsp[(1) - (2)].op) == OP_POS ? (yyvsp[(2) - (2)].object_val).f_var : -(yyvsp[(2) - (2)].object_val).f_var );
        printf ( "%s\n", get_op_name ( (yyvsp[(1) - (2)].op) ) );
    }
    break;

  case 43:
#line 327 "./compiler.y"
    {
        if ( is_cast ) {
            printf ( "Cast to %s\n", get_type_name ( cast_type ) );
            is_cast = false;
        }
    }
    break;

  case 46:
#line 338 "./compiler.y"
    {
        if ( !strcmp ( "endl", (yyvsp[(1) - (1)].s_var) ) ) {
            (yyval.object_val).type = OBJECT_TYPE_STR;
            puts ( "IDENT (name=endl, address=-1)" );
            codeRaw ( "ldc \"\\n\"" );
        }
        else {
            Node *tmp = Query_Symbol ( (yyvsp[(1) - (1)].s_var) );
            if ( tmp ) {
                (yyval.object_val).type = tmp -> type;
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
    break;

  case 47:
#line 360 "./compiler.y"
    {
        ops[++op_idx] = OP_LBRA;
    }
    break;

  case 48:
#line 362 "./compiler.y"
    {
        while ( op_idx ) {
            if ( ops[op_idx] == OP_LBRA ) {
                op_idx--;
                break;
            }
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
        }
    }
    break;

  case 49:
#line 371 "./compiler.y"
    {
        Node *tmp = Query_Symbol ( (yyvsp[(1) - (2)].s_var) );
        if ( tmp ) {
            (yyval.object_val).type = tmp -> type;
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
    break;

  case 50:
#line 386 "./compiler.y"
    {
        Node *tmp = Query_Symbol ( (yyvsp[(1) - (2)].s_var) );
        if ( tmp ) {
            (yyval.object_val).type = tmp -> type;
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
    break;

  case 51:
#line 404 "./compiler.y"
    {
        (yyval.object_val).type = OBJECT_TYPE_INT;
        (yyval.object_val).i_var = (yyvsp[(1) - (1)].i_var);
        printf ( "INT_LIT %d\n", (yyval.object_val).i_var );
        if ( !is_return ) {
            code ( "ldc %d", (yyval.object_val).i_var );
        }
    }
    break;

  case 52:
#line 412 "./compiler.y"
    {
        (yyval.object_val).type = OBJECT_TYPE_FLOAT;
        (yyval.object_val).f_var = (yyvsp[(1) - (1)].f_var);
        printf ( "FLOAT_LIT %f\n", (yyval.object_val).f_var );
        if ( !is_return ) {
            code ( "ldc %f", (yyval.object_val).f_var );
        }
    }
    break;

  case 53:
#line 420 "./compiler.y"
    {
        (yyval.object_val).type = OBJECT_TYPE_BOOL;
        (yyval.object_val).b_var = (yyvsp[(1) - (1)].b_var);
        printf ( "BOOL_LIT %s\n", (yyval.object_val).b_var ? "TRUE" : "FALSE" );
        if ( !is_return ) {
            code ( "ldc %d", (yyval.object_val).b_var );
        }
    }
    break;

  case 54:
#line 428 "./compiler.y"
    {
        (yyval.object_val).type = OBJECT_TYPE_STR;
        (yyval.object_val).s_var = (yyvsp[(1) - (1)].s_var);
        printf ( "STR_LIT \"%s\"\n", (yyval.object_val).s_var );
        if ( !is_return ) {
            code ( "ldc \"%s\"", (yyval.object_val).s_var );
        }
    }
    break;

  case 55:
#line 439 "./compiler.y"
    { (yyval.op) = OP_LOR; }
    break;

  case 56:
#line 440 "./compiler.y"
    { (yyval.op) = OP_LAN; }
    break;

  case 60:
#line 447 "./compiler.y"
    { (yyval.op) = OP_EQL; }
    break;

  case 61:
#line 448 "./compiler.y"
    { (yyval.op) = OP_NEQ; }
    break;

  case 62:
#line 449 "./compiler.y"
    { (yyval.op) = OP_LES; }
    break;

  case 63:
#line 450 "./compiler.y"
    { (yyval.op) = OP_LEQ; }
    break;

  case 64:
#line 451 "./compiler.y"
    { (yyval.op) = OP_GTR; }
    break;

  case 65:
#line 452 "./compiler.y"
    { (yyval.op) = OP_GEQ; }
    break;

  case 66:
#line 456 "./compiler.y"
    { (yyval.op) = OP_ADD; }
    break;

  case 67:
#line 457 "./compiler.y"
    { (yyval.op) = OP_SUB; }
    break;

  case 68:
#line 458 "./compiler.y"
    { (yyval.op) = OP_BAN; }
    break;

  case 69:
#line 459 "./compiler.y"
    { (yyval.op) = OP_BOR; }
    break;

  case 70:
#line 460 "./compiler.y"
    { (yyval.op) = OP_BXO; }
    break;

  case 71:
#line 464 "./compiler.y"
    { (yyval.op) = OP_MUL; }
    break;

  case 72:
#line 465 "./compiler.y"
    { (yyval.op) = OP_DIV; }
    break;

  case 73:
#line 466 "./compiler.y"
    { (yyval.op) = OP_REM; }
    break;

  case 74:
#line 467 "./compiler.y"
    { (yyval.op) = OP_RSHIFT; }
    break;

  case 75:
#line 471 "./compiler.y"
    { (yyval.op) = OP_POS; }
    break;

  case 76:
#line 472 "./compiler.y"
    { (yyval.op) = OP_NEG; }
    break;

  case 77:
#line 473 "./compiler.y"
    { (yyval.op) = OP_NOT; }
    break;

  case 78:
#line 474 "./compiler.y"
    { (yyval.op) = OP_BNT; }
    break;

  case 79:
#line 478 "./compiler.y"
    {
        is_declare = true;
        declare_type = (yyvsp[(1) - (1)].var_type);
        if ( (yyvsp[(1) - (1)].var_type) == OBJECT_TYPE_AUTO )
            is_auto = true;
    }
    break;

  case 80:
#line 483 "./compiler.y"
    { is_auto = is_declare = false; }
    break;

  case 81:
#line 484 "./compiler.y"
    {
        is_auto = true;
    }
    break;

  case 82:
#line 486 "./compiler.y"
    { is_auto = is_declare = false; }
    break;

  case 85:
#line 495 "./compiler.y"
    {
        Insert_Symbol ( (yyvsp[(1) - (1)].s_var), declare_type, "", yylineno );
    }
    break;

  case 86:
#line 498 "./compiler.y"
    {
        Insert_Symbol ( (yyvsp[(1) - (3)].s_var), ( is_auto ? (yyvsp[(3) - (3)].object_val).type : declare_type ), "", yylineno );
    }
    break;

  case 87:
#line 501 "./compiler.y"
    {
        Insert_Symbol ( (yyvsp[(1) - (2)].s_var), declare_type, "", yylineno );
    }
    break;

  case 88:
#line 504 "./compiler.y"
    { arr_len = 0; }
    break;

  case 89:
#line 504 "./compiler.y"
    {
        printf ( "create array: %d\n", arr_len );
        Insert_Symbol ( (yyvsp[(1) - (7)].s_var), declare_type, "", yylineno );
    }
    break;

  case 92:
#line 519 "./compiler.y"
    { arr_len++; }
    break;

  case 93:
#line 520 "./compiler.y"
    { arr_len++; }
    break;

  case 95:
#line 525 "./compiler.y"
    {
        ops[++op_idx] = (yyvsp[(2) - (3)].op);
        while ( op_idx )
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
    }
    break;

  case 96:
#line 530 "./compiler.y"
    { ops[++op_idx] = OP_INC_ASSIGN; }
    break;

  case 97:
#line 531 "./compiler.y"
    { ops[++op_idx] = OP_DEC_ASSIGN; }
    break;

  case 98:
#line 535 "./compiler.y"
    { (yyval.op) = OP_VAL_ASSIGN; }
    break;

  case 99:
#line 536 "./compiler.y"
    { (yyval.op) = OP_ADD_ASSIGN; }
    break;

  case 100:
#line 537 "./compiler.y"
    { (yyval.op) = OP_SUB_ASSIGN; }
    break;

  case 101:
#line 538 "./compiler.y"
    { (yyval.op) = OP_MUL_ASSIGN; }
    break;

  case 102:
#line 539 "./compiler.y"
    { (yyval.op) = OP_DIV_ASSIGN; }
    break;

  case 103:
#line 540 "./compiler.y"
    { (yyval.op) = OP_REM_ASSIGN; }
    break;

  case 104:
#line 541 "./compiler.y"
    { (yyval.op) = OP_BOR_ASSIGN; }
    break;

  case 105:
#line 542 "./compiler.y"
    { (yyval.op) = OP_BAN_ASSIGN; }
    break;

  case 106:
#line 543 "./compiler.y"
    { (yyval.op) = OP_BXO_ASSIGN; }
    break;

  case 107:
#line 544 "./compiler.y"
    { (yyval.op) = OP_SHR_ASSIGN; }
    break;

  case 108:
#line 545 "./compiler.y"
    { (yyval.op) = OP_SHL_ASSIGN; }
    break;

  case 111:
#line 551 "./compiler.y"
    { puts ( "ELSE" ); }
    break;

  case 113:
#line 552 "./compiler.y"
    { puts ( "ELSE" ); }
    break;

  case 115:
#line 556 "./compiler.y"
    { if_flag = true; }
    break;

  case 116:
#line 556 "./compiler.y"
    { puts ( "IF" ); if_flag = false; }
    break;

  case 118:
#line 564 "./compiler.y"
    { Create_Table(); }
    break;

  case 119:
#line 564 "./compiler.y"
    { Dump_Table(); }
    break;

  case 120:
#line 568 "./compiler.y"
    { if_flag = true; puts ( "WHILE" ); }
    break;

  case 121:
#line 568 "./compiler.y"
    { if_flag = false; }
    break;

  case 123:
#line 572 "./compiler.y"
    {
        puts ( "FOR" );
        Create_Table();
    }
    break;

  case 124:
#line 575 "./compiler.y"
    { Dump_Table(); }
    break;

  case 125:
#line 579 "./compiler.y"
    {
        Node *tmp = Query_Symbol ( (yyvsp[(5) - (5)].s_var) );
        Insert_Symbol ( (yyvsp[(3) - (5)].s_var), tmp -> type, "", yylineno );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
    }
    break;

  case 127:
#line 584 "./compiler.y"
    { if_flag = true; }
    break;

  case 128:
#line 584 "./compiler.y"
    { if_flag = false; }
    break;

  case 129:
#line 584 "./compiler.y"
    {
        while ( op_idx )
            printf ( "%s\n", get_op_name ( ops[op_idx--] ) );
    }
    break;

  case 133:
#line 597 "./compiler.y"
    {
        Node *tmp = Query_Symbol ( (yyvsp[(1) - (3)].s_var) );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        printf ( "call: %s%s\n", tmp -> name, tmp -> func );
    }
    break;

  case 134:
#line 602 "./compiler.y"
    {
        Node *tmp = Query_Symbol ( (yyvsp[(1) - (4)].s_var) );
        printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
        printf ( "call: %s%s\n", tmp -> name, tmp -> func );
    }
    break;

  case 138:
#line 616 "./compiler.y"
    {
        printf ( "INT_LIT %d\n", (yyvsp[(1) - (1)].i_var) );
    }
    break;

  case 139:
#line 619 "./compiler.y"
    {
        Node *tmp = Query_Symbol ( (yyvsp[(1) - (1)].s_var) );
        if ( tmp )
            printf ( "IDENT (name=%s, address=%d)\n", tmp -> name, tmp -> addr );
    }
    break;


/* Line 1267 of yacc.c.  */
#line 2502 "./build/y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 626 "./compiler.y"

/* C code section */

