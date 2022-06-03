/*  COOLang VM implementation
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
#ifndef VM_SCAN_HPP_
#define VM_SCAN_HPP_

#include "vm.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

/***************************************
 *                 Scan
 ***************************************/
using namespace std;
namespace VM {
class Arg {
public:
    Numb arg_i = 0;
    Addr arg_a;
    Strg arg_s;
    Intg argFlag = 0;
    Addr asc;
    DataTable *arg_ar = NULL;
    bool formalArg = true;
    Intg changeable = F_;
    Intg unknown = F_;

    Arg() :
            arg_i(0), arg_a(Addr()), arg_s(""), argFlag(0), asc(Addr()), arg_ar(
            NULL), formalArg(true), changeable(F_) {
    }
    const bool operator<(const Arg &arg) const;

    /**
     * @brief 是否S_Bs、S_Dz、Z_Dz、S_AR、Y_Dz
     * @return
     */
    const bool isVar() const {
        return (!(arg_i == 0 && arg_a == addrnull && arg_s == ""
                && arg_ar == NULL))
                && (argFlag == S_Bs || argFlag == S_Dz || argFlag == Z_Dz
                        || argFlag == S_AR || argFlag == Y_Dz);

    }
    /**
     * @brief 是否S_Bs
     * @return
     */
    const bool isBs() const {
        return (!(arg_i == 0 && arg_a == addrnull && arg_s == ""
                && arg_ar == NULL)) && argFlag == S_Bs;
    }
    /**
     * @brief 是否S_Dz、Z_Dz、Y_Dz
     * @return
     */
    const bool isDz() const {
        return (!(arg_i == 0 && arg_a == addrnull && arg_s == ""
                && arg_ar == NULL))
                && (argFlag == S_Dz || argFlag == Z_Dz || argFlag == Y_Dz);

    }

    /**
     * "=="仅比较内容是否相同，不比较除argFlag外的标志位
     * @param arg
     * @return
     */
    bool operator==(const Arg &arg) const;

    bool operator!=(const Arg &arg) const;

    Arg& operator=(const Arg &arg);

    Arg& clone(const Arg &arg);

    Arg(Intg flag, const Addr &arg_a);

    Arg(Intg flag, const Numb &arg_i);

    Arg(Intg flag, const Strg &arg_s);

    Arg(Intg flag, DataTable *const ar);

    Arg(Intg flag, const Addr &addr, DataTable *const ar);

    Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s);

    Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s,
            const bool &formalArg);

    Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s,
            const bool &formalArg, const Addr &asc);

    Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s,
            const bool &formalArg, const Intg &changeable, const Addr &asc);

    Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s,
            DataTable *arg_ar, const bool &formalArg, const Intg &changeable,
            const Intg &unknown, const Addr &asc);

    Strg toStrg() const;

};
static Arg argnull = Arg();
class AssemblyCode;
class AssemblyCodeTable;
class AssemblyCode {
public:
    Intg type = 0;

    Intg arg1Flag = 0;

    Intg arg2Flag = 0;

    Intg operatorFlag = 0;

    Intg resultFlag = 0;

    Numb arg1_i = 0;

    Strg arg1_s;

    Numb arg2_i = 0;

    Strg arg2_s;

    Numb operator_i = 0;

    Strg operator_s;

    Numb result_i = 0;

    Strg result_s;

    AssemblyCode(Strg assemblyCode);

    static vector<Strg> splitArgs(Strg str, Strg pattern) {
        vector<Strg> result;
        str += pattern;
        for (int i = 0; i < (int) str.size(); i++) {
            int pos = (int) str.find(pattern, i);
            if (pos != (int) string::npos) {
                result.push_back(str.substr(i, pos - i));
                i = pos + (int) pattern.size() - 1;
            }
        }
        return result;
    }
    AssemblyCode() {
    }
    Strg toStrg() const;
    AssemblyCode& null() {

        *this = AssemblyCode();
        return *this;
    }
    const bool operator==(const AssemblyCode &asc) {
        return type == asc.type && arg1Flag == asc.arg1Flag &&

        arg2Flag == asc.arg2Flag &&

        operatorFlag == asc.operatorFlag &&

        resultFlag == asc.resultFlag &&

        arg1_i == asc.arg1_i && arg1_s == asc.arg1_s &&

        arg2_i == asc.arg2_i &&

        arg2_s == asc.arg2_s &&

        operator_i == asc.operator_i &&

        operator_s == asc.operator_s &&

        result_i == asc.result_i &&

        result_s == asc.result_s;

    }

};
static AssemblyCode ascnull = AssemblyCode();
AssemblyCode::AssemblyCode(Strg assemblyCode) {
    vector<Strg> args = splitArgs(assemblyCode, ",");
    if (args.size() != 9) {
        *this = AssemblyCode();
        return;
    }
    type = toIntg(args.at(0));
    arg1Flag = toIntg(args.at(5));
    arg2Flag = toIntg(args.at(6));
    operatorFlag = toIntg(args.at(7));
    resultFlag = toIntg(args.at(8));
    if (arg1Flag == Dz || arg1Flag == Sz) {
        arg1_i = toNumb(args.at(1));
    } else if (arg1Flag == Bs || arg1Flag == Zf) {
        arg1_s = args.at(1);
    } else {
#if debug
        cerr << "AssemblyCode arg1Flag err" << endl;
#endif
    }

    if (arg2Flag == Dz || arg2Flag == Sz) {
        arg2_i = toNumb(args.at(2));
    } else if (arg2Flag == Bs || arg2Flag == Zf) {
        arg2_s = args.at(2);
    } else {
#if debug
        cerr << "AssemblyCode arg2Flag err" << endl;
#endif
    }

    if (operatorFlag == Dz || operatorFlag == Sz) {
        operator_i = toNumb(args.at(3));
    } else if (operatorFlag == Bs || operatorFlag == Zf) {
        operator_s = args.at(3);
    } else {
#if debug
        cerr << "AssemblyCode operatorFlag err" << endl;
#endif
    }

    if (resultFlag == Dz || resultFlag == Sz) {
        result_i = toNumb(args.at(4));
    } else if (resultFlag == Bs || resultFlag == Zf) {
        result_s = args.at(4);
    } else {
#if debug
        cerr << "AssemblyCode resultFlag err" << endl;
#endif
    }
    {
#if debug
        static int ignorecount = 0;
        cout
                << "AssemblyCode(Strg) enclosing_method(enclosing_method_arguments)"
                << "\nas:[" << this->toStrg() << "\t]" << "\nignorecount:["
                << ignorecount++ << "\t](" << "vm_scan.hpp:" << __LINE__ << ":0"
                << ")" << endl;
#endif
    }

}

Strg AssemblyCode::toStrg() const {
    stringstream ss;

    ss << type << "," << arg1_i << " " << arg1_s << "," << arg2_i << " "
            << arg2_s << "," << operator_i << " " << operator_s << ","
            << result_i << " " << result_s << "," << arg1Flag << "," << arg2Flag
            << ","\
 << operatorFlag << "," << resultFlag << endl;

    return ss.str();
}

class AssemblyCodeTable {
public:
    deque<AssemblyCode> acdq;
    deque<Intg> acpdq;
    Strg toStrg() const;
};
Strg AssemblyCodeTable::toStrg() const {
    return ListPrinter<deque<AssemblyCode> >(acdq).ss.str();

}

/**
 * @brief 获得当前地址之前的类型为type的代码的地址。
 * @attention
 * @param type Y不被认为是M
 * @param ad
 * @return
 */
Addr CodeTable::getLast(const Intg &type, const Addr &ad) const {
    Intg pos = getLastAddrPosition(addrdq, ad);
    Addr a;

    while (pos >= 0) {
        if (codedq[pos].assemblyFormula.flagBit.type
                != type||codedq[pos].assemblyFormula.flagBit.execute==Du) {
            pos--;
        } else {
            a = addrdq[pos];
            return a;
        }

    }
    return a;
}
/**
 * @brief 获得当前地址之前的类型为types的代码的地址。
 * @attention
 * @param type Y不被认为是M
 * @param ad
 * @return
 */
Addr CodeTable::getLast(const vector<Intg> &types, const Addr &ad) const {
    Intg pos = getLastAddrPosition(addrdq, ad);
    if (pos < 0) {
#if debug
        cerr << "CodeTable::getLast(types) debug::get Array end,return addrnull"
                << endl;
#endif
        return Addr();
    }
    Addr a;

    while (pos >= 0) {
        if (std::find(types.begin(), types.end(),
                codedq[pos].assemblyFormula.flagBit.type)
                == types.end()||codedq[pos].assemblyFormula.flagBit.execute==Du) {
            pos--;
        } else {
            a = addrdq[pos];
            return a;
        }

    }
    return a;
}
/**
 * @brief 获得当前地址之前的类型不为type的代码的地址。
 * @attention
 * @param type Y不被认为是M
 * @param ad
 * @return
 */
Addr CodeTable::getLastNot(const Intg &type, const Addr &ad) const {
    Intg pos = getLastAddrPosition(addrdq, ad);
    if (pos < 0) {
#if debug
        cerr << "CodeTable::getLastNot debug::get Array end,return addrnull"
                << endl;
#endif
        return Addr();
    }
    Addr a;

    while (pos >= 0) {
        if (codedq[pos].assemblyFormula.flagBit.type
                == type||codedq[pos].assemblyFormula.flagBit.execute==Du) {
            pos--;
        } else {
            a = addrdq[pos];
            return a;
        }

    }
    return a;
}
/**
 * @brief 获得当前地址之前的类型不为types的代码的地址。
 * @attention
 * @param type Y不被认为是M
 * @param ad
 * @return
 */
Addr CodeTable::getLastNot(const vector<Intg> &types, const Addr &ad) const {
    Intg pos = getLastAddrPosition(addrdq, ad);
    if (pos < 0) {
#if debug
        cerr
                << "CodeTable::getLastNot(types) debug::get Array end,return addrnull"
                << endl;
#endif
        return Addr();
    }
    Addr a;

    while (pos >= 0) {
        if (std::find(types.begin(), types.end(),
                codedq[pos].assemblyFormula.flagBit.type)
                != types.end()||codedq[pos].assemblyFormula.flagBit.execute==Du) {
            pos--;
        } else {
            a = addrdq[pos];
            return a;
        }

    }
    return a;
}
/**
 * @brief 获得当前地址之后的类型不为type的代码的地址。
 * @attention
 * @param type Y不被认为是M
 * @param ad
 * @return
 */
Addr CodeTable::getNextNot(const Intg &type, const Addr &ad) const {
    Intg pos = getNextAddrPosition(addrdq, ad);
    if (pos < 0) {
#if debug
        cerr << "CodeTable::getNextNot debug::get Array end,return addrnull"
                << endl;
#endif
        return Addr();
    }
    Addr a;

    while (pos < (Intg) addrdq.size()) {
        if (codedq[pos].assemblyFormula.flagBit.type
                == type||codedq[pos].assemblyFormula.flagBit.execute==Du) {
            pos++;
        } else {
            a = addrdq[pos];
            return a;
        }

    }
    return a;
}
/**
 * @brief 获得当前地址之后的类型不为types的代码的地址。
 * @attention
 * @param type Y不被认为是M
 * @param ad
 * @return
 */
Addr CodeTable::getNextNot(const vector<Intg> &types, const Addr &ad) const {
    Intg pos = getNextAddrPosition(addrdq, ad);
    if (pos < 0) {
#if debug
        cerr
                << "CodeTable::getNextNot(types) debug::get Array end,return addrnull"
                << endl;
#endif
        return Addr();
    }
    Addr a;

    while (pos >= 0) {
        if (std::find(types.begin(), types.end(),
                codedq[pos].assemblyFormula.flagBit.type)
                != types.end()||codedq[pos].assemblyFormula.flagBit.execute==Du) {
            pos--;
        } else {
            a = addrdq[pos];
            return a;
        }

    }
    return a;
}
/**
 * @brief 获得当前地址之后的类型为type的代码的地址。
 * @attention
 * @param type Y不被认为是M
 * @param ad
 * @return
 */
Addr CodeTable::getNext(const Intg &type, const Addr &ad) const {
    Intg pos = getNextAddrPosition(addrdq, ad);
    if (pos < 0) {
#if debug
        cerr << "CodeTable::getNext debug::get Array end,return addrnull"
                << endl;
#endif
        return Addr();
    }
    Addr a;

    while (pos < (Intg) addrdq.size()) {
        if (codedq[pos].assemblyFormula.flagBit.type
                != type&&codedq[pos].assemblyFormula.flagBit.execute!=Du) {
            pos++;
        } else {
            a = addrdq[pos];
            return a;
        }

    }
    return a;
}
/**
 * @brief 获得当前地址之后的类型为types的代码的地址。
 * @attention
 * @param type Y不被认为是M
 * @param ad
 * @return
 */
Addr CodeTable::getNext(const vector<Intg> &types, const Addr &ad) const {
    Intg pos = getNextAddrPosition(addrdq, ad);
    if (pos < 0) {
#if debug
        cerr << "CodeTable::getNext(types) debug::get Array end,return addrnull"
                << endl;
#endif
        return Addr();
    }
    Addr a;

    while (pos >= 0) {
        if (std::find(types.begin(), types.end(),
                codedq[pos].assemblyFormula.flagBit.type)
                == types.end()||codedq[pos].assemblyFormula.flagBit.execute==Du) {
            pos++;
        } else {
            a = addrdq[pos];
            return a;
        }

    }
    return a;
}

typedef Addr ScopeAddr;
class Scanner;
/**
 * @brief 此类进行（解析类表，解析作用域表，解析函数表）
 */
class Scanner {
public:
    //以下两个表用于短期存储数据的类型，其实其已经基本失效
    map<ScopeAddr, map<Strg, Intg>> name_attr_map;
    map<ScopeAddr, map<Addr, Intg>> addr_attr_map;
    //以下map用于存储addr与ar的对应关系。scan过程中每个scope对应一个ar，不自动释放ar
    map<ScopeAddr, DataTable*> addr_ar_map;
    CodeTable *codeTable;
    ScopeTable *scopeTable;
    SystemTable *systemTable;
    TemplateFunctionTable *templateFunctionTable;
    DataTable *currentAR;

    AssemblyCodeTable assemblyCodeTable;

    Intg currentAssemblyCodePos = 0;
    AssemblyCode currentAssemblyCode;
    AssemblyCode nextAssemblyCode;

    vector<Addr> scopeStack;

    const Intg pushStack(Intg addr);
    const Intg pushStack(const Addr &addr);

    Code currentCode;
    Scope currentScope;

    Addr currentScopeAddr;
    Addr currentCodeAddr;
    Addr lastM; //上一个类型为M的code的地址

    vector<Addr> pendM;
    vector<Addr> realM;

    bool loadAssemblyCode(const Strg &filePath);
    const bool updateCurrentAssemblyCode();
    const bool setArgScopeAddr();
    const bool addArgsToCurrentAR();
    Addr stackToAddr() const;
    const Addr getQSAddr(const Addr &addr);
    const Addr getQSAddr(Intg addr);
    const Addr getScopeAddr(const Addr &addr) const;
    const Addr getScopeAddr(Intg addr) const;
    bool setQuaternionAndFlagBit(const AssemblyCode &asc, AssemblyFormula *asf);
    /**
     * @brief 查询一个变量的属性
     * @param varname
     * @param scopeAddr
     * @param out 是否从上层作用域开始检索
     * @return 变量的属性，如果变量不存在，则返回-1；
     */
    Intg getAttribute(const Strg &varname, const ScopeAddr &scopeAddr,
            bool out);
    Intg getAttribute(const Addr &varaddr, const ScopeAddr &scopeAddr,
            bool out);
    /**
     * @brief 查询一个变量的信息，其储存在一个arg中
     * @param varname
     * @param scopeAddr
     * @param out 是否从上层作用域开始检索
     * @return 一个变量，其储存了变量所在scope（asc）、属性等，如果变量不存在，则返回argnull；
     */
    Arg getArg(const Strg &varname, const ScopeAddr &scopeAddr, bool out);
    Arg getArg(const Addr &varaddr, const ScopeAddr &scopeAddr, bool out);
    /**
     * @brief 根据之前的变量重置一个变量的属性，若这个变量不存在且out为假，
     * 则在scopeAddr中创建一个新的变量
     * @param varname
     * @param scopeAddr 变量所在作用域
     * @param out 是否从上层作用域开始搜索
     * @return
     */
    void resetAttribute(const Strg &varname, const ScopeAddr &scopeAddr,
            const Intg &attr, bool out);
    void resetAttribute(const Addr &varaddr, const ScopeAddr &scopeAddr,
            const Intg &attr, bool out);
    /**
     * @brief 向作用域中添加一个变量属性的索引
     * @param varname
     * @param scopeAddr
     */
    void addAttribute(const Strg &varname, const ScopeAddr &scopeAddr,
            const Intg &attr);
    void addAttribute(const Addr &varaddr, const ScopeAddr &scopeAddr,
            const Intg &attr);
    const bool scan();
    Scanner() {
        codeTable = NULL;
        scopeTable = NULL;
        templateFunctionTable = NULL;
        currentAR = NULL;
        systemTable = NULL;
    }

    Scanner(CodeTable *codeTable_, ScopeTable *scopeTable_,
            TemplateFunctionTable *templateFunctionTable_,
            DataTable *currentAR_, SystemTable *systemTable_) {
        codeTable = codeTable_;
        scopeTable = scopeTable_;
        templateFunctionTable = templateFunctionTable_;
        systemTable = systemTable_;
        currentAR = currentAR_;
        systemTable->scopeTable = scopeTable_;

    }

    const Addr nextSameScopeTfAddr(const Addr &parentScopeAddr,
            const Addr &codeAddr) const;
    /**
     * @brief 将code进行解析，更新（reset）参数属性表中的属性，尤其是地址属性
     * @param code
     * @brief 这个类用于在扫描过程中存储各个变量对应的属性。
     * @brief 包括S_Dz、S_AR、、Z_Dz、S_AR、Y_Dz、S_Bs等类型
     * @attention 由于在经过一些操作后变量的类型可能会修改，所以注意及时更新变量的类型
     */
    void updateArgAttributeTable(const Code &code) {
#if debug
        {
            static int ignorecount = 0;
            cout << "updateArgAttributeTable(const Code&)" << " "
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        // setArgScopeAddr();
        addArgsToCurrentAR();

        Arg &&arg1 = code[0];
        Arg &&arg2 = code[1];
        Arg &&argop = code[2];
        Arg &&argres = code[3];
        if (arg1.isBs()) {
            resetAttribute(arg1.arg_s, currentCode.scopeAddr, arg1.argFlag,
                    false);

        } else if (arg1.isDz()) {
            resetAttribute(arg1.arg_a, currentCode.scopeAddr, arg1.argFlag,
                    false);
        };
        if (arg2.isBs()) {
            resetAttribute(arg2.arg_s, currentCode.scopeAddr, arg2.argFlag,
                    false);

        } else if (arg2.isDz()) {
            resetAttribute(arg2.arg_a, currentCode.scopeAddr, arg2.argFlag,
                    false);
        };
        if (argop.isBs()) {
            resetAttribute(argop.arg_s, currentCode.scopeAddr, argop.argFlag,
                    false);

        } else if (argop.isDz()) {
            resetAttribute(argop.arg_a, currentCode.scopeAddr, argop.argFlag,
                    false);
        };
        if (argres.isBs()) {
            resetAttribute(argres.arg_s, currentCode.scopeAddr, argres.argFlag,
                    false);

        } else if (argres.isDz()) {
            resetAttribute(argres.arg_a, currentCode.scopeAddr, argres.argFlag,
                    false);
        };
    }

};
class DataTable;

class DataTable;
class Data {
public:
    //dataFlagBit 相当于Arg的flagBit（其实data和Arg差不多）
    Ints dataFlagBit = 0;

