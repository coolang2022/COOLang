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

#ifndef HAN_COMPILER_HPP_
#define HAN_COMPILER_HPP_

#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<string>
#include <sstream>
#include<fstream>
#include"rule_token_number.h"
#include<map>
#include<vector>

#define F_ false
#define T_ true

#define Intg int
#define Addr int

#define QS 1
#define QE 2
#define C 3
#define M 4
#define ME 5
#define F 6
#define R 7
#define B 8
#define L 9
#define T 10
#define S 11
#define Y 12

#define Dz 0
#define Sz 1
#define Bs 2
#define Zf 3

#ifndef debug
#define debug false
#endif

#define default_organization 100
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

class Arg {
public:
    Strg arg_s;
    Intg argFlag = 0;

    Intg changeable = F_;
    Arg() {

    }

    Arg(Intg flag, const Strg &arg_s) {
        argFlag = flag;
        this->arg_s = arg_s;
    }

    Strg toStrg() const {
        stringstream ss;
        ss << "argFlag:[" << argFlag << "]\t" << "arg_s:[" << arg_s << "]\t"
                << endl;

        return ss.str();
    }

};
class AssemblyCode {
public:
    Intg type = 0;

    Intg arg1Flag = 0;

    Intg arg2Flag = 0;

    Intg operatorFlag = 0;

    Intg resultFlag = 0;

    Strg arg1_s;

    Strg arg2_s;

    Strg operator_s;

    Strg result_s;

    Arg *arg1 = NULL;
    Arg *arg2 = NULL;
    Arg *argoperator = NULL;
    Arg *argresult = NULL;

    AssemblyCode(const Arg &arg1, const Arg &arg2, const Arg &argoperator,
            const Arg &argresult) {
        arg1Flag = arg1.argFlag;
        arg2Flag = arg2.argFlag;
        operatorFlag = argoperator.argFlag;
        resultFlag = argresult.argFlag;
        arg1_s = arg1.arg_s;
        arg2_s = arg2.arg_s;
        operator_s = argoperator.arg_s;
        result_s = argresult.arg_s;

    }
    AssemblyCode() {
    }
    void setArg(const Intg argPosition, const Arg arg) {
        switch (argPosition) {
        case 0:
            arg1Flag = arg.argFlag;
            arg1_s = arg.arg_s;
            break;
        case 1:
            arg2Flag = arg.argFlag;
            arg2_s = arg.arg_s;
            break;
        case 2:
            operatorFlag = arg.argFlag;
            operator_s = arg.arg_s;
            break;
        case 3:
            resultFlag = arg.argFlag;
            result_s = arg.arg_s;
            break;
        default:
            break;
        }
    }
    Strg toStrg() const {
        stringstream ss;

        ss << type << "," << arg1_s << "," << arg2_s << "," << operator_s << ","
                << result_s << "," << arg1Flag << "," << arg2Flag << ","
                << operatorFlag << "," << resultFlag << endl;

        return ss.str();
    }

    const bool operator==(const AssemblyCode &asc) {
        return type == asc.type && arg1Flag == asc.arg1Flag &&

        arg2Flag == asc.arg2Flag &&

        operatorFlag == asc.operatorFlag &&

        resultFlag == asc.resultFlag &&

        arg1_s == asc.arg1_s &&

        arg2_s == asc.arg2_s &&

        operator_s == asc.operator_s &&

        result_s == asc.result_s;

    }

};
class Compiler {
public:
    vector<Intg> heap;
    ofstream outputfstream;
    Intg lineNumber = 0;
    Intg tokenNumber = 0;
    map<Intg, Arg> token_arg_map;
    Compiler() {

    }
    Compiler(const Strg &filePath) {
        outputfstream.open(filePath.c_str(), ios::trunc);
    }

