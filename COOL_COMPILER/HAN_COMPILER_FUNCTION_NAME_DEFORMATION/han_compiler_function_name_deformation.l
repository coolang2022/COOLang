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
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>


	#include "han_compiler_function_name_deformation.hpp"

	void yyerror(const char*);
	int num_num = 0;
	int num_id = 0;
	int word_count = 0;
//	extern char*	yylval;头文件里面有

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
<INITIAL>"\("				{puts("lex>>Find LSCUR");puts(yytext);addToken(Token::LSCUR,yytext);}
<INITIAL>"\)"				{puts("lex>>Find RSCUR");puts(yytext);addToken(Token::RSCUR,yytext);}
<INITIAL>"\["				{puts("lex>>Find LMCUR");puts(yytext);addToken(Token::LMCUR,yytext);}
<INITIAL>"\]"				{puts("lex>>Find RMCUR");puts(yytext);addToken(Token::RMCUR,yytext);}
<INITIAL>"\{"				{puts("lex>>Find LBCUR");puts(yytext);addToken(Token::LBCUR,yytext);}
<INITIAL>"\}"				{puts("lex>>Find RBCUR");puts(yytext);addToken(Token::RBCUR,yytext);}
<INITIAL>","				{puts("lex>>Find COMMA");puts(yytext);addToken(Token::COMMA,yytext);}
<INITIAL>";"				{puts("lex>>Find SEMICOLON");puts(yytext);addToken(Token::SEMICOLON,yytext);}

\"返回\"
<INITIAL>("re:"|"re"|"return"|"return:")	{puts("lex>>Find _return_");puts(yytext);addToken(Token::_return_,yytext);}


\"运算符\"
<INITIAL>"="				{puts("lex>>Find FUZHI");puts(yytext);addToken(Token::FUZHI,yytext);}
<INITIAL>"=="				{puts("lex>>Find DENGYU");puts(yytext);addToken(Token::DENGYU,yytext);}
<INITIAL>"-->"				{puts("lex>>Find SHUCHU");puts(yytext);addToken(Token::SHUCHU,yytext);}
<INITIAL>">"				{puts("lex>>Find DAYU");puts(yytext);addToken(Token::DAYU,yytext);}
<INITIAL>"<"				{puts("lex>>Find XIAOYU");puts(yytext);addToken(Token::XIAOYU,yytext);}
<INITIAL>"+"				{puts("lex>>Find JIA");puts(yytext);addToken(Token::JIA,yytext);}
<INITIAL>"-"				{puts("lex>>Find JIAN");puts(yytext);addToken(Token::JIAN,yytext);}
<INITIAL>"*"				{puts("lex>>Find CHENG");puts(yytext);addToken(Token::CHENG,yytext);}
<INITIAL>"/"				{puts("lex>>Find CHU");puts(yytext);addToken(Token::CHU,yytext);}
<INITIAL>"^"				{puts("lex>>Find MI");puts(yytext);addToken(Token::MI,yytext);}
<INITIAL>":"				{puts("lex>>Find DOTDOT");puts(yytext);addToken(Token::DOTDOT,yytext);}
<INITIAL>"$"				{puts("lex>>Find CHANGEABLE");puts(yytext);addToken(Token::CHANGEABLE,yytext);}
<INITIAL>"#"				{puts("lex>>Find PENDCHANGEABLE");puts(yytext);addToken(Token::PENDCHANGEABLE,yytext);}

