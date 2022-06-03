/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton implementation for Bison GLR parsers in C
   
      Copyright (C) 2002-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* C GLR parser skeleton written by Paul Hilfinger.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "glr.c"

/* Pure parsers.  */
#define YYPURE 0






/* Copy the first part of user declarations.  */
/* Line 207 of glr.c  */
#line 1 "han_compiler.y"

	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	#include<iostream>

	#include "han_compiler.hpp"

	int yylex(void);

	void yyerror(const char *);
	int pretreate();
	using namespace std;
	extern const char* yytext;
	//#define YYSTYPE char[100]
	#define YYERROR_VERBOSE
	#define YYDEBUG 1
	char* stradd(char* dest,const char* src1,const char* src2){
		if(dest!=src1){
			return strcat(dest,src2);
		}
		if(dest!=src2){
			return strcat(dest,src1);
		}
		return strcat(strcat(dest,src1),src2);
	}

	int lineNumber = 1;

	void	setCurrentAssemblyCode();//C++ÊµÏÖ



/* Line 207 of glr.c  */
#line 90 "y.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#include "y.tab.h"

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Default (constant) value used for initialization for null
   right-hand sides.  Unlike the standard yacc.c template, here we set
   the default value of $$ to a zeroed-out value.  Since the default
   value is undefined, this behavior is technically correct.  */
static YYSTYPE yyval_default;

/* Copy the second part of user declarations.  */

/* Line 230 of glr.c  */
#line 119 "y.tab.c"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
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

#ifndef YYFREE
# define YYFREE free
#endif
#ifndef YYMALLOC
# define YYMALLOC malloc
#endif
#ifndef YYREALLOC
# define YYREALLOC realloc
#endif

#define YYSIZEMAX ((size_t) -1)

#ifdef __cplusplus
   typedef bool yybool;
#else
   typedef unsigned char yybool;
#endif
#define yytrue 1
#define yyfalse 0

#ifndef YYSETJMP
# include <setjmp.h>
# define YYJMP_BUF jmp_buf
# define YYSETJMP(Env) setjmp (Env)
/* Pacify clang.  */
# define YYLONGJMP(Env, Val) (longjmp (Env, Val), YYASSERT (0))
#endif

/*-----------------.
| GCC extensions.  |
`-----------------*/

#ifndef __attribute__
/* This feature is available in gcc versions 2.5 and later.  */
# if (! defined __GNUC__ || __GNUC__ < 2 \
      || (__GNUC__ == 2 && __GNUC_MINOR__ < 5))
#  define __attribute__(Spec) /* empty */
# endif
#endif

#ifndef YYASSERT
# define YYASSERT(Condition) ((void) ((Condition) || (abort (), 0)))
#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  57
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   889

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  46
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  107
/* YYNRULES -- Number of states.  */
#define YYNSTATES  187
/* YYMAXRHS -- Maximum number of symbols on right-hand side of rule.  */
#define YYMAXRHS 6
/* YYMAXLEFT -- Maximum number of symbols to the left of a handle
   accessed by $0, $-1, etc., in any rule.  */
#define YYMAXLEFT 0

/* YYTRANSLATE(X) -- Bison symbol number corresponding to X.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   300

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      45
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    16,    18,
      21,    24,    28,    29,    31,    34,    37,    41,    45,    49,
      53,    57,    61,    65,    69,    73,    77,    81,    85,    89,
      93,    98,   103,   105,   108,   111,   114,   116,   118,   121,
     125,   127,   129,   131,   134,   137,   140,   143,   146,   149,
     152,   156,   160,   164,   168,   172,   176,   180,   184,   188,
     192,   196,   200,   204,   209,   214,   216,   219,   222,   225,
     229,   232,   236,   239,   240,   241,   244,   247,   251,   255,
     258,   261,   264,   268,   272,   276,   278,   280,   283,   287,
     291,   295,   299,   301,   303,   306,   312,   318,   325,   332,
     336,   340,   344,   348,   350,   352,   354,   360
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const signed char yyrhs[] =
{
      47,     0,    -1,    48,    -1,    47,    48,    -1,    68,    -1,
      72,    -1,    55,    -1,    52,    -1,    18,    -1,    43,    18,
      -1,    42,    18,    -1,    16,    41,    49,    -1,    -1,    49,
      -1,    32,    51,    -1,    33,    51,    -1,    51,    44,    51,
      -1,    51,    33,    51,    -1,    51,    32,    51,    -1,    51,
      35,    51,    -1,    51,    34,    51,    -1,    51,    36,    51,
      -1,    51,    28,    51,    -1,    51,    31,    51,    -1,    51,
      30,    51,    -1,    51,    29,    51,    -1,    51,    41,    51,
      -1,    51,    24,    51,    -1,    51,    27,    51,    -1,    39,
      51,    38,    -1,    51,    39,    50,    38,    -1,    51,    39,
      51,    38,    -1,     7,    -1,    51,     7,    -1,    67,     7,
      -1,    71,     7,    -1,    68,    -1,    72,    -1,    19,     7,
      -1,    19,    51,     7,    -1,    76,    -1,    77,    -1,     5,
      -1,    53,    52,    -1,    54,    52,    -1,    54,    55,    -1,
      54,     6,    -1,    53,     6,    -1,    32,    51,    -1,    33,
      51,    -1,    51,    33,    51,    -1,    51,    32,    51,    -1,
      51,    35,    51,    -1,    51,    34,    51,    -1,    51,    36,
      51,    -1,    51,    28,    51,    -1,    51,    31,    51,    -1,
      51,    30,    51,    -1,    51,    29,    51,    -1,    51,    41,
      51,    -1,    51,    24,    51,    -1,    51,    27,    51,    -1,
      39,    56,    38,    -1,    51,    39,    50,    38,    -1,    51,
      39,    51,    38,    -1,     5,    -1,    57,    52,    -1,    58,
      52,    -1,    57,    56,    -1,    57,    56,     7,    -1,    58,
      56,    -1,    58,    56,     7,    -1,    59,     6,    -1,    -1,
      -1,    61,    56,    -1,    63,    62,    -1,    13,    41,    64,
      -1,    13,    41,    60,    -1,    13,    60,    -1,    13,    64,
      -1,    13,    65,    -1,    13,    41,    65,    -1,    65,    26,
      18,    -1,    66,    24,    18,    -1,    65,    -1,    66,    -1,
      67,    55,    -1,    68,    25,    67,    -1,    14,    41,    18,
      -1,    69,    26,    18,    -1,    70,    24,    18,    -1,    69,
      -1,    70,    -1,    71,    55,    -1,     8,    39,    51,    38,
      64,    -1,     8,    39,    51,    38,    55,    -1,    73,    11,
      39,    51,    38,    64,    -1,    73,    11,    39,    51,    38,
      55,    -1,    73,     9,    64,    -1,    73,     9,    55,    -1,
      74,     9,    64,    -1,    74,     9,    55,    -1,    73,    -1,
      74,    -1,    75,    -1,    12,    39,    51,    38,    64,    -1,
      12,    39,    51,    38,    55,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    97,    97,    98,   102,   103,   104,   105,   108,   109,
     110,   111,   114,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   151,   154,   155,   160,   163,   164,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   192,   195,   196,   198,   199,
     200,   201,   203,   205,   208,   212,   216,   221,   222,   223,
     224,   225,   226,   228,   229,   231,   232,   234,   235,   238,
     240,   241,   245,   246,   248,   250,   251,   253,   254,   256,
     257,   258,   259,   261,   262,   263,   266,   267
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LMCUR", "RMCUR", "LBCUR", "RBCUR",
  "SEMICOLON", "IF", "ELSE", "ELSEIF", "ELIF", "WHILE",
  "function_declare_attribute", "system_declare_attribute",
  "changeable_attribute", "variable_specific_attribute", "attribute",
  "arg", "_return_", "LOWEST", "LOW", "_return_low", "_return_high",
  "COMMA", "TUIDAO", "JICHENG", "SHUCHU", "FUZHI", "XIAOYU", "DAYU",
  "DENGYU", "JIAN", "JIA", "CHU", "CHENG", "MI", "NEG", "RSCUR", "LSCUR",
  "CALL", "DOTDOT", "PENDCHANGEABLE", "CHANGEABLE", "DOT", "HIGHEST",
  "$accept", "code", "environment", "_arg_", "expression_null",
  "expression", "sentence", "scopeBegin", "scopeHalf", "scope",
  "expressionFunctionDeclareRoot", "scopeFunctionDeclareBegin",
  "scopeFunctionDeclareHalf", "scopeFunctionDeclareEnd",
  "scopeFunctionDeclare", "autoScopeBegin", "autoScopeEnd",
  "autoScopeHalf", "autoScope", "functionDeclareBegin",
  "functionDeclareHalf", "functionDeclare", "functionImplement",
  "systemDeclareBegin", "systemDeclareHalf", "systemDeclare",
  "systemImplement", "branchBegin", "branchHalf", "branchEnd", "branch",
  "loop", YY_NULL
};
#endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    46,    47,    47,    48,    48,    48,    48,    49,    49,
      49,    49,    50,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    53,    54,    54,    54,    55,    55,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    57,    58,    58,    59,    59,
      59,    59,    60,    61,    62,    63,    64,    65,    65,    65,
      65,    65,    65,    66,    66,    67,    67,    68,    68,    69,
      70,    70,    71,    71,    72,    73,    73,    74,    74,    75,
      75,    75,    75,    76,    76,    76,    77,    77
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     2,
       2,     3,     0,     1,     2,     2,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       4,     4,     1,     2,     2,     2,     1,     1,     2,     3,
       1,     1,     1,     2,     2,     2,     2,     2,     2,     2,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     1,     2,     2,     2,     3,
       2,     3,     2,     0,     0,     2,     2,     3,     3,     2,
       2,     2,     3,     3,     3,     1,     1,     2,     3,     3,
       3,     3,     1,     1,     2,     5,     5,     6,     6,     3,
       3,     3,     3,     1,     1,     1,     5,     5
};

/* YYDPREC[RULE-NUM] -- Dynamic precedence of rule #RULE-NUM (0 if none).  */
static const unsigned char yydprec[] =
{
       0,     0,     1,     2,     5,     5,     3,     2,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     1,     5,     2,     2,     2,     2,     3,     4,
       0,     0,     0,     0,     0,     3,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     1,     2,
       3,     4,     0,     0,     1,     1,     1,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0
};

/* YYMERGER[RULE-NUM] -- Index of merging function for rule #RULE-NUM.  */
static const unsigned char yymerger[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0
};

/* YYDEFACT[S] -- default reduction number in state S.  Performed when
   YYTABLE doesn't specify something else to do.  Zero means the default
   is an error.  */
static const unsigned char yydefact[] =
{
       0,    42,    32,     0,     0,    73,     0,     0,     8,     0,
       0,     0,     0,     0,     0,     0,     2,    13,     0,     7,
       0,     0,     6,    85,    86,     0,     4,    92,    93,     0,
       5,   103,   104,   105,    40,    41,     0,     0,    65,    73,
       0,     0,     0,    79,     0,    74,    80,    81,     0,     0,
      38,     0,    14,    15,     0,    10,     9,     1,     3,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    12,     0,     0,    47,    43,    36,    37,    46,    44,
      45,     0,     0,    34,    87,     0,     0,     0,    35,    94,
      73,     0,    73,     0,     0,    78,    77,    82,     0,     0,
       0,     0,    66,    68,    67,    70,    72,     0,    75,    76,
      89,    11,    39,    29,    27,    28,    22,    25,    24,    23,
      18,    17,    20,    19,    21,     0,     0,    26,    16,    83,
      84,    88,    90,    91,   100,    99,     0,   102,   101,    73,
      73,    48,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    12,     0,    69,    71,
      30,    31,     0,    96,    95,   107,   106,    62,    60,    61,
      55,    58,    57,    56,    51,    50,    53,    52,    54,     0,
       0,    59,    73,    30,    31,    98,    97
};

