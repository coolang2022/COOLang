# COOLang
这是COOLang的编译器与虚拟机的源码
  1 “COOL_COMPILER”，编译器代码，其中包括：
  a 预编译部分：
    HAN_COMPILER_DELETE_REDUNDANCY，用于删除空格注释等；
    HAN_COMPILER_CONVERT_NON_ASCII_CHARACTER，用于将非ASCII码字符转成ASCII码字符；
    HAN_COMPILER_FUNCTION_NAME_DEFORMATION，用于将对函数名字进行变形；
  b 编译部分：
    HAN_COMPILER_AUTO_INTEGRATE，生成字符码
  在编译器中的debug宏为true时，“codeTest1.txt”为输入的代码的文件，“codeTest1out.txt”为输出的代码的文件；在debug宏为false时，调用程序时需要在命令行中指明输入输出文件；
  
  2 “COOL_VM”，虚拟机代码，在虚拟机中的debug宏为true时，“assemblyCode.txt”为输入的代码的文件；在debug宏为false时，调用程序时需要在命令行中指明输入的代码的文件；
   
  3 “grammar”，COOL的语法说明
  
  4 “code for test”，测试用代码，包含了一些代码示例


This is the source code of COOLang's compiler and virtual machine
1 "COOL_COMPILER", the compiler code, which includes:
  a. Precompilation part: 
    HAN_COMPILER_DELETE_REDUNDANCY, used to delete space comments, etc.;            
    HAN_COMPILER_CONVERT_NON_ASCII_CHARACTER, used to convert non-ASCII characters into ASCII characters;
    HAN_COMPILER_FUNCTION_NAME_DEFORMATION, used to modify the function name;
  b. Compilation part: 
    HAN_COMPILER_AUTO_INTEGRATE, used to generate character codes.
  When the debug macro of the generated character code in the compiler is true,"codeTest1.txt" is the input code file, "codeTest1out.txt" is the output code file; when the debug macro is false, you need to specify the input and output files on the command line;

2 "COOL_VM", virtual machine's code, when the debug macro in the virtual machine is true, "assemblyCode.txt" is the file of the input code; when the debug macro is false, you need to specify the input code file on the command ;

3 "grammar", the grammar description of COOLang;

4 "code for test", the test code, contains some code examples for users.
