/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton interface for Bison GLR parsers in C
   
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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     LMCUR = 258,
     RMCUR = 259,
     LBCUR = 260,
     RBCUR = 261,
     SEMICOLON = 262,
     IF = 263,
     ELSE = 264,
     ELSEIF = 265,
     ELIF = 266,
     WHILE = 267,
     function_declare_attribute = 268,
     system_declare_attribute = 269,
     changeable_attribute = 270,
     variable_specific_attribute = 271,
     attribute = 272,
     arg = 273,
     _return_ = 274,
     LOWEST = 275,
     LOW = 276,
     _return_low = 277,
     _return_high = 278,
     COMMA = 279,
     TUIDAO = 280,
     JICHENG = 281,
     SHUCHU = 282,
     FUZHI = 283,
     XIAOYU = 284,
     DAYU = 285,
     DENGYU = 286,
     JIAN = 287,
     JIA = 288,
     CHU = 289,
     CHENG = 290,
     MI = 291,
     NEG = 292,
     RSCUR = 293,
     LSCUR = 294,
     CALL = 295,
     DOTDOT = 296,
     PENDCHANGEABLE = 297,
     CHANGEABLE = 298,
     DOT = 299,
     HIGHEST = 300
   };
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2579 of glr.c  */
#line 34 "han_compiler.y"

	
	char	str[2048];
	char *  toString(){return str;}
	int argp;
	int exprp;




/* Line 2579 of glr.c  */
#line 112 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
