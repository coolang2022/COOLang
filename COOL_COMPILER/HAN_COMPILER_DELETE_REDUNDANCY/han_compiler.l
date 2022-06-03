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

	#ifndef fileno
	#define fileno(__F) ((__F)->_file)
	#endif


	#include "y.tab.h" 

	void yyerror(const char*);
	int num_num = 0;
	int num_id = 0;
	int word_count = 0;
//	extern char*	yylval;ͷ�ļ�������
	void st();

%}


%s	SINGLE_COMMENT
%s	MULTI_COMMENT
%s	STRING	
%s	STRING_TRANSM
		

ID 			[a-zA-Z_0-9]+
SPACE 			[ ��\n\t]+





%%

\"ȥ��ע��\"
<INITIAL>"//"				{BEGIN SINGLE_COMMENT;puts("Find single comment");}
<SINGLE_COMMENT>[\n]{1}			{BEGIN INITIAL;puts("End single comment");}
<SINGLE_COMMENT>.*			{;}
<INITIAL>"/\*"				{BEGIN MULTI_COMMENT;puts("Find multi comment");}
<MULTI_COMMENT>[^\*]*			{;}
<MULTI_COMMENT>"*"+[^\*\/]*		{;}
<MULTI_COMMENT>"\*/"			{BEGIN INITIAL;puts("End multi comment");}


\"ȥ�����ַ����ո�\"
<INITIAL>{SPACE}			{;}

\"�ַ���\"
<INITIAL>"\""(.*\\\")*"\""		{puts("Find string");ECHO;}



<INITIAL>[^"//""\*/""/\*" ��\n\t]*				{ECHO;}
%%


FILE * openfile(){

	printf("in openfile\n");
	FILE *fp;
	char ch;
	//"D:\\Flex Windows\\FlexFiles\\chan\\2.0canshu_keyi_qianru_biaoshifu_zhong\\test.chan"


	char * buffer = NULL;
	if((buffer = _getcwd( NULL, 0 )) == NULL)perror( "_getcwd error" );
	printf( "%s \n", buffer );


	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_DELETE_REDUNDANCY\\codeTest1.txt", "r") )== NULL){
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
	if ((fp = fopen("D:\\Flex Windows\\FlexFiles\\HAN_COMPILER_DELETE_REDUNDANCY\\codeTest1out.txt", "w") )== NULL){
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
	yyout=precompileFile();

	if(yyin==NULL||yyout==NULL){ puts("cant open all File");system("pause");return 1;}
	//yylex();
	//printf("num = %d, id = %d\n", num_num, num_id);
	//��Ϊ�˷�����Ϊһѭ������ӡ������������ʹ���ļ�����ʱ�ɶ���
	//fclose(yyin);
	//fclose(yyout);
	//system("pause");
	return 0;
}

int yywrap()//�˺����������û��ṩ,��������
{
    return 1;//����1�����ļ���β0���,Ϊ0�����ɨ��
}
void st(){

	//strcpy(char *dest,char *src);
	strcpy(yylval.toString(),yytext);
}