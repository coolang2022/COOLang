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

#ifndef VM_HPP_
#define VM_HPP_

#include "vm_debug.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <map>
#include <vector>
#include <deque>
#include <stdint.h>
#include <limits.h>
#include <set>
#include <algorithm>
#include <iterator>

#define safe_delete(p)  { if(p) { delete (p);     (p)=NULL; } }

#define T_  true
#define F_ false
#define A_ 3

#define S_S_JIA -1
#define S_S_JIAN -2
#define S_S_CHENG -3
#define S_S_CHU -4
#define S_S_FUZHI -5
#define S_S_DENGYU -6
#define S_S_MI -7
#define CALL -8
#define SHUCHU -9
#define S_S_DAYU -10
#define S_S_XIAOYU -11
#define COMMA -12
#define TUIDAO -13
#define ADDR_NEG_LOWBOUND -13
#define QS 1
#define QE 2
#define C 3
#define M 4
#define ME 5
#define F 6
#define R 7
#define B 8
#define L 9
#define TD 10
#define S 11
#define Y 12 //引用

#define Dz 0
#define Sz 1
#define Bs 2
#define Zf 3

#define S_Dz 0  //用于arg、data均可，表明其内容为数据区的地址，可用于<dataTable>键、值
#define Sz 1    //用于arg，或data，可用于<dataTable>值
#define S_Bs 2  //用于arg、data均可，表明其内容为数据区的标识，可用于<dataTable>键、值
#define Zf 3    //用于arg、data均可，表明其内容为字符，可用于<dataTable>值
#define Z_Dz 4  //用于arg，表明其内容为作用域表的地址，在<dataTable>中被替换为S_AR
#define M_Bs 5  //用于arg，表明这是一个模板用到的符号+-*/等
#define M_Dz 6  //用于arg，表明这是一个模板用到的符号在符号表中的地址
#define X_Bs 7  //用于arg，表明这是一个作用于变量的属性，亦或是用户定义的在系统表中的类
#define X_Dz 8  //用于arg，表明这是一个作用于变量的属性，亦或是用户定义的在系统表中的类
#define S_AR 9  //通常用于data，表明这是一个类的实例，可用于<dataTable>的值
#define Y_Dz 10 //用于arg，表明这是一个引用，储存在引用表，可用于<dataTable>键，注意，包括Y_Dz的arg其rootAR不能为空

#define FN 1   //函数名称作用域
#define FB 2   //普通函数体
#define RFB 3  //函数体，包含return
#define BRB 4  //分支的体
#define LOB 5  //循环的体
#define SYSB 6 //系统的体

#define Du 2
#define R_ R //一定条件下执行

#define MODE_SCAN 0  //globalRefTable 工作在何种模式，scan模式下没有AR搭配
#define MODE_PREEXE 1 //preexe模式下有ar搭配
#define MODE_EXECUTE 2

using namespace std;
namespace VM {
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
/**
 * union two set（s）
 * @tparam T
 * @param argL
 * @param argR
 * @return
 */
template<class T = Intg>
set<T>& operator+=(set<T> &argL, set<T> &argR) {
    set<T> temp;
    set_union(argL.begin(), argL.end(), argR.begin(), argR.end(),
            inserter(temp, temp.begin()));
    argL = temp;
    return argL;

}
/**
 * L = exit in L not in R
 * @tparam T
 * @param argL
 * @param argR
 * @return
 */
template<typename T = Intg>
set<T>& operator-=(set<T> &argL, set<T> &argR) {
    set<T> temp;

    set_difference(argL.begin(), argL.end(), argR.begin(), argR.end(),
            insert_iterator<set<T>>(temp, temp.begin()));
    argL = temp;
    return argL;
}
/**
 * L = exit in L && in R
 * @tparam T
 * @param argL
 * @param argR
 * @return
 */
template<class T = Intg>
set<T>& operator^=(set<T> &argL, const set<T> &argR) {
    set<T> temp;
    set_intersection(argL.begin(), argL.end(), argR.begin(), argR.end(),
            insert_iterator<set<T>>(temp, temp.begin()));
    argL = temp;
    return argL;
}

class CodeTable;
class Code;
class Addr;
class AssemblyFormula;
class Arg;
class DataTable;
class CodeTable {
public:
    deque<Addr> addrdq;
    deque<Code> codedq;
    Addr getCodeAddrByOffset(const Addr &addr, const Intg offset) const;
    Addr getNextCodeAddr(const Addr &addr) const;
    Addr getAboveCodeAddr(const Addr &addr) const;
    const Intg InsertCode(const Intg position, const Intg offset,
            const Addr &addr, const Code &code);
    const Intg eraseCode(const Intg position, const Intg offset);
    const Intg eraseCode(const Addr &addr, const Intg offset);
    const Addr getScopeAddr(const Addr &codeAddr) const;
    const AssemblyFormula& getAssemblyFormula(const Addr &codeAddr) const;
    const bool compareAssemblyFormula(const Addr &codeAddr) const;
    const bool compareAssemblyFormula(
            const AssemblyFormula &assemblyFormula) const;
    const bool sameScope(const Addr &addr1, const Addr &addr2) const;
    const bool addCode(const Addr &codeAddr, const Code &code);
    Code& operator[](const Addr &addr);
    Code& operator[](const Intg &pos);
    Addr getLast(const Intg &type, const Addr &ad) const;
    Addr getLast(const vector<Intg> &types, const Addr &ad) const;
    Addr getLastNot(const Intg &type, const Addr &ad) const;
    Addr getLastNot(const vector<Intg> &types, const Addr &ad) const;
    Addr getNextNot(const Intg &type, const Addr &ad) const;
    Addr getNextNot(const vector<Intg> &types, const Addr &ad) const;
    Addr getNext(const vector<Intg> &types, const Addr &ad) const;
    Addr getNext(const Intg &type, const Addr &ad) const;
    Addr getSameTrBranchLastArgAddr(const Addr &ad, const Arg &arg);
    Addr getSameTrBranchNextArgAddr(const Addr &ad, const Arg &arg);
    Addr getSameTrBranchLastArgAddrIgnoreProperty(const Addr &ad,
            const Arg &arg);
    pair<Addr, Intg> getSameTrBranchLastArgPositionIgnoreProperty(
            const Addr &ad, const Arg &arg);
    pair<Addr, Intg> getNextArgPosition(const Addr &ad, const Arg &arg);
    Addr getSameTrBranchNextArgAddrIgnoreProperty(const Addr &ad,
            const Arg &arg);
    CodeTable copyTree(const Addr &ad) const;
    void setRefArgAR(DataTable * ar);