    void setOutputCodePath(const char *filePath) {
        outputfstream.close();
        outputfstream.open(filePath, ios::trunc);

        return;
    }
    void outputAssemblyCode(AssemblyCode &as) {
        cout << "Compiler::outputAssemblyCode in Strg:[" << as.toStrg() << "\t]"
                << endl;
        if (as.arg1Flag == Dz && as.arg1_s == __line__) {
            cout << "\t1" << endl;
            if (as.arg1) {
                if (as.arg1->arg_s != __line__) {
                    as.arg1_s = as.arg1->arg_s;
                } else {
                    as.arg1->arg_s = as.arg1_s = toStrg(lineNumber);
                }
            } else {
                cerr
                        << "Compiler::outputAssemblyCode err1,can't certain lineNumber,exit(-1)"
                        << endl;
            }
            cout << "\t1.1" << endl;
        }
        if (as.arg2Flag == Dz && as.arg2_s == __line__) {
            cout << "\t2" << endl;
            if (as.arg2) {
                if (as.arg2->arg_s != __line__) {
                    as.arg2_s = as.arg2->arg_s;
                } else {
                    as.arg2->arg_s = as.arg2_s = toStrg(lineNumber);
                }
            } else {
                cerr
                        << "Compiler::outputAssemblyCode err2,can't certain lineNumber,exit(-1)"
                        << endl;
            }
            cout << "\t2.1" << endl;
        }
        if (as.operatorFlag == Dz && as.operator_s == __line__) {
            cout << "\t3" << endl;
            if (as.argoperator) {
                if (as.argoperator->arg_s != __line__) {
                    as.operator_s = as.argoperator->arg_s;
                } else {
                    as.argoperator->arg_s = as.operator_s = toStrg(lineNumber);
                }
            } else {
                cerr
                        << "Compiler::outputAssemblyCode err operator,can't certain lineNumber,exit(-1)"
                        << endl;
            }
            cout << "\t3.1" << endl;
        }
        if (as.resultFlag == Dz && as.result_s == __line__) {
            cout << "\t4" << endl;
            if (as.argresult) {
                if (as.argresult->arg_s != __line__) {
                    as.result_s = as.argresult->arg_s;
                } else {
                    as.argresult->arg_s = as.result_s = toStrg(lineNumber);
                }
            } else {
                cerr
                        << "Compiler::outputAssemblyCode err result,can't certain lineNumber,exit(-1)"
                        << endl;
            }
            cout << "\t4.1" << endl;
        }
        if (as.arg1)
            cout << "\t" << as.arg1->toStrg();
        if (as.arg2)
            cout << "\t" << as.arg2->toStrg();
        if (as.argoperator)
            cout << "\t" << as.argoperator->toStrg();
        if (as.argresult)
            cout << "\t" << as.argresult->toStrg();
        Strg &&asc = as.toStrg();
        cout << "Compiler::outputAssemblyCode Strg end Strg:[" << asc << "\t]"
                << endl;
        outputfstream << asc;
    }
    /**
     * @brief 当这个函数用于将扫描到的标识符转换成arg与tokennumber组成键值对储存起来。
     * createNew用于决定是否创建新的键值对（即不覆盖之前键值对），请将其始终置为1
     * @param createNew=1
     * @param tokenType
     * @param argFlag
     * @param tokenString
     * @return
     */
    Intg matchToken(bool createNew, Intg tokenType, Intg argFlag,
            char *tokenString) {
        cout << "in Compiler::matchToken:createNew:[" << createNew
                << "\t]tokenType:[" << tokenType << "\t]"
                        "argFlag:[" << argFlag << "\t]tokenString:["
                << tokenString << "\t]" << endl;
//        tokenNumber -= createNew;
        tokenNumber--;
        Strg tokenStrg(tokenString);
        Arg tokenArg(argFlag, tokenStrg);
        switch (tokenType) {
        case Token::CALL:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::CHANGEABLE:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::PENDCHANGEABLE:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::CHENG:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::CHU:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::COMMA:
            tokenArg.arg_s = "COMMA";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::DAYU:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::DENGYU:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::TUIDAO:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::DOTDOT:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::JICHENG:
            tokenArg.arg_s = "<<";
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::FUZHI:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::HIGHEST:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::JIA:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::DOT:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::JIAN:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LBCUR:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LMCUR:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LOWEST:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::LSCUR:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::MI:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::RBCUR:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::RMCUR:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::RSCUR:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::SEMICOLON:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::SHUCHU:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::XIAOYU:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_return_:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_return_high:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::_return_low:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::arg:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::attribute:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::changeable_attribute:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::function_declare_attribute:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::variable_specific_attribute:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::IF:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::ELSE:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::ELSEIF:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::ELIF:
            token_arg_map[tokenNumber] = tokenArg;
            break;
        case Token::WHILE:
            token_arg_map[tokenNumber] = tokenArg;
            break;

        default:
            break;
        }
//        if (createNew) {
//            tokenNumber--;
//        }

        tokenNumber--;
        cout << "in Compiler::matchToken: out tokenNumber:[" << tokenNumber
                << "\t]tokenType:[" << tokenType << "\t]"
                        "argFlag:[" << argFlag << "\t]tokenString:["
                << tokenString << "\t]" << endl;
        return tokenNumber + 1;

    }
    /**
     * @brief 此函数用于创建字符码，其中createNew用于表明是否要创建新的字符码
     * @param createNew
     * @param ruleType
     * @param tokenNumber1
     * @param tokenNumber2
     * @param tokenNumber3
     * @param tokenNumber4
     */
    void matchRule(bool createNew, Intg ruleType, Intg *tokenNumber1,
    Intg *tokenNumber2, Intg *tokenNumber3, Intg *tokenNumber4) {
        cout << "in Compiler::matchRule:createNew:[" << createNew
                << "\t]ruleType:[" << ruleType << "\t]"
                        "tokenNumber1:["
                << toStrg(tokenNumber1 ? (*tokenNumber1) : -65536) << "\t]"
                << "tokenNumber2:["
                << toStrg(tokenNumber2 ? (*tokenNumber2) : -65536) << "\t]"
                << "tokenNumber3:["
                << toStrg(tokenNumber3 ? (*tokenNumber3) : -65536) << "\t]"
                << "tokenNumber4:["
                << toStrg(tokenNumber4 ? (*tokenNumber4) : -65536) << "\t]";
        AssemblyCode as;
        Arg *&arg1 = as.arg1;
        Arg *&arg2 = as.arg2;
        Arg *&argoperator = as.argoperator;
        Arg *&argresult = as.argresult;
        cout << "in Compiler::matchRule 1" << endl;
        if (tokenNumber1
                && (token_arg_map.find(*tokenNumber1) != token_arg_map.end())) {
            cout << "in Compiler::matchRule arg1!=NULL" << endl;
            arg1 = &token_arg_map[*tokenNumber1];
        }
        if (tokenNumber2
                && (token_arg_map.find(*tokenNumber2) != token_arg_map.end())) {
            cout << "in Compiler::matchRule arg2!=NULL" << endl;
            arg2 = &token_arg_map[*tokenNumber2];
        }
        if (tokenNumber3
                && (token_arg_map.find(*tokenNumber3) != token_arg_map.end())) {
            cout << "in Compiler::matchRule argoperator!=NULL" << endl;
            argoperator = &token_arg_map[*tokenNumber3];
        }
        if (tokenNumber4
                && (token_arg_map.find(*tokenNumber4) != token_arg_map.end())) {
            cout << "in Compiler::matchRule argresult!=NULL" << endl;
            argresult = &token_arg_map[*tokenNumber4];
        }
        cout << "in Compiler::matchRule 2" << endl;

        cout << "in Compiler::matchRule 3" << endl;
        switch (ruleType) {
        case Rule::_arg____CHANGEABLE_arg:
            cout << "in Compiler::matchRule switch:["
                    << "_arg____CHANGEABLE_arg" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::_arg____PENDCHANGEABLE_arg:
            cout << "in Compiler::matchRule switch:["
                    << "_arg____PENDCHANGEABLE_arg" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::_arg____arg:
            cout << "in Compiler::matchRule switch:[" << "_arg____arg" << "]"
                    << endl;
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::_arg____variable_specific_attribute_DOTDOT__arg_:
            cout << "in Compiler::matchRule switch:["
                    << "_arg____variable_specific_attribute_DOTDOT__arg_" << "]"
                    << endl;

            (*tokenNumber4) = (*tokenNumber1);
            if (arg2) {

                if (arg2->arg_s == "new") {
                    as.type = C;
                } else if (arg2->arg_s == "out") {
                    as.type = M;
                }
            } else {
                cout << "\terr:variable_specific_attribute null,exit(-1)"
                        << endl;
                //exit(-1);
            }

            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::code___code_environment:
            cout << "in Compiler::matchRule switch:["
                    << "code___code_environment" << "]" << endl;
            flushMap();

            break;
        case Rule::code___environment:
            cout << "in Compiler::matchRule switch:[" << "code___environment"
                    << "]" << endl;
            flushMap();

            break;
        case Rule::environment___functionImplement:
            cout << "in Compiler::matchRule switch:["
                    << "environment___functionImplement" << "]" << endl;
            flushMap();

            break;
        case Rule::environment___scope:
            cout << "in Compiler::matchRule switch:[" << "environment___scope"
                    << "]" << endl;
            flushMap();

            break;
        case Rule::environment___sentence:
            cout << "in Compiler::matchRule switch:["
                    << "environment___sentence" << "]" << endl;
            flushMap();

            break;
        case Rule::environment_null___NULL:
            cout << "in Compiler::matchRule switch:["
                    << "environment_null___NULL" << "]" << endl;
            flushMap();

            break;

        case Rule::expression___LSCUR_expression_RSCUR:
            cout << "in Compiler::matchRule switch:["
                    << "expression___LSCUR_expression_RSCUR" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::expression____arg_:
            cout << "in Compiler::matchRule switch:[" << "expression____arg_"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::sentence____return__SEMICOLON:
            cout << "in Compiler::matchRule switch:["
                    << "sentence____return__SEMICOLON" << "]" << endl;

            as.type = R;
            break;
        case Rule::sentence____return__expression_SEMICOLON:
            cout << "in Compiler::matchRule switch:["
                    << "sentence____return__expression_SEMICOLON" << "]"
                    << endl;

            as.type = R;
            as.setArg(0, *arg1);

            break;
        case Rule::expression___expression_CHENG_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_CHENG_expression" << "]"
                    << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_CHU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_CHU_expression" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_COMMA_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_COMMA_expression" << "]"
                    << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_DENGYU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_DENGYU_expression" << "]"
                    << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_DAYU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_DAYU_expression" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_XIAOYU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_XIAOYU_expression" << "]"
                    << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_DOTDOT_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_DOTDOT_expression" << "]"
                    << endl;
            cout
                    << "HAN_COMPILER::Compiler::matchRule::expression___expression_DOTDOT_expression"
                    << endl;

            //默认这一步为创建system的实例。将来可能有改变
            (*tokenNumber4) = (*tokenNumber1);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = C;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_FUZHI_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_FUZHI_expression" << "]"
                    << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression___expression_JIAN_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_JIAN_expression" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___JIAN_expression:

            if (isNumber(arg2->arg_s) == true) {
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIAN_expression" << "],createNew == F"
                        << endl;
                createNew = false;
                (*tokenNumber4) = *tokenNumber2;
                token_arg_map[(*tokenNumber4)].arg_s =
                        token_arg_map[(*tokenNumber3)].arg_s
                                + token_arg_map[(*tokenNumber4)].arg_s;
                argresult = &token_arg_map[(*tokenNumber4)];

            } else {
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIAN_expression" << "],createNew == T"
                        << endl;
                createNew = true;
                (*tokenNumber4) = --tokenNumber;
                token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
                argresult = &token_arg_map[(*tokenNumber4)];
                as.type = M;
                as.setArg(0, Arg(Sz, "0"));
                as.setArg(1, *arg2);
                as.setArg(2, *argoperator);
                as.setArg(3, *argresult);
            }
            break;
        case Rule::expression___JIA_expression:

            if (isNumber(arg2->arg_s) == true) {
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIA_expression" << "],createNew == F"
                        << endl;
                createNew = false;
                (*tokenNumber4) = *tokenNumber2;
                token_arg_map[(*tokenNumber4)].arg_s =
                        token_arg_map[(*tokenNumber3)].arg_s
                                + token_arg_map[(*tokenNumber4)].arg_s;
                argresult = &token_arg_map[(*tokenNumber4)];

            } else {
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIA_expression" << "],createNew == T"
                        << endl;
                createNew = true;
                (*tokenNumber4) = --tokenNumber;
                token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
                argresult = &token_arg_map[(*tokenNumber4)];
                as.type = M;
                as.setArg(0, Arg(Sz, "0"));
                as.setArg(1, *arg2);
                as.setArg(2, *argoperator);
                as.setArg(3, *argresult);
            }
            break;
        case Rule::expression___expression_JIA_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_JIA_expression" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_DOT_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_DOT_expression" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = Y;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_LSCUR_expression_RSCUR:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_LSCUR_expression_RSCUR" << "]"
                    << endl;