    //弃用
    Ints addrFlagBit = 0;

    Numb content_i = 0;
    Strg content_s;
    Addr content_a;
    DataTable *content_ar = NULL;
    //弃用
//    Addr scopeStructureAddr;
    Addr linkAddr;
    Data() {
    }
    ~Data() {
    }
    Data& DataNull() {

        dataFlagBit = -1;
        return (*this);
    }
    Data& operator=(const Data &data_);
    Data& operator=(const Arg &arg);
    Data& deepClone(const Data &data_);
    Data& deepClone(const Arg &arg);
    const bool operator==(const Data &data_) const;
    Data(const Arg &argName);

    Data(const Numb &num);

    Data(const Strg &strg);

    Strg toStrg() const;

};
static Data datanull = Data();
typedef Arg ArgFormal;
typedef Arg ArgReal;
class DataTable {
public:
    bool backwardExecute = F_;
    //此deque用于指定反向函数运行结束后应当跳转到的meetaddr。
    deque<Addr> bfMeetAddrdq;

    Intg type = 0;
    map<Addr, Data> admap;
    map<Strg, Data> sdmap;
    map<ArgFormal, ArgReal> frmap;

    Addr scopeStructureAddr;
    DataTable *parentAR = NULL;
    DataTable *returnAR = NULL;
    Addr meetAddr;
    deque<DataTable*> queryARList;
    DataTable() {
    }
    /**
     *
     * @param parentAR
     * @param queryAR
     * @param scopeStructureAddr
     */
    DataTable(DataTable *parentAR, DataTable *queryAR,
            const Addr &scopeStructureAddr);
    DataTable(DataTable *parentAR, DataTable *queryAR, DataTable *returnAR,
            const Addr &scopeStructureAddr, const Intg &type);

    void setFRMap(const map<ArgFormal, ArgReal> &frmap);

    const Intg addData(const Addr &addr, const Data &data);
    Data& operator[](const Addr &addr);
    Data& operator[](const Strg &name);
    Data& operator[](const Arg &arg);
    bool operator==(DataTable &dt) const;
    DataTable& operator=(const DataTable &dt);
    bool add(const Arg &arg);
    bool findcurrent(const Arg &arg);

    /**
     * @ 返回arg所在scope的addr，当前ar及其子ar
     * @param arg
     * @return
     */
    Addr getArgScopeAddr(const Arg &arg);
    bool findall(const Arg &arg);
    DataTable* getArgAR(const Arg &arg);
    Addr setArgScopeAddr(Arg &arg);
    DataTable* getAccessibleAR(const Addr &arScopeAddr);
};
class ReferenceData {
public:
    Data *refData = NULL;
    deque<Arg> referencePath;
    DataTable *rootAR = NULL;
    Addr scopeAddr;

    Data& getData();

    DataTable* const getDestAR();

    Arg getArg() const;

    void clear();

    bool operator==(const ReferenceData &refdata);

    ReferenceData& operator=(const ReferenceData &referenceData) {
        this->refData = referenceData.refData;
        this->referencePath = referenceData.referencePath;
        this->rootAR = referenceData.rootAR;
        this->scopeAddr = referenceData.scopeAddr;
        return *this;
    }

    Strg toStrg();
    ~ReferenceData() {
#if debug
        {
            cout << "~ReferenceData()" << " " << "\tignorecount:[" << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
    }

};
class ReferenceTable {
public:
    Intg mode = MODE_SCAN;

    //Addr 为Y_Dz对应的Addr，亦即变量名
    multimap<Addr, ReferenceData> addr_ref_map;
//    //rootAR用于检索、删除、修改引用的实例。不同rootAR中的引用不一定相同
//    DataTable *rootAR = NULL;
//    void setRootAR(const DataTable *rootAR) {
//        this->rootAR = rootAR;
//    }

    void addRefData(DataTable *rootAR, CodeTable *codeTable,
            const Addr &currentCodeAddr, const Intg &pos);

    void addRefData(DataTable *rootAR, CodeTable *codeTable,
            const Addr &currentCodeAddr, Arg argref);

    void clear();

    void erase(const Arg &arg);

    Arg operator[](const Arg &y_dz_arg) const;

    Data& getDataRef(const Arg &y_dz_arg);