/* YYPDEFGOTO[NTERM-NUM].  */
static const signed char yydefgoto[] =
{
      -1,    15,    16,    17,   125,    18,    19,    20,    21,    22,
     103,    40,    41,    42,    43,    44,   109,    45,    46,    23,
      24,    25,    76,    27,    28,    29,    77,    31,    32,    33,
      34,    35
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -40
static const short int yypact[] =
{
     281,   -40,   -40,   -36,   -23,    29,   -18,   -11,   -40,   649,
     193,   193,   193,    15,    22,   237,   -40,   -40,   544,   -40,
     619,    88,   -40,    20,    23,    58,   215,    24,    40,    67,
     259,   102,    82,   -40,   -40,   -40,   193,   193,   -40,    46,
     641,   641,    86,   -40,   654,   -40,   -40,   -40,    55,    25,
     -40,   562,    64,    64,   671,   -40,   -40,   -40,   -40,   -40,
     193,   193,   193,   193,   193,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   -40,   -40,    72,   -40,   -40,   -40,
     -40,    80,    81,   -40,   -40,   101,   104,   107,   -40,   -40,
     118,    85,   118,   689,   707,   -40,   -40,   -40,   193,   193,
     654,   580,   -40,   119,   -40,   121,   -40,   797,   -40,   -40,
     -40,   -40,   -40,   -40,   815,   815,   832,   845,   845,   845,
     310,   310,   106,   106,    64,   110,   725,   -39,   -40,   -40,
     -40,   -40,   -40,   -40,   -40,   -40,   193,   -40,   -40,   118,
     118,   105,   298,   743,   113,   193,   193,   193,   193,   193,
     193,   193,   193,   193,   193,   193,   193,   193,   -40,   -40,
     -40,   -40,   761,   -40,   -40,   -40,   -40,   -40,   328,   346,
     364,   382,   400,   418,   436,   454,   472,   490,   508,   116,
     779,   526,   118,     6,   175,   -40,   -40
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
     -40,   -40,   140,   108,     0,    17,    35,   -40,   -40,   -21,
     -34,   -40,   -40,   -40,   120,   -40,   -40,   -40,   -30,    -4,
     -40,    73,    21,   -40,   -40,   -40,    37,   -40,   -40,   -40,
     -40,   -40
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -65
static const short int yytable[] =
{
      80,    47,    72,    36,    84,    73,   -63,   105,    89,    96,
     108,   -63,   -63,   -30,   -63,   -63,    37,   -63,   -63,   -63,
     -63,    26,   -63,    48,   -63,   -63,    51,    52,    53,    54,
      49,   -63,   -63,    55,    38,    97,    26,    30,   -30,   -30,
      56,     7,     5,     8,   -30,   -30,    81,    82,   -63,   -63,
      86,    38,    30,    93,    94,    75,    79,   101,   101,     5,
     135,   107,   138,     1,    87,    83,   144,    13,    14,   134,
      39,   137,     1,   110,    88,   102,   104,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   126,   127,
     128,    92,   106,     1,    78,     2,     3,    85,   129,   130,
       4,     5,     6,    71,     7,    72,     8,     9,    73,   164,
     166,    90,   -14,    91,     5,   141,   142,   143,   163,   165,
      10,    11,   132,     1,   136,   133,   158,    12,   159,   -14,
      13,    14,   -14,   -14,   -14,   -14,   -14,   -14,   -14,   -14,
     -14,   -14,    70,   -14,    71,    71,    72,    72,   160,    73,
      73,   167,   186,   162,   183,    58,   179,   111,   131,    95,
       0,   185,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   180,   181,   -64,     0,     0,     0,     0,
     -64,   -64,   -31,   -64,   -64,     0,   -64,   -64,   -64,   -64,
       0,   -64,     0,   -64,   -64,     0,     0,     0,     0,     0,
     -64,   -64,     0,     0,     0,     0,     0,   -31,   -31,     7,
       0,     8,     0,   -31,   -31,    -4,     0,   -64,   -64,     0,
      -4,     0,    -4,    -4,     0,    10,    11,    -4,    -4,    -4,
       0,    -4,    12,    -4,    -4,    13,    14,    57,     0,     0,
      85,     0,     1,     0,     2,     3,     0,    -4,    -4,     4,
       5,     6,     0,     7,    -4,     8,     9,    -4,    -4,    -5,
       0,     0,     0,     0,    -5,     0,    -5,    -5,     0,    10,
      11,    -5,    -5,    -5,     0,    -5,    12,    -5,    -5,    13,
      14,     0,     0,     0,     0,     0,     1,     0,     2,     3,
       0,    -5,    -5,     4,     5,     6,     0,     7,    -5,     8,
       9,    -5,    -5,     0,     0,   -15,     0,     0,     0,     0,
       0,     0,     0,    10,    11,     0,     0,     0,     0,     0,
      12,     0,   -15,    13,    14,   -15,   -15,   -15,   -15,   -15,
     -15,   -15,   -15,   -15,   -15,   -27,   -15,    71,     0,    72,
       0,     0,    73,     0,    68,    69,    70,     0,     0,    71,
       0,    72,   -27,   -28,    73,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,     0,   -27,    71,     0,    72,
     -28,   -22,    73,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,     0,   -28,    71,     0,    72,   -22,   -25,
      73,   -22,    62,    63,    64,    65,    66,    67,    68,    69,
      70,     0,   -22,    71,     0,    72,   -25,   -24,    73,   -25,
     -25,   -25,   -25,   -25,    66,    67,    68,    69,    70,     0,
     -25,    71,     0,    72,   -24,   -23,    73,   -24,   -24,   -24,
     -24,   -24,    66,    67,    68,    69,    70,     0,   -24,    71,
       0,    72,   -23,   -18,    73,   -23,   -23,   -23,   -23,   -23,
      66,    67,    68,    69,    70,     0,   -23,    71,     0,    72,
     -18,   -17,    73,   -18,   -18,   -18,   -18,   -18,   -18,   -18,
      68,    69,    70,     0,   -18,    71,     0,    72,   -17,   -20,
      73,   -17,   -17,   -17,   -17,   -17,   -17,   -17,    68,    69,
      70,     0,   -17,    71,     0,    72,   -20,   -19,    73,   -20,
     -20,   -20,   -20,   -20,   -20,   -20,   -20,   -20,    70,     0,
     -20,    71,     0,    72,   -19,   -21,    73,   -19,   -19,   -19,
     -19,   -19,   -19,   -19,   -19,   -19,    70,     0,   -19,    71,
       0,    72,   -21,   -26,    73,   -21,   -21,   -21,   -21,   -21,
     -21,   -21,   -21,   -21,   -21,     0,   -21,    71,     0,    72,
     -26,    59,    73,   -26,   -26,   -26,   -26,   -26,   -26,   -26,
     -26,   -26,   -26,     0,   -26,   -26,     0,    72,    60,   112,
      73,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,     0,     0,    71,     0,    72,    60,    59,    73,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,     0,
       0,    71,     0,    72,   145,     0,    73,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,     0,     0,   156,
       0,   157,     0,     0,    73,    74,     2,     3,     0,     0,
       0,     4,     5,     6,     0,     7,     0,     8,     9,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     2,     3,
       0,    10,    11,     4,     5,     6,    50,     7,    12,     8,
       9,    13,    14,     0,     0,     7,     0,     8,     0,     0,
       7,     0,     8,    98,    99,     0,     0,     0,     0,     0,
     100,    10,    11,    13,    14,     0,    98,    99,    12,     0,
       0,    13,    14,   100,     0,    60,    13,    14,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,     0,   113,
      71,     0,    72,    60,     0,    73,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,     0,   139,    71,     0,
      72,    60,     0,    73,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,     0,   140,    71,     0,    72,    60,
       0,    73,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,     0,   161,    71,     0,    72,   145,     0,    73,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
       0,   113,   156,     0,   157,    60,     0,    73,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,     0,   182,
      71,     0,    72,    60,     0,    73,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,     0,   184,    71,     0,
      72,   145,     0,    73,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,     0,     0,   156,     0,   157,     0,
       0,    73,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,     0,     0,    71,     0,    72,     0,     0,    73,
      62,    63,    64,    65,    66,    67,    68,    69,    70,     0,
       0,    71,     0,    72,     0,     0,    73,    66,    67,    68,
      69,    70,     0,     0,    71,     0,    72,     0,     0,    73
};

/* YYCONFLP[YYPACT[STATE-NUM]] -- Pointer into YYCONFL of start of
   list of conflicting reductions corresponding to action entry for
   state STATE-NUM in yytable.  0 means no conflicts.  The list in
   yyconfl is terminated by a rule number of 0.  */
static const unsigned char yyconflp[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   151,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   153,   155,
       0,     0,     0,     0,   157,   159,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    63,    65,     0,
       0,     0,     0,    67,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   161,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   163,   165,     0,
       0,     0,     0,   167,   169,     1,     0,     0,     0,     0,
       3,     0,     5,     7,     0,     0,     0,     9,    11,    13,
       0,    15,     0,    17,    19,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    21,    23,     0,
       0,     0,     0,     0,    25,     0,     0,    27,    29,    31,
       0,     0,     0,     0,    33,     0,    35,    37,     0,     0,
       0,    39,    41,    43,     0,    45,     0,    47,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    51,    53,     0,     0,     0,     0,     0,    55,     0,
       0,    57,    59,     0,     0,    69,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      71,    73,     0,     0,     0,    77,    75,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    81,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    79,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,     0,     0,     0,     0,    89,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,     0,     0,     0,     0,    93,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      91,     0,     0,     0,     0,    97,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    95,     0,
       0,     0,     0,   101,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    99,     0,     0,     0,
       0,   109,     0,     0,     0,     0,     0,     0,   103,   105,
       0,     0,     0,     0,   107,     0,     0,     0,     0,   117,
       0,     0,     0,     0,     0,     0,   111,   113,     0,     0,
       0,     0,   115,     0,     0,     0,     0,   125,     0,     0,
       0,     0,     0,     0,   119,   121,     0,     0,     0,     0,
     123,     0,     0,     0,     0,   133,     0,     0,     0,     0,
       0,     0,   127,   129,     0,     0,     0,     0,   131,     0,
       0,     0,     0,   141,     0,     0,     0,     0,     0,     0,
     135,   137,     0,     0,     0,     0,   139,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   143,   145,
       0,     0,     0,     0,   147,   149,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0
};

/* YYCONFL[I] -- lists of conflicting rule numbers, each terminated by
   0, pointed into by YYCONFLP.  */
static const short int yyconfl[] =
{
       0,    36,     0,    36,     0,    36,     0,    36,     0,    36,
       0,    36,     0,    36,     0,    36,     0,    36,     0,    36,
       0,    36,     0,    36,     0,    36,     0,    36,     0,    36,
       0,    37,     0,    37,     0,    37,     0,    37,     0,    37,
       0,    37,     0,    37,     0,    37,     0,    37,     0,    37,
       0,    37,     0,    37,     0,    37,     0,    37,     0,    37,
       0,    48,     0,    48,     0,    48,     0,    48,     0,    49,
       0,    49,     0,    49,     0,    49,     0,    60,     0,    60,
       0,    61,     0,    61,     0,    55,     0,    55,     0,    58,
       0,    58,     0,    57,     0,    57,     0,    56,     0,    56,
       0,    51,     0,    51,     0,    51,     0,    51,     0,    50,
       0,    50,     0,    50,     0,    50,     0,    53,     0,    53,
       0,    53,     0,    53,     0,    52,     0,    52,     0,    52,
       0,    52,     0,    54,     0,    54,     0,    54,     0,    54,
       0,    59,     0,    59,     0,    59,     0,    59,     0,    59,
       0,    63,     0,    63,     0,    63,     0,    63,     0,    63,
       0,    64,     0,    64,     0,    64,     0,    64,     0,    64,
       0
};

static const short int yycheck[] =
{
      21,     5,    41,    39,    25,    44,     0,    41,    29,    39,
      44,     5,     6,     7,     8,     9,    39,    11,    12,    13,
      14,     0,    16,    41,    18,    19,     9,    10,    11,    12,
      41,    25,    26,    18,     5,    39,    15,     0,    32,    33,
      18,    16,    13,    18,    38,    39,    26,    24,    42,    43,
      26,     5,    15,    36,    37,    20,    21,    40,    41,    13,
      90,    44,    92,     5,    24,     7,   100,    42,    43,    90,
      41,    92,     5,    18,     7,    40,    41,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,     9,     6,     5,     6,     7,     8,    25,    18,    18,
      12,    13,    14,    39,    16,    41,    18,    19,    44,   139,
     140,     9,     7,    11,    13,    98,    99,   100,   139,   140,
      32,    33,    18,     5,    39,    18,     7,    39,     7,    24,
      42,    43,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    36,    38,    39,    39,    41,    41,    38,    44,
      44,    38,   182,   136,    38,    15,   156,    49,    85,    39,
      -1,   182,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,     0,    -1,    -1,    -1,    -1,
       5,     6,     7,     8,     9,    -1,    11,    12,    13,    14,
      -1,    16,    -1,    18,    19,    -1,    -1,    -1,    -1,    -1,
      25,    26,    -1,    -1,    -1,    -1,    -1,    32,    33,    16,
      -1,    18,    -1,    38,    39,     0,    -1,    42,    43,    -1,
       5,    -1,     7,     8,    -1,    32,    33,    12,    13,    14,
      -1,    16,    39,    18,    19,    42,    43,     0,    -1,    -1,
      25,    -1,     5,    -1,     7,     8,    -1,    32,    33,    12,
      13,    14,    -1,    16,    39,    18,    19,    42,    43,     0,
      -1,    -1,    -1,    -1,     5,    -1,     7,     8,    -1,    32,
      33,    12,    13,    14,    -1,    16,    39,    18,    19,    42,
      43,    -1,    -1,    -1,    -1,    -1,     5,    -1,     7,     8,
      -1,    32,    33,    12,    13,    14,    -1,    16,    39,    18,
      19,    42,    43,    -1,    -1,     7,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    32,    33,    -1,    -1,    -1,    -1,    -1,
      39,    -1,    24,    42,    43,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,     7,    38,    39,    -1,    41,
      -1,    -1,    44,    -1,    34,    35,    36,    -1,    -1,    39,
      -1,    41,    24,     7,    44,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      24,     7,    44,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    24,     7,
      44,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    -1,    41,    24,     7,    44,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    41,    24,     7,    44,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    24,     7,    44,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      24,     7,    44,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    24,     7,
      44,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    38,    39,    -1,    41,    24,     7,    44,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      38,    39,    -1,    41,    24,     7,    44,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    38,    39,
      -1,    41,    24,     7,    44,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    -1,    38,    39,    -1,    41,
      24,     7,    44,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    -1,    38,    39,    -1,    41,    24,     7,
      44,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    -1,    -1,    39,    -1,    41,    24,     7,    44,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    41,    24,    -1,    44,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    -1,    -1,    39,
      -1,    41,    -1,    -1,    44,     6,     7,     8,    -1,    -1,
      -1,    12,    13,    14,    -1,    16,    -1,    18,    19,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,     7,     8,
      -1,    32,    33,    12,    13,    14,     7,    16,    39,    18,
      19,    42,    43,    -1,    -1,    16,    -1,    18,    -1,    -1,
      16,    -1,    18,    32,    33,    -1,    -1,    -1,    -1,    -1,
      39,    32,    33,    42,    43,    -1,    32,    33,    39,    -1,
      -1,    42,    43,    39,    -1,    24,    42,    43,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    24,    -1,    44,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    24,    -1,    44,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    38,    39,    -1,    41,    24,
      -1,    44,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    38,    39,    -1,    41,    24,    -1,    44,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      -1,    38,    39,    -1,    41,    24,    -1,    44,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    -1,    38,
      39,    -1,    41,    24,    -1,    44,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    -1,    38,    39,    -1,
      41,    24,    -1,    44,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    -1,    -1,    39,    -1,    41,    -1,
      -1,    44,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    41,    -1,    -1,    44,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    -1,
      -1,    39,    -1,    41,    -1,    -1,    44,    32,    33,    34,
      35,    36,    -1,    -1,    39,    -1,    41,    -1,    -1,    44
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     5,     7,     8,    12,    13,    14,    16,    18,    19,
      32,    33,    39,    42,    43,    47,    48,    49,    51,    52,
      53,    54,    55,    65,    66,    67,    68,    69,    70,    71,
      72,    73,    74,    75,    76,    77,    39,    39,     5,    41,
      57,    58,    59,    60,    61,    63,    64,    65,    41,    41,
       7,    51,    51,    51,    51,    18,    18,     0,    48,     7,
      24,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    39,    41,    44,     6,    52,    68,    72,     6,    52,
      55,    26,    24,     7,    55,    25,    26,    24,     7,    55,
       9,    11,     9,    51,    51,    60,    64,    65,    32,    33,
      39,    51,    52,    56,    52,    56,     6,    51,    56,    62,
      18,    49,     7,    38,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    50,    51,    51,    51,    18,
      18,    67,    18,    18,    55,    64,    39,    55,    64,    38,
      38,    51,    51,    51,    56,    24,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    39,    41,     7,     7,
      38,    38,    51,    55,    64,    55,    64,    38,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    50,
      51,    51,    38,    38,    38,    55,    64
};

/* Error token number */
#define YYTERROR 1


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#define YYLEX yylex ()

YYSTYPE yylval;

int yynerrs;
int yychar;

static const int YYEOF = 0;
static const int YYEMPTY = -2;

typedef enum { yyok, yyaccept, yyabort, yyerr } YYRESULTTAG;

#define YYCHK(YYE)                                                           \
   do { YYRESULTTAG yyflag = YYE; if (yyflag != yyok) return yyflag; }       \
   while (YYID (0))

#if YYDEBUG

# ifndef YYFPRINTF
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
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

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)          \
do {                                                            \
  if (yydebug)                                                  \
    {                                                           \
      YYFPRINTF (stderr, "%s ", Title);                         \
      yy_symbol_print (stderr, Type, Value);        \
      YYFPRINTF (stderr, "\n");                                 \
    }                                                           \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;

#else /* !YYDEBUG */

# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)

#endif /* !YYDEBUG */

/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   SIZE_MAX < YYMAXDEPTH * sizeof (GLRStackItem)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif

/* Minimum number of free items on the stack allowed after an
   allocation.  This is to allow allocation and initialization
   to be completed by functions that call yyexpandGLRStack before the
   stack is expanded, thus insuring that all necessary pointers get
   properly redirected to new data.  */
#define YYHEADROOM 2

#ifndef YYSTACKEXPANDABLE
# if (! defined __cplusplus \
      || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL))