#if debug
            {
                static int ignorecount = 0;
                cout << "matchRule(bool, int, int*, int*, int*, int*)" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "han_compiler.hpp:" << __LINE__ << ":0" << ")"
                        << endl;
            }
#endif
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "call"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_LSCUR_expression_null_RSCUR:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_LSCUR_expression_null_RSCUR"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);

            as.setArg(2, Arg(Bs, "call"));
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_MI_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_MI_expression" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expression___expression_SHUCHU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expression___expression_SHUCHU_expression" << "]"
                    << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expression_null___NULL:
            cout << "in Compiler::matchRule switch:["
                    << "expression_null___NULL" << "]" << endl;

            break;
//        case Rule::functionDeclare___function_declare_attribute_DOTDOT_functionDeclare:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            (*tokenNumber4) = (*tokenNumber1);
//            as.type = F;
//            if (arg2->argFlag == Sz) {
//                argoperator->arg_s = "@";
//            }
//            as.setArg(0, *arg1);
//            as.setArg(1, *arg2);
//            as.setArg(2, *argoperator);
//            as.setArg(3, *arg1);
//            break;
//        case Rule::functionDeclare___function_declare_attribute_DOTDOT_scopeFunctionDeclare:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            (*tokenNumber4) = (*tokenNumber1);
//            as.type = F;
//            if (arg2->argFlag == Sz) {
//                argoperator->arg_s = "@";
//            }
//            as.setArg(0, *arg1);
//            as.setArg(1, *arg2);
//            as.setArg(2, *argoperator);
//            as.setArg(3, *arg1);
//            break;
//        case Rule::functionDeclare___function_declare_attribute_functionDeclare:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            cout
//                    << "HAN_COMPILER::Compiler::matchRule::functionDeclare___function_declare_attribute_functionDeclare"
//                    << endl;
//            (*tokenNumber4) = (*tokenNumber1);
//            as.type = F;
//            as.setArg(0, *arg1);
//            as.setArg(1, Arg(Sz, 0));
//            as.setArg(2, Arg(Bs, "@"));
//            as.setArg(3, *arg1);
//            break;
//        case Rule::functionDeclare___function_declare_attribute_scopeFunctionDeclare:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            cout
//                    << "HAN_COMPILER::Compiler::matchRule::functionDeclare___function_declare_attribute_functionDeclare"
//                    << endl;
//            (*tokenNumber4) = (*tokenNumber1);
//            as.type = F;
//            as.setArg(0, *arg1);
//            as.setArg(1, Arg(Sz, default_organization));
//            as.setArg(2, Arg(Bs, "@"));
//            as.setArg(3, *arg1);
//            break;
//        case Rule::functionImplement___functionDeclare_scope:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            (*tokenNumber4) = (*tokenNumber1);
//            as.type = F;
//            as.setArg(0, *arg1);
//            as.setArg(1, Arg(Sz, 0));
//            as.setArg(2, Arg(Bs, "@"));
//            as.setArg(3, *arg1);
//            break;
//        case Rule::scopeFunctionDeclareBegin___LBCUR:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            (*tokenNumber4) = (*tokenNumber1);
//            token_arg_map[(*tokenNumber1)] = Arg(Dz, toStrg(lineNumber));
//            as.type = QS;
//            as.setArg(0, token_arg_map[(*tokenNumber1)]);
//
//            break;
//        case Rule::scopeFunctionDeclareBegin___NULL:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            (*tokenNumber4) = lineNumber;
//            token_arg_map[(*tokenNumber4)] = Arg(Dz, toStrg(lineNumber));
//            as.type = QS;
//            as.setArg(0, token_arg_map[(*tokenNumber4)]);
//
//            break;
//        case Rule::scopeFunctionDeclareEnd___NULL:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//
//            break;
//        case Rule::scopeFunctionDeclareEnd___RBCUR:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//
//            break;
//        case Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentenceFunctionDeclare:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            (*tokenNumber4) = (*tokenNumber1);
//
//            break;
//        case Rule::scopeFunctionDeclare___scopeFunctionDeclareHalf_scopeFunctionDeclareEnd:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            (*tokenNumber4) = (*tokenNumber1);
//            as.type = QE;
//            as.setArg(0, token_arg_map[(*tokenNumber4)]);
//            break;
        case Rule::scopeBegin___LBCUR:
            cout << "in Compiler::matchRule switch:[" << "scopeBegin___LBCUR"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            token_arg_map[(*tokenNumber1)] = Arg(Dz, __line__);
            arg1 = argresult = &token_arg_map[(*tokenNumber1)];
            as.type = QS;
            as.setArg(3, *argresult);
            break;
        case Rule::scopeHalf___scopeBegin_sentence:
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeBegin_sentence" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_functionDeclare:
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_functionDeclare" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_functionImplement:
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_functionImplement" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_scope:
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_scope" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_sentence:
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_sentence" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scope___scopeHalf_RBCUR:
            cout << "in Compiler::matchRule switch:["
                    << "scope___scopeHalf_RBCUR" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = argresult = &token_arg_map[(*tokenNumber4)];
            as.type = QE;
            as.setArg(3, *argresult);
            break;
        case Rule::scope___scopeBegin_RBCUR:
            cout << "in Compiler::matchRule switch:["
                    << "scope___scopeBegin_RBCUR" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = argresult = &token_arg_map[(*tokenNumber4)];
            as.type = QE;
            as.setArg(3, *argresult);
            break;

            /*函数声明部分*/
        case Rule::expressionFunctionDeclareRoot___JIAN_expression:

            if (isNumber(arg2->arg_s) == true) {
                cout << "in Compiler::matchRule switch:["
                        << "expressionFunctionDeclareRoot___JIAN_expression"
                        << "],createNew == F" << endl;
                createNew = false;
                (*tokenNumber4) = *tokenNumber2;
                token_arg_map[(*tokenNumber4)].arg_s =
                        token_arg_map[(*tokenNumber3)].arg_s
                                + token_arg_map[(*tokenNumber4)].arg_s;
                argresult = &token_arg_map[(*tokenNumber4)];

            } else {
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIAN_expression" << "],createNew == T"
                        << endl;
                createNew = true;
                (*tokenNumber4) = --tokenNumber;
                token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
                argresult = &token_arg_map[(*tokenNumber4)];
                as.type = M;
                as.setArg(0, Arg(Sz, "0"));
                as.setArg(1, *arg2);
                as.setArg(2, *argoperator);
                as.setArg(3, *argresult);
            }
            break;
        case Rule::expressionFunctionDeclareRoot___JIA_expression:

            if (isNumber(arg2->arg_s) == true) {
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIA_expression" << "],createNew == F"
                        << endl;
                createNew = false;
                (*tokenNumber4) = *tokenNumber2;
                token_arg_map[(*tokenNumber4)].arg_s =
                        token_arg_map[(*tokenNumber3)].arg_s
                                + token_arg_map[(*tokenNumber4)].arg_s;
                argresult = &token_arg_map[(*tokenNumber4)];

            } else {
                cout << "in Compiler::matchRule switch:["
                        << "expression___JIA_expression" << "],createNew == T"
                        << endl;
                createNew = true;
                (*tokenNumber4) = --tokenNumber;
                token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
                argresult = &token_arg_map[(*tokenNumber4)];
                as.type = M;
                as.setArg(0, Arg(Sz, "0"));
                as.setArg(1, *arg2);
                as.setArg(2, *argoperator);
                as.setArg(3, *argresult);
            }
            break;
        case Rule::expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___LSCUR_expressionFunctionDeclareRoot_RSCUR"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_CHENG_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_CHENG_expression"
                    << "]" << endl;

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_CHU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_CHU_expression"
                    << "]" << endl;

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_COMMA_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_COMMA_expression"
                    << "]" << endl;

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_DENGYU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_DENGYU_expression"
                    << "]" << endl;

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_DAYU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_DAYU_expression"
                    << "]" << endl;

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_XIAOYU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_XIAOYU_expression"
                    << "]" << endl;

            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_DOTDOT_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_DOTDOT_expression"
                    << "]" << endl;

            cout
                    << "HAN_COMPILER::Compiler::matchRule::expressionFunctionDeclareRoot___expression_DOTDOT_expression"
                    << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_FUZHI_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_FUZHI_expression"
                    << "]" << endl;
            cout
                    << "HAN_COMPILER::Compiler::matchRule::expressionFunctionDeclareRoot___expression_FUZHI_expression"
                    << endl;

            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_JIAN_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_JIAN_expression"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_JIA_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_JIA_expression"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_LSCUR_expression_RSCUR"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "call"));
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_LSCUR_expression_null_RSCUR"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);

            as.setArg(2, Arg(Bs, "call"));
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_MI_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_MI_expression"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::expressionFunctionDeclareRoot___expression_SHUCHU_expression:
            cout << "in Compiler::matchRule switch:["
                    << "expressionFunctionDeclareRoot___expression_SHUCHU_expression"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Bs, "ans");
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = M;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;

        case Rule::scopeFunctionDeclareBegin___LBCUR:
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareBegin___LBCUR" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            token_arg_map[(*tokenNumber1)] = Arg(Dz, __line__);
            arg1 = argresult = &token_arg_map[(*tokenNumber1)];
            as.type = QS;
            as.setArg(3, *argresult);
            break;
        case Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence:
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareHalf___scopeFunctionDeclareBegin_sentence"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence:
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareHalf___scopeFunctionDeclareHalf_sentence"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot:
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON:
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareEnd___scopeFunctionDeclareBegin_expressionFunctionDeclareRoot_SRMICOLON"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot:
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON:
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclareEnd___scopeFunctionDeclareHalf_expressionFunctionDeclareRoot_SRMICOLON"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR:
            cout << "in Compiler::matchRule switch:["
                    << "scopeFunctionDeclare___scopeFunctionDeclareEnd_RBCUR"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = argresult = &token_arg_map[(*tokenNumber1)];
            as.type = QE;
            as.setArg(3, *argresult);
            break;

        case Rule::autoScopeBegin___NULL:
            cout << "in Compiler::matchRule switch:[" << "autoScopeBegin___NULL"
                    << "]" << endl;
            cout << "in Compiler::matchRule::autoScopeBegin___NULL 1" << endl;
            (*tokenNumber4) = --tokenNumber;
            cout << "in Compiler::matchRule::autoScopeBegin___NULL 2" << endl;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            cout << "in Compiler::matchRule::autoScopeBegin___NULL 3" << endl;
            as.type = QS;
            as.setArg(3, token_arg_map[(*tokenNumber4)]);
            cout << "in Compiler::matchRule::autoScopeBegin___NULL 4" << endl;
            break;
        case Rule::autoScopeEnd___NULL:
            cout << "in Compiler::matchRule switch:[" << "autoScopeEnd___NULL"
                    << "]" << endl;
            //do nothing
            break;

        case Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot:
            cout << "in Compiler::matchRule switch:["
                    << "autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot_SEMICOLON:
            cout << "in Compiler::matchRule switch:["
                    << "autoScopeHalf___autoScopeBegin_expressionFunctionDeclareRoot_SEMICOLON"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;

        case Rule::autoScope___autoScopeHalf_autoScopeEnd:
            cout << "in Compiler::matchRule switch:["
                    << "autoScope___autoScopeHalf_autoScopeEnd" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            arg1 = argresult = &token_arg_map[(*tokenNumber1)];
            as.type = QE;
            as.setArg(3, *argresult);
            break;

        case Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_DOTDOT_autoScope"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = QE;
            as.setArg(0, token_arg_map[(*tokenNumber4)]);
            break;
        case Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_DOTDOT_scopeFunctionDeclare"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            if (arg2->argFlag == Sz) {
                argoperator->arg_s = "@";
            }
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_scopeFunctionDeclare"
                    << "]" << endl;
            //todo: 需要修改
            (*tokenNumber4) = (*tokenNumber1);
            if (arg2 && arg2->argFlag == Bs) {
                as.type = F;
                as.setArg(0, *arg1);
                as.setArg(1, Arg(Sz, toStrg(default_organization)));
                as.setArg(2, Arg(Bs, "@"));
                as.setArg(3, *arg1);
            } else if (arg2 && arg2->argFlag == Sz) {
                as.type = F;
                as.setArg(0, *arg1);
                as.setArg(1, *arg2);
                as.setArg(2, Arg(Bs, "@"));
                as.setArg(3, *arg1);
            }

            break;
        case Rule::functionDeclareBegin___function_declare_attribute_autoscope:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_autoscope"
                    << "]" << endl;
            cout
                    << "HAN_COMPILER::Compiler::matchRule::functionDeclare___function_declare_attribute_autoscope"
                    << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, toStrg(default_organization)));
            as.setArg(2, Arg(Bs, "@"));
            as.setArg(3, *arg1);
            break;
        case Rule::functionDeclareBegin___function_declare_attribute_functionDeclareBegin:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_functionDeclareBegin"
                    << "]" << endl;

            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, Arg(Bs, "@"));
            as.setArg(3, *arg1);
            break;
        case Rule::functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareBegin___function_declare_attribute_DOTDOT_functionDeclareBegin"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            if (arg2->argFlag == Sz) {
                argoperator->arg_s = "@";
            }
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;

        case Rule::functionDeclareHalf___functionDeclareBegin_JICHENG_arg:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclareHalf___functionDeclareBegin_JICHENG_arg"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            argoperator->arg_s = "<<";
            as.type = S;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::functionDeclare___functionDeclareBegin:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclare___functionDeclareBegin" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::functionDeclare___functionDeclareHalf:
            cout << "in Compiler::matchRule switch:["
                    << "functionDeclare___functionDeclareHalf" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::functionImplement___functionDeclare_scope:
            cout << "in Compiler::matchRule switch:["
                    << "functionImplement___functionDeclare_scope" << "]"
                    << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = F;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(3, *arg1);
            break;
        case Rule::functionImplement___functionImplement_TUIDAO_functionDeclare:
            cout << "in Compiler::matchRule switch:["
                    << "functionImplement___functionImplement_TUIDAO_functionDeclare"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = T;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;

