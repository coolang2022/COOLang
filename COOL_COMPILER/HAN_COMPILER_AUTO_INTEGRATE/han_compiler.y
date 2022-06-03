%{
/*  COOLang Compiler
    Copyright (C) 2022,Han JiPeng,Beijing Huagui Technology Co., Ltd

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, version LGPL-3.0-or-later.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
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

	void	setCurrentAssemblyCode();//C++实现


%}
%union{
	
	char	str[2048];
	char *  toString(){return str;}
	int argp;
	int exprp;


}


%token	<argp>	LMCUR	RMCUR	LBCUR	RBCUR	SEMICOLON
%token	<argp>  IF ELSE ELSEIF ELIF WHILE
%token	<argp>	function_declare_attribute	system_declare_attribute
%token	<argp>	changeable_attribute
%token	<argp>	variable_specific_attribute
%token	<argp>	attribute
%token	<argp>	arg
%token	<argp>	_return_



%type	<argp>	sentence	sentence_null
%type	<argp>	scopeBegin	scopeHalf	scope	scopeFunctionDeclareBegin	scopeFunctionDeclareHalf	scopeFunctionDeclareEnd	scopeFunctionDeclare
%type	<argp>	autoScopeBegin	autoScopeEnd	autoScopeHalf	autoScope
%type	<argp>	functionDeclare	functionDeclareBegin	functionDeclareHalf	functionImplement
%type	<argp>	systemDeclare	systemDeclareBegin	systemDeclareHalf	systemImplement
%type	<argp>	_arg_
%type	<argp>	expression	expression_null	expressionFunctionDeclareRoot
%type	<argp>	branch		branchBegin	branchHalf	branchEnd
%type	<argp>	loop
%type	<argp>	environment	environment_null

%right	<argp>	LOWEST
%right	<argp>	LOW
%left	<argp>	_return_low
%left	<argp>	_return_high
%left	<argp>	COMMA
%left	<argp>	TUIDAO
%left	<argp>	JICHENG
%right	<argp>	SHUCHU
%right	<argp>	FUZHI
%left	<argp>	DENGYU	DAYU	XIAOYU	
%left	<argp>	JIA	JIAN	
%left	<argp>	CHENG	CHU
%left	<argp>	MI
%nonassoc <argp> NEG
%nonassoc <argp> LSCUR	RSCUR	
%left	<argp>	CALL	//用RSCUR的优先级代表CALL的优先级，因为CALL运算符始终无法出现
%right	<argp>	DOTDOT
%right	<argp>	CHANGEABLE PENDCHANGEABLE
%left	<argp>	DOT
%right	<argp>	HIGHEST

%debug
%glr-parser


%start	code	
%error-verbose

%% 
code:	
		environment			%dprec 1				{puts("yacc>>code:environment");matchRule(0, Rule::code___environment, NULL,NULL,NULL,NULL);}
	|	code	environment		%dprec 2				{puts("yacc>>code:code	environment");matchRule(0, Rule::code___code_environment, NULL,NULL,NULL,NULL);}
	;
environment_null:									{puts("yacc>>environment_null:NULL");matchRule(0, Rule::environment_null___NULL, NULL,NULL,NULL,NULL);}
	;
environment:	functionImplement		%dprec 5				{puts("yacc>>environment:functionImplement");matchRule(0, Rule::environment___functionImplement, NULL,NULL,NULL,NULL);}
	|	systemImplement			%dprec 5				{puts("yacc>>environment:systemImplement");matchRule(0, Rule::environment___systemImplement, NULL,NULL,NULL,NULL);}
	|	scope				%dprec 3				{puts("yacc>>environment:scope");matchRule(0, Rule::environment___scope, NULL,NULL,NULL,NULL);}
	|	sentence			%dprec 2				{puts("yacc>>environment:sentence");matchRule(0, Rule::environment___sentence, NULL,NULL,NULL,NULL);}
	;

_arg_:		arg									{puts("yacc>>_arg_:arg");matchRule(0, Rule::_arg____arg, &$1,NULL,NULL,&$$);}
	|	CHANGEABLE	arg							{puts("yacc>>_arg_:CHANGEABLE	arg");matchRule(1, Rule::_arg____CHANGEABLE_arg,&$2,NULL,&$1,&$$);}	
	|	PENDCHANGEABLE	arg							{puts("yacc>>_arg_:PENDCHANGEABLE	arg");matchRule(1, Rule::_arg____PENDCHANGEABLE_arg,&$2,NULL,&$1,&$$);}	
	|	variable_specific_attribute	DOTDOT	_arg_				{puts("yacc>>_arg_:variable_specific_attribute	DOTDOT	_arg_");matchRule(1, Rule::_arg____variable_specific_attribute_DOTDOT__arg_, &$3,&$1,&$2,&$$);}
	;

expression_null:									{puts("yacc>>expression_null:NULL");matchRule(0, Rule::expression_null___NULL, NULL,NULL,NULL,NULL);}
	;
expression:	_arg_									{puts("yacc>>expression:_arg_");matchRule(0, Rule::expression____arg_,&$1,NULL,NULL,&$$);}
	|	JIAN	expression				%prec	NEG		{puts("yacc>>expression:JIAN	expression");matchRule(0, Rule::expression___JIAN_expression,NULL,&$2,&$1,&$$);}
	|	JIA	expression				%prec	NEG		{puts("yacc>>expression:JIA	expression");matchRule(0, Rule::expression___JIA_expression,NULL,&$2,&$1,&$$);}
	|	expression	DOT	expression		%prec	DOT		{puts("yacc>>expression:expression	DOT	expression");matchRule(1, Rule::expression___expression_DOT_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIA	expression		%prec	JIA		{puts("yacc>>expression:expression	JIA	expression");matchRule(1, Rule::expression___expression_JIA_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIAN	expression		%prec	JIAN		{puts("yacc>>expression:expression	JIAN	expression");matchRule(1, Rule::expression___expression_JIAN_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHENG	expression		%prec	CHENG		{puts("yacc>>expression:expression	CHENG	expression");matchRule(1, Rule::expression___expression_CHENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHU	expression		%prec	CHU		{puts("yacc>>expression:expression	CHU	expression");matchRule(1, Rule::expression___expression_CHU_expression,&$1,&$3,&$2,&$$);}
	|	expression	MI	expression		%prec	MI		{puts("yacc>>expression:expression	MI	expression");matchRule(1, Rule::expression___expression_MI_expression,&$1,&$3,&$2,&$$);}
	|	expression	FUZHI	expression		%prec	FUZHI		{puts("yacc>>expression:expression	FUZHI	expression");matchRule(1, Rule::expression___expression_FUZHI_expression,&$1,&$3,&$2,&$$);}
	|	expression	DENGYU	expression		%prec	DENGYU		{puts("yacc>>expression:expression	DENGYU	expression");matchRule(1, Rule::expression___expression_DENGYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	DAYU	expression		%prec	DAYU		{puts("yacc>>expression:expression	DAYU	expression");matchRule(1, Rule::expression___expression_DAYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	XIAOYU	expression		%prec	XIAOYU		{puts("yacc>>expression:expression	XIAOYU	expression");matchRule(1, Rule::expression___expression_XIAOYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	DOTDOT	expression		%prec	DOTDOT		{puts("yacc>>expression:expression	DOTDOT	expression");matchRule(1, Rule::expression___expression_DOTDOT_expression,&$3,&$1,&$2,&$$);}
	|	expression	COMMA	expression		%prec	COMMA		{puts("yacc>>expression:expression	COMMA	expression");matchRule(1, Rule::expression___expression_COMMA_expression,&$1,&$3,&$2,&$$);}
	|	expression	SHUCHU	expression		%prec	SHUCHU		{puts("yacc>>expression:expression	SHUCHU	expression");matchRule(1, Rule::expression___expression_SHUCHU_expression,&$1,&$3,&$2,&$$);}
	|	LSCUR	expression	RSCUR						{puts("yacc>>expression:LSCUR	expression	RSCUR");matchRule(0, Rule::expression___LSCUR_expression_RSCUR,&$2,NULL,NULL,&$$);}
	|	expression	LSCUR	expression_null	RSCUR	%prec	LSCUR		{puts("yacc>>expression:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_null_RSCUR,&$1,NULL,NULL,&$$);}
	|	expression	LSCUR	expression	RSCUR	%prec	LSCUR		{puts("yacc>>expression:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expression___expression_LSCUR_expression_RSCUR,&$1,&$3,NULL,&$$);}
	;

sentence_null:										{puts("yacc>>sentence_null:NULL");matchRule(1, Rule::sentence_null___NULL,NULL,NULL,NULL,NULL);}	
	;
sentence:	SEMICOLON					%dprec	1		{puts("yacc>>sentence:SEMICOLON");matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);}
	|	expression	SEMICOLON			%dprec	5		{puts("yacc>>sentence:expression	SEMICOLON");matchRule(1, Rule::sentence___expression_SEMICOLON,NULL,NULL,NULL,NULL);}
	|	functionDeclare	SEMICOLON			%dprec	2		{puts("yacc>>sentence:functionDeclare	SEMICOLON");matchRule(1, Rule::sentence___functionDeclare_SEMICOLON,NULL,NULL,NULL,NULL);}
	|	systemDeclare	SEMICOLON			%dprec	2		{puts("yacc>>sentence:systemDeclare	SEMICOLON");matchRule(1, Rule::sentence___systemDeclare_SEMICOLON,NULL,NULL,NULL,NULL);}
	|	functionImplement				%dprec	2		{puts("yacc>>sentence:functionImplement	SEMICOLON");matchRule(1, Rule::sentence___functionImplement,NULL,NULL,NULL,NULL);}
	|	systemImplement					%dprec	2		{puts("yacc>>sentence:systemImplement	SEMICOLON");matchRule(1, Rule::sentence___systemImplement,NULL,NULL,NULL,NULL);}
	|	_return_	SEMICOLON			%dprec	3		{puts("yacc>>sentence:_return_ SEMICOLON");matchRule(1, Rule::sentence____return__SEMICOLON,NULL,NULL,NULL,NULL);}	
	|	_return_	expression	SEMICOLON	%dprec	4		{puts("yacc>>sentence:_return_	expression SEMICOLON");matchRule(1, Rule::sentence____return__expression_SEMICOLON,&$2,NULL,NULL,NULL);}
	|	branch									{puts("yacc>>sentence:branch");matchRule(1, Rule::sentence___branch,NULL,NULL,NULL,NULL);}
	|	loop									{puts("yacc>>sentence:loop");matchRule(1, Rule::sentence___loop,NULL,NULL,NULL,NULL);}
	;

scopeBegin:	LBCUR									{puts("yacc>>scopeBegin:LBCUR");matchRule(1, Rule::scopeBegin___LBCUR,&$1,NULL,NULL,&$$);}
	;

scopeHalf:	scopeBegin	sentence						{puts("yacc>>scopeHalf:scopeBegin	sentence");matchRule(0, Rule::scopeHalf___scopeBegin_sentence,&$1,NULL,NULL,&$$);}
	|	scopeHalf	sentence						{puts("yacc>>scopeHalf:scopeHalf	sentence");matchRule(0, Rule::scopeHalf___scopeHalf_sentence,&$1,NULL,NULL,&$$);}
//	|	scopeHalf	functionImplement	%dprec 5			{puts("yacc>>scopeHalf:scopeHalf	functionImplement");matchRule(0, Rule::scopeHalf___scopeHalf_functionImplement,&$1,NULL,NULL,&$$);}
//	|	scopeHalf	functionDeclare		%dprec 4			{puts("yacc>>scopeHalf:scopeHalf	functionDeclare");matchRule(0, Rule::scopeHalf___scopeHalf_functionDeclare,&$1,NULL,NULL,&$$);}
//	|	scopeHalf	systemImplement		%dprec 5			{puts("yacc>>scopeHalf:scopeHalf	systemImplement");matchRule(0, Rule::scopeHalf___scopeHalf_systemImplement,&$1,NULL,NULL,&$$);}
//	|	scopeHalf	systemDeclare		%dprec 4			{puts("yacc>>scopeHalf:scopeHalf	systemDeclare");matchRule(0, Rule::scopeHalf___scopeHalf_systemDeclare,&$1,NULL,NULL,&$$);}
	|	scopeHalf	scope			%dprec 3			{puts("yacc>>scopeHalf:scopeHalf	scope");matchRule(0, Rule::scopeHalf___scopeHalf_scope,&$1,NULL,NULL,&$$);}
	;

scope:		scopeHalf	RBCUR							{puts("yacc>>scope:scopeHalf	RBCUR");matchRule(1, Rule::scope___scopeHalf_RBCUR,&$1,NULL,NULL,&$$);}
	|	scopeBegin	RBCUR							{puts("yacc>>scope:scopeBegin	RBCUR");matchRule(1, Rule::scope___scopeBegin_RBCUR,&$1,NULL,NULL,&$$);}
	;






expressionFunctionDeclareRoot:
		JIAN	expression				%prec	NEG		{puts("yacc>>expression:JIAN	expression");matchRule(0, Rule::expressionFunctionDeclareRoot___JIAN_expression,NULL,&$2,&$1,&$$);}
	|	JIA	expression				%prec	NEG		{puts("yacc>>expression:JIA	expression");matchRule(0, Rule::expressionFunctionDeclareRoot___JIA_expression,NULL,&$2,&$1,&$$);}
	|	expression	JIA	expression		%prec	JIA		{puts("yacc>>expressionFunctionDeclareRoot:expression	JIA	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_JIA_expression,&$1,&$3,&$2,&$$);}
	|	expression	JIAN	expression		%prec	JIAN		{puts("yacc>>expressionFunctionDeclareRoot:expression	JIAN	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_JIAN_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHENG	expression		%prec	CHENG		{puts("yacc>>expressionFunctionDeclareRoot:expression	CHENG	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_CHENG_expression,&$1,&$3,&$2,&$$);}
	|	expression	CHU	expression		%prec	CHU		{puts("yacc>>expressionFunctionDeclareRoot:expression	CHU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_CHU_expression,&$1,&$3,&$2,&$$);}
	|	expression	MI	expression		%prec	MI		{puts("yacc>>expressionFunctionDeclareRoot:expression	MI	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_MI_expression,&$1,&$3,&$2,&$$);}
	|	expression	FUZHI	expression		%prec	FUZHI		{puts("yacc>>expressionFunctionDeclareRoot:expression	FUZHI	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_FUZHI_expression,&$1,&$3,&$2,&$$);}
	|	expression	DENGYU	expression		%prec	DENGYU		{puts("yacc>>expressionFunctionDeclareRoot:expression	DENGYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DENGYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	DAYU	expression		%prec	DAYU		{puts("yacc>>expressionFunctionDeclareRoot:expression	DAYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DAYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	XIAOYU	expression		%prec	XIAOYU		{puts("yacc>>expressionFunctionDeclareRoot:expression	XIAOYU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_XIAOYU_expression,&$1,&$3,&$2,&$$);}
	|	expression	DOTDOT	expression		%prec	DOTDOT		{puts("yacc>>expressionFunctionDeclareRoot:expression	DOTDOT	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_DOTDOT_expression,&$3,&$1,&$2,&$$);}
	|	expression	COMMA	expression		%prec	COMMA		{puts("yacc>>expressionFunctionDeclareRoot:expression	COMMA	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_COMMA_expression,&$1,&$3,&$2,&$$);}
	|	expression	SHUCHU	expression		%prec	SHUCHU		{puts("yacc>>expressionFunctionDeclareRoot:expression	SHUCHU	expression");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_SHUCHU_expression,&$1,&$3,&$2,&$$);}
	|	LSCUR	expressionFunctionDeclareRoot	RSCUR				{puts("yacc>>expressionFunctionDeclareRoot:LSCUR	expressionFunctionDeclareRoot	RSCUR");matchRule(0, Rule::expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR,&$2,NULL,NULL,&$$);}
	|	expression	LSCUR	expression_null	RSCUR	%prec	CALL		{puts("yacc>>expressionFunctionDeclareRoot:expression	LSCUR	expression_null	RSCUR");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR,&$1,NULL,NULL,&$$);}
	|	expression	LSCUR	expression	RSCUR	%prec	CALL		{puts("yacc>>expressionFunctionDeclareRoot:expression	LSCUR	expression	RSCUR");matchRule(1, Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR,&$1,&$3,NULL,&$$);}
	;

scopeFunctionDeclareBegin:	LBCUR									{puts("yacc>>scopeFunctionDeclareBegin:LBCUR");matchRule(1, Rule::scopeFunctionDeclareBegin___LBCUR,&$1,NULL,NULL,&$$);}
	;

scopeFunctionDeclareHalf:	scopeFunctionDeclareBegin	sentence				{puts("yacc>>scopeFunctionDeclareHalf:scopeFunctionDeclareBegin	sentence");matchRule(0, Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence,&$1,NULL,NULL,&$$);}
	|	scopeFunctionDeclareHalf	sentence						{puts("yacc>>scopeFunctionDeclareHalf:scopeFunctionDeclareHalf	sentence");matchRule(0, Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence,&$1,NULL,NULL,&$$);}
	;
scopeFunctionDeclareEnd:	scopeFunctionDeclareBegin	expressionFunctionDeclareRoot	%dprec	1	{puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareBegin	expressionFunctionDeclareRoot");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot,&$1,NULL,NULL,&$$);}
	|	scopeFunctionDeclareBegin	expressionFunctionDeclareRoot	SEMICOLON	%dprec	2	{puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareBegin	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON,&$1,NULL,NULL,&$$);}	
	|	scopeFunctionDeclareHalf	expressionFunctionDeclareRoot			%dprec	3	{puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareHalf	expressionFunctionDeclareRoot");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot,&$1,NULL,NULL,&$$);}
	|	scopeFunctionDeclareHalf	expressionFunctionDeclareRoot	SEMICOLON	%dprec	4	{puts("yacc>>scopeFunctionDeclareEnd:scopeFunctionDeclareHalf	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON,&$1,NULL,NULL,&$$);}
	;
scopeFunctionDeclare:		scopeFunctionDeclareEnd	RBCUR							{puts("yacc>>scopeFunctionDeclare:scopeFunctionDeclareEnd	RBCUR");matchRule(1, Rule::scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR,&$1,NULL,NULL,&$$);}
	;
autoScopeBegin:										{puts("yacc>>autoScopeBegin:NULL");$$=0;matchRule(1, Rule::autoScopeBegin___NULL,NULL,NULL,NULL,&$$);}
	;

autoScopeEnd:									%dprec 1	{puts("yacc>>autoScopeEnd:NULL");$$=0;matchRule(0, Rule::autoScopeEnd___NULL,NULL,NULL,NULL,NULL);}
//	|	SEMICOLON							%dprec 2	{puts("yacc>>autoScopeEnd:NULL");$$=0;matchRule(0, Rule::autoScopeEnd___NULL,NULL,NULL,NULL,NULL);}
	;

autoScopeHalf:	autoScopeBegin	expressionFunctionDeclareRoot			%dprec 1	{puts("yacc>>autoScopeHalf:autoScopeBegin	expressionFunctionDeclareRoot");matchRule(0, Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot,&$1,NULL,NULL,&$$);}
//	|	autoScopeBegin	expressionFunctionDeclareRoot	SEMICOLON	%dprec 2	{puts("yacc>>autoScopeHalf:autoScopeBegin	expressionFunctionDeclareRoot	SEMICOLON");matchRule(0, Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot_SEMICOLON,&$1,NULL,NULL,&$$);}
	;

autoScope:	autoScopeHalf	autoScopeEnd			%dprec 1			{puts("yacc>>autoScope:autoScopeHalf	autoScopeEnd");matchRule(1, Rule::autoScope___autoScopeHalf_autoScopeEnd,&$1,NULL,NULL,&$$);}
//	|	autoScopeHalf	SEMICOLON	autoScopeEnd	%dprec 2			{puts("yacc>>autoScope:autoScopeHalf	autoScopeEnd");matchRule(1, Rule::autoScope___autoScopeHalf_autoScopeEnd,&$1,NULL,NULL,&$$);}
	;

functionDeclareBegin:
		function_declare_attribute	DOTDOT	autoScope			{puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	autoScope");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope,&$3,&$1,&$2,&$$);}
	|	function_declare_attribute	DOTDOT	scopeFunctionDeclare		{puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	scopeFunctionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare,&$3,&$1,&$2,&$$);}
	|	function_declare_attribute	scopeFunctionDeclare			{puts("yacc>>functionDeclareBegin:function_declare_attribute	scopeFunctionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare,&$2,&$1,NULL,&$$);}
	|	function_declare_attribute	autoScope				{puts("yacc>>functionDeclareBegin:function_declare_attribute	autoscope");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_autoscope,&$2,&$1,NULL,&$$);}
	|	function_declare_attribute	functionDeclareBegin			{puts("yacc>>functionDeclareBegin:function_declare_attribute	functionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_functionDeclareBegin,&$2,&$1,NULL,&$$);}
	|	function_declare_attribute	DOTDOT	functionDeclareBegin		{puts("yacc>>functionDeclareBegin:function_declare_attribute	DOTDOT	functionDeclare");matchRule(1, Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin,&$3,&$1,&$2,&$$);}
	;
functionDeclareHalf:	functionDeclareBegin	JICHENG	arg				{puts("yacc>>functionDeclareHalf:functionDeclareBegin	JICHENG	arg");matchRule(1, Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg,&$1,&$3,&$2,&$$);}
	|		functionDeclareHalf	COMMA	arg				{puts("yacc>>functionDeclareHalf:functionDeclareHalf	COMMA	arg");matchRule(1, Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg,&$1,&$3,&$2,&$$);}
	;
functionDeclare:	functionDeclareBegin						{puts("yacc>>functionDeclare:functionDeclareBegin");matchRule(0, Rule::functionDeclare___functionDeclareBegin,&$1,NULL,NULL,&$$);}
	|		functionDeclareHalf						{puts("yacc>>functionDeclare:functionDeclareHalf");matchRule(0, Rule::functionDeclare___functionDeclareHalf,&$1,NULL,NULL,&$$);}
	;
functionImplement:	functionDeclare	scope						{puts("yacc>>functionImplement:functionDeclare	scope");matchRule(1, Rule::functionImplement___functionDeclare_scope,&$1,&$2,NULL,&$$);}
	|		functionImplement	TUIDAO	functionDeclare			{puts("yacc>>functionImplement:functionImplement	TUIDAO	functionDeclare");matchRule(1, Rule::functionImplement___functionImplement_TUIDAO_functionDeclare,&$3,&$1,&$2,&$$);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);matchRule(1, Rule::sentence___SEMICOLON,NULL,NULL,NULL,NULL);}
	;

systemDeclareBegin:	system_declare_attribute	DOTDOT	arg			{puts("yacc>>systemDeclareBegin:system_declare_attribute	DOTDOT	arg");matchRule(1, Rule::systemDeclareBegin___system_declare_attribute_DOTDOT_arg,&$3,&$1,&$2,&$$);}
	;
systemDeclareHalf:	systemDeclareBegin	JICHENG	arg				{puts("yacc>>systemDeclareHalf:systemDeclareBegin	JICHENG	arg");matchRule(1, Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg,&$1,&$3,&$2,&$$);}
	|		systemDeclareHalf	COMMA	arg				{puts("yacc>>systemDeclareHalf:systemDeclareHalf	COMMA	arg");matchRule(1, Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg,&$1,&$3,&$2,&$$);}
	;


systemDeclare:	systemDeclareBegin							{puts("yacc>>systemDeclare:systemDeclareBegin");matchRule(0, Rule::systemDeclare___systemDeclareBegin,&$1,NULL,NULL,&$$);}
	|	systemDeclareHalf							{puts("yacc>>systemDeclare:systemDeclareHalf");matchRule(0, Rule::systemDeclare___systemDeclareHalf,&$1,NULL,NULL,&$$);}
	;
systemImplement:	systemDeclare	scope						{puts("yacc>>systemImplement:systemDeclare	scope");matchRule(1, Rule::systemImplement___systemDeclare_scope,&$1,&$2,NULL,&$$);}
	;
branchBegin:	IF	LSCUR	expression	RSCUR	autoScope			{puts("yacc>>branchBegin:IF	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::branchBegin___IF_LSCUR_expression_RSCUR_autoScope,NULL,&$3,&$5,&$$);}
	|	IF	LSCUR	expression	RSCUR	scope				{puts("yacc>>branchBegin:IF	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::branchBegin___IF_LSCUR_expression_RSCUR_scope,NULL,&$3,&$5,&$$);}
	;
branchHalf:	branchBegin	ELIF	LSCUR	expression	RSCUR	autoScope	{puts("yacc>>branchHalf:branchBegin	ELIF	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope,&$1,&$4,&$6,&$$);}
	|	branchBegin	ELIF	LSCUR	expression	RSCUR	scope		{puts("yacc>>branchHalf:branchBegin	ELIF	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope,&$1,&$4,&$6,&$$);}
	;
branchEnd:	branchBegin	ELSE	autoScope					{puts("yacc>>branchEnd:branchBegin	ELSE	autoScope");matchRule(1, Rule::branchEnd___branchBegin_ELSE__autoScope,&$1,NULL,&$3,&$$);}
	|	branchBegin	ELSE	scope						{puts("yacc>>branchEnd:branchBegin	ELSE	scope");matchRule(1,  Rule::branchEnd___branchBegin_ELSE__scope,&$1,NULL,&$3,&$$);}
	|	branchHalf	ELSE	autoScope					{puts("yacc>>branchEnd:branchHalf	ELSE	autoScope");matchRule(1, Rule::branchEnd___branchHalf_ELSE__autoScope,&$1,NULL,&$3,&$$);}
	|	branchHalf	ELSE	scope						{puts("yacc>>branchEnd:branchHalf	ELSE	scope");matchRule(1,  Rule::branchEnd___branchHalf_ELSE__scope,&$1,NULL,&$3,&$$);}
	;
branch:		branchBegin								{puts("yacc>>branch:branchBegin");matchRule(0, Rule::branch___branchBegin,&$1,NULL,NULL,&$$);}
	|	branchHalf								{puts("yacc>>branch:branchHalf");matchRule(0,  Rule::branch___branchHalf,&$1,NULL,NULL,&$$);}
	|	branchEnd								{puts("yacc>>branch:branchEnd");matchRule(0, Rule::branch___branchEnd,&$1,NULL,NULL,&$$);}
	;

loop:		WHILE	LSCUR	expression	RSCUR	autoScope			{puts("yacc>>loop:WHILE	LSCUR	expression	RSCUR	autoScope");matchRule(1, Rule::loop___WHILE_LSCUR_expression_RSCUR_autoScope,&$$,&$3,&$5,NULL);}
	|	WHILE	LSCUR	expression	RSCUR	scope				{puts("yacc>>loop:WHILE	LSCUR	expression	RSCUR	scope");matchRule(1, Rule::loop___WHILE_LSCUR_expression_RSCUR_scope,&$$,&$3,&$5,NULL);}
	;
%%

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
	printf("出错啦\n");
	printf("%s\n", s);

}