\"函数声明属性\"
<INITIAL>"e"/":"			{puts("lex>>Find function_declare_attribute e");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<INITIAL>"@"/(":")			{puts("lex>>Find function_declare_attribute @");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<INITIAL>"@"/(([a-zA-Z_]+[a-zA-Z_0-9]*)"(")		{puts("lex>>Find function_declare_attribute @{FUNCTIONNAME}");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<INITIAL>"@("				{BEGIN ATTRIBUTE;puts("lex>>Find function_declare_attribute @");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<ATTRIBUTE>{SIGNEDNUMBER}			{puts("lex>>Find function_declare_attribute @(number)");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<ATTRIBUTE>")"				{BEGIN INITIAL;puts("lex>>End function_declare_attribute @(number)");puts(yytext);addToken(Token::function_declare_attribute,yytext);}
<ATTRIBUTE>.|\n				{puts("lex>>Unrecognized character in ATTRIBUTE");puts(yytext);}

\"可变属性\"
<INITIAL>"$"/":"			{puts("lex>>Find changeable_attribute $");puts(yytext);addToken(Token::changeable_attribute,yytext);}

\"变量特有属性\"
<INITIAL>"out"/":"			{puts("lex>>Find variable_specific_attribute out");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}
<INITIAL>"new"/":"			{puts("lex>>Find variable_specific_attribute new");puts(yytext);addToken(Token::variable_specific_attribute,yytext);}

\"变量\"

<INITIAL>"\""				{BEGIN STRING;puts("lex>>Find string start");puts(yytext);}
<STRING>[.\n]*/"\""			{puts("lex>>Find string content");puts(yytext);addToken(Token::arg,yytext);}
<STRING>"\""				{puts("lex>>Find string end");puts(yytext);}
<INITIAL>([a-zA-Z_]+[a-zA-Z_0-9]*)/("("|"{")	{
    if (strcmp(yytext, "if") == 0) {
	puts("lex>>Find IF");puts(yytext);
	addToken( Token::IF, yytext);

    } else if (strcmp(yytext, "elif") == 0||strcmp(yytext, "elseif") == 0) {
	puts("lex>>Find ELIF");puts(yytext);
	addToken( Token::ELIF, yytext);

    } else if (strcmp(yytext, "else") == 0) {
	puts("lex>>Find ELSE");puts(yytext);
        addToken( Token::ELSE, yytext);

    } else if (strcmp(yytext, "while") == 0) {
	puts("lex>>Find WHILE");puts(yytext);
        addToken( Token::WHILE, yytext);

    }else{
        puts("lex>>Find function name");puts(yytext);
	addToken(Token::arg,yytext);

    }
}
<INITIAL>([a-zA-Z_]+[a-zA-Z_0-9]*)/([^"("a-zA-Z_0-9]{1})	{
    if (strcmp(yytext, "if") == 0) {
	puts("lex>>Find IF");puts(yytext);
	addToken( Token::arg,  yytext);

    } else if (strcmp(yytext, "elif") == 0||strcmp(yytext, "elseif") == 0) {
	puts("lex>>Find ELIF");puts(yytext);
	addToken( Token::arg, yytext);

    } else if (strcmp(yytext, "else") == 0) {
	puts("lex>>Find ELSE");puts(yytext);
        addToken( Token::arg,  yytext);

    } else if (strcmp(yytext, "while") == 0) {
	puts("lex>>Find WHILE");puts(yytext);
        addToken( Token::arg, yytext);

    }else{
        puts("lex>>Find arg");puts(yytext);
	addToken(Token::arg,yytext);

    }
}
<INITIAL>{UNSIGNEDNUMBER}			{puts("lex>>Find number");puts(yytext);addToken(Token::arg,yytext);}




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


	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_FUNCTION_NAME_DEFORMATION\\codeTest1.txt", "r") )== NULL){
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
	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_FUNCTION_NAME_DEFORMATION\\codeTest1out.txt", "w") )== NULL){
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
	setOutputCodePath("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_FUNCTION_NAME_DEFORMATION\\codeTest1out.txt");
	//yyout=precompileFile();

	if(yyin==NULL||yyout==NULL){ puts("lex>>cant open all File");system("pause");return 1;}
	yylex();
	//printf("num = %d, id = %d\n", num_num, num_id);
	//因为此法分析为一循环，打印总数字数仅当使用文件操作时可读出
	fclose(yyin);
	fclose(yyout);
	system("pause");
	return 0;
}

int yywrap()//此函数必须由用户提供,或者声明
{
    return 1;//返回1报告文件结尾0标记,为0则继续扫描
}

int main(int argc, char *argv[]){

#define debug false
#if debug
	pretreate();
#else
	yyin = fopen(argv[1],"r");
	 yyout = stdout;
	setOutputCodePath(argv[2]);
	if(yyin==NULL||yyout==NULL){ puts("cant open all File");system("pause");return 1;}
	yylex();
	fclose(yyin);
	fclose(yyout);
#endif
	return 0;
}