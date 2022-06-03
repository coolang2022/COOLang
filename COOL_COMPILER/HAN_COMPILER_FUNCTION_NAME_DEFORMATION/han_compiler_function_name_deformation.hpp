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

#ifndef HAN_COMPILER_FUNCTION_NAME_DEFORMATION_HPP
#define HAN_COMPILER_FUNCTION_NAME_DEFORMATION_HPP
#define safe_delete(p) { if(p) { delete (p);     (p)=NULL; } }

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "rule_token_number.h"
#include "y.tab.h"
#include<map>
#include<vector>
#include <deque>

#define F_ false
#define T_ true

#define Intg int
#define Addr int
#define FN -1  //类型为函数名称

#define debug false

#define INSERTSTRG "_ARG_"

#define __line__ "#line"
#ifndef fileno
#define fileno(__F) ((__F)->_file)
#endif
using namespace std;
namespace HAN_COMPILER {
class Strg: public string {
public:
    Strg() :
            string("") {

    }
    Strg(const string &s) :
            string(s) {
    }
    Strg(const char *s) :
            string(s) {
    }
    Intg size() {
        return this->string::size();
    }
    Strg& operator+=(const string &s) {
        this->string::operator+=(s);
        return (*this);
    }
    Strg& operator=(const string &s) {
        this->string::operator =(s);
        return (*this);
    }
    Strg& operator=(const char *s) {
        this->string::operator =(s);
        return (*this);
    }

};

extern bool isNumber(const string &str);
extern Intg toIntg(Strg s);
extern Strg toStrg(const Intg &i);
class FunctionName;
class Block {
public:
    bool isFN = false;
    Strg s;
    FunctionName *funn = NULL;
    Strg toStrg() const;
    Block(Intg isFN, const char *s) {
        this->isFN = isFN;
        this->s = Strg(s);
    }
    Block(Intg isFN, const Strg &s) {
        this->isFN = isFN;
        this->s = Strg(s);
    }
    Block() {

    }
};

class FunctionName {
public:
    Intg lastSwitch = 0;
#define switch_name 1
#define switch_arg 2
#define switch_prefix 3
#define switch_suffix 4
#define switch_other 5
#define switch_lscur 6
#define switch_rscur 7
#define switch_comma 8

    Intg switcher = 0;
    FunctionName *parentFN = NULL;
    //appendFN:一个表达式后面的同级别表达式
    FunctionName *appendFN = NULL;
    //appendFN:一个表达式前面的同级别表达式
    FunctionName *oldBrotherFN = NULL;
    //专门存放函数名称，只允许有ascii
    deque<Block> namedq;
    //专门存放小括号
    deque<Block> lscurdq;
    //专门存放大括号
    deque<Block> rscurdq;
    //存放函数参数，函数参数用一个位于block中的funn表示，因为参数也可能是函数调用
    deque<Block> argdq;
    //前缀：：存放@#$+——*/,?:等可以位于函数/变量前面，而不属于其他函数/变量的字符（串）
    deque<Block> prefixdq;
    //后缀：：存放+——*/,?:等可以位于函数后面的操作符，一般位于函数/变量后面，而不属于其他函数/变量的字符（串）
    //如果一个操作符可以放在前面也可以放在后面（作为后缀），优先将其作为后缀
    deque<Block> suffixdq;
private:
    //禁止创建栈实例
    FunctionName() {

    }
public:
    static FunctionName* FunctionName_() {
        return new FunctionName();
    }

    Strg toStrg() const {
        stringstream ss;
        for (auto it : prefixdq) {
            ss << it.toStrg();

        }
        bool isFunction = false;
        stringstream sname;
        for (auto it = namedq.begin(); it != namedq.end(); it++) {
            if (it->toStrg() != INSERTSTRG) {
                isFunction = true;
            }
#if debug
            {

                static int ignorecount = 0;
                cout << "toStrg()" << " for(namedq)" << "\tignorecount:["
                        << ignorecount++ << "\t]("
                        << "han_compiler_function_name_deformation.hpp:"
                        << __LINE__ << ":0" << ")" << "it.toStrg:["
                        << it->toStrg() << "\t]" << endl;
            }
#endif
            sname << it->toStrg();

        }
        if (isFunction) {
            ss << sname.str();
        }
        if (lscurdq.size() > 0) {
            ss << "(";
        }

        stringstream argss;
        bool argssNotNull = false;
        for (auto it = argdq.begin(); it != argdq.end(); it++) {
            if (it->toStrg() != "") {

                if (argssNotNull && isFunction && it != argdq.begin()) {
                    argss << ",";
                }
                argssNotNull = true;
                argss << it->toStrg();
            }
        }
        ss << argss.str();
        if (rscurdq.size() > 0) {
            ss << ")";
        }
        for (auto it : suffixdq) {
            ss << it.toStrg();

        }
        if (appendFN) {
            ss << appendFN->toStrg();

        }
        return ss.str();
    }
    Block& getLastArg() {
        return argdq.back();
    }
    Block& getLastName() {
        return namedq.back();
    }
    bool isEmpty() const {
        return !(namedq.size() + lscurdq.size() + rscurdq.size() + argdq.size()
                + prefixdq.size() + suffixdq.size());
    }

};
/**
 * Compiler Function Name Deformation
 */
class Compiler_FND {
public:
    FunctionName *currentFN = NULL;
    char *currentTokenStrg = NULL;
    Intg lastSwitcher = 0;
    ofstream outputfstream;
    Strg filePath;
    deque<FunctionName*> pdelete;
    void flush();
    void setOutputCodePath(const char *filePath) {
        this->filePath = Strg(filePath);
        outputfstream.close();
        outputfstream.open(filePath, ios::trunc);
        outputfstream.close();

        return;
    }
    /*
     * 向currentFN的argdq中添加一个新的FN Block，并将其定位到currentFN
     * parent(child)
     */
    void addChildArgFNBlock() {
        if (currentFN->isEmpty()) {
            return;
        }
        currentFN->argdq.push_back(Block());
        currentFN->getLastArg().isFN = true;
        currentFN->getLastArg().funn = FunctionName::FunctionName_();
        currentFN->lastSwitch = switch_arg;
        pdelete.push_back(currentFN->getLastArg().funn);
        FunctionName *parent = currentFN;
        currentFN = currentFN->getLastArg().funn;
        currentFN->parentFN = parent;
    }
    /*
     * 向currentFN的parent的argdq中添加一个新的FN Block，并将其定位到currentFN
     * parent（brother，brother）
     */
    void addBrotherArgFNBlock() {
        FNParent();
        addChildArgFNBlock();
    }
    /*
     * 截断当前functionName
     */
    void truncFN() {
        FunctionName *childFN = currentFN;
        currentFN = currentFN->parentFN;
        if (!currentFN) {
#if debug
            {
                static int ignorecount = 0;
                cout << "truncFN()" << " currentFN==NULL!!!"
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "han_compiler_function_name_deformation.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            currentFN = FunctionName::FunctionName_();
            currentFN->argdq.push_back(Block());
            currentFN->getLastArg().isFN = true;
            currentFN->getLastArg().funn = childFN;
            pdelete.push_back(currentFN);

        }
    }
    /*
     * 将当前fn设置为追加的fn，不存在则为当前fn追加一个fn，然后将当前fn设置为追加的fn
     * 对于追加的fn，其父Fn设置为空，其oldBrother设置为连接的上一个fn
     */
    void appendFN() {

        if (!currentFN->appendFN) {
#if debug
            {
                static int ignorecount = 0;
                cout << "appendFN()" << " appendFN==NULL!!!"
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "han_compiler_function_name_deformation.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            currentFN->appendFN = FunctionName::FunctionName_();

            pdelete.push_back(currentFN->appendFN);

            currentFN->lastSwitch = switch_other;
        }
        FunctionName *oldBrotherOfAppend = currentFN;
        currentFN = currentFN->appendFN;
        currentFN->oldBrotherFN = oldBrotherOfAppend;
        currentFN->parentFN = currentFN->oldBrotherFN->parentFN;
    }
    /**
     *向Compiler中添加一个新的块，注意，当添加一个新的functionName时，s不会被使用
     * @param switcher
     * @param s
     */
    void addBlock(Intg switcher, char *s) {
#if debug
        {
            static int ignorecount = 0;
            cout << "addBlock(int, char*)" << " in" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << "\n\tswitcher:[" << switcher << "]\t"
                    << "blockStrg:[" << s << "]\t" << endl;

        }
#endif
        //0 确保currentFN不为空
        if (!currentFN) {
            currentFN = FunctionName::FunctionName_();
            pdelete.push_back(currentFN);
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "addBlock(int, char*)" << " 1" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << endl;
        }
#endif
        //1 遇到特定字符输出
        if (switcher == switch_other) {
            flush();
            lastSwitcher = switcher;
            return;
        }

#if debug
        {
            static int ignorecount = 0;
            cout << "addBlock(int, char*)" << " 2" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << endl;
        }
#endif
        //2 当当前字符是逗号或右括号、前一个字符不是左括号或逗号，
        //则向parentFN的namedq中添加一个_ARG_block
        //需要注意的是，一个FN的所有namedq成员若都为“_ARG_”则其不是一个函数，清空namedq。
        if ((switcher == switch_comma || switcher == switch_rscur)
                && (lastSwitcher != switch_lscur)) {
#if debug
            {
                static int ignorecount = 0;
                cout << "addBlock(int, char*)" << " 2.1" << "\tignorecount:["
                        << ignorecount++ << "\t]("
                        << "han_compiler_function_name_deformation.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            if (switcher == switch_comma) {
                addBrotherArgFNBlock();
                currentFN->parentFN->namedq.push_back(Block(false, INSERTSTRG));

            } else if (switcher == switch_rscur) {
                currentFN->parentFN->namedq.push_back(Block(false, INSERTSTRG));
            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "addBlock(int, char*)" << " 3" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << endl;
        }
#endif
        //3 不允许funcName中在namedq和argdq均为空的情况下直接插入后缀
        //若是，将其改为前缀(例如±)

        if (switcher == switch_suffix
                && (currentFN->namedq.size() == 0
                        && currentFN->argdq.size() == 0)) {

            switcher = switch_prefix;

        }
        //同时，当 前一个类型为后缀（switch_suffix）时，
        //只能插入后缀suffix/清空缓冲flush（other）/新建参数（arg）/右括号（rscur）指令无操作通过；
        //其他操作将在此处追加并列arg（appendFN）后通过
        //或者，当前类型为前缀时，若当前funn除去prefix外其他队列不为空，同样追加arg
        if (lastSwitcher == switch_suffix && switcher != switch_suffix
                && switcher != switch_other && switcher != switch_arg
                && switcher != switch_rscur) {
            appendFN();
        } else if (switcher == switch_prefix
                && (currentFN->namedq.size() > 0
                        || currentFN->lscurdq.size() > 0
                        || currentFN->argdq.size() > 0
                        || currentFN->rscurdq.size() > 0
                        || currentFN->suffixdq.size() > 0)) {
            appendFN();

        }
        //新建分支arg指令
        if (switcher == switch_arg) {
            addChildArgFNBlock();
        }
        //右括号指令在添加前需要将其定位到parentFN
        if (switcher == switch_rscur) {
            FNParent();
        }

#if debug
        {
            static int ignorecount = 0;
            cout << "addBlock(int, char*)" << " 5" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << endl;
        }
#endif

        //在这里将block添加到合适的位置，以下选择只能选一个，
        //此步骤中不能在添加block之前添加FN和修改currentFN指针，但添加block之后可以
        //注意，如果可以在前文添加的逻辑一定不要在此处添加！
        if (switcher == switch_name) {
            currentFN->namedq.push_back(Block(false, s));
            currentFN->lastSwitch = switch_name;
        } else if (switcher == switch_prefix) {
            currentFN->prefixdq.push_back(Block(false, s));
        } else if (switcher == switch_arg) {
            currentFN->lastSwitch = switch_arg;
        } else if (switcher == switch_suffix) {
            currentFN->suffixdq.push_back(Block(false, s));
            currentFN->lastSwitch = switch_suffix;
        } else if (switcher == switch_lscur) {
            currentFN->lscurdq.push_back(Block(false, s));
            addChildArgFNBlock();

        } else if (switcher == switch_rscur) {
            currentFN->rscurdq.push_back(Block(false, s));
            currentFN->lastSwitch = switch_rscur;
        }

        //赋值结束后可以修改FN指针
        lastSwitcher = switcher;
#if debug
        {
            FunctionName *rootfunn = currentFN;
            while (rootfunn->parentFN || rootfunn->oldBrotherFN) {
                rootfunn =
                        rootfunn->parentFN ?
                                rootfunn->parentFN : rootfunn->oldBrotherFN;
            }
            static int ignorecount = 0;
            Strg &&rootStrg = rootfunn->toStrg();
            cout << "addBlock(int, char*)" << " out" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << "\n\t----deformation------"
                            "\n\t\t" << rootStrg
                    << "\n\t-------------------------"

                    << endl;
        }
#endif
    }
//返回父函数声明
    void FNParent() {
        currentFN = currentFN->parentFN;
    }

    Compiler_FND() {
        currentFN = FunctionName::FunctionName_();
        pdelete.push_back(currentFN);
    }
    void addToken(Intg type, char *text) {

        currentTokenStrg = text;
        switch (type) {
        case Token::CALL:
#if debug
        {
            static int ignorecount = 0;
            cout << "addToken(int, char*)" << " CALL" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "han_compiler_function_name_deformation.hpp:" << __LINE__
                    << ":0" << ")" << endl;
        }
#endif
            exit(-1);
            break;
        case Token::CHANGEABLE:
            addBlock(switch_prefix, text);
            break;
        case Token::PENDCHANGEABLE:
            addBlock(switch_prefix, text);
            break;
        case Token::CHENG:
            addBlock(switch_suffix, text);

            break;
        case Token::CHU:
            addBlock(switch_suffix, text);
            break;
        case Token::COMMA:
            addBlock(switch_comma, text);
            break;
        case Token::DAYU:
            addBlock(switch_suffix, text);
            break;
        case Token::DENGYU:
            addBlock(switch_suffix, text);
            break;
        case Token::DOTDOT:
            addBlock(switch_prefix, text);
            break;
        case Token::FUZHI:
            addBlock(switch_suffix, text);

            break;
        case Token::HIGHEST:

            break;
        case Token::JIA:
            //加减也有可能是前缀
            if (lastSwitcher == switch_other || lastSwitcher == switch_lscur) {
                addBlock(switch_prefix, text);
            } else {
                addBlock(switch_suffix, text);
            }

            break;
        case Token::JIAN:
            //加减也有可能是前缀
            if (lastSwitcher == switch_other || lastSwitcher == switch_lscur) {
                addBlock(switch_prefix, text);
            } else {
                addBlock(switch_suffix, text);
            }

            break;
        case Token::LBCUR:
            addBlock(switch_other, text);
            break;
        case Token::LMCUR:
            addBlock(switch_other, text);

            break;
        case Token::LOWEST:

            break;
        case Token::LSCUR:

            addBlock(switch_lscur, text);

            break;
        case Token::MI:
            addBlock(switch_suffix, text);
            break;
        case Token::RBCUR:
            addBlock(switch_other, text);
            break;
        case Token::RMCUR:
            addBlock(switch_other, text);
            break;
        case Token::RSCUR:
            addBlock(switch_rscur, text);
            break;
        case Token::SEMICOLON:
            addBlock(switch_other, text);
            break;
        case Token::SHUCHU:
            addBlock(switch_suffix, text);
            break;
        case Token::XIAOYU:
            addBlock(switch_suffix, text);
            break;
        case Token::_return_:
            addBlock(switch_prefix, text);
            break;
        case Token::_return_high:

            break;
        case Token::_return_low:

            break;
        case Token::arg:
            //当arg前是“)”时，说明此arg属于后置的name，不是新的参数
            if (lastSwitcher == switch_rscur) {
                addBlock(switch_name, text);
            } else {
                addBlock(switch_name, text);
            }

            break;
        case Token::attribute:
            addBlock(switch_prefix, text);
            break;
        case Token::changeable_attribute:
            addBlock(switch_prefix, text);
            break;
        case Token::function_declare_attribute:
            addBlock(switch_prefix, text);
            break;
        case Token::variable_specific_attribute:
            addBlock(switch_prefix, text);
            break;
        case Token::IF:
            addBlock(switch_other, text);
            break;
        case Token::ELSE:
            addBlock(switch_other, text);
            break;
        case Token::ELSEIF:
            addBlock(switch_other, text);
            break;
        case Token::ELIF:
            addBlock(switch_other, text);
            break;
        case Token::WHILE:
            addBlock(switch_other, text);
            break;

        default:
            break;
        }

    }

};

}
/*
 *interface
 */

extern void setOutputCodePath(const char *filePath);
extern void addToken(Intg tokenType, char *tokenString);
extern void flush();

#endif /* HAN_COMPILER_H_ */