//        case Rule::sentenceFunctionDeclare___expressionFunctionDeclareRoot:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            as.type = ME;
//            break;
//        case Rule::sentenceFunctionDeclare___expressionFunctionDeclareRoot_SEMICOLON:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            as.type = ME;
//            break;
//        case Rule::sentenceFunctionDeclare_null___NULL:cout<<"in Compiler::matchRule switch:["<<""<<"]"<<endl;
//            as.type = ME;
//            break;
        case Rule::sentence___SEMICOLON:
            cout << "in Compiler::matchRule switch:[" << "sentence___SEMICOLON"
                    << "]" << endl;
            as.type = ME;
            break;
        case Rule::sentence___expression_SEMICOLON:
            cout << "in Compiler::matchRule switch:["
                    << "sentence___expression_SEMICOLON" << "]" << endl;
            as.type = ME;
            break;
        case Rule::sentence___functionDeclare_SEMICOLON:
            cout << "in Compiler::matchRule switch:["
                    << "sentence___functionDeclare_SEMICOLON" << "]" << endl;
            as.type = ME;
            break;
        case Rule::sentence___functionImplement:
            cout << "in Compiler::matchRule switch:["
                    << "sentence___functionImplement" << "]" << endl;
            as.type = ME;
            break;
        case Rule::sentence___systemImplement:
            cout << "in Compiler::matchRule switch:["
                    << "sentence___systemImplement" << "]" << endl;
            as.type = ME;
            break;
        case Rule::sentence___branch:
            cout << "in Compiler::matchRule switch:[" << "sentence___branch"
                    << "]" << endl;
            as.type = ME;
            break;
        case Rule::sentence___loop:
            cout << "in Compiler::matchRule switch:[" << "sentence___loop"
                    << "]" << endl;
            as.type = ME;
            break;
        case Rule::sentence_null___NULL:
            cout << "in Compiler::matchRule switch:[" << "sentence_null___NULL"
                    << "]" << endl;
            as.type = ME;
            break;

        case Rule::branchBegin___IF_LSCUR_expression_RSCUR_autoScope:
            cout << "in Compiler::matchRule switch:["
                    << "branchBegin___IF_LSCUR_expression_RSCUR_autoScope"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, Arg(Sz, "0"));
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchBegin___IF_LSCUR_expression_RSCUR_scope:
            cout << "in Compiler::matchRule switch:["
                    << "branchBegin___IF_LSCUR_expression_RSCUR_scope" << "]"
                    << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, Arg(Sz, "0"));
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope:
            cout << "in Compiler::matchRule switch:["
                    << "branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_autoScope"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope:
            cout << "in Compiler::matchRule switch:["
                    << "branchHalf___branchBegin_ELIF_LSCUR_expression_RSCUR_scope"
                    << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchEnd___branchBegin_ELSE__autoScope:
            cout << "in Compiler::matchRule switch:["
                    << "branchEnd___branchBegin_ELSE__autoScope" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, "1"));
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchEnd___branchBegin_ELSE__scope:
            cout << "in Compiler::matchRule switch:["
                    << "branchEnd___branchBegin_ELSE__scope" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, "1"));
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchEnd___branchHalf_ELSE__autoScope:
            cout << "in Compiler::matchRule switch:["
                    << "branchEnd___branchHalf_ELSE__autoScope" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, "1"));
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branchEnd___branchHalf_ELSE__scope:
            cout << "in Compiler::matchRule switch:["
                    << "branchEnd___branchHalf_ELSE__scope" << "]" << endl;
            (*tokenNumber4) = --tokenNumber;
            token_arg_map[(*tokenNumber4)] = Arg(Dz, __line__);
            argresult = &token_arg_map[(*tokenNumber4)];
            as.type = B;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Sz, "1"));
            as.setArg(2, *argoperator);
            as.setArg(3, *argresult);
            break;
        case Rule::branch___branchBegin:
            cout << "in Compiler::matchRule switch:[" << "branch___branchBegin"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::branch___branchHalf:
            cout << "in Compiler::matchRule switch:[" << "branch___branchHalf"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::branch___branchEnd:
            cout << "in Compiler::matchRule switch:[" << "branch___branchEnd"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);

            break;
        case Rule::loop___WHILE_LSCUR_expression_RSCUR_autoScope:
            cout << "in Compiler::matchRule switch:["
                    << "loop___WHILE_LSCUR_expression_RSCUR_autoScope" << "]"
                    << endl;

            (*tokenNumber1) = --tokenNumber;

            token_arg_map[(*tokenNumber1)] = Arg(Dz, __line__);
            arg1 = &token_arg_map[(*tokenNumber1)];
            argresult = arg1;
            as.type = L;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);

            break;
        case Rule::loop___WHILE_LSCUR_expression_RSCUR_scope:
            cout << "in Compiler::matchRule switch:["
                    << "loop___WHILE_LSCUR_expression_RSCUR_scope" << "]"
                    << endl;

            (*tokenNumber1) = --tokenNumber;

            token_arg_map[(*tokenNumber1)] = Arg(Dz, __line__);
            arg1 = &token_arg_map[(*tokenNumber1)];
            argresult = arg1;
            as.type = L;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);

            break;
            //matchRule(1, Rule::systemDeclareBegin___system_declare_attribute_DOTDOT_arg,&$3,&$1,&$2,&$$)
        case Rule::systemDeclareBegin___system_declare_attribute_DOTDOT_arg:
            cout << "in Compiler::matchRule switch:["
                    << "systemDeclareBegin___system_declare_attribute_DOTDOT_arg"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = S;
            as.setArg(0, *arg1);
            as.setArg(1, Arg(Bs, "system"));
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::systemDeclareHalf___systemDeclareBegin_JICHENG_arg:
            cout << "in Compiler::matchRule switch:["
                    << "systemDeclareHalf___systemDeclareBegin_JICHENG_arg"
                    << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            argoperator->arg_s = "<<";
            as.type = S;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(2, *argoperator);
            as.setArg(3, *arg1);
            break;
        case Rule::systemDeclare___systemDeclareBegin:
            cout << "in Compiler::matchRule switch:["
                    << "systemDeclare___systemDeclareBegin" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::systemDeclare___systemDeclareHalf:
            cout << "in Compiler::matchRule switch:["
                    << "systemDeclare___systemDeclareHalf" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::systemImplement___systemDeclare_scope:
            cout << "in Compiler::matchRule switch:["
                    << "systemImplement___systemDeclare_scope" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            as.type = S;
            as.setArg(0, *arg1);
            as.setArg(1, *arg2);
            as.setArg(3, *arg1);
            break;
        case Rule::environment___systemImplement:
            cout << "in Compiler::matchRule switch:["
                    << "environment___systemImplement" << "]" << endl;
            flushMap();
            break;
        case Rule::sentence___systemDeclare_SEMICOLON:
            cout << "in Compiler::matchRule switch:["
                    << "sentence___systemDeclare_SEMICOLON" << "]" << endl;
            as.type = ME;
            break;
        case Rule::scopeHalf___scopeHalf_systemImplement:
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_systemImplement" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        case Rule::scopeHalf___scopeHalf_systemDeclare:
            cout << "in Compiler::matchRule switch:["
                    << "scopeHalf___scopeHalf_systemDeclare" << "]" << endl;
            (*tokenNumber4) = (*tokenNumber1);
            break;
        default:
            cout << "in Compiler::matchRule switch:[" << "default" << "]"
                    << endl;
            cout << "in Compiler::matchRule no rule found" << endl;
            break;

        }