    /**
     * @attention copy[adlow,adUp]
     * @param adLow
     * @param adUp
     * @return
     */
    CodeTable copyCodeTable(const Addr &adLow, const Addr &adUp) const;
    CodeTable copyTreeBranch(const Addr &ad) const;
    map<Addr, Addr> resetAddrRange(const Addr &min, const Addr &max);
    void insertCodeTable(const Addr &insertAddr,
            const CodeTable &codeTableInsert);
    /**
     * @brief 删除指定间距内的所有Code，即[min+offset,max+offset]
     * @param eraseAddrMin
     * @param offsetRight1
     * @param eraseAddrMax
     * @param offsetRight2
     */
    void eraseCode(const Addr &eraseAddrMin, Intg offsetRight1,
            const Addr &eraseAddrMax, Intg offsetRight2);
    void appendCodeTable(const Addr &appendAddr,
            const CodeTable &codeTableAppend);
    void appendCodeTable(const CodeTable &codeTableAppend);
    pair<Addr, Addr> unbindFunction(const Addr &addr);
    /**
     * @brief 将CodeTable中所有的argOld替换成argNew
     * @param argOld
     * @param argNew
     */
    void replaceVar(const Arg &argOld, const Arg &argNew);
    /**
     * 复制ad所在区域的函数调用
     * @param ad
     * @return
     */
    CodeTable copyFunctionCall(const Addr &ad);
    Strg toStrg() const;
    Intg size() const {
        return this->addrdq.size();
    }
    CodeTable& operator=(const CodeTable &cdt) {
        this->addrdq = cdt.addrdq;
        this->codedq = cdt.codedq;
        return (*this);

    }
    bool find(const Addr &addr);
    Intg count(const Addr &addr);
    /**
     * @brief 重新设置codeTable中和argset有关的变量的可变属性。无视所有标志位
     * @param arg
     * @param changeable true(设置为可变) false（设置为不可变）
     * @attention argset的changeable属性无效
     */
    void resetChangeable(bool changeable, const set<Arg> &argset);
    CodeTable() {
    }
};

vector<Strg> split(Strg str, Strg pattern) {
    vector<Strg> result;
    str += pattern;
    for (int i = 0; i < (int) str.size(); i++) {
        int pos = (int) str.find(pattern, i);
        if (pos != (int) string::npos && str.substr(i, pos - i) != "") {
            result.push_back(str.substr(i, pos - i));
            i = pos + (int) pattern.size() - 1;
        }
    }
    return result;
}
Intg toIntg(Strg s) {
    if (s == "") {
        {
#if debug
            static int ignorecount = 0;
            cerr << "toIntg(Strg)" << " convert \"\" to Intg,return 0"
                    << "\tignorecount:[" << ignorecount++ << "\t](" << "vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
#endif
        }
    }
    return atoi(s.c_str());
}
Numb toNumb(Strg s) {
    if (s == "") {
        {
#if debug
            static int ignorecount = 0;
            cerr << "toNumb(Strg)" << " convert \"\" to Numb,return 0"
                    << "\tignorecount:[" << ignorecount++ << "\t](" << "vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
#endif
        }
        return 0;
    }
    stringstream ss;
    ss << s;
    Numb num;
    ss >> num;
    if (num < 1e-50 && num > -1e-50) {
        num = 0;
    }
    {
#if debug
        static int ignorecount = 0;
        cout << "toNumb(Strg)" << "s:[" << s << "]\tnum:[" << num << "]"
                << "\tignorecount:[" << ignorecount++ << "\t](" << "vm.hpp:"
                << __LINE__ << ":0" << ")" << endl;
#endif
    }
    return num;
}
Strg toStrg(Intg i) {
    stringstream ss;
    ss << i;
    return ss.str();
}
Strg toStrg(Numb num) {
    stringstream ss;
    ss << num;
    return ss.str();
}
template<typename key, typename value>
Strg toStrg(const map<key, value> &map) {
    stringstream ss;
    ss << "key-value:\n";
    for (auto it : map) {
        ss << "key:\t" << it.first.toStrg() << "\t\nvalue:\t"
                << it.second.toStrg() << "\n";
    }
    return ss.str();

}
template<typename value>
Strg toStrg(const multimap<Intg, value> &map) {
    stringstream ss;
    ss << "key-value:\n";
    for (auto it : map) {
        ss << "key:\t" << it.first << "\t\nvalue:\t" << it.second.toStrg()
                << "\n";
    }
    return ss.str();

}
template<typename List>
Strg toStrg(const List &list) {
    stringstream ss;
    ss << "list:\n";
    for (auto it : list) {
        ss << "element:\t" << it.toStrg() << "\n";
    }
    return ss.str();

}
class Addr {
public:

    vector<Intg> addrv;
    Addr() {

    }
    Addr(Strg addrStrg) {
        vector<Strg> addrStrg_ = split(addrStrg, ".");
        for (Intg i = 0; i < (Intg) addrStrg_.size(); i++) {
            Intg val = toIntg(addrStrg_.at(i));
            if (val != 0) {
                addrv.push_back(val);

            } else {
                return;
            }
        }

    }
    Addr(Intg addri) {
        addrv.push_back(addri);
    }
    Intg size() const {
        return addrv.size();
    }
    Addr(vector<Intg> addr_) {
        addrv.assign(addr_.begin(), addr_.end());
    }

    const Addr& operator=(const Addr &addr_) {
        addrv.assign(addr_.addrv.begin(), addr_.addrv.end());
        return *this;
    }
    const Addr& operator=(const Intg addri) {
        if (addri != 0) {
            addrv.assign(1, addri);
        } else {
#if debug
            cout << "Addr::operator= err,try to assign 0 to addr" << endl;
#endif
        }
        return *this;
    }
    const Addr& operator=(const Strg &addrs) {
        Addr addr_(addrs);
        addrv.assign(addr_.addrv.begin(), addr_.addrv.end());
        return *this;
    }