    ReferenceData getReferenceData(const Arg &y_dz_arg);

};
static ReferenceTable globalRefTable;
/**
 * @brief 返回reference指向的实例Data的引用，例如a.b.c = 10，则返回10对应data
 * @return
 */
Data& ReferenceData::getData() {
    if (refData) {
        return *refData;

    } else if (referencePath.size() > 0 && rootAR) {
        Data *data = NULL;
        DataTable *rootAR = this->rootAR;
        for (auto &arg : referencePath) {
            data = &rootAR->operator [](arg);
            if (data->dataFlagBit == S_AR) {
                rootAR = data->content_ar;
            }
        }
        return *data;
    }
#if debug
    {
        static int ignorecount = 0;
        cerr << "getData()" << " can't find ref,exit(-1)" << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    exit(-1);
}
/**
 *
 * @return 返回所指Arg所被创建的AR
 */
DataTable* const ReferenceData::getDestAR() {

    if (referencePath.size() == 0) {
        return rootAR;

    } else if (referencePath.size() > 0 && rootAR) {
        Data *data = NULL;
        DataTable *rootAR = this->rootAR;
        Intg pos = 0;
        Intg len = referencePath.size();

        while (pos < len - 1) {
            Arg& arg = referencePath[pos];
            try {
                data = &rootAR->operator [](arg);
                if (data->dataFlagBit == S_AR) {
                    rootAR = data->content_ar;
                } else {
                    break;
                }

            } catch (Arg &immed_arg) {
#if debug
                {
                    static int ignorecount = 0;
                    cout << "ReferenceData::getDestAR()" << " err ,exit(-1) "
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
                exit(-1);
            }
            ++pos;
        }
        return rootAR;
    }
#if debug
    {
        static int ignorecount = 0;
        cerr << "getDestAR()" << " can't find ref,exit(-1)" << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    exit(-1);

}
/**
 * @brief 获得reference的末端Arg的复制，例如a.b.c = 10，则返回c，通过c的asc来确定(此步骤重置c的asc),
 * 当为MODE_SCAN时,需要提供AttributeTable*
 * @return
 */
Arg ReferenceData::getArg() const {
#if debug
    {
        static int ignorecount = 0;
        cout << "getArg()" << " " << "\tignorecount:[" << ignorecount++
                << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    Arg refArg;

    Data *data = NULL;
    DataTable *rootAR = this->rootAR;
    Intg pathLen = referencePath.size();
    Intg pos = 0;
    while (pos < pathLen - 1) {
        try {
            data = &rootAR->operator [](referencePath[pos]);
            if (data->dataFlagBit == S_AR) {
                rootAR = data->content_ar;
            }
        } catch (Arg &is_immed_arg) {
            break;

        }
        pos++;
    }
    refArg = referencePath.back();
    refArg.asc = rootAR->getArgScopeAddr(referencePath.back());
    return refArg;

#if debug
    {
        static int ignorecount = 0;
        cout << "getArg()" << " can't find arg,exit(-1)" << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    exit(-1);
}
void ReferenceData::clear() {
    refData = NULL;

    referencePath.clear();
}
/**
 * @attention 当两者之一不存在实例时（refData == NULL &&rootAR == NULL），只比较两个引用的指向arg（基本不会出现）
 * @attention 当两者均存在实例时refData != NULL ||rootAR != NULL，只有两个引用的指向Arg和指向arg所在AR
 * @attention 不要比较两个引用所指的data，因为预执行时其所指的data可能为空
 * @param refdata
 * @return
 */
bool ReferenceData::operator==(const ReferenceData &refdata) {
    if ((refData == NULL && rootAR == NULL)
            || (refdata.refData == NULL && refdata.rootAR == NULL)) {
        return getArg() == const_cast<ReferenceData*>(&refdata)->getArg();
    } else {
        return getArg() == const_cast<ReferenceData*>(&refdata)->getArg()
                && getDestAR()
                        == const_cast<ReferenceData*>(&refdata)->getDestAR();
    }
}
Strg ReferenceData::toStrg() {
    stringstream ss;
    for (auto &arg : referencePath) {
        ss << arg.toStrg() << "\t";
    }
    return ss.str();
}

/**
 * @brief 定位一个引用被使用的地址，然后分析代码并初始化一个referenceData
 * @param rootAR 引用所在的ar
 * @param codeTable
 * @param currentCodeAddr【引用被定义的地址，此地址（类型为Y）】
 * @param pos 【0,1,2,3】
 */

void ReferenceTable::addRefData(DataTable *rootAR, CodeTable *codeTable,
        const Addr &currentCodeAddr, const Intg &pos) {
#if debug
    {
        static int ignorecount = 0;
        cout
                << "ReferenceTable::addRefData(DataTable*, CodeTable*, const Addr&, const int&)"
                << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    Addr adref = currentCodeAddr;
    Code *code = &(*codeTable)[adref];
    Arg &&argref = (*code)[pos];
    argref.arg_ar = rootAR;
#if debug
    {
        static int ignorecount = 0;
        cout
                << "ReferenceTable::addRefData(DataTable*, CodeTable*, const Addr&, const int&)"
                << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    auto &&it = addr_ref_map.find(argref.arg_a);
#if debug
    {
        static int ignorecount = 0;
        cout
                << "ReferenceTable::addRefData(DataTable*, CodeTable*, const Addr&, const int&)"
                << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    if (it != addr_ref_map.end()) {
        Intg n = addr_ref_map.count(argref.arg_a);
        while (--n >= 0) {
            if (it->second.rootAR == rootAR) {
#if debug
                {
                    static int ignorecount = 0;
                    cout
                            << "addRefData(DataTable*, CodeTable*, const Addr&, const int&)"
                            << " referenceData exist,return"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
                return;
            }
            ++it;
        }
        pair<Addr, ReferenceData> p(*it);
        p.second.rootAR = rootAR;
        addr_ref_map.insert(p);
        return;
    }
    pair<Addr, ReferenceData> p;
    while (true) {
        if (code->includeArg(argref)) {
            Arg &&arg1 = (*code)[0];
            Arg &&arg2 = (*code)[1];
            Arg &&argop = (*code)[2];
            p.second.referencePath.push_front(arg2);
            if (arg1.argFlag == Y_Dz) {
                argref = arg1;
                adref = codeTable->getLast(Y, adref);
                code = &(*codeTable)[adref];
            } else {
                p.second.referencePath.push_front(arg1);
                break;
            }

        }
    }
    p.first = adref;
    p.second.rootAR = rootAR;
    p.second.scopeAddr = (*codeTable)[currentCodeAddr].scopeAddr;
    addr_ref_map.insert(p);
    return;

}
/**
 * @brief 定位一个引用被使用的地址，以及引用变量，然后分析代码并初始化一个referenceData
 * @param rootAR 非scan模式专用
 * @param currentCodeAddr【引用被定义的地址，此地址（类型为Y）】
 * @param argref 【类型Y_Dz】
 */
void ReferenceTable::addRefData(DataTable *rootAR, CodeTable *codeTable,
        const Addr &currentCodeAddr, Arg argref) {
#if debug
    {
        static int ignorecount = 0;
        cout << "addRefData(DataTable*, CodeTable*, const Addr&, Arg)" << " "
                << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    Addr adref = currentCodeAddr;
    Code *code = &(*codeTable)[adref];

    auto &&it = addr_ref_map.find(argref.arg_a);
    if (it != addr_ref_map.end()) {
        Intg n = addr_ref_map.count(argref.arg_a);
        while (--n >= 0) {
            if (it->second.rootAR == rootAR) {
#if debug
                {
                    static int ignorecount = 0;
                    cout
                            << "addRefData(DataTable*, CodeTable*, const Addr&, const int&)"
                            << " referenceData exist,return"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
                return;
            }
            ++it;
        }
        pair<Addr, ReferenceData> p(*it);
        p.second.rootAR = rootAR;
        addr_ref_map.insert(p);
        return;
    }
    pair<Addr, ReferenceData> p;
    while (true) {
        if (code->includeArg(argref)) {
            Arg &&arg1 = (*code)[0];
            Arg &&arg2 = (*code)[1];
            Arg &&argop = (*code)[2];
            p.second.referencePath.push_front(arg2);
            if (arg1.argFlag == Y_Dz) {
                argref = arg1;
                adref = codeTable->getLast(Y, adref);
                code = &(*codeTable)[adref];
            } else {
                p.second.referencePath.push_front(arg1);
                break;
            }

        }
    }
    p.first = adref;
    p.second.rootAR = rootAR;
    p.second.scopeAddr = (*codeTable)[currentCodeAddr].scopeAddr;
    addr_ref_map.insert(p);
    return;

}
void ReferenceTable::clear() {
#if debug
    {
        static int ignorecount = 0;
        cout << "ReferenceTable::clear()" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    addr_ref_map.clear();
}
void ReferenceTable::erase(const Arg &arg) {
    if (arg.argFlag != Y_Dz) {
#if debug
        {
            static int ignorecount = 0;
            cout << "void erase(const Arg &arg)" << " arg isn't ref,exit(-1)"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }
    addr_ref_map.erase(arg.arg_a);
}
/**
 * @brief 根据arg获得其对应的referenceData（注意得到的不是引用），参数arg的asc以表示其所引用的参数所在scope
 * @param arg必须为引用Y_Dz！
 * @return
 */

ReferenceData ReferenceTable::getReferenceData(const Arg &y_dz_arg) {
#if debug
    {
        static int ignorecount = 0;
        cout << "ReferenceTable::getReferenceData(const Arg&)" << " "
                << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    const DataTable *rootAR = y_dz_arg.arg_ar;
    const Arg &arg = y_dz_arg;
    if (arg.argFlag != Y_Dz) {
#if debug
        {
            static int ignorecount = 0;
            cout << "operator [](const Arg&)" << " arg isn't ref,exit(-1)"

            << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }

    if (rootAR == NULL) {
        return addr_ref_map.find(arg.arg_a)->second;
    } else {
        Intg n = addr_ref_map.count(arg.arg_a);
        auto it = addr_ref_map.find(arg.arg_a);
        while (--n >= 0) {
            if ((*it).second.rootAR == rootAR) {

                return it->second;
            }
            ++it;
        }
#if debug
        {
            static int ignorecount = 0;
            cerr << "getReferenceData(const Arg&)"
                    << " can't find matched AR,exit(-1)" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }

}
/**
 * @brief 根据arg获得其对应的参数（非Data），参数arg的asc以表示其所引用的参数所在scope
 * @param arg必须为引用Y_Dz！
 * @return
 */
Arg ReferenceTable::operator[](const Arg &y_dz_arg) const {

#if debug
    {
        static int ignorecount = 0;
        cout << "ReferenceTable::operator [](const Arg&)" << " "
                << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    const DataTable *rootAR = y_dz_arg.arg_ar;
    const Arg &arg = y_dz_arg;
    if (arg.argFlag != Y_Dz) {
#if debug
        {
            static int ignorecount = 0;
            cout << "operator [](const Arg&)" << " arg isn't ref,exit(-1)"

            << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }

    if (rootAR == NULL) {
        return addr_ref_map.find(arg.arg_a)->second.getArg();
    } else {
        Intg n = addr_ref_map.count(arg.arg_a);
        auto it = addr_ref_map.find(arg.arg_a);
        while (--n >= 0) {
            if ((*it).second.rootAR == rootAR) {

                return it->second.getArg();
            }
            ++it;
        }
#if debug
        {
            static int ignorecount = 0;
            cerr << "operator [](const Arg&)"
                    << " can't find matched AR,exit(-1)" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }

}
/**
 * @brief 根据arg获得其对应的参数（非Data），参数arg的asc以表示其所引用的参数所在scope
 * @param arg必须为引用Y_Dz！
 * @return
 */
Data& ReferenceTable::getDataRef(const Arg &y_dz_arg) {
    const Arg &arg = y_dz_arg;
    const DataTable *rootAR = y_dz_arg.arg_ar;
    if (arg.argFlag != Y_Dz) {
#if debug
        {
            static int ignorecount = 0;
            cout << "getDataRef(const Arg &arg)" << " arg isn't ref,exit(-1)"

            << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }
    if (rootAR == NULL) {
#if debug
        {
            static int ignorecount = 0;
            cout << "getDataRef(const pair<DataTable*,Arg>)"
                    << " rootAR == NULL,can't get examples" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    } else {
        Intg n = addr_ref_map.count(arg.arg_a);
        auto &&it = addr_ref_map.find(arg.arg_a);
        while (--n >= 0) {
            if ((*it).second.rootAR == rootAR) {
                return it->second.getData();
            }
            ++it;
        }
#if debug
        {
            static int ignorecount = 0;
            cerr << "getDataRef(const Arg &arg)"
                    << " can't find matched AR,exit(-1)" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }

}
/**
 * @var globalRefTable 全局均仅使用此一个引用表，引用只在执行和预执行时被确定，因此此表也在这两个阶段被使用
 * @attention 引用将被转换为所指向的参数进行比较
 */

const bool Arg::operator<(const Arg &arg) const {
    /*    if (this->argFlag == Y_Dz) {
     return globalRefTable[*this] < arg;
     }
     if (arg.argFlag == Y_Dz) {
     return *this < globalRefTable[arg];
     }*/

    if (argFlag != arg.argFlag) {
        return argFlag < arg.argFlag;
    } else {
        return (100 * (arg_i < arg.arg_i) + 10 * (arg_a < arg.arg_a)
                + (arg_s < arg.arg_s)) > 0;
    }
}
/**
 * @attention 对于非引用，不比较asc等标志位，仅仅比较内容是否相同。
 * 对于引用与非引用的比较，则比较引用所指变量的asc与非引用变量的asc是否相同。
 * 引用间的比较，仅比较引用路径是否相同。
 * @attention 注意！！由于引用与非引用之间进行比较时比较asc位，此时应该选取asc位有效的变量进行比较
 * @param arg
 * @return
 */
bool Arg::operator==(const Arg &arg) const {

    if (this->argFlag == Y_Dz && arg.argFlag == Y_Dz) {
#if debug
        {
            static int ignorecount = 0;
            cout << "Arg::operator ==(const Arg&)" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        //在预执行过程中，multimap相当于普通的map
        return globalRefTable.addr_ref_map.find(this->arg_a)->second
                == globalRefTable.addr_ref_map.find(arg.arg_a)->second;
    } else {
        if (this->argFlag == Y_Dz) {
#if debug
            {
                static int ignorecount = 0;
                cout << "Arg::operator ==(const Arg&)" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            return globalRefTable[*this] == arg
                    && globalRefTable[*this].asc == arg.asc;
        }
        if (arg.argFlag == Y_Dz) {
#if debug
            {
                static int ignorecount = 0;
                cout << "Arg::operator ==(const Arg&)" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            return *this == globalRefTable[*this]
                    && this->asc == globalRefTable[*this].asc;
        }
    }
#if debug
    {
        static int ignorecount = 0;
        cout << "Arg::operator ==(const Arg&)" << " arg1:[" << this->toStrg()
                << "]\targ2:[" << arg.toStrg() << "]\t" << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;

    }
#endif

    return argFlag == arg.argFlag && arg_i == arg.arg_i && arg_a == arg.arg_a
            && arg_s == arg.arg_s;

}
bool Arg::operator!=(const Arg &arg) const {
    return !((*this) == arg);
}
/**
 * @attention 使用等号赋值，若右值为引用，则会将右值的引用转化为其所引用的arg进行赋值。
 * @param arg
 * @return
 */
Arg& Arg::operator=(const Arg &arg) {

    arg_i = arg.arg_i;
    arg_a = arg.arg_a;
    arg_s = arg.arg_s;
    arg_ar = arg.arg_ar;
    argFlag = arg.argFlag;
    formalArg = arg.formalArg;
    asc = arg.asc;
    this->changeable = arg.changeable;
    this->unknown = arg.unknown;
    this->arg_ar = arg.arg_ar;
    return (*this);
}
Arg& Arg::clone(const Arg &arg) {

    arg_i = arg.arg_i;
    arg_a = arg.arg_a;
    arg_s = arg.arg_s;
    argFlag = arg.argFlag;
    formalArg = arg.formalArg;
    asc = arg.asc;
    this->changeable = arg.changeable;
    this->unknown = arg.unknown;
    this->arg_ar = arg.arg_ar;
    return (*this);
}
Arg::Arg(Intg flag, const Addr &arg_a) {
    argFlag = flag;
    this->arg_a = arg_a;
}
Arg::Arg(Intg flag, const Numb &arg_i) {
    argFlag = flag;
    this->arg_i = arg_i;
}
Arg::Arg(Intg flag, const Strg &arg_s) {
    argFlag = flag;
    this->arg_s = arg_s;
}
Arg::Arg(Intg flag, DataTable *const ar) {
    argFlag = flag;
    this->arg_ar = arg_ar;
}
Arg::Arg(Intg flag, const Addr &addr, DataTable *const ar) {
    argFlag = flag;
    this->arg_a = addr;
    this->arg_ar = arg_ar;
}
Arg::Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s) {
    argFlag = flag;
    this->arg_a = arg_a;
    this->arg_i = arg_i;
    this->arg_s = arg_s;
}
Arg::Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s,
        const bool &formalArg) {
    argFlag = flag;
    this->arg_a = arg_a;
    this->arg_i = arg_i;
    this->arg_s = arg_s;
    this->formalArg = formalArg;
}
Arg::Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s,
        const bool &formalArg, const Addr &asc) {
    argFlag = flag;
    this->arg_a = arg_a;
    this->arg_i = arg_i;
    this->arg_s = arg_s;
    this->formalArg = formalArg;
    this->asc = asc;
}
Arg::Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s,
        const bool &formalArg, const Intg &changeable, const Addr &asc) {
    argFlag = flag;
    this->arg_a = arg_a;
    this->arg_i = arg_i;
    this->arg_s = arg_s;
    this->formalArg = formalArg;
    this->changeable = changeable;
    this->asc = asc;
}
Arg::Arg(Intg flag, const Addr &arg_a, const Numb &arg_i, const Strg &arg_s,
        DataTable *arg_ar, const bool &formalArg, const Intg &changeable,
        const Intg &unknown, const Addr &asc) {
    argFlag = flag;
    this->arg_a = arg_a;
    this->arg_i = arg_i;
    this->arg_s = arg_s;
    this->arg_ar = arg_ar;
    this->formalArg = formalArg;
    this->changeable = changeable;
    this->unknown = unknown;
    this->asc = asc;
}
Strg Arg::toStrg() const {
    /* if (this->argFlag == Y_Dz) {
     cout << "Y_Dz\t" << "path:["
     << globalRefTable.addr_ref_map.find(arg_a)->second.toStrg();
     return globalRefTable[*this].toStrg();
     }*/
    stringstream ss;
    ss << "argFlag:[" << argFlag << "]\t" << "arg_i:[" << arg_i << "]\t"
            << "arg_a:[" << arg_a.toStrg() << "]\t" << "arg_s:[" << arg_s
            << "]\t" << "arg_ar:[" << arg_ar << "]\t" << "formalArg:["
            << formalArg << "]\t" << "changeable:[" << changeable << "]\t"
            << "asc:[" << asc.toStrg() << "]\t" << endl;

    return ss.str();
}
/**
 * @brief 将CodeTable中所有的argOld替换成argNew
 * @attention 注意，不替换非参数类型标志位/ar
 * @param argOld
 * @param argNew
 */
void CodeTable::replaceVar(const Arg &argOld, const Arg &argNew) {
    for (auto &it : codedq) {
        if (it.assemblyFormula.flagBit.arg1_flag == argOld.argFlag
                && it.assemblyFormula.quaternion.arg1_a == argOld.arg_a
                && it.assemblyFormula.quaternion.arg1_s == argOld.arg_s
                && it.assemblyFormula.quaternion.arg1_i == argOld.arg_i) {
            it.assemblyFormula.flagBit.arg1_flag = argNew.argFlag;
            it.assemblyFormula.quaternion.arg1_a = argNew.arg_a;
            it.assemblyFormula.quaternion.arg1_s = argNew.arg_s;
            it.assemblyFormula.quaternion.arg1_i = argNew.arg_i;

        }
        if (it.assemblyFormula.flagBit.arg2_flag == argOld.argFlag
                && it.assemblyFormula.quaternion.arg2_a == argOld.arg_a
                && it.assemblyFormula.quaternion.arg2_s == argOld.arg_s
                && it.assemblyFormula.quaternion.arg2_i == argOld.arg_i) {
            it.assemblyFormula.flagBit.arg2_flag = argNew.argFlag;
            it.assemblyFormula.quaternion.arg2_a = argNew.arg_a;
            it.assemblyFormula.quaternion.arg2_s = argNew.arg_s;
            it.assemblyFormula.quaternion.arg2_i = argNew.arg_i;

        }
        if (it.assemblyFormula.flagBit.operator_flag == argOld.argFlag
                && it.assemblyFormula.quaternion.operator_a == argOld.arg_a
                && it.assemblyFormula.quaternion.operator_s == argOld.arg_s
                && it.assemblyFormula.quaternion.operator_i == argOld.arg_i) {
            it.assemblyFormula.flagBit.operator_flag = argNew.argFlag;
            it.assemblyFormula.quaternion.operator_a = argNew.arg_a;
            it.assemblyFormula.quaternion.operator_s = argNew.arg_s;
            it.assemblyFormula.quaternion.operator_i = argNew.arg_i;

        }
        if (it.assemblyFormula.flagBit.result_flag == argOld.argFlag
                && it.assemblyFormula.quaternion.result_a == argOld.arg_a
                && it.assemblyFormula.quaternion.result_s == argOld.arg_s
                && it.assemblyFormula.quaternion.result_i == argOld.arg_i) {
            it.assemblyFormula.flagBit.result_flag = argNew.argFlag;
            it.assemblyFormula.quaternion.result_a = argNew.arg_a;
            it.assemblyFormula.quaternion.result_s = argNew.arg_s;
            it.assemblyFormula.quaternion.result_i = argNew.arg_i;

        }
    }

}
/**
 * @brief 重新设置codeTable中和argset有关的变量的可变属性。无视所有标志位
 * @param arg
 * @param changeable true(设置为可变) false（设置为不可变）
 * @attention argset的changeable属性无效
 */
void CodeTable::resetChangeable(bool changeable, const set<Arg> &argset) {
    set<Arg> argsettemp = argset;
    if (changeable == true) {
        for (auto it : codedq) {
            Arg &&arg1 = it[0];
            Arg &&arg2 = it[1];
            Arg &&argop = it[2];
            Arg &&argres = it[3];
            if ((!arg1.changeable) && argsettemp.count(arg1)) {
                it.assemblyFormula.flagBit.changeable[0] = T_;

            }
            if ((!arg2.changeable) && argsettemp.count(arg2)) {
                it.assemblyFormula.flagBit.changeable[1] = T_;

            }
            if ((!argop.changeable) && argsettemp.count(argop)) {
                it.assemblyFormula.flagBit.changeable[2] = T_;

            }
            if (!argres.changeable) {
                if (argsettemp.count(argres)) {
                    it.assemblyFormula.flagBit.changeable[3] = T_;
                } else if (it.assemblyFormula.flagBit.changeable[0] == T_
                        || it.assemblyFormula.flagBit.changeable[1] == T_
                        || it.assemblyFormula.flagBit.changeable[2] == T_) {
                    it.assemblyFormula.flagBit.changeable[3] = T_;
                    argsettemp.insert(argres);

                }

            }

        }

    } else {
        for (auto it : codedq) {
            Arg &&arg1 = it[0];
            Arg &&arg2 = it[1];
            Arg &&argop = it[2];
            Arg &&argres = it[3];
            if (arg1.changeable && argsettemp.count(arg1)) {
                it.assemblyFormula.flagBit.changeable[0] = F_;

            }
            if (arg2.changeable && argsettemp.count(arg2)) {
                it.assemblyFormula.flagBit.changeable[1] = F_;

            }
            if (argop.changeable && argsettemp.count(argop)) {
                it.assemblyFormula.flagBit.changeable[2] = F_;

            }

            if (argres.changeable) {
                if (argsettemp.count(argres)) {
                    it.assemblyFormula.flagBit.changeable[3] = F_;
                } else if (it.assemblyFormula.flagBit.changeable[0] == F_
                        && it.assemblyFormula.flagBit.changeable[1] == F_
                        && it.assemblyFormula.flagBit.changeable[2] == F_) {
                    it.assemblyFormula.flagBit.changeable[3] = F_;
                    argsettemp.insert(argres);

                }

            }

        }

    }

}
/**
 * @brief 将code的指定位置（0,1,2,3,）的相应参数的“参数类型”、“参数的值”替换为arg的对应值，其他属性不变
 * @param position
 * @param arg
 */
void Code::setArg(const Intg &position, const Arg &arg) {
    switch (position) {
    case 0:
        assemblyFormula.flagBit.arg1_flag = arg.argFlag;
        assemblyFormula.quaternion.arg1_a = arg.arg_a;
        assemblyFormula.quaternion.arg1_s = arg.arg_s;
        assemblyFormula.quaternion.arg1_i = arg.arg_i;
        break;
    case 1:
        assemblyFormula.flagBit.arg2_flag = arg.argFlag;
        assemblyFormula.quaternion.arg2_a = arg.arg_a;
        assemblyFormula.quaternion.arg2_s = arg.arg_s;
        assemblyFormula.quaternion.arg2_i = arg.arg_i;
        break;
    case 2:
        assemblyFormula.flagBit.operator_flag = arg.argFlag;
        assemblyFormula.quaternion.operator_a = arg.arg_a;
        assemblyFormula.quaternion.operator_s = arg.arg_s;
        assemblyFormula.quaternion.operator_i = arg.arg_i;
        break;
    case 3:
        assemblyFormula.flagBit.result_flag = arg.argFlag;
        assemblyFormula.quaternion.result_a = arg.arg_a;
        assemblyFormula.quaternion.result_s = arg.arg_s;
        assemblyFormula.quaternion.result_i = arg.arg_i;
        break;
    default:
        break;
    }

}
/**
 * @brief 获得对应位置（arg1，arg2，op，res）位置的参数
 * @attention 对于asc标志位，若代码中明确定义了asc，则以asc为准，
 * 否则若asc为空且对应arg确实为一个变量，则asc取代码所在scopeAddr
 * @param pos
 * @return
 */
Arg Code::operator[](const Intg &pos) const {
    /*
     #if debug
     {
     static int ignorecount = 0;
     cout << "Code::operator [](const int&)" << " " << "\tignorecount:["
     << ignorecount++ << "\t]("
     << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
     << __LINE__ << ":0" << ")" << endl;
     }
     #endif
     */
    Intg enableFormalArgBit = false;
// if (assemblyFormula.flagBit.execute == F_) {
    enableFormalArgBit = true;
//}
    Arg arg;
    if (pos == 0) {
        arg.clone(
                Arg(assemblyFormula.flagBit.arg1_flag,
                        assemblyFormula.quaternion.arg1_a,
                        assemblyFormula.quaternion.arg1_i,
                        assemblyFormula.quaternion.arg1_s,
                        assemblyFormula.quaternion.arg1_ar,
                        enableFormalArgBit
                                && assemblyFormula.flagBit.formalArgFlag[0],
                        assemblyFormula.flagBit.changeable[0],
                        assemblyFormula.flagBit.unknown[0],
                        assemblyFormula.flagBit.a1s));

    } else if (pos == 1) {
        arg.clone(
                Arg(assemblyFormula.flagBit.arg2_flag,
                        assemblyFormula.quaternion.arg2_a,
                        assemblyFormula.quaternion.arg2_i,
                        assemblyFormula.quaternion.arg2_s,
                        assemblyFormula.quaternion.arg2_ar,
                        enableFormalArgBit
                                && assemblyFormula.flagBit.formalArgFlag[1],
                        assemblyFormula.flagBit.changeable[1],
                        assemblyFormula.flagBit.unknown[1],
                        assemblyFormula.flagBit.a2s));
    } else if (pos == 2) {
        arg.clone(
                Arg(assemblyFormula.flagBit.operator_flag,
                        assemblyFormula.quaternion.operator_a,
                        assemblyFormula.quaternion.operator_i,
                        assemblyFormula.quaternion.operator_s,
                        assemblyFormula.quaternion.operator_ar,
                        enableFormalArgBit
                                && assemblyFormula.flagBit.formalArgFlag[2],
                        assemblyFormula.flagBit.changeable[2],
                        assemblyFormula.flagBit.unknown[2],
                        assemblyFormula.flagBit.ops));
    } else if (pos == 3) {
        arg.clone(
                Arg(assemblyFormula.flagBit.result_flag,
                        assemblyFormula.quaternion.result_a,
                        assemblyFormula.quaternion.result_i,
                        assemblyFormula.quaternion.result_s,
                        assemblyFormula.quaternion.result_ar,
                        enableFormalArgBit
                                && assemblyFormula.flagBit.formalArgFlag[3],
                        assemblyFormula.flagBit.changeable[3],
                        assemblyFormula.flagBit.unknown[3],
                        assemblyFormula.flagBit.res));
    }

    /* if (arg.asc != addrnull
     && (arg.argFlag == S_Bs
     || (arg.argFlag == S_Dz && arg.arg_a != addrnull)
     || arg.argFlag == Y_Dz)) {
     arg.asc = scopeAddr;
     }*/
    return arg;
#if debug
    cerr << "Code[n] ouf of bound,should be 0~3 " << endl;
#endif
}
/**
 * @brief 判断一行代码中是否包含另一个变量。
 * @attention 不考虑引用所指向的变量
 * @param arg
 * @return
 */
const bool Code::includeArg(const Arg &arg) const {

    return (assemblyFormula.flagBit.arg1_flag == arg.argFlag
            && assemblyFormula.quaternion.arg1_a == arg.arg_a
            && assemblyFormula.quaternion.arg1_i == arg.arg_i
            && assemblyFormula.quaternion.arg1_s == arg.arg_s)
            || (assemblyFormula.flagBit.arg2_flag == arg.argFlag
                    && assemblyFormula.quaternion.arg2_a == arg.arg_a
                    && assemblyFormula.quaternion.arg2_i == arg.arg_i
                    && assemblyFormula.quaternion.arg2_s == arg.arg_s)

            || (assemblyFormula.flagBit.operator_flag == arg.argFlag
                    && assemblyFormula.quaternion.operator_a == arg.arg_a
                    && assemblyFormula.quaternion.operator_i == arg.arg_i
                    && assemblyFormula.quaternion.operator_s == arg.arg_s)
            || (assemblyFormula.flagBit.result_flag == arg.argFlag
                    && assemblyFormula.quaternion.result_a == arg.arg_a
                    && assemblyFormula.quaternion.result_i == arg.arg_i
                    && assemblyFormula.quaternion.result_s == arg.arg_s);

}
/**
 * @brief 获得树 分支 上的上一个包括arg的代码的地址
 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量）
 * @param ad
 * @param arg
 * @return
 */
Addr CodeTable::getSameTrBranchLastArgAddr(const Addr &ad, const Arg &arg) {
    Addr addr = getAboveCodeAddr(ad);
    while (this->count(addr)
            && ((*this)[addr].assemblyFormula.flagBit.type == M
                    || (*this)[addr].assemblyFormula.flagBit.type == Y
                    || (*this)[addr].assemblyFormula.flagBit.execute == Du)) {

        if ((*this)[addr].assemblyFormula.flagBit.execute != Du
                && (*this)[addr].assemblyFormula.flagBit.type != Y
                && (*this)[addr].includeArg(arg)) {
            return addr;
        }
        addr = getAboveCodeAddr(addr);
    }
    return addrnull;
}
Addr CodeTable::getSameTrBranchNextArgAddr(const Addr &ad, const Arg &arg) {
#if debug
    {
        static int ignorecount = 0;
        cout << "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
                << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    if (ad == addrnull) {
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        return addrnull;
    }
    Addr addr = getNextCodeAddr(ad);
    while (this->count(addr)
            && ((*this)[addr].assemblyFormula.flagBit.type == M
                    || (*this)[addr].assemblyFormula.flagBit.type == R
                    || (*this)[addr].assemblyFormula.flagBit.type == Y
                    || (*this)[addr].assemblyFormula.flagBit.execute == Du)) {
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if ((*this)[addr].assemblyFormula.flagBit.execute != Du
                && (*this)[addr].assemblyFormula.flagBit.type != Y
                && (*this)[addr].includeArg(arg) == true) {
#if debug
            {
                static int ignorecount = 0;
                cout
                        << "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
                        << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            return addr;
        }
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        addr = getNextCodeAddr(addr);
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "CodeTable::getSameTrBranchNextArgAddr(const Addr&, const Arg&)"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
    }
    return Addr();
}
/**
 * @brief 寻找以一个以root为根节点的子树中的变量arg，root被省略。
 * 因为其实无论root为何值，其都可以被确定。
 * 当arg为标识符变量(M_Bs||S_Bs)时，只需要寻找父树中codeAddr比root的addr小的上一个arg；
 * 当arg为数据地址(S_Dz)时，只需要寻找父树中codeAddr比root的addr小且arg为code[3]的arg（对应的addr）
 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量）
 * @param ad (root)
 * @param arg
 * @return
 */
Addr CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr &ad,
        const Arg &arg) {
    {
#if debug
        static int ignorecount = 0;
        cout
                << "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr&, const Arg&)"
                << "in addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_scan.hpp:" << __LINE__ << ":0"
                << ")" << endl;
#endif
    }
    Addr addr = getAboveCodeAddr(ad);
    if (addr == addrnull) {
#if debug
        cerr
                << "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty can't find above addr,return addrnull, ad:["
                << ad.toStrg() << "]" << endl;
#endif
        return addrnull;
    }
    {
#if debug
        static int ignorecount = 0;
        cout
                << "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr&, const Arg&)"
                << "1 addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_scan.hpp:" << __LINE__ << ":0"
                << ")" << endl;
#endif
    }
    while (this->count(addr)
            && ((*this)[addr].assemblyFormula.flagBit.type == M
                    || (*this)[addr].assemblyFormula.flagBit.type == Y
                    || (*this)[addr].assemblyFormula.flagBit.execute == Du)) {

        if ((*this)[addr].assemblyFormula.flagBit.execute != Du
                && (*this)[addr].assemblyFormula.flagBit.type != Y
                && !((*this)[addr].assemblyFormula.quaternion.operator_s == "$"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)
                && !((*this)[addr].assemblyFormula.quaternion.operator_s == "#"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)
                && !((*this)[addr].assemblyFormula.quaternion.operator_s == ":"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)

                && (*this)[addr].includeArg(arg)) {
            {
#if debug
                static int ignorecount = 0;
                cout
                        << "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr&, const Arg&)"
                        << "out addr:[" << ad.toStrg() << "\t]"
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "vm_scan.hpp:" << __LINE__ << ":0" << ")" << endl;
#endif
            }
            if ((*this)[addr][3] == arg) {
                //arg必须与code的结果位对应才能表明code为根节点的子叶结点，
                //否则只能证明code与根节点有相同的参数
                return addr;
            }

        }
        addr = getAboveCodeAddr(addr);
    }
    {
#if debug
        static int ignorecount = 0;
        cout
                << "CodeTable::getSameTrBranchLastArgAddrIgnoreProperty(const Addr&, const Arg&)"
                << "out addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_scan.hpp:" << __LINE__ << ":0"
                << ")" << endl;
#endif
    }
    return addrnull;
}
/**
 * @brief 寻找以一个以root为根节点的子树中的变量arg，root被省略。
 * 因为其实无论root为何值，其都可以被确定。
 * 当arg为标识符变量(M_Bs||S_Bs)时，只需要寻找父树中codeAddr比root的addr小的上一个arg；
 * 当arg为数据地址(S_Dz)时，只需要寻找父树中codeAddr比root的addr小且arg为code[3]的arg（对应的addr）
 * @param ad (root)
 * @param arg
 * @return Addr, PositionInCode(0,1,2,3)
 */
pair<Addr, Intg> CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(
        const Addr &ad, const Arg &arg) {
    {
#if debug
        static int ignorecount = 0;
        cout
                << "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(const Addr&, const Arg&)"
                << "in addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_scan.hpp:" << __LINE__ << ":0"
                << ")" << endl;
#endif
    }
    Addr addr = getAboveCodeAddr(ad);
    if (addr == addrnull) {
#if debug
        cerr
                << "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty can't find above addr,return addrnull, ad:["
                << ad.toStrg() << "]" << endl;
#endif
        return pair<Addr, Intg>(addrnull, 0);
    }
    {
#if debug
        static int ignorecount = 0;
        cout
                << "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(const Addr&, const Arg&)"
                << "1 addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_scan.hpp:" << __LINE__ << ":0"
                << ")" << endl;
#endif
    }
    while (this->count(addr)
            && ((*this)[addr].assemblyFormula.flagBit.type == M
                    || (*this)[addr].assemblyFormula.flagBit.execute == Du)) {

        if ((*this)[addr].assemblyFormula.flagBit.execute != Du
                && !((*this)[addr].assemblyFormula.quaternion.operator_s == "$"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)
                && !((*this)[addr].assemblyFormula.quaternion.operator_s == "#"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)
                && !((*this)[addr].assemblyFormula.quaternion.operator_s == ":"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)
                && !((*this)[addr].assemblyFormula.quaternion.operator_s
                        == "out"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)

                && (*this)[addr].includeArg(arg)) {
            {
#if debug
                static int ignorecount = 0;
                cout
                        << "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(const Addr&, const Arg&)"
                        << "out addr:[" << ad.toStrg() << "\t]"
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "vm_scan.hpp:" << __LINE__ << ":0" << ")" << endl;
#endif
            }

            if ((*this)[addr][3] == arg) {
                return pair<Addr, Intg>(addr, 3);
            }

        }
        addr = getAboveCodeAddr(addr);
    }
    {
#if debug
        static int ignorecount = 0;
        cout
                << "CodeTable::getSameTrBranchLastArgPositionIgnoreProperty(const Addr&, const Arg&)"
                << "out addr:[" << ad.toStrg() << "\t]" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_scan.hpp:" << __LINE__ << ":0"
                << ")" << endl;
#endif
    }
    return pair<Addr, Intg>(addrnull, 0);
}
/**
 * @brief 查找某个参数在当前代码后出现的位置，若不存在，返回addrnull
 * @attention 忽略Y中的变量（引用路径中的变量不被认为是此作用域中的变量）
 * @param ad (root)
 * @param arg
 * @return Addr, PositionInCode(0,1,2,3)
 */
pair<Addr, Intg> CodeTable::getNextArgPosition(const Addr &ad, const Arg &arg) {
    Intg pos = getAddrPosition(addrdq, ad);
    while (++pos < size()) {
        Code &cd = codedq[pos];
        if (cd.assemblyFormula.flagBit.type == Y) {
            continue;
        }
        if (cd[0] == arg) {

            return pair<Addr, Intg>(addrdq[pos], 0);

        }
        if (cd[1] == arg) {

            return pair<Addr, Intg>(addrdq[pos], 1);

        }
        if (cd[2] == arg) {

            return pair<Addr, Intg>(addrdq[pos], 2);

        }
        if (cd[3] == arg) {

            return pair<Addr, Intg>(addrdq[pos], 3);

        }
    }
    return pair<Addr, Intg>(Addr(), 0);

}
/**
 * @attention 忽略Y，引用中的变量不属于本作用域
 * @param ad
 * @param arg
 * @return
 */
Addr CodeTable::getSameTrBranchNextArgAddrIgnoreProperty(const Addr &ad,
        const Arg &arg) {
#if debug
    cout << "CodeTable::getSameTrBranchNextArgAddr1 ad:[" << ad.toStrg()
            << "]\targ:[" << arg.toStrg() << "]" << endl;
#endif
    if (ad == addrnull) {
#if debug
        cout << "CodeTable::getSameTrBranchNextArgAddr2 addrnull" << endl;
#endif
        return addrnull;
    }
    Addr addr = getNextCodeAddr(ad);
    while (this->count(addr)
            && ((*this)[addr].assemblyFormula.flagBit.type == M
                    //            || (*this)[addr].assemblyFormula.flagBit.type == R
                    || (*this)[addr].assemblyFormula.flagBit.type == Y
                    || (*this)[addr].assemblyFormula.flagBit.execute == Du)) {
#if debug
        cout << "CodeTable::getSameTrBranchNextArgAddr3 " << endl;
#endif
        if ((*this)[addr].assemblyFormula.flagBit.type != Y
                && (*this)[addr].includeArg(arg) == true
                && (*this)[addr].assemblyFormula.flagBit.execute != Du

                && !((*this)[addr].assemblyFormula.quaternion.operator_s == "$"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)
                && !((*this)[addr].assemblyFormula.quaternion.operator_s == "#"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)
                && !((*this)[addr].assemblyFormula.quaternion.operator_s == ":"
                        && (*this)[addr].assemblyFormula.flagBit.operator_flag
                                == M_Bs)

                ) {
#if debug
            cout << "CodeTable::getSameTrBranchNextArgAddr3.5 includeArg"
                    << endl;
#endif
            return addr;
        }
#if debug
        cout << "CodeTable::getSameTrBranchNextArgAddr4 " << endl;
#endif
        addr = getNextCodeAddr(addr);
#if debug
        cout << "CodeTable::getSameTrBranchNextArgAddr5 " << endl;
#endif
    }
    return Addr();
}
typedef Arg ArgDec;
static ArgDec argDecnull = ArgDec();
class DecComparator {
public:
    Scanner *scanner;
    CodeTable *codeTable;
    DecComparator(Scanner *scanner_) {
        scanner = scanner_;
        codeTable = scanner_->codeTable;
    }
    map<ArgDec, ArgDec> am;
//const bool add(const ArgDec& arg, const ArgDec &argtf);
    map<Addr, Addr> cdm;
//const Addr getSameTrBranchLastArgAddr(const Addr& ad, const ArgDec& a);
//const bool cmpDecCodeTree(const Addr & tr, const Addr & trTf);

    DecComparator& null() {
        am.clear();
        cdm.clear();
        return (*this);
    }
    const bool add(const ArgDec &arg, const ArgDec &argtf) {
        if (am[arg] == argtf) {
            return true;
        } else if (am[arg] == argDecnull) {
            am[arg] = argtf;
            return true;
        } else {
            return false;
        }
    }
    const bool add(const Addr &ad1, const Addr &ad2) {
        if (cdm[ad1] == ad2) {
            return true;
        } else if (cdm[ad1] == addrnull) {
            cdm[ad1] = ad2;
            return true;
        } else {
            return false;
        }
    }
    const bool includeArg(const Addr &cdAddr, const ArgDec &arg) const {
        return ((*codeTable)[cdAddr].assemblyFormula.flagBit.arg1_flag
                == arg.argFlag
                && (*codeTable)[cdAddr].assemblyFormula.quaternion.arg1_a
                        == arg.arg_a
                && (*codeTable)[cdAddr].assemblyFormula.quaternion.arg1_i
                        == arg.arg_i
                && (*codeTable)[cdAddr].assemblyFormula.quaternion.arg1_s
                        == arg.arg_s)
                || ((*codeTable)[cdAddr].assemblyFormula.flagBit.arg2_flag
                        == arg.argFlag
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.arg2_a
                                == arg.arg_a
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.arg2_i
                                == arg.arg_i
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.arg2_s
                                == arg.arg_s)

                || ((*codeTable)[cdAddr].assemblyFormula.flagBit.operator_flag
                        == arg.argFlag
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.operator_a
                                == arg.arg_a
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.operator_i
                                == arg.arg_i
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.operator_s
                                == arg.arg_s)
                || ((*codeTable)[cdAddr].assemblyFormula.flagBit.result_flag
                        == arg.argFlag
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.result_a
                                == arg.arg_a
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.result_i
                                == arg.arg_i
                        && (*codeTable)[cdAddr].assemblyFormula.quaternion.result_s
                                == arg.arg_s);

    }
    const Addr getSameTrBranchLastArgAddr(const Addr &ad, const ArgDec &a) {
        Addr addr = (*codeTable).getAboveCodeAddr(ad);
        while ((*codeTable)[addr].assemblyFormula.flagBit.type == M
                || (*codeTable)[addr].assemblyFormula.flagBit.type == Y
                || (*codeTable)[addr].assemblyFormula.flagBit.execute == Du) {
            if ((*codeTable)[addr].assemblyFormula.flagBit.type != Y
                    && includeArg(addr,
                            a) && (*codeTable)[addr].assemblyFormula.flagBit.execute != Du) {
                return addr;
            }
            addr = (*codeTable).getAboveCodeAddr(addr);
        }
        return Addr();

    }
    const bool cmpDecCodeTree(const Addr &tr, const Addr &trTf) {
        if (cdm[tr] == trTf) {
            return true;
        } else if (add(tr, trTf) == false) {
            return false;
        }
        ArgDec arg1_1((*codeTable)[tr].assemblyFormula.flagBit.arg1_flag,
                (*codeTable)[tr].assemblyFormula.quaternion.arg1_a,
                (*codeTable)[tr].assemblyFormula.quaternion.arg1_i,
                (*codeTable)[tr].assemblyFormula.quaternion.arg1_s);
        ArgDec arg2_1((*codeTable)[trTf].assemblyFormula.flagBit.arg1_flag,
                (*codeTable)[trTf].assemblyFormula.quaternion.arg1_a,
                (*codeTable)[trTf].assemblyFormula.quaternion.arg1_i,
                (*codeTable)[trTf].assemblyFormula.quaternion.arg1_s);
        ArgDec arg1_2((*codeTable)[tr].assemblyFormula.flagBit.arg2_flag,
                (*codeTable)[tr].assemblyFormula.quaternion.arg2_a,
                (*codeTable)[tr].assemblyFormula.quaternion.arg2_i,
                (*codeTable)[tr].assemblyFormula.quaternion.arg2_s);
        ArgDec arg2_2((*codeTable)[trTf].assemblyFormula.flagBit.arg2_flag,
                (*codeTable)[trTf].assemblyFormula.quaternion.arg2_a,
                (*codeTable)[trTf].assemblyFormula.quaternion.arg2_i,
                (*codeTable)[trTf].assemblyFormula.quaternion.arg2_s);
        ArgDec operator1_1(
                (*codeTable)[tr].assemblyFormula.flagBit.operator_flag,
                (*codeTable)[tr].assemblyFormula.quaternion.operator_a,
                (*codeTable)[tr].assemblyFormula.quaternion.operator_i,
                (*codeTable)[tr].assemblyFormula.quaternion.operator_s);
        ArgDec operator2_1((*codeTable)[trTf].assemblyFormula.flagBit.arg1_flag,
                (*codeTable)[trTf].assemblyFormula.quaternion.operator_a,
                (*codeTable)[trTf].assemblyFormula.quaternion.operator_i,
                (*codeTable)[trTf].assemblyFormula.quaternion.operator_s);
        ArgDec result1_1((*codeTable)[tr].assemblyFormula.flagBit.result_flag,
                (*codeTable)[tr].assemblyFormula.quaternion.result_a,
                (*codeTable)[tr].assemblyFormula.quaternion.result_i,
                (*codeTable)[tr].assemblyFormula.quaternion.result_s);
        ArgDec result2_1((*codeTable)[trTf].assemblyFormula.flagBit.result_flag,
                (*codeTable)[trTf].assemblyFormula.quaternion.result_a,
                (*codeTable)[trTf].assemblyFormula.quaternion.result_i,
                (*codeTable)[trTf].assemblyFormula.quaternion.result_s);
        if (tr != addrnull && trTf != addrnull) {
            if (add(arg1_1, arg2_1) && add(arg1_2, arg2_2)
                    && add(operator1_1, operator2_1)
                    && add(result1_1, result2_1)) {
                return cmpDecCodeTree(getSameTrBranchLastArgAddr(tr, arg1_1),
                        getSameTrBranchLastArgAddr(trTf, arg2_1))
                        && cmpDecCodeTree(
                                getSameTrBranchLastArgAddr(tr, arg1_2),
                                getSameTrBranchLastArgAddr(trTf, arg2_2))
                        && cmpDecCodeTree(
                                getSameTrBranchLastArgAddr(tr, operator1_1),
                                getSameTrBranchLastArgAddr(trTf, operator2_1))
                        && cmpDecCodeTree(
                                getSameTrBranchLastArgAddr(tr, result1_1),
                                getSameTrBranchLastArgAddr(trTf, result2_1));
            }
            return false;
        } else if (tr == addrnull && trTf == addrnull) {
            return true;
        } else {
            return false;
        }

    }
};
/**
 * 用于确定参数的可变性
 */
typedef Arg ArgPend;
class PendPainter {
public:
    Scanner *scanner = NULL;
    CodeTable *codeTable = NULL;
    PendPainter(Scanner *s_) {
        scanner = s_;
        codeTable = scanner->codeTable;
    }
    const bool painted(const Arg &arg, const Addr &pendAddr) {
        Intg flagBitChangeable = arg.changeable;
        if (flagBitChangeable == T_) {
            return ((*codeTable)[pendAddr][0] != arg
                    || ((*codeTable)[pendAddr][0] == arg
                            && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0]
                                    == T_))
                    && ((*codeTable)[pendAddr][1] != arg
                            || ((*codeTable)[pendAddr][1] == arg
                                    && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1]
                                            == T_))
                    && ((*codeTable)[pendAddr][2] != arg
                            || ((*codeTable)[pendAddr][2] == arg
                                    && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2]
                                            == T_))
                    && ((*codeTable)[pendAddr][3] != arg
                            || ((*codeTable)[pendAddr][3] == arg
                                    && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3]
                                            == T_));
        } else if (flagBitChangeable == A_) {
            return ((*codeTable)[pendAddr][0] != arg
                    || ((*codeTable)[pendAddr][0] == arg
                            && ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0]
                                    == A_
                                    || (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0]
                                            == T_)))
                    && ((*codeTable)[pendAddr][1] != arg
                            || ((*codeTable)[pendAddr][1] == arg
                                    && ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1]
                                            == A_
                                            || (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1]
                                                    == T_)))
                    && ((*codeTable)[pendAddr][2] != arg
                            || ((*codeTable)[pendAddr][2] == arg
                                    && ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2]
                                            == A_
                                            || (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2]
                                                    == T_)))
                    && ((*codeTable)[pendAddr][3] != arg
                            || ((*codeTable)[pendAddr][3] == arg
                                    && ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3]
                                            == A_
                                            || (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3]
                                                    == T_)));
        } else if (flagBitChangeable == F_) {
            return ((*codeTable)[pendAddr][0] != arg
                    || ((*codeTable)[pendAddr][0] == arg
                            && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0]
                                    == F_))
                    && ((*codeTable)[pendAddr][1] != arg
                            || ((*codeTable)[pendAddr][1] == arg
                                    && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1]
                                            == F_))
                    && ((*codeTable)[pendAddr][2] != arg
                            || ((*codeTable)[pendAddr][2] == arg
                                    && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2]
                                            == F_))
                    && ((*codeTable)[pendAddr][3] != arg
                            || ((*codeTable)[pendAddr][3] == arg
                                    && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3]
                                            == F_));
        }
        {
#if debug
            static int ignorecount = 0;
            cerr << "painted(const Arg&, const Addr&)" << " exit(-1)"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "vm_scan.hpp:" << __LINE__ << ":0" << ")" << endl;
#endif
            exit(-1);
        }

    }
    bool paint(const Arg &arg, const Addr &pendAddr) {
        Intg flagBitChangeable = arg.changeable;
        if (pendAddr == addrnull || arg == argnull) {
#if debug
            cout << "PendPainter::paint 1" << endl;
#endif
            return false;
        }

        if (painted(arg, pendAddr) == true) {
#if debug
            cout << "PendPainter::paint 2 painted==true" << endl;
#endif
            return true;
        } else {
            Intg paintArgResultFlag = false;
            if (flagBitChangeable == T_) {
                if ((*codeTable)[pendAddr][0] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0] =
                    T_;
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                    T_;
                    paintArgResultFlag = true;

                }
                if ((*codeTable)[pendAddr][1] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1] =
                    T_;
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                    T_;
                    paintArgResultFlag = true;
                }
                if ((*codeTable)[pendAddr][2] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2] =
                    T_;
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                    T_;
                }
                if ((*codeTable)[pendAddr][3] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                    T_;
                    paintArgResultFlag = true;
                }

            } else if (flagBitChangeable == A_) {

                if ((*codeTable)[pendAddr][0] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0] =
                    A_;
                    if ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3]
                            !=
                            T_) {
                        (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                        A_;
                    }
                    paintArgResultFlag = true;
                }
                if ((*codeTable)[pendAddr][1] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1] =
                    A_;
                    if ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3]
                            !=
                            T_) {
                        (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                        A_;
                    }
                    paintArgResultFlag = true;
                }
                if ((*codeTable)[pendAddr][2] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2] =
                    A_;
                    if ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3]
                            !=
                            T_) {
                        (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                        A_;
                    }
                }
                if ((*codeTable)[pendAddr][3] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                    A_;
                    paintArgResultFlag = true;
                }
                if ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0]
                        == T_
                        || (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1]
                                == T_
                        || (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2]
                                == T_
                        || (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3]
                                == T_) {
                    paintArgResultFlag = false;
                }
            } else if (flagBitChangeable == F_) {
                if ((*codeTable)[pendAddr][0] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0] =
                    F_;

                }
                if ((*codeTable)[pendAddr][1] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1] =
                    F_;
                }
                if ((*codeTable)[pendAddr][2] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2] =
                    F_;
                }
                if ((*codeTable)[pendAddr][3] == arg) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                    F_;
                    paintArgResultFlag = true;
                }
                if ((*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[0]
                        == F_
                        && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[1]
                                == F_
                        && (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[2]
                                == F_) {
                    (*codeTable)[pendAddr].assemblyFormula.flagBit.changeable[3] =
                    F_;
                    paintArgResultFlag = true;
                } else {
                    paintArgResultFlag = false;
                }
            }
            if (paintArgResultFlag == true) {
                Arg argResult = (*codeTable)[pendAddr][3];
                Addr nextaddr = codeTable->getSameTrBranchNextArgAddr(pendAddr,
                        argResult);
                paint(argResult,
                        codeTable->getSameTrBranchNextArgAddr(pendAddr,
                                argResult));
            }

            paint(arg, codeTable->getSameTrBranchNextArgAddr(pendAddr, arg));

            return true;
        }

    }
    const bool paintAll() {
        ;
        while (scanner->pendM.size() != 0) {
            Addr a = scanner->pendM.back();
            scanner->pendM.pop_back();
            paint((*codeTable)[a][0], a);
            /**
             * 将#x和$x语句从返回运算值的函数体语句中去掉
             */
            (*codeTable)[a].assemblyFormula.flagBit.execute = Du;
        }
        scanner->pendM.clear();
        return true;

    }

};