#  define YYSTACKEXPANDABLE 1
# else
#  define YYSTACKEXPANDABLE 0
# endif
#endif

#if YYSTACKEXPANDABLE
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyexpandGLRStack (Yystack);                       \
  } while (YYID (0))
#else
# define YY_RESERVE_GLRSTACK(Yystack)                   \
  do {                                                  \
    if (Yystack->yyspaceLeft < YYHEADROOM)              \
      yyMemoryExhausted (Yystack);                      \
  } while (YYID (0))
#endif


#if YYERROR_VERBOSE

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static size_t
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
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
    return strlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* !YYERROR_VERBOSE */

/** State numbers, as in LALR(1) machine */
typedef int yyStateNum;

/** Rule numbers, as in LALR(1) machine */
typedef int yyRuleNum;

/** Grammar symbol */
typedef short int yySymbol;

/** Item references, as in LALR(1) machine */
typedef short int yyItemNum;

typedef struct yyGLRState yyGLRState;
typedef struct yyGLRStateSet yyGLRStateSet;
typedef struct yySemanticOption yySemanticOption;
typedef union yyGLRStackItem yyGLRStackItem;
typedef struct yyGLRStack yyGLRStack;

struct yyGLRState {
  /** Type tag: always true.  */
  yybool yyisState;
  /** Type tag for yysemantics.  If true, yysval applies, otherwise
   *  yyfirstVal applies.  */
  yybool yyresolved;
  /** Number of corresponding LALR(1) machine state.  */
  yyStateNum yylrState;
  /** Preceding state in this stack */
  yyGLRState* yypred;
  /** Source position of the first token produced by my symbol */
  size_t yyposn;
  union {
    /** First in a chain of alternative reductions producing the
     *  non-terminal corresponding to this state, threaded through
     *  yynext.  */
    yySemanticOption* yyfirstVal;
    /** Semantic value for this state.  */
    YYSTYPE yysval;
  } yysemantics;
};

struct yyGLRStateSet {
  yyGLRState** yystates;
  /** During nondeterministic operation, yylookaheadNeeds tracks which
   *  stacks have actually needed the current lookahead.  During deterministic
   *  operation, yylookaheadNeeds[0] is not maintained since it would merely
   *  duplicate yychar != YYEMPTY.  */
  yybool* yylookaheadNeeds;
  size_t yysize, yycapacity;
};

struct yySemanticOption {
  /** Type tag: always false.  */
  yybool yyisState;
  /** Rule number for this reduction */
  yyRuleNum yyrule;
  /** The last RHS state in the list of states to be reduced.  */
  yyGLRState* yystate;
  /** The lookahead for this reduction.  */
  int yyrawchar;
  YYSTYPE yyval;
  /** Next sibling in chain of options.  To facilitate merging,
   *  options are chained in decreasing order by address.  */
  yySemanticOption* yynext;
};

/** Type of the items in the GLR stack.  The yyisState field
 *  indicates which item of the union is valid.  */
union yyGLRStackItem {
  yyGLRState yystate;
  yySemanticOption yyoption;
};

struct yyGLRStack {
  int yyerrState;


  YYJMP_BUF yyexception_buffer;
  yyGLRStackItem* yyitems;
  yyGLRStackItem* yynextFree;
  size_t yyspaceLeft;
  yyGLRState* yysplitPoint;
  yyGLRState* yylastDeleted;
  yyGLRStateSet yytops;
};

#if YYSTACKEXPANDABLE
static void yyexpandGLRStack (yyGLRStack* yystackp);
#endif

static void yyFail (yyGLRStack* yystackp, const char* yymsg)
  __attribute__ ((__noreturn__));
static void
yyFail (yyGLRStack* yystackp, const char* yymsg)
{
  if (yymsg != YY_NULL)
    yyerror (yymsg);
  YYLONGJMP (yystackp->yyexception_buffer, 1);
}

static void yyMemoryExhausted (yyGLRStack* yystackp)
  __attribute__ ((__noreturn__));
static void
yyMemoryExhausted (yyGLRStack* yystackp)
{
  YYLONGJMP (yystackp->yyexception_buffer, 2);
}

#if YYDEBUG || YYERROR_VERBOSE
/** A printable representation of TOKEN.  */
static inline const char*
yytokenName (yySymbol yytoken)
{
  if (yytoken == YYEMPTY)
    return "";

  return yytname[yytoken];
}
#endif

/** Fill in YYVSP[YYLOW1 .. YYLOW0-1] from the chain of states starting
 *  at YYVSP[YYLOW0].yystate.yypred.  Leaves YYVSP[YYLOW1].yystate.yypred
 *  containing the pointer to the next state in the chain.  */
static void yyfillin (yyGLRStackItem *, int, int) __attribute__ ((__unused__));
static void
yyfillin (yyGLRStackItem *yyvsp, int yylow0, int yylow1)
{
  int i;
  yyGLRState *s = yyvsp[yylow0].yystate.yypred;
  for (i = yylow0-1; i >= yylow1; i -= 1)
    {
      YYASSERT (s->yyresolved);
      yyvsp[i].yystate.yyresolved = yytrue;
      yyvsp[i].yystate.yysemantics.yysval = s->yysemantics.yysval;
      s = yyvsp[i].yystate.yypred = s->yypred;
    }
}

/* Do nothing if YYNORMAL or if *YYLOW <= YYLOW1.  Otherwise, fill in
 * YYVSP[YYLOW1 .. *YYLOW-1] as in yyfillin and set *YYLOW = YYLOW1.
 * For convenience, always return YYLOW1.  */
static inline int yyfill (yyGLRStackItem *, int *, int, yybool)
     __attribute__ ((__unused__));
static inline int
yyfill (yyGLRStackItem *yyvsp, int *yylow, int yylow1, yybool yynormal)
{
  if (!yynormal && yylow1 < *yylow)
    {
      yyfillin (yyvsp, *yylow, yylow1);
      *yylow = yylow1;
    }
  return yylow1;
}

/** Perform user action for rule number YYN, with RHS length YYRHSLEN,
 *  and top stack item YYVSP.  YYLVALP points to place to put semantic
 *  value ($$), and yylocp points to place for location information
 *  (@$).  Returns yyok for normal return, yyaccept for YYACCEPT,
 *  yyerr for YYERROR, yyabort for YYABORT.  */
/*ARGSUSED*/ static YYRESULTTAG
yyuserAction (yyRuleNum yyn, int yyrhslen, yyGLRStackItem* yyvsp,
              yyGLRStack* yystackp,
              YYSTYPE* yyvalp)
{
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == YY_NULL);
  int yylow;