    const Addr createNext(Intg offset) const {
        Addr addrNext = *this;
        addrNext.addrv.pop_back();
        addrNext.addrv.push_back(addrv.back() + offset);
        return addrNext;
    }
    const Addr createNext() const {

        return createNext(1);
    }
    const Addr createSub(const Addr &suffix) const {
        Addr sub = *this;
        for (int i = 0; i < suffix.size(); i++) {
            sub.addrv.push_back(suffix.addrv.at(i));
        }
        return sub;
    }
    const Addr createSub(const Strg &suffix) const {
        return createSub(Addr(suffix));
    }
    const Addr createSub() const {

        return createSub(Addr("1"));
    }
    const bool operator==(const Addr &addr_) const {
        return addrv == addr_.addrv;
    }
    const bool operator!=(const Addr &addr_) const {
        return addrv != addr_.addrv;
    }
    const bool operator==(const Intg addri) const {
        return addrv == Addr(addri).addrv;
    }
    const bool operator==(const Strg &addrs) const {
        return addrv == Addr(addrs).addrv;
    }
    const bool operator<(const Addr &addr_) const {

        Intg m = min(this->size(), addr_.size());
        for (Intg i = 0; i < m; i++) {
            if (addrv.at(i) < addr_.addrv.at(i)) {
                return true;
            } else if (addrv.at(i) > addr_.addrv.at(i)) {
                return false;
            }

        }
        if (this->size() < addr_.size()) {
            if (addr_.addrv.at(m) > 0) {
                return true;
            } else {
                return false;
            }
            return true;
        } else if (this->size() == addr_.size()) {
            return false;
        } else if (this->size() > addr_.size()) {
            if (addrv.at(m) < 0) {
                return true;
            } else {
                return false;
            }
        }
        return false;
    }
    const bool operator<=(const Addr &addr_) const {

        Intg m = min(this->size(), addr_.size());

        for (Intg i = 0; i < m; i++) {
            if (addrv.at(i) < addr_.addrv.at(i)) {
                return true;
            } else if (addrv.at(i) > addr_.addrv.at(i)) {
                return false;
            }

        }

        if (this->size() < addr_.size()) {

            if (addr_.addrv.at(m) > 0) {
                return true;
            } else {
                return false;
            }
            return true;
        } else if (this->size() == addr_.size()) {

            return true;
        } else if (this->size() > addr_.size()) {

            if (addrv.at(m) < 0) {

                return true;
            } else {

                return false;
            }
        }
        return false;
    }
    const bool operator>=(const Addr &addr_) const {
        return !(*this < addr_);
    }
    const bool operator>(const Addr &addr_) const {
        return !(*this <= addr_);
    }
    /**
     * @name operator++()
     * @brief ==createNext
     * @return
     */
    Addr& operator++() {
        addrv.back() += 1;
        return (*this);
    }
    /**
     * @name operator++(int)
     * @brief ==createNext
     * @return
     */
    const Addr operator++(int) const {
        return createNext();
    }
    /**
     * @name operator+(Intg offset)
     * @brief ==createNext()
     * @return
     */
    const Addr operator+(Intg offset) const {
        return createNext(offset);
    }
    const Intg operator[](Intg pos) const {
        return this->addrv.at(pos);
    }
    const Strg toStrg() const {
        stringstream addrss;
        int sz = this->size();
        for (int i = 0; i < sz; i++) {
            if (i != 0) {
                addrss << ".";
            }
            addrss << ((*this)[i]);
        }
        return addrss.str();

    }

};
static Addr addrnull = Addr();
static Addr addrmax = Addr(/*INT32_MAX*/INT_MAX);
Addr link(const Addr &addr1, const Addr &addr2) {
    Addr a;
    a = addr1;
    for (int i = 0; i < addr2.size(); i++) {
        a.addrv.push_back(addr2[i]);

    }
    return a;

}
Addr link(const Addr &addr1, const Intg &addr2) {

    return link(addr1, Addr(addr2));

}

bool push_back(vector<Addr> &arr1, const vector<Addr> arr2) {

    Intg s = arr2.size();
    Intg pos = 0;
    while (pos <= s) {
        arr1.push_back(arr2[pos]);
    }
    return true;
}

class AddrList;
class AssemblyFormula;
class FlagBit;
class Quaternion;
class ResultProperty;

#define Ints int
class FlagBit {
public:
    Ints execute = T_;

    //   Ints forward = 0;

    Ints type = 0;

    Addr tfAddr;

    Ints argFlag = 0;

    Ints operationOrderFlag = 0;

//    Ints resultFlag = 0;

    bool functionHandleFlag = false;

    bool formalArgFlag[4] = { T_, T_, T_, T_ };

    Intg changeable[4] = { F_, F_, F_, F_ };

    Intg unknown[4] = { F_, F_, F_, F_ };

    Intg arg1_flag = 0;
    Intg arg2_flag = 0;
    Intg operator_flag = 0;
    Intg result_flag = 0;
    Addr a1s;
    Addr a2s;
    Addr ops;
    Addr res;

    Strg toStrg() const {
        stringstream ss;
        ss << "exe:" << execute << "\ttype:" << type << "\targF:" << argFlag
                << " tfAddr:[" << tfAddr.toStrg() << "]\t" << "asc:["
                << a1s.toStrg() << "\t| " << a2s.toStrg() << "\t| "
                << ops.toStrg() << "\t| " << res.toStrg() << "\t]"

                << "form:[" << formalArgFlag[0] << " " << formalArgFlag[1]
                << " " << formalArgFlag[2] << " " << formalArgFlag[3]
                << "]\tfhf:[" << functionHandleFlag << "]\tchag:["
                << changeable[0] << " " << changeable[1] << " " << changeable[2]
                << " " << changeable[3] << "]\targF:[" << arg1_flag << " "
                << arg2_flag << " " << operator_flag << " " << result_flag
                << "] ";
        return ss.str();
    }
};
class Quaternion {
public:
    Numb arg1_i = 0;

    Addr arg1_a;

    Strg arg1_s;

    DataTable * arg1_ar = NULL;

    Numb arg2_i = 0;

    Addr arg2_a;

    Strg arg2_s;

    DataTable * arg2_ar = NULL;

    Numb operator_i = 0;

    Addr operator_a;

    Strg operator_s;

    DataTable * operator_ar = NULL;

    Numb result_i = 0;

    Addr result_a;

    Strg result_s;

    DataTable * result_ar = NULL;

    Strg toStrg() const {
        stringstream ss;
        ss << " [" << arg1_i << " " << arg1_a.toStrg() << " " << arg1_s << "\t|"
                << arg2_i << " " << arg2_a.toStrg() << " " << arg2_s << "\t|"
                << operator_i << " " << operator_a.toStrg() << " " << operator_s
                << "\t|" << result_i << " " << result_a.toStrg() << " "
                << result_s << "]";
        return ss.str();
    }

};
class Property;
class PropertyList;
PropertyList getPropertyList(const CodeTable *cdt, Addr codeAddr, Intg pos);

class AssemblyFormula;
class AssemblyFormula {
public:
    FlagBit flagBit;
    Quaternion quaternion;