typedef Arg ArgReal;
class RealPainter {
public:
    Scanner *scanner = NULL;
    CodeTable *codeTable = NULL;
    RealPainter(Scanner *s_) {
        scanner = s_;
        codeTable = scanner->codeTable;
    }
    const bool painted(const Arg &arg, const Addr &realAddr) {
        return ((*codeTable)[realAddr][0] != arg
                || ((*codeTable)[realAddr][0] == arg
                        && (*codeTable)[realAddr].assemblyFormula.flagBit.formalArgFlag[0]
                                == F_))
                && ((*codeTable)[realAddr][1] != arg
                        || ((*codeTable)[realAddr][1] == arg
                                && (*codeTable)[realAddr].assemblyFormula.flagBit.formalArgFlag[1]
                                        == F_))
                && ((*codeTable)[realAddr][2] != arg
                        || ((*codeTable)[realAddr][2] == arg
                                && (*codeTable)[realAddr].assemblyFormula.flagBit.formalArgFlag[2]
                                        == F_))
                && ((*codeTable)[realAddr][3] != arg
                        || ((*codeTable)[realAddr][3] == arg
                                && (*codeTable)[realAddr].assemblyFormula.flagBit.formalArgFlag[3]
                                        == F_));
    }
    bool paint(const Arg &arg, const Addr &realAddr) {
        if (realAddr == addrnull) {
            return false;
        }

        if (painted(arg, realAddr) == true || realAddr == addrnull) {
#if debug
            cout << 11.5 << endl;
#endif
            return true;
        } else {
            if ((*codeTable)[realAddr][0] == arg) {
                (*codeTable)[realAddr].assemblyFormula.flagBit.formalArgFlag[0] =
                F_;
            }
            if ((*codeTable)[realAddr][1] == arg) {
                (*codeTable)[realAddr].assemblyFormula.flagBit.formalArgFlag[1] =
                F_;
            }
            if ((*codeTable)[realAddr][2] == arg) {
                (*codeTable)[realAddr].assemblyFormula.flagBit.formalArgFlag[2] =
                F_;
            }
            if ((*codeTable)[realAddr][3] == arg) {
                (*codeTable)[realAddr].assemblyFormula.flagBit.formalArgFlag[3] =
                F_;
            }
            Addr nextaddr = codeTable->getSameTrBranchNextArgAddr(realAddr,
                    arg);
            if (nextaddr != addrnull) {

                paint(arg, nextaddr);
            }

            return true;
        }

    }
    const bool paintAll() {
        Intg s = scanner->realM.size();
        while (--s >= 0) {
            paint((*codeTable)[scanner->realM[s]][0], scanner->realM[s]);
            /**
             * 将out:x语句从返回运算值的函数体语句中去掉
             */
            (*codeTable)[scanner->realM[s]].assemblyFormula.flagBit.execute =
            Du;

        }
        scanner->realM.clear();
        return true;

    }

};