#if debug
        {
            static int ignorecount = 0;
            cout << "matchRule(bool, int, int*, int*, int*, int*)"
                    << " matchRule success" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/han_compiler.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if (createNew == true) {
            ++lineNumber;
            outputAssemblyCode(as);
        }

    }
    void matchFinish() {
        outputfstream.close();
    }
    void flushMap() {
#if debug
        {
            static int ignorecount = 0;
            cout << "flushMap()" << " " << "\tignorecount:[" << ignorecount++
                    << "\t](" << "han_compiler.hpp:" << __LINE__ << ":0" << ")"
                    << endl;
        }
#endif
//由于flushmap之前，
//lex可能已经将归属于下一个表达式的token输入并储存进入token_arg_map，
//因此保留表内新近添加的十个数据(小的（负数）、新添加的在前面，大的在后面)
        Intg size = token_arg_map.size();
        auto it = token_arg_map.end();
        if (size > 0) {
            it--;
        }
        while (size > 10) {

            token_arg_map.erase(it--);
            size--;
        }
    }

};
extern void output();
}
/*
 *interface
 */

extern void output();
extern HAN_COMPILER::Compiler compiler;

extern void setOutputCodePath(const char *filePath);

extern Intg matchToken(Intg createNew, Intg tokenType, Intg argFlag,
        char *tokenString);
extern void matchRule(const Intg createNew, const Intg ruleType,
        const Intg *tokenNumber1, const Intg *tokenNumber2,
        const Intg *tokenNumber3, const Intg *tokenNumber4);
extern void matchFinish();
extern void flushMap();

#endif /* HAN_COMPILER_H_ */