    Strg toStrg() const {
        stringstream ss;
        ss << " " << flagBit.toStrg() << " " << quaternion.toStrg() << " ";
        return ss.str();
    }
};
static AssemblyFormula asfnull = AssemblyFormula();
class Scope;
class Code {
public:
    Addr scopeAddr;
    AssemblyFormula assemblyFormula;
    Code() {
    }
    Code(const Addr &scopeAddr_, const AssemblyFormula &assemblyFormula_) {
        scopeAddr = scopeAddr_;
        assemblyFormula = assemblyFormula_;
    }
//    Code& operator=(const Code * const code_) {
//        if (code_ == NULL) {
//            this->~Code();
//            return *this;
//        }
//        scopeAddr = code_->scopeAddr;
//        assemblyFormula = code_->assemblyFormula;
//        return *this;
//    }
    Code& null() {
        this->scopeAddr = Addr();
        this->assemblyFormula = AssemblyFormula();
        return *this;
    }
    Code& operator=(const Code &code_) {

        scopeAddr = code_.scopeAddr;
        assemblyFormula = code_.assemblyFormula;

        return *this;
    }
    Arg operator[](const Intg &pos) const;
    const bool includeArg(const Arg &arg) const;
    Strg toStrg() const {
        stringstream ss;
        ss << "Code::scop:[" << scopeAddr.toStrg() << "]\t "
                << assemblyFormula.toStrg() << endl;

        return ss.str();
    }
    /**
     * @brief 将code的指定位置（0,1,2,3,）的相应参数的“参数类型”、“参数的值”替换为arg的对应值，其他属性不变
     * @param position
     * @param arg
     */
    void setArg(const Intg &position, const Arg &arg);

};
static Code codenull = Code();
const bool CodeTable::addCode(const Addr &codeAddr, const Code &code) {
    addrdq.push_back(codeAddr);
    codedq.push_back(code);
    return true;
}

Strg CodeTable::toStrg() const {
    stringstream ss;
    ss << "CodeTable:" << endl;

    Intg s = addrdq.size();
    for (Intg p = 0; p < s; p++) {
        ss << "Addr:" << addrdq[p].toStrg() << "\t" << codedq[p].toStrg();

    }

    return ss.str();
}
template<typename Array_>
const Intg getAddrPosition(const Array_ &arr, const Addr &addr) {
    Intg left = 0;
    Intg right = arr.size() - 1;
    Intg mid;
    while (left <= right) {
        mid = (left + right) / 2;

        //要找的数在中点左边
        if (addr < arr[mid]) {
            right = mid - 1;

        }
        //要找的数在中点右边
        else if (addr > arr[mid]) {
            left = mid + 1;

        }
        //否则找到了，直接跳出
        else {

            return mid;
            break;
        }

    }
#if debug
    cerr << "getAddrPosition: can't found Addr[" << addr.toStrg()
            << "] position,return -1" << endl;
#endif
    mid = -1;
    return mid;

}
template<typename Array_>
const Intg getLastAddrPosition(const Array_ &arr, const Addr &addr) {
    Intg left = 0;
    Intg right = arr.size() - 1;
    Intg mid;
    if (addr <= arr[0]) {
#if debug
        cerr << "getLastAddrPosition err::addr<=arr[0]:addr:[" << addr.toStrg()
                << "]" << endl;
#endif

        return -1;
    }
    if (addr > arr.back()) {
        return right;
    }
    while (left <= right) {
        mid = (left + right) / 2;

        //要找的数在中点左边
        if (addr < arr[mid]) {
            right = mid - 1;

        }
        //要找的数在中点右边
        else if (addr > arr[mid]) {
            left = mid + 1;

        }
        //否则找到了，直接跳出
        else {

            return mid - 1;
            break;
        }

    }
    return (left + right) / 2;

}

template<typename Array_>
const Intg getNextAddrPosition(const Array_ &arr, const Addr &addr) {
    Intg left = 0;
    Intg right = arr.size() - 1;
    Intg mid;
    if (addr < arr[0]) {
        return 0;
    }
    if (addr >= arr.back()) {
#if debug
        cerr << "getNextAddrPosition err::addr>arr[max]:addr:[" << addr.toStrg()
                << "]" << endl;
#endif
        return -1;
    }
    while (left <= right) {
        mid = (left + right) / 2;

        //要找的数在中点左边
        if (addr < arr[mid]) {
            right = mid - 1;

        }
        //要找的数在中点右边
        else if (addr > arr[mid]) {
            left = mid + 1;

        }
        //否则找到了，直接跳出
        else {

            return mid + 1;
            break;
        }

    }
    return (left + right) / 2 + 1;

}
bool CodeTable::find(const Addr &addr) {
    if (getAddrPosition(addrdq, addr) >= 0) {
        return true;
    } else {
        return false;
    }
}
Intg CodeTable::count(const Addr &addr) {
    if (getAddrPosition(addrdq, addr) >= 0) {
        return 1;
    } else {
        return 0;
    }
}
template<typename Array_>
const bool erase(Array_ &arr, const Addr &addr) {
    arr.erase(arr.begin() + getAddrPosition(arr, addr));
    return true;
}
template<typename Array_1, typename Array_2>
const bool erase(Array_1 &arrfrom, const Array_2 &arr) {
    Intg s = arr.size();
    while (s >= 0) {
        arrfrom.erase(arrfrom.begin() + getAddrPosition(arrfrom, arr[s]));
        s--;
    }
    return true;
}
Addr CodeTable::getCodeAddrByOffset(const Addr &addr,
        const Intg offset) const {
    try {
        return addrdq.at(
                getAddrPosition<deque<Addr> >(this->addrdq, addr) + offset);
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::getCodeAddrByOffset:[" << e.what() << "] Addr:["
                << addr.toStrg() << "] AddrPosition:["
                << getAddrPosition<deque<Addr> >(this->addrdq, addr)
                << "] offset:[" << offset << "]" << endl;
#endif
        return addrnull;
    }
}
Addr CodeTable::getNextCodeAddr(const Addr &addr) const {
    try {
        return getCodeAddrByOffset(addr, 1);
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::getNextCodeAddr:[" << e.what() << "] Addr:["
                << addr.toStrg() << "]" << endl;
#endif
        return addrnull;
    }
}
Addr CodeTable::getAboveCodeAddr(const Addr &addr) const {
    try {
        return getCodeAddrByOffset(addr, -1);
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::getNextCodeAddr:[" << e.what() << "] Addr:["
                << addr.toStrg() << "]" << endl;
#endif
        return addrnull;
    }
}
const Intg CodeTable::InsertCode(const Intg position, const Intg offset,
        const Addr &addr, const Code &code) {
    Intg pos = position + offset;
    try {
        deque<Addr>::iterator ita = (this->addrdq).begin() + pos;
        this->addrdq.insert(ita, addr);
        deque<Code>::iterator itc = (this->codedq).begin() + pos;
        this->codedq.insert(itc, code);
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::InsertCode:[" << e.what() << "] position:["
                << position << "]" << endl;
#endif
    }
    return pos;

}
const Intg CodeTable::eraseCode(const Intg position, const Intg offset) {
    try {
        this->addrdq.erase(addrdq.begin() + position + offset);
        this->codedq.erase(codedq.begin() + position + offset);
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::eraseCode:[" << e.what() << "] position:["
                << position << "]" << endl;
#endif
    }
    return position;
}
const Intg CodeTable::eraseCode(const Addr &addr, const Intg offset) {
    Intg position = getAddrPosition<deque<Addr> >(this->addrdq, addr);
    if (position < 0 || position >= (Intg) addrdq.size()) {
#if debug
        cout << "CodeTable::eraseCode err::can't find addr:[" << addr.toStrg()
                << "]" << endl;
#endif
        return -1;
    }
    try {
        this->addrdq.erase(addrdq.begin() + position + offset);
        this->codedq.erase(codedq.begin() + position + offset);
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::eraseCode:[" << e.what() << "] position:["
                << position << "]" << endl;
#endif
    }
    return position;
}
const Addr CodeTable::getScopeAddr(const Addr &codeAddr) const {
    try {
        return codedq.at(getAddrPosition<deque<Addr> >(this->addrdq, codeAddr)).scopeAddr;
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::getScopeAddr:[" << e.what() << "] Addr:["
                << codeAddr.toStrg() << "]" << endl;
#endif
        return addrnull;
    }
}

const AssemblyFormula& CodeTable::getAssemblyFormula(
        const Addr &codeAddr) const {
    try {
        return codedq.at(getAddrPosition<deque<Addr> >(this->addrdq, codeAddr)).assemblyFormula;
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::getAssemblyFormula:[" << e.what() << "] Addr:["
                << codeAddr.toStrg() << "]" << endl;
#endif
        return asfnull;
    }
}
const bool CodeTable::sameScope(const Addr &addr1, const Addr &addr2) const {
    try {
        return getScopeAddr(addr1) == getScopeAddr(addr2);
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::sameScope:[" << e.what() << "] Addr:["
                << addr1.toStrg() << " | " << addr2.toStrg() << "]" << endl;
#endif
        return false;
    }
}
/**
 *operator[ad]
 * @attention 注意此函数当且仅当ad对应的code存在时才返回正确的引用，
 * 当ad不存在时返回错误的引用，这样做是为了保证不被莫名插入代码。
 * 因此在使用此函数之前一定要确定此地址正确
 * @param addr
 * @return
 */
Code& CodeTable::operator[](const Addr &addr) {
    if (addr == addrnull) {
#if debug
        {
            static int ignorecount = 0;
            cerr << "CodeTable::operator [](const Addr&)"
                    << " input addrnull，exit(-1)" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:\\eclipseC&CPP-workspace\\HAN_VM\\includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
        return codenull.null();
    }
    try {
        return codedq[getAddrPosition<deque<Addr> >(this->addrdq, addr)];
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::operator[]:[" << e.what() << "] Addr:["
                << addr.toStrg() << "]" << endl;
#endif
        return codenull.null();
    }
}
Code& CodeTable::operator[](const Intg &pos) {
    if (pos < 0 || pos >= (Intg) (addrdq.size())) {
#if debug
        {
            static int ignorecount = 0;
            cerr << "CodeTable::operator [](const Intg&)" << " input pos<<["
                    << pos << "] out of range ，exit(-1)" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:\\eclipseC&CPP-workspace\\HAN_VM\\includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
        return codenull.null();
    }
    try {
        return codedq[pos];
    } catch (exception &e) {
#if debug
        cerr << "CodeTable::operator[]:[" << e.what() << "] pos:[" << pos << "]"
                << endl;
#endif
        return codenull.null();
    }
}
/**
 * @brief 此函数用于将codeTable中addr对应所在语法树取消与函数的绑定。如果本来就没有绑定，则什么都不做
 * @param addr
 * @return pair<解绑的最小地址，解绑的最大地址>
 */
pair<Addr, Addr> CodeTable::unbindFunction(const Addr &addr) {
    Code &cd = (*this)[addr];
    Addr tfaddr = cd.assemblyFormula.flagBit.tfAddr;
    Addr unbindAddrMin = addr;
    Addr unbindAddrMax = addr;
    if (tfaddr == addrnull) {
        return pair<Addr, Addr>();
    } else {

        Intg pos = getAddrPosition(this->addrdq, addr);
        Intg pos_to_top = pos;
        --pos_to_top;
        while (pos_to_top >= 0) {
            Code &cd1 = codedq[pos_to_top];

            if (cd1.assemblyFormula.flagBit.tfAddr
                    == tfaddr&&cd1.assemblyFormula.flagBit.functionHandleFlag==F_) {
                cd1.assemblyFormula.flagBit.tfAddr = addrnull;
                unbindAddrMin = addrdq[pos_to_top];
            } else {
                break;
            }
        }
        cd.assemblyFormula.flagBit.tfAddr = addrnull;
        if (cd.assemblyFormula.flagBit.functionHandleFlag == true) {
            cd.assemblyFormula.flagBit.functionHandleFlag = false;
            return pair<Addr, Addr>(unbindAddrMin, unbindAddrMax);
        }
        Intg pos_to_bottom = pos;
        ++pos_to_bottom;
        while (pos_to_bottom < size()) {
            Code &cd2 = codedq[pos_to_bottom];
            if (cd2.assemblyFormula.flagBit.tfAddr == tfaddr) {
                unbindAddrMax = addrdq[pos_to_bottom];
                if (cd2.assemblyFormula.flagBit.functionHandleFlag == F_) {
                    cd2.assemblyFormula.flagBit.tfAddr = addrnull;
                } else {
                    cd2.assemblyFormula.flagBit.tfAddr = addrnull;
                    cd.assemblyFormula.flagBit.functionHandleFlag = false;
                    return pair<Addr, Addr>(unbindAddrMin, unbindAddrMax);
                }
            } else {
                return pair<Addr, Addr>(unbindAddrMin, unbindAddrMax);
            }
        }
    }
    return pair<Addr, Addr>(unbindAddrMin, unbindAddrMax);

}

/**
 * 复制addr所在区域的函数调用
 * @param addr
 * @return
 */
CodeTable CodeTable::copyFunctionCall(const Addr &addr) {
    Code &cd = (*this)[addr];
    Addr tfaddr = cd.assemblyFormula.flagBit.tfAddr;
    Addr functionCallAddrMin = addr;
    Addr functionCallAddrMax = addr;
    if (tfaddr == addrnull) {
        return CodeTable();
    } else {

        Intg pos = getAddrPosition(this->addrdq, addr);
        Intg pos_to_top = pos;
        --pos_to_top;
        while (pos_to_top >= 0) {
            Code &cd1 = codedq[pos_to_top];

            if (cd1.assemblyFormula.flagBit.tfAddr
                    == tfaddr&&cd1.assemblyFormula.flagBit.functionHandleFlag==F_) {
                functionCallAddrMin = addrdq[pos_to_top];
                --pos_to_top;
            } else {
                break;
            }
        }
        if (cd.assemblyFormula.flagBit.functionHandleFlag == true) {
            return copyCodeTable(functionCallAddrMin, functionCallAddrMax);
        }
        Intg pos_to_bottom = pos;
        ++pos_to_bottom;
        while (pos_to_bottom < size()) {
            Code &cd2 = codedq[pos_to_bottom];
            if (cd2.assemblyFormula.flagBit.tfAddr == tfaddr) {
                functionCallAddrMax = addrdq[pos_to_bottom];
                if (cd2.assemblyFormula.flagBit.functionHandleFlag == F_) {
                    ++pos_to_bottom;
                } else {
                    return copyCodeTable(functionCallAddrMin,
                            functionCallAddrMax);
                }
            } else {
                return copyCodeTable(functionCallAddrMin, functionCallAddrMax);
            }
        }
    }
    return copyCodeTable(functionCallAddrMin, functionCallAddrMax);
}
class ScopeTable;
class SystemTable;
class TemplateFunctionTable;
class TemplateFunction;
class TemplateFunction {
public:
    Addr nameStartAddr;
    Addr nameEndAddr;
    Intg matchLevel = 0;
//vector<Addr> templateHandleAddrList;
    Addr templateHandleAddr;
    Addr bodyScopeAddr;

    vector<Addr> returnHandle;
    Addr bodyStartAddr;
    Addr bodyEndAddr;
    Intg integrate = F_;
    Intg organization = 100;
    bool isBackwardFunction = false;
    Addr forwardFunctionAddr;
    TemplateFunction(const Scope &functionNameScope);
    TemplateFunction(Addr addrneg) {
        nameStartAddr = addrneg;
        nameEndAddr = addrneg;
        templateHandleAddr = addrneg;
        bodyScopeAddr = addrneg;
        bodyEndAddr = addrneg;
        returnHandle.push_back(addrneg);
        integrate = F_;
        organization = 10;
    }
};

class TemplateFunctionTable {
public:
    deque<Addr> addrdq;
    deque<TemplateFunction> tfdq;

    const Intg addTemplateFunction(const Addr &addr,
            const TemplateFunction &templateFunction);

    TemplateFunction& operator[](const Addr &tfAddr) {
        return tfdq[getAddrPosition<deque<Addr> >(addrdq, tfAddr)];
    }

    typedef Addr TFAddr;
    const Addr lastTfAddr(const TFAddr &tfAddr) {
#if debug
        cout << "TemplateFunctionTable::lastTfAddr tfaddr in:["
                << tfAddr.toStrg() << "]" << endl;
#endif
        Intg i = getLastAddrPosition<deque<Addr> >(addrdq, tfAddr);
        if (i < 0) {
            return Addr();
        }
        if (tfdq[i].nameEndAddr >= tfAddr && tfdq[i].nameStartAddr <= tfAddr) {
            i -= 1;
        }
        Addr a = addrdq[i];
#if debug
        cout << "TemplateFunctionTable::lastTfAddr addr out:[" << a.toStrg()
                << "]" << endl;
#endif
        return a;
    }

    const Addr nextTfAddr(const Addr &codeTableAddr) {
        Intg i = getNextAddrPosition<deque<Addr> >(addrdq, codeTableAddr);
        if (i > (Intg) addrdq.size() - 1 || i < 0) {
            return Addr();
        }
        Addr a =
                addrdq[getLastAddrPosition<deque<Addr> >(addrdq, codeTableAddr)];
        return a;
    }

    const bool add(const Addr &tfAddr, const TemplateFunction &tf) {
        addrdq.push_back(tfAddr);
        tfdq.push_back(tf);
        return true;
    }
    const Addr lastAccessibleTFAddr(ScopeTable *const scopeTable,
            CodeTable *const codeTable, SystemTable *const systemTable,
            const Addr &lowbound, const Addr &codeAddr);
    TemplateFunctionTable() {
        Intg p = ADDR_NEG_LOWBOUND;
        while (p < 0) {
            add(Addr(p), TemplateFunction(Addr(p)));
            p++;
        }

    }
};

class Property;
class Property {
public:
    Addr propertyNameStartAddr;
    Addr propertyNameEndAddr;
    vector<Addr> propertyHandleAddrList;
    vector<Addr> propertyArgAddrList;
    Addr propertyStartAddr;
    Addr propertyEndAddr;

};
class PropertyTable;
class PropertyTable {
public:
    deque<Addr> addrdq;
    deque<Property> propdq;

    const Intg addProperty(const Addr &addr, const Property &property) {
        addrdq.push_back(addr);
        propdq.push_back(property);
        return 1;
    }

};
class Data;
class DataTable;

class ScopeTable;
class Scope;
class Scope {
public:
    Ints functionFlag = 0;
    Ints conditionFlag = 0;
    bool matched = false;
    Addr scopeNameAddr;
    Addr scopeCoor1;
    Addr scopeCoor2;
    Addr parentScopeAddr;
    Addr requireScopeAddr;
    Addr conditionCodeAddr;
    vector<Addr> returnHandle;
    vector<Property> scopeProperty;
    Addr trueAddr;

};
static Scope scopenull = Scope();
class ScopeTable {
public:
    deque<Addr> addrdq;
    deque<Scope> scopedq;
    Intg newScopeStart();
    Intg newScopeStart(Addr &scopeAddr, Scope &scope);
    Intg addScopeProperty();
    Intg scopeEnd();
    Intg scopeEnd(Addr &scopeAddr, Scope &scope);
    /*
     * @attention 不能通过此[] 运算符创建新的scope。
     */
    Scope& operator[](const Addr &addr) {
        if (addr == addrnull) {
            return scopenull;
        }
        return scopedq[getAddrPosition<deque<Addr> >(addrdq, addr)];
    }
    Addr getNextScopeAddr(const Addr &codeTableAddr) {
        return addrdq[getNextAddrPosition(addrdq, codeTableAddr)];
    }
    Addr getLastScopeAddr(const Addr &codeTableAddr) {
        return addrdq[getLastAddrPosition(addrdq, codeTableAddr)];
    }
    /**
     * @brief 用于判断是否能从作用域origin访问到作用域dest(scope to scope)，适用于类与函数
     * @param origin 起始作用域的QS地址，
     * @param dest 目标作用域的QS地址
     * @return
     */
    bool accessible(const Addr &origin, const Addr &dest) {
        //不能访问在后面定义的作用域
        if (origin < dest) {
            return false;
        }
        //两者相同可以访问
        if (origin == dest) {
            return true;
        }
        //dest是公共域也可以访问
        if (dest == addrnull) {
            return true;
        }
        //dest是origin上层scope或上层scope的直接子scope的话可以访问
        Addr dest_parent_addr = this->operator [](dest).parentScopeAddr;
        if (dest_parent_addr == addrnull) {
            return true;
        }
        Addr orig_parent_addr = this->operator [](origin).parentScopeAddr;
        while (orig_parent_addr != addrnull) {
            if (orig_parent_addr == dest_parent_addr) {
                return true;
            }
            orig_parent_addr =
                    this->operator [](orig_parent_addr).parentScopeAddr;
        }

        //其他情况无法访问
        return false;

    }
    /**
     * @brief 用于判断是否能从代码地址origin访问到作用域dest(addr to scope)，适用于类与函数
     * @attention 仅仅从作用域角度考虑是否可以访问，不考虑类之间的继承关系引起的访问
     * @param origin 起始代码codeTable地址（不能是scope地址）
     * @param parent 起始代码所在scope的地址
     * @param dest 目标作用域的scope地址
     * @return
     */
    bool accessible(const Addr &origin, const Addr &parent, const Addr &dest) {
        //不能访问在后面定义的作用域
        if (origin < dest) {
            return false;
        }
        //可以访问父作用域
        if (parent == dest) {
            return true;
        }
        //dest是公共域也可以访问
        if (dest == addrnull) {
            return true;
        }
        //dest与origin有相同的父作用域
        if (this->operator [](dest).parentScopeAddr == parent) {
            return true;
        }
        //dest是origin上层scope或上层scope的直接子scope的话可以访问
        Addr dest_parent_addr = this->operator [](dest).parentScopeAddr;
        if (dest_parent_addr == addrnull) {
            return true;
        }
        Addr orig_parent_addr = this->operator [](parent).parentScopeAddr;
        while (orig_parent_addr != addrnull) {
            if (orig_parent_addr == dest_parent_addr) {
                return true;
            }
            orig_parent_addr =
                    this->operator [](orig_parent_addr).parentScopeAddr;
        }

        //其他情况无法访问
        return false;

    }

};
class System {
public:
    //id由systemTable自动分配，禁止手动赋值
    Intg id = 0;
    Strg systemName;
    Addr systemAddr;
    Addr parentScopeAddr;
    set<Addr> constructorAddrList;
//    deque<Addr> parentSystemAddrList;
    set<Intg> decedentSystemIDList;
    System() {
    }
    System(const Strg &name);

    System(const Strg &name, const Addr &parentScopeAddr);

    System(const Strg &name, const Addr &systemScopeAddr,
            const Addr &parentScopeAddr);

    Strg toStrg();

};
/**
 * @attention 可以通过【】运算符来检索system的引用，但不能通过【】来创建新的引用。
 */
class SystemTable {
public:
    ScopeTable *scopeTable = NULL;
    Intg id = 0;

    map<Intg, System> id_system_map; //逆向函数执行时的顺序backwardIDdeque
    map<Strg, deque<Intg>> name_iddq_map;
    map<Addr, Intg> addr_id_map;
    void add(const System &sys);

    bool systemExisted(const System &sys);

    bool isChild(const Addr &maychild, const Addr &mayparent);

    System& operator[](const Addr &addr);

    System& operator[](const Intg &id);

    System& operator[](const pair<Strg, Addr> &name_parentScopeAddr);

    System& getDirectSystemByCodeAddr(CodeTable *codeTable,
            const Addr &codeAddr);

    Intg getSystemID(const pair<Strg, Addr> &name_parentScopeAddr);

    void erase(Intg id);

    Strg toStrg();

    void inherit(const Strg &childName, const Addr &childParentScopeAddr,
            const Strg &parentName);

};
Intg ScopeTable::newScopeStart(Addr &scopeAddr, Scope &scope) {
    addrdq.push_back(scopeAddr);
    scopedq.push_back(scope);
    return 0;
}
Intg ScopeTable::scopeEnd(Addr &scopeAddr, Scope &scope) {
    scopedq[getAddrPosition<deque<Addr> >(addrdq, scopeAddr)] = scope;
    return 0;
}
TemplateFunction::TemplateFunction(const Scope &functionNameScope) {
    nameStartAddr = functionNameScope.scopeCoor1;
    nameEndAddr = functionNameScope.scopeCoor2;

}
/**
 * @brief 查询codeORscopeAddr可访问到，且在upbound之前的函数（在函数表中）的地址
 * @attention 对无效输入的代码地址报错，没有合适的结果返回addrnull。
 * @param scopeTable
 * @param codeTable
 * @param upbound 查询的tf地址必须大于此地址,此地址为scopeAddr时会取用scope的coor1
 * @param codeORscopeAddr 可以输入代码表地址或函数表、作用域表地址，都行的,查询此处地址可访问到的tf地址
 * @param
 * @return
 */
const Addr TemplateFunctionTable::lastAccessibleTFAddr(
        ScopeTable *const scopeTable, CodeTable *const codeTable,
        SystemTable *const systemTable, const Addr &upbound,
        const Addr &codeORscopeAddr) {
#if debug
    {
        static int ignorecount = 0;
        cout
                << "TemplateFunctionTable::lastAccessibleTFAddr(ScopeTable* const, CodeTable* const, const Addr&)"
                << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    Addr origin; // codeAddr
    if (codeTable->find(codeORscopeAddr) == true) {
        origin = codeORscopeAddr;
    } else if (getAddrPosition(scopeTable->addrdq, codeORscopeAddr) > 0) {
        origin = (*scopeTable)[codeORscopeAddr].scopeCoor1;
    }
    Addr dest;
    if (codeTable->find(upbound) == true) {
        dest = upbound;
    } else if (getAddrPosition(scopeTable->addrdq, upbound) > 0) {
        dest = (*scopeTable)[upbound].scopeCoor1;
    } else if (upbound < addrnull) {
        dest = upbound;
    } else if (upbound >= codeORscopeAddr) {
        dest = codeORscopeAddr;
    }
    dest = this->lastTfAddr(dest);
    if (dest > addrnull) {
        //以下代码，从其父作用域开始递归查询上一个可访问tf

        if (scopeTable->accessible(origin, (*codeTable)[origin].scopeAddr,
                dest)) {
            return dest;
        } else {
            //判断
            Addr dest_parentScopeAddr = (*scopeTable)[dest].parentScopeAddr;
            if (dest_parentScopeAddr == addrnull) {
                return lastAccessibleTFAddr(scopeTable, codeTable, systemTable,
                        dest, codeORscopeAddr);
            }
            Scope &dest_parentScope =
                    (*scopeTable)[(*scopeTable)[dest].parentScopeAddr];
            if (dest_parentScope.functionFlag == SYSB) {
                Addr origin_systemAddr;
                try {
                    origin_systemAddr = systemTable->getDirectSystemByCodeAddr(
                            codeTable, origin).systemAddr;

                } catch (int &e) {

                    origin_systemAddr = addrnull;
                }
                if (origin_systemAddr == addrnull) {
                    return lastAccessibleTFAddr(scopeTable, codeTable,
                            systemTable, dest, codeORscopeAddr);
                } else if (systemTable->isChild(origin_systemAddr,
                        dest_parentScopeAddr)) {
                    return dest;
                } else {
                    return lastAccessibleTFAddr(scopeTable, codeTable,
                            systemTable, dest, codeORscopeAddr);
                }

            }

        }

    }

    else if (dest < addrnull) {
#if debug
        cout << "TemplateFunctionTable::lastAccessibleTFAddr 6" << endl;
#endif

        return dest;

    } else {
#if debug
        cout << "TemplateFunctionTable::lastAccessibleTFAddr 7" << endl;
#endif
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "TemplateFunctionTable::lastAccessibleTFAddr(ScopeTable* const, CodeTable* const, const Addr&)"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        return addrnull;
    }
#if debug
    cout << "TemplateFunctionTable::lastAccessibleTFAddr 8" << endl;
#endif
    return addrnull;
}

System::System(const Strg &name) {
    systemName = name;
}
System::System(const Strg &name, const Addr &parentScopeAddr) {
    systemName = name;
    this->parentScopeAddr = parentScopeAddr;
}
System::System(const Strg &name, const Addr &systemScopeAddr,
        const Addr &parentScopeAddr) {
    systemName = name;
    systemAddr = systemScopeAddr;
    this->parentScopeAddr = parentScopeAddr;
}
Strg System::toStrg() {
    stringstream ss;
    ss << systemName << "\t" << systemAddr.toStrg() << "\tconstructorAddrList:";
    for (auto &addr : constructorAddrList) {
        ss << addr.toStrg() << "\t";
    }
    ss << "decedentSystemIDList:[";
    for (auto &id : decedentSystemIDList) {
        ss << id << "\t";
    }
    ss << "]";

    return ss.str();
}
/**
 * @brief 相当于reset，当sys不存在时，创建sys，存在时，则令已存在的sys等于新的sys
 * @param sys
 */
void SystemTable::add(const System &sys) {
#if debug
    {
        static int ignorecount = 0;
        cout << "SystemTable::add(const System&)" << "\tsysName:["
                << sys.systemName << "]" << "\tignorecount:[" << ignorecount++
                << "\t](" << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    if (systemExisted(sys)) {
        System &sysexist = this->operator [](
                getSystemID(make_pair(sys.systemName, sys.parentScopeAddr)));

        sysexist.constructorAddrList.insert(sys.constructorAddrList.begin(),
                sys.constructorAddrList.end());
        sysexist.systemName = sys.systemName;
        sysexist.decedentSystemIDList.insert(sys.decedentSystemIDList.begin(),
                sys.decedentSystemIDList.end());
        sysexist.parentScopeAddr = sys.parentScopeAddr;
        if (sys.systemAddr != addrnull) {
            sysexist.systemAddr = sys.systemAddr;
            addr_id_map[sysexist.systemAddr] = sysexist.id;
        }
        return;
    }
    ++id;
    id_system_map[id] = sys;
    id_system_map[id].id = id;
    name_iddq_map[sys.systemName].push_back(id);
    if (sys.systemAddr != addrnull) {
        addr_id_map[sys.systemAddr] = id;
    }
}
/**
 * @brief 判断system是否已经存在（声明过）同名、同父作用域
 * @param sys
 * @return
 */
bool SystemTable::systemExisted(const System &sys) {
    if (name_iddq_map.count(sys.systemName) > 0) {
        for (auto &id : name_iddq_map[sys.systemName]) {
            if (id_system_map[id].parentScopeAddr == sys.parentScopeAddr) {
                return true;
            }
        }

    } else {
        return false;
    }
    return false;
}
/**
 *
 * @param maychild (SYSB addr)
 * @param mayparent(SYSB addr)
 * @return
 */
bool SystemTable::isChild(const Addr &maychild, const Addr &mayparent) {
    auto &&itchild = addr_id_map.find(maychild);
    auto &&itparent = addr_id_map.find(mayparent);
    if (maychild < mayparent || maychild == addrnull
            || itchild == addr_id_map.end() || itparent == addr_id_map.end()) {
        return false;
    }

    if (id_system_map[itchild->second].decedentSystemIDList.count(
            itparent->second)) {
        return true;
    } else {
        for (auto id : id_system_map[itchild->second].decedentSystemIDList) {
            Addr &maychild_ = id_system_map[id].systemAddr;
            if (isChild(maychild_, mayparent) == true) {
                return true;

            }

        }
        return false;

    }

}
/**
 * @attention 禁止通过此函数在表中创建system实例。
 */
System& SystemTable::operator[](const Addr &addr) {
    if (addr_id_map.count(addr) > 0) {
        return id_system_map[addr_id_map[addr]];
    } else {
#if debug
        {
            static int ignorecount = 0;
            cerr << "operator [](const Addr&)"
                    << " cant find system by addr,exit(-1);"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }

}
System& SystemTable::operator[](const Intg &id) {
    return id_system_map[id];
}

/**
 * @attention 禁止通过此函数在表中创建system实例。
 * @param name_parentScopeAddr，给出当前所在父活动域以及system名称，返回满足要求的作用域的引用。
 * @return
 */
System& SystemTable::operator[](const pair<Strg, Addr> &name_parentScopeAddr) {
    const Strg &name = name_parentScopeAddr.first;
    const Addr &addr = name_parentScopeAddr.second;
    if (name_iddq_map.count(name) > 0) {
        auto &iddq = name_iddq_map[name];
        for (auto &id : iddq) {
            System &sys = id_system_map[id];
            if (scopeTable->accessible(addrmax, addr, sys.systemAddr)) {
                return sys;
            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "operator [](const pair<Strg,Addr>&)"
                    << " system unaccessible,exit(-1)" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);

    }

    else {
#if debug
        {
            static int ignorecount = 0;
            cerr << "operator [](const Addr&)"
                    << " cant find system by addr,exit(-1);"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }

}
System& SystemTable::getDirectSystemByCodeAddr(CodeTable *codeTable,
        const Addr &codeAddr) {
    Addr scopeAddr = (*codeTable)[codeAddr].scopeAddr;
    if (scopeAddr == addrnull) {
#if debug
        {
            static int ignorecount = 0;
            cout << "getDirectSystemByCodeAddr(CodeTable*, const Addr&)"
                    << " not in system" << "\tignorecount:[" << ignorecount++
                    << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        throw -1;
    } else {
        while (scopeAddr != addrnull) {

            if ((*scopeTable)[scopeAddr].functionFlag == SYSB) {
                return this->operator [](scopeAddr);
            }
            scopeAddr = (*scopeTable)[scopeAddr].parentScopeAddr;
        }
        //找不到说明也不在system中
        throw -1;
    }
}
Intg SystemTable::getSystemID(const pair<Strg, Addr> &name_parentScopeAddr) {
    const Strg &name = name_parentScopeAddr.first;
    const Addr &addr = name_parentScopeAddr.second;
    bool exist = name_iddq_map.count(name);
    if (exist) {
        for (auto &id : name_iddq_map[name]) {
            System &sys = id_system_map[id];
            if (scopeTable->accessible(addrmax, addr, sys.systemAddr)) {
                return id;
            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "getSystemID(const pair<Strg, Addr> &name_parentScopeAddr)"
                    << " system unaccessible,exit(-1)" << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);

    }

    else {
#if debug
        {
            static int ignorecount = 0;
            cerr << "getSystemID(const pair<Strg, Addr> &name_parentScopeAddr)"
                    << " cant find system by addr,exit(-1);"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        exit(-1);
    }

}
void SystemTable::erase(Intg id) {
    id_system_map.erase(id);
}
Strg SystemTable::toStrg() {
    stringstream ss;
    for (auto &sys : id_system_map) {
        ss << "[" << sys.first << "]\t" << sys.second.toStrg() << endl;
    }
    ss << endl;
    return ss.str();
}
/**
 * @brief 这个函数用于添加继承关系
 * @param childName 子名称
 * @param childParentScopeAddr 子系统体的父作用域
 * @param parentName 父名称
 */
void SystemTable::inherit(const Strg &childName,
        const Addr &childParentScopeAddr, const Strg &parentName) {
    System &child = this->operator [](
            make_pair(childName, childParentScopeAddr));
    Intg id = getSystemID(make_pair(parentName, childParentScopeAddr));
    child.decedentSystemIDList.insert(id);

}
}

#endif /* VM_HPP_ */