bool Scanner::loadAssemblyCode(const Strg &filePath) {
    {
#if debug
        static int ignorecount = 0;
        cout << "Scanner::loadAssemblyCode(const Strg&)" << " filePath:["
                << filePath << "]" << "\tignorecount:[" << ignorecount++
                << "\t](" << "vm_scan.hpp:" << __LINE__ << ":0" << ")" << endl;
#endif
    }
    fstream f(filePath.c_str());
    string line;
    AssemblyCode asc;
    Intg position = 0;
    while (getline(f, line)) {
#if debug
        cout << line << endl;
#endif
        asc = AssemblyCode(line);
        if (asc == ascnull) {
            continue;
        }
        if (asc.operatorFlag == Y) {
            auto &&it = assemblyCodeTable.acdq.end();
            ++it;
            while (it->type == M) {
                ++it;

            }
            --it;
            assemblyCodeTable.acdq.insert(it, asc);
            assemblyCodeTable.acpdq.push_back(position);
        } else {
            assemblyCodeTable.acdq.push_back(asc);
            assemblyCodeTable.acpdq.push_back(position);

        }

    }
    f.close();
    return true;
}
const bool Scanner::updateCurrentAssemblyCode() {

    if (currentCode.assemblyFormula.flagBit.type == M
            || currentCode.assemblyFormula.flagBit.type == R) {
        lastM = currentCodeAddr;
    }
    currentAssemblyCodePos++;
    currentCode.null();
    Intg size_ = assemblyCodeTable.acdq.size();
    if (currentAssemblyCodePos < size_) {
        currentAssemblyCode = assemblyCodeTable.acdq.at(
                currentAssemblyCodePos - 1);
        nextAssemblyCode = assemblyCodeTable.acdq.at(currentAssemblyCodePos);

    } else if (currentAssemblyCodePos == size_) {
        currentAssemblyCode = assemblyCodeTable.acdq.at(
                currentAssemblyCodePos - 1);
        nextAssemblyCode.null();

    } else {

        return false;
    }

    return true;
}

