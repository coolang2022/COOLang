%{
/*  COOLang compiler implementation
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
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>


	#include "y.tab.h"
	#include "han_compiler.hpp"

	void yyerror(const char*);
	int num_num = 0;
	int num_id = 0;
	int word_count = 0;
//	extern char*	yylval;头文件里面有
	void st();

//%token	<str>	LSCUR	RSCUR	LMCUR	RMCUR	LBCUR	RBCUR	COMMA	SEMICOLON
//%token	<argp>	arg
%}


%s	SINGLE_COMMENT
%s	MULTI_COMMENT
%s	STRING	
%s	STRING_TRANSM

%s	ATTRIBUTE
		

ID 			[a-zA-Z_0-9]+
SPACE 			[ 　\n\t]+
SIGNEDNUMBER		[-+]?([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?)
UNSIGNEDNUMBER		([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?)
FUNCTIONNAME		([a-zA-Z_]+[a-zA-Z_0-9]*)/"("



%%

\"提取作用域\"
<INITIAL>"\("				{puts("lex>>Find LSCUR");puts(yytext);yylval.argp=matchToken(1, Token::LSCUR, Bs,yytext);return LSCUR;}
<INITIAL>"\)"				{puts("lex>>Find RSCUR");puts(yytext);yylval.argp=matchToken(1, Token::RSCUR, Bs,yytext);return RSCUR;}
<INITIAL>"\["				{puts("lex>>Find LMCUR");puts(yytext);yylval.argp=matchToken(1, Token::LMCUR, Bs,yytext);return LMCUR;}
<INITIAL>"\]"				{puts("lex>>Find RMCUR");puts(yytext);yylval.argp=matchToken(1, Token::RMCUR, Bs,yytext);return RMCUR;}
<INITIAL>"\{"				{puts("lex>>Find LBCUR");puts(yytext);yylval.argp=matchToken(1, Token::LBCUR, Bs,yytext);return LBCUR;}
<INITIAL>"\}"				{puts("lex>>Find RBCUR");puts(yytext);yylval.argp=matchToken(1, Token::RBCUR, Bs,yytext);return RBCUR;}
<INITIAL>","				{puts("lex>>Find COMMA");puts(yytext);yylval.argp=matchToken(1, Token::COMMA, Bs,yytext);return COMMA;}
<INITIAL>";"				{puts("lex>>Find SEMICOLON");puts(yytext);yylval.argp=matchToken(1, Token::SEMICOLON, Bs,yytext);return SEMICOLON;}

\"返回\"
<INITIAL>("re:"|"re"|"return"|"return:")	{puts("lex>>Find _return_");puts(yytext);yylval.argp=matchToken(1, Token::_return_, Bs,yytext);return _return_;}


\"运算符\"
<INITIAL>"=="				{puts("lex>>Find DENGYU");puts(yytext);yylval.argp=matchToken(1, Token::DENGYU, Bs,yytext);return DENGYU;}
<INITIAL>"=>"				{puts("lex>>Find TUIDAO");puts(yytext);yylval.argp=matchToken(1, Token::TUIDAO, Bs,yytext);return TUIDAO;}
<INITIAL>"<<"				{puts("lex>>Find JICHENG");puts(yytext);yylval.argp=matchToken(1, Token::JICHENG, Bs,yytext);return JICHENG;}
<INITIAL>"="				{puts("lex>>Find FUZHI");puts(yytext);yylval.argp=matchToken(1, Token::FUZHI, Bs,yytext);return FUZHI;}
<INITIAL>"-->"				{puts("lex>>Find SHUCHU");puts(yytext);yylval.argp=matchToken(1, Token::SHUCHU, Bs,yytext);return SHUCHU;}
<INITIAL>">"				{puts("lex>>Find DAYU");puts(yytext);yylval.argp=matchToken(1, Token::DAYU, Bs,yytext);return DAYU;}
<INITIAL>"<"				{puts("lex>>Find XIAOYU");puts(yytext);yylval.argp=matchToken(1, Token::XIAOYU, Bs,yytext);return XIAOYU;}
<INITIAL>"+"				{puts("lex>>Find JIA");puts(yytext);yylval.argp=matchToken(1, Token::JIA, Bs,yytext);return JIA;}
<INITIAL>"-"				{puts("lex>>Find JIAN");puts(yytext);yylval.argp=matchToken(1, Token::JIAN, Bs,yytext);return JIAN;}
<INITIAL>"*"				{puts("lex>>Find CHENG");puts(yytext);yylval.argp=matchToken(1, Token::CHENG, Bs,yytext);return CHENG;}
<INITIAL>"/"				{puts("lex>>Find CHU");puts(yytext);yylval.argp=matchToken(1, Token::CHU, Bs,yytext);return CHU;}
<INITIAL>"^"				{puts("lex>>Find MI");puts(yytext);yylval.argp=matchToken(1, Token::MI, Bs,yytext);return MI;}
<INITIAL>":"				{puts("lex>>Find DOTDOT");puts(yytext);yylval.argp=matchToken(1, Token::DOTDOT, Bs,yytext);return DOTDOT;}
<INITIAL>"$"				{puts("lex>>Find CHANGEABLE");puts(yytext);yylval.argp=matchToken(1, Token::CHANGEABLE, Bs,yytext);return CHANGEABLE;}
<INITIAL>"#"				{puts("lex>>Find PENDCHANGEABLE");puts(yytext);yylval.argp=matchToken(1, Token::PENDCHANGEABLE, Bs,yytext);return PENDCHANGEABLE;}
<INITIAL>"."				{puts("lex>>Find DOT");puts(yytext);yylval.argp=matchToken(1, Token::DOT, Bs,yytext);return DOT;}

\"函数声明属性\"
<INITIAL>("e"|"exp"|"expression")/":"			{puts("lex>>Find function_declare_attribute e");puts(yytext);yylval.argp=matchToken(1, Token::function_declare_attribute, Bs,yytext);return function_declare_attribute;}
<INITIAL>"!"/":"			{puts("lex>>Find function_declare_attribute !");puts(yytext);yylval.argp=matchToken(1, Token::function_declare_attribute, Bs,yytext);return function_declare_attribute;}
<INITIAL>"@"/(":")			{puts("lex>>Find function_declare_attribute @");puts(yytext);yylval.argp=matchToken(1, Token::function_declare_attribute, Bs,yytext);return function_declare_attribute;}
<INITIAL>"@"/(([a-zA-Z_]+[a-zA-Z_0-9]*)"(")		{puts("lex>>Find function_declare_attribute @{FUNCTIONNAME}");puts(yytext);yylval.argp=matchToken(1, Token::function_declare_attribute, Bs,yytext);return function_declare_attribute;}
<INITIAL>"@("				{BEGIN ATTRIBUTE;puts("lex>>Find function_declare_attribute @");puts(yytext);}
<ATTRIBUTE>{SIGNEDNUMBER}			{puts("lex>>Find function_declare_attribute @(number)");puts(yytext);yylval.argp=matchToken(1, Token::function_declare_attribute, Sz,yytext);return function_declare_attribute;}
<ATTRIBUTE>")"				{BEGIN INITIAL;puts("lex>>End function_declare_attribute @(number)");puts(yytext);}
<ATTRIBUTE>.|\n				{puts("lex>>Unrecognized character in ATTRIBUTE");puts(yytext);}

\"可变属性\"
<INITIAL>"$"/":"			{puts("lex>>Find changeable_attribute $");puts(yytext);yylval.argp=matchToken(1, Token::changeable_attribute, Bs,yytext);return changeable_attribute;}

\"变量特有属性\"
<INITIAL>"out"/":"			{puts("lex>>Find variable_specific_attribute out");puts(yytext);yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yytext);return variable_specific_attribute;}
<INITIAL>"new"/":"			{puts("lex>>Find variable_specific_attribute new");puts(yytext);yylval.argp=matchToken(1, Token::variable_specific_attribute, Bs,yytext);return variable_specific_attribute;}

\"class、system属性\"
<INITIAL>"system"/":"			{puts("lex>>Find system_declare_attribute system");puts(yytext);yylval.argp=matchToken(1, Token::system_declare_attribute, Bs,yytext);return system_declare_attribute;}
<INITIAL>"class"/":"			{puts("lex>>Find system_declare_attribute class");puts(yytext);yylval.argp=matchToken(1, Token::system_declare_attribute, Bs,yytext);return system_declare_attribute;}

\"变量\"
<INITIAL>"\""				{BEGIN STRING;puts("lex>>Find string start");puts("[");puts(yytext);puts("]");}
<STRING>[^\n"]*				{puts("lex>>Find string content");puts(yytext);yylval.argp=matchToken(1, Token::arg,Zf,yytext);return arg;}
<STRING>"\""				{BEGIN INITIAL;puts("lex>>Find string end");puts(yytext);}
<INITIAL>([a-zA-Z_]+[a-zA-Z_0-9]*)/("(")	{
    if (strcmp(yytext, "if") == 0) {
	puts("lex>>Find IF");puts(yytext);
	yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return IF;
    } else if (strcmp(yytext, "elif") == 0||strcmp(yytext, "elseif") == 0) {
	puts("lex>>Find ELIF");puts(yytext);
	yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return ELIF;
    } else if (strcmp(yytext, "else") == 0) {
	puts("lex>>Find ELSE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return ELSE;
    } else if (strcmp(yytext, "while") == 0) {
	puts("lex>>Find WHILE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return WHILE;
    }else{
        puts("lex>>Find function name");puts(yytext);
	yylval.argp=matchToken(1, Token::arg,Zf,yytext);
	return arg;
    }
}
<INITIAL>([a-zA-Z_]+[a-zA-Z_0-9]*)/([^"("a-zA-Z_0-9]{1})	{
    if (strcmp(yytext, "if") == 0) {
	puts("lex>>Find IF");puts(yytext);
	yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return IF;
    } else if (strcmp(yytext, "elif") == 0||strcmp(yytext, "elseif") == 0) {
	puts("lex>>Find ELIF");puts(yytext);
	yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return ELIF;
    } else if (strcmp(yytext, "else") == 0) {
	puts("lex>>Find ELSE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return ELSE;
    } else if (strcmp(yytext, "while") == 0) {
	puts("lex>>Find WHILE");puts(yytext);
        yylval.argp = matchToken(1, Token::arg, Bs, yytext);
        return WHILE;
    }else{
        puts("lex>>Find arg");puts(yytext);
	yylval.argp=matchToken(1, Token::arg,Bs,yytext);
	return arg;
    }
}
<INITIAL>{UNSIGNEDNUMBER}			{puts("lex>>Find number");puts(yytext);yylval.argp=matchToken(1, Token::arg,Sz,yytext);return arg;}




\"未识别字符\"
<INITIAL>.|\n				{puts("lex>>Unrecognized character");puts(yytext);}
%%

FILE * openfile(){

	printf("in openfile\n");
	FILE *fp;
	char ch;


	char * buffer = NULL;
	if((buffer = _getcwd( NULL, 0 )) == NULL)perror( "_getcwd error" );
	printf( "%s \n", buffer );


	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_AUTO_INTEGRATE_10_21\\codeTest1.txt", "r") )== NULL){
		printf("Cannot open file : codeTest1.txt\n");
		return NULL;
	}
	else{
        
		return fp;
	}

}

FILE * precompileFile(){
	printf("in precompileFile\n");
	FILE *fp;
	char ch;
	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_AUTO_INTEGRATE_10_21\\codeTest1out.txt", "w") )== NULL){
		printf("Cannot open file : codeTest1out.txt\n");
		return NULL;
	}
	else{
		return fp;
	}
}

int pretreate()
{

	//[^ \t\n]+       printf("执行REJECT");REJECT;

	yyin=openfile();
	yyout=stdout;

	if(yyin==NULL||yyout==NULL){ puts("lex>>cant open all File");system("pause");return 1;}
	//yylex();
	//printf("num = %d, id = %d\n", num_num, num_id);
	//因为此法分析为一循环，打印总数字数仅当使用文件操作时可读出
	//fclose(yyin);
	//fclose(yyout);
	//system("pause");
	return 0;
}

int yywrap()//此函数必须由用户提供,或者声明
{
    return 1;//返回1报告文件结尾0标记,为0则继续扫描
}
void st(){

	//strcpy(char *dest,char *src);
	strcpy(yylval.toString(),yytext);

}