# undef yyerrok
# define yyerrok (yystackp->yyerrState = 0)
# undef YYACCEPT
# define YYACCEPT return yyaccept
# undef YYABORT
# define YYABORT return yyabort
# undef YYERROR
# define YYERROR return yyerrok, yyerr
# undef YYRECOVERING
# define YYRECOVERING() (yystackp->yyerrState != 0)
# undef yyclearin
# define yyclearin (yychar = YYEMPTY)
# undef YYFILL
# define YYFILL(N) yyfill (yyvsp, &yylow, N, yynormal)
# undef YYBACKUP
# define YYBACKUP(Token, Value)                                              \
  return yyerror (YY_("syntax error: cannot back up")),     \
         yyerrok, yyerr

  yylow = 1;
  if (yyrhslen == 0)
    *yyvalp = yyval_default;
  else
    *yyvalp = yyvsp[YYFILL (1-yyrhslen)].yystate.yysemantics.yysval;
  switch (yyn)
    {
        case 2:
/* Line 868 of glr.c  */
#line 97 "han_compiler.y"
    {puts("yacc>>code:environment");matchRule(0, Rule::code___environment, NULL,NULL,NULL,NULL);}
    break;

  case 3:
/* Line 868 of glr.c  */
#line 98 "han_compiler.y"
    {puts("yacc>>code:code	environment");matchRule(0, Rule::code___code_environment, NULL,NULL,NULL,NULL);}
    break;

  case 4:
/* Line 868 of glr.c  */
#line 102 "han_compiler.y"
    {puts("yacc>>environment:functionImplement");matchRule(0, Rule::environment___functionImplement, NULL,NULL,NULL,NULL);}
    break;

  case 5:
/* Line 868 of glr.c  */
#line 103 "han_compiler.y"
    {puts("yacc>>environment:systemImplement");matchRule(0, Rule::environment___systemImplement, NULL,NULL,NULL,NULL);}
    break;

  case 6:
/* Line 868 of glr.c  */
#line 104 "han_compiler.y"
    {puts("yacc>>environment:scope");matchRule(0, Rule::environment___scope, NULL,NULL,NULL,NULL);}
    break;

  case 7:
/* Line 868 of glr.c  */
#line 105 "han_compiler.y"
    {puts("yacc>>environment:sentence");matchRule(0, Rule::environment___sentence, NULL,NULL,NULL,NULL);}
    break;

  case 8:
/* Line 868 of glr.c  */
#line 108 "han_compiler.y"
    {puts("yacc>>_arg_:arg");matchRule(0, Rule::_arg____arg, &(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 9:
/* Line 868 of glr.c  */
#line 109 "han_compiler.y"
    {puts("yacc>>_arg_:CHANGEABLE	arg");matchRule(1, Rule::_arg____CHANGEABLE_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 10:
/* Line 868 of glr.c  */
#line 110 "han_compiler.y"
    {puts("yacc>>_arg_:PENDCHANGEABLE	arg");matchRule(1, Rule::_arg____PENDCHANGEABLE_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 11:
/* Line 868 of glr.c  */
#line 111 "han_compiler.y"
    {puts("yacc>>_arg_:variable_specific_attribute	DOTDOT	_arg_");matchRule(1, Rule::_arg____variable_specific_attribute_DOTDOT__arg_, &(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 12:
/* Line 868 of glr.c  */
#line 114 "han_compiler.y"
    {puts("yacc>>expression_null:NULL");matchRule(0, Rule::expression_null___NULL, NULL,NULL,NULL,NULL);}
    break;

  case 13:
/* Line 868 of glr.c  */
#line 116 "han_compiler.y"
    {puts("yacc>>expression:_arg_");matchRule(0, Rule::expression____arg_,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 14:
/* Line 868 of glr.c  */
#line 117 "han_compiler.y"
    {puts("yacc>>expression:JIAN	expression");matchRule(0, Rule::expression___JIAN_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 15:
/* Line 868 of glr.c  */
#line 118 "han_compiler.y"
    {puts("yacc>>expression:JIA	expression");matchRule(0, Rule::expression___JIA_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 16:
/* Line 868 of glr.c  */
#line 119 "han_compiler.y"
    {puts("yacc>>expression:expression	DOT	expression");matchRule(1, Rule::expression___expression_DOT_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 17:
/* Line 868 of glr.c  */
#line 120 "han_compiler.y"
    {puts("yacc>>expression:expression	JIA	expression");matchRule(1, Rule::expression___expression_JIA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 18:
/* Line 868 of glr.c  */
#line 121 "han_compiler.y"
    {puts("yacc>>expression:expression	JIAN	expression");matchRule(1, Rule::expression___expression_JIAN_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 19:
/* Line 868 of glr.c  */
#line 122 "han_compiler.y"
    {puts("yacc>>expression:expression	CHENG	expression");matchRule(1, Rule::expression___expression_CHENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 20:
/* Line 868 of glr.c  */
#line 123 "han_compiler.y"
    {puts("yacc>>expression:expression	CHU	expression");matchRule(1, Rule::expression___expression_CHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 21:
/* Line 868 of glr.c  */
#line 124 "han_compiler.y"
    {puts("yacc>>expression:expression	MI	expression");matchRule(1, Rule::expression___expression_MI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 22:
/* Line 868 of glr.c  */
#line 125 "han_compiler.y"
    {puts("yacc>>expression:expression	FUZHI	expression");matchRule(1, Rule::expression___expression_FUZHI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 23:
/* Line 868 of glr.c  */
#line 126 "han_compiler.y"
    {puts("yacc>>expression:expression	DENGYU	expression");matchRule(1, Rule::expression___expression_DENGYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 24:
/* Line 868 of glr.c  */
#line 127 "han_compiler.y"
    {puts("yacc>>expression:expression	DAYU	expression");matchRule(1, Rule::expression___expression_DAYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 25:
/* Line 868 of glr.c  */
#line 128 "han_compiler.y"
    {puts("yacc>>expression:expression	XIAOYU	expression");matchRule(1, Rule::expression___expression_XIAOYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 26:
/* Line 868 of glr.c  */
#line 129 "han_compiler.y"
    {puts("yacc>>expression:expression	DOTDOT	expression");matchRule(1, Rule::expression___expression_DOTDOT_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 27:
/* Line 868 of glr.c  */
#line 130 "han_compiler.y"
    {puts("yacc>>expression:expression	COMMA	expression");matchRule(1, Rule::expression___expression_COMMA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 28:
/* Line 868 of glr.c  */
#line 131 "han_compiler.y"
    {puts("yacc>>expression:expression	SHUCHU	expression");matchRule(1, Rule::expression___expression_SHUCHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 29:
/* Line 868 of glr.c  */
#line 132 "han_compiler.y"
    {puts("yacc>>expression:LSCUR	expression	RSCUR");matchRule(0, Rule::expression___LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 30:
/* Line 868 of glr.c  */
#line 133 "han_compiler.y"
    {puts("yacc>>expression:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_null_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 31:
/* Line 868 of glr.c  */
#line 134 "han_compiler.y"
    {puts("yacc>>expression:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 32:
/* Line 868 of glr.c  */
#line 139 "han_compiler.y"
    {puts("yacc>>sentence:SEMICOLON");matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 33:
/* Line 868 of glr.c  */
#line 140 "han_compiler.y"
    {puts("yacc>>sentence:expression	SEMICOLON");matchRule(1, Rule::sentence___expression_SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 34:
/* Line 868 of glr.c  */
#line 141 "han_compiler.y"
    {puts("yacc>>sentence:functionDeclare	SEMICOLON");matchRule(1, Rule::sentence___functionDeclare_SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 35:
/* Line 868 of glr.c  */
#line 142 "han_compiler.y"
    {puts("yacc>>sentence:systemDeclare	SEMICOLON");matchRule(1, Rule::sentence___systemDeclare_SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 36:
/* Line 868 of glr.c  */
#line 143 "han_compiler.y"
    {puts("yacc>>sentence:functionImplement	SEMICOLON");matchRule(1, Rule::sentence___functionImplement,NULL,NULL,NULL,NULL);}
    break;

  case 37:
/* Line 868 of glr.c  */
#line 144 "han_compiler.y"
    {puts("yacc>>sentence:systemImplement	SEMICOLON");matchRule(1, Rule::sentence___systemImplement,NULL,NULL,NULL,NULL);}
    break;

  case 38:
/* Line 868 of glr.c  */
#line 145 "han_compiler.y"
    {puts("yacc>>sentence:_return_ SEMICOLON");matchRule(1, Rule::sentence____return__SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 39:
/* Line 868 of glr.c  */
#line 146 "han_compiler.y"
    {puts("yacc>>sentence:_return_	expression SEMICOLON");matchRule(1, Rule::sentence____return__expression_SEMICOLON,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,NULL);}
    break;

  case 40:
/* Line 868 of glr.c  */
#line 147 "han_compiler.y"
    {puts("yacc>>sentence:branch");matchRule(1, Rule::sentence___branch,NULL,NULL,NULL,NULL);}
    break;

  case 41:
/* Line 868 of glr.c  */
#line 148 "han_compiler.y"
    {puts("yacc>>sentence:loop");matchRule(1, Rule::sentence___loop,NULL,NULL,NULL,NULL);}
    break;

  case 42:
/* Line 868 of glr.c  */
#line 151 "han_compiler.y"
    {puts("yacc>>scopeBegin:LBCUR");matchRule(1, Rule::scopeBegin___LBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 43:
/* Line 868 of glr.c  */
#line 154 "han_compiler.y"
    {puts("yacc>>scopeHalf:scopeBegin	sentence");matchRule(0, Rule::scopeHalf___scopeBegin_sentence,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 44:
/* Line 868 of glr.c  */
#line 155 "han_compiler.y"
    {puts("yacc>>scopeHalf:scopeHalf	sentence");matchRule(0, Rule::scopeHalf___scopeHalf_sentence,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 45:
/* Line 868 of glr.c  */
#line 160 "han_compiler.y"
    {puts("yacc>>scopeHalf:scopeHalf	scope");matchRule(0, Rule::scopeHalf___scopeHalf_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 46:
/* Line 868 of glr.c  */
#line 163 "han_compiler.y"
    {puts("yacc>>scope:scopeHalf	RBCUR");matchRule(1, Rule::scope___scopeHalf_RBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 47:
/* Line 868 of glr.c  */
#line 164 "han_compiler.y"
    {puts("yacc>>scope:scopeBegin	RBCUR");matchRule(1, Rule::scope___scopeBegin_RBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 48:
/* Line 868 of glr.c  */
#line 173 "han_compiler.y"
    {puts("yacc>>expression:JIAN	expression");matchRule(0, Rule::expressionFunctionDeclareRoot___JIAN_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 49:
/* Line 868 of glr.c  */
#line 174 "han_compiler.y"
    {puts("yacc>>expression:JIA	expression");matchRule(0, Rule::expressionFunctionDeclareRoot___JIA_expression,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 50:
/* Line 868 of glr.c  */
#line 175 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	JIA	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_JIA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 51:
/* Line 868 of glr.c  */
#line 176 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	JIAN	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_JIAN_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 52:
/* Line 868 of glr.c  */
#line 177 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	CHENG	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_CHENG_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 53:
/* Line 868 of glr.c  */
#line 178 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	CHU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_CHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 54:
/* Line 868 of glr.c  */
#line 179 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	MI	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_MI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 55:
/* Line 868 of glr.c  */
#line 180 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	FUZHI	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_FUZHI_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 56:
/* Line 868 of glr.c  */
#line 181 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	DENGYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DENGYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 57:
/* Line 868 of glr.c  */
#line 182 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	DAYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DAYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 58:
/* Line 868 of glr.c  */
#line 183 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	XIAOYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_XIAOYU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 59:
/* Line 868 of glr.c  */
#line 184 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	DOTDOT	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DOTDOT_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 60:
/* Line 868 of glr.c  */
#line 185 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	COMMA	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_COMMA_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 61:
/* Line 868 of glr.c  */
#line 186 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	SHUCHU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_SHUCHU_expression,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 62:
/* Line 868 of glr.c  */
#line 187 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:LSCUR	expressionFunctionDeclareRoot	RSCUR");matchRule(0, Rule::expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 63:
/* Line 868 of glr.c  */
#line 188 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 64:
/* Line 868 of glr.c  */
#line 189 "han_compiler.y"
    {puts("yacc>>expressionFunctionDeclareRoot:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (4))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (4))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 65:
/* Line 868 of glr.c  */
#line 192 "han_compiler.y"
    {puts("yacc>>scopeFunctionDeclareBegin:LBCUR");matchRule(1, Rule::scopeFunctionDeclareBegin___LBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 66:
/* Line 868 of glr.c  */
#line 195 "han_compiler.y"
    {puts("yacc>>scopeFunctionDeclareHalf:scopeFunctionDeclareBegin	sentence");matchRule(0, Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 67:
/* Line 868 of glr.c  */
#line 196 "han_compiler.y"
    {puts("yacc>>scopeFunctionDeclareHalf:scopeFunctionDeclareHalf	sentence");matchRule(0, Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 68:
/* Line 868 of glr.c  */
#line 198 "han_compiler.y"
    {puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareBegin	expressionFunctionDeclareRoot");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 69:
/* Line 868 of glr.c  */
#line 199 "han_compiler.y"
    {puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareBegin	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 70:
/* Line 868 of glr.c  */
#line 200 "han_compiler.y"
    {puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareHalf	expressionFunctionDeclareRoot");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 71:
/* Line 868 of glr.c  */
#line 201 "han_compiler.y"
    {puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareHalf	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 72:
/* Line 868 of glr.c  */
#line 203 "han_compiler.y"
    {puts("yacc>>scopeFunctionDeclare:scopeFunctionDeclareEnd	RBCUR");matchRule(1, Rule::scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 73:
/* Line 868 of glr.c  */
#line 205 "han_compiler.y"
    {puts("yacc>>autoScopeBegin:NULL");((*yyvalp).argp)=0;matchRule(1, Rule::autoScopeBegin___NULL,NULL,NULL,NULL,&((*yyvalp).argp));}
    break;

  case 74:
/* Line 868 of glr.c  */
#line 208 "han_compiler.y"
    {puts("yacc>>autoScopeEnd:NULL");((*yyvalp).argp)=0;matchRule(0, Rule::autoScopeEnd___NULL,NULL,NULL,NULL,NULL);}
    break;

  case 75:
/* Line 868 of glr.c  */
#line 212 "han_compiler.y"
    {puts("yacc>>autoScopeHalf:autoScopeBegin	expressionFunctionDeclareRoot");matchRule(0, Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 76:
/* Line 868 of glr.c  */
#line 216 "han_compiler.y"
    {puts("yacc>>autoScope:autoScopeHalf	autoScopeEnd");matchRule(1, Rule::autoScope___autoScopeHalf_autoScopeEnd,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 77:
/* Line 868 of glr.c  */
#line 221 "han_compiler.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	autoScope");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 78:
/* Line 868 of glr.c  */
#line 222 "han_compiler.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	scopeFunctionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 79:
/* Line 868 of glr.c  */
#line 223 "han_compiler.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	scopeFunctionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 80:
/* Line 868 of glr.c  */
#line 224 "han_compiler.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	autoscope");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_autoscope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 81:
/* Line 868 of glr.c  */
#line 225 "han_compiler.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	functionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_functionDeclareBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 82:
/* Line 868 of glr.c  */
#line 226 "han_compiler.y"
    {puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	functionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 83:
/* Line 868 of glr.c  */
#line 228 "han_compiler.y"
    {puts("yacc>>functionDeclareHalf:functionDeclareBegin	JICHENG	arg");matchRule(1, Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 84:
/* Line 868 of glr.c  */
#line 229 "han_compiler.y"
    {puts("yacc>>functionDeclareHalf:functionDeclareHalf	COMMA	arg");matchRule(1, Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 85:
/* Line 868 of glr.c  */
#line 231 "han_compiler.y"
    {puts("yacc>>functionDeclare:functionDeclareBegin");matchRule(0, Rule::functionDeclare___functionDeclareBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 86:
/* Line 868 of glr.c  */
#line 232 "han_compiler.y"
    {puts("yacc>>functionDeclare:functionDeclareHalf");matchRule(0, Rule::functionDeclare___functionDeclareHalf,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 87:
/* Line 868 of glr.c  */
#line 234 "han_compiler.y"
    {puts("yacc>>functionImplement:functionDeclare	scope");matchRule(1, Rule::functionImplement___functionDeclare_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 88:
/* Line 868 of glr.c  */
#line 235 "han_compiler.y"
    {puts("yacc>>functionImplement:functionImplement	TUIDAO	functionDeclare");matchRule(1, Rule::functionImplement___functionImplement_TUIDAO_functionDeclare,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);}
    break;

  case 89:
/* Line 868 of glr.c  */
#line 238 "han_compiler.y"
    {puts("yacc>>systemDeclareBegin:system_declare_attribute	DOTDOT	arg");matchRule(1, Rule::systemDeclareBegin___system_declare_attribute_DOTDOT_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 90:
/* Line 868 of glr.c  */
#line 240 "han_compiler.y"
    {puts("yacc>>systemDeclareHalf:systemDeclareBegin	JICHENG	arg");matchRule(1, Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 91:
/* Line 868 of glr.c  */
#line 241 "han_compiler.y"
    {puts("yacc>>systemDeclareHalf:systemDeclareHalf	COMMA	arg");matchRule(1, Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 92:
/* Line 868 of glr.c  */
#line 245 "han_compiler.y"
    {puts("yacc>>systemDeclare:systemDeclareBegin");matchRule(0, Rule::systemDeclare___systemDeclareBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 93:
/* Line 868 of glr.c  */
#line 246 "han_compiler.y"
    {puts("yacc>>systemDeclare:systemDeclareHalf");matchRule(0, Rule::systemDeclare___systemDeclareHalf,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 94:
/* Line 868 of glr.c  */
#line 248 "han_compiler.y"
    {puts("yacc>>systemImplement:systemDeclare	scope");matchRule(1, Rule::systemImplement___systemDeclare_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (2))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((2) - (2))].yystate.yysemantics.yysval.argp),NULL,&((*yyvalp).argp));}
    break;

  case 95:
/* Line 868 of glr.c  */
#line 250 "han_compiler.y"
    {puts("yacc>>branchBegin:IF	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::branchBegin___IF_LSCUR_expression_RSCUR_autoScope,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 96:
/* Line 868 of glr.c  */
#line 251 "han_compiler.y"
    {puts("yacc>>branchBegin:IF	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::branchBegin___IF_LSCUR_expression_RSCUR_scope,NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 97:
/* Line 868 of glr.c  */
#line 253 "han_compiler.y"
    {puts("yacc>>branchHalf:branchBegin	ELIF	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 98:
/* Line 868 of glr.c  */
#line 254 "han_compiler.y"
    {puts("yacc>>branchHalf:branchBegin	ELIF	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (6))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((4) - (6))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((6) - (6))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 99:
/* Line 868 of glr.c  */
#line 256 "han_compiler.y"
    {puts("yacc>>branchEnd:branchBegin	ELSE	autoScope");matchRule(1, Rule::branchEnd___branchBegin_ELSE__autoScope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 100:
/* Line 868 of glr.c  */
#line 257 "han_compiler.y"
    {puts("yacc>>branchEnd:branchBegin	ELSE	scope");matchRule(1,  Rule::branchEnd___branchBegin_ELSE__scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 101:
/* Line 868 of glr.c  */
#line 258 "han_compiler.y"
    {puts("yacc>>branchEnd:branchHalf	ELSE	autoScope");matchRule(1, Rule::branchEnd___branchHalf_ELSE__autoScope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 102:
/* Line 868 of glr.c  */
#line 259 "han_compiler.y"
    {puts("yacc>>branchEnd:branchHalf	ELSE	scope");matchRule(1,  Rule::branchEnd___branchHalf_ELSE__scope,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (3))].yystate.yysemantics.yysval.argp),NULL,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (3))].yystate.yysemantics.yysval.argp),&((*yyvalp).argp));}
    break;

  case 103:
/* Line 868 of glr.c  */
#line 261 "han_compiler.y"
    {puts("yacc>>branch:branchBegin");matchRule(0, Rule::branch___branchBegin,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 104:
/* Line 868 of glr.c  */
#line 262 "han_compiler.y"
    {puts("yacc>>branch:branchHalf");matchRule(0,  Rule::branch___branchHalf,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 105:
/* Line 868 of glr.c  */
#line 263 "han_compiler.y"
    {puts("yacc>>branch:branchEnd");matchRule(0, Rule::branch___branchEnd,&(((yyGLRStackItem const *)yyvsp)[YYFILL ((1) - (1))].yystate.yysemantics.yysval.argp),NULL,NULL,&((*yyvalp).argp));}
    break;

  case 106:
/* Line 868 of glr.c  */
#line 266 "han_compiler.y"
    {puts("yacc>>loop:WHILE	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::loop___WHILE_LSCUR_expression_RSCUR_autoScope,&((*yyvalp).argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.argp),NULL);}
    break;

  case 107:
/* Line 868 of glr.c  */
#line 267 "han_compiler.y"
    {puts("yacc>>loop:WHILE	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::loop___WHILE_LSCUR_expression_RSCUR_scope,&((*yyvalp).argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((3) - (5))].yystate.yysemantics.yysval.argp),&(((yyGLRStackItem const *)yyvsp)[YYFILL ((5) - (5))].yystate.yysemantics.yysval.argp),NULL);}
    break;


/* Line 868 of glr.c  */
#line 1893 "y.tab.c"
      default: break;
    }

  return yyok;
# undef yyerrok
# undef YYABORT
# undef YYACCEPT
# undef YYERROR
# undef YYBACKUP
# undef yyclearin
# undef YYRECOVERING
}


/*ARGSUSED*/ static void
yyuserMerge (int yyn, YYSTYPE* yy0, YYSTYPE* yy1)
{
  YYUSE (yy0);
  YYUSE (yy1);

  switch (yyn)
    {
      
      default: break;
    }
}

                              /* Bison grammar-table manipulation.  */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
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

/** Number of symbols composing the right hand side of rule #RULE.  */
static inline int
yyrhsLength (yyRuleNum yyrule)
{
  return yyr2[yyrule];
}

static void
yydestroyGLRState (char const *yymsg, yyGLRState *yys)
{
  if (yys->yyresolved)
    yydestruct (yymsg, yystos[yys->yylrState],
                &yys->yysemantics.yysval);
  else
    {
#if YYDEBUG
      if (yydebug)
        {
          if (yys->yysemantics.yyfirstVal)
            YYFPRINTF (stderr, "%s unresolved ", yymsg);
          else
            YYFPRINTF (stderr, "%s incomplete ", yymsg);
          yy_symbol_print (stderr, yystos[yys->yylrState],
                           YY_NULL);
          YYFPRINTF (stderr, "\n");
        }
#endif

      if (yys->yysemantics.yyfirstVal)
        {
          yySemanticOption *yyoption = yys->yysemantics.yyfirstVal;
          yyGLRState *yyrh;
          int yyn;
          for (yyrh = yyoption->yystate, yyn = yyrhsLength (yyoption->yyrule);
               yyn > 0;
               yyrh = yyrh->yypred, yyn -= 1)
            yydestroyGLRState (yymsg, yyrh);
        }
    }
}

/** Left-hand-side symbol for rule #RULE.  */
static inline yySymbol
yylhsNonterm (yyRuleNum yyrule)
{
  return yyr1[yyrule];
}

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-40)))

/** True iff LR state STATE has only a default reduction (regardless
 *  of token).  */
static inline yybool
yyisDefaultedState (yyStateNum yystate)
{
  return yypact_value_is_default (yypact[yystate]);
}

/** The default reduction for STATE, assuming it has one.  */
static inline yyRuleNum
yydefaultAction (yyStateNum yystate)
{
  return yydefact[yystate];
}

#define yytable_value_is_error(Yytable_value) \
  YYID (0)

/** Set *YYACTION to the action to take in YYSTATE on seeing YYTOKEN.
 *  Result R means
 *    R < 0:  Reduce on rule -R.
 *    R = 0:  Error.
 *    R > 0:  Shift to state R.
 *  Set *CONFLICTS to a pointer into yyconfl to 0-terminated list of
 *  conflicting reductions.
 */
static inline void
yygetLRActions (yyStateNum yystate, int yytoken,
                int* yyaction, const short int** yyconflicts)
{
  int yyindex = yypact[yystate] + yytoken;
  if (yypact_value_is_default (yypact[yystate])
      || yyindex < 0 || YYLAST < yyindex || yycheck[yyindex] != yytoken)
    {
      *yyaction = -yydefact[yystate];
      *yyconflicts = yyconfl;
    }
  else if (! yytable_value_is_error (yytable[yyindex]))
    {
      *yyaction = yytable[yyindex];
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
  else
    {
      *yyaction = 0;
      *yyconflicts = yyconfl + yyconflp[yyindex];
    }
}

static inline yyStateNum
yyLRgotoState (yyStateNum yystate, yySymbol yylhs)
{
  int yyr;
  yyr = yypgoto[yylhs - YYNTOKENS] + yystate;
  if (0 <= yyr && yyr <= YYLAST && yycheck[yyr] == yystate)
    return yytable[yyr];
  else
    return yydefgoto[yylhs - YYNTOKENS];
}

static inline yybool
yyisShiftAction (int yyaction)
{
  return 0 < yyaction;
}

static inline yybool
yyisErrorAction (int yyaction)
{
  return yyaction == 0;
}

                                /* GLRStates */

/** Return a fresh GLRStackItem.  Callers should call
 * YY_RESERVE_GLRSTACK afterwards to make sure there is sufficient
 * headroom.  */

static inline yyGLRStackItem*
yynewGLRStackItem (yyGLRStack* yystackp, yybool yyisState)
{
  yyGLRStackItem* yynewItem = yystackp->yynextFree;
  yystackp->yyspaceLeft -= 1;
  yystackp->yynextFree += 1;
  yynewItem->yystate.yyisState = yyisState;
  return yynewItem;
}

/** Add a new semantic action that will execute the action for rule
 *  RULENUM on the semantic values in RHS to the list of
 *  alternative actions for STATE.  Assumes that RHS comes from
 *  stack #K of *STACKP. */
static void
yyaddDeferredAction (yyGLRStack* yystackp, size_t yyk, yyGLRState* yystate,
                     yyGLRState* rhs, yyRuleNum yyrule)
{
  yySemanticOption* yynewOption =
    &yynewGLRStackItem (yystackp, yyfalse)->yyoption;
  yynewOption->yystate = rhs;
  yynewOption->yyrule = yyrule;
  if (yystackp->yytops.yylookaheadNeeds[yyk])
    {
      yynewOption->yyrawchar = yychar;
      yynewOption->yyval = yylval;
    }
  else
    yynewOption->yyrawchar = YYEMPTY;
  yynewOption->yynext = yystate->yysemantics.yyfirstVal;
  yystate->yysemantics.yyfirstVal = yynewOption;

  YY_RESERVE_GLRSTACK (yystackp);
}

                                /* GLRStacks */

/** Initialize SET to a singleton set containing an empty stack.  */
static yybool
yyinitStateSet (yyGLRStateSet* yyset)
{
  yyset->yysize = 1;
  yyset->yycapacity = 16;
  yyset->yystates = (yyGLRState**) YYMALLOC (16 * sizeof yyset->yystates[0]);
  if (! yyset->yystates)
    return yyfalse;
  yyset->yystates[0] = YY_NULL;
  yyset->yylookaheadNeeds =
    (yybool*) YYMALLOC (16 * sizeof yyset->yylookaheadNeeds[0]);
  if (! yyset->yylookaheadNeeds)
    {
      YYFREE (yyset->yystates);
      return yyfalse;
    }
  return yytrue;
}

static void yyfreeStateSet (yyGLRStateSet* yyset)
{
  YYFREE (yyset->yystates);
  YYFREE (yyset->yylookaheadNeeds);
}

/** Initialize STACK to a single empty stack, with total maximum
 *  capacity for all stacks of SIZE.  */
static yybool
yyinitGLRStack (yyGLRStack* yystackp, size_t yysize)
{
  yystackp->yyerrState = 0;
  yynerrs = 0;
  yystackp->yyspaceLeft = yysize;
  yystackp->yyitems =
    (yyGLRStackItem*) YYMALLOC (yysize * sizeof yystackp->yynextFree[0]);
  if (!yystackp->yyitems)
    return yyfalse;
  yystackp->yynextFree = yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;
  return yyinitStateSet (&yystackp->yytops);
}


#if YYSTACKEXPANDABLE
# define YYRELOC(YYFROMITEMS,YYTOITEMS,YYX,YYTYPE) \
  &((YYTOITEMS) - ((YYFROMITEMS) - (yyGLRStackItem*) (YYX)))->YYTYPE

/** If STACK is expandable, extend it.  WARNING: Pointers into the
    stack from outside should be considered invalid after this call.
    We always expand when there are 1 or fewer items left AFTER an
    allocation, so that we can avoid having external pointers exist
    across an allocation.  */
static void
yyexpandGLRStack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yynewItems;
  yyGLRStackItem* yyp0, *yyp1;
  size_t yynewSize;
  size_t yyn;
  size_t yysize = yystackp->yynextFree - yystackp->yyitems;
  if (YYMAXDEPTH - YYHEADROOM < yysize)
    yyMemoryExhausted (yystackp);
  yynewSize = 2*yysize;
  if (YYMAXDEPTH < yynewSize)
    yynewSize = YYMAXDEPTH;
  yynewItems = (yyGLRStackItem*) YYMALLOC (yynewSize * sizeof yynewItems[0]);
  if (! yynewItems)
    yyMemoryExhausted (yystackp);
  for (yyp0 = yystackp->yyitems, yyp1 = yynewItems, yyn = yysize;
       0 < yyn;
       yyn -= 1, yyp0 += 1, yyp1 += 1)
    {
      *yyp1 = *yyp0;
      if (*(yybool *) yyp0)
        {
          yyGLRState* yys0 = &yyp0->yystate;
          yyGLRState* yys1 = &yyp1->yystate;
          if (yys0->yypred != YY_NULL)
            yys1->yypred =
              YYRELOC (yyp0, yyp1, yys0->yypred, yystate);
          if (! yys0->yyresolved && yys0->yysemantics.yyfirstVal != YY_NULL)
            yys1->yysemantics.yyfirstVal =
              YYRELOC (yyp0, yyp1, yys0->yysemantics.yyfirstVal, yyoption);
        }
      else
        {
          yySemanticOption* yyv0 = &yyp0->yyoption;
          yySemanticOption* yyv1 = &yyp1->yyoption;
          if (yyv0->yystate != YY_NULL)
            yyv1->yystate = YYRELOC (yyp0, yyp1, yyv0->yystate, yystate);
          if (yyv0->yynext != YY_NULL)
            yyv1->yynext = YYRELOC (yyp0, yyp1, yyv0->yynext, yyoption);
        }
    }
  if (yystackp->yysplitPoint != YY_NULL)
    yystackp->yysplitPoint = YYRELOC (yystackp->yyitems, yynewItems,
                                 yystackp->yysplitPoint, yystate);

  for (yyn = 0; yyn < yystackp->yytops.yysize; yyn += 1)
    if (yystackp->yytops.yystates[yyn] != YY_NULL)
      yystackp->yytops.yystates[yyn] =
        YYRELOC (yystackp->yyitems, yynewItems,
                 yystackp->yytops.yystates[yyn], yystate);
  YYFREE (yystackp->yyitems);
  yystackp->yyitems = yynewItems;
  yystackp->yynextFree = yynewItems + yysize;
  yystackp->yyspaceLeft = yynewSize - yysize;
}
#endif

static void
yyfreeGLRStack (yyGLRStack* yystackp)
{
  YYFREE (yystackp->yyitems);
  yyfreeStateSet (&yystackp->yytops);
}

/** Assuming that S is a GLRState somewhere on STACK, update the
 *  splitpoint of STACK, if needed, so that it is at least as deep as
 *  S.  */
static inline void
yyupdateSplit (yyGLRStack* yystackp, yyGLRState* yys)
{
  if (yystackp->yysplitPoint != YY_NULL && yystackp->yysplitPoint > yys)
    yystackp->yysplitPoint = yys;
}

/** Invalidate stack #K in STACK.  */
static inline void
yymarkStackDeleted (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yytops.yystates[yyk] != YY_NULL)
    yystackp->yylastDeleted = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yystates[yyk] = YY_NULL;
}

/** Undelete the last stack that was marked as deleted.  Can only be
    done once after a deletion, and only when all other stacks have
    been deleted.  */
static void
yyundeleteLastStack (yyGLRStack* yystackp)
{
  if (yystackp->yylastDeleted == YY_NULL || yystackp->yytops.yysize != 0)
    return;
  yystackp->yytops.yystates[0] = yystackp->yylastDeleted;
  yystackp->yytops.yysize = 1;
  YYDPRINTF ((stderr, "Restoring last deleted stack as stack #0.\n"));
  yystackp->yylastDeleted = YY_NULL;
}

static inline void
yyremoveDeletes (yyGLRStack* yystackp)
{
  size_t yyi, yyj;
  yyi = yyj = 0;
  while (yyj < yystackp->yytops.yysize)
    {
      if (yystackp->yytops.yystates[yyi] == YY_NULL)
        {
          if (yyi == yyj)
            {
              YYDPRINTF ((stderr, "Removing dead stacks.\n"));
            }
          yystackp->yytops.yysize -= 1;
        }
      else
        {
          yystackp->yytops.yystates[yyj] = yystackp->yytops.yystates[yyi];
          /* In the current implementation, it's unnecessary to copy
             yystackp->yytops.yylookaheadNeeds[yyi] since, after
             yyremoveDeletes returns, the parser immediately either enters
             deterministic operation or shifts a token.  However, it doesn't
             hurt, and the code might evolve to need it.  */
          yystackp->yytops.yylookaheadNeeds[yyj] =
            yystackp->yytops.yylookaheadNeeds[yyi];
          if (yyj != yyi)
            {
              YYDPRINTF ((stderr, "Rename stack %lu -> %lu.\n",
                          (unsigned long int) yyi, (unsigned long int) yyj));
            }
          yyj += 1;
        }
      yyi += 1;
    }
}

/** Shift to a new state on stack #K of STACK, corresponding to LR state
 * LRSTATE, at input position POSN, with (resolved) semantic value SVAL.  */
static inline void
yyglrShift (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
            size_t yyposn,
            YYSTYPE* yyvalp)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yytrue;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yysval = *yyvalp;
  yystackp->yytops.yystates[yyk] = yynewState;

  YY_RESERVE_GLRSTACK (yystackp);
}

/** Shift stack #K of YYSTACK, to a new state corresponding to LR
 *  state YYLRSTATE, at input position YYPOSN, with the (unresolved)
 *  semantic value of YYRHS under the action for YYRULE.  */
static inline void
yyglrShiftDefer (yyGLRStack* yystackp, size_t yyk, yyStateNum yylrState,
                 size_t yyposn, yyGLRState* rhs, yyRuleNum yyrule)
{
  yyGLRState* yynewState = &yynewGLRStackItem (yystackp, yytrue)->yystate;

  yynewState->yylrState = yylrState;
  yynewState->yyposn = yyposn;
  yynewState->yyresolved = yyfalse;
  yynewState->yypred = yystackp->yytops.yystates[yyk];
  yynewState->yysemantics.yyfirstVal = YY_NULL;
  yystackp->yytops.yystates[yyk] = yynewState;

  /* Invokes YY_RESERVE_GLRSTACK.  */
  yyaddDeferredAction (yystackp, yyk, yynewState, rhs, yyrule);
}

/** Pop the symbols consumed by reduction #RULE from the top of stack
 *  #K of STACK, and perform the appropriate semantic action on their
 *  semantic values.  Assumes that all ambiguities in semantic values
 *  have been previously resolved.  Set *VALP to the resulting value,
 *  and *LOCP to the computed location (if any).  Return value is as
 *  for userAction.  */
static inline YYRESULTTAG
yydoAction (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
            YYSTYPE* yyvalp)
{
  int yynrhs = yyrhsLength (yyrule);

  if (yystackp->yysplitPoint == YY_NULL)
    {
      /* Standard special case: single stack.  */
      yyGLRStackItem* rhs = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
      YYASSERT (yyk == 0);
      yystackp->yynextFree -= yynrhs;
      yystackp->yyspaceLeft += yynrhs;
      yystackp->yytops.yystates[0] = & yystackp->yynextFree[-1].yystate;
      return yyuserAction (yyrule, yynrhs, rhs, yystackp,
                           yyvalp);
    }
  else
    {
      /* At present, doAction is never called in nondeterministic
       * mode, so this branch is never taken.  It is here in
       * anticipation of a future feature that will allow immediate
       * evaluation of selected actions in nondeterministic mode.  */
      int yyi;
      yyGLRState* yys;
      yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
      yys = yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred
        = yystackp->yytops.yystates[yyk];
      for (yyi = 0; yyi < yynrhs; yyi += 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yystackp->yytops.yystates[yyk] = yys;
      return yyuserAction (yyrule, yynrhs, yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp);
    }
}

#if !YYDEBUG
# define YY_REDUCE_PRINT(Args)
#else
# define YY_REDUCE_PRINT(Args)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print Args;               \
} while (YYID (0))

/*----------------------------------------------------------.
| Report that the RULE is going to be reduced on stack #K.  |
`----------------------------------------------------------*/

/*ARGSUSED*/ static inline void
yy_reduce_print (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
                 YYSTYPE* yyvalp)
{
  int yynrhs = yyrhsLength (yyrule);
  yybool yynormal __attribute__ ((__unused__)) =
    (yystackp->yysplitPoint == YY_NULL);
  yyGLRStackItem* yyvsp = (yyGLRStackItem*) yystackp->yytops.yystates[yyk];
  int yylow = 1;
  int yyi;
  YYUSE (yyvalp);
  YYFPRINTF (stderr, "Reducing stack %lu by rule %d (line %lu):\n",
             (unsigned long int) yyk, yyrule - 1,
             (unsigned long int) yyrline[yyrule]);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
                       &(((yyGLRStackItem const *)yyvsp)[YYFILL ((yyi + 1) - (yynrhs))].yystate.yysemantics.yysval)
                                              );
      YYFPRINTF (stderr, "\n");
    }
}
#endif

/** Pop items off stack #K of STACK according to grammar rule RULE,
 *  and push back on the resulting nonterminal symbol.  Perform the
 *  semantic action associated with RULE and store its value with the
 *  newly pushed state, if FORCEEVAL or if STACK is currently
 *  unambiguous.  Otherwise, store the deferred semantic action with
 *  the new state.  If the new state would have an identical input
 *  position, LR state, and predecessor to an existing state on the stack,
 *  it is identified with that existing state, eliminating stack #K from
 *  the STACK.  In this case, the (necessarily deferred) semantic value is
 *  added to the options for the existing state's semantic value.
 */
static inline YYRESULTTAG
yyglrReduce (yyGLRStack* yystackp, size_t yyk, yyRuleNum yyrule,
             yybool yyforceEval)
{
  size_t yyposn = yystackp->yytops.yystates[yyk]->yyposn;

  if (yyforceEval || yystackp->yysplitPoint == YY_NULL)
    {
      YYSTYPE yysval;

      YY_REDUCE_PRINT ((yystackp, yyk, yyrule, &yysval));
      YYCHK (yydoAction (yystackp, yyk, yyrule, &yysval));
      YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyrule], &yysval, &yyloc);
      yyglrShift (yystackp, yyk,
                  yyLRgotoState (yystackp->yytops.yystates[yyk]->yylrState,
                                 yylhsNonterm (yyrule)),
                  yyposn, &yysval);
    }
  else
    {
      size_t yyi;
      int yyn;
      yyGLRState* yys, *yys0 = yystackp->yytops.yystates[yyk];
      yyStateNum yynewLRState;

      for (yys = yystackp->yytops.yystates[yyk], yyn = yyrhsLength (yyrule);
           0 < yyn; yyn -= 1)
        {
          yys = yys->yypred;
          YYASSERT (yys);
        }
      yyupdateSplit (yystackp, yys);
      yynewLRState = yyLRgotoState (yys->yylrState, yylhsNonterm (yyrule));
      YYDPRINTF ((stderr,
                  "Reduced stack %lu by rule #%d; action deferred.  Now in state %d.\n",
                  (unsigned long int) yyk, yyrule - 1, yynewLRState));
      for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
        if (yyi != yyk && yystackp->yytops.yystates[yyi] != YY_NULL)
          {
            yyGLRState *yysplit = yystackp->yysplitPoint;
            yyGLRState *yyp = yystackp->yytops.yystates[yyi];
            while (yyp != yys && yyp != yysplit && yyp->yyposn >= yyposn)
              {
                if (yyp->yylrState == yynewLRState && yyp->yypred == yys)
                  {
                    yyaddDeferredAction (yystackp, yyk, yyp, yys0, yyrule);
                    yymarkStackDeleted (yystackp, yyk);
                    YYDPRINTF ((stderr, "Merging stack %lu into stack %lu.\n",
                                (unsigned long int) yyk,
                                (unsigned long int) yyi));
                    return yyok;
                  }
                yyp = yyp->yypred;
              }
          }
      yystackp->yytops.yystates[yyk] = yys;
      yyglrShiftDefer (yystackp, yyk, yynewLRState, yyposn, yys0, yyrule);
    }
  return yyok;
}

static size_t
yysplitStack (yyGLRStack* yystackp, size_t yyk)
{
  if (yystackp->yysplitPoint == YY_NULL)
    {
      YYASSERT (yyk == 0);
      yystackp->yysplitPoint = yystackp->yytops.yystates[yyk];
    }
  if (yystackp->yytops.yysize >= yystackp->yytops.yycapacity)
    {
      yyGLRState** yynewStates;
      yybool* yynewLookaheadNeeds;

      yynewStates = YY_NULL;

      if (yystackp->yytops.yycapacity
          > (YYSIZEMAX / (2 * sizeof yynewStates[0])))
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yycapacity *= 2;

      yynewStates =
        (yyGLRState**) YYREALLOC (yystackp->yytops.yystates,
                                  (yystackp->yytops.yycapacity
                                   * sizeof yynewStates[0]));
      if (yynewStates == YY_NULL)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yystates = yynewStates;

      yynewLookaheadNeeds =
        (yybool*) YYREALLOC (yystackp->yytops.yylookaheadNeeds,
                             (yystackp->yytops.yycapacity
                              * sizeof yynewLookaheadNeeds[0]));
      if (yynewLookaheadNeeds == YY_NULL)
        yyMemoryExhausted (yystackp);
      yystackp->yytops.yylookaheadNeeds = yynewLookaheadNeeds;
    }
  yystackp->yytops.yystates[yystackp->yytops.yysize]
    = yystackp->yytops.yystates[yyk];
  yystackp->yytops.yylookaheadNeeds[yystackp->yytops.yysize]
    = yystackp->yytops.yylookaheadNeeds[yyk];
  yystackp->yytops.yysize += 1;
  return yystackp->yytops.yysize-1;
}

/** True iff Y0 and Y1 represent identical options at the top level.
 *  That is, they represent the same rule applied to RHS symbols
 *  that produce the same terminal symbols.  */
static yybool
yyidenticalOptions (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  if (yyy0->yyrule == yyy1->yyrule)
    {
      yyGLRState *yys0, *yys1;
      int yyn;
      for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
           yyn = yyrhsLength (yyy0->yyrule);
           yyn > 0;
           yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
        if (yys0->yyposn != yys1->yyposn)
          return yyfalse;
      return yytrue;
    }
  else
    return yyfalse;
}

/** Assuming identicalOptions (Y0,Y1), destructively merge the
 *  alternative semantic values for the RHS-symbols of Y1 and Y0.  */
static void
yymergeOptionSets (yySemanticOption* yyy0, yySemanticOption* yyy1)
{
  yyGLRState *yys0, *yys1;
  int yyn;
  for (yys0 = yyy0->yystate, yys1 = yyy1->yystate,
       yyn = yyrhsLength (yyy0->yyrule);
       yyn > 0;
       yys0 = yys0->yypred, yys1 = yys1->yypred, yyn -= 1)
    {
      if (yys0 == yys1)
        break;
      else if (yys0->yyresolved)
        {
          yys1->yyresolved = yytrue;
          yys1->yysemantics.yysval = yys0->yysemantics.yysval;
        }
      else if (yys1->yyresolved)
        {
          yys0->yyresolved = yytrue;
          yys0->yysemantics.yysval = yys1->yysemantics.yysval;
        }
      else
        {
          yySemanticOption** yyz0p = &yys0->yysemantics.yyfirstVal;
          yySemanticOption* yyz1 = yys1->yysemantics.yyfirstVal;
          while (YYID (yytrue))
            {
              if (yyz1 == *yyz0p || yyz1 == YY_NULL)
                break;
              else if (*yyz0p == YY_NULL)
                {
                  *yyz0p = yyz1;
                  break;
                }
              else if (*yyz0p < yyz1)
                {
                  yySemanticOption* yyz = *yyz0p;
                  *yyz0p = yyz1;
                  yyz1 = yyz1->yynext;
                  (*yyz0p)->yynext = yyz;
                }
              yyz0p = &(*yyz0p)->yynext;
            }
          yys1->yysemantics.yyfirstVal = yys0->yysemantics.yyfirstVal;
        }
    }
}

/** Y0 and Y1 represent two possible actions to take in a given
 *  parsing state; return 0 if no combination is possible,
 *  1 if user-mergeable, 2 if Y0 is preferred, 3 if Y1 is preferred.  */
static int
yypreference (yySemanticOption* y0, yySemanticOption* y1)
{
  yyRuleNum r0 = y0->yyrule, r1 = y1->yyrule;
  int p0 = yydprec[r0], p1 = yydprec[r1];

  if (p0 == p1)
    {
      if (yymerger[r0] == 0 || yymerger[r0] != yymerger[r1])
        return 0;
      else
        return 1;
    }
  if (p0 == 0 || p1 == 0)
    return 0;
  if (p0 < p1)
    return 3;
  if (p1 < p0)
    return 2;
  return 0;
}

static YYRESULTTAG yyresolveValue (yyGLRState* yys,
                                   yyGLRStack* yystackp);


/** Resolve the previous N states starting at and including state S.  If result
 *  != yyok, some states may have been left unresolved possibly with empty
 *  semantic option chains.  Regardless of whether result = yyok, each state
 *  has been left with consistent data so that yydestroyGLRState can be invoked
 *  if necessary.  */
static YYRESULTTAG
yyresolveStates (yyGLRState* yys, int yyn,
                 yyGLRStack* yystackp)
{
  if (0 < yyn)
    {
      YYASSERT (yys->yypred);
      YYCHK (yyresolveStates (yys->yypred, yyn-1, yystackp));
      if (! yys->yyresolved)
        YYCHK (yyresolveValue (yys, yystackp));
    }
  return yyok;
}

/** Resolve the states for the RHS of OPT, perform its user action, and return
 *  the semantic value and location.  Regardless of whether result = yyok, all
 *  RHS states have been destroyed (assuming the user action destroys all RHS
 *  semantic values if invoked).  */
static YYRESULTTAG
yyresolveAction (yySemanticOption* yyopt, yyGLRStack* yystackp,
                 YYSTYPE* yyvalp)
{
  yyGLRStackItem yyrhsVals[YYMAXRHS + YYMAXLEFT + 1];
  int yynrhs = yyrhsLength (yyopt->yyrule);
  YYRESULTTAG yyflag =
    yyresolveStates (yyopt->yystate, yynrhs, yystackp);
  if (yyflag != yyok)
    {
      yyGLRState *yys;
      for (yys = yyopt->yystate; yynrhs > 0; yys = yys->yypred, yynrhs -= 1)
        yydestroyGLRState ("Cleanup: popping", yys);
      return yyflag;
    }

  yyrhsVals[YYMAXRHS + YYMAXLEFT].yystate.yypred = yyopt->yystate;
  {
    int yychar_current = yychar;
    YYSTYPE yylval_current = yylval;
    yychar = yyopt->yyrawchar;
    yylval = yyopt->yyval;
    yyflag = yyuserAction (yyopt->yyrule, yynrhs,
                           yyrhsVals + YYMAXRHS + YYMAXLEFT - 1,
                           yystackp, yyvalp);
    yychar = yychar_current;
    yylval = yylval_current;
  }
  return yyflag;
}

#if YYDEBUG
static void
yyreportTree (yySemanticOption* yyx, int yyindent)
{
  int yynrhs = yyrhsLength (yyx->yyrule);
  int yyi;
  yyGLRState* yys;
  yyGLRState* yystates[1 + YYMAXRHS];
  yyGLRState yyleftmost_state;

  for (yyi = yynrhs, yys = yyx->yystate; 0 < yyi; yyi -= 1, yys = yys->yypred)
    yystates[yyi] = yys;
  if (yys == YY_NULL)
    {
      yyleftmost_state.yyposn = 0;
      yystates[0] = &yyleftmost_state;
    }
  else
    yystates[0] = yys;

  if (yyx->yystate->yyposn < yys->yyposn + 1)
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, empty>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1);
  else
    YYFPRINTF (stderr, "%*s%s -> <Rule %d, tokens %lu .. %lu>\n",
               yyindent, "", yytokenName (yylhsNonterm (yyx->yyrule)),
               yyx->yyrule - 1, (unsigned long int) (yys->yyposn + 1),
               (unsigned long int) yyx->yystate->yyposn);
  for (yyi = 1; yyi <= yynrhs; yyi += 1)
    {
      if (yystates[yyi]->yyresolved)
        {
          if (yystates[yyi-1]->yyposn+1 > yystates[yyi]->yyposn)
            YYFPRINTF (stderr, "%*s%s <empty>\n", yyindent+2, "",
                       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]));
          else
            YYFPRINTF (stderr, "%*s%s <tokens %lu .. %lu>\n", yyindent+2, "",
                       yytokenName (yyrhs[yyprhs[yyx->yyrule]+yyi-1]),
                       (unsigned long int) (yystates[yyi - 1]->yyposn + 1),
                       (unsigned long int) yystates[yyi]->yyposn);
        }
      else
        yyreportTree (yystates[yyi]->yysemantics.yyfirstVal, yyindent+2);
    }
}
#endif

/*ARGSUSED*/ static YYRESULTTAG
yyreportAmbiguity (yySemanticOption* yyx0,
                   yySemanticOption* yyx1)
{
  YYUSE (yyx0);
  YYUSE (yyx1);

#if YYDEBUG
  YYFPRINTF (stderr, "Ambiguity detected.\n");
  YYFPRINTF (stderr, "Option 1,\n");
  yyreportTree (yyx0, 2);
  YYFPRINTF (stderr, "\nOption 2,\n");
  yyreportTree (yyx1, 2);
  YYFPRINTF (stderr, "\n");
#endif

  yyerror (YY_("syntax is ambiguous"));
  return yyabort;
}

/** Resolve the ambiguity represented in state S, perform the indicated
 *  actions, and set the semantic value of S.  If result != yyok, the chain of
 *  semantic options in S has been cleared instead or it has been left
 *  unmodified except that redundant options may have been removed.  Regardless
 *  of whether result = yyok, S has been left with consistent data so that
 *  yydestroyGLRState can be invoked if necessary.  */
static YYRESULTTAG
yyresolveValue (yyGLRState* yys, yyGLRStack* yystackp)
{
  yySemanticOption* yyoptionList = yys->yysemantics.yyfirstVal;
  yySemanticOption* yybest = yyoptionList;
  yySemanticOption** yypp;
  yybool yymerge = yyfalse;
  YYSTYPE yysval;
  YYRESULTTAG yyflag;

  for (yypp = &yyoptionList->yynext; *yypp != YY_NULL; )
    {
      yySemanticOption* yyp = *yypp;

      if (yyidenticalOptions (yybest, yyp))
        {
          yymergeOptionSets (yybest, yyp);
          *yypp = yyp->yynext;
        }
      else
        {
          switch (yypreference (yybest, yyp))
            {
            case 0:
              return yyreportAmbiguity (yybest, yyp);
              break;
            case 1:
              yymerge = yytrue;
              break;
            case 2:
              break;
            case 3:
              yybest = yyp;
              yymerge = yyfalse;
              break;
            default:
              /* This cannot happen so it is not worth a YYASSERT (yyfalse),
                 but some compilers complain if the default case is
                 omitted.  */
              break;
            }
          yypp = &yyp->yynext;
        }
    }

  if (yymerge)
    {
      yySemanticOption* yyp;
      int yyprec = yydprec[yybest->yyrule];
      yyflag = yyresolveAction (yybest, yystackp, &yysval);
      if (yyflag == yyok)
        for (yyp = yybest->yynext; yyp != YY_NULL; yyp = yyp->yynext)
          {
            if (yyprec == yydprec[yyp->yyrule])
              {
                YYSTYPE yysval_other;
                yyflag = yyresolveAction (yyp, yystackp, &yysval_other);
                if (yyflag != yyok)
                  {
                    yydestruct ("Cleanup: discarding incompletely merged value for",
                                yystos[yys->yylrState],
                                &yysval);
                    break;
                  }
                yyuserMerge (yymerger[yyp->yyrule], &yysval, &yysval_other);
              }
          }
    }
  else
    yyflag = yyresolveAction (yybest, yystackp, &yysval);

  if (yyflag == yyok)
    {
      yys->yyresolved = yytrue;
      yys->yysemantics.yysval = yysval;
    }
  else
    yys->yysemantics.yyfirstVal = YY_NULL;
  return yyflag;
}

static YYRESULTTAG
yyresolveStack (yyGLRStack* yystackp)
{
  if (yystackp->yysplitPoint != YY_NULL)
    {
      yyGLRState* yys;
      int yyn;

      for (yyn = 0, yys = yystackp->yytops.yystates[0];
           yys != yystackp->yysplitPoint;
           yys = yys->yypred, yyn += 1)
        continue;
      YYCHK (yyresolveStates (yystackp->yytops.yystates[0], yyn, yystackp
                             ));
    }
  return yyok;
}

static void
yycompressStack (yyGLRStack* yystackp)
{
  yyGLRState* yyp, *yyq, *yyr;

  if (yystackp->yytops.yysize != 1 || yystackp->yysplitPoint == YY_NULL)
    return;

  for (yyp = yystackp->yytops.yystates[0], yyq = yyp->yypred, yyr = YY_NULL;
       yyp != yystackp->yysplitPoint;
       yyr = yyp, yyp = yyq, yyq = yyp->yypred)
    yyp->yypred = yyr;

  yystackp->yyspaceLeft += yystackp->yynextFree - yystackp->yyitems;
  yystackp->yynextFree = ((yyGLRStackItem*) yystackp->yysplitPoint) + 1;
  yystackp->yyspaceLeft -= yystackp->yynextFree - yystackp->yyitems;
  yystackp->yysplitPoint = YY_NULL;
  yystackp->yylastDeleted = YY_NULL;

  while (yyr != YY_NULL)
    {
      yystackp->yynextFree->yystate = *yyr;
      yyr = yyr->yypred;
      yystackp->yynextFree->yystate.yypred = &yystackp->yynextFree[-1].yystate;
      yystackp->yytops.yystates[0] = &yystackp->yynextFree->yystate;
      yystackp->yynextFree += 1;
      yystackp->yyspaceLeft -= 1;
    }
}

static YYRESULTTAG
yyprocessOneStack (yyGLRStack* yystackp, size_t yyk,
                   size_t yyposn)
{
  int yyaction;
  const short int* yyconflicts;
  yyRuleNum yyrule;

  while (yystackp->yytops.yystates[yyk] != YY_NULL)
    {
      yyStateNum yystate = yystackp->yytops.yystates[yyk]->yylrState;
      YYDPRINTF ((stderr, "Stack %lu Entering state %d\n",
                  (unsigned long int) yyk, yystate));

      YYASSERT (yystate != YYFINAL);

      if (yyisDefaultedState (yystate))
        {
          yyrule = yydefaultAction (yystate);
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              return yyok;
            }
          YYCHK (yyglrReduce (yystackp, yyk, yyrule, yyfalse));
        }
      else
        {
          yySymbol yytoken;
          yystackp->yytops.yylookaheadNeeds[yyk] = yytrue;
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

          yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);

          while (*yyconflicts != 0)
            {
              size_t yynewStack = yysplitStack (yystackp, yyk);
              YYDPRINTF ((stderr, "Splitting off stack %lu from %lu.\n",
                          (unsigned long int) yynewStack,
                          (unsigned long int) yyk));
              YYCHK (yyglrReduce (yystackp, yynewStack,
                                  *yyconflicts, yyfalse));
              YYCHK (yyprocessOneStack (yystackp, yynewStack,
                                        yyposn));
              yyconflicts += 1;
            }

          if (yyisShiftAction (yyaction))
            break;
          else if (yyisErrorAction (yyaction))
            {
              YYDPRINTF ((stderr, "Stack %lu dies.\n",
                          (unsigned long int) yyk));
              yymarkStackDeleted (yystackp, yyk);
              break;
            }
          else
            YYCHK (yyglrReduce (yystackp, yyk, -yyaction,
                                yyfalse));
        }
    }
  return yyok;
}

/*ARGSUSED*/ static void
yyreportSyntaxError (yyGLRStack* yystackp)
{
  if (yystackp->yyerrState != 0)
    return;
#if ! YYERROR_VERBOSE
  yyerror (YY_("syntax error"));
#else
  {
  yySymbol yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);
  size_t yysize0 = yytnamerr (YY_NULL, yytokenName (yytoken));
  size_t yysize = yysize0;
  yybool yysize_overflow = yyfalse;
  char* yymsg = YY_NULL;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected").  */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[yystackp->yytops.yystates[0]->yylrState];
      yyarg[yycount++] = yytokenName (yytoken);
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for this
             state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;
          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytokenName (yyx);
                {
                  size_t yysz = yysize + yytnamerr (YY_NULL, yytokenName (yyx));
                  yysize_overflow |= yysz < yysize;
                  yysize = yysz;
                }
              }
        }
    }

  switch (yycount)
    {
#define YYCASE_(N, S)                   \
      case N:                           \
        yyformat = S;                   \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  {
    size_t yysz = yysize + strlen (yyformat);
    yysize_overflow |= yysz < yysize;
    yysize = yysz;
  }

  if (!yysize_overflow)
    yymsg = (char *) YYMALLOC (yysize);

  if (yymsg)
    {
      char *yyp = yymsg;
      int yyi = 0;
      while ((*yyp = *yyformat))
        {
          if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
            {
              yyp += yytnamerr (yyp, yyarg[yyi++]);
              yyformat += 2;
            }
          else
            {
              yyp++;
              yyformat++;
            }
        }
      yyerror (yymsg);
      YYFREE (yymsg);
    }
  else
    {
      yyerror (YY_("syntax error"));
      yyMemoryExhausted (yystackp);
    }
  }
#endif /* YYERROR_VERBOSE */
  yynerrs += 1;
}

/* Recover from a syntax error on *YYSTACKP, assuming that *YYSTACKP->YYTOKENP,
   yylval, and yylloc are the syntactic category, semantic value, and location
   of the lookahead.  */
/*ARGSUSED*/ static void
yyrecoverSyntaxError (yyGLRStack* yystackp)
{
  size_t yyk;
  int yyj;

  if (yystackp->yyerrState == 3)
    /* We just shifted the error token and (perhaps) took some
       reductions.  Skip tokens until we can proceed.  */
    while (YYID (yytrue))
      {
        yySymbol yytoken;
        if (yychar == YYEOF)
          yyFail (yystackp, YY_NULL);
        if (yychar != YYEMPTY)
          {
            yytoken = YYTRANSLATE (yychar);
            yydestruct ("Error: discarding",
                        yytoken, &yylval);
          }
        YYDPRINTF ((stderr, "Reading a token: "));
        yychar = YYLEX;
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
        yyj = yypact[yystackp->yytops.yystates[0]->yylrState];
        if (yypact_value_is_default (yyj))
          return;
        yyj += yytoken;
        if (yyj < 0 || YYLAST < yyj || yycheck[yyj] != yytoken)
          {
            if (yydefact[yystackp->yytops.yystates[0]->yylrState] != 0)
              return;
          }
        else if (! yytable_value_is_error (yytable[yyj]))
          return;
      }

  /* Reduce to one stack.  */
  for (yyk = 0; yyk < yystackp->yytops.yysize; yyk += 1)
    if (yystackp->yytops.yystates[yyk] != YY_NULL)
      break;
  if (yyk >= yystackp->yytops.yysize)
    yyFail (yystackp, YY_NULL);
  for (yyk += 1; yyk < yystackp->yytops.yysize; yyk += 1)
    yymarkStackDeleted (yystackp, yyk);
  yyremoveDeletes (yystackp);
  yycompressStack (yystackp);

  /* Now pop stack until we find a state that shifts the error token.  */
  yystackp->yyerrState = 3;
  while (yystackp->yytops.yystates[0] != YY_NULL)
    {
      yyGLRState *yys = yystackp->yytops.yystates[0];
      yyj = yypact[yys->yylrState];
      if (! yypact_value_is_default (yyj))
        {
          yyj += YYTERROR;
          if (0 <= yyj && yyj <= YYLAST && yycheck[yyj] == YYTERROR
              && yyisShiftAction (yytable[yyj]))
            {
              /* Shift the error token.  */
              YY_SYMBOL_PRINT ("Shifting", yystos[yytable[yyj]],
                               &yylval, &yyerrloc);
              yyglrShift (yystackp, 0, yytable[yyj],
                          yys->yyposn, &yylval);
              yys = yystackp->yytops.yystates[0];
              break;
            }
        }
      if (yys->yypred != YY_NULL)
        yydestroyGLRState ("Error: popping", yys);
      yystackp->yytops.yystates[0] = yys->yypred;
      yystackp->yynextFree -= 1;
      yystackp->yyspaceLeft += 1;
    }
  if (yystackp->yytops.yystates[0] == YY_NULL)
    yyFail (yystackp, YY_NULL);
}

#define YYCHK1(YYE)                                                          \
  do {                                                                       \
    switch (YYE) {                                                           \
    case yyok:                                                               \
      break;                                                                 \
    case yyabort:                                                            \
      goto yyabortlab;                                                       \
    case yyaccept:                                                           \
      goto yyacceptlab;                                                      \
    case yyerr:                                                              \
      goto yyuser_error;                                                     \
    default:                                                                 \
      goto yybuglab;                                                         \
    }                                                                        \
  } while (YYID (0))


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
  int yyresult;
  yyGLRStack yystack;
  yyGLRStack* const yystackp = &yystack;
  size_t yyposn;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY;
  yylval = yyval_default;

  if (! yyinitGLRStack (yystackp, YYINITDEPTH))
    goto yyexhaustedlab;
  switch (YYSETJMP (yystack.yyexception_buffer))
    {
    case 0: break;
    case 1: goto yyabortlab;
    case 2: goto yyexhaustedlab;
    default: goto yybuglab;
    }
  yyglrShift (&yystack, 0, 0, 0, &yylval);
  yyposn = 0;

  while (YYID (yytrue))
    {
      /* For efficiency, we have two loops, the first of which is
         specialized to deterministic operation (single stack, no
         potential ambiguity).  */
      /* Standard mode */
      while (YYID (yytrue))
        {
          yyRuleNum yyrule;
          int yyaction;
          const short int* yyconflicts;

          yyStateNum yystate = yystack.yytops.yystates[0]->yylrState;
          YYDPRINTF ((stderr, "Entering state %d\n", yystate));
          if (yystate == YYFINAL)
            goto yyacceptlab;
          if (yyisDefaultedState (yystate))
            {
              yyrule = yydefaultAction (yystate);
              if (yyrule == 0)
                {

                  yyreportSyntaxError (&yystack);
                  goto yyuser_error;
                }
              YYCHK1 (yyglrReduce (&yystack, 0, yyrule, yytrue));
            }
          else
            {
              yySymbol yytoken;
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

              yygetLRActions (yystate, yytoken, &yyaction, &yyconflicts);
              if (*yyconflicts != 0)
                break;
              if (yyisShiftAction (yyaction))
                {
                  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
                  yychar = YYEMPTY;
                  yyposn += 1;
                  yyglrShift (&yystack, 0, yyaction, yyposn, &yylval);
                  if (0 < yystack.yyerrState)
                    yystack.yyerrState -= 1;
                }
              else if (yyisErrorAction (yyaction))
                {

                  yyreportSyntaxError (&yystack);
                  goto yyuser_error;
                }
              else
                YYCHK1 (yyglrReduce (&yystack, 0, -yyaction, yytrue));
            }
        }

      while (YYID (yytrue))
        {
          yySymbol yytoken_to_shift;
          size_t yys;

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            yystackp->yytops.yylookaheadNeeds[yys] = yychar != YYEMPTY;

          /* yyprocessOneStack returns one of three things:

              - An error flag.  If the caller is yyprocessOneStack, it
                immediately returns as well.  When the caller is finally
                yyparse, it jumps to an error label via YYCHK1.

              - yyok, but yyprocessOneStack has invoked yymarkStackDeleted
                (&yystack, yys), which sets the top state of yys to NULL.  Thus,
                yyparse's following invocation of yyremoveDeletes will remove
                the stack.

              - yyok, when ready to shift a token.

             Except in the first case, yyparse will invoke yyremoveDeletes and
             then shift the next token onto all remaining stacks.  This
             synchronization of the shift (that is, after all preceding
             reductions on all stacks) helps prevent double destructor calls
             on yylval in the event of memory exhaustion.  */

          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            YYCHK1 (yyprocessOneStack (&yystack, yys, yyposn));
          yyremoveDeletes (&yystack);
          if (yystack.yytops.yysize == 0)
            {
              yyundeleteLastStack (&yystack);
              if (yystack.yytops.yysize == 0)
                yyFail (&yystack, YY_("syntax error"));
              YYCHK1 (yyresolveStack (&yystack));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));

              yyreportSyntaxError (&yystack);
              goto yyuser_error;
            }

          /* If any yyglrShift call fails, it will fail after shifting.  Thus,
             a copy of yylval will already be on stack 0 in the event of a
             failure in the following loop.  Thus, yychar is set to YYEMPTY
             before the loop to make sure the user destructor for yylval isn't
             called twice.  */
          yytoken_to_shift = YYTRANSLATE (yychar);
          yychar = YYEMPTY;
          yyposn += 1;
          for (yys = 0; yys < yystack.yytops.yysize; yys += 1)
            {
              int yyaction;
              const short int* yyconflicts;
              yyStateNum yystate = yystack.yytops.yystates[yys]->yylrState;
              yygetLRActions (yystate, yytoken_to_shift, &yyaction,
                              &yyconflicts);
              /* Note that yyconflicts were handled by yyprocessOneStack.  */
              YYDPRINTF ((stderr, "On stack %lu, ", (unsigned long int) yys));
              YY_SYMBOL_PRINT ("shifting", yytoken_to_shift, &yylval, &yylloc);
              yyglrShift (&yystack, yys, yyaction, yyposn,
                          &yylval);
              YYDPRINTF ((stderr, "Stack %lu now in state #%d\n",
                          (unsigned long int) yys,
                          yystack.yytops.yystates[yys]->yylrState));
            }

          if (yystack.yytops.yysize == 1)
            {
              YYCHK1 (yyresolveStack (&yystack));
              YYDPRINTF ((stderr, "Returning to deterministic operation.\n"));
              yycompressStack (&yystack);
              break;
            }
        }
      continue;
    yyuser_error:
      yyrecoverSyntaxError (&yystack);
      yyposn = yystack.yytops.yystates[0]->yyposn;
    }

 yyacceptlab:
  yyresult = 0;
  goto yyreturn;

 yybuglab:
  YYASSERT (yyfalse);
  goto yyabortlab;

 yyabortlab:
  yyresult = 1;
  goto yyreturn;

 yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;

 yyreturn:
  if (yychar != YYEMPTY)
    yydestruct ("Cleanup: discarding lookahead",
                YYTRANSLATE (yychar), &yylval);

  /* If the stack is well-formed, pop the stack until it is empty,
     destroying its entries as we go.  But free the stack regardless
     of whether it is well-formed.  */
  if (yystack.yyitems)
    {
      yyGLRState** yystates = yystack.yytops.yystates;
      if (yystates)
        {
          size_t yysize = yystack.yytops.yysize;
          size_t yyk;
          for (yyk = 0; yyk < yysize; yyk += 1)
            if (yystates[yyk])
              {
                while (yystates[yyk])
                  {
                    yyGLRState *yys = yystates[yyk];
                  if (yys->yypred != YY_NULL)
                      yydestroyGLRState ("Cleanup: popping", yys);
                    yystates[yyk] = yys->yypred;
                    yystack.yynextFree -= 1;
                    yystack.yyspaceLeft += 1;
                  }
                break;
              }
        }
      yyfreeGLRStack (&yystack);
    }

  /* Make sure YYID is used.  */
  return YYID (yyresult);
}

/* DEBUGGING ONLY */
#if YYDEBUG
static void yypstack (yyGLRStack* yystackp, size_t yyk)
  __attribute__ ((__unused__));
static void yypdumpstack (yyGLRStack* yystackp) __attribute__ ((__unused__));

static void
yy_yypstack (yyGLRState* yys)
{
  if (yys->yypred)
    {
      yy_yypstack (yys->yypred);
      YYFPRINTF (stderr, " -> ");
    }
  YYFPRINTF (stderr, "%d@%lu", yys->yylrState,
             (unsigned long int) yys->yyposn);
}

static void
yypstates (yyGLRState* yyst)
{
  if (yyst == YY_NULL)
    YYFPRINTF (stderr, "<null>");
  else
    yy_yypstack (yyst);
  YYFPRINTF (stderr, "\n");
}

static void
yypstack (yyGLRStack* yystackp, size_t yyk)
{
  yypstates (yystackp->yytops.yystates[yyk]);
}

#define YYINDEX(YYX)                                                         \
    ((YYX) == YY_NULL ? -1 : (yyGLRStackItem*) (YYX) - yystackp->yyitems)


static void
yypdumpstack (yyGLRStack* yystackp)
{
  yyGLRStackItem* yyp;
  size_t yyi;
  for (yyp = yystackp->yyitems; yyp < yystackp->yynextFree; yyp += 1)
    {
      YYFPRINTF (stderr, "%3lu. ",
                 (unsigned long int) (yyp - yystackp->yyitems));
      if (*(yybool *) yyp)
        {
          YYFPRINTF (stderr, "Res: %d, LR State: %d, posn: %lu, pred: %ld",
                     yyp->yystate.yyresolved, yyp->yystate.yylrState,
                     (unsigned long int) yyp->yystate.yyposn,
                     (long int) YYINDEX (yyp->yystate.yypred));
          if (! yyp->yystate.yyresolved)
            YYFPRINTF (stderr, ", firstVal: %ld",
                       (long int) YYINDEX (yyp->yystate
                                             .yysemantics.yyfirstVal));
        }
      else
        {
          YYFPRINTF (stderr, "Option. rule: %d, state: %ld, next: %ld",
                     yyp->yyoption.yyrule - 1,
                     (long int) YYINDEX (yyp->yyoption.yystate),
                     (long int) YYINDEX (yyp->yyoption.yynext));
        }
      YYFPRINTF (stderr, "\n");
    }
  YYFPRINTF (stderr, "Tops:");
  for (yyi = 0; yyi < yystackp->yytops.yysize; yyi += 1)
    YYFPRINTF (stderr, "%lu: %ld; ", (unsigned long int) yyi,
               (long int) YYINDEX (yystackp->yytops.yystates[yyi]));
  YYFPRINTF (stderr, "\n");
}
#endif
/* Line 2575 of glr.c  */
#line 269 "han_compiler.y"


int main(int argc, char *argv[]){
#define debug true
#if debug
	extern FILE * yyin;
	extern FILE * yyout ;
	setOutputCodePath("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_AUTO_INTEGRATE_10_21\\codeTest1out.txt");
	pretreate();
#else
	extern FILE * yyin;
	yyin = fopen(argv[1],"r");
	extern FILE * yyout ;
	 yyout = stdout;
	setOutputCodePath(argv[2]);
	if(yyin==NULL||yyout==NULL){ puts("cant open all File");system("pause");return 1;}
#endif
      
	//pretreate();
	//yylex();
	yyparse();

	puts("yacc>>yacc finish!");
	matchFinish();
#if debug
	system("pause");
#endif
	return 0;
}
void yyerror(const char *s){
	printf("³ö´íÀ²\n");
	printf("%s\n", s);

}