const bool Scanner::setArgScopeAddr() {
#if debug
    {
        static int ignorecount = 0;
        cout << "Scanner::setArgScopeAddr()" << " in " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    if (currentCode.assemblyFormula.flagBit.type == C
            && currentCode[0].argFlag == S_Bs && currentCode[1].argFlag == X_Bs) {
        currentCode.assemblyFormula.flagBit.a1s = currentCode.scopeAddr;
        currentCode.assemblyFormula.flagBit.res = currentCode.scopeAddr;

    } else if (currentCode.assemblyFormula.flagBit.type == M
            && currentCode.assemblyFormula.flagBit.execute == F_) {
        if ((currentCode.assemblyFormula.flagBit.result_flag == S_Dz)
                && currentCode.assemblyFormula.flagBit.formalArgFlag[3]
                        == true) {
            (*currentAR).add(currentCode[3]);
        }
#if debug
        cout << "Scanner::setArgScopeAddr a1s" << endl;
#endif
        if (currentCode[0].isVar()) {
            try {
                currentCode.assemblyFormula.flagBit.a1s =
                        currentAR->getArgScopeAddr(currentCode[0]);
            } catch (Arg &var_not_exist) {
                currentCode.assemblyFormula.flagBit.a1s = currentCode.scopeAddr;
            }
        }

#if debug
        cout << "Scanner::setArgScopeAddr a2s" << endl;
#endif
        if (currentCode[1].isVar()) {
            try {
                currentCode.assemblyFormula.flagBit.a2s =
                        currentAR->getArgScopeAddr(currentCode[1]);
            } catch (Arg &var_not_exist) {
                currentCode.assemblyFormula.flagBit.a2s = currentCode.scopeAddr;
            }
        }

#if debug
        cout << "Scanner::setArgScopeAddr ops" << endl;
#endif
        if (currentCode[2].isVar()) {
            try {
                currentCode.assemblyFormula.flagBit.ops =
                        currentAR->getArgScopeAddr(currentCode[2]);
            } catch (Arg &var_not_exist) {
                currentCode.assemblyFormula.flagBit.ops = currentCode.scopeAddr;
            }
        }
#if debug
        cout << "Scanner::setArgScopeAddr res" << endl;
#endif
        if (currentCode[3].isVar()) {
            try {
                currentCode.assemblyFormula.flagBit.res =
                        currentAR->getArgScopeAddr(currentCode[3]);
            } catch (Arg &var_not_exist) {
                currentCode.assemblyFormula.flagBit.res = currentCode.scopeAddr;
            }
        }
#if debug
        cout << "Scanner::setArgScopeAddr end" << endl;
#endif

    } else if (currentCode.assemblyFormula.flagBit.type == R
            && currentCode.assemblyFormula.flagBit.execute == T_) {
        currentCode.assemblyFormula.flagBit.a1s = currentAR->getArgScopeAddr(
                currentCode[0]);

    } else if (currentCode.assemblyFormula.flagBit.type == M
            && currentCode.assemblyFormula.flagBit.execute == T_) {
        if ((currentCode.assemblyFormula.flagBit.result_flag == S_Dz)
                && currentCode.assemblyFormula.flagBit.formalArgFlag[3]
                        == true) {
            (*currentAR).add(currentCode[3]);
        }
#if debug
        cout << "Scanner::setArgScopeAddr a1s2" << endl;
#endif
        if (currentCode[0].isVar()) {
            try {
                currentCode.assemblyFormula.flagBit.a1s =
                        currentAR->getArgScopeAddr(currentCode[0]);
            } catch (Arg &var_not_exist) {
                currentCode.assemblyFormula.flagBit.a1s = currentCode.scopeAddr;
            }
        }

#if debug
        cout << "Scanner::setArgScopeAddr a2s2" << endl;
#endif
        if (currentCode[1].isVar()) {
            try {
                currentCode.assemblyFormula.flagBit.a2s =
                        currentAR->getArgScopeAddr(currentCode[1]);
            } catch (Arg &var_not_exist) {
                currentCode.assemblyFormula.flagBit.a2s = currentCode.scopeAddr;
            }
        }

#if debug
        cout << "Scanner::setArgScopeAddr ops2" << endl;
#endif
        if (currentCode[2].isVar()) {
            try {
                currentCode.assemblyFormula.flagBit.ops =
                        currentAR->getArgScopeAddr(currentCode[2]);
            } catch (Arg &var_not_exist) {
                currentCode.assemblyFormula.flagBit.ops = currentCode.scopeAddr;
            }
        }
#if debug
        cout << "Scanner::setArgScopeAddr res2" << endl;
#endif
        if (currentCode[3].isVar()) {
            try {
                currentCode.assemblyFormula.flagBit.res =
                        currentAR->getArgScopeAddr(currentCode[3]);
            } catch (Arg &var_not_exist) {
                currentCode.assemblyFormula.flagBit.res = currentCode.scopeAddr;
            }
        }
#if debug
        cout << "Scanner::setArgScopeAddr end2" << endl;
#endif

    } else if (currentCode.assemblyFormula.flagBit.type == QS
            && currentCode.assemblyFormula.flagBit.execute == F_) {
        currentCode.assemblyFormula.flagBit.res =
                currentCode.assemblyFormula.quaternion.result_a;

    } else if (currentCode.assemblyFormula.flagBit.type == QE
            && currentCode.assemblyFormula.flagBit.execute == F_) {
        currentCode.assemblyFormula.flagBit.res =
                currentCode.assemblyFormula.quaternion.result_a;

    } else if (currentCode.assemblyFormula.flagBit.type == QS
            && currentCode.assemblyFormula.flagBit.execute == R) {
        currentCode.assemblyFormula.flagBit.res =
                currentCode.assemblyFormula.quaternion.result_a;

    } else if (currentCode.assemblyFormula.flagBit.type == QE
            && currentCode.assemblyFormula.flagBit.execute == R) {
        currentCode.assemblyFormula.flagBit.res =
                currentCode.assemblyFormula.quaternion.result_a;

    } else if (currentCode.assemblyFormula.flagBit.type == F) {
        currentCode.assemblyFormula.flagBit.a1s =
                currentCode.assemblyFormula.quaternion.arg1_a;
        currentCode.assemblyFormula.flagBit.a2s =
                currentCode.assemblyFormula.quaternion.arg2_a;
        currentCode.assemblyFormula.flagBit.res =
                currentCode.assemblyFormula.quaternion.result_a;
    } else if (currentCode.assemblyFormula.flagBit.type == TD) {
        currentCode.assemblyFormula.flagBit.a1s =
                currentCode.assemblyFormula.quaternion.arg1_a;
        currentCode.assemblyFormula.flagBit.a2s =
                currentCode.assemblyFormula.quaternion.arg2_a;
        currentCode.assemblyFormula.flagBit.res =
                currentCode.assemblyFormula.quaternion.result_a;
    }
#if debug
    {
        static int ignorecount = 0;
        cout << "Scanner::setArgScopeAddr()" << " out " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    return true;
}
const bool Scanner::addArgsToCurrentAR() {
#if debug
    {
        static int ignorecount = 0;
        cout << "Scanner::addArgsToCurrentAR()" << " in " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    if (currentCode.assemblyFormula.flagBit.type == C
            && currentCode[0].argFlag == S_Bs && currentCode[1].argFlag == X_Bs) {
        if (currentCode[1].arg_s == "new" || currentCode[1].arg_s == "out") {
            (*currentAR).add(currentCode[0]);
        } else {
//创建类的实例，直接使用类的AR
            (*currentAR).add(currentCode[0]);
            System &sys = (*systemTable)[pair<Strg, Addr>(
                    currentCode.assemblyFormula.quaternion.arg2_s,
                    currentCode.scopeAddr)];
            (*currentAR)[currentCode[0]] = (*currentAR)[Arg(S_Dz,
                    sys.systemAddr)];
        }

    } else if (currentCode.assemblyFormula.flagBit.type == M
            && currentCode.assemblyFormula.flagBit.execute != Du) {
        /**
         * 不用管引用，引用已经被置于M之前，已经被处理过了
         */

        Arg &&arg_1 = currentCode[0];
        Arg &&arg_2 = currentCode[1];
        Arg &&arg_op = currentCode[2];
        Arg &&arg_res = currentCode[3];
        if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
            (*currentAR).add(currentCode[0]);
        }
        if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
            (*currentAR).add(currentCode[1]);
        }
        if (arg_op.isVar() && arg_op.argFlag != Y_Dz) {
            (*currentAR).add(currentCode[2]);
        }
        if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
            (*currentAR).add(currentCode[3]);
        }
    } else if (currentCode.assemblyFormula.flagBit.type == Y
            && currentCode.assemblyFormula.flagBit.execute != Du) {
        Arg &&arg_1 = currentCode[0];
        Arg &&arg_2 = currentCode[1];
        Arg &&arg_op = currentCode[2];
        Arg &&arg_res = currentCode[3];
        //引用的arg1、arg2无意义，故忽略其添加
        /*if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
         (*currentAR).add(currentCode[0]);
         } else if (currentCode.assemblyFormula.flagBit.arg1_flag == Y_Dz) {

         arg_1.arg_ar = currentAR;
         (*currentAR).add(arg_1);
         (*currentAR)[arg_1].content_ar = currentAR;
         }
         if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
         (*currentAR).add(currentCode[1]);
         } else if (currentCode.assemblyFormula.flagBit.arg2_flag == Y_Dz) {

         arg_2.arg_ar = currentAR;
         (*currentAR).add(arg_2);
         (*currentAR)[arg_2].content_ar = currentAR;
         }
         if (currentCode.assemblyFormula.flagBit.operator_flag == S_Dz) {
         (*currentAR).add(currentCode[2]);
         } else if (currentCode.assemblyFormula.flagBit.operator_flag == Y_Dz) {
         globalRefTable.addRefData(this->currentAR, codeTable,
         currentCodeAddr, 2);

         arg_op.arg_ar = currentAR;
         (*currentAR).add(arg_op);
         (*currentAR)[arg_op].content_ar = currentAR;
         }*/
        if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
            (*currentAR).add(currentCode[3]);
        } else if (currentCode.assemblyFormula.flagBit.result_flag == Y_Dz) {
            globalRefTable.addRefData(this->currentAR, codeTable,
                    currentCodeAddr, 3);

            arg_res.arg_ar = currentAR;
            (*currentAR).add(arg_res);
            try {
                Data &resData = (*currentAR)[arg_res];
                resData.content_ar = currentAR;
            } catch (Arg &referencePath_include_immed) {
#if debug
                {
                    static int ignorecount = 0;
                    cout << "Scanner::addArgsToCurrentAR()"
                            << " referencePath_include_immed arg:["
                            << referencePath_include_immed.toStrg() << "]\t"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
            }
        }

    } else if (currentCode.assemblyFormula.flagBit.type == QS) {
        (*currentAR).add(currentCode[3]);
        (*currentAR)[currentCode.scopeAddr].dataFlagBit = S_AR;
        safe_delete((*currentAR)[currentCode.scopeAddr].content_ar);
        DataTable *ar = (*currentAR)[currentCode.scopeAddr].content_ar =
                new DataTable(currentAR, currentAR, currentCode.scopeAddr);
        addr_ar_map.insert(make_pair(currentCode.scopeAddr, ar));
        currentAR = (*currentAR)[currentCode.scopeAddr].content_ar;

    } else if (currentCode.assemblyFormula.flagBit.type == QE) {

        (*currentAR)[currentCode[3]].dataFlagBit = S_AR;

        currentAR = currentAR->parentAR;
        scopeStack.pop_back();
        currentScopeAddr = stackToAddr();
        currentScope = (*scopeTable)[currentScopeAddr];

    } else if (currentCode.assemblyFormula.flagBit.type == R
            && currentCode.assemblyFormula.flagBit.execute == R) {

    } else if (currentCode.assemblyFormula.flagBit.type == QS
            && currentCode.assemblyFormula.flagBit.execute == R) {
        //scan步骤无法进入此步,预执行步骤可进入此步
        if ((*scopeTable)[stackToAddr()].functionFlag == SYSB) {
            //预执行过程中的继承处理
            (*currentAR).add(currentCode[3]);
            (*currentAR)[currentCode.scopeAddr].dataFlagBit = S_AR;
            (*currentAR)[currentCode.scopeAddr].content_ar->scopeStructureAddr =
                    currentCode.scopeAddr;
            safe_delete((*currentAR)[currentCode.scopeAddr].content_ar);

            DataTable *ar =
                    (*currentAR)[currentCode.scopeAddr].content_ar =
                            new DataTable(currentAR,
                                    (*currentAR)[(*scopeTable)[currentCode.scopeAddr].parentScopeAddr].content_ar,
                                    currentCode.scopeAddr);
            addr_ar_map.insert(make_pair(currentCode.scopeAddr, ar));
            currentAR = (*currentAR)[currentCode.scopeAddr].content_ar;

            System &currentSystem = (*systemTable)[currentCode.scopeAddr];
            //将继承类的预执行实例直接添加到querylist中完成继承
            for (auto &id : currentSystem.decedentSystemIDList) {
                currentAR->queryARList.push_front(
                        (*currentAR->parentAR)[(*systemTable)[id].systemAddr].content_ar);
            }

        } else {
            //预执行过程中的函数调用等处理
            (*currentAR).add(currentCode[3]);
            (*currentAR)[currentCode.scopeAddr].dataFlagBit = S_AR;
            (*currentAR)[currentCode.scopeAddr].content_ar->scopeStructureAddr =
                    currentCode.scopeAddr;
            safe_delete((*currentAR)[currentCode.scopeAddr].content_ar);
            DataTable *queryAR_ = NULL;
            if ((*scopeTable)[currentCode.scopeAddr].requireScopeAddr
                    != addrnull) {
                queryAR_ =
                        (*currentAR)[(*scopeTable)[currentCode.scopeAddr].requireScopeAddr].content_ar;
            } else {
                queryAR_ = currentAR;
            }

            DataTable *ar = (*currentAR)[currentCode.scopeAddr].content_ar =
                    new DataTable(currentAR, queryAR_, currentCode.scopeAddr);
            addr_ar_map.insert(make_pair(currentCode.scopeAddr, ar));
            currentAR = (*currentAR)[currentCode.scopeAddr].content_ar;
        }

    } else if (currentCode.assemblyFormula.flagBit.type == QE
            && currentCode.assemblyFormula.flagBit.execute == R) {
        //scan步骤无法进入此步

        (*currentAR)[currentCode[3]].dataFlagBit = S_AR;

        currentAR = currentAR->parentAR;

    } else if (currentCode.assemblyFormula.flagBit.type == TD
            && currentCode.assemblyFormula.flagBit.execute == T_) {
        //scan过程中不处理TD
    }
#if debug
    {
        static int ignorecount = 0;
        cout << "Scanner::addArgsToCurrentAR()" << " out " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    return true;
}

//const AssemblyCode Scanner::getNewAssemblyCode();
Addr Scanner::stackToAddr() const {
    Addr a;
    for (int i = 0; i < (int) scopeStack.size(); i++) {
        a = link(a, scopeStack.at(i));

    }
    return a;
}
Addr stackToAddr(const vector<Addr> &scopeStack) {
    Addr a;
    for (int i = 0; i < (int) scopeStack.size(); i++) {
        a = link(a, scopeStack.at(i));

    }
    return a;
}
const Addr Scanner::getQSAddr(const Addr &addr) {
    return link(stackToAddr(), addr);
}
const Addr Scanner::getQSAddr(Intg addr) {
    return link(stackToAddr(), addr);
}

const Addr Scanner::nextSameScopeTfAddr(const Addr &parentScopeAddr,
        const Addr &codeAddr) const {
    Addr a = templateFunctionTable->nextTfAddr(codeAddr);
    Addr b = (*scopeTable)[a].parentScopeAddr;
    Addr null;
    if (a != null) {
        if (b == parentScopeAddr) {
            return a;
        } else if ((*scopeTable)[a].scopeCoor2
                > (*scopeTable)[parentScopeAddr].scopeCoor2
                && (*scopeTable)[parentScopeAddr].scopeCoor2 != null) {
            return null;

        } else {
            while ((*scopeTable)[b].parentScopeAddr != parentScopeAddr) {
                b = (*scopeTable)[b].parentScopeAddr;
            }
            return this->nextSameScopeTfAddr(parentScopeAddr,
                    (*scopeTable)[b].scopeCoor2);
        }

    }
    return a;
}
/**
 * @brief 查询一个变量的属性
 * @param varname
 * @param scopeAddr
 * @param out 是否从上层作用域开始检索
 * @return 变量的属性，如果变量不存在，则返回-1；
 */
Intg Scanner::getAttribute(const Strg &varname, const ScopeAddr &scopeAddr,
        bool out) {
    Addr indexScopeAddr = scopeAddr;
    if (out == true) {
        if (name_attr_map.find(indexScopeAddr) != name_attr_map.end()) {
            indexScopeAddr = (*scopeTable)[indexScopeAddr].parentScopeAddr;
        } else {
            return -1;
        }
    }
    do {
        auto &&it_name_attr_map = name_attr_map.find(indexScopeAddr);
        if (it_name_attr_map == name_attr_map.end()) {
            return -1;
        } else {
            auto &&it_attr = it_name_attr_map->second.find(varname);
            if (it_attr == it_name_attr_map->second.end()) {
                if (indexScopeAddr == addrnull) {
                    return -1;
                } else if (name_attr_map.find(indexScopeAddr)
                        != name_attr_map.end()) {
                    indexScopeAddr =
                            (*scopeTable)[indexScopeAddr].parentScopeAddr;
                } else {
                    return -1;
                }
            } else {
                return it_attr->second;
            }

        }

    } while (true);

}
Intg Scanner::getAttribute(const Addr &varaddr, const ScopeAddr &scopeAddr,
        bool out) {

    Addr indexScopeAddr = scopeAddr;
    if (out == true) {
        if (addr_attr_map.find(indexScopeAddr) != addr_attr_map.end()) {
            indexScopeAddr = (*scopeTable)[indexScopeAddr].parentScopeAddr;
        } else {
            return -1;
        }
    }
    do {
        auto &&it_addr_attr_map = addr_attr_map.find(indexScopeAddr);
        if (it_addr_attr_map == addr_attr_map.end()) {
            return -1;
        } else {
            auto &&it_attr = it_addr_attr_map->second.find(varaddr);
            if (it_attr == it_addr_attr_map->second.end()) {
                if (indexScopeAddr == addrnull) {
                    return -1;
                } else if (addr_attr_map.find(indexScopeAddr)
                        != addr_attr_map.end()) {
                    indexScopeAddr =
                            (*scopeTable)[indexScopeAddr].parentScopeAddr;
                } else {
                    return -1;
                }
            } else {
                return it_attr->second;
            }

        }

    } while (true);

}
/**
 * @brief 查询一个变量的信息，其储存在一个arg中
 * @param varname
 * @param scopeAddr
 * @param out 是否从上层作用域开始检索
 * @return 一个变量，其储存了变量所在scope（asc）、属性等，如果变量不存在，则返回argnull；
 */
Arg Scanner::getArg(const Strg &varname, const ScopeAddr &scopeAddr, bool out) {
    Addr indexScopeAddr = scopeAddr;
    if (out == true) {
        if (name_attr_map.find(indexScopeAddr) != name_attr_map.end()) {
            indexScopeAddr = (*scopeTable)[indexScopeAddr].parentScopeAddr;
        } else {
            return argnull;
        }
    }
    do {
        auto &&it_name_attr_map = name_attr_map.find(indexScopeAddr);
        if (it_name_attr_map == name_attr_map.end()) {
            return argnull;
        } else {
            auto &&it_attr = it_name_attr_map->second.find(varname);
            if (it_attr == it_name_attr_map->second.end()) {
                if (indexScopeAddr == addrnull) {
                    return argnull;
                } else if (name_attr_map.find(indexScopeAddr)
                        != name_attr_map.end()) {
                    indexScopeAddr =
                            (*scopeTable)[indexScopeAddr].parentScopeAddr;
                } else {
                    return argnull;
                }
            } else {
                Arg argInfo;
                argInfo.argFlag = it_attr->second;
                argInfo.arg_s = it_attr->first;
                argInfo.asc = indexScopeAddr;
                return argInfo;
            }

        }

    } while (true);

}
Arg Scanner::getArg(const Addr &varaddr, const ScopeAddr &scopeAddr, bool out) {

    Addr indexScopeAddr = scopeAddr;
    if (out == true) {
        if (addr_attr_map.find(indexScopeAddr) != addr_attr_map.end()) {
            indexScopeAddr = (*scopeTable)[indexScopeAddr].parentScopeAddr;
        } else {
            return argnull;
        }
    }
    do {
        auto &&it_addr_attr_map = addr_attr_map.find(indexScopeAddr);
        if (it_addr_attr_map == addr_attr_map.end()) {
            return argnull;
        } else {
            auto &&it_attr = it_addr_attr_map->second.find(varaddr);
            if (it_attr == it_addr_attr_map->second.end()) {
                if (indexScopeAddr == addrnull) {
                    return argnull;
                } else if (addr_attr_map.find(indexScopeAddr)
                        != addr_attr_map.end()) {
                    indexScopeAddr =
                            (*scopeTable)[indexScopeAddr].parentScopeAddr;
                } else {
                    return argnull;
                }
            } else {
                Arg argInfo;
                argInfo.argFlag = it_attr->second;
                argInfo.arg_a = it_attr->first;
                argInfo.asc = indexScopeAddr;
                return argInfo;
            }

        }

    } while (true);

}
/**
 * @brief 根据之前的变量重置一个变量的属性，若这个变量不存在且out为假，
 * 则在scopeAddr中创建一个新的变量
 * @param varname
 * @param scopeAddr 变量所在作用域
 * @param out 是否从上层作用域开始搜索
 * @return
 */
void Scanner::resetAttribute(const Strg &varname, const ScopeAddr &scopeAddr,
        const Intg &attr, bool out) {
#if debug
    {
        static int ignorecount = 0;
        cout
                << "resetAttribute(const Strg&, const ScopeAddr&, const int&, bool)"
                << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    bool varNotExist = false;
    Addr indexScopeAddr = scopeAddr;
    if (out == true) {
        if (name_attr_map.find(indexScopeAddr) != name_attr_map.end()) {
            indexScopeAddr = (*scopeTable)[indexScopeAddr].parentScopeAddr;
        } else {
            varNotExist = true;
        }
    }
    do {
        auto &&it_name_attr_map = name_attr_map.find(indexScopeAddr);
        if (it_name_attr_map == name_attr_map.end()) {
            varNotExist = true;
            break;
        } else {
            auto &&it_attr = it_name_attr_map->second.find(varname);
            if (it_attr == it_name_attr_map->second.end()) {
                if (indexScopeAddr == addrnull) {
                    varNotExist = true;
                    break;
                } else if (name_attr_map.find(indexScopeAddr)
                        != name_attr_map.end()) {
                    indexScopeAddr =
                            (*scopeTable)[indexScopeAddr].parentScopeAddr;
                } else {
                    varNotExist = true;
                    break;
                }
            } else {
                name_attr_map[indexScopeAddr][varname] = attr;
                return;
            }

        }

    } while (true);
    if (out == false && varNotExist == true) {
        name_attr_map[scopeAddr][varname] = attr;
    }

}
void Scanner::resetAttribute(const Addr &varaddr, const ScopeAddr &scopeAddr,
        const Intg &attr, bool out) {
#if debug
    {
        static int ignorecount = 0;
        cout
                << "resetAttribute(const Addr&, const ScopeAddr&, const int&, bool)"
                << " in " << "varaddr:[" << varaddr.toStrg() << "]\tscopeAddr:["
                << scopeAddr.toStrg() << "]\tattr:[" << attr << "]\tout:["
                << out << "]" << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    bool varNotExist = false;
    Addr indexScopeAddr = scopeAddr;
    if (out == true) {
        if (addr_attr_map.find(indexScopeAddr) != addr_attr_map.end()) {
            indexScopeAddr = (*scopeTable)[indexScopeAddr].parentScopeAddr;
        } else {
            varNotExist = true;
        }
    }
    do {
        auto &&it_addr_attr_map = addr_attr_map.find(indexScopeAddr);
        if (it_addr_attr_map == addr_attr_map.end()) {
            varNotExist = true;
            break;
        } else {
            auto &&it_attr = it_addr_attr_map->second.find(varaddr);
            if (it_attr == it_addr_attr_map->second.end()) {
                if (indexScopeAddr == addrnull) {
                    varNotExist = true;
                    break;
                } else if (indexScopeAddr != addrnull
                        && addr_attr_map.find(indexScopeAddr)
                                != addr_attr_map.end()) {
                    indexScopeAddr =
                            (*scopeTable)[indexScopeAddr].parentScopeAddr;
                } else {
                    varNotExist = true;
                    break;
                }
            } else {
                addr_attr_map[indexScopeAddr][varaddr] = attr;
                return;
            }

        }

    } while (true);
    if (out == false && varNotExist == true) {
        addr_attr_map[scopeAddr][varaddr] = attr;
    }

}
/**
 * @brief 向作用域中添加一个变量属性的索引
 * @param varname
 * @param scopeAddr
 */
void Scanner::addAttribute(const Strg &varname, const ScopeAddr &scopeAddr,
        const Intg &attr) {
    addr_attr_map[scopeAddr][varname] = attr;

}
void Scanner::addAttribute(const Addr &varaddr, const ScopeAddr &scopeAddr,
        const Intg &attr) {
    addr_attr_map[scopeAddr][varaddr] = attr;
}
//const Addr Scanner::getScopeAddr(const Addr& addr) const;
//const Addr Scanner::getScopeAddr(Intg addr) const;
/**
 * @attention 在这个函数中，所有域地址（Z_Dz）、函数地址都会被转化为绝对地址，
 * 所有变量地址（S_Dz）都仍为相对地址
 * @param asc
 * @param asf
 * @return
 */
bool Scanner::setQuaternionAndFlagBit(const AssemblyCode &asc,
        AssemblyFormula *asf) {
    asf->flagBit.type = asc.type;

    if (asc.type == QS) {
        asf->flagBit.result_flag = Z_Dz;
        asf->quaternion.result_a = stackToAddr();
    } else if (asc.type == C && asc.arg2Flag == Bs && asc.arg2_s == "new") {
        switch (asc.arg1Flag) {
        case Dz:
            asf->flagBit.arg1_flag = S_Dz;
            asf->quaternion.arg1_a = asc.arg1_i;
            break;
        case Sz:
            asf->flagBit.arg1_flag = Sz;
            asf->quaternion.arg1_i = asc.arg1_i;
            break;
        case Bs:
            asf->flagBit.arg1_flag = S_Bs;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;
        case Zf:
            asf->flagBit.arg1_flag = Zf;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;

        default:
            break;
        }
        switch (asc.arg2Flag) {
        case Dz:
            asf->flagBit.arg2_flag = S_Dz;
            asf->quaternion.arg2_a = asc.arg2_i;
            break;
        case Sz:
            asf->flagBit.arg2_flag = Sz;
            asf->quaternion.arg2_i = asc.arg2_i;
            break;
        case Bs:
            asf->flagBit.arg2_flag = X_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;
        case Zf:
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;

        default:
            break;
        }
        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.operator_flag = M_Bs;
        switch (asc.resultFlag) {
        case Dz:
            asf->flagBit.result_flag = S_Dz;
            asf->quaternion.result_a = asc.result_i;
            break;
        case Sz:
            asf->flagBit.result_flag = Sz;
            asf->quaternion.result_i = asc.result_i;
            break;
        case Bs:
            asf->flagBit.result_flag = S_Bs;
            asf->quaternion.result_s = asc.result_s;
            break;
        case Zf:
            asf->flagBit.result_flag = Zf;
            asf->quaternion.result_s = asc.result_s;
            break;

        default:
            break;
        }
        if (asc.arg1_s != asc.result_s) {
#if debug
            cerr << "C (a1,x,x,result) err, should be a1==result" << endl;
#endif
        }
    } else if (asc.type == C && asc.arg2Flag == Bs && asc.arg2_s != "new") {
        asf->flagBit.arg1_flag = S_Bs;
        asf->quaternion.arg1_s = asc.arg1_s;
        asf->flagBit.arg2_flag = X_Bs;
        asf->quaternion.arg2_s = asc.arg2_s;
        asf->flagBit.operator_flag = M_Bs;
        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.result_flag = S_Bs;
        asf->quaternion.result_s = asc.result_s;

    } else if (asc.type == R
            || (asc.type == M && asc.operatorFlag == Bs
                    && (asc.operator_s == "+" || asc.operator_s == "-"
                            || asc.operator_s == "*" || asc.operator_s == "/"
                            || asc.operator_s == "^" || asc.operator_s == "=="
                            || asc.operator_s == "=" || asc.operator_s == "-->"
                            || asc.operator_s == ">" || asc.operator_s == "<"
                            || asc.operator_s == "COMMA"))) {
        switch (asc.arg1Flag) {
        case Dz:
            asf->flagBit.arg1_flag = S_Dz;
            asf->quaternion.arg1_a = asc.arg1_i;
            break;
        case Sz:
            asf->flagBit.arg1_flag = Sz;
            asf->quaternion.arg1_i = asc.arg1_i;
            break;
        case Bs:
            asf->flagBit.arg1_flag = S_Bs;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;
        case Zf:
            asf->flagBit.arg1_flag = Zf;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;

        default:
            break;
        }
        switch (asc.arg2Flag) {
        case Dz:
            asf->flagBit.arg2_flag = S_Dz;
            asf->quaternion.arg2_a = asc.arg2_i;
            break;
        case Sz:
            asf->flagBit.arg2_flag = Sz;
            asf->quaternion.arg2_i = asc.arg2_i;
            break;
        case Bs:
            asf->flagBit.arg2_flag = S_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;
        case Zf:
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;

        default:
            break;
        }
        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.operator_flag = M_Bs;
        switch (asc.resultFlag) {
        case Dz:
            asf->flagBit.result_flag = S_Dz;
            asf->quaternion.result_a = asc.result_i;
            break;
        case Sz:
            asf->flagBit.result_flag = Sz;
            asf->quaternion.result_i = asc.result_i;
            break;
        case Bs:
            asf->flagBit.result_flag = S_Bs;
            asf->quaternion.result_s = asc.result_s;
            break;
        case Zf:
            asf->flagBit.result_flag = Zf;
            asf->quaternion.result_s = asc.result_s;
            break;

        default:
            break;
        }

        Intg attr = -1;
        if (asc.arg1Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg1_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;

        } else if (asc.arg1Flag == Bs
                && (attr = getAttribute(asf->quaternion.arg1_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;

        }
        if (asc.arg2Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg2_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg2_flag = attr;

        } else if (asc.arg2Flag == Bs
                && (attr = getAttribute(asf->quaternion.arg2_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg2_flag = attr;

        }
        if (asc.resultFlag == Dz
                && (attr = getAttribute(asf->quaternion.result_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.result_flag = attr;

        } else if (asc.resultFlag == Bs
                && (attr = getAttribute(asf->quaternion.result_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.result_flag = attr;

        }

    } else if (asc.type == Y && asc.operatorFlag == Bs
            && asc.operator_s == ".") {
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "Scanner::setQuaternionAndFlagBit(const AssemblyCode&, AssemblyFormula*)"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        asf->quaternion.result_a = link(stackToAddr(), asc.result_i);
        asf->flagBit.result_flag = Y_Dz;
        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.operator_flag = M_Bs;
        Intg attr = -1;
        Addr arg2scopeAddr;
        if (asc.arg1Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg1_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;
            asf->quaternion.arg1_a = asc.arg1_i;
            if (asf->flagBit.arg1_flag == Y_Dz) {
                arg2scopeAddr = globalRefTable[Arg(Y_Dz, asf->quaternion.arg1_a,
                        currentAR)].asc;

            } else {
                arg2scopeAddr = (*currentAR).getArgScopeAddr(
                        Arg(S_Dz, asf->quaternion.arg1_a));
            }

        } else if (asc.arg1Flag == Bs) {
            asf->flagBit.arg1_flag = S_Bs;
            asf->quaternion.arg1_s = asc.arg1_s;
            /*
             * 以下代码无效，尝试通过函数名字检索函数是徒劳的，因为函数以S_AR形式储存在
             * AR中，意味着AR中是没有函数名字的键的
             DataTable *arg1AR = (*currentAR)[asf->quaternion.arg1_s].content_ar;
             arg2scopeAddr = arg1AR->scopeStructureAddr;
             */
        }

        if (asc.arg2Flag == Dz) {
            asf->flagBit.arg2_flag = S_Dz;
            asf->quaternion.arg2_a = asc.arg2_i;

        } else if (asc.arg2Flag == Bs) {
            asf->flagBit.arg2_flag = S_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
        } else if (asc.arg2Flag == Zf) {
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
        }

    } else if (asc.type == M && asc.operatorFlag == Bs
            && ((asc.operator_s == "$"
                    || (asc.arg2Flag == Bs && asc.operator_s == ":"
                            && asc.arg2_s == "$"))
                    || (asc.operator_s == "#"
                            || (asc.arg2Flag == Bs && asc.operator_s == ":"
                                    && asc.arg2_s == "#")))) {

        switch (asc.arg1Flag) {
        case Dz:
            asf->flagBit.arg1_flag = S_Dz;
            asf->quaternion.arg1_a = asc.arg1_i;
            break;
        case Sz:
#if debug
            cerr << "M (Sz,x,$#,x) err,should be Dz" << endl;
#endif
            asf->flagBit.arg1_flag = Sz;
            asf->quaternion.arg1_i = asc.arg1_i;
            break;
        case Bs:
            asf->flagBit.arg1_flag = S_Bs;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;
        case Zf:
#if debug
            cerr << "M (Zf,x,$#,x) err,should be Dz/Bs" << endl;
#endif
            asf->flagBit.arg1_flag = Zf;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;

        default:
            break;
        }
        switch (asc.arg2Flag) {
        case Dz:
            asf->flagBit.arg2_flag = X_Dz;
            asf->quaternion.arg2_a = asc.arg2_i;
            break;
        case Sz:
            asf->flagBit.arg2_flag = Sz;
            asf->quaternion.arg2_i = asc.arg2_i;
            break;
        case Bs:
            asf->flagBit.arg2_flag = X_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;
        case Zf:
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;

        default:
            break;
        }
        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.operator_flag = M_Bs;
        switch (asc.resultFlag) {
        case Dz:
            asf->flagBit.result_flag = S_Dz;
            asf->quaternion.result_a = asc.result_i;
            break;
        case Sz:
#if debug
            cerr << "M (x,x,$#,Sz) err,should be Dz" << endl;
#endif
            asf->flagBit.result_flag = Sz;
            asf->quaternion.result_i = asc.result_i;
            break;
        case Bs:
            asf->flagBit.result_flag = S_Bs;
            asf->quaternion.result_s = asc.result_s;
            break;
        case Zf:
#if debug
            cerr << "M (x,x,$#,Zf) err,should be Dz/Bs" << endl;
#endif
            asf->flagBit.result_flag = Zf;
            asf->quaternion.result_s = asc.result_s;
            break;

        default:
            break;
        }
        Intg attr = -1;
        if (asc.arg1Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg1_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;

        } else if (asc.arg1Flag == Bs
                && (attr = getAttribute(asf->quaternion.arg1_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;

        }
        if (asc.arg2Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg2_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg2_flag = attr;

        } else if (asc.arg2Flag == Bs
                && (attr = getAttribute(asf->quaternion.arg2_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg2_flag = attr;

        }
        if (asc.resultFlag == Dz
                && (attr = getAttribute(asf->quaternion.result_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.result_flag = attr;

        } else if (asc.resultFlag == Bs
                && (attr = getAttribute(asf->quaternion.result_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.result_flag = attr;

        }
        if (!(asc.arg1Flag == asc.resultFlag && asc.arg1_i == asc.result_i
                && asc.arg1_s == asc.result_s)) {
#if debug
            cerr << "M (arg1,x,$#,result) err,should be arg1==result" << endl;
#endif
        }
        if (asc.operator_s == "$"
                || (asc.arg2Flag == Bs && asc.operator_s == ":"
                        && asc.arg2_s == "$")) {
            asf->flagBit.changeable[0] = T_;
            /*            asf->flagBit.changeable[3] = T_;
             * 虽然此步可以确定achangeable3的可变性，但由于pendpainter会跳过可变性完全确定的代码，因此不能在此处直接给changeable3赋值
             *
             * */
        }
        if (asc.operator_s == "#"
                || (asc.arg2Flag == Bs && asc.operator_s == ":"
                        && asc.arg2_s == "#")) {
            asf->flagBit.changeable[0] = A_;
            /*            asf->flagBit.changeable[3] = A_;
             * 虽然此步可以确定achangeable3的可变性，但由于pendpainter会跳过可变性完全确定的代码，因此不能在此处直接给changeable3赋值
             * */
        }
        pendM.push_back(currentCodeAddr);

    } else if (asc.type == M && asc.operatorFlag == Bs && asc.operator_s == ":"
            && asc.arg2Flag == Bs && asc.arg2_s == "out") {

        asf->flagBit.execute = Du;
        realM.push_back(currentCodeAddr);

        switch (asc.arg1Flag) {
        case Dz:
            asf->flagBit.arg1_flag = S_Dz;
            asf->quaternion.arg1_a = asc.arg1_i;
            break;
        case Sz:
            asf->flagBit.arg1_flag = Sz;
            asf->quaternion.arg1_i = asc.arg1_i;
            break;
        case Bs:
            asf->flagBit.arg1_flag = S_Bs;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;
        case Zf:

            asf->flagBit.arg1_flag = Zf;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;

        default:
            break;
        }
        switch (asc.arg2Flag) {
        case Dz:
            asf->flagBit.arg2_flag = X_Dz;
            asf->quaternion.arg2_a = asc.arg2_i;
            break;
        case Sz:
            asf->flagBit.arg2_flag = Sz;
            asf->quaternion.arg2_i = asc.arg2_i;
            break;
        case Bs:
            asf->flagBit.arg2_flag = X_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;
        case Zf:
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;

        default:
            break;
        }
        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.operator_flag = M_Bs;
        switch (asc.resultFlag) {
        case Dz:
            asf->flagBit.result_flag = S_Dz;
            asf->quaternion.result_a = asc.result_i;
            break;
        case Sz:
            asf->flagBit.result_flag = Sz;
            asf->quaternion.result_i = asc.result_i;
            break;
        case Bs:
            asf->flagBit.result_flag = S_Bs;
            asf->quaternion.result_s = asc.result_s;
            break;
        case Zf:
            asf->flagBit.result_flag = Zf;
            asf->quaternion.result_s = asc.result_s;
            break;

        default:
            break;
        }
        Intg attr = -1;
        if (asc.arg1Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg1_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;

        } else if (asc.arg1Flag == Bs
                && (attr = getAttribute(asf->quaternion.arg1_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;

        }
        if (asc.arg2Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg2_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg2_flag = attr;

        } else if (asc.arg2Flag == Bs
                && (attr = getAttribute(asf->quaternion.arg2_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg2_flag = attr;

        }
        if (asc.resultFlag == Dz
                && (attr = getAttribute(asf->quaternion.result_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.result_flag = attr;

        } else if (asc.resultFlag == Bs
                && (attr = getAttribute(asf->quaternion.result_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.result_flag = attr;

        }
        if (!(asc.arg1Flag == asc.resultFlag && asc.arg1_i == asc.result_i
                && asc.arg1_s != asc.result_s)) {
#if debug
            cerr << "M (arg1,out,:,result) err,should be arg1==result" << endl;
#endif
        }
        realM.push_back(currentCodeAddr);

    } else if (asc.type == M && asc.operatorFlag == Bs
            && (asc.operator_s == "[]" || asc.operator_s == "ca"
                    || asc.operator_s == "call")) {
        switch (asc.arg1Flag) {
        case Dz:
            asf->flagBit.arg1_flag = S_Dz;
            asf->quaternion.arg1_a = asc.arg1_i;
            break;
        case Sz:
            asf->flagBit.arg1_flag = Sz;
            asf->quaternion.arg1_i = asc.arg1_i;
            break;
        case Bs:
            asf->flagBit.arg1_flag = S_Bs;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;
        case Zf:
            asf->flagBit.arg1_flag = Zf;
            asf->quaternion.arg1_s = asc.arg1_s;
            break;

        default:
            break;
        }
        switch (asc.arg2Flag) {
        case Dz:
            asf->flagBit.arg2_flag = S_Dz;
            asf->quaternion.arg2_a = asc.arg2_i;
            break;
        case Sz:
            asf->flagBit.arg2_flag = Sz;
            asf->quaternion.arg2_i = asc.arg2_i;
            break;
        case Bs:
            asf->flagBit.arg2_flag = S_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;
        case Zf:
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;

        default:
            break;
        }
        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.operator_flag = M_Bs;
        switch (asc.resultFlag) {
        case Dz:
            asf->flagBit.result_flag = S_Dz;
            asf->quaternion.result_a = asc.result_i;
            break;
        case Sz:
            asf->flagBit.result_flag = Sz;
            asf->quaternion.result_i = asc.result_i;
            break;
        case Bs:
            asf->flagBit.result_flag = S_Bs;
            asf->quaternion.result_s = asc.result_s;
            break;
        case Zf:
            asf->flagBit.result_flag = Zf;
            asf->quaternion.result_s = asc.result_s;
            break;

        default:
            break;
        }
        Intg attr = -1;
        if (asc.arg1Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg1_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;

        } else if (asc.arg1Flag == Bs
                && (attr = getAttribute(asf->quaternion.arg1_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg1_flag = attr;

        }
        if (asc.arg2Flag == Dz
                && (attr = getAttribute(asf->quaternion.arg2_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg2_flag = attr;

        } else if (asc.arg2Flag == Bs
                && (attr = getAttribute(asf->quaternion.arg2_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.arg2_flag = attr;

        }
        if (asc.resultFlag == Dz
                && (attr = getAttribute(asf->quaternion.result_a, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.result_flag = attr;

        } else if (asc.resultFlag == Bs
                && (attr = getAttribute(asf->quaternion.result_s, stackToAddr(),
                        false)) != -1) {
            asf->flagBit.result_flag = attr;

        }
    } else if (asc.type == ME) {

        asf->flagBit.type = ME;

    } else if (asc.type == QE) {
        asf->flagBit.type = QE;
        asf->flagBit.result_flag = Z_Dz;

        asf->quaternion.result_a = stackToAddr();
    } else if (((asc.type == F && asc.operator_s == "@")
            || (asc.type == TD && asc.operator_s == "=>"))
            && asc.operatorFlag == Bs) {
        switch (asc.arg1Flag) {
        case Dz:
            asf->flagBit.arg1_flag = Z_Dz;
            asf->quaternion.arg1_a = link(currentScopeAddr, asc.arg1_i);
            break;
        case Sz:
#if debug
            cerr << "F TD (Sz,x,x,x) err(should be Dz)" << endl;
#endif
            break;
        case Bs:
#if debug
            cerr << "F TD(Bs,x,x,x) err(should be Dz)" << endl;
#endif
            break;
        case Zf:
#if debug
            cerr << "F TD(Zf,x,x,x) err(should be Dz)" << endl;
#endif
            break;

        default:
#if debug
            cerr << "F TD(arg1Flag,x,x,x) err(should be Dz)" << endl;
#endif
            break;
        }
        switch (asc.arg2Flag) {
        case Dz:
            asf->flagBit.arg2_flag = Z_Dz;
            asf->quaternion.arg2_a = link(currentScopeAddr, asc.arg2_i);
            break;
        case Sz:
            asf->flagBit.arg2_flag = Sz;
            asf->quaternion.arg2_i = asc.arg2_i;
            break;
        case Bs:
            asf->flagBit.arg2_flag = S_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
#if debug
            clog << "F TD(x,Bs,x,x) warn(should be Sz)" << endl;
#endif
            break;
        case Zf:
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
#if debug
            cerr << "F TD(x,Zf,x,x) err(should be Sz)" << endl;
#endif
            break;

        default:
            break;
        }
        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.operator_flag = M_Bs;
        switch (asc.resultFlag) {
        case Dz:
            asf->flagBit.result_flag = Z_Dz;
            asf->quaternion.result_a = link(currentScopeAddr, asc.result_i);
            break;
        case Sz:
            asf->flagBit.result_flag = Sz;
            asf->quaternion.result_i = asc.result_i;
            break;
        case Bs:
            asf->flagBit.result_flag = S_Bs;
            asf->quaternion.result_s = asc.result_s;
            break;
        case Zf:
            asf->flagBit.result_flag = Zf;
            asf->quaternion.result_s = asc.result_s;
            break;

        default:
            break;
        }
        if (asc.resultFlag != asc.arg1Flag || asc.result_i != asc.arg1_i) {
#if debug
            cerr << "F TD(arg1,x,x,result) err(arg1 not equal result)" << endl;
#endif
        }
        asf->quaternion.arg1_a = link(currentScopeAddr, asc.arg1_i);
        asf->quaternion.result_a = asf->quaternion.arg1_a;

    } else if (asc.type == F && asc.operator_s == ":" && asc.operatorFlag == Bs
            && asc.arg2Flag == Bs && asc.arg2_s == "e") {
        if (asc.resultFlag != asc.arg1Flag || asc.result_i != asc.arg1_i) {
#if debug
            cerr << "F (arg1,x,x,result) err(arg1 not equal result)" << endl;
#endif
        }
        asf->flagBit.type = F;
        asf->flagBit.arg1_flag = Z_Dz;
        asf->quaternion.arg1_a = link(currentScopeAddr, asc.arg1_i);
        asf->flagBit.arg2_flag = X_Bs;
        asf->quaternion.arg2_s = "e";
        asf->flagBit.operator_flag = M_Bs;
        asf->quaternion.operator_s = ":";
        asf->flagBit.result_flag = Z_Dz;
        asf->quaternion.result_a = asf->quaternion.arg1_a;

    } else if (asc.type == F && asc.operator_s == ":" && asc.operatorFlag == Bs
            && asc.arg2Flag == Bs && asc.arg2_s == "!") {
        if (asc.resultFlag != asc.arg1Flag || asc.result_i != asc.arg1_i) {
#if debug
            cerr << "F (arg1,x,x,result) err(arg1 not equal result)" << endl;
#endif
        }
        asf->flagBit.type = F;
        asf->flagBit.arg1_flag = Z_Dz;
        asf->quaternion.arg1_a = link(currentScopeAddr, asc.arg1_i);
        asf->flagBit.arg2_flag = X_Bs;
        asf->quaternion.arg2_s = "!";
        asf->flagBit.operator_flag = M_Bs;
        asf->quaternion.operator_s = ":";
        asf->flagBit.result_flag = Z_Dz;
        asf->quaternion.result_a = asf->quaternion.arg1_a;
        //backward function name
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "Scanner::setQuaternionAndFlagBit(const AssemblyCode&, AssemblyFormula*)"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        Addr bfnScopeAddr = link(stackToAddr(), currentAssemblyCode.arg1_i);

        (*templateFunctionTable)[bfnScopeAddr].isBackwardFunction = true;

        Addr &bfnHandleAddr =
                (*templateFunctionTable)[bfnScopeAddr].templateHandleAddr;
        Code &bfnHandle = (*codeTable)[bfnHandleAddr];

    } else if (asc.type == F && asc.operator_s == "" && asc.operator_i == 0
            && asc.arg1Flag == Dz && asc.arg2Flag == Dz && asc.resultFlag == Dz
            && asc.arg1_i == asc.result_i && asc.arg1_i != asc.arg2_i) {
        asf->flagBit.type = F;
        asf->flagBit.arg1_flag = Z_Dz;
        Addr a = link(currentScopeAddr, asc.arg1_i);
        while ((*scopeTable)[a].trueAddr != addrnull) {
            a = (*scopeTable)[a].trueAddr;
        }
        asf->quaternion.arg1_a = a;
        asf->flagBit.arg2_flag = Z_Dz;
        asf->quaternion.arg2_a = link(currentScopeAddr, asc.arg2_i);
        asf->flagBit.operator_flag = 0;
        asf->flagBit.result_flag = Z_Dz;
        asf->quaternion.result_a = a;
    } else if (asc.type == B) {
        asf->flagBit.type = B;
        switch (asc.arg1Flag) {
        case Dz:
            asf->flagBit.arg1_flag = S_Dz;
            asf->quaternion.arg1_a = link(currentScopeAddr, asc.arg1_i);
            break;
        case Sz:
            asf->flagBit.arg1_flag = Sz;
            asf->quaternion.arg1_i = asc.arg1_i;
            break;
        default:
            break;
        }
        switch (asc.arg2Flag) {
        case Dz:
            asf->flagBit.arg2_flag = S_Dz;
            asf->quaternion.arg2_a = link(currentScopeAddr, asc.arg2_i);
            break;
        case Sz:
            asf->flagBit.arg2_flag = Sz;
            asf->quaternion.arg2_i = asc.arg2_i;
            break;
        case Bs:
            asf->flagBit.arg2_flag = S_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;
        case Zf:
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;

        default:
            break;
        }
        asf->flagBit.operator_flag = Z_Dz;
        asf->quaternion.operator_a = link(currentScopeAddr, asc.operator_i);
        asf->flagBit.result_flag = S_Dz;
        asf->quaternion.result_a = link(currentScopeAddr, asc.result_i);
    } else if (asc.type == L) {
        asf->flagBit.type = L;

        asf->flagBit.arg1_flag = S_Dz;
        asf->quaternion.arg1_a = link(currentScopeAddr, asc.arg1_i);

        switch (asc.arg2Flag) {
        case Dz:
            asf->flagBit.arg2_flag = S_Dz;
            asf->quaternion.arg2_a = link(currentScopeAddr, asc.arg2_i);
            break;
        case Sz:
            asf->flagBit.arg2_flag = Sz;
            asf->quaternion.arg2_i = asc.arg2_i;
            break;
        case Bs:
            asf->flagBit.arg2_flag = S_Bs;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;
        case Zf:
            asf->flagBit.arg2_flag = Zf;
            asf->quaternion.arg2_s = asc.arg2_s;
            break;

        default:
            break;
        }
        asf->flagBit.operator_flag = Z_Dz;
        asf->quaternion.operator_a = link(currentScopeAddr, asc.operator_i);

    } else if (asc.type == S && asc.operatorFlag == Bs
            && asc.operator_s == ":") {
        //(systemName,system,:,systemName)

        asf->flagBit.arg1_flag = X_Bs;
        asf->quaternion.arg1_s = asc.arg1_s;

        asf->flagBit.arg2_flag = X_Bs;
        asf->quaternion.arg2_s = asc.arg2_s;

        asf->quaternion.operator_s = asc.operator_s;
        asf->flagBit.operator_flag = M_Bs;

        asf->flagBit.result_flag = X_Bs;
        asf->quaternion.result_s = asc.result_s;

    } else if (asc.type == S && asc.arg1Flag == Bs && asc.arg2Flag == Dz
            && asc.operator_s == "") {
        //(systemName,systemScopeAddr,,systemName)

        asf->flagBit.arg1_flag = X_Bs;
        asf->quaternion.arg1_s = asc.arg1_s;

        asf->flagBit.arg2_flag = Z_Dz;
        asf->quaternion.arg2_a = link(stackToAddr(), asc.arg2_i);

        asf->flagBit.result_flag = X_Bs;
        asf->quaternion.result_s = asc.result_s;

    }

//如果变量属性在argAttributeTable中可查，则以argAttributeTable中内容为准

    return true;
}
const bool Scanner::scan() {

    while (updateCurrentAssemblyCode() == true) {
        {
#if debug
            static int ignorecount = 0;
            cout << "Scanner::scan()" << "currentAssemblyCode:["
                    << currentAssemblyCode.toStrg() << "\t]"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "vm_scan.hpp:" << __LINE__ << ":0" << ")" << endl;
#endif
        }
        if (currentAssemblyCode.type == QS) {
            currentScope.parentScopeAddr = stackToAddr();
            scopeStack.push_back(currentAssemblyCode.result_i);
            currentScopeAddr = stackToAddr();
            currentCodeAddr = getQSAddr(currentAssemblyCodePos);
            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);

            currentCode.assemblyFormula.flagBit.execute = T_;

            codeTable->addCode(currentCodeAddr, currentCode);
#if debug
            cout << "p 2" << endl;
#endif
            currentScope.scopeCoor1 = currentCodeAddr;

            scopeTable->newScopeStart(currentScopeAddr, currentScope);
        } else if (currentAssemblyCode.type == Y) {
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);
            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            currentCode.assemblyFormula.flagBit.execute = T_;
            codeTable->addCode(currentCodeAddr, currentCode);
            globalRefTable.addRefData(currentAR, codeTable, currentCodeAddr, 3);
        } else if (currentAssemblyCode.type == M) {
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            if (currentScopeAddr != addrnull) {
                /*                if (currentCode.assemblyFormula.flagBit.arg1_flag == S_Bs
                 || currentCode.assemblyFormula.flagBit.arg1_flag == S_Dz) {
                 currentCode.assemblyFormula.flagBit.changeable[0] = A_;

                 }
                 if (currentCode.assemblyFormula.flagBit.arg2_flag == S_Bs
                 || currentCode.assemblyFormula.flagBit.arg2_flag == S_Dz) {
                 currentCode.assemblyFormula.flagBit.changeable[1] = A_;

                 }
                 if (currentCode.assemblyFormula.flagBit.operator_flag == S_Bs
                 || currentCode.assemblyFormula.flagBit.operator_flag
                 == S_Dz) {
                 currentCode.assemblyFormula.flagBit.changeable[2] = A_;

                 }
                 if (currentCode.assemblyFormula.flagBit.result_flag == S_Bs
                 || currentCode.assemblyFormula.flagBit.result_flag
                 == S_Dz) {
                 currentCode.assemblyFormula.flagBit.changeable[3] = A_;

                 }*/
            }
            codeTable->addCode(currentCodeAddr, currentCode);
            if (nextAssemblyCode.type != M && nextAssemblyCode.type != R) {
#if debug
                {
                    static int ignorecount = 0;
                    cout << "Scanner::scan()" << " " << "\tignorecount:["
                            << ignorecount++ << "\t]("
                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
                if (pendM.size() != 0) {
                    PendPainter pp(this);
#if debug
                    {
                        static int ignorecount = 0;
                        cout << "Scanner::scan()" << " " << "\tignorecount:["
                                << ignorecount++ << "\t]("
                                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                                << __LINE__ << ":0" << ")" << endl;
                    }
#endif
                    pp.paintAll();
                    pendM.clear();
                }
#if debug
                {
                    static int ignorecount = 0;
                    cout << "Scanner::scan()" << " " << "\tignorecount:["
                            << ignorecount++ << "\t]("
                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
                if (realM.size() != 0) {
                    RealPainter rp(this);
                    rp.paintAll();
                    realM.clear();
                }

            }

        } else if (currentAssemblyCode.type == ME) {
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);

        } else if (currentAssemblyCode.type == C) {
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);

        } else if (currentAssemblyCode.type == QE) {
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);
            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);

            currentScope = (*scopeTable)[currentScopeAddr];
            currentScope.scopeCoor2 = currentCodeAddr;
            if (currentScope.parentScopeAddr
                    != addrnull&&currentScope.functionFlag == RFB
                    && (*scopeTable)[currentScope.parentScopeAddr].functionFlag
                    != FB) {
                (*scopeTable)[currentScope.parentScopeAddr].functionFlag =
                RFB;
                push_back(
                        (*scopeTable)[currentScope.parentScopeAddr].returnHandle,
                        currentScope.returnHandle);
            }

            (*scopeTable)[currentScopeAddr] = currentScope;

        } else if (currentAssemblyCode.type == F
                && currentAssemblyCode.operatorFlag == Bs
                && currentAssemblyCode.operator_s == "@") {

            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);
            Addr scopeAddr = link(currentScopeAddr, currentAssemblyCode.arg1_i);
            (*scopeTable)[scopeAddr].functionFlag = FN;

#if debug
            {
                static int ignorecount = 0;
                cout << "Scanner::scan()" << " " << "\tignorecount:["
                        << ignorecount++ << "\t]("
                        << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            for (Addr ad = (*scopeTable)[scopeAddr].scopeCoor1;

            ad != addrnull && ad <= (*scopeTable)[scopeAddr].scopeCoor2;

            ad = codeTable->getNextCodeAddr(ad)) {
                if ((*codeTable)[ad].assemblyFormula.flagBit.execute != Du) {
                    (*codeTable)[ad].assemblyFormula.flagBit.execute =
                    F_;
                }

            }

            TemplateFunction tf((*scopeTable)[scopeAddr]);
            Addr tfAddr = scopeAddr;
            tf.templateHandleAddr = lastM;
            tf.organization = currentAssemblyCode.arg2_i;

            if ((*codeTable)[lastM].assemblyFormula.flagBit.changeable[3] == T_) {
                tf.isBackwardFunction = T_;
            }

            //以下为比较之前是否有相同声明
            DecComparator dc(this);
            Addr tfaddr1 = nextSameScopeTfAddr(
                    (*scopeTable)[tfAddr].parentScopeAddr, tfAddr);
            while (tfaddr1 != addrnull) {
                if (((*templateFunctionTable)[tfaddr1].organization
                        != tf.organization)
                        || ((*templateFunctionTable)[tfaddr1].integrate
                                != tf.integrate)) {
                    continue;
                }
                if (dc.cmpDecCodeTree(
                        (*codeTable).getLast(M,
                                (*scopeTable)[tfAddr].scopeCoor2),
                        (*codeTable).getLast(M,
                                (*scopeTable)[tfaddr1].scopeCoor2)) == true) {
                    Intg pos1 = getAddrPosition(codeTable->addrdq,
                            tf.nameStartAddr);
                    Intg pos2 = getAddrPosition(codeTable->addrdq,
                            tf.nameEndAddr);
                    while (pos1 <= pos2) {
                        codeTable->codedq[pos1].assemblyFormula.flagBit.execute =
                        Du;
                        pos1++;
                    }
                    if ((*scopeTable)[tfaddr1].trueAddr != addrnull) {
                        (*scopeTable)[tfAddr].trueAddr =
                                (*scopeTable)[tfaddr1].trueAddr;
                    } else {
                        (*scopeTable)[tfAddr].trueAddr = tfaddr1;
                    }
                    break;

                } else {
                    tfaddr1 = nextSameScopeTfAddr(
                            (*scopeTable)[tfAddr].parentScopeAddr, tfaddr1);
                }
                dc.null();

            }
            //将函数声明添加到templateFunctionTable中，并根据后续属性设置进行完善
            TemplateFunction *tfptr = NULL;
            if (tfaddr1 == addrnull) {
                (*templateFunctionTable).add(tfAddr, tf);
                tfptr = &(*templateFunctionTable)[tfAddr];

            } else {
                tfptr = &(*templateFunctionTable)[tfaddr1];
            }
            while (nextAssemblyCode.type == F
                    && link(currentScopeAddr, nextAssemblyCode.arg1_i) == tfAddr
//                    && nextAssemblyCode.operatorFlag == Bs
//                    && nextAssemblyCode.operator_s == ":"
            ) {
                updateCurrentAssemblyCode();
                currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);
                AssemblyFormula a;
                setQuaternionAndFlagBit(currentAssemblyCode, &a);
                currentCode = Code(currentScopeAddr, a);
                codeTable->addCode(currentCodeAddr, currentCode);
                updateArgAttributeTable(currentCode);
                if (currentCode.assemblyFormula.quaternion.arg2_s == "e") {
                    tfptr->integrate = T_;
                }

            }

        } else if (currentAssemblyCode.type == S
                && currentAssemblyCode.operatorFlag == Bs
                && currentAssemblyCode.operator_s == ":") {
#if debug
            {
                static int ignorecount = 0;
                cout << "Scanner::scan()" << " " << "\tignorecount:["
                        << ignorecount++ << "\t]("
                        << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif

            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);
            System sys(currentAssemblyCode.arg1_s, stackToAddr());
            systemTable->add(sys);

        } else if (currentAssemblyCode.type == S
                && currentAssemblyCode.operatorFlag == Bs
                && currentAssemblyCode.operator_s == "<<") {

            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);
            systemTable->inherit(currentAssemblyCode.arg1_s, stackToAddr(),
                    currentAssemblyCode.arg2_s);

        } else if (currentAssemblyCode.type == S
                && currentAssemblyCode.arg2Flag == Dz
                && currentAssemblyCode.operator_s == "") {

            //此步骤进行scope与sys进行绑定。由于scope位于sys声明之后，
            //因此需要对已有的sys进行补充，
            //同时，重新添加sys对应的ar并补充其继承的类对应的ar
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);
            Addr &&bodyScopeAddr = link(stackToAddr(),
                    currentAssemblyCode.arg2_i);
            Scope &bodyScope = (*scopeTable)[bodyScopeAddr];
            bodyScope.functionFlag = SYSB;
            Addr &scopeCoor1 = bodyScope.scopeCoor1;
            Addr &scopeCoor2 = bodyScope.scopeCoor2;
            (*codeTable)[scopeCoor1].assemblyFormula.flagBit.execute = R_;
            (*codeTable)[scopeCoor2].assemblyFormula.flagBit.execute = R_;

            System sys(currentAssemblyCode.arg1_s, bodyScopeAddr,
                    stackToAddr());
            systemTable->add(sys);
            currentAR->add(Arg(S_Dz, bodyScopeAddr));
            (*currentAR)[bodyScopeAddr].dataFlagBit = S_AR;
            DataTable *sysAR = (*currentAR)[bodyScopeAddr].content_ar =
                    addr_ar_map.find(bodyScopeAddr)->second;
            sysAR->type = SYSB;
            System &sysWhole = (*systemTable)[bodyScopeAddr];

            for (auto id : sysWhole.decedentSystemIDList) {
                Addr &decedentSysAddr = (*systemTable)[id].systemAddr;
                sysAR->queryARList.push_front(addr_ar_map[decedentSysAddr]);
            }

        } else if (currentAssemblyCode.type == TD
                && currentAssemblyCode.operatorFlag == Bs
                && currentAssemblyCode.operator_s == "=>") {

            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);

            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);
            //backward funciton name
            Addr bfnScopeAddr = currentCode.assemblyFormula.quaternion.arg1_a;
            //forward function name
            Addr ffnScopeAddr = currentCode.assemblyFormula.quaternion.arg2_a;
            (*templateFunctionTable)[bfnScopeAddr].isBackwardFunction = true;
            (*templateFunctionTable)[bfnScopeAddr].forwardFunctionAddr =
                    ffnScopeAddr;

            Addr &bfnHandleAddr =
                    (*templateFunctionTable)[bfnScopeAddr].templateHandleAddr;
            Code &bfnHandle = (*codeTable)[bfnHandleAddr];

        } else if (currentAssemblyCode.type == F
                && currentAssemblyCode.arg1Flag == Dz
                && currentAssemblyCode.arg2Flag == Dz
                && currentAssemblyCode.resultFlag == Dz
                && currentAssemblyCode.arg1_i == currentAssemblyCode.result_i
                && currentAssemblyCode.arg1_i != currentAssemblyCode.arg2_i
                && currentAssemblyCode.operator_i == 0
                && currentAssemblyCode.operator_s == "") {

            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);
            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);
            (*templateFunctionTable)[a.quaternion.arg1_a].bodyScopeAddr =
                    a.quaternion.arg2_a;
            (*templateFunctionTable)[a.quaternion.arg1_a].bodyStartAddr =
                    (*scopeTable)[a.quaternion.arg2_a].scopeCoor1;
            (*templateFunctionTable)[a.quaternion.arg1_a].bodyEndAddr =
                    (*scopeTable)[a.quaternion.arg2_a].scopeCoor2;
            (*templateFunctionTable)[a.quaternion.arg1_a].returnHandle =
                    (*scopeTable)[a.quaternion.arg2_a].returnHandle;
            (*scopeTable)[a.quaternion.arg2_a].requireScopeAddr =
                    a.quaternion.arg1_a;
#if debug
            cout << "Scanner::scan F Dz Dz null Dz" << endl;
#endif

            if ((*scopeTable)[a.quaternion.arg2_a].functionFlag == RFB
                    && (*scopeTable)[a.quaternion.arg2_a].parentScopeAddr
                            != addrnull) {
#if debug
                cout << "Scanner::scan F Dz Dz null Dz 3" << endl;
#endif
                Addr pa = (*scopeTable)[a.quaternion.arg2_a].parentScopeAddr;
#if debug
                cout << "Scanner::scan F Dz Dz null Dz 4" << endl;
#endif
                erase((*scopeTable)[pa].returnHandle,
                        (*scopeTable)[a.quaternion.arg2_a].returnHandle);
            }

            (*scopeTable)[a.quaternion.arg2_a].functionFlag = FB;

            (*codeTable)[(*scopeTable)[a.quaternion.arg2_a].scopeCoor1].assemblyFormula.flagBit.execute =
            R;
            (*codeTable)[(*scopeTable)[a.quaternion.arg2_a].scopeCoor2].assemblyFormula.flagBit.execute =
            R;

            Addr scopeAddr = link(currentScopeAddr, currentAssemblyCode.arg2_i);
            Addr ad = (*scopeTable)[scopeAddr].scopeCoor1;
            for (ad = codeTable->getNextCodeAddr(ad);

            ad <= (*scopeTable)[scopeAddr].scopeCoor2 && ad != addrnull;

            ad = codeTable->getNextCodeAddr(ad)) {
                if ((*codeTable)[ad].assemblyFormula.flagBit.type == QS
                        && ((*codeTable)[ad].assemblyFormula.flagBit.execute
                                == R
                                || (*codeTable)[ad].assemblyFormula.flagBit.execute
                                        == F_)) {
                    Addr jumpscopeAddr = (*codeTable)[ad].scopeAddr;
                    ad = (*scopeTable)[jumpscopeAddr].scopeCoor2;
                }
                if ((*codeTable)[ad].assemblyFormula.flagBit.type == M) {
                    Intg *changeable_ =
                            (*codeTable)[ad].assemblyFormula.flagBit.changeable;

                    if (changeable_[0] == A_) {
                        changeable_[0] = F_;
                    }
                    if (changeable_[1] == A_) {
                        changeable_[1] = F_;
                    }
                    if (changeable_[2] == A_) {
                        changeable_[2] = F_;
                    }
                    if (changeable_[3] == A_) {
                        changeable_[3] = F_;
                    }

                }

            }
        } else if (currentAssemblyCode.type == R) {
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            codeTable->addCode(currentCodeAddr, currentCode);

            if (pendM.size() != 0) {
                PendPainter pp(this);
                pp.paintAll();
                pendM.clear();
            }
            if (realM.size() != 0) {
                RealPainter rp(this);
                rp.paintAll();
                realM.clear();
            }

            if (currentScope.functionFlag != FB) {
                currentScope.functionFlag = RFB;
                (*scopeTable)[currentScopeAddr].returnHandle.push_back(
                        currentCodeAddr);
            }

            while (nextAssemblyCode.type != QE) {
                updateCurrentAssemblyCode();
            }

        } else if (currentAssemblyCode.type == B) {
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            if (currentAssemblyCode.arg2Flag == S_Dz) {

                Code &conditionRoot =
                        (*codeTable)[currentCode.assemblyFormula.quaternion.arg2_a];
                currentCode.assemblyFormula.flagBit.changeable[1] =
                        conditionRoot.assemblyFormula.flagBit.changeable[3];
                currentCode.assemblyFormula.flagBit.formalArgFlag[1] =
                        conditionRoot.assemblyFormula.flagBit.formalArgFlag[3];

            }
            codeTable->addCode(currentCodeAddr, currentCode);

            Scope &conditionalBodyScope = (*scopeTable)[a.quaternion.operator_a];
            conditionalBodyScope.conditionFlag = BRB;
            conditionalBodyScope.conditionCodeAddr = currentCodeAddr;

            (*codeTable)[conditionalBodyScope.scopeCoor1].assemblyFormula.flagBit.execute =
            R;
            (*codeTable)[conditionalBodyScope.scopeCoor2].assemblyFormula.flagBit.execute =
            R;

        } else if (currentAssemblyCode.type == L) {
            currentCodeAddr = link(stackToAddr(), currentAssemblyCodePos);

            AssemblyFormula a;
            setQuaternionAndFlagBit(currentAssemblyCode, &a);
            currentCode = Code(currentScopeAddr, a);
            if (currentAssemblyCode.arg2Flag == S_Dz) {

                Code &conditionRoot =
                        (*codeTable)[currentCode.assemblyFormula.quaternion.arg2_a];
                currentCode.assemblyFormula.flagBit.changeable[1] =
                        conditionRoot.assemblyFormula.flagBit.changeable[3];
                currentCode.assemblyFormula.flagBit.formalArgFlag[1] =
                        conditionRoot.assemblyFormula.flagBit.formalArgFlag[3];

            }
            codeTable->addCode(currentCodeAddr, currentCode);

            Scope &conditionalBodyScope = (*scopeTable)[a.quaternion.operator_a];
            conditionalBodyScope.conditionFlag = LOB;
            conditionalBodyScope.conditionCodeAddr = currentCodeAddr;

            (*codeTable)[conditionalBodyScope.scopeCoor1].assemblyFormula.flagBit.execute =
            R;
            (*codeTable)[conditionalBodyScope.scopeCoor2].assemblyFormula.flagBit.execute =
            R;

        }

        updateArgAttributeTable(currentCode);
    }

//{
#if debug
    cout << "Scanner::scan updateCurrentAssemblyCode == false\t" << endl;
#endif
    for (Addr ad = codeTable->addrdq[0];
            ad != addrnull && ad <= codeTable->addrdq.back();
            ad = codeTable->getNextCodeAddr(ad)) {
#if debug
        cout << "Scanner::scan updateCurrentAssemblyCode == false\t"
                "ad:[" << ad.toStrg() << "]" << endl;
#endif
        if ((*codeTable)[ad].assemblyFormula.flagBit.type == QS
                && ((*codeTable)[ad].assemblyFormula.flagBit.execute == R
                        || (*codeTable)[ad].assemblyFormula.flagBit.execute
                                == F_)) {
            Addr jumpscopeAddr = (*codeTable)[ad].scopeAddr;
            ad = (*scopeTable)[jumpscopeAddr].scopeCoor2;
        }
        if ((*codeTable)[ad].assemblyFormula.flagBit.type == M) {
            Intg *changeable_ =
                    (*codeTable)[ad].assemblyFormula.flagBit.changeable;
            /**
             * 这一段意味着在可执行的语句中不允许pendchangeable的存在，如果存在则会被替换为unchangeable
             */

            if (changeable_[0] == A_) {
                changeable_[0] = F_;
            }
            if (changeable_[1] == A_) {
                changeable_[1] = F_;
            }
            if (changeable_[2] == A_) {
                changeable_[2] = F_;
            }
            if (changeable_[3] == A_) {
                changeable_[3] = F_;
            }

        }

    }
//}

    return true;
}

}

#endif /* VM_SCAN_HPP_ */
