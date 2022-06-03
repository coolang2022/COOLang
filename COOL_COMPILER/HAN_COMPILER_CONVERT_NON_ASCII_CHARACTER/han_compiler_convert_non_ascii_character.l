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
   #include<cstdio>
#include"han_compiler_convert_non_ascii_character.hpp"
   int num_num = 0;
   int num_id = 0;
   int word_count = 0;

	using namespace HAN_COMPILER;

	FILE * precompileFp;

%}


%s	COMMENT		
%s	ASCIIS
%s	STRING	
%s	STRING_TRANSM
		

INTEGER			[-+]?[1-9][0-9]*
SYSM			"="|"+"|"-"|"*"|"/"|"~"|"`"|"!"|"@"|"#"|"$"|"%"|"^"|"&"|"("|")"
ARG			[0-9a-zA-Z_.]*
SPACE 			[ ��\n\t]+
STRING			\"(((^\")|(\\\"))*)\"
KEYWORD			("��"|"ѭ��"|"����"|"������"|"��"|"��"|"��"|"��"|"��"|"��")
HANZI			[^\x00-\x7f��]*
ASCII			[\x00-\x21\x23-\x7f]*



		

%%
\"ע��\"
<INITIAL>^[{SPACE}\t]*"/*"			{ BEGIN COMMENT; /* �������ע�ʹ���״̬ */ } 
<COMMENT>[.|{SPACE}]*"*/"[ \t]*\n	{ BEGIN 0;  /*����ע�ʹ�������*/} 
<INITIAL>^[{SPACE}\t]*"/*".*"*/"[ \t]*\n		{ /* ����ע�� */    }
<INITIAL>^[{SPACE}\t]*"//".*\n			{/*����ע��*/}

\"�ַ���\"
<INITIAL>[\"]{1}			{BEGIN STRING;puts("�����ַ���");puts("lex>>Find \"1 ");puts(yytext);addToken(Token::arg,yytext);}
<STRING>(\\)				{BEGIN STRING_TRANSM;puts("lex>>Find \\ ");puts(yytext);addToken(Token::arg,yytext);}
<STRING_TRANSM>[.]{1}			{BEGIN STRING;puts("ת�����");puts("lex>>Find STRING_TRANSM");puts(yytext);addToken(Token::arg,yytext);}
<STRING>[^\x22\x5c]*				{puts("lex>>Find STRING");puts(yytext);addToken(Token::arg,yytext);}
<STRING>[\"]{1}				{BEGIN INITIAL;/*�ڶ�������*/puts("�ַ�������");puts("lex>>Find \"2 ");puts(yytext);addToken(Token::arg,yytext);}


<INITIAL>{KEYWORD}			{    if (strcmp(yytext, "��") == 0) {
		puts("lex>>Find IF");puts(yytext);
		addToken( Token::IF, "if");
	
	    } else if (strcmp(yytext, "������") == 0) {
		puts("lex>>Find ELIF");puts(yytext);
		addToken( Token::ELIF, "elif");
	
	    } else if (strcmp(yytext, "����") == 0) {
		puts("lex>>Find ELSE");puts(yytext);
		addToken( Token::ELSE, "else");

	    } else if (strcmp(yytext, "ѭ��") == 0) {
		puts("lex>>Find WHILE");puts(yytext);
		addToken( Token::WHILE, "while");
	
	    } else if (strcmp(yytext, "��") == 0) {
		puts("lex>>Find ZHEN");puts(yytext);
		addToken( Token::arg, "1");
	
	    } else if (strcmp(yytext, "��") == 0) {
		puts("lex>>Find JIA cn");puts(yytext);
		addToken( Token::arg, "0");
	
	    } else if (strcmp(yytext, "��") == 0) {
		puts("lex>>Find SHI");puts(yytext);
	        addToken( Token::arg, "1");
	
	    } else if (strcmp(yytext, "��") == 0) {
		puts("lex>>Find FOU");puts(yytext);
	        addToken( Token::arg, "0");
	
	    }else if (strcmp(yytext, "��") == 0) {
		puts("lex>>Find HAN");puts(yytext);
		addToken( Token::arg, "include");
	
	}
    }

<INITIAL>{SPACE}  	{/*ʲôҲ����,�˵����ַ��������ַ�*/}
<INITIAL>{HANZI}	{
		puts("lex>>Find HANZI");printf("\n����:%s,����:%d,���:%s\n",yytext,yyleng,nonASCIICodeToHex(yytext,yyleng).c_str());
		addToken(Token::changeable_attribute,nonASCIICodeToHex(yytext,yyleng).c_str());
		}
<INITIAL>{ASCII}	{puts("lex>>Find  ASCII");puts(yytext);addToken(Token::arg,yytext);}

%%


FILE * openfile(){

	printf("in openfile\n");
	FILE *fp;
	char ch;


	char * buffer = NULL;
	if((buffer = _getcwd( NULL, 0 )) == NULL)perror( "_getcwd error" );
	printf( "%s \n", buffer );


	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_CONVERT_NON_ASCII_CHARACTER\\codeTest1.txt", "r") )== NULL){
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
	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_CONVERT_NON_ASCII_CHARACTER\\codeTest1out.txt", "w") )== NULL){
		printf("Cannot open file : codeTest1out.txt\n");
		return NULL;
	}
	else{
		return fp;
	}
}

int pretreate()
{

	//[^ \t\n]+       printf("ִ��REJECT");REJECT;

	yyin=openfile();
	yyout=stdout;
	setOutputCodePath("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_CONVERT_NON_ASCII_CHARACTER\\codeTest1out.txt");
	//yyout=precompileFile();

	if(yyin==NULL||yyout==NULL){ puts("lex>>cant open all File");system("pause");return 1;}
	yylex();
	//printf("num = %d, id = %d\n", num_num, num_id);
	//��Ϊ�˷�����Ϊһѭ������ӡ������������ʹ���ļ�����ʱ�ɶ���
	fclose(yyin);
	fclose(yyout);
	//system("pause");
	return 0;
}

int yywrap()//�˺����������û��ṩ,��������
{
    return 1;//����1�����ļ���β0���,Ϊ0�����ɨ��
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