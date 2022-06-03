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

#ifndef VM_INTEGRATE_HPP_
#define VM_INTEGRATE_HPP_

#include "vm_scan.hpp"
#include <set>
#include <cstring>
#include <algorithm>

#define MODE_DEFAULT 0   //不提供cdtdes，直接整合到cdtmain中
#define MODE_INTEGRATE 0   //不提供cdtdes，直接整合到cdtmain中
#define MODE_SEPARATE 1  //提供cdtdes，只对cdtdes匹配，不将cdtdes整合到cdtmain中

using namespace std;
namespace VM {
/**
 * @param cdtref:函数名作用域中的树,仅用于与未绑定树进行匹配
 * @param cdtfrom：将整合入cdtmid中的树，当integrate为否时，为函数名作用域内的树；为是时，则为函数体作用域返回的树
 * @param cdtmid：储存在silo中，从silo中选择满足要求的函数表将其整合入cdtmain中
 * @param cdtmain：一次代码表，完全整合后成为二次代码表
 */

class Matcher;
class SingleTFIntegrator;
class MultiTFIntegrator;
CodeTable codeTablenull = CodeTable();
/**
 * @attention copytree不会将Y类型代码一并拷贝，引用对应的代码段必须位于一段表达式对应代码段的前部。
 * @param ad
 * @return
 */
CodeTable CodeTable::copyTree(const Addr &ad) const {

    Intg pos = getAddrPosition(addrdq, ad);

    if ((codedq[pos].assemblyFormula.flagBit.type == M)
            && codedq[pos].assemblyFormula.flagBit.execute != Du) {

        CodeTable cdt = CodeTable();
        cdt.addrdq.push_back(addrdq[pos]);
        cdt.codedq.push_back(codedq[pos]);
        Intg pos_to_top = pos - 1;

        while (pos_to_top >= 0

        && (codedq[pos_to_top].assemblyFormula.flagBit.type == M

        || codedq[pos_to_top].assemblyFormula.flagBit.execute == Du)) {
            if (codedq[pos_to_top].assemblyFormula.flagBit.execute != Du) {
                cdt.addrdq.push_front(addrdq[pos_to_top]);
                cdt.codedq.push_front(codedq[pos_to_top]);
            }
            pos_to_top--;
        }

        Intg pos_to_root = pos + 1;
        while (pos_to_root < (Intg) addrdq.size()

        && (codedq[pos_to_root].assemblyFormula.flagBit.type == M

        || codedq[pos_to_root].assemblyFormula.flagBit.execute == Du)) {
            if (codedq[pos_to_root].assemblyFormula.flagBit.execute != Du) {
                cdt.addrdq.push_back(addrdq[pos_to_root]);
                cdt.codedq.push_back(codedq[pos_to_root]);
            }
            pos_to_root++;
        }
        return cdt;
    }
    return codeTablenull;
}
/**
 * @brief 将codeTable中出现的所有类型为Y_Dz的变量的arg_ar设置为ar
 * @param ar
 */
void CodeTable::setRefArgAR(DataTable *ar) {

    for (Code &cd : codedq) {

        if (cd.assemblyFormula.flagBit.arg1_flag == Y_Dz) {
            cd.assemblyFormula.quaternion.arg1_ar = ar;
        }
        if (cd.assemblyFormula.flagBit.arg2_flag == Y_Dz) {
            cd.assemblyFormula.quaternion.arg2_ar = ar;
        }
        if (cd.assemblyFormula.flagBit.operator_flag == Y_Dz) {
            cd.assemblyFormula.quaternion.operator_ar = ar;
        }
        if (cd.assemblyFormula.flagBit.result_flag == Y_Dz) {
            cd.assemblyFormula.quaternion.result_ar = ar;
        }

    }

}
/**
 * copy[adLow,admax]
 * @param adLow
 * @param adUp
 * @return
 */
CodeTable CodeTable::copyCodeTable(const Addr &adLow, const Addr &adUp) const {

    Intg posMin = getAddrPosition(addrdq, adLow);
    Intg posMax = getAddrPosition(addrdq, adUp);
    CodeTable cdt = CodeTable();
    while (posMin <= posMax) {
        cdt.addrdq.push_back(const_cast<CodeTable*>(this)->addrdq[posMin]);
        cdt.codedq.push_back(const_cast<CodeTable*>(this)->codedq[posMin]);
        posMin++;

    }
    return cdt;
}
CodeTable CodeTable::copyTreeBranch(const Addr &ad) const {

    Intg pos = getAddrPosition(addrdq, ad);

    if (codedq[pos].assemblyFormula.flagBit.type == M
            && codedq[pos].assemblyFormula.flagBit.execute != Du) {

        CodeTable cdt = CodeTable();

        set<Addr> treeNode;
        set<Addr> cdtNode;
        treeNode.insert(ad);
        while (treeNode.size() > 0) {
            auto it = treeNode.begin();
            Addr currentCodeAddr = *it;
            Code &currentCode = (*const_cast<CodeTable*>(this))[currentCodeAddr];
            treeNode.erase(it);
            cdtNode.insert(currentCodeAddr);
            cdt.addrdq.push_front(currentCodeAddr);
            cdt.codedq.push_front(currentCode);
            Arg &&arg0 = (currentCode)[0];
            Arg &&arg1 = (currentCode)[1];
            Arg &&arg2 = (currentCode)[2];
            Arg &&arg3 = (currentCode)[3];
            if (arg0.argFlag == S_Dz) {
                pair<Addr, Intg> &&addr_intg_leaf =
                        (*const_cast<CodeTable*>(this)).getSameTrBranchLastArgPositionIgnoreProperty(
                                currentCodeAddr, arg0);
                if (addr_intg_leaf.first != addrnull) {
                    treeNode.insert(addr_intg_leaf.first);
                }

            }
            if (arg1.argFlag == S_Dz) {
                pair<Addr, Intg> &&addr_intg_leaf =
                        (*const_cast<CodeTable*>(this)).getSameTrBranchLastArgPositionIgnoreProperty(
                                currentCodeAddr, arg1);
                if (addr_intg_leaf.first != addrnull) {
                    treeNode.insert(addr_intg_leaf.first);
                }

            }
            if (arg2.argFlag == S_Dz) {
                pair<Addr, Intg> &&addr_intg_leaf =
                        (*const_cast<CodeTable*>(this)).getSameTrBranchLastArgPositionIgnoreProperty(
                                currentCodeAddr, arg2);
                if (addr_intg_leaf.first != addrnull) {
                    treeNode.insert(addr_intg_leaf.first);
                }

            }
            if (arg3.argFlag == S_Dz) {
                pair<Addr, Intg> &&addr_intg_leaf =
                        (*const_cast<CodeTable*>(this)).getSameTrBranchLastArgPositionIgnoreProperty(
                                currentCodeAddr, arg3);
                if (addr_intg_leaf.first != addrnull) {
                    treeNode.insert(addr_intg_leaf.first);
                }

            }

        }
        for (auto it : cdtNode) {
            cdt.addrdq.push_back(it);
            cdt[it] = (*const_cast<CodeTable*>(this))[it];
        }
        return cdt;
    }
    return codeTablenull;
}

Data::Data(const Arg &argName) {
    if (argName.isDz()) {
        dataFlagBit = argName.argFlag;

    } else if (argName.isBs()) {
        dataFlagBit = S_Bs;

    } else if (argName.argFlag == S_AR) {
        dataFlagBit = S_AR;
    }
}
Data::Data(const Numb &num) {
    dataFlagBit = Sz;
    content_i = num;
}
Data::Data(const Strg &strg) {
    dataFlagBit = Zf;
    content_s = strg;
}
Strg Data::toStrg() const {
    stringstream ss;
    ss << "Data:[" << "dataFlagBit:" << dataFlagBit << "\t" << "addrFlagBit:"
            << addrFlagBit << "\t" << "content_i:" << content_i << "\t"
            << "content_s:" << content_s << "\t" << "content_a:"
            << content_a.toStrg() << "\t"

            << "] ";
    return ss.str();
}
Data* copy(Data *to, const Data &from) {
    to = new Data();
    *to = from;
    return to;
}
Data* copy(Data *to, const Data *const from) {
    to = new Data();
    *to = *from;
    return to;
}

DataTable::DataTable(DataTable *parentAR, DataTable *queryAR,
        const Addr &scopeStructureAddr) {
    this->parentAR = parentAR;
    if (queryAR) {
        this->queryARList.push_back(queryAR);
    }
    this->scopeStructureAddr = scopeStructureAddr;
}
DataTable::DataTable(DataTable *parentAR, DataTable *queryAR,
        DataTable *returnAR, const Addr &scopeStructureAddr, const Intg &type) {
    this->parentAR = parentAR;
    if (queryAR) {
        this->queryARList.push_back(queryAR);
    }
    this->returnAR = returnAR;
    this->scopeStructureAddr = scopeStructureAddr;
    this->type = type;
}
void DataTable::setFRMap(const map<ArgFormal, ArgReal> &frmap) {
    this->frmap = frmap;
    return;
}
DataTable& DataTable::operator=(const DataTable &dt) {
    admap = dt.admap;
    sdmap = dt.sdmap;
    scopeStructureAddr = dt.scopeStructureAddr;
    meetAddr = dt.meetAddr;
    parentAR = dt.parentAR;
    returnAR = dt.returnAR;
    queryARList = dt.queryARList;

    return (*this);
}
bool DataTable::operator==(DataTable &dt) const {

    const map<Addr, Data> &dtadmap = dt.admap;
    const map<Strg, Data> &dtsdmap = dt.sdmap;
    const map<ArgFormal, ArgReal> &dtfrmap = dt.frmap;
    const Addr &dtscopeStructureAddr = dt.scopeStructureAddr;
    const Addr &dtmeetAddr = dt.meetAddr;
    return admap ==

    dtadmap && sdmap ==

    dtsdmap && scopeStructureAddr ==

    dtscopeStructureAddr && meetAddr ==

    dtmeetAddr && frmap == dtfrmap;
}
/**
 * @name findArgInAR
 * @param arg
 * @attention 引用不考虑引用目标
 * @return 查询范围只包括当前AR（包括frmap），考虑formalarg 标志位（formalarg为假直接返回否）
 */
bool findArgInAR(const DataTable *ar, const Arg &arg) {
#if debug
    static Intg ignorecount = 0;
    cout << "findArgInAR arg1:[" << arg.toStrg() << "]\targ:[" << arg.toStrg()
            << "]\tignorecount:[" << ignorecount++ << "]" << endl;
#endif

    if (arg.isVar() == false) {

        return true;
    } else if (arg.argFlag == Z_Dz) {
        Arg arg_;
        arg_ = arg;
        arg_.argFlag = S_AR;
        return findArgInAR(ar, arg_);
    }
    if (arg.formalArg == F_) {
        return false;
    }
    if (arg.isDz()) {
#if debug
        cout << "findArgInAR arg2:[" << arg.toStrg() << "]" << endl;
#endif
        auto iter = ar->admap.find(arg.arg_a);
        if (iter != ar->admap.end()) {
            return true;
        }

    } else if (arg.argFlag == S_Bs) {
#if debug
        cout << "findArgInAR arg3:[" << arg.toStrg() << "]" << endl;
#endif
        auto iter = ar->sdmap.find(arg.arg_s);
        if (iter != ar->sdmap.end()) {
            return true;
        }

    }
#if debug
    {
        static int ignorecount = 0;
        cout << "findArgInAR(const DataTable*, const Arg&)" << " "
                << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    auto iter1 = ar->frmap.find(arg);
    if (iter1 != ar->frmap.end()) {
        return true;
    }
    return false;

}
/**
 * @name findcurrent
 * @param arg
 * @return whether arg exit in THIS(current) ar,include frmap ,not include parentAR and queryAR
 */
bool DataTable::findcurrent(const Arg &arg) {
    return findArgInAR(this, arg);
}

bool DataTable::findall(const Arg &arg) {
#if debug
    {
        static int ignorecount = 0;
        cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    if (findcurrent(arg) == true) {
        return true;
    }
//    if (arg.formalArg == false || arg.asc != this->scopeStructureAddr) {
#if debug
    {
        static int ignorecount = 0;
        cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    Arg arg2 = arg;
    arg2.formalArg = true;
    deque<deque<DataTable*>*> queryARList_deque;
    queryARList_deque.push_back(&queryARList);
    set<DataTable*> checkedAR; //不重复检索AR
#if debug
    {
        static int ignorecount = 0;
        cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    while (queryARList_deque.size() > 0) {
#if debug
        {
            static int ignorecount = 0;
            cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        deque<DataTable*> *queryARList_ = queryARList_deque.front();
        queryARList_deque.pop_front();
        auto it = queryARList_->begin();
        for (auto ar = *it; it != queryARList_->end(); ++it) {
#if debug
            {
                static int ignorecount = 0;
                cout << "DataTable::findall(const Arg&)" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            if (checkedAR.count(ar)) {
                continue;
            }
#if debug
            {
                static int ignorecount = 0;
                cout << "DataTable::findall(const Arg&)" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
            checkedAR.insert(ar);
            if (ar != NULL) {
                if (ar->findcurrent(arg2) == true) {
#if debug
                    {
                        static int ignorecount = 0;
                        cout << "DataTable::findall(const Arg&)" << " "
                                << "\tignorecount:[" << ignorecount++ << "\t]("
                                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                                << __LINE__ << ":0" << ")" << endl;
                    }
#endif
                    return true;
                }
                queryARList_deque.push_back(&(ar->queryARList));
            }
#if debug
            {
                static int ignorecount = 0;
                cout << "DataTable::findall(const Arg&)" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
            }
#endif
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
    }
#if debug
    {
        static int ignorecount = 0;
        cout << "DataTable::findall(const Arg&)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    return false;

//    }
//    return false;

}
/**
 * @brief findall的改进版，获得arg所在scope的Addr，如果无法检索到变量，则抛出false异常。
 * @param arg
 * @return
 * @throw 立即数将throw
 */
Addr DataTable::getArgScopeAddr(const Arg &arg) {
#if debug
    {
        static int ignorecount = 0;
        cout << "DataTable::getArgScopeAddr(const Arg&)" << " "
                << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

    if (findcurrent(arg) == true) {
        return this->scopeStructureAddr;
    }
//    if (arg.formalArg == false || arg.asc != this->scopeStructureAddr) {
    Arg arg2 = arg;
    arg2.formalArg = true;
    deque<deque<DataTable*>*> queryARList_deque;
    queryARList_deque.push_back(&queryARList);
    set<DataTable*> checkedAR; //不重复检索AR
    while (queryARList_deque.size() > 0) {

        deque<DataTable*> *queryARList_ = queryARList_deque.front();
        queryARList_deque.pop_front();
        auto it = queryARList_->begin();
        for (auto ar = *it; it != queryARList_->end(); ++it) {
            if (checkedAR.count(ar)) {
                continue;
            }
            checkedAR.insert(ar);
            if (ar != NULL) {
                if (ar->findcurrent(arg2) == true) {
                    return ar->scopeStructureAddr;
                }
                queryARList_deque.push_back(&(ar->queryARList));
            }
        }
    }

#if debug
    {
        static int ignorecount = 0;
        cout << "DataTable::getArgScopeAddr(const Arg&)"
                << " can't find arg,throw arg" << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    throw arg;
//    }
//    return false;

}
/**
 * @attention 引用不考虑引用目标
 * @param arg
 * @return
 */
DataTable* DataTable::getArgAR(const Arg &arg) {

    if (findcurrent(arg) == true) {
        return this;
    }
//    if (arg.formalArg == false || arg.asc != this->scopeStructureAddr) {
    Arg arg2 = arg;
    arg2.formalArg = true;
    deque<deque<DataTable*>*> queryARList_deque;
    queryARList_deque.push_back(&queryARList);
    set<DataTable*> checkedAR; //不重复检索AR
    while (queryARList_deque.size() > 0) {

        deque<DataTable*> *queryARList_ = queryARList_deque.front();
        queryARList_deque.pop_front();
        auto it = queryARList_->begin();
        for (auto ar = *it; it != queryARList_->end(); ++it) {
            if (checkedAR.count(ar)) {
                continue;
            }
            checkedAR.insert(ar);
            if (ar != NULL) {
                if (ar->findcurrent(arg2) == true) {
                    return ar;
                }
                queryARList_deque.push_back(&(ar->queryARList));
            }
        }
    }
#if debug
    {
        static int ignorecount = 0;
        cout << "DataTable::getArgAR(const Arg&)" << " can't find AR"
                << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    return NULL;

//    }
//    return false;

}
Addr DataTable::setArgScopeAddr(Arg &arg) {
    if (arg.argFlag == M_Bs) {
        return addrnull;
    }
    if (arg.argFlag == X_Bs) {
        return addrnull;
    }
    if (arg.argFlag == Z_Dz) {
        arg.argFlag = S_AR;
    }
    DataTable *dt = getArgAR(arg);
    if (dt) {
        arg.asc = dt->scopeStructureAddr;
        return arg.asc;
    } else {
#if debug
        cerr << "DataTable::setArgScope err , can't find arg!" << endl;
#endif
        return addrnull;
    }
}
/**
 * @brief 获得当前ar可以访问到的ar，可以是自身、父ar、queryar或returnar
 * @param parentScopeAddr
 * @return
 */
DataTable* DataTable::getAccessibleAR(const Addr &arScopeAddr) {

//    set<DataTable*> checkedAR; //
//    set<DataTable*> checkingAR;
//    set<DataTable*> candidateAR;
    map<DataTable*, Intg> checkedAR;
    map<DataTable*, Intg> checkingAR;
    map<DataTable*, Intg> candidateAR;
    checkingAR.insert(make_pair(this, 0));

    while (true) {
        for (auto ar_it : checkingAR) {
            auto ar = ar_it.first;
            if (checkedAR.count(ar)) {
                continue;

            } else {
                if (ar && ar->scopeStructureAddr == arScopeAddr) {
                    return ar;
                } else if(ar) {
                    candidateAR.insert(make_pair(ar->parentAR, 0));
                    candidateAR.insert(make_pair(ar->returnAR, 0));
                    for (auto queryAR : ar->queryARList) {
                        candidateAR.insert(make_pair(queryAR, 0));
                    }
                    checkedAR.insert(make_pair(ar, 0));
                }
            }

        }
        for (auto checkedAR_ : checkedAR) {

            candidateAR.erase(checkedAR_.first);
        }
        if (candidateAR.size() > 0) {
            checkingAR.clear();
            candidateAR.swap(checkingAR);
            checkingAR.erase(NULL);
        } else {
            return NULL;
        }

    }
}
//Addr DataTable::getArgScopeAddr(const Arg &arg) {
//#if debug
//    cout << "DataTable::getArgScopeAddr arg:[" << arg.toStrg() << "]" << endl;
//#endif
//    if (arg.argFlag == M_Bs || arg.argFlag == Sz || arg.argFlag == X_Bs) {
//        return Addr();
//
//    } else if (arg.argFlag == Z_Dz) {
//        Arg arg_(arg);
//        arg_.argFlag = S_AR;
//        return getArgScopeAddr(arg_);
//    }
//    DataTable *dt = getArgAR(arg);
//    if (dt) {
//        return dt->scopeStructureAddr;
//
//    } else {
//#if debug
//        cout << "DataTable::getArgScope err , can't find arg!\tArg:["
//                << arg.toStrg() << "]\t" << endl;
//#endif
//        return Addr();
//    }
//}
/**
 * @brief 将一个以arg为key的元组添加到ar中，元组的值为默认值
 * @param arg
 * @return
 */
bool DataTable::add(const Arg &arg) {
//need_modify
    if (arg.argFlag == X_Bs) {
        return true;
    }

    if (arg.argFlag == M_Bs || arg.argFlag == Sz || arg.argFlag == Zf) {
        return true;
    }
    if (arg.formalArg == false) {
        return true;

    }
    if (arg.argFlag == Z_Dz) {
#if debug
        cout << "DataTable::add1" << endl;
#endif
        Arg arg_;
        arg_ = arg;
        arg_.argFlag = S_AR;
#if debug
        cout << "DataTable::add2" << endl;
#endif
        return DataTable::add(arg_);
    }
#if debug
    cout << "DataTable::add ,Arg:[" << arg.toStrg() << "]\t" << endl;
#endif
//@attention 函数声明中的引用默认不是形式参数
    if (arg.formalArg == true && arg.argFlag != Y_Dz
            && findcurrent(arg) == true) {
        return true;
    } else if (arg.formalArg == true && findcurrent(arg) == false) {
        if (arg.argFlag == S_Dz) {
            admap[arg.arg_a] = Data(arg);
            admap[arg.arg_a].dataFlagBit = S_Dz;
        } else if (arg.argFlag == Y_Dz) {
            admap[arg.arg_a] = Data(arg);
            admap[arg.arg_a].dataFlagBit = Y_Dz;
            admap[arg.arg_a].content_ar = arg.arg_ar;
        } else if (arg.argFlag == S_Bs) {
            sdmap[arg.arg_s] = Data(arg);
            sdmap[arg.arg_s].dataFlagBit = S_Bs;
        } else if (arg.argFlag == S_AR) {
            admap[arg.arg_a] = Data(arg);
            admap[arg.arg_a].dataFlagBit = S_AR;
        }
        return true;
    }
#if debug
    cerr << "DataTable::add err,Arg:[" << arg.toStrg() << "]\t" << endl;
#endif
    return false;

}
/*Data& DataTable::operator[](const Addr& addr) {

 if (addr == addrnull) {
 #if debug
 cerr << "DataTable::operator[] err: addrnull" << endl;
 #endif
 }
 DataTable* ar = this;
 Arg arg(S_Dz, addr);
 if (findall(arg) == false) {
 arg.argFlag = S_AR;
 }
 if (findall(arg) == false) {
 return Data().DataNull();
 #if debug
 cerr << "DataTable::operator[Addr] err: can't find Data. Addr:["
 << addr.toStrg() << "]" << endl;
 #endif
 }
 while (ar != NULL) {
 if ((ar->findcurrent(arg)) != true) {
 ar = ar->queryAR;
 } else {
 return ar->admap[addr];
 }

 }

 #if debug
 cerr << "DataTable::operator[Addr] err: can't find Data. Addr:["
 << addr.toStrg() << "]" << endl;
 #endif
 return datanull;

 }*/
Data& DataTable::operator[](const Addr &addr) {
    Data &data = (*this)[Arg(S_Dz, addr)];
    /*        if (data.dataFlagBit == Y_Dz) {
     return globalRefTable.getDataRef(Arg(Y_Dz, data.content_a, this));

     } else
     return data;*/
    return data;
}
/*Data& DataTable::operator[](const Strg& name) {
 DataTable* ar = this;
 Arg arg(S_Bs, name);
 if (findall(arg) == false) {
 return Data().DataNull();
 #if debug
 cerr << "DataTable::operator[Strg:name] err: can't find Data. name:["
 << name << "]" << endl;
 #endif
 }
 while (ar != NULL) {
 if ((ar->findcurrent(arg)) != true) {
 ar = ar->queryAR;
 } else {
 return ar->sdmap[name];
 }

 }

 #if debug
 cerr << "DataTable::operator[Strg:name] err: can't find Data. name:["
 << name << "]" << endl;
 #endif
 return datanull;
 }*/
Data& DataTable::operator[](const Strg &name) {
    return (*this)[Arg(S_Bs, name)];
}
/**
 * 虽然返回的也是引用，但是和通常[key]运算符的差别是：
 * 如果key不存在，会报错，因此在使用key修改值时需要先使用add 添加元组
 * @param arg
 * @return
 */
Data& DataTable::operator[](const Arg &arg) {
    if (arg.argFlag == Y_Dz) {
        return globalRefTable.getDataRef(arg);
    }
    if (arg.argFlag == Z_Dz) {
        Arg arg_ = arg;
        arg_.argFlag = S_Dz;
        return (*this)[arg_];
    }
    if (arg.argFlag == Sz || arg.argFlag == Zf) {
#if debug
        cout << "DataTable::operator[] warm 0 , arg is Sz||Zf ,arg:["
                << arg.toStrg() << "]" << endl;
#endif
        throw arg;
    }
    {
#if debug
        static Intg ignorecount = 0;
        cout << "DataTable::operator[] in , arg:[" << arg.toStrg()
                << "]\tignorecount:[" << ignorecount++ << "]" << endl;
#endif
    }
    if (findall(arg) == false) {
#if debug
        cerr << "DataTable::operator[] err 1,exit(-1),arg:[" << arg.toStrg()
                << "]" << endl;
#endif
        exit(-1);

    }
#if debug
    {
        static int ignorecount = 0;
        cout << "DataTable::operator [](const Arg&)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    if (findcurrent(arg) == true) {
        if (arg.isDz()) {
            if (admap.find(arg.arg_a) != admap.end()) {
                return admap[arg.arg_a];
            }
        } else if (arg.isBs()) {
            if (sdmap.find(arg.arg_s) != sdmap.end()) {
                return sdmap[arg.arg_s];
            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "DataTable::operator [](const Arg&)" << " "
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if (frmap.find(arg) != frmap.end()) {

            Arg &argr = frmap[arg];
            if (argr.argFlag == Sz || argr.argFlag == Zf) {

#if debug
                cout << "DataTable::operator[] warm 1 , arg is Sz||Zf ,arg:["
                        << arg.toStrg() << "]" << endl;
#endif
#if debug
                {
                    static int ignorecount = 0;
                    cout << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "vm_integrate.hpp:" << __LINE__ << ":0" << ")"
                            << endl;
                }
#endif
                throw argr;
            } else {
#if debug
                {
                    static int ignorecount = 0;
                    cout << "DataTable::operator [](const Arg&)" << " "
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
                DataTable *argr_locate_ar = this->getAccessibleAR(argr.asc);
                if (argr_locate_ar) {
                    return (*argr_locate_ar)[argr];
                } else {
                    throw argr;
                }

            }

        }

    }

    Arg arg2 = arg;
    if (arg2.argFlag == Z_Dz) {
        arg2.argFlag = S_Bs;
    }
    arg2.formalArg = true;
    deque<deque<DataTable*>*> queryARList_deque;
    queryARList_deque.push_back(&queryARList);
    //这里应该用set<DataTable*>的，但是使用set后无法进入此函数进行调试，所以用map先代替了
    std::map<DataTable*, Intg> checkedAR; //不重复检索AR
    while (queryARList_deque.size() > 0) {

        deque<DataTable*> *queryARList_ = queryARList_deque.front();
        queryARList_deque.pop_front();
        auto it = queryARList_->begin();
        for (auto ar = *it; it != queryARList_->end(); ++it) {
            if (checkedAR.count(ar)) {
                continue;
            }
            checkedAR.insert(make_pair(ar, 0));
            if (ar != NULL) {
                if (ar->findcurrent(arg2) == true) {

                    if (arg2.argFlag == S_Dz || arg2.argFlag == S_AR) {
                        if (ar->admap.find(arg2.arg_a) != ar->admap.end()) {
                            return ar->admap[arg2.arg_a];
                        }
                    } else if (arg2.argFlag == S_Bs) {
                        if (ar->sdmap.find(arg2.arg_s) != ar->sdmap.end()) {
                            return ar->sdmap[arg2.arg_s];
                        }
                    } else {
                        //todo: other argFlag
                    }
                    if (frmap.find(arg2) != frmap.end()) {
                        if (parentAR) {
                            Arg &argr = frmap[arg2];
                            if (argr.argFlag == Sz || argr.argFlag == Zf) {
#if debug
                                cout
                                        << "DataTable::operator[] warm 2 , arg is Sz||Zf ,argr:["
                                        << argr.toStrg() << "]" << endl;
#endif
                                throw argr;
                            } else {
                                return (*parentAR)[argr];
                            }
                        }
                    }

                } else {
                    queryARList_deque.push_back(&(ar->queryARList));
                }

            }
        }
    }

#if debug
    cerr << "DataTable::operator[] err 3,exit(-1)" << endl;
#endif
    exit(-1);
}
Data& Data::operator=(const Data &data_) {
    dataFlagBit = data_.dataFlagBit;
    addrFlagBit = data_.addrFlagBit;
    content_i = data_.content_i;
    content_s = data_.content_s;
    content_a = data_.content_a;
    if (data_.dataFlagBit == S_AR) {
        if (data_.content_ar != NULL)

            content_ar = data_.content_ar;
    }

//    scopeStructureAddr = data_.scopeStructureAddr;
    linkAddr = data_.linkAddr;
    return (*this);
}
Data& Data::operator=(const Arg &arg) {
    if (arg.argFlag == S_Dz) {
        dataFlagBit = S_Dz;
        content_a = arg.arg_a;

    } else if (arg.argFlag == S_Bs) {
        dataFlagBit = S_Bs;
        content_s = arg.arg_s;
    } else if (arg.argFlag == Zf) {
        dataFlagBit = Zf;
        content_s = arg.arg_s;
    } else if (arg.argFlag == Sz) {
        dataFlagBit = Sz;
        content_i = arg.arg_i;
    }
    if (arg.argFlag == S_AR) {
        dataFlagBit = S_AR;
        safe_delete(content_ar);
        if (arg.arg_ar) {
            content_ar = new DataTable();
            *content_ar = *arg.arg_ar;
        }

    }

    return (*this);
}
const bool Data::operator==(const Data &data_) const {
    return dataFlagBit == data_.dataFlagBit && addrFlagBit == data_.addrFlagBit
            && content_i == data_.content_i && content_s == data_.content_s
            && content_a == data_.content_a && content_ar == data_.content_ar
//            && scopeStructureAddr == data_.scopeStructureAddr
            && linkAddr == data_.linkAddr;

    return true;
}

typedef Addr AddrCmp;
typedef Addr AddrRef;
typedef ArgReal ArgCmp;
typedef Arg ArgRef;
class MatchStateSilo;
class MatchState;
class MatchState {
private:
    MatchState() {
    }
    ;
public:

    CodeTable *cdtcmp = NULL;
    CodeTable *cdtref = NULL;
    vector<AddrCmp> addrcmpv;
    vector<Intg> argposcmpv;
    vector<AddrRef> addrrefv;
    vector<Intg> argposrefv;

    map<ArgCmp, deque<ArgRef> > cram;
    map<ArgRef, pair<ArgCmp, AddrCmp> > rcam;
    map<AddrCmp, AddrRef> addrm;
    MatchState(CodeTable *cdtcmp, CodeTable *cdtref) {
        this->cdtcmp = cdtcmp;
        this->cdtref = cdtref;
    }
    MatchState(const MatchState &ms) {
        cdtcmp = ms.cdtcmp;
        cdtref = ms.cdtref;
        addrcmpv = ms.addrcmpv;
        argposcmpv = ms.argposcmpv;
        addrrefv = ms.addrrefv;
        argposrefv = ms.argposrefv;
        cram = ms.cram;
        rcam = ms.rcam;
        addrm = ms.addrm;
    }
    MatchState& operator=(const MatchState &ms) {
        addrcmpv = ms.addrcmpv;
        argposcmpv = ms.argposcmpv;
        addrrefv = ms.addrrefv;
        argposrefv = ms.argposrefv;
        cram = ms.cram;
        addrm = ms.addrm;
        return (*this);
    }

    const bool finished() const {
        return argposrefv.size() == 0 && addrm.size() > 0;
    }
    void pop(AddrCmp &addrcmp, Intg &argposcmp, Addr &addrref,
    Intg &argposref) {
        addrcmp = addrcmpv.back();
        argposcmp = argposcmpv.back();
        addrref = addrrefv.back();
        argposref = argposrefv.back();
        addrcmpv.pop_back();
        argposcmpv.pop_back();
        addrrefv.pop_back();
        argposrefv.pop_back();
    }
    void push(const AddrCmp &addrcmp, const Intg &argposcmp,
            const AddrRef &addrref, const Intg &argposref) {
        addrcmpv.push_back(addrcmp);
        argposcmpv.push_back(argposcmp);
        addrrefv.push_back(addrref);
        argposrefv.push_back(argposref);

    }
    /**
     * @brief 比较两个参数是否在事实上等价
     * @param argcmp
     * @param argref
     * @return
     */
    bool sameOneArg(const ArgCmp &argcmp, const ArgRef &argref) const {
        if (/*argref.formalArg == argref.formalArg &&*/argcmp.asc == argref.asc
                && argcmp == argref) {
            return true;
        }
        return false;
    }
    const bool add(const AddrCmp &addrcmp, const AddrRef &addrref) {
        if (addrref > addrnull) {
            if (addrm[addrcmp] == addrref) {
                return true;
            } else if (addrm[addrcmp] == addrnull) {
                addrm[addrcmp] = addrref;
                return true;
            } else {
                return false;
            }
        } else if (addrref < addrnull) {
            addrm[addrcmp] = addrref;
            return true;
        } else {
            return false;
        }
    }
    /**
     * @brief 这个函数用于判断以argcmp1和argcmp2为根节点的两个子树的结构是否相同。
     * 即子叶结点的（标识符类）参数相同（sameOneArg），子叶结点间的运算符也相同，
     * 允许非子叶结点的（地址类）参数不同。
     * @param root1
     * @param root2
     * @param rootaddr1
     * @param rootaddr2
     * @return
     */
    const bool sameTreeStruct(const ArgCmp &root1, const ArgCmp &root2,
            const AddrCmp &rootaddr1, const AddrCmp &rootaddr2) {
        //todo:: logic here
        if (root1.argFlag != root2.argFlag) {
            return false;
        }
        if (root1.argFlag == M_Bs && root2.argFlag == M_Bs) {
            return root1 == root2;
        }
        if (root1.argFlag == Sz && root2.argFlag == Sz) {
            return root1.arg_i == root2.arg_i;
        }
        if (root1.argFlag == Zf && root2.argFlag == Zf) {
            return root1.arg_s == root2.arg_s;
        }
        if (root1.argFlag == S_Bs && root2.argFlag == S_Bs) {
            return sameOneArg(root1, root2);
        } else {
            //root1.argFlag ==S_Dz && root2.argFlag == S_Dz
            Addr leafaddr1 = rootaddr1;
            while (!((*cdtcmp)[leafaddr1][3] == root1)) {
                leafaddr1 = cdtcmp->getSameTrBranchLastArgAddrIgnoreProperty(
                        leafaddr1, root1);
                if (leafaddr1 == addrnull) {
#if debug
                    cerr
                            << "sameTreeStruct err: can't find leafaddr1 definition, exit(-1)\n"
                                    "\trootaddr1:[" << rootaddr1.toStrg()
                            << "\t]\n"
                                    "\troot1:[" << root1.toStrg() << "\t]\n"
                                    "rootaddr2:[" << rootaddr2.toStrg()
                            << "\t]\n"
                                    "root2:[" << root2.toStrg() << "\t]\n"
                            << endl;
#endif
                    exit(-1);
                }

            }
            Addr leafaddr2 = rootaddr2;
            while (!((*cdtcmp)[leafaddr2][3] == root2)) {
                leafaddr2 = cdtcmp->getSameTrBranchLastArgAddrIgnoreProperty(
                        leafaddr2, root2);
                if (leafaddr2 == addrnull) {
#if debug
                    cerr
                            << "sameTreeStruct err: can't find leafaddr2 definition\n"
                                    "\trootaddr1:[" << rootaddr1.toStrg()
                            << "\t]\n"
                                    "\troot1:[" << root1.toStrg() << "\t]\n"
                                    "rootaddr2:[" << rootaddr2.toStrg()
                            << "\t]\n"
                                    "root2:[" << root2.toStrg() << "\t]\n"
                            << endl;
#endif
                    exit(-1);
                }

            }
            Code &leafcode1 = (*cdtcmp)[leafaddr1];
            Arg &&leaf1_0 = leafcode1[0];
            Arg &&leaf1_1 = leafcode1[1];
            Arg &&leaf1_2 = leafcode1[2];
            Code &leafcode2 = (*cdtcmp)[leafaddr2];
            Arg &&leaf2_0 = leafcode2[0];
            Arg &&leaf2_1 = leafcode2[1];
            Arg &&leaf2_2 = leafcode2[2];
            bool same0 = false;
            same0 = sameTreeStruct(leaf1_0, leaf2_0, leafaddr1, leafaddr2);
            if (!same0) {
#if debug
                {
                    static int ignorecount = 0;
                    cout
                            << "sameTreeStruct(const ArgCmp&, const ArgCmp&, const AddrCmp&, const AddrCmp&)"
                            << " "
                                    "same1 false \n"
                                    "leaf1_0:[" << leaf1_0.toStrg() << "\t]\n"
                                    "leaf2_0:[" << leaf2_0.toStrg() << "\t]\n"
                                    "leafaddr1:[" << leafaddr1.toStrg()
                            << "\t]\tleafaddr2:[" << leafaddr2.toStrg()
                            << "\t]\t\n" << "\tignorecount:[" << ignorecount++
                            << "\t](" << "vm_integrate.hpp:" << __LINE__ << ":0"
                            << ")" << endl;
                }
#endif
                return false;
            }
            bool same1 = false;
            same1 = sameTreeStruct(leaf1_1, leaf2_1, leafaddr1, leafaddr2);
            if (!same1) {
#if debug
                {
                    static int ignorecount = 0;
                    cout
                            << "sameTreeStruct(const ArgCmp&, const ArgCmp&, const AddrCmp&, const AddrCmp&)"
                            << " "
                                    "same1 false \n"
                                    "leaf1_0:[" << leaf1_1.toStrg() << "\t]\n"
                                    "leaf2_0:[" << leaf2_1.toStrg() << "\t]\n"
                                    "leafaddr1:[" << leafaddr1.toStrg()
                            << "\t]\tleafaddr2:[" << leafaddr2.toStrg()
                            << "\t]\t\n" << "\tignorecount:[" << ignorecount++
                            << "\t](" << "vm_integrate.hpp:" << __LINE__ << ":0"
                            << ")" << endl;
                }
#endif
                return false;
            }
            bool same2 = false;
            same2 = sameTreeStruct(leaf1_2, leaf2_2, leafaddr1, leafaddr2);
            if (!same2) {
#if debug
                {
                    static int ignorecount = 0;
                    cout
                            << "sameTreeStruct(const ArgCmp&, const ArgCmp&, const AddrCmp&, const AddrCmp&)"
                            << " "
                                    "same1 false \n"
                                    "leaf1_0:[" << leaf1_2.toStrg() << "\t]\n"
                                    "leaf2_0:[" << leaf2_2.toStrg() << "\t]\n"
                                    "leafaddr1:[" << leafaddr1.toStrg()
                            << "\t]\tleafaddr2:[" << leafaddr2.toStrg()
                            << "\t]\t\n" << "\tignorecount:[" << ignorecount++
                            << "\t](" << "vm_integrate.hpp:" << __LINE__ << ":0"
                            << ")" << endl;
                }
#endif
                return false;
            }
            return true;

        }

        return false;
    }
    /**
     * @brief 这个函数用以检查形参的一致性,即在添加argcmp与argref之前确保argref对应的argcmp为空。
     * 即添加后每个形参(ArgRef)最多只能对应一个实参(ArgCmp)。
     * 如果一个形参对应多个实参，那么这些多的实参是相同的参数，那么将删除未重复的（未被引用的树）树。
     * 如果实参不相同则返回假，如果重复树被引用则不删除。
     * @change 使用rcam
     * @param
     * @return
     */
    const bool checkArgRefConsistency(const ArgCmp &argcmp,
            const ArgRef &argref, const Addr &addrcmp, const Addr &addrref) {
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "checkArgRefConsistency(const ArgCmp&, const ArgRef&, const Addr&, const Addr&)"
                    << " in " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "vm_integrate.hpp:" << __LINE__ << ":0" << ")" << endl;
        }
#endif
        auto argRefIt = rcam.find(argref);
        if (argRefIt == rcam.end() || argRefIt->second.first == argnull) {
            rcam[argref] = pair<ArgCmp, AddrCmp>(argcmp, addrcmp);
            return true;
        } else {
            ArgCmp &argcmp2 = argRefIt->second.first;
            AddrCmp &addrcmp2 = argRefIt->second.second;
            if (sameOneArg(argcmp, argcmp2) == true) {
                return true;

            } else if (sameTreeStruct(argcmp, argcmp2, addrcmp, addrcmp2)
                    == true) {
#if debug
                {
                    static int ignorecount = 0;
                    cout
                            << "checkArgRefConsistency(const ArgCmp&, const ArgRef&, const Addr&, const Addr&)"
                            << " sameTreeStruct(argcmp, argref, addrcmp, addrref)== true"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "vm_integrate.hpp:" << __LINE__ << ":0" << ")"
                            << endl;
                }
#endif
                argRefIt->second = pair<ArgCmp, AddrCmp>(argcmp, addrcmp);
                return true;

            } else {
                return false;
            }
        }

        return false;

    }
    /**
     * @brief 此函数中，引用被转化为所指进行比较，但cram依然使用原引用作为键
     * @param argcmp
     * @param argref
     * @param addrcmp
     * @param addrref
     * @return
     */
    const bool add(const ArgCmp &argcmp, const ArgRef &argref,
            const Addr &addrcmp, const Addr &addrref) {

#if debug
        {
            static int ignorecount = 0;
            cout
                    << "add(const ArgCmp&, const ArgRef&, const Addr&, const Addr&)"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
#if debug
        cout << "Matcher::add \n\targcmp:[" << argcmp.toStrg()
                << "]\n\targref:[" << argref.toStrg() << "]" << endl;
#endif
        Arg argcmpTmp;
        argcmpTmp.clone(argcmp);
        if (argcmp.argFlag == Y_Dz) {
            argcmpTmp = globalRefTable[argcmp];

        }
        if (argref.changeable != A_
                && argcmpTmp.changeable != argref.changeable) {
#if debug
            cout << "Matcher::add false1\n\targcmp:[" << argcmpTmp.toStrg()
                    << "]\n\targref:[" << argref.toStrg() << "]" << endl;
#endif
            return false;
        }

        if (argref.argFlag == Sz || argref.argFlag == Zf) {
            if (argcmpTmp == argref && argcmpTmp.changeable == false
                    && argref.changeable == false) {

                if (argcmp.argFlag == Y_Dz) {
                    if (cram[argcmp].size() == 0) {
                        cram[argcmp].push_back(Arg());
                        cram[argcmp][0] = argref;
                    } else {
                        cram[argcmp].push_back(argref);
                    }
                }
                return true;
            } else {
                return false;
            }
        }

        if (argcmpTmp.argFlag == M_Bs) {
            return argcmpTmp == argref;
        }
        if (cram[argcmp].size() == 0) {
            cram[argcmp].push_back(Arg());
        }

        if (argcmpTmp.argFlag == Sz) {
            if (argcmpTmp.changeable == true) {
#if debug
                cerr << "Matcher::add err, Sz & changeable==true ,exit -1"
                        << endl;
#endif
                exit(-1);
            }
            if (argref.changeable == true) {
#if debug
                cout << "Matcher::add false2\n\targcmp:[" << argcmpTmp.toStrg()
                        << "]\n\targref:[" << argref.toStrg() << "]" << endl;
#endif
                return false;
            }
            if (argref.argFlag == Sz) {
                return argcmpTmp == argref;
            } else if (argref.argFlag == S_Dz || argref.argFlag == S_Bs) {
                for (const ArgRef &a : cram[argcmp]) {
                    if (a == argref) {
                        return true;
                    }
                }
                if (cram[argcmp][0] == argnull) {
                    if (checkArgRefConsistency(argcmp, argref, addrcmp,
                            addrref)) {
                        cram[argcmp][0] = argref;
                    } else {
                        return false;
                    }

                } else {
                    if (checkArgRefConsistency(argcmp, argref, addrcmp,
                            addrref)) {
                        cram[argcmp].push_back(argref);
                    } else {
                        return false;
                    }

                }
                return true;
            }

        }

        if (argcmpTmp.argFlag == X_Bs) {
#if debug
            cout
                    << "Matcher::add compare SHUXING,jump and return true. argcmpTmp:["
                    << argcmpTmp.toStrg() << "]\targref:[" << argcmpTmp.toStrg()
                    << "]" << endl;
#endif
            return true;
        }

        if (cram[argcmp][0] == argref) {
            {
#if debug
                static int ignorecount = 0;
                cout << "add(const ArgCmp&, const ArgRef&)"
                        << "cram[argcmpTmp][0] == argref" << "\tignorecount:["
                        << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                        << __LINE__ << ":0" << ")" << endl;
#endif
            }
            return true;
        } else if (argref.formalArg == true && cram[argcmp][0] == argnull) {
            if (checkArgRefConsistency(argcmp, argref, addrcmp, addrref)) {
                cram[argcmp][0] = argref;

            } else {
                return false;
            }
            return true;
        } else if (argref.formalArg == false
                && sameOneArg(argcmp, argref) == true) {
            if (checkArgRefConsistency(argcmp, argref, addrcmp, addrref)) {

                cram[argcmp][0] = argref;
            } else {
                return false;
            }
            return true;
        } else {
#if debug
            cout << "Matcher::add false3\n\targcmp:[" << argcmpTmp.toStrg()
                    << "]\n\targref:[" << argref.toStrg() << "]" << endl;
#endif
            return false;
        }
    }
    /**
     * @name compareSingleCode
     * @attention 引用会被转化为所指向的参数
     * @brief compare CODE in different lines
     * @param addrcmp
     * @param cdcmp
     * @param addrref
     * @param cdref
     * @return
     */
    const bool compareSingleCode(const AddrCmp &addrcmp, const Code &cdcmp,
            const AddrRef &addrref, const Code &cdref) {
        if (addrref == addrnull) {
            return true;
        } else if (addrref < addrnull) {

#if debug
            cout << "Matcher::compareSingleCode addrref < addrnull,addrcmp:["
                    << addrcmp.toStrg() << "]\taddrref:[" << addrref.toStrg()
                    << "]" << endl;
#endif
        } else if (addrref > addrnull) {

            if (addrm[addrcmp] == addrref) {
                return true;

            } else if (add(addrcmp, addrref) == false) {
#if debug
                cout << "Matcher::compareSingleCode err,\naddrcmp:["
                        << addrcmp.toStrg() << "]\tcdcmp:[" << cdcmp.toStrg()
                        << "]\naddrref:[" << addrref.toStrg() << "]\tcdref:["
                        << cdref.toStrg() << "]" << endl;
#endif
                return false;
            }

        } else {
#if debug
            cerr << "MatchState::compareSingleCode err::addrref compare err"
                    << endl;
#endif
            exit(-1);
        }

        ArgCmp &&argcmp1 = cdcmp[0];
        ArgCmp &&argcmp2 = cdcmp[1];
        ArgCmp &&operatorcmp = cdcmp[2];
        ArgCmp &&resultcmp = cdcmp[3];

//        if (argcmp1.argFlag == Y_Dz) {
//            argcmp1 = globalRefTable[argcmp1];
//        }
//        if (argcmp2.argFlag == Y_Dz) {
//            argcmp2 = globalRefTable[argcmp2];
//        }
//        if (operatorcmp.argFlag == Y_Dz) {
//            operatorcmp = globalRefTable[operatorcmp];
//        }
//        if (resultcmp.argFlag == Y_Dz) {
//            resultcmp = globalRefTable[resultcmp];
//        }

        if (addrref < addrnull) {
            bool compareresult = false;
            switch (addrref[0]) {
            case S_S_JIA:
                compareresult =
                        (operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "+")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case S_S_JIAN:
                compareresult =
                        (operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "-")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case S_S_CHENG:
                compareresult =
                        (operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "*")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case S_S_CHU:
                compareresult =
                        (operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "/")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case S_S_FUZHI:
                compareresult =
                        argcmp1.isVar()
                                && (operatorcmp.argFlag == M_Bs
                                        && operatorcmp.arg_s == "=")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case S_S_DENGYU:
                compareresult =
                        (operatorcmp.argFlag == M_Bs
                                && operatorcmp.arg_s == "==")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case S_S_DAYU:
                compareresult =
                        (operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == ">")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case S_S_XIAOYU:
                compareresult =
                        (operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "<")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case S_S_MI:
                compareresult =
                        (operatorcmp.argFlag == M_Bs && operatorcmp.arg_s == "^")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;

            case CALL:

                compareresult =
                        ((operatorcmp.arg_s == "call"
                                || operatorcmp.arg_s == "ca"))
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case SHUCHU:
                compareresult =
                        (operatorcmp.argFlag == M_Bs
                                && operatorcmp.arg_s == "-->")
                                && (argcmp1.changeable != T_
                                        && argcmp2.changeable != T_);
                break;
            case COMMA:
                /**todo::如果要支持多态，请在预编译时对函数名称进行变形
                 * 将逗号表达式纳入匹配项会导致原来对多态的自动支持不可用
                 */
                compareresult = (operatorcmp.argFlag == M_Bs
                        && operatorcmp.arg_s == "COMMA")
                /*&& (argcmp1.changeable != T_
                 && argcmp2.changeable != T_)*/
                ;
                break;
            default:
                break;
            }
            if (compareresult == true) {
                add(addrcmp, addrref);
                return true;
            } else {
                return false;
            }

        }
        ArgRef &&argref1 = cdref[0];
        ArgRef &&argref2 = cdref[1];
        ArgRef &&operatorref = cdref[2];
        ArgRef &&resultref = cdref[3];
        if (addrcmp != addrnull && addrref > addrnull) {
            bool add1 = add(argcmp1, argref1, addrcmp, addrref);
            if (add1 == false) {
#if debug
                cout << "add1false:addrcmp:[" << addrcmp.toStrg()
                        << "]\tadref:[" << addrref.toStrg() << "]"
                        << "\n\targcmp1:[" << argcmp1.toStrg() << "]"
                        << "\n\targref1:[" << argref1.toStrg() << "]"

                        << endl;
#endif
            }

            bool add2 = add(argcmp2, argref2, addrcmp, addrref);
            if (add2 == false) {
#if debug
                cout << "add2false:addrcmp:[" << addrcmp.toStrg()
                        << "]\tadref:[" << addrref.toStrg() << "]"
                        << "\n\targcmp2:[" << argcmp2.toStrg() << "]"
                        << "\n\targref2:[" << argref2.toStrg() << "]" << endl;
#endif
            }

            bool addop = add(operatorcmp, operatorref, addrcmp, addrref);

            if (addop == false) {
#if debug
                cout << "addopfalse:addrcmp:[" << addrcmp.toStrg()
                        << "]\tadref:[" << addrref.toStrg() << "]"
                        << "\n\toperatorcmp:[" << operatorcmp.toStrg() << "]"
                        << "\n\toperatorref:[" << operatorref.toStrg() << "]"
                        << endl;
#endif
            }
            bool addre = add(resultcmp, resultref, addrcmp, addrref);
            if (addre == false) {
#if debug
                cout << "addrefalse:addrcmp:[" << addrcmp.toStrg()
                        << "]\tadref:[" << addrref.toStrg() << "]"

                        << "\n\tresultcmp:[" << resultcmp.toStrg() << "]"
                        << "\n\tresultref:[" << resultref.toStrg() << "]"
                        << endl;
#endif
            }
            if (add1 && add2 && addop && addre) {
                push(addrcmp, 3, addrref, 3);
                push(addrcmp, 1, addrref, 1);
                push(addrcmp, 0, addrref, 0);

                return true;
            } else {
#if debug
                static Intg ignorecount = 0;
                cout << "addfalse:addrcmp:[" << addrcmp.toStrg()
                        << "]\taddrref:[" << addrref.toStrg() << "]"

                        << "\n\tresultcmp:[" << resultcmp.toStrg() << "]"
                        << "\n\tresultref:[" << resultref.toStrg() << "]"
                        << "\n\tignorecount:[" << ignorecount++ << "]" << endl;
#endif
                return false;
            }

        }
        //{

#if debug
        cerr << "MatchState::compareSingleCode err"
                "\n\t addrcmp:[" << addrcmp.toStrg() << "]\taddrref:["
                << addrref.toStrg() << "]"
                        "\n\tcdcmp::" << cdcmp.toStrg() << "\n\tcdref::"
                << cdref.toStrg() << endl;
#endif
        exit(-1);

        //}
    }
    const Intg size() const {
        return argposcmpv.size();
    }

};
class MatchStateSilo {
public:
    /**
     * @param matchStateSiloMaxCapacity : max capacity of the tree a root node can match
     */
    Intg matchStateSiloMaxCapacity = 10;
    multimap<Intg, MatchState> waitmsm;
    multimap<Intg, MatchState> resmsm;
    MatchState pop() {
        if (waitmsm.size() <= 0) {
#if debug
            cerr << "MatchStateSilo::pop err::no content" << endl;
#endif
            exit(-1);
        }
        MatchState ms(waitmsm.rbegin()->second);
        auto it = waitmsm.end();
        waitmsm.erase(--it);
        return ms;
    }
    void push(const Intg &organization, const MatchState &ms) {
        while ((Intg) waitmsm.size() > matchStateSiloMaxCapacity) {
            waitmsm.erase(waitmsm.begin());
        }
        if (ms.finished() == true) {
            resmsm.insert(pair<Intg, MatchState>(organization, ms));
        } else if (ms.addrm.size() > 0) {
            waitmsm.insert(pair<Intg, MatchState>(organization, ms));
        } else {
#if debug
            cerr << "MatchStateSilo push err::ms is null" << endl;
#endif
            exit(-1);
        }
    }

    const Intg size() const {
        return waitmsm.size();
    }
    MatchStateSilo() {
    }
    void setMaxCapacity(Intg matchStateSiloMaxCapacity) {
        this->matchStateSiloMaxCapacity = matchStateSiloMaxCapacity;
    }

};

class Matcher {
public:
    MatchStateSilo mss;
    CodeTable *cdtcmp;

    CodeTable *cdtref = NULL;
    TemplateFunctionTable *tft = NULL;
    AddrCmp addrcmp;
    AddrRef addrref;

    Matcher(CodeTable *cdtcmp, CodeTable *cdtref, TemplateFunctionTable *tft,
            const AddrCmp &addrcmp, const AddrRef &addrref) {
        if (cdtcmp == NULL) {
#if debug
            cerr << "Matcher::Matcher err::cdtcmp == null" << endl;
#endif
            exit(-1);
        }
        if (cdtref == NULL) {
#if debug
            cerr << "Matcher::Matcher err::cdtref == null" << endl;
#endif
            exit(-1);
        }

        this->cdtcmp = cdtcmp;
        this->cdtref = cdtref;
        this->tft = tft;
        this->addrcmp = addrcmp;
        this->addrref = addrref;

    }
    /**
     * @name compareMultiCode
     * @brief compare two trees
     * @param ms
     * @return
     */
    const bool compareMultiCode(MatchState &ms) {

        if (ms.size() <= 0) {
            return true;
        }
        AddrCmp addrcmp;
        Intg argposcmp;
        AddrRef addrref;
        Intg argposref;
        ms.pop(addrcmp, argposcmp, addrref, argposref);

#if debug
        static Intg ignorecount = 0;
        cout << "Matcher::compareMultiCode start, addrcmp:[" << addrcmp.toStrg()
                << "]\targposcmp:[" << argposcmp << "]"
                        "\taddrref:[" << addrref.toStrg() << "]\targposref:["
                << argposref << "]\tignorecount:[" << ignorecount++ << "]"
                << endl;
#endif

        if (addrref > addrnull && addrcmp > addrnull) {
            ArgCmp argcmp((*cdtcmp)[addrcmp][argposcmp]);
            AddrCmp cdaddrcmp =
                    cdtcmp->getSameTrBranchLastArgAddrIgnoreProperty(addrcmp,
                            argcmp);

            ArgRef argref((*cdtref)[addrref][argposref]);
            AddrRef cdaddrref =
                    cdtref->getSameTrBranchLastArgAddrIgnoreProperty(addrref,
                            argref);

            while (cdaddrcmp != addrnull && cdaddrref != addrnull) {
#if debug
                static Intg ignorecount = 0;
                cout
                        << "Matcher::compareMultiCode 2, cdaddrcmp != addrnull && cdaddrref != addrnull , "
                                "addrcmp:[" << addrcmp.toStrg()
                        << "]\targposcmp:[" << argposcmp << "]"
                                "\taddrref:[" << addrref.toStrg()
                        << "]\targposcmp:[" << argposref << "]\tignorecount:["
                        << ignorecount++ << "]" << endl;
#endif
                MatchState msmid(ms);
                if (msmid.compareSingleCode(cdaddrcmp, (*cdtcmp)[cdaddrcmp],
                        cdaddrref, (*cdtref)[cdaddrref]) == true) {

                    Intg organization = 0;
                    if (tft) {
                        const Addr &tfAddr = cdtref->getScopeAddr(addrref);
                        organization = (*tft)[tfAddr].organization;
                    }
                    mss.push(organization, msmid);
                }
                cdaddrcmp = cdtcmp->getSameTrBranchLastArgAddrIgnoreProperty(
                        cdaddrcmp, argcmp);
            }
            if (cdaddrref == addrnull) {
#if debug
                static Intg ignorecount = 0;
                cout
                        << "Matcher::compareMultiCode 3, cdaddrref == addrnull , match true"
                                "addrcmp:[" << addrcmp.toStrg()
                        << "]\targposcmp:[" << argposcmp << "]"
                                "\taddrref:[" << addrref.toStrg()
                        << "]\targposcmp:[" << argposref << "]\tignorecount:["
                        << ignorecount++ << "]" << endl;
#endif

                Intg organization = 0;
                if (tft) {
                    const Addr &tfAddr = cdtref->getScopeAddr(addrref);
                    organization = (*tft)[tfAddr].organization;
                }
                mss.push(organization, ms);
            }
            return true;
        } else {
#if debug
            cerr << "Matcher::compareMultiCode err,exit-1. addrcmp:["
                    << addrcmp.toStrg() << "]\t"
                            "addrref:[" << addrref.toStrg() << "]" << endl;
#endif

            exit(-1);
        }
    }
    const bool match() {
        MatchState ms(cdtcmp, cdtref);

        if (addrref > addrnull && addrcmp > addrnull) {
#if debug
            static Intg ignorecount = 0;
            cout << "Matcher::match addrref>addrnull \t"
                    "addrref:[" << addrref.toStrg() << "]\taddrcmp:["
                    << addrcmp.toStrg() << "]\tignorecount:[" << ignorecount++
                    << "]" << endl;
#endif

            if (ms.compareSingleCode(addrcmp, cdtcmp->operator [](addrcmp),
                    addrref, cdtref->operator [](addrref)) == true) {

#if debug
                static Intg ignorecount = 0;
                cout
                        << "Matcher::match addrref>addrnull compareSingleCode == true\t"
                                "addrref:[" << addrref.toStrg()
                        << "]\taddrcmp:[" << addrcmp.toStrg()
                        << "]\tignorecount:[" << ignorecount++ << "]" << endl;
#endif

                Intg organization = 0;
                if (tft) {
                    const Addr &tfAddr = cdtref->getScopeAddr(addrref);
                    organization = (*tft)[tfAddr].organization;
                }
                mss.push(organization, ms);
            } else {
                return false;
            }
        } else if (addrref < addrnull && addrcmp > addrnull) {
            return ms.compareSingleCode(addrcmp, cdtcmp->operator [](addrcmp),
                    addrref, Code());
        } else {

#if debug
            cerr << "Matcher::match err::invalid addrref:[" << addrref.toStrg()
                    << "]\t"
                            "addrcmp:[" << addrcmp.toStrg() << "]" << endl;
#endif

        }
        while (mss.waitmsm.size() > 0) {
#if debug
            static Intg ignorecount = 0;
            cout << "Matcher::match mss.waitmsm.size() > 0 \t"
                    "addrref:[" << addrref.toStrg() << "]\taddrcmp:["
                    << addrcmp.toStrg() << "]\tignorecount:[" << ignorecount++
                    << "]" << endl;
#endif
            MatchState ms = mss.pop();
            compareMultiCode(ms);
        }
        if (mss.resmsm.size() > 0) {
            return true;

        } else {
            return false;
        }

    }
};
/**
 * @name split 返回[min,max)(n==1)或(min,max)(n>1)
 * @param min
 * @param max
 * @param n
 * @return
 */
vector<Addr> split(const Addr &min, const Addr &max, const Intg n) {

    if ((min == max && n != 1) || n == 0) {
#if debug
        cerr << "split err,(min == max && n != 1) || n == 0 min:["
                << min.toStrg() << "]\tmax:[" << max.toStrg() << "]\tn:[" << n
                << "]" << endl;
#endif

        exit(-1);
    }

    vector<Addr> av;
    if (min == max && n == 1) {
        av.push_back(min);
    }
    Addr mid = min;

    Intg m = (min.size() <= max.size()) ? min.size() : max.size();
    for (Intg p = 0; p < m - 1; p++) {
        if (mid.addrv[p] < max.addrv[p]) {
            while ((Intg) av.size() < n) {
                av.push_back(++mid);

            }
            return av;
        }
    }

    if (mid.size() <= max.size() && mid < max) {
        while (mid.size() < max.size() && mid < max) {
            mid.addrv.push_back(1);

        }
        mid.addrv.push_back(2);
        while ((Intg) av.size() < n) {

            av.push_back(++mid);
            continue;

#if debug
            cerr << "split err 1" << endl;
#endif

        }
        return av;
    }

//    if (mid.size() == max.size()) {
//        mid.addrv.push_back(2);
//        while ((Intg) av.size() < n) {
//
//            av.push_back(++mid);
//            continue;
//
//            #if debug
//    cerr << "split err 2" << endl;
//    #endif
//
//        }
//    }
    if (mid.size() > max.size()) {
        while ((Intg) av.size() < n) {

            av.push_back(++mid);
            continue;

#if debug
            cerr << "split err 3" << endl;
#endif

        }
        return av;
    }
#if debug
    cerr << "split err, can't split" << endl;
#endif
    return av;
}
typedef Arg ArgReal;
typedef Arg ArgForm;
class SingleTFIntegrator {
public:
    CodeTable *cdtdes = NULL;
    CodeTable *cdtfrom = NULL;
    ScopeTable *st = NULL;
    Addr mindes;
    Addr maxdes;
    Addr trfrom;
    map<ArgForm, ArgReal> cram;
    map<AddrCmp, AddrRef> addrm;

    CodeTable cdtmid;
    SingleTFIntegrator(CodeTable *cdtdes, CodeTable *cdtfrom, ScopeTable *st,
            const Addr &mindes, const Addr &maxdes, const Addr &trfrom,
            const map<ArgCmp, deque<ArgRef> > &am_cmp_ref,
            const map<AddrCmp, AddrRef> &addrm) {
        this->cdtdes = cdtdes;
        this->cdtfrom = cdtfrom;
        this->st = st;
        this->mindes = mindes;
        this->maxdes = maxdes;
        this->trfrom = trfrom;
        this->addrm = addrm;
        cdtmid = cdtfrom->copyTree(trfrom);
        this->cram.clear();
        for (auto iter = am_cmp_ref.begin(); iter != am_cmp_ref.end(); iter++) {
            for (const ArgReal &ades : iter->second) {
                this->cram.insert(pair<ArgForm, ArgReal>(ades, iter->first));
            }

        }
    }
    void infer_changeable(const Arg &arg1_, const Arg &arg2_, Arg &argres_) {

        if (arg1_.changeable == T_ || arg2_.changeable == T_) {
            argres_.changeable = T_;
        } else if (arg1_.changeable == A_ || arg2_.changeable == A_) {
            argres_.changeable = A_;
        } else {
            argres_.changeable = F_;
        }
    }
    void infer_changeable(const Arg &arg1_, Arg &argres_) {
        argres_.changeable = arg1_.changeable == true;
    }
    const bool replaceLine(Code &code, const Addr &scopeAddrdes,
            const Addr &codeAddrdes) {
        {
#if debug
            static int ignorecount = 0;
            cout << "SingleTFIntegrator::replaceLine 1.0 ignorecount:["
                    << ignorecount++ << "\t]" << endl;
#endif
        }
        if (code.assemblyFormula.flagBit.execute == F_) {
            code.assemblyFormula.flagBit.execute = T_;
        }
        if (code.assemblyFormula.flagBit.type == M
                && code.assemblyFormula.flagBit.operator_flag == M_Bs
                && (code.assemblyFormula.quaternion.operator_s == "+"
                        || code.assemblyFormula.quaternion.operator_s == "-"
                        || code.assemblyFormula.quaternion.operator_s == "*"
                        || code.assemblyFormula.quaternion.operator_s == "/"
                        || code.assemblyFormula.quaternion.operator_s == "^"
                        || code.assemblyFormula.quaternion.operator_s == "=="
                        || code.assemblyFormula.quaternion.operator_s == "="
                        || code.assemblyFormula.quaternion.operator_s == "-->"
                        || code.assemblyFormula.quaternion.operator_s == "call"
                        || code.assemblyFormula.quaternion.operator_s == "ca"
                        || code.assemblyFormula.quaternion.operator_s == "COMMA"
                /*todo:add new common operator here*/
                )) {
            ArgDec *arg1 = &cram[code[0]];
            ArgDec *arg2 = &cram[code[1]];
            ArgDec *argres = &cram[code[3]];
            {
#if debug
                static int ignorecount = 0;
                cout << "SingleTFIntegrator::replaceLine 1.1 ignorecount:["
                        << ignorecount++ << "\t]" << endl;
#endif
            }

            Intg argNullNum = 0;
            if (*arg1 == argnull) {
                if (code.assemblyFormula.flagBit.arg1_flag != Sz
                        && code.assemblyFormula.flagBit.arg1_flag != Zf

                ) {
                    arg1->argFlag = S_Dz;
                    arg1->arg_a = codeAddrdes;
                    arg1->formalArg = T_;
                    arg1->changeable =
                            code.assemblyFormula.flagBit.changeable[0];
                    arg1->asc = scopeAddrdes;
                    ++argNullNum;
                } else {
                    (*arg1) = code[0];
                }

            }
            if (*arg2 == argnull) {
                if (code.assemblyFormula.flagBit.arg2_flag != Sz
                        && code.assemblyFormula.flagBit.arg2_flag != Zf) {
                    arg2->argFlag = S_Dz;
                    arg2->arg_a = codeAddrdes;
                    arg2->formalArg = T_;
                    arg2->changeable =
                            code.assemblyFormula.flagBit.changeable[1];
                    arg2->asc = scopeAddrdes;
                    ++argNullNum;
                } else {
                    *arg2 = code[1];
                }
            }
            if (*argres == argnull) {
                if (code.assemblyFormula.flagBit.result_flag != Sz
                        && code.assemblyFormula.flagBit.result_flag != Zf) {
                    argres->argFlag = S_Dz;
                    argres->arg_a = codeAddrdes;
                    argres->formalArg = T_;
                    argres->changeable =
                            code.assemblyFormula.flagBit.changeable[3];
                    argres->asc = scopeAddrdes;
                    ++argNullNum;
                } else {
                    *argres = code[3];
                }
            }
            if (argNullNum > 1) {
#if debug
                cerr
                        << "replaceLine err::more than one argnull to replace. argNullNum:["
                        << argNullNum << "]" << endl;
#endif
                exit(-1);
            }
            infer_changeable(*arg1, *arg2, *argres);
            code.scopeAddr = scopeAddrdes;

            code.assemblyFormula.flagBit.arg1_flag = arg1->argFlag;
            code.assemblyFormula.quaternion.arg1_a = arg1->arg_a;
            code.assemblyFormula.quaternion.arg1_i = arg1->arg_i;
            code.assemblyFormula.quaternion.arg1_s = arg1->arg_s;
            code.assemblyFormula.flagBit.formalArgFlag[0] = arg1->formalArg;
            code.assemblyFormula.flagBit.changeable[0] = arg1->changeable;
            code.assemblyFormula.flagBit.a1s = arg1->asc;
            code.assemblyFormula.flagBit.arg2_flag = arg2->argFlag;
            code.assemblyFormula.quaternion.arg2_a = arg2->arg_a;
            code.assemblyFormula.quaternion.arg2_i = arg2->arg_i;
            code.assemblyFormula.quaternion.arg2_s = arg2->arg_s;
            code.assemblyFormula.flagBit.formalArgFlag[1] = arg2->formalArg;
            code.assemblyFormula.flagBit.changeable[1] = arg2->changeable;
            code.assemblyFormula.flagBit.a2s = arg2->asc;
            code.assemblyFormula.flagBit.result_flag = argres->argFlag;
            code.assemblyFormula.quaternion.result_a = argres->arg_a;
            code.assemblyFormula.quaternion.result_i = argres->arg_i;
            code.assemblyFormula.quaternion.result_s = argres->arg_s;
            code.assemblyFormula.flagBit.formalArgFlag[3] = argres->formalArg;
            code.assemblyFormula.flagBit.changeable[3] = argres->changeable;
            code.assemblyFormula.flagBit.res = argres->asc;
            return true;
        } else if (code.assemblyFormula.flagBit.type == M
                && ((code.assemblyFormula.flagBit.operator_flag == M_Bs
                        && (code.assemblyFormula.quaternion.operator_s == ":"
                                && (code.assemblyFormula.quaternion.arg2_s
                                        == "out"
                                /*todo:add new attribute exclude here*/

                                )))
                        || (code.assemblyFormula.flagBit.operator_flag == M_Bs
                                && (code.assemblyFormula.quaternion.operator_s
                                        == "$"
                                        || code.assemblyFormula.quaternion.operator_s
                                                == "#")

                        /*todo:add new attribute operator exclude here*/
                        ))) {
            /**
             * 代码替换过程中将cdtfrom中的out、#、$属性赋值语句去掉
             */
            code.assemblyFormula.flagBit.execute = Du;
            ArgDec &arg1 = cram[code[0]];
            ArgDec &argres = cram[code[3]];

            Intg argNullNum = 0;
            if (arg1 == argnull) {
                arg1.argFlag = S_Dz;
                arg1.arg_a = codeAddrdes;
                arg1.formalArg = T_;
                arg1.changeable = code.assemblyFormula.flagBit.changeable[0];
                arg1.asc = scopeAddrdes;
                ++argNullNum;
            }

            if (argres == argnull) {
                argres.argFlag = S_Dz;
                argres.arg_a = codeAddrdes;
                argres.formalArg = T_;
                argres.changeable = code.assemblyFormula.flagBit.changeable[3];
                argres.asc = scopeAddrdes;
                ++argNullNum;
            }
            if (argNullNum > 1) {
#if debug
                cerr
                        << "replaceLine err::more than one argnull to replace. argNullNum:["
                        << argNullNum << "]" << endl;
#endif
                exit(-1);
            }
            infer_changeable(arg1, argres);
            code.scopeAddr = scopeAddrdes;

            code.assemblyFormula.flagBit.arg1_flag = arg1.argFlag;
            code.assemblyFormula.quaternion.arg1_a = arg1.arg_a;
            code.assemblyFormula.quaternion.arg1_i = arg1.arg_i;
            code.assemblyFormula.quaternion.arg1_s = arg1.arg_s;
            code.assemblyFormula.flagBit.formalArgFlag[0] = arg1.formalArg;
            code.assemblyFormula.flagBit.changeable[0] = arg1.changeable;
            code.assemblyFormula.flagBit.a1s = arg1.asc;

            code.assemblyFormula.flagBit.result_flag = argres.argFlag;
            code.assemblyFormula.quaternion.result_a = argres.arg_a;
            code.assemblyFormula.quaternion.result_i = argres.arg_i;
            code.assemblyFormula.quaternion.result_s = argres.arg_s;
            code.assemblyFormula.flagBit.formalArgFlag[3] = argres.formalArg;
            code.assemblyFormula.flagBit.changeable[3] = argres.changeable;
            code.assemblyFormula.flagBit.res = argres.asc;
            return true;
        }
        return false;
    }
    const bool replaceVar() {
        Intg s = cdtmid.addrdq.size();

        vector<Addr> av = split(mindes, maxdes, s);
        Addr scopeAddrdes = (*cdtdes)[cdtdes->getLast(M, maxdes)].scopeAddr;

        Addr cdtdesTreeHandleAddr = cdtdes->getLast(M, maxdes);
        Arg cdtdesTreeHandleArg((*cdtdes)[cdtdesTreeHandleAddr][3]);
        Arg cdtmidTreeHandleArg(cdtmid.codedq.back()[3]);
        cram[cdtmidTreeHandleArg] = cdtdesTreeHandleArg;
        for (Intg pos = 0; pos < s; pos++) {

            replaceLine(cdtmid[pos], scopeAddrdes, av[pos]);
            cdtmid.addrdq[pos] = av[pos];

        }
        return true;
    }
    const bool integrate() {
#if debug
        cout << "SingleTFIntegrator::integrator1" << endl;
#endif
        if (cram.size() != 0) {
            replaceVar();
            Addr &scopeFrom = (*cdtfrom)[trfrom].scopeAddr;
            if (st && (*st)[scopeFrom].functionFlag == FN) {
                for (auto &cdmid : cdtmid.codedq) {
                    cdmid.scopeAddr = (*cdtdes)[0].scopeAddr;
                    cdmid.assemblyFormula.flagBit.tfAddr = scopeFrom;
                    cdmid.assemblyFormula.flagBit.functionHandleFlag = F_;
                }
                cdtmid.codedq.back().assemblyFormula.flagBit.functionHandleFlag =
                T_;

            } else if (st
                    && ((*st)[scopeFrom].functionFlag == FB
                            || (*st)[scopeFrom].functionFlag == RFB)) {
                for (auto &cdmid : cdtmid.codedq) {
                    cdmid.scopeAddr = (*cdtdes)[0].scopeAddr;
                    cdmid.assemblyFormula.flagBit.functionHandleFlag = F_;
                }
            } else {
#if debug
                cout << "SingleTFIntegrator::integrate ,st==NULL" << endl;
#endif
            }
        }
#if debug
        cout << "SingleTFIntegrator::integrator2" << endl;
#endif

        if (addrm.size() == 0) {
#if debug
            cout
                    << "SingleTFIntegrator::integrate debug::addrm.size == 0,delete all (addrmin,addrmax)"
                    << endl;
#endif
            Intg posmin = 0;
            if (mindes != addrnull && mindes >= cdtdes->addrdq.front()) {

                posmin = getAddrPosition(cdtdes->addrdq, mindes);
                if (posmin == -1) {
#if debug
                    cerr
                            << "SingleTFIntegrator::integrate err::posmin == -1,exit-1"
                            << endl;
#endif
                    exit(-1);
                }
            } else {
                posmin = -1;
            }
            Intg posmax = INT_MAX - 10000;
            if (maxdes != addrnull && maxdes <= cdtdes->addrdq.back()) {
                posmax = getAddrPosition(cdtdes->addrdq, maxdes);
                if (posmax == -1) {
#if debug
                    cerr
                            << "SingleTFIntegrator::integrate err::posmax == -1,exit-1"
                            << endl;
#endif
                    exit(-1);
                }
            } else {
                posmax = cdtdes->addrdq.size();
            }
#if debug
            cout << "SingleTFIntegrator::integrate2.2 posmin:[" << posmin
                    << "]\tposmax:[" << posmax << "]" << endl;
#endif
            cdtdes->addrdq.erase(cdtdes->addrdq.begin() + posmin + 1,
                    cdtdes->addrdq.begin() + posmax);
            cdtdes->codedq.erase(cdtdes->codedq.begin() + posmin + 1,
                    cdtdes->codedq.begin() + posmax);
        } else {
#if debug
            cout << "SingleTFIntegrator::integrate2.3 addrm.size > 0" << endl;
#endif
            for (auto &it : addrm) {
#if debug
                static Intg ignorecount = 0;
                cout << "SingleTFIntegrator::integrate2.4 it:["
                        << it.first.toStrg() << "]\tignorecount:["
                        << ignorecount++ << "]" << endl;
#endif
                cdtdes->eraseCode(it.first, 0);
            }
        }
        Intg posInsert = 0;
        if (cdtdes->addrdq.size() > 0 && maxdes != addrnull
                && maxdes <= cdtdes->addrdq.back()) {
            posInsert = getAddrPosition(cdtdes->addrdq, maxdes);
            if (posInsert == -1) {
#if debug
                cerr
                        << "SingleTFIntegrator::integrate err:: posInsert == -1,exit-1"
                        << endl;
#endif
                exit(-1);
            }
        } else {

            posInsert = cdtdes->addrdq.size();
        }

#ifdef __linux__
        cdtdes->addrdq.insert<std::deque<Addr, allocator<Addr>>::iterator,
                std::deque<Addr, allocator<Addr>>::iterator>(
                cdtdes->addrdq.begin() + posInsert, cdtmid.addrdq.begin(),
                cdtmid.addrdq.end());
        cdtdes->codedq.insert<std::deque<Code, allocator<Code>>::iterator,
                std::deque<Code, allocator<Code>>::iterator>(
                cdtdes->codedq.begin() + posInsert, cdtmid.codedq.begin(),
                cdtmid.codedq.end());
#elif __WIN32__
        std::deque<VM::Addr>::iterator addrit = cdtdes->addrdq.begin()
                + posInsert;
        cdtdes->addrdq.insert(addrit, cdtmid.addrdq.begin(),
                cdtmid.addrdq.end());
        std::deque<VM::Code>::iterator codeit = cdtdes->codedq.begin()
                + posInsert;
        cdtdes->codedq.insert(codeit, cdtmid.codedq.begin(),
                cdtmid.codedq.end());
#endif
        return true;
    }

};
/**
 *this class used to record the functions a expression matched, in list ,more forward, earlier
 */
class CodeTableWithIntegrateProcess;
class CodeTableWithIntegrateProcess {
public:
    CodeTable codeTable;
    typedef Addr TemplateFunctionAddr;
    vector<TemplateFunctionAddr> integrateProcess;
    CodeTableWithIntegrateProcess(const CodeTable &codeTable,
            const vector<Addr> &integrateProcess) {
        this->codeTable = codeTable;
#if debug
        this->integrateProcess = integrateProcess;
#endif
    }
    CodeTableWithIntegrateProcess(const CodeTable &codeTable) {
        this->codeTable = codeTable;

    }
    CodeTableWithIntegrateProcess() {
    }
    Strg toStrg() const {
        stringstream ss;
        ss << "integrateProcess:\n" << VM::toStrg(integrateProcess) << endl;

        ss << "codeTable:\n" << this->codeTable.toStrg();
        return ss.str();

    }
};
/**
 * @brief 此函数用于将语法树中不被访问到的子树删除。
 * 删除的原则是：如果一行代码的四元式结果参数位在语法树中不被访问到，那么就删除此行代码
 * @param cdt
 */
void deleteInaccessibleSubtree(CodeTable &cdt) {
    set<Arg> argUseful;
    auto it = cdt.codedq.rbegin();
    Intg position = cdt.codedq.size() - 1;
    argUseful.insert(it->operator [](0));
    argUseful.insert(it->operator [](1));
    argUseful.insert(it->operator [](3));
    it++;
    position--;

    while (it != cdt.codedq.rend()) {
        if (argUseful.count(it->operator [](3))) {
            argUseful.insert(it->operator [](0));
            argUseful.insert(it->operator [](1));
            argUseful.insert(it->operator [](3));
        } else {
            cdt.eraseCode(position, 0);
        }
        position--;
        it++;

    }

    return;
}
/**
 * @brief 重置代码段的地址为min到max之间并修改语法树中出现的S_Dz地址参数的值以满足代码段地址范围。
 * 具体值取决于split(min,max,n)函数的返回值
 * @attention 此函数只适用于属于同一棵树的代码段
 * @param min
 * @param max
 * @return Code中被替换的地址与原地址对照表map<original,new>
 */
map<Addr, Addr> CodeTable::resetAddrRange(const Addr &min, const Addr &max) {
    if (this->size() == 0) {
        return map<Addr, Addr>();
    }
    vector<Addr> addrvNew = split(min, max, size());
    typedef Addr AddrOriginal;
    typedef Addr AddrNew;
    map<AddrOriginal, AddrNew> onm;
    Intg currentPosition = 0;
    Intg maxPosition = size();
    while (currentPosition < maxPosition) {
        addrdq[currentPosition] = addrvNew[currentPosition];
        Addr &currentCodeAddr = addrdq[currentPosition];
        Code &currentCode = codedq[currentPosition];
        if (currentCode.assemblyFormula.flagBit.arg1_flag == S_Dz
                && currentCode.assemblyFormula.quaternion.arg1_a != addrnull) {
            AddrOriginal *addrOriginal =
                    &(currentCode.assemblyFormula.quaternion.arg1_a);
            auto addrNewIt = onm.find(*addrOriginal);
            if (addrNewIt != onm.end()) {
                *addrOriginal = addrNewIt->second;
            } else {
                onm[*addrOriginal] = currentCodeAddr;
                *addrOriginal = currentCodeAddr;
            }
        }
        if (currentCode.assemblyFormula.flagBit.arg2_flag == S_Dz
                && currentCode.assemblyFormula.quaternion.arg2_a != addrnull) {
            AddrOriginal *addrOriginal =
                    &(currentCode.assemblyFormula.quaternion.arg2_a);
            auto addrNewIt = onm.find(*addrOriginal);
            if (addrNewIt != onm.end()) {
                *addrOriginal = addrNewIt->second;
            } else {
                onm[*addrOriginal] = currentCodeAddr;
                *addrOriginal = currentCodeAddr;
            }
        }
        if (currentCode.assemblyFormula.flagBit.operator_flag == S_Dz
                && currentCode.assemblyFormula.quaternion.operator_a
                        != addrnull) {
            AddrOriginal *addrOriginal =
                    &(currentCode.assemblyFormula.quaternion.operator_a);
            auto addrNewIt = onm.find(*addrOriginal);
            if (addrNewIt != onm.end()) {
                *addrOriginal = addrNewIt->second;
            } else {
                onm[*addrOriginal] = currentCodeAddr;
                *addrOriginal = currentCodeAddr;
            }
        }
        if (currentCode.assemblyFormula.flagBit.result_flag == S_Dz
                && currentCode.assemblyFormula.quaternion.result_a
                        != addrnull) {
            AddrOriginal *addrOriginal =
                    &(currentCode.assemblyFormula.quaternion.result_a);
            auto addrNewIt = onm.find(*addrOriginal);
            if (addrNewIt != onm.end()) {
                *addrOriginal = addrNewIt->second;
            } else {
                onm[*addrOriginal] = currentCodeAddr;
                *addrOriginal = currentCodeAddr;
            }
        }

        ++currentPosition;
    }
    return onm;

}
/**
 * @attention 将codeTableInsert插入到insertAddr “之前”，如果insertAddr的值为空，这将其插入最后面
 * @param insertAddr
 * @param codeTableInsert
 */
void CodeTable::insertCodeTable(const Addr &insertAddr,
        const CodeTable &codeTableInsert) {
    Intg posInsert = 0;
    if (this->addrdq.size() > 0 && insertAddr != addrnull
            && insertAddr <= this->addrdq.back()) {
        posInsert = getAddrPosition(this->addrdq, insertAddr);
        if (posInsert == -1) {
#if debug
            cerr << "CodeTable::insertCodeTable err:: posInsert == -1,exit-1"
                    << endl;
#endif
            exit(-1);
        }
    } else {

        posInsert = this->addrdq.size();
    }
#ifdef __linux__
    this->addrdq.insert<std::deque<Addr, allocator<Addr>>::const_iterator,
            std::deque<Addr, allocator<Addr>>::const_iterator>(
            this->addrdq.begin() + posInsert, codeTableInsert.addrdq.begin(),
            codeTableInsert.addrdq.end());
    this->codedq.insert<std::deque<Code, allocator<Code>>::const_iterator,
            std::deque<Code, allocator<Code>>::const_iterator>(
            this->codedq.begin() + posInsert, codeTableInsert.codedq.begin(),
            codeTableInsert.codedq.end());
#elif __WIN32__
    std::deque<VM::Addr>::iterator addrit = this->addrdq.begin() + posInsert;
    this->addrdq.insert(addrit, codeTableInsert.addrdq.begin(),
            codeTableInsert.addrdq.end());
    std::deque<VM::Code>::iterator codeit = this->codedq.begin() + posInsert;
    this->codedq.insert(codeit, codeTableInsert.codedq.begin(),
            codeTableInsert.codedq.end());
#endif
    return;

}
/**
 * @attention 将codeTableAppend插入到appendAddr “之后”,如果@arg{appendAddr} == addrnull，则将其插入到最后面
 * @param insertAddr
 * @param codeTableInsert
 */
void CodeTable::appendCodeTable(const Addr &appendAddr,
        const CodeTable &codeTableAppend) {
    Intg posAppend = 0;
    if (this->addrdq.size() > 0 && appendAddr != addrnull
            && appendAddr <= this->addrdq.back()) {
        posAppend = getAddrPosition(this->addrdq, appendAddr) + 1;
        if (posAppend == -1) {
#if debug
            cerr << "CodeTable::appendCodeTable err:: posAppend == -1,exit-1"
                    << endl;
#endif
            exit(-1);
        }
    } else {

        posAppend = this->addrdq.size();
    }
#ifdef __linux__
    this->addrdq.insert<std::deque<Addr, allocator<Addr>>::const_iterator,
            std::deque<Addr, allocator<Addr>>::const_iterator>(
            this->addrdq.begin() + posAppend, codeTableAppend.addrdq.begin(),
            codeTableAppend.addrdq.end());
    this->codedq.insert<std::deque<Code, allocator<Code>>::const_iterator,
            std::deque<Code, allocator<Code>>::const_iterator>(
            this->codedq.begin() + posAppend, codeTableAppend.codedq.begin(),
            codeTableAppend.codedq.end());
#elif __WIN32__
    std::deque<VM::Addr>::iterator addrit = this->addrdq.begin() + posAppend;
    this->addrdq.insert(addrit, codeTableAppend.addrdq.begin(),
            codeTableAppend.addrdq.end());
    std::deque<VM::Code>::iterator codeit = this->codedq.begin() + posAppend;
    this->codedq.insert(codeit, codeTableAppend.codedq.begin(),
            codeTableAppend.codedq.end());
#endif
    return;

}
/**
 * @brief 删除指定间距内的所有Code，即[min+offset1,max+offset2],offset可以为负数
 * @param eraseAddrMin
 * @param offsetRight1
 * @param eraseAddrMax
 * @param offsetRight2
 */
void CodeTable::eraseCode(const Addr &eraseAddrMin, Intg offsetRight1,
        const Addr &eraseAddrMax, Intg offsetRight2) {
    if (eraseAddrMin == addrnull || eraseAddrMax == addrnull
            || eraseAddrMin > eraseAddrMax) {
#if debug
        {
            static int ignorecount = 0;
            cout << "CodeTable::eraseCode(const Addr&, int, const Addr&, int)"
                    << " err,exit(-1)" << "\tignorecount:[" << ignorecount++
                    << "\t](" << "vm_integrate.hpp:" << __LINE__ << ":0" << ")"
                    << endl;
        }
#endif
        exit(-1);
    }
    Intg positionMin = getAddrPosition(addrdq, eraseAddrMin) + offsetRight1;
    Intg positionMax = getAddrPosition(addrdq, eraseAddrMax) + offsetRight2 + 1;
    addrdq.erase(addrdq.begin() + positionMin, addrdq.begin() + positionMax);
    codedq.erase(codedq.begin() + positionMin, codedq.begin() + positionMax);
}
void CodeTable::appendCodeTable(const CodeTable &codeTableAppend) {
    this->appendCodeTable(addrnull, codeTableAppend);
}
void unfoldRingStruct(CodeTable &cdt) {
#if debug
    {
        static int ignorecount = 0;
        cout << "unfoldRingStruct(CodeTable&)" << " in" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_integrate.hpp:" << __LINE__
                << ":0" << ")" << endl;
    }
#endif
//todo:: logic here
    if (cdt.addrdq.size() == 0) {
        return;
    }
#if debug
    {
        static int ignorecount = 0;
        cout << "unfoldRingStruct(CodeTable&)" << " 1" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_integrate.hpp:" << __LINE__
                << ":0" << ")" << endl;
    }
#endif
    auto currentAddrIt = cdt.addrdq.begin();
    Addr *currentCodeAddr = NULL;
    auto currentCodeIt = cdt.codedq.begin();
    Code *currentCode = NULL;

#if debug
    {
        static int ignorecount = 0;
        cout << "unfoldRingStruct(CodeTable&)" << " 2" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_integrate.hpp:" << __LINE__
                << ":0" << ")" << endl;
    }
#endif
//find ring root

    /*
     * 查找形成环状结构的节点（即出现一个地址被多次访问的情况）。
     * 这种情况一般是因为出现了化繁步骤导致地址复制。
     * 这一步需要找到重复的地址(S_Dz)及其位置。
     *
     * 需要说明的是，一个表达式的第三个参数，即结果参数，
     * 在保证地址结果地址不重复的情况下不对其进行重复性检测，因为其只要被调用必是重复的
     */
    set<Arg> argAlreadyExist;
#if debug
    {
        static int ignorecount = 0;
        cout << "unfoldRingStruct(CodeTable&)" << " 3" << "\tignorecount:["
                << ignorecount++ << "\t](" << "vm_integrate.hpp:" << __LINE__
                << ":0" << ")" << endl;
    }
#endif
    while (currentAddrIt != cdt.addrdq.end()) {
#if debug
        {
            static int ignorecount = 0;
            cout << "unfoldRingStruct(CodeTable&)" << " 4" << "\tignorecount:["
                    << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        currentCodeAddr = &*currentAddrIt;
        currentCode = &*currentCodeIt;
        ++currentAddrIt;
        ++currentCodeIt;
        Arg &&arg0 = (*currentCode)[0];
        Arg &&arg1 = (*currentCode)[1];
        Arg &&arg2 = (*currentCode)[2];
        Arg &&arg3 = (*currentCode)[3];
#if debug
        {
            static int ignorecount = 0;
            cout << "unfoldRingStruct(CodeTable&)" << " 5" << "\tignorecount:["
                    << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if (arg0.argFlag == S_Dz) {
            if (argAlreadyExist.count(arg0) <= 0) {
                argAlreadyExist.insert(arg0);
            } else {
                Addr branchRoot = cdt.getSameTrBranchLastArgAddrIgnoreProperty(
                        *currentCodeAddr, arg0);
                CodeTable &&treeBranchRef = cdt.copyTreeBranch(branchRoot);

                const Addr &&insertAddrMin = cdt.getAboveCodeAddr(
                        *currentCodeAddr);
                Addr &insertAddrMax = *currentCodeAddr;

                CodeTable &treeBranchInsert = treeBranchRef;
                map<Addr, Addr> &&onm = treeBranchInsert.resetAddrRange(
                        insertAddrMin, insertAddrMax);
                currentCode->assemblyFormula.quaternion.arg1_a =
                        onm[arg0.arg_a];
                cdt.insertCodeTable(*currentCodeAddr, treeBranchInsert);

            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "unfoldRingStruct(CodeTable&)" << " 6" << "\tignorecount:["
                    << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if (arg1.argFlag == S_Dz) {
            if (argAlreadyExist.count(arg1) <= 0) {
                argAlreadyExist.insert(arg1);
            } else {
                Addr branchRoot = cdt.getSameTrBranchLastArgAddrIgnoreProperty(
                        *currentCodeAddr, arg1);
                CodeTable &&treeBranchRef = cdt.copyTreeBranch(branchRoot);

                const Addr &&insertAddrMin = cdt.getAboveCodeAddr(
                        *currentCodeAddr);
                Addr &insertAddrMax = *currentCodeAddr;

                CodeTable &treeBranchInsert = treeBranchRef;
                map<Addr, Addr> &&onm = treeBranchInsert.resetAddrRange(
                        insertAddrMin, insertAddrMax);
                currentCode->assemblyFormula.quaternion.arg2_a =
                        onm[arg1.arg_a];
                cdt.insertCodeTable(*currentCodeAddr, treeBranchInsert);

            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "unfoldRingStruct(CodeTable&)" << " 7" << "\tignorecount:["
                    << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if (arg2.argFlag == S_Dz) {
            if (argAlreadyExist.count(arg2) <= 0) {
                argAlreadyExist.insert(arg2);
            } else {
                Addr branchRoot = cdt.getSameTrBranchLastArgAddrIgnoreProperty(
                        *currentCodeAddr, arg2);
                CodeTable &&treeBranchRef = cdt.copyTreeBranch(branchRoot);

                const Addr &&insertAddrMin = cdt.getAboveCodeAddr(
                        *currentCodeAddr);
                Addr &insertAddrMax = *currentCodeAddr;

                CodeTable &treeBranchInsert = treeBranchRef;
                map<Addr, Addr> &&onm = treeBranchInsert.resetAddrRange(
                        insertAddrMin, insertAddrMax);
                currentCode->assemblyFormula.quaternion.operator_a =
                        onm[arg2.arg_a];
                cdt.insertCodeTable(*currentCodeAddr, treeBranchInsert);

            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "unfoldRingStruct(CodeTable&)" << " 8" << "\tignorecount:["
                    << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif

    }

//replace ring root var
//copy tree branch
//replace tree branch var
//insert tree branch

}
class CodeTableSilo;
class CodeTableSilo {

public:
    Intg codeTableSiloMaxCapacity = 300;
    bool siloEmpty = false;

    const bool (*onIntegrateFinish)(
            pair<Intg, CodeTableWithIntegrateProcess> p)=NULL;

    Intg currentSiloTurn = 0;
    Intg pos = -1;

    std::multimap<Intg, CodeTableWithIntegrateProcess>::reverse_iterator posrit;
    multimap<Intg, CodeTableWithIntegrateProcess> silo1;
    multimap<Intg, CodeTableWithIntegrateProcess> silo2;
    multimap<Intg, CodeTableWithIntegrateProcess> resultsilo;
    multimap<Intg, CodeTableWithIntegrateProcess> *currentSilo = &silo1;
    multimap<Intg, CodeTableWithIntegrateProcess> *nextSilo = &silo2;
private:
    const bool switchSilo() {
        multimap<Intg, CodeTableWithIntegrateProcess> *transitSilo = currentSilo;
        currentSilo->clear();
        currentSilo = nextSilo;
        nextSilo = transitSilo;
        currentSiloTurn++;
        {

#if debug
            static int ignorecount = 0;
            cout << "switchSilo()" << "\n" << "\t\tcurrentSiloTurn:["
                    << currentSiloTurn << "\t]\n" << "map size:["
                    << currentSilo->size() << "\t]\n" << "\tignorecount:["
                    << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")\n" << toStrg(*currentSilo)

                    << endl;
#endif
        }
        return true;
    }
public:
    /**
     *@attention 自动切换silo,如果currentsilo和nextsilo同时为空，则将siloEmpty置为真
     * @return pair<组织度，cdt>
     */
    pair<Intg, CodeTableWithIntegrateProcess> getNext() {
        pos++;
        if (pos == 0 && pos < (Intg) currentSilo->size()) {
            posrit = currentSilo->rbegin();
            return pair<Intg, CodeTableWithIntegrateProcess>(posrit->first,
                    posrit->second);
        } else if (pos > 0 && pos < (Intg) currentSilo->size()) {
            ++posrit;
            return pair<Intg, CodeTableWithIntegrateProcess>(posrit->first,
                    posrit->second);
        } else {
            pos = -1;
            switchSilo();
            if (currentSilo->size() == 0) {
#if debug
                cout
                        << "CodeTableSilo::getNext() , silo empty, match failed! line:["
                        << __LINE__ << "]" << endl;
#endif
                siloEmpty = true;
                return pair<Intg, CodeTableWithIntegrateProcess>(0,
                        CodeTableWithIntegrateProcess());
            }
            return getNext();
        }

    }
    const bool add(const Intg organization, CodeTable &cdt,
            const vector<Addr> &integrateProcess) {
        {
#if debug
            static Intg ignorecount = 0;
            cout << "CodeTableSilo::add in ,ignorecount:[" << ignorecount++
                    << "]"
                            "\n\t organization:[" << organization << "]"
                            "\n\t cdt:[" << cdt.toStrg() << "]" << endl;
#endif

        }
        deleteInaccessibleSubtree(cdt);
        unfoldRingStruct(cdt);

        Intg count = nextSilo->count(organization);
        auto it = nextSilo->find(organization);
        for (Intg i = 0; i < count && it != nextSilo->end(); ++i, ++it) {
            CodeTable &cdtexist = it->second.codeTable;
            if (cdtexist.addrdq.size() == cdt.addrdq.size()) {
                Matcher m(const_cast<CodeTable*>(&cdt),
                        const_cast<CodeTable*>(&cdtexist), NULL,
                        cdt.addrdq.back(), cdtexist.addrdq.back());
                if (m.match() == true) {
                    bool same = true;
                    for (auto &it : m.mss.resmsm.begin()->second.cram) {
                        if (it.second.size() != 1
                                || (it.first == it.second[0]) == false) {
                            same = false;
                            break;
                        }

                    }
                    if (same) {
                        {
#if debug
                            static int ignorecount = 0;
                            cout << "add(const int, const CodeTable&)"
                                    << "\ttwo trees the same\n"
                                    << "tree1(cmp):\n" << cdt.toStrg() << "\n"
                                    << "tree2(exist):\n" << cdtexist.toStrg()

                                    << "\tignorecount:[" << ignorecount++
                                    << "\t](" << "vm_integrate.hpp:" << __LINE__
                                    << ":0" << ")" << endl;
#endif
                        }
                        return true;
                    }

                }
            }

        }

        while ((Intg) nextSilo->size() > codeTableSiloMaxCapacity) {
            nextSilo->erase(nextSilo->begin());
        }

        Intg s = cdt.addrdq.size();
        if (s == 0) {
#if debug
            cerr << "CodeTableSilo::add err,add null" << endl;
#endif
        }
        Intg pos = 0;
        while (pos < s) {
            if (cdt.codedq[pos].assemblyFormula.flagBit.tfAddr == addrnull) {
                {
#if debug
                    static Intg ignorecount = 0;
                    cout << "CodeTableSilo::add nextSilo.insert ,ignorecount:["
                            << ignorecount++ << "]"
                                    "\n\t organization:[" << organization << "]"
                                    "\n\t cdt:[" << cdt.toStrg() << "]" << endl;
#endif

                }
                nextSilo->insert(
                        pair<Intg, CodeTableWithIntegrateProcess>(organization,
                                CodeTableWithIntegrateProcess(cdt,
                                        integrateProcess)));
                while ((Intg) nextSilo->size() > codeTableSiloMaxCapacity) {
                    nextSilo->erase(nextSilo->begin());
                }
                return true;
            }
            pos++;
        }

        {
#if debug
            static Intg ignorecount = 0;
            cout << "CodeTableSilo::add resultSilo.insert ,ignorecount:["
                    << ignorecount++ << "]"
                            "\n\t organization:[" << organization << "]"
                            "\n\t cdt:[" << cdt.toStrg() << "]" << endl;
#endif

        }
        resultsilo.insert(
                pair<Intg, CodeTableWithIntegrateProcess>(organization,
                        CodeTableWithIntegrateProcess(cdt, integrateProcess)));
        if (onIntegrateFinish != NULL) {
            (*onIntegrateFinish)(
                    pair<Intg, CodeTableWithIntegrateProcess>(organization,
                            CodeTableWithIntegrateProcess(cdt,
                                    integrateProcess)));

        }
        {
#if debug
            static Intg ignorecount = 0;
            cout << "CodeTableSilo::add out ,ignorecount:[" << ignorecount++
                    << "]"
                            "\n\t organization:[" << organization << "]"
                            "\n\t cdt:[" << cdt.toStrg() << "]" << endl;
#endif

        }
        return true;
    }
    CodeTableSilo& reset(Intg organization, CodeTableWithIntegrateProcess cip,
            const bool (*onIntegrateFinish)(
                    pair<Intg, CodeTableWithIntegrateProcess> p)) {

        currentSilo->clear();
        nextSilo->clear();
        resultsilo.clear();
        currentSiloTurn = 0;
        pos = -1;

//        currentSilo->insert(p);
        this->onIntegrateFinish = onIntegrateFinish;
        this->add(organization, cip.codeTable, cip.integrateProcess);
        return (*this);
    }
    CodeTableSilo(const pair<Intg, CodeTableWithIntegrateProcess> &p,
            const bool (*onIntegrateFinish)(
                    pair<Intg, CodeTableWithIntegrateProcess> p)) {
        currentSilo->insert(p);
        this->onIntegrateFinish = onIntegrateFinish;
    }
    CodeTableSilo() {
    }
    void setMaxCapacity(Intg codeTableSiloMaxCapacity) {
        this->codeTableSiloMaxCapacity = codeTableSiloMaxCapacity;
    }

};
/**
 * @class 这个类提供了将函数与代码匹配的方法
 * @attention 在2022年3月17日21:34:16之后的版本中，此类将提供指定匹配模式的接口
 */
class MultiTFIntegrator {
public:
    Intg mode = MODE_DEFAULT;
    CodeTable *codeTableDes = NULL;
    Intg matchStateSiloMaxCapacity = 30;

    Intg codeTableSiloMaxCapacity = 300;
    bool root = false;
    Intg maxSwitchTurn = 10;
    multimap<Intg, CodeTable> icmap;
    MultiTFIntegrator *parent = NULL;
    CodeTableSilo cts;
    DataTable *currentAR = NULL;

    CodeTable *cdtmain = NULL;
    Addr addrdes;
    TemplateFunctionTable *tft = NULL;
    ScopeTable *st = NULL;
    SystemTable *systemTable = NULL;

    /***
     *
     * @param cdtmain
     * @param addrdes
     * @param tft
     * @param st
     */
    MultiTFIntegrator(CodeTable *cdtmain, const Addr &addrdes,
            TemplateFunctionTable *tft, ScopeTable *st,
            SystemTable *systemTable, DataTable *currentAR) {
        this->cdtmain = cdtmain;
        this->addrdes = addrdes;
        this->tft = tft;
        this->st = st;
        this->systemTable = systemTable;
        this->currentAR = currentAR;
    }
    /**
     * 将工作模式设置为MODE_SEPARATE，其效果是匹配时不再将结果直接整合到cdtmain中，
     * 但需提供整合代码段目标以及来源
     * @arg
     */
    void setModeSeparate(CodeTable *codeTableDes) {
        mode = MODE_SEPARATE;
        this->codeTableDes = codeTableDes;
    }
    void setMaxCapacity(Intg matchStateSiloMaxCapacity,
    Intg codeTableSiloMaxCapacity, Intg maxSwitchTurn) {
        this->matchStateSiloMaxCapacity = matchStateSiloMaxCapacity;
        this->codeTableSiloMaxCapacity = codeTableSiloMaxCapacity;
        this->maxSwitchTurn = maxSwitchTurn;
        this->cts.setMaxCapacity(codeTableSiloMaxCapacity);
    }
    /**
     * @brief 获得指定表达式匹配函数时从何处开始寻找可访问函数
     * @param expAddr表达式中的任意一个地址
     * @return
     */
    Addr getFunIndexAddr(const Addr &expAddr) {
        Addr mindeslimit = cdtmain->getLastNot(M, addrdes);
        Addr maxdeslimit = cdtmain->getNextNot(M, addrdes);

        Intg posmax = getAddrPosition(cdtmain->addrdq, maxdeslimit);
        Intg posmin = getAddrPosition(cdtmain->addrdq, mindeslimit);
        while (--posmax > posmin) {
            if (cdtmain->codedq[posmax].assemblyFormula.flagBit.operator_flag
                    == M_Bs
                    || cdtmain->codedq[posmax].assemblyFormula.quaternion.operator_s
                            == "call") {
                Arg &&funNameArg = (cdtmain->codedq[posmax])[0];
                if (funNameArg.argFlag == Y_Dz) {
                    funNameArg.arg_ar = currentAR;

                    Addr &&sysScopeAddr = globalRefTable[funNameArg].asc;
                    Scope &sysScope = (*st)[sysScopeAddr];
                    //需要返回类内的一行代码才能访问类内的函数
                    Addr &&expAddr_ = cdtmain->getAboveCodeAddr(
                            sysScope.scopeCoor2);
                    return expAddr_;

                }
                break;
            }
        }
        return expAddr;

    }

    const bool integrate() {
        CodeTable *cdtdes = NULL;
        CodeTable cdttemp(
                (mode != MODE_SEPARATE) ?
                        (cdtmain->copyTree(addrdes)) : CodeTable());
        if (mode == MODE_SEPARATE) {
            cdtdes = codeTableDes;
        } else {
            cdtdes = &cdttemp;
        }
        cdtdes->setRefArgAR(currentAR);
#if debug
        static Intg ignorecount = 0;
        cout << "Integrator::integrate1:(*cdtdes):[" << (*cdtdes).toStrg()
                << "]\tignorecount:[" << ignorecount++ << "]" << endl;
#endif
        cts.reset(0, CodeTableWithIntegrateProcess((*cdtdes)), NULL);
        Addr mindeslimit = cdtmain->getLastNot(M, addrdes);
        Addr maxdeslimit = cdtmain->getNextNot(M, addrdes);
        Addr funIndexAddr = getFunIndexAddr(addrdes); //检索函数时，从这个地址开始向上检索,如果此地址为空则无效
        if (maxdeslimit == addrnull) {
            maxdeslimit = addrmax;
        }

        while (cts.resultsilo.size() == 0) {
            if (cts.currentSiloTurn >= maxSwitchTurn) {
#if debug
                {
                    static int ignorecount = 0;
                    cout << "integrate()"
                            << " cts.currentSiloTurn >= maxSwitchTurn"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "vm_integrate.hpp:" << __LINE__ << ":0" << ")"
                            << endl;
                }
#endif
                return false;
            } else if (cts.siloEmpty) {
#if debug
                {
                    static int ignorecount = 0;
                    cout << "integrate()" << " siloEmpty " << "\tignorecount:["
                            << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
                return false;
            }
            auto p = cts.getNext();
            Intg organization = p.first;
            CodeTable *cdtdes = &p.second.codeTable;

            Intg leng = (*cdtdes).addrdq.size();
            Intg pos = 0;

            while (pos < leng) {

                if ((*cdtdes)[pos].assemblyFormula.flagBit.tfAddr != addrnull) {
                    pos++;
                    continue;

                }
                Addr addrnode = (*cdtdes).addrdq[pos];
                vector<Addr> integrateProcess = p.second.integrateProcess;
#if debug
                cout << "Integrator::integrate2:addrdes:[" << addrdes.toStrg()
                        << "]" << endl;
#endif
                Addr lastTFAddr = tft->lastAccessibleTFAddr(st, cdtmain,
                        systemTable, maxdeslimit, funIndexAddr);
#if debug
                cout << "Integrator::integrate3:lastTFAddr:["
                        << lastTFAddr.toStrg() << "]" << endl;
#endif

                while (lastTFAddr != addrnull) {
                    Addr addrref;

                    if (lastTFAddr < addrnull) {
                        addrref = lastTFAddr;

                    } else if (lastTFAddr > addrnull) {
                        addrref = cdtmain->getLast(M,
                                (*st)[lastTFAddr].scopeCoor2);
                    } else {

#if debug
                        cerr
                                << "MultiTFIntegrator::integrator err: no suitable template"
                                << endl;
#endif
                        return false;
                    }
                    Matcher m(cdtdes, cdtmain, tft, addrnode, addrref);
                    m.mss.setMaxCapacity(matchStateSiloMaxCapacity);
                    if (m.match() == true) {
                        vector<Addr> integrateProcess =
                                p.second.integrateProcess;
                        integrateProcess.push_back(lastTFAddr);
                        if ((*tft)[lastTFAddr].integrate == true) {
#if debug
                            cout
                                    << "MultiTFIntegrator::integrator integrate == true"
                                    << endl;
#endif
                            CodeTable cdtmid(*cdtdes);
                            Intg organizationmid = organization
                                    + (*tft)[lastTFAddr].organization;
#if debug
                            cout << "MultiTFIntegrator::integrator4" << endl;
#endif
                            CodeTable cdtfrom;
                            //MultiTFIntegrator会认为以下几种类型的代码不属于语法树。其不会被用来替换原代码段
                            vector<Intg> exclude_types;
                            exclude_types.push_back(QS);
                            exclude_types.push_back(QE);
                            exclude_types.push_back(C);
                            exclude_types.push_back(ME);
                            exclude_types.push_back(F);
                            exclude_types.push_back(R);
                            {
#if debug
                                static int ignorecount = 0;
                                cout << "integrate()" << " 4.1"
                                        << "\tignorecount:[" << ignorecount++
                                        << "\t](" << "vm_integrate.hpp:"
                                        << __LINE__ << ":0" << ")" << endl;
#endif
                            }
                            const Addr &returnHandle =
                                    (*st)[(*tft)[lastTFAddr].bodyScopeAddr].returnHandle[0];
                            {
#if debug
                                static int ignorecount = 0;
                                cout << "integrate()" << " returnHandle:["
                                        << returnHandle.toStrg() << "\t]"
                                        << "\tignorecount:[" << ignorecount++
                                        << "\t](" << "vm_integrate.hpp:"
                                        << __LINE__ << ":0" << ")" << endl;
#endif
                            }
                            const Addr &&aboveCodeAddr =
                                    cdtmain->getAboveCodeAddr(returnHandle);
                            {
#if debug
                                static int ignorecount = 0;
                                cout << "integrate()" << " aboveCodeAddr:["
                                        << aboveCodeAddr.toStrg() << "\t]"
                                        << "\tignorecount:[" << ignorecount++
                                        << "\t](" << "vm_integrate.hpp:"
                                        << __LINE__ << ":0" << ")" << endl;
#endif
                            }
                            bool &&notexclude =
                                    (find(exclude_types.begin(),
                                            exclude_types.end(),
                                            (*cdtmain)[aboveCodeAddr].assemblyFormula.flagBit.type)
                                            == exclude_types.end());
                            {
#if debug
                                static int ignorecount = 0;
                                cout << "integrate()" << " "
                                        << "\tignorecount:[" << ignorecount++
                                        << "\t](" << "vm_integrate.hpp:"
                                        << __LINE__ << ":0" << ")" << endl;
#endif
                            }

                            if (notexclude) {
                                cdtfrom =
                                        cdtmain->copyTree(
                                                cdtmain->getLast(M,
                                                        (*st)[(*tft)[lastTFAddr].bodyScopeAddr].returnHandle[0]));

                            } else {
                                {
#if debug
                                    static int ignorecount = 0;
                                    cout << "integrate()" << "no tree found"
                                            << "\tignorecount:["
                                            << ignorecount++ << "\t]("
                                            << "vm_integrate.hpp:" << __LINE__
                                            << ":0" << ")" << endl;
#endif
                                }
                            }
                            for (Code &cd : cdtfrom.codedq) {
                                cd.assemblyFormula.flagBit.tfAddr = addrnull;
                                cd.assemblyFormula.flagBit.functionHandleFlag =
                                        false;

                            }
#if debug
                            cout << "MultiTFIntegrator::integrator5" << endl;
#endif

                            Addr maxdes =
                                    (pos < (Intg) (cdtmid.addrdq.size() - 1) ?
                                            cdtmid.addrdq[pos + 1] : maxdeslimit);

#if debug
                            cout << "MultiTFIntegrator::integrator6" << endl;
#endif
#if debug
                            cout << "Integrator::integrate stfi: "
                                    "\n\tcdtmid:\n" << cdtmid.toStrg() << endl;
#endif
#if debug
                            cout << "\n\tcdtfrom:\n" << cdtfrom.toStrg()
                                    << endl;
#endif
                            for (auto it : m.mss.resmsm) {
                                MatchState &ms = it.second;
                                CodeTable cdtmid_clone(cdtmid);
                                Intg posmindes = pos;
                                Addr mindes = cdtmid_clone.addrdq[posmindes];
#if debug
                                static Intg ignorecounts = 0;
                                cout
                                        << "MultiTFIntegrator::integrator6.2\tposmindes:["
                                        << posmindes << "]\t"
                                                "(Addr)mindes:["
                                        << mindes.toStrg() << "]\t"
                                                "ignorecount:["
                                        << ignorecounts++ << "]" << endl;
#endif
#if debug
                                cout << "\n\tcdtmid_clone:\n"
                                        << cdtmid_clone.toStrg() << endl;
#endif
#if debug
                                cout << "\n\tcdtfrom:\n" << cdtfrom.toStrg()
                                        << endl;
#endif
                                while (ms.addrm.find(mindes) != ms.addrm.end()) {
                                    if (posmindes >= 0) {
                                        mindes =
                                                cdtmid_clone.addrdq[posmindes--];
                                    } else {
                                        mindes = mindeslimit;
                                        break;
                                    }

                                }
                                {
#if debug
                                    static Intg ignorecounts__ = 0;
                                    cout
                                            << "MultiTFIntegrator::integrator6.3\tmaxdes:["
                                            << maxdes.toStrg() << "]\t"
                                                    "(Addr)mindes:["
                                            << mindes.toStrg() << "]\t"
                                                    "ignorecount:["
                                            << ignorecounts__++ << "]" << endl;
#endif
#if debug
                                    cout << "\n\tcdtmid_clone:\n"
                                            << cdtmid_clone.toStrg() << endl;
#endif
#if debug
                                    cout << "\n\tcdtfrom:\n" << cdtfrom.toStrg()
                                            << endl;
#endif
                                }

                                {
#if debug
                                    static Intg ignorecount = 0;
                                    cout
                                            << "MultiTFIntegrator::integrator6.9\tmindes:["
                                            << mindes.toStrg() << "]"
                                                    "\tmaxdes:["
                                            << maxdes.toStrg()
                                            << "]\tignorecount:["
                                            << ignorecount++ << "]" << endl;
#endif
                                }

                                if (cdtfrom.addrdq.size() > 0) {
                                    SingleTFIntegrator stfi(&cdtmid_clone,
                                            &cdtfrom, st, mindes, maxdes,
                                            cdtfrom.addrdq[0], ms.cram,
                                            ms.addrm);
#if debug
                                    cout << "MultiTFIntegrator::integrator7"
                                            << endl;
#endif
                                    stfi.integrate();
#if debug
                                    cout << "MultiTFIntegrator::integrator8"
                                            << endl;
#endif
#if debug
                                    cout << "Integrator::integrate stfi: "
                                            "\n\tcdtmid_clone:\n"
                                            << cdtmid_clone.toStrg() << endl;
#endif
#if debug
                                    cout << "\n\tcdtfrom:\n" << cdtfrom.toStrg()
                                            << endl;
#endif
                                } else {
                                    //todo:当cdtfrom内储存代码长度为零时，说明返回表达式是一个参数。
                                    //在cdtmid中寻找与所匹配函数名称（cdtref）的最后一行的结果相同的参数，
                                    //将其替换为返回的参数。
                                    Addr &addrcmp = addrnode;
                                    map<ArgForm, ArgReal> frmap;
                                    map<ArgReal, deque<ArgRef> > &rfmap =
                                            ms.cram;
                                    for (auto iter = rfmap.begin();
                                            iter != rfmap.end(); iter++) {
                                        for (const ArgForm &aform : iter->second) {
                                            frmap.insert(
                                                    pair<ArgForm, ArgReal>(
                                                            aform,
                                                            iter->first));
                                        }

                                    }
                                    Arg &argBeReplaced = frmap[Arg(Bs, "ans")];
                                    Addr &returnHandleAddr =
                                            (*st)[(*tft)[lastTFAddr].bodyScopeAddr].returnHandle[0];
                                    CodeTable *cdtref = m.cdtref;
                                    Arg &argBeUsedToReplace =
                                            frmap[(*cdtref)[returnHandleAddr][0]];

                                    const Addr &nextArgAddr =
                                            cdtmid_clone.getSameTrBranchNextArgAddr(
                                                    addrcmp, argBeReplaced);
                                    {
#if debug
                                        static int ignorecount = 0;
                                        cout << "integrate() 8.9"
                                                << "nextArgAddr:["
                                                << nextArgAddr.toStrg() << "\t]"
                                                << "argBeReplaced:["
                                                << argBeReplaced.toStrg()
                                                << "\t]" << "\tignorecount:["
                                                << ignorecount++ << "\t]("
                                                << "vm_integrate.hpp:"
                                                << __LINE__ << ":0" << ")"
                                                << endl;
#endif
                                    }
                                    if (nextArgAddr == addrnull) {
                                        {

#if debug
                                            static int ignorecount = 0;
                                            cout << "integrate()"
                                                    << " 9 ,err in integrate no code Codetable, exit(-1)\n"
                                                            "m.cdtcmp->copyTree(addrcmp):["
                                                    << m.cdtcmp->copyTree(
                                                            addrcmp).toStrg()
                                                    << "\t]\n" << "addrcmp:["
                                                    << addrcmp.toStrg() << "\t]"
                                                    << "argBeReplaced:["
                                                    << argBeReplaced.toStrg()
                                                    << "\t]" << "frmap:["
                                                    << toStrg(frmap) << "]"
                                                    << "\tignorecount:["
                                                    << ignorecount++ << "\t]("
                                                    << "vm_integrate.hpp:"
                                                    << __LINE__ << ":0" << ")"
                                                    << endl;
#endif
                                        }
                                        exit(-1);
                                    }
                                    for (auto &it : ms.addrm) {
#if debug
                                        static Intg ignorecount = 0;
                                        cout
                                                << "SingleTFIntegrator::integrate 10 erase code addr:["
                                                << it.first.toStrg()
                                                << "]\tignorecount:["
                                                << ignorecount++ << "]" << endl;
#endif
                                        cdtmid_clone.eraseCode(it.first, 0);
                                    }
                                    Code &codeReplace =
                                            cdtmid_clone[nextArgAddr];
                                    if (cdtmid_clone[nextArgAddr][0]
                                            == argBeReplaced) {

                                        codeReplace.assemblyFormula.flagBit.arg1_flag =
                                                argBeUsedToReplace.argFlag;
                                        codeReplace.assemblyFormula.quaternion.arg1_a =
                                                argBeUsedToReplace.arg_a;
                                        codeReplace.assemblyFormula.quaternion.arg1_i =
                                                argBeUsedToReplace.arg_i;
                                        codeReplace.assemblyFormula.quaternion.arg1_s =
                                                argBeUsedToReplace.arg_s;
                                        codeReplace.assemblyFormula.flagBit.formalArgFlag[0] =
                                                argBeUsedToReplace.formalArg;
                                        codeReplace.assemblyFormula.flagBit.changeable[0] =
                                                argBeUsedToReplace.changeable;
                                    } else if (cdtmid_clone[nextArgAddr][1]
                                            == argBeReplaced) {
                                        codeReplace.assemblyFormula.flagBit.arg2_flag =
                                                argBeUsedToReplace.argFlag;
                                        codeReplace.assemblyFormula.quaternion.arg2_a =
                                                argBeUsedToReplace.arg_a;
                                        codeReplace.assemblyFormula.quaternion.arg2_i =
                                                argBeUsedToReplace.arg_i;
                                        codeReplace.assemblyFormula.quaternion.arg2_s =
                                                argBeUsedToReplace.arg_s;
                                        codeReplace.assemblyFormula.flagBit.formalArgFlag[1] =
                                                argBeUsedToReplace.formalArg;
                                        codeReplace.assemblyFormula.flagBit.changeable[1] =
                                                argBeUsedToReplace.changeable;
                                    } else if (cdtmid_clone[nextArgAddr][2]
                                            == argBeReplaced) {
                                        codeReplace.assemblyFormula.flagBit.operator_flag =
                                                argBeUsedToReplace.argFlag;
                                        codeReplace.assemblyFormula.quaternion.operator_a =
                                                argBeUsedToReplace.arg_a;
                                        codeReplace.assemblyFormula.quaternion.operator_i =
                                                argBeUsedToReplace.arg_i;
                                        codeReplace.assemblyFormula.quaternion.operator_s =
                                                argBeUsedToReplace.arg_s;
                                        codeReplace.assemblyFormula.flagBit.formalArgFlag[2] =
                                                argBeUsedToReplace.formalArg;
                                        codeReplace.assemblyFormula.flagBit.changeable[2] =
                                                argBeUsedToReplace.changeable;
                                    } else if (cdtmid_clone[nextArgAddr][3]
                                            == argBeReplaced) {
                                        codeReplace.assemblyFormula.flagBit.result_flag =
                                                argBeUsedToReplace.argFlag;
                                        codeReplace.assemblyFormula.quaternion.result_a =
                                                argBeUsedToReplace.arg_a;
                                        codeReplace.assemblyFormula.quaternion.result_i =
                                                argBeUsedToReplace.arg_i;
                                        codeReplace.assemblyFormula.quaternion.result_s =
                                                argBeUsedToReplace.arg_s;
                                        codeReplace.assemblyFormula.flagBit.formalArgFlag[3] =
                                                argBeUsedToReplace.formalArg;
                                        codeReplace.assemblyFormula.flagBit.changeable[3] =
                                                argBeUsedToReplace.changeable;
                                    }

                                }
                                cts.add(organizationmid, cdtmid_clone,
                                        integrateProcess);
                            }
                        } else if (lastTFAddr > addrnull) {
#if debug
                            cout
                                    << "MultiTFIntegrator::integrator integrate == false"
                                    << endl;
#endif
                            CodeTable cdtmid((*cdtdes));
                            Intg organizationmid = organization
                                    + (*tft)[lastTFAddr].organization;
#if debug
                            cout << "MultiTFIntegrator::integrator9" << endl;
#endif
                            CodeTable cdtfrom = cdtmain->copyTree(
                                    (*tft)[lastTFAddr].templateHandleAddr);
#if debug
                            cout << "MultiTFIntegrator::integrator10" << endl;
#endif

                            Addr maxdes =
                                    (pos < (Intg) (cdtmid.addrdq.size() - 1) ?
                                            cdtmid.addrdq[pos + 1] : maxdeslimit);
#if debug
                            cout << "MultiTFIntegrator::integrator11" << endl;
#endif
#if debug
                            cout << "Integrator::integrate stfi: "
                                    "\n\tcdtmid:\n" << cdtmid.toStrg() << endl;
#endif
#if debug
                            cout << "\n\tcdtfrom:\n" << cdtfrom.toStrg()
                                    << endl;
#endif
                            for (auto it : m.mss.resmsm) {
                                CodeTable cdtmid_clone(cdtmid);
                                MatchState &ms = it.second;
#if debug
                                static Intg ignorecount = 0;
                                cout
                                        << "MultiTFIntegrator::integrator11.2\tignorecount:["
                                        << ignorecount++ << "]" << endl;
#endif

                                Intg posmindes = pos;
                                Addr mindes = cdtmid_clone.addrdq[posmindes];
                                do {
                                    --posmindes;
                                    if (posmindes < 0) {
                                        mindes = mindeslimit;
                                        break;
                                    }
                                    mindes = cdtmid_clone.addrdq[posmindes];

                                } while (ms.addrm.find(mindes) != ms.addrm.end()
                                        || (*cdtdes)[mindes].assemblyFormula.flagBit.execute
                                                == Du);
                                SingleTFIntegrator stfi(&cdtmid_clone, &cdtfrom,
                                        st, mindes, maxdes, cdtfrom.addrdq[0],
                                        ms.cram, ms.addrm);
#if debug
                                cout << "MultiTFIntegrator::integrator12"
                                        << endl;
#endif
                                stfi.integrate();

#if debug
                                cout << "MultiTFIntegrator::integrator13"
                                        << endl;
#endif
#if debug
                                cout << "Integrator::integrate stfi: "
                                        "\n\tcdtmid_clone:\n"
                                        << cdtmid_clone.toStrg() << endl;
#endif
#if debug
                                cout << "\n\tcdtfrom:\n" << cdtfrom.toStrg()
                                        << endl;
#endif
#if debug
                                {
                                    static int ignorecount = 0;
                                    cout << "integrate()" << " "
                                            << "\tignorecount:["
                                            << ignorecount++ << "\t]("
                                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                                            << __LINE__ << ":0" << ")" << endl;
                                }
#endif
                                cts.add(organizationmid, cdtmid_clone,
                                        integrateProcess);
                            }
                        } else {
                            CodeTable cdtmid((*cdtdes));
                            Intg organizationmid = organization
                                    + (*tft)[lastTFAddr].organization;

                            Intg s = 1;

                            for (Intg offset = 0; offset < s; offset++) {
                                cdtmid[pos - offset].assemblyFormula.flagBit.tfAddr =
                                        lastTFAddr;
                                cdtmid[pos - offset].assemblyFormula.flagBit.functionHandleFlag =
                                T_;
                            }
                            cts.add(organizationmid, cdtmid, integrateProcess);

                        }

                    }
#if debug
                    cout << "lastTFAddr 1:[" << lastTFAddr.toStrg() << "]"
                            << endl;
#endif
                    lastTFAddr = tft->lastAccessibleTFAddr(st, cdtmain,
                            systemTable, lastTFAddr, funIndexAddr);
#if debug
                    cout << "lastTFAddr 2:[" << lastTFAddr.toStrg() << "]"
                            << endl;
#endif
                }
                pos++;

            }

        }
        if (mode == MODE_DEFAULT || mode == MODE_INTEGRATE) {
#if debug
            cout << "Integrator::integrate stfi: resultSilo["
                    << cts.resultsilo.rbegin()->second.toStrg() << "]" << endl;
#endif
            SingleTFIntegrator stfi(cdtmain,
                    &(cts.resultsilo.rbegin()->second.codeTable), st,
                    cdtmain->getLastNot(M, addrdes),
                    cdtmain->getNextNot(M, addrdes),
                    cts.resultsilo.rbegin()->second.codeTable.addrdq[0],
                    map<ArgForm, deque<ArgReal> >(), map<AddrCmp, AddrRef>());
            stfi.integrate();
        } else if (mode == MODE_SEPARATE) {
            (*codeTableDes) = cts.resultsilo.rbegin()->second.codeTable;
        }
        return true;
    }

};
class Integrator;
class CodeBlock {
public:
    Intg type = M;                                    //可以为M、QS、QE、C、R
    bool dependent = false; //如果一个codeBlock在创建CodeTable时没有涉及未知值，那么它就是独立的（false），有未知值时，是依赖的（true）
    set<Arg> argUnknown; // 这个codeBlock所要求的未知值
    set<Arg> argTemporary; //储存所有在本block中   产生   临时变量（引用的临时变量不算）
    CodeTable codeTable; //codeTable
    Intg QSid = 0;
    Intg QEid = 0;
    Intg id = 0; //唯一id
    CodeBlock(Intg type, bool dependent, Intg id, Intg QSid) {
        this->type = type;
        this->dependent = dependent;
        this->id = id;

    }
    CodeBlock() {
    }
    void addUnknownArg(const Arg &arg) {
        argUnknown.insert(arg);
    }
    Strg toStrg() {
        stringstream ss;
        ss << "CodeBlock\t" << "id:[" << id << "]\t" << "dependent:["
                << dependent << "]\t" << "QSid:[" << QSid << "]\t" << "QEid:["
                << QEid << "]\n\t\n";
        ss << "argUnknown:\n";
        for (auto &arg : argUnknown) {
            ss << arg.toStrg();
        }
        ss << "\t\n";
        ss << codeTable.toStrg() << "\t\n";
        return ss.str();

    }

};
class CodeBlockTable {
public:

    deque<Intg> iddq;
    deque<Intg> biddq; //逆向函数执行时的顺序backwardIDdeque

    map<Intg, CodeBlock> icbmap;
    void addBlock(Intg id, const CodeBlock &cb) {
        iddq.push_back(id);
        icbmap[id] = cb;
    }
    /**
     * @attention 这个函数可以向CodeBlockTable中添加codeBlock。
     * 当id在icbmap中不存在时，会在返回引用的同时向iddq中pushback id。
     * @param id
     * @return
     */
    CodeBlock& operator[](Intg id) {
        if (icbmap.count(id) <= 0) {
            iddq.push_back(id);
        }
        return icbmap[id];
    }
    void erase(Intg id) {
        for (auto it = iddq.begin(); it != iddq.end(); it++) {
            if (*it == id) {
                iddq.erase(it);
                icbmap.erase(id);
                break; //必须加break，否则it是最后一个元素，会陷入死循环
            }
        }

    }
    Strg toStrg() {
        stringstream ss;
        for (auto it : iddq) {
            ss << icbmap[it].toStrg();
        }
        return ss.str();
    }

};
class BackStepper {
public:
    Integrator *integrator = NULL;
    CodeTable *codeTable = NULL;
    ScopeTable *scopeTable = NULL;
    DataTable *currentAR = NULL;
    TemplateFunctionTable *templateFunctionTable = NULL;
    SystemTable *systemTable = NULL;
    Intg numID = 1;

    Intg currentScopeID = 0;
    vector<Intg> scopeIDStack;

    CodeBlockTable codeBlockTable;

    map<Arg, Arg> old_new_map; //map<oldArg,newArg>
    map<Arg, set<Intg>> unknown_id_map; //储存所有的未知变量以及依赖此arg的codeBlockID
    set<Arg> newarg; //储存所有引入的newarg，初始化时要加上
//储存所有临时变量，并记录其出现次数。临时变量即不能被用户访问的变量，属性S地址，
//一个临时变量只能出现最多两次，首次出现必须作为四元式的结果，第二次出现不能作为四元式的结果。
    map<Arg, Intg> temporaryArg_occ_map;
//储存所有临时变量，以及引用这些临时变量值的block的集合（set长度最大为2）
    map<Arg, set<Intg>> temporaryArg_id_map;
//  map<Arg, set<Intg>> unknown_id_map; //arg_dependentBlockID_map
    set<Intg> return_id;
    deque<Arg> argAnnounce;
    Intg maxBackStep = 10;  //逆推时最大允许代入的步数
    typedef multimap<Intg, set<Intg>> T_num_idset_map;
    deque<T_num_idset_map> step_map_dq; //每个map都是未知量个数、代入顺序的map
    set<Addr> scopeAddrSet;

    Addr currentFBAd;
    Code *currentFBCd = NULL;

    Addr nextFBAd;
    Addr currentScopeAddr;
    Intg currentFBCdPos = 0;
    Intg nextFBCdPos = 0;
    vector<Addr> scopeStack;

    set<Arg> argKnown;
    set<Arg> argUnknown;
    set<Arg> mustKnown;

    Addr bfnScopeAddr;
    CodeTable *bfbScopeCdt = NULL;
    Intg argID = 0;
    ~BackStepper() {
#if debug
        {
            static int ignorecount = 0;
            cout << "~BackStepper()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        integrator = nullptr;
        codeTable = nullptr;
        scopeTable = nullptr;
        currentAR = nullptr;
        templateFunctionTable = nullptr;
        systemTable = nullptr;
        argKnown.clear();

        argUnknown.clear();

        mustKnown.clear();

        return_id.clear();
        scopeStack.clear();
        unknown_id_map.clear();
        step_map_dq.clear();

#if debug
        {
            static int ignorecount = 0;
            cout << "~BackStepper()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
    }

    /**
     * 删除指定id的codeBlock，同时清理new_id_map
     * @param id
     */
    void eraseBlock(Intg id) {
        CodeBlock &blockremove = codeBlockTable[id];
        for (auto it : blockremove.argUnknown) {
            unknown_id_map[it].erase(id);
            if (unknown_id_map[it].size() == 0) {
                unknown_id_map.erase(it);
            }
        }
        codeBlockTable.erase(id);

    }
    /**
     * 当idset中的所有式子互相代入后，还有多少个未知变量（估计值，未计算）
     * @param idset
     * @return
     */
    Intg getUnknownArgNum(set<Intg> idset) {
        set<Arg> argset;
#if debug
        {
            static int ignorecount = 0;
            cout << "getUnknownArgNum(set<int>)" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        //合并所有block的未知变量至argset
        for (auto it : idset) {
            argset.insert(codeBlockTable[it].argUnknown.begin(),
                    codeBlockTable[it].argUnknown.end());
        }
        //如果求解一个未知变量的所依赖block均在idset中，
        //那么对这个idset而言这各变量是已知的，可以从argset中移除

        Intg num = argset.size();
        for (auto it : argset) {
            if (includes(idset.begin(), idset.end(), unknown_id_map[it].begin(),
                    unknown_id_map[it].end())) {
                --num;
            }
        }
        return num;
    }
    /**
     * 当idset中的所有式子互相代入后，还有多少个未知变量（估计值，未计算）
     * @param idset
     * @return
     */
    set<Arg> getUnknownArgSet(set<Intg> idset) {

        set<Arg> argset;
        for (auto it : idset) {

            argset.insert(codeBlockTable[it].argUnknown.begin(),
                    codeBlockTable[it].argUnknown.end());
        }
//        Intg num = argset.size();
        auto argsetTemp = argset;
        for (auto it : argset) {
            if (includes(idset.begin(), idset.end(), unknown_id_map[it].begin(),
                    unknown_id_map[it].end())) {
                argsetTemp.erase(it);

            }
        }
        return argsetTemp;
    }
    /**
     * @brief 将id1代表的codeBlock整合到id2代表的block中。所述操作的block均在cbt中。
     * @param id1
     * @param id2
     * @param cbt
     * @return 代入成功/失败
     */
    bool integrateTwoBlock(Intg id1, Intg id2, CodeBlockTable *cbt) {
        CodeBlock &cb1 = (*cbt)[id1];
        CodeBlock &cb2 = (*cbt)[id2];
        Code &cb1root = cb1.codeTable.codedq.back();
        Arg &&arg1 = cb1root[0];
        Arg &&arg2 = cb1root[1];
        Arg &&argop = cb1root[2];
        Arg &&argres = cb1root[3];
        if (cb2.argUnknown.count(argres) == 0) {
            //如果一个block的根节点不是另一个block的未知数，则两个block不能代入
            return false;
        } else {
            if (argop.argFlag == M_Bs && argop.arg_s == "=") {
                //如果一个block的根节点是未知数，且block的最后一个最小表达式为赋值式，那么
                //1 获得赋值变量
                Arg &argassignR = arg2;
                Arg &argassignL = arg1;
                //2 删去赋值表达式
                cb1.codeTable.addrdq.pop_back();
                cb1.codeTable.codedq.pop_back();
                //3 将cb2中的参数换成赋值变量
                cb2.codeTable.replaceVar(argassignL, argassignR);
                //4 合并cdt，合并并更新argUnknown
                cb2.codeTable.addrdq.insert(cb2.codeTable.addrdq.begin(),
                        cb1.codeTable.addrdq.begin(),
                        cb1.codeTable.addrdq.end());
                cb2.codeTable.codedq.insert(cb2.codeTable.codedq.begin(),
                        cb1.codeTable.codedq.begin(),
                        cb1.codeTable.codedq.end());
                set<Arg> argUnknown2temp;

                set_union(cb1.argUnknown.begin(), cb1.argUnknown.end(),
                        cb2.argUnknown.begin(), cb2.argUnknown.end(),
                        inserter(argUnknown2temp, argUnknown2temp.begin()));
                argUnknown2temp.erase(arg1);
                cb2.argUnknown = argUnknown2temp;

            } else {
                //如果一个block的根节点是未知数，且block的最后一个最小表达式不为赋值式，直接将两个block合并
                cb2.codeTable.addrdq.insert(cb2.codeTable.addrdq.begin(),
                        cb1.codeTable.addrdq.begin(),
                        cb1.codeTable.addrdq.end());
                cb2.codeTable.codedq.insert(cb2.codeTable.codedq.begin(),
                        cb1.codeTable.codedq.begin(),
                        cb1.codeTable.codedq.end());
                set<Arg> argUnknown2temp;

                set_union(cb1.argUnknown.begin(), cb1.argUnknown.end(),
                        cb2.argUnknown.begin(), cb2.argUnknown.end(),
                        inserter(argUnknown2temp, argUnknown2temp.begin()));
                cb2.argUnknown = argUnknown2temp;

            }
            return true;
        }

    }
    /**
     * @brief 将blockIDset中的所有代码段进行整合，其基本思路是，从前向后依次合并：
     * 找到最小id对应的block，先将其与和其有公共未知变量的block合并，
     * 然后逐步推进到根节点
     * @param integrateIDblock
     * @return
     */
    CodeBlock integrateBlock(set<Intg> blockIDset) {
#if debug
        {
            static int ignorecount = 0;
            cout << "integrateBlock(set<int>)" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if (blockIDset.size() == 0) {
            return CodeBlock();
        } else if (blockIDset.size() > 0) {
            set<Intg> blockIDsetRemain = blockIDset;
            deque<pair<Intg, Intg>> integrateOrder;
            CodeBlockTable codeBlockTableTemp;
            //此循环进行合并
            //1。1 从小到大获得block的id
            for (auto it : blockIDset) {
                codeBlockTableTemp.addBlock(it, codeBlockTable[it]);
                blockIDsetRemain.erase(it);

                //1.2遍历依赖该block对应的未知量的block的集合block1
                for (auto arg_unknown : codeBlockTable[it].argUnknown) {
                    set<Intg> &intersection_1 = unknown_id_map[arg_unknown];

                    set<Intg> intersection_res;

                    //1.3如果block1中的元素同时也是blockidset中除去block的元素（blockIDsetRemain），
                    //那么这个元素可以与该block组合
                    //通过每循环删除blockIDsetRemain中的it使得不重复匹配
                    set_intersection(intersection_1.begin(),
                            intersection_1.end(), blockIDsetRemain.begin(),
                            blockIDsetRemain.end(),
                            inserter(intersection_res,
                                    intersection_res.begin()));
                    //1.4 将该block与1.3所得元素组对后填入整合顺序中
                    for (auto id_ : intersection_res) {
                        integrateOrder.push_back(make_pair(it, id_));

                    }

                }

            }
            //2 根据integrateOrder（尝试）合并一个个的codeBlock.合并的codeBlock单独储存。
            //注意，合并后不需要重整地址范围。地址范围最后再整理

            set<Intg> substitudeID;            //已经代入的block
            for (auto it : integrateOrder) {

                bool substituteSuccess = integrateTwoBlock(it.first, it.second,
                        &codeBlockTableTemp);
                if (substituteSuccess) {
                    substitudeID.insert(it.first);
                }

            }
            //3 正常情况下，除了最后一个block（root），其他block都会被整合（代入），
            //也就是说，如果原block集合与其中被整合的block的集合做差，肯定只剩一个block根
            set<Intg> substitude_result;            //代入后的结果
            set_difference(blockIDset.begin(), blockIDset.end(),
                    substitudeID.begin(), substitudeID.end(),
                    inserter(substitude_result, substitude_result.begin()));
            if (substitude_result.size() != 1) {
                //todo::匹配有点问题
#if debug
                {
                    static int ignorecount = 0;
                    cout << "integrateBlock(set<int>)" << " err ,exit(-1)"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "vm_integrate.hpp:" << __LINE__ << ":0" << ")"
                            << endl;

                }
#endif

                exit(-1);
            } else {
                //无需为代码段重新分配地址，因为地址顺序本就是正确的，（虽然地址不正确）
                //直接用integrator对block中的代码段进行匹配
                CodeBlock &resultcb =
                        codeBlockTableTemp[(*substitude_result.begin())];
                CodeTable &cdt = resultcb.codeTable;
                unfoldRingStruct(cdt);
                Addr &addrdes = cdt.addrdq[0];

                MultiTFIntegrator mi(codeTable, addrdes, templateFunctionTable,
                        scopeTable, systemTable, currentAR);
                mi.setModeSeparate(&cdt);
                if (mi.integrate()) {
                    //匹配成功
                    return codeBlockTableTemp[(*substitude_result.begin())];
                } else {
                    return CodeBlock();
                }

            }

        }
        return CodeBlock();

    }
    void setBackwardFunctionInfo(Addr &bfnScopeAddr, CodeTable *bfbScopeCdt) {
        this->bfbScopeCdt = bfbScopeCdt;
        this->bfnScopeAddr = bfnScopeAddr;
    }
    /**
     * 以arg为模板返回一个新的arg，这个arg的标识符除argFlag外继承原arg
     * @param arg
     * @return
     */
    Arg getNewArg(const Arg &arg) {
        Arg arg_(arg);
        arg_.argFlag = S_Bs;
        arg_.arg_a = addrnull;
        arg_.arg_i = 0;
        arg_.arg_s = "_ARG_" + toStrg(++argID);
        return arg_;
    }
    /**
     * 根据当前argKnown、argUnknown设置cdt代码的可变属性，
     * 在此过程中会进一步修改argKnown，argUnknown、temporaryArg_occ_map、temporaryArg_id_map的值
     * @brief 判断方法：
     * 1 当此函数的所有参数中至少包含一个未知参数时
     * 2 该未知参数到根节点路径上的参数
     * 3 该表达式上所有待定（被修改）的参数到最终根节点的参数，换句话说，这一步要将所有的可变变量变成待定变量
     *@attention 注意，这一步需要对cdt进行参数值替换，并返回CodeBlock
     *@attention 注意，如果cdt中存在赋值“=”语句，那么一个值在被赋值后的后续语句中只能被引用，不能再被主动修改。
     * @param cdt
     * @return cb
     */
    CodeBlock setUnknownAttributeAndBlock(CodeTable &cdt) {
#if debug
        {
            static int ignorecount = 0;
            cout << "setUnknownAttributeAndBlock(CodeTable&)" << " "
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        numID += 2;
        CodeBlock cb(M, false, numID, currentScopeID);
        cb.codeTable = cdt;
        //先进行代码参数替换，如果原值没有change（或赋值号），则直接用新值替换；
        //如果原值change了（或赋值了），说明原新值已经变旧，用新的新值替换原新值
        //注意！用新的新值替换掉旧值后，新的新值不可再被替换！！！！
        //所有值在一个表达式中最多change一次!!!!
        set<Arg> changedarg;
        for (auto &it : cb.codeTable.codedq) {
            Arg arg1 = it[0];
            Arg arg2 = it[1];
            Arg argop = it[2];
            Arg argres = it[3];

            Arg &newarg1 = old_new_map[arg1];
            if ((arg1.changeable
                    || (argop.argFlag == M_Bs && argop.arg_s == "="))
                    && changedarg.count(arg1) <= 0) {
                if (temporaryArg_occ_map.count(arg1) <= 0) {
                    newarg1 = getNewArg(arg1);
                    changedarg.insert(arg1);
                    changedarg.insert(newarg1);
                    newarg.insert(newarg1);        //告诉stepper有新创建的参数。
                } else if (temporaryArg_occ_map[arg1] == 1) {
                    temporaryArg_occ_map[arg1] = 2;
                    temporaryArg_id_map[arg1].insert(cb.id);
                } else {
#if debug
                    {
                        static int ignorecount = 0;
                        cerr << "setUnknownAttributeAndBlock(CodeTable&)"
                                << " temporaryArg err arg1" << "\tignorecount:["
                                << ignorecount++ << "\t]("
                                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                                << __LINE__ << ":0" << ")" << endl;

                    }
#endif
                    exit(-1);
                }

            } else {
                if (newarg1 == argnull) {
                    newarg1 = arg1;
                }
            }
            it.setArg(0, newarg1);

            Arg &newarg2 = old_new_map[arg2];
            if (arg2.changeable && changedarg.count(arg2) <= 0) {
                if (temporaryArg_occ_map.count(arg2) <= 0) {
                    newarg2 = getNewArg(arg2);
                    changedarg.insert(arg2);
                    changedarg.insert(newarg2);
                    newarg.insert(newarg2);
                } else if (temporaryArg_occ_map[arg2] == 1) {
                    temporaryArg_occ_map[arg2] = 2;
                    temporaryArg_id_map[arg2].insert(cb.id);
                } else {
#if debug
                    {
                        static int ignorecount = 0;
                        cerr << "setUnknownAttributeAndBlock(CodeTable&)"
                                << " temporaryArg err arg2" << "\tignorecount:["
                                << ignorecount++ << "\t]("
                                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                                << __LINE__ << ":0" << ")" << endl;

                    }
#endif
                    exit(-1);
                }

            } else {
                if (newarg2 == argnull) {
                    newarg2 = arg2;
                }
            }
            it.setArg(1, newarg2);

            Arg &newargop = old_new_map[argop];
            if (argop.changeable && changedarg.count(argop) <= 0) {
                if (temporaryArg_occ_map.count(argop) <= 0) {
                    newargop = getNewArg(argop);
                    changedarg.insert(argop);
                    changedarg.insert(newargop);
                    newarg.insert(newargop);
                } else if (temporaryArg_occ_map[argop] == 1) {
                    temporaryArg_occ_map[argop] = 2;
                    temporaryArg_id_map[argop].insert(cb.id);
                } else {
#if debug
                    {
                        static int ignorecount = 0;
                        cerr << "setUnknownAttributeAndBlock(CodeTable&)"
                                << " temporaryArg err op" << "\tignorecount:["
                                << ignorecount++ << "\t]("
                                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                                << __LINE__ << ":0" << ")" << endl;

                    }
#endif
                }

            } else {

                if (newargop == argnull) {
                    newargop = argop;
                }
            }
            it.setArg(2, newargop);

            Arg &newargres = old_new_map[argres];
            if ((argres.changeable
                    || (argop.argFlag == M_Bs && argop.arg_s == "="))
                    && changedarg.count(argres) <= 0) {
                //赋值情况
                if (argop.argFlag == M_Bs && argop.arg_s == "=") {
                    newargres = newarg1;
                } else if (argres.argFlag == S_Dz
                        && temporaryArg_occ_map.count(argres) <= 0) {
                    //添加为临时变量，出现次数置为1
                    temporaryArg_occ_map[argres] = 1;
                    newargres = argres;
                    cb.argTemporary.insert(argres);
                    temporaryArg_id_map[argres].insert(cb.id);
                } else if (temporaryArg_occ_map.count(argres) > 0) {
                    //临时变量不可能作为res出现两次，报错
#if debug
                    {
                        static int ignorecount = 0;
                        cerr << "setUnknownAttributeAndBlock(CodeTable&)"
                                << " temporaryArg err res" << "\tignorecount:["
                                << ignorecount++ << "\t]("
                                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                                << __LINE__ << ":0" << ")" << endl;

                    }
#endif
                } else {
                    //一般情况(这一步虽然比较冗余，但先这么写吧，结果变量可能不需要创建)
                    newargres = getNewArg(argres);
                    changedarg.insert(argres);
                    changedarg.insert(newargres);
                    newarg.insert(newargres);
                }
            } else {
                if (newargres == argnull) {
                    newargres = argres;
                }
            }
            it.setArg(3, newargres);

        }
        //推导参数可知性
        //一些参数在替换后由不可知变成了可知，
        //这是因为他们的值被改变了，本质上与原参数不是同一个参数了，属于正常现象。
        //如果在首个表达式中修改了所有参数，甚至可能导致所有变量已知（即与输入待定量无关）
        for (auto &it : cb.codeTable.codedq) {
            Arg arg1 = it[0];
            Arg arg2 = it[1];
            Arg argop = it[2];
            Arg argres = it[3];
            if (argUnknown.count(arg1)) {
                arg1.unknown = T_;
                it.assemblyFormula.flagBit.unknown[0] = T_;
            }
            if (argUnknown.count(arg2)) {
                arg2.unknown = T_;
                it.assemblyFormula.flagBit.unknown[1] = T_;
            }
            if (argUnknown.count(argop)) {
                argop.unknown = T_;
                it.assemblyFormula.flagBit.unknown[2] = T_;
            }
            if (argUnknown.count(argres)) {
                cb.dependent = true;
                argres.unknown = T_;
                it.assemblyFormula.flagBit.unknown[3] = T_;
            } else if (arg1.unknown || arg2.unknown || argop.unknown) {
                cb.dependent = true;
                argres.unknown = T_;
                it.assemblyFormula.flagBit.unknown[3] = T_;

            }

        }
        //存在不可知情况dependent = true（依赖未知值）：
        //对于不可知情况，应将所有标志位为不可知的变量的changeable置为true，
        //同时所有changeable变量的unknown置为true。
        if (cb.dependent) {
            cb.codeTable.unbindFunction(cb.codeTable.addrdq.front());
            map<Arg, Arg> old_new_map_temp;
            for (auto &it : cb.codeTable.codedq) {
                Arg arg1 = it[0];
                Arg arg2 = it[1];
                Arg argop = it[2];
                Arg argres = it[3];

                if (arg1.changeable || arg1.unknown) {

                    it.assemblyFormula.flagBit.changeable[0] = T_;
                    it.assemblyFormula.flagBit.unknown[0] = T_;
                    argUnknown.insert(arg1);
                    cb.addUnknownArg(arg1);
                    unknown_id_map[arg1].insert(cb.id);
                }
                if (arg2.changeable || arg2.unknown) {

                    it.assemblyFormula.flagBit.changeable[1] = T_;
                    it.assemblyFormula.flagBit.unknown[1] = T_;

                    argUnknown.insert(arg2);
                    cb.addUnknownArg(arg2);
                    unknown_id_map[arg2].insert(cb.id);
                }
                if (argop.changeable || argop.unknown) {

                    it.assemblyFormula.flagBit.changeable[2] = T_;
                    it.assemblyFormula.flagBit.unknown[2] = T_;

                    argUnknown.insert(argop);
                    cb.addUnknownArg(argop);
                    unknown_id_map[argop].insert(cb.id);
                }
                if (argres.changeable || argres.unknown || arg1.changeable
                        || arg1.unknown || arg2.changeable || arg2.unknown
                        || argop.changeable || argop.unknown) {

                    it.assemblyFormula.flagBit.changeable[3] = T_;
                    it.assemblyFormula.flagBit.unknown[3] = T_;

                    cb.addUnknownArg(argres);
                    argUnknown.insert(argres);
                    unknown_id_map[argres].insert(cb.id);
                }

            }

            //todo:: 此作用域内代码可以优化，即去除对中间变量的新建变量。
        }
        return cb;
    }
    /**
     * 分析函数名称表达式，获取已知未知参数,初始化old_new_map
     */
    void analysisBFN() {

        mustKnown.insert(Arg(S_Bs, "ans"));
        Scope &bfnScope = (*scopeTable)[bfnScopeAddr];
        Intg posBegin = getAddrPosition(codeTable->addrdq, bfnScope.scopeCoor1);
        Intg posEnd = getAddrPosition(codeTable->addrdq, bfnScope.scopeCoor2);
        ++posBegin;
        while (posBegin < posEnd) {
            Code &fncd = (*codeTable)[posBegin];
            Arg &&arg1 = fncd[0];
            Arg &&arg2 = fncd[1];
            Arg &&argop = fncd[2];
            Arg &&argres = fncd[3];
            if (arg1.argFlag == S_Bs) {
                if (arg1.changeable) {
                    argUnknown.insert(arg1);
                    argKnown.erase(arg1);
                } else if (argUnknown.count(arg1) <= 0) {
                    argKnown.insert(arg1);
                }
            }
            if (arg2.argFlag == S_Bs) {
                if (arg2.changeable) {
                    argUnknown.insert(arg2);
                    argKnown.erase(arg2);
                } else if (argUnknown.count(arg2) <= 0) {
                    argKnown.insert(arg2);
                }
            }
            if (argop.argFlag == S_Bs) {
                if (argop.changeable) {
                    argUnknown.insert(argop);
                    argKnown.erase(argop);
                } else if (argUnknown.count(argop) <= 0) {
                    argKnown.insert(argop);
                }
            }
            if (argres.argFlag == S_Bs) {
                if (argres.changeable) {
                    argUnknown.insert(argres);
                    argKnown.erase(argres);
                } else if (argUnknown.count(argres) <= 0) {
                    argKnown.insert(argres);
                }
            }
            ++posBegin;
        }
        for (auto it : argUnknown) {
            old_new_map.insert(pair<Arg, Arg>(it, argnull));
        }
        argUnknown.erase(*(mustKnown.begin()));
        argKnown.insert(mustKnown.begin(), mustKnown.end());
    }
    BackStepper(Integrator *integrator);
//2 进行第一次正向执行，修改Z_Dz地址
    void backwardFunctionFirstForwardExecution();

//3 第二次正向执行
    void backwardFunctionSecondForwardExecution();
//3 第3次正向执行
    void backwardFunctionThirdForwardExecution();
//3 第一次反向执行
    void backwardFunctionFirstBackwardExecution();
//4 将代码整合到程序中
    /**
     * 代码段实际地址取决于 split函数，这一步会对原scopeAddr进行优化
     * @param appendAddr 修改的代码段将插入appendAddr之后
     * @param addrmin 修改后的代码段的起始地址
     * @param addrmax 修改后代码段终止地址
     */
    void backwardFunctionIntegrate(const Addr &appendAddr, const Addr &addrmin,
            const Addr &addrmax);
};
class Integrator {
public:
    Intg matchStateSiloMaxCapacity = 30;
    Intg codeTableSiloMaxCapacity = 300;
    Intg maxSwitchTurn = 30;
    Scanner *scanner = NULL;

    CodeTable *codeTable = NULL;
    ScopeTable *scopeTable = NULL;
    TemplateFunctionTable *templateFunctionTable = NULL;
    DataTable *currentAR = NULL;
    SystemTable *systemTable = NULL;

    Intg currentCodePos = -1;
    Addr currentCodeAddr;

    Code *currentCode = NULL;

    Intg nextCodePos = 0;
    Addr nextCodeAddr;

    Code *nextCode = NULL;
    const bool getNewCode();
    const bool setNextCodePos(Intg &pos);
    const bool integrate();

    const bool setArgScopeAddr();
    const bool addArgsToCurrentAR();
    Integrator() {

    }
    Integrator(CodeTable *codeTable,
            TemplateFunctionTable *templateFunctionTable,
            ScopeTable *scopeTable, DataTable *dataTable,
            SystemTable *systemTable) {
        this->codeTable = codeTable;
        this->templateFunctionTable = templateFunctionTable;
        this->scopeTable = scopeTable;
        this->currentAR = dataTable;
        this->systemTable = systemTable;
    }
    void setMaxCapacity(Intg matchStateSiloMaxCapacity,
    Intg codeTableSiloMaxCapacity, Intg maxSwitchTurn) {
        this->matchStateSiloMaxCapacity = matchStateSiloMaxCapacity;
        this->codeTableSiloMaxCapacity = codeTableSiloMaxCapacity;
        this->maxSwitchTurn = maxSwitchTurn;
    }
};
BackStepper::BackStepper(Integrator *integrator) {
    this->integrator = integrator;
    this->codeTable = integrator->codeTable;
    this->scopeTable = integrator->scopeTable;
    this->currentAR = integrator->currentAR;
    this->templateFunctionTable = integrator->templateFunctionTable;
    this->systemTable = integrator->systemTable;
}
/**
 * 虽然名字为setNextCodePos，但是这个函数并不主动寻找nextCodePos，
 * 而是检测传入的nextCodePos是否有效，无效的话寻找下一个有效的pos，否则不变
 * @param pos
 * @return
 */
const bool Integrator::setNextCodePos(Intg &pos) {
    if (pos >= (Intg) codeTable->addrdq.size()) {
        pos = 0;
        return false;
    } else if (pos == 0) {
        return false;
    }

    if (
//            (*codeTable)[pos].assemblyFormula.flagBit.execute == F_
//            ||
    (*codeTable)[pos].assemblyFormula.flagBit.execute == Du) {
        pos++;
        return setNextCodePos(pos);
    } else if ((*codeTable)[pos].assemblyFormula.flagBit.execute == R
            && (*codeTable)[pos].assemblyFormula.flagBit.type == QS) {
        {
#if debug
            static int ignorecount = 0;
            cout << "Integrator::setNextCodePos(int&)" << " "
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "vm_integrate.hpp:" << __LINE__ << ":0" << ")" << endl;
#endif
        }
        Addr &scopeAddrJump = (*codeTable)[pos].scopeAddr;
        Addr &functionNameScope = (*scopeTable)[scopeAddrJump].requireScopeAddr;
        if (functionNameScope == addrnull
                || (*scopeTable)[scopeAddrJump].conditionFlag != 0) {
            //这是条件语句条件体，需要进入匹配
            return true;
        }
        if ((*templateFunctionTable)[functionNameScope].integrate == true) {
            //这是返回表达式的函数的函数体，不需要进入匹配
            Addr &coor2 = (*scopeTable)[scopeAddrJump].scopeCoor2;
            pos = getAddrPosition(codeTable->addrdq, coor2);
            ++pos;
#if debug
            cout << "Executer::setNextCodePos 5, nextCodeAddr:["
                    << nextCodeAddr.toStrg() << "]" << endl;
#endif
            return setNextCodePos(pos);
        } else {
            //这是返回运算值的函数的函数体，需要进入匹配
            return true;
        }

    }
    return true;
}

const bool Integrator::getNewCode() {
    if (nextCodeAddr != addrnull) {
        nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
    }

    if (nextCodePos < 0) {
        return false;
    }
    currentCodePos = nextCodePos;
    nextCodePos++;
    setNextCodePos(nextCodePos);
    if (currentCodePos < (Intg) codeTable->addrdq.size() - 1) {

        currentCodeAddr = codeTable->addrdq[currentCodePos];
        currentCode = &(*codeTable)[currentCodePos];

        if (nextCodePos < (Intg) codeTable->addrdq.size()) {
            nextCodeAddr = codeTable->addrdq[nextCodePos];
            nextCode = &(*codeTable)[nextCodePos];

        }

        return true;

    } else if (currentCodePos == (Intg) codeTable->addrdq.size() - 1) {
        currentCodeAddr = codeTable->addrdq[currentCodePos];
        currentCode = &(*codeTable)[currentCodePos];
        nextCodeAddr = addrnull;
        nextCode = NULL;
        nextCodePos = -1;
        return true;
    }
    return false;
}
const bool Integrator::setArgScopeAddr() {
    if ((*currentCode).assemblyFormula.flagBit.type == C
            && (*currentCode)[0].argFlag == S_Bs
            && (*currentCode)[1].argFlag == X_Bs) {
        (*currentCode).assemblyFormula.flagBit.a1s = (*currentCode).scopeAddr;
        (*currentCode).assemblyFormula.flagBit.res = (*currentCode).scopeAddr;

    } else if ((*currentCode).assemblyFormula.flagBit.type == M
            && (*currentCode).assemblyFormula.flagBit.execute == F_) {
        if (((*currentCode).assemblyFormula.flagBit.result_flag == S_Dz)
                && (*currentCode).assemblyFormula.flagBit.formalArgFlag[3]
                        == true) {
            (*currentAR).add((*currentCode)[3]);
        }
#if debug
        cout << "Integrator::setArgScopeAddr a1s" << endl;
#endif
        if ((*currentCode)[0].isVar()) {
            try {
                (*currentCode).assemblyFormula.flagBit.a1s =
                        currentAR->getArgScopeAddr((*currentCode)[0]);
            } catch (Arg &var_not_exist) {
                (*currentCode).assemblyFormula.flagBit.a1s =
                        (*currentCode).scopeAddr;
            }
        }

#if debug
        cout << "Integrator::setArgScopeAddr a2s" << endl;
#endif
        if ((*currentCode)[1].isVar()) {
            try {
                (*currentCode).assemblyFormula.flagBit.a2s =
                        currentAR->getArgScopeAddr((*currentCode)[1]);
            } catch (Arg &var_not_exist) {
                (*currentCode).assemblyFormula.flagBit.a2s =
                        (*currentCode).scopeAddr;
            }
        }

#if debug
        cout << "Integrator::setArgScopeAddr ops" << endl;
#endif
        if ((*currentCode)[2].isVar()) {
            try {
                (*currentCode).assemblyFormula.flagBit.ops =
                        currentAR->getArgScopeAddr((*currentCode)[2]);
            } catch (Arg &var_not_exist) {
                (*currentCode).assemblyFormula.flagBit.ops =
                        (*currentCode).scopeAddr;
            }
        }
#if debug
        cout << "Integrator::setArgScopeAddr res" << endl;
#endif
        if ((*currentCode)[3].isVar()) {
            try {
                (*currentCode).assemblyFormula.flagBit.res =
                        currentAR->getArgScopeAddr((*currentCode)[3]);
            } catch (Arg &var_not_exist) {
                (*currentCode).assemblyFormula.flagBit.res =
                        (*currentCode).scopeAddr;
            }
        }
#if debug
        cout << "Integrator::setArgScopeAddr end" << endl;
#endif

    } else if ((*currentCode).assemblyFormula.flagBit.type == R
            && (*currentCode).assemblyFormula.flagBit.execute == T_) {
        (*currentCode).assemblyFormula.flagBit.a1s = currentAR->getArgScopeAddr(
                (*currentCode)[0]);

    } else if ((*currentCode).assemblyFormula.flagBit.type == M
            && (*currentCode).assemblyFormula.flagBit.execute == T_) {
        if (((*currentCode).assemblyFormula.flagBit.result_flag == S_Dz)
                && (*currentCode).assemblyFormula.flagBit.formalArgFlag[3]
                        == true) {
            (*currentAR).add((*currentCode)[3]);
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "Integrator::setArgScopeAddr()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if ((*currentCode)[0].isVar()) {
            try {
                (*currentCode).assemblyFormula.flagBit.a1s =
                        currentAR->getArgScopeAddr((*currentCode)[0]);
            } catch (Arg &var_not_exist) {
                (*currentCode).assemblyFormula.flagBit.a1s =
                        (*currentCode).scopeAddr;
            }
        }

#if debug
        {
            static int ignorecount = 0;
            cout << "Integrator::setArgScopeAddr()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if ((*currentCode)[1].isVar()) {
            try {
                (*currentCode).assemblyFormula.flagBit.a2s =
                        currentAR->getArgScopeAddr((*currentCode)[1]);
            } catch (Arg &var_not_exist) {
                (*currentCode).assemblyFormula.flagBit.a2s =
                        (*currentCode).scopeAddr;
            }
        }

#if debug
        {
            static int ignorecount = 0;
            cout << "Integrator::setArgScopeAddr()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if ((*currentCode)[2].isVar()) {
            try {
                (*currentCode).assemblyFormula.flagBit.ops =
                        currentAR->getArgScopeAddr((*currentCode)[2]);
            } catch (Arg &var_not_exist) {
                (*currentCode).assemblyFormula.flagBit.ops =
                        (*currentCode).scopeAddr;
            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "Integrator::setArgScopeAddr()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if ((*currentCode)[3].isVar()) {
            try {
                (*currentCode).assemblyFormula.flagBit.res =
                        currentAR->getArgScopeAddr((*currentCode)[3]);
            } catch (Arg &var_not_exist) {
                (*currentCode).assemblyFormula.flagBit.res =
                        (*currentCode).scopeAddr;
            }
        }
#if debug
        {
            static int ignorecount = 0;
            cout << "Integrator::setArgScopeAddr()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif

    } else if ((*currentCode).assemblyFormula.flagBit.type == QS
            && (*currentCode).assemblyFormula.flagBit.execute == F_) {
        (*currentCode).assemblyFormula.flagBit.res =
                (*currentCode).assemblyFormula.quaternion.result_a;

    } else if ((*currentCode).assemblyFormula.flagBit.type == QE
            && (*currentCode).assemblyFormula.flagBit.execute == F_) {
        (*currentCode).assemblyFormula.flagBit.res =
                (*currentCode).assemblyFormula.quaternion.result_a;

    } else if ((*currentCode).assemblyFormula.flagBit.type == QS
            && (*currentCode).assemblyFormula.flagBit.execute == R) {
        (*currentCode).assemblyFormula.flagBit.res =
                (*currentCode).assemblyFormula.quaternion.result_a;

    } else if ((*currentCode).assemblyFormula.flagBit.type == QE
            && (*currentCode).assemblyFormula.flagBit.execute == R) {
        (*currentCode).assemblyFormula.flagBit.res =
                (*currentCode).assemblyFormula.quaternion.result_a;

    } else if ((*currentCode).assemblyFormula.flagBit.type == F) {
        (*currentCode).assemblyFormula.flagBit.a1s =
                (*currentCode).assemblyFormula.quaternion.arg1_a;
        (*currentCode).assemblyFormula.flagBit.a2s =
                (*currentCode).assemblyFormula.quaternion.arg2_a;
        (*currentCode).assemblyFormula.flagBit.res =
                (*currentCode).assemblyFormula.quaternion.result_a;
    } else if ((*currentCode).assemblyFormula.flagBit.type == TD) {
        (*currentCode).assemblyFormula.flagBit.a1s =
                (*currentCode).assemblyFormula.quaternion.arg1_a;
        (*currentCode).assemblyFormula.flagBit.a2s =
                (*currentCode).assemblyFormula.quaternion.arg2_a;
        (*currentCode).assemblyFormula.flagBit.res =
                (*currentCode).assemblyFormula.quaternion.result_a;
    }

    return true;
}
const bool Integrator::addArgsToCurrentAR() {
    if ((*currentCode).assemblyFormula.flagBit.type == C
            && (*currentCode)[0].argFlag == S_Bs
            && (*currentCode)[1].argFlag == X_Bs) {
        if ((*currentCode)[1].arg_s == "new"
                || (*currentCode)[1].arg_s == "out") {
            (*currentAR).add((*currentCode)[0]);
        } else {
//创建类的实例，直接使用类的AR
            (*currentAR).add((*currentCode)[0]);
            System &sys = (*systemTable)[pair<Strg, Addr>(
                    currentCode->assemblyFormula.quaternion.arg2_s,
                    currentCode->scopeAddr)];
            (*currentAR)[(*currentCode)[0]] = (*currentAR)[Arg(S_Dz,
                    sys.systemAddr)];
        }

    } else if ((*currentCode).assemblyFormula.flagBit.type == M
            && (*currentCode).assemblyFormula.flagBit.execute != Du) {
        /**
         * 不用管引用，引用已经被置于M之前，已经被处理过了
         */

        Arg &&arg_1 = (*currentCode)[0];
        Arg &&arg_2 = (*currentCode)[1];
        Arg &&arg_op = (*currentCode)[2];
        Arg &&arg_res = (*currentCode)[3];
        if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
            (*currentAR).add((*currentCode)[0]);
        }
        if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
            (*currentAR).add((*currentCode)[1]);
        }
        if (arg_op.isVar() && arg_op.argFlag != Y_Dz) {
            (*currentAR).add((*currentCode)[2]);
        }
        if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
            (*currentAR).add((*currentCode)[3]);
        }
    } else if ((*currentCode).assemblyFormula.flagBit.type == Y
            && (*currentCode).assemblyFormula.flagBit.execute != Du) {
        Arg &&arg_1 = (*currentCode)[0];
        Arg &&arg_2 = (*currentCode)[1];
        Arg &&arg_op = (*currentCode)[2];
        Arg &&arg_res = (*currentCode)[3];
        //引用的arg1、arg2无意义，故忽略其添加
        /*if (arg_1.isVar() && arg_1.argFlag != Y_Dz) {
         (*currentAR).add((*currentCode)[0]);
         } else if ((*currentCode).assemblyFormula.flagBit.arg1_flag == Y_Dz) {

         arg_1.arg_ar = currentAR;
         (*currentAR).add(arg_1);
         (*currentAR)[arg_1].content_ar = currentAR;
         }
         if (arg_2.isVar() && arg_2.argFlag != Y_Dz) {
         (*currentAR).add((*currentCode)[1]);
         } else if ((*currentCode).assemblyFormula.flagBit.arg2_flag == Y_Dz) {

         arg_2.arg_ar = currentAR;
         (*currentAR).add(arg_2);
         (*currentAR)[arg_2].content_ar = currentAR;
         }
         if ((*currentCode).assemblyFormula.flagBit.operator_flag == S_Dz) {
         (*currentAR).add((*currentCode)[2]);
         } else if ((*currentCode).assemblyFormula.flagBit.operator_flag == Y_Dz) {
         globalRefTable.addRefData(this->currentAR, codeTable,
         currentCodeAddr, 2);

         arg_op.arg_ar = currentAR;
         (*currentAR).add(arg_op);
         (*currentAR)[arg_op].content_ar = currentAR;
         }*/
        if (arg_res.isVar() && arg_res.argFlag != Y_Dz) {
            (*currentAR).add((*currentCode)[3]);
        } else if ((*currentCode).assemblyFormula.flagBit.result_flag == Y_Dz) {
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
                    cout << "Integrator::addArgsToCurrentAR()"
                            << " referencePath_include_immed arg:["
                            << referencePath_include_immed.toStrg() << "]\t"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_scan.hpp:"
                            << __LINE__ << ":0" << ")" << endl;
                }
#endif
            }
        }

    } else if ((*currentCode).assemblyFormula.flagBit.type == M
            && (*currentCode).assemblyFormula.flagBit.execute == F_) {
        (*currentAR).add((*currentCode)[0]);
        (*currentAR).add((*currentCode)[1]);
        (*currentAR).add((*currentCode)[2]);
        (*currentAR).add((*currentCode)[3]);

    } else if ((*currentCode).assemblyFormula.flagBit.type == QS
            && (*currentCode).assemblyFormula.flagBit.execute == F_) {
        (*currentAR).add((*currentCode)[3]);
        (*currentAR)[(*currentCode).scopeAddr].dataFlagBit = S_AR;
        safe_delete((*currentAR)[(*currentCode).scopeAddr].content_ar);
        (*currentAR)[(*currentCode).scopeAddr].content_ar = new DataTable(
                currentAR, currentAR, (*currentCode).scopeAddr);
        currentAR = (*currentAR)[(*currentCode).scopeAddr].content_ar;

    } else if ((*currentCode).assemblyFormula.flagBit.type == QE
            && (*currentCode).assemblyFormula.flagBit.execute == F_) {

        (*currentAR)[(*currentCode)[3]].dataFlagBit = S_AR;

        currentAR = currentAR->parentAR;

    } else if ((*currentCode).assemblyFormula.flagBit.type == R
            && (*currentCode).assemblyFormula.flagBit.execute == R) {

    } else if ((*currentCode).assemblyFormula.flagBit.type == QS
            && (*currentCode).assemblyFormula.flagBit.execute == R) {
        if ((*scopeTable)[currentCode->scopeAddr].functionFlag == SYSB) {
            //预执行过程中的继承处理
            (*currentAR).add((*currentCode)[3]);
            Data &sysData = (*currentAR)[(*currentCode).scopeAddr];
            sysData.dataFlagBit = S_AR;
            safe_delete(sysData.content_ar);

            DataTable *sysAR = sysData.content_ar = new DataTable(currentAR,
                    currentAR, currentAR, (*currentCode).scopeAddr, SYSB);
            sysAR->scopeStructureAddr = currentCode->scopeAddr;
            currentAR = sysAR;

            System &currentSystem = (*systemTable)[currentCode->scopeAddr];
            //将继承类的预执行实例直接添加到querylist中完成继承
            for (auto &id : currentSystem.decedentSystemIDList) {
                sysAR->queryARList.push_front(
                        (*currentAR->parentAR)[(*systemTable)[id].systemAddr].content_ar);
            }

        } else {
            //预执行过程中的函数调用等处理
            (*currentAR).add((*currentCode)[3]);
            (*currentAR)[(*currentCode).scopeAddr].dataFlagBit = S_AR;

            safe_delete((*currentAR)[(*currentCode).scopeAddr].content_ar);
            DataTable *queryAR_ = NULL;
            if ((*scopeTable)[(*currentCode).scopeAddr].requireScopeAddr
                    != addrnull) {
                queryAR_ =
                        (*currentAR)[(*scopeTable)[(*currentCode).scopeAddr].requireScopeAddr].content_ar;
            } else {
                queryAR_ = currentAR;
            }

            DataTable * funAR = (*currentAR)[(*currentCode).scopeAddr].content_ar = new DataTable(
                    currentAR, queryAR_, (*currentCode).scopeAddr);
            funAR->scopeStructureAddr = currentCode->scopeAddr;
            currentAR = funAR;
        }

    } else if ((*currentCode).assemblyFormula.flagBit.type == QE
            && (*currentCode).assemblyFormula.flagBit.execute == R) {
        Arg &&scopeArg = (*currentCode)[3];
        (*currentAR)[scopeArg].dataFlagBit = S_AR;

        currentAR = currentAR->parentAR;

    } else if ((*currentCode).assemblyFormula.flagBit.type == TD
            && (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
        {
            static int ignorecount = 0;
            cout << "Integrator::addArgsToCurrentAR()" << " TD "
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        //todo::logic here

        //1
        //复制原函数的作用域SCOPE，将该scope添加至codeTable表中，并与函数进行绑定。
        //由于作用域内部地址不用管，只需要修改codeaddr
        //为了能合适地将scope添加到codeTable中，在compiler时已经在T类型后面连续添加了多个ME类型代码
        //因此,将这些ME代码删除后可以放心添加
        Addr &ffbScopeAddr =
                (*templateFunctionTable)[currentCode->assemblyFormula.quaternion.arg2_a].bodyScopeAddr;
        Addr ffbScopeAddrLowBound = (*scopeTable)[ffbScopeAddr].scopeCoor1;
        Addr ffbScopeAddrUpBound = (*scopeTable)[ffbScopeAddr].scopeCoor2;
        CodeTable &&ffbScopeCdt = codeTable->copyCodeTable(ffbScopeAddrLowBound,
                ffbScopeAddrUpBound);

        Addr &bfbScopeAddrLowBound = currentCodeAddr;
        const Addr &&bfbScopeAddrUpBound = codeTable->getNextNot(ME,
                currentCodeAddr);
        //注意！此步骤中没有修改Z_Dz地址，后续一定要修改
        ffbScopeCdt.resetAddrRange(bfbScopeAddrLowBound, bfbScopeAddrUpBound);
        CodeTable &bfbScopeCdt = ffbScopeCdt;
        Addr bfnScopeAddr = (*currentCode).assemblyFormula.quaternion.arg1_a;

        Intg pos = getAddrPosition(codeTable->addrdq, currentCodeAddr);
        pos++;
        while ((*codeTable)[pos].assemblyFormula.flagBit.type == ME) {
            codeTable->eraseCode(pos++, 0);

        }
        BackStepper bs(this);
        bs.bfbScopeCdt = &bfbScopeCdt;
        bs.bfnScopeAddr = bfnScopeAddr;
        //2 进行第一次正向执行，解除绑定，修改Z_Dz地址
        bs.backwardFunctionFirstForwardExecution();

        //3 第二次正向执行
        bs.backwardFunctionSecondForwardExecution();
        // 第三次正向执行
        bs.backwardFunctionThirdForwardExecution();
        //3 第一次反向执行
        bs.backwardFunctionFirstBackwardExecution();
        //4 整合
        bs.backwardFunctionIntegrate(currentCodeAddr, currentCodeAddr,
                codeTable->getNextCodeAddr(currentCodeAddr));

        //5 设置nextCodeAddr
//        nextCodeAddr = codeTable->getNextCodeAddr(currentCodeAddr);
        nextCodeAddr = bfbScopeAddrUpBound;

    }

    return true;
}

/**
 * @brief 根据逆函数的输入重新逐步确定可变标志位（直接根据基本表达式的四元式用到的操作符来确定），
 * 而且受输入变量中影响的待定系数的标志位会始终向后传递,不会被“；”所终止。
 * 使用到待定变量的与函数绑定的代码段会取消其绑定效果，同时在执行过程中注意修改内部代码的地址。
 * @param bfnScopeAddr
 * @param bfnScopeCdt
 */
void BackStepper::backwardFunctionFirstForwardExecution() {

    analysisBFN();
//2 根据逆函数的输入重新逐步确定可变标志位.这个循环会遍历所有Code，请在这一步中完成
//2.1对作用域地址的重新赋值并将产生的新作用域添加到（局部）作用域表中，
//内层作用域可能不需要添加，但最外层一定要添加
//2.2

    currentFBAd = (*bfbScopeCdt).addrdq.front();
    currentFBCd = &(*bfbScopeCdt)[currentFBAd];
    Addr nextFBAd = currentFBAd;
    for (; currentFBAd != addrnull; currentFBAd = nextFBAd) {
#if debug
        {
            static int ignorecount = 0;
            cout << "BackStepper::backwardFunctionFirstForwardExecution()"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if (currentFBAd != addrnull) {
            currentFBCd = &(*bfbScopeCdt)[currentFBAd];
        }
        if (nextFBAd != addrnull) {
            nextFBAd = (*bfbScopeCdt).getNextCodeAddr(nextFBAd);
        }
        currentScopeAddr = currentFBCd->scopeAddr;

        //2.1 将bfb拆解为block
        if ((*currentFBCd).assemblyFormula.flagBit.type == QS) {

            ++numID;
            currentScopeID = numID;
            codeBlockTable[numID] = CodeBlock(QS, F_, numID, currentScopeID);
            codeBlockTable[numID].QSid = numID;
            codeBlockTable[numID].codeTable.addCode(currentFBAd, *currentFBCd);
            scopeIDStack.push_back(currentScopeID);
            continue;

        } else if ((*currentFBCd).assemblyFormula.flagBit.type == QE) {

            ++numID;
            codeBlockTable[numID] = CodeBlock(QE, F_, numID, currentScopeID);
            codeBlockTable[numID].codeTable.addCode(currentFBAd,
                    (*currentFBCd));
            codeBlockTable[currentScopeID].QEid = numID;
            scopeIDStack.pop_back();
            currentScopeID = scopeIDStack.back();

            continue;

        } else if ((*currentFBCd).assemblyFormula.flagBit.type == ME
                || (*currentFBCd).assemblyFormula.flagBit.execute == Du) {
            (*bfbScopeCdt).eraseCode(currentFBAd, 0);
            continue;
        } else if (currentFBCd->assemblyFormula.flagBit.type == M) {
            CodeTable &&cdtmid = bfbScopeCdt->copyFunctionCall(currentFBAd);
            CodeBlock &&cb = setUnknownAttributeAndBlock(cdtmid);
            codeBlockTable[cb.id] = cb;
            currentFBAd = cb.codeTable.addrdq.back();
            nextFBAd = bfbScopeCdt->getNextCodeAddr(currentFBAd);
            continue;

        } else if (currentFBCd->assemblyFormula.flagBit.type == R) {

            CodeTable cdt;
            cdt.addCode(currentFBAd, (*currentFBCd));
            cdt[currentFBAd].setArg(1, Arg(S_Bs, "ans"));
            cdt[currentFBAd].setArg(2, Arg(M_Bs, "="));
            cdt[currentFBAd].setArg(3, cdt[currentFBAd][0]);
            cdt[currentFBAd].assemblyFormula.flagBit.type = M;
            cdt[currentFBAd].assemblyFormula.flagBit.tfAddr = Addr(
            S_S_FUZHI);
            cdt[currentFBAd].assemblyFormula.flagBit.functionHandleFlag = true;
            numID += 2;
            //这里虽然将cdt的type设置为M，但是codeBlock的类型依然需要被设置为R，用以确定从何处逆向运行
            CodeBlock cb(R, false, numID, currentScopeID);
            cb.codeTable = cdt;
            codeBlockTable[numID] = cb;
            return_id.insert(numID);

            continue;
        }

    }
#if debug
    {
        static int ignorecount = 0;
        cout << "enclosing_method(enclosing_method_arguments)" << " "
                << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif

}
/**todo::（预执行主要用于处理全局变量（域外变量），
 * 但是，我们这里暂且禁用全局变量、同时要求各作用域的变量不得重复，
 * 就可以跳过2次执行）
 * 我们暂且忽略这一方法，先跑起来再说 嘻嘻嘻~~~
 * @brief 这一步用于创建适合的AR
 */
void BackStepper::backwardFunctionSecondForwardExecution() {

    return;

}
void BackStepper::backwardFunctionThirdForwardExecution() {
    return;
}
/**
 * @brief 这一步完成每一个block内部的代码匹配，同时，重整codeBlockTable中Block的排列顺序。
 * @attention 这一步不对每一个block中的地址进行处理！处理还需一步！
 */
void BackStepper::backwardFunctionFirstBackwardExecution() {
    CodeBlockTable blockTableDependent;   //仅仅包含已知变量的block，注意此block内biddq无效！
    CodeBlockTable blockTableIndependent; //仅仅包含未知变量的block，注意此block内biddq无效！
    deque<CodeBlock> blockIntegrate;

#if debug
    {
        static int ignorecount = 0;
        cout << "BackStepper::backwardFunctionFirstBackwardExecution()" << " "
                << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
        cout << codeBlockTable.toStrg() << endl;
    }
#endif
    for (auto it : codeBlockTable.iddq) {
        if (codeBlockTable[it].dependent) {
            blockTableDependent.addBlock(it, codeBlockTable[it]);
        } else {
            blockTableIndependent.addBlock(it, codeBlockTable[it]);
        }
    }

//删除return后面的Block(emm...好像原函数匹配过程中已经删过了)
//    Intg returnBlockID = (*return_id.rbegin());
//    CodeBlock &returnBlock = codeBlockTable[returnBlockID];

    /**
     * @brief 当仍然存在包含未知值的模块还未确定时，重复执行此循环，至所有未知模块均匹配。
     * 匹配后的模块储存在@arg blockIntegrate 中
     */
    while (blockTableDependent.icbmap.size() > 0) {
        //获得所有可能可以融合的组合（基于最大逆推步数决定）我这个方法有点太暴力了，影响速度
        //这一步可能有更优秀的算法,大佬来补(我想设计一个回收的东西，用来排除掉那些已经比较过的，优化时候再搞)
        step_map_dq.clear();
#if debug
        {
            static int ignorecount = 0;
            cout << "BackStepper::backwardFunctionFirstBackwardExecution()"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif

        //对于dq ，out_of_range lookups are not defined.
        step_map_dq.push_back(T_num_idset_map());
#if debug
        {
            static int ignorecount = 0;
            cout << "BackStepper::backwardFunctionFirstBackwardExecution()"
                    << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        //向step0中放入距离返回表达式最近的需要逆推的表达式
        T_num_idset_map &step0_map = step_map_dq[0];
        auto step0_map_idset = set<Intg>();
        step0_map_idset.insert(blockTableDependent.iddq.back());
        auto step0_map_num = getUnknownArgNum(step0_map_idset);
        step0_map.insert(make_pair(step0_map_num, step0_map_idset));

        //从step0依次递推step1，step2.。。。。
        for (Intg currentStep = 0; currentStep < maxBackStep; currentStep++) {
            step_map_dq.push_back(T_num_idset_map());
            auto &nextStepMap = step_map_dq[currentStep + 1];
            for (auto it : step_map_dq[currentStep]) {
                set<Intg> &idset = it.second;    //这个idset是已经代入（联立）的代码块的id集合
                set<Arg> &&unKnownArgSet = getUnknownArgSet(idset);
                for (auto unknownarg : unKnownArgSet) {

                    for (Intg substituteBlockID : unknown_id_map[unknownarg]) {
                        if (idset.count(substituteBlockID) > 0) { //重复的模块不代入
                            continue;
                        } else {    //代入不重复的模块。
                            set<Intg> nextidset_temp = idset;
                            nextidset_temp.insert(substituteBlockID);
                            Intg nextnum_temp = getUnknownArgNum(
                                    nextidset_temp);
                            //不重复添加模块
                            bool exist = false;
                            for (auto it = nextStepMap.lower_bound(
                                    nextnum_temp);
                                    it != nextStepMap.upper_bound(nextnum_temp)
                                            && it != nextStepMap.end(); ++it) {
                                if (nextidset_temp == it->second) {
                                    exist = true;
                                    break;
                                }

                            }
                            if (!exist) {
                                nextStepMap.insert(
                                        make_pair(nextnum_temp,
                                                nextidset_temp));
                            }

                        }

                    }
                }

            }
        }

        //代入并整合
        //处理原则：未知量个数越小越优先>step越小越优先
        //unknownnum:用以显示这个表达式还依赖多少外部未知变量,越少越好
        for (Intg unknownnum = 0, cantIntegrateAny = false, integrateFinish =
                false; cantIntegrateAny == false && integrateFinish == false;
                unknownnum++) {
            cantIntegrateAny = true;
            for (auto num_idset_map : step_map_dq) {
                if (unknownnum >= 0) {
                    if (num_idset_map.count(unknownnum)) {
                        cantIntegrateAny = false;
                        break;
                    }
                }

            }
            if (cantIntegrateAny) {
#if debug
                {
                    static int ignorecount = 0;
                    cout
                            << "BackStepper::backwardFunctionFirstBackwardExecution()"
                            << " cant integrate any,exit(-1)"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "vm_integrate.hpp:" << __LINE__ << ":0" << ")"
                            << endl;
                }
#endif
                exit(-1);
            }

            for (auto num_idset_map : step_map_dq) {

                for (auto it = num_idset_map.lower_bound(unknownnum);
                        it != num_idset_map.upper_bound(unknownnum); it++) {

                    set<Intg> &integrateIDset = it->second;
                    //这里对integrateIDset进行匹配与整合
                    CodeBlock &&resultBlock = integrateBlock(integrateIDset);
                    if (resultBlock.id == 0) {
                        //整合失败
                        continue;
                    } else {
                        //整合成功
                        //整合成功意味着所有参与整合的式子的未知值都将已知，
                        //因此在下一步整合前需要
                        //1 将所有已经已知的值从argUnknown中去掉，并更新依赖这些值的CodeBlock
                        //2 将参与匹配的Block从blockDependentIDdq（依赖值的iddq）中移除
                        //2? 为什么可以移除？难道不担心其他式子引用吗？不用担心。如果有式子引用以被删除的模块，说明
                        //这个式子脱离了逆推的语法树，那么这个式子也可以一并删除（事实是我们的匹配机制过于冗余，脱离逆推语法树
                        //也可以正常匹配）

                        set<Arg> argRecentlyKnown;

                        for (auto it : integrateIDset) {
                            set<Arg> &argUnknown_ =
                                    blockTableDependent[it].argUnknown;
                            argRecentlyKnown.insert(argUnknown_.begin(),
                                    argUnknown_.end());

                            blockTableDependent.erase(it);

                        }
                        argKnown += argRecentlyKnown;
                        argUnknown -= argKnown;
                        for (auto arg_ : argRecentlyKnown) {
                            for (auto id_ : unknown_id_map[arg_]) {
                                if (integrateIDset.count(id_)) {
                                    continue;
                                }
                                auto cb = blockTableDependent[id_];
                                cb.argUnknown -= argKnown;
                                cb.codeTable.resetChangeable(false, argKnown);

                            }

                        }
                        blockIntegrate.push_back(resultBlock);
                        integrateFinish = true;

                    }

                }

            }

        }
    }

//在所有模块均已经匹配后，开始进行代码的拼接，具体为
    /* 1 从blockIndependentTable中顺序读取一个block，其第一个block必须为QS类型，将其codeTable压入CodeTableTemp中
     * 2 在压入QS类型后，初始化arg_new中的变量，以C类型创建
     * 3 继续压入blockIndependentTable中的变量至倒数第二个
     * 4 将blockIntegrate中的所有block的codeTable全部依次压入
     * 5 将blockIndependentTable中最后一个block压入
     * 6 重新分配地址
     * 7 校正作用域
     * 8 @attention 每压入一个M或R类型的block，一定要在后面补一个ME，将其隔断
     */
    auto blockTableIndependent_it = blockTableIndependent.iddq.begin();

    CodeTable cdt_ME = CodeTable();
    cdt_ME.addrdq.push_back(addrnull); //注意addrnull不能出现在实际的CodeTable中，这里在经过代码替换后会被消掉
    Code cd_ME = Code();

    cd_ME.assemblyFormula.flagBit.type = ME;
    cdt_ME.codedq.push_back(cd_ME);
    (*bfbScopeCdt) = CodeTable();
//1
    bfbScopeCdt->appendCodeTable(
            blockTableIndependent[*(blockTableIndependent_it)].codeTable);
    ++blockTableIndependent_it;
    bfbScopeCdt->appendCodeTable(cdt_ME);

//2 初始化变量(这些变量是冗余的，尤其是地址变量，在进行地址变换后其值会变，因此在这里先插入一些ME占位)
    for (int menum = 10; menum > 0; menum--) {
        bfbScopeCdt->appendCodeTable(cdt_ME);

    }

//3 继续压入blockIndependentTable中的block至倒数第二个(即最后一个QE不压入)
//注意！不要在其中穿插cdt_ME,会破坏临时变量的创建与调用顺序
    Intg maxTemporaryArgOccID = 0;    //用以记录下一个TemporaryArg出现的ID的最大值
    for (;
            blockTableIndependent_it
                    != blockTableIndependent.iddq.begin()
                            + blockTableIndependent.iddq.size() - 1;) {
        bfbScopeCdt->appendCodeTable(
                blockTableIndependent[*(blockTableIndependent_it)].codeTable);
        if (blockTableIndependent[*(blockTableIndependent_it)].argTemporary.size()
                > 0) {

            //将共享临时变量的block之间不能用CdtME断开
            for (auto temporaryarg : blockTableIndependent[*(blockTableIndependent_it)].argTemporary) {
                for (auto id : temporaryArg_id_map[temporaryarg]) {
                    if (blockTableIndependent.icbmap.count(id)) {
                    }
                    maxTemporaryArgOccID = max(maxTemporaryArgOccID, id);
                }

            }
            //不共享临时变量的block之间需要用CdtME断开以确保非临时变量的创建与调用顺序
            if (maxTemporaryArgOccID <= *(blockTableIndependent_it)) {
                bfbScopeCdt->appendCodeTable(cdt_ME);
            }

        }
        ++blockTableIndependent_it;
    }
//4 将blockIntegrate中的所有block的codeTable全部依次压入
//注意！整合后的block不要在函数间填充
    for (auto cb : blockIntegrate) {
        bfbScopeCdt->appendCodeTable(cb.codeTable);

    }

//5 压入最后的block
    bfbScopeCdt->appendCodeTable(
            blockTableIndependent[*(blockTableIndependent_it)].codeTable);

//接下来我们开始整合。。。。。

}
//4 将代码整合到程序中
/**
 * 代码段实际地址取决于 split函数
 * @param appendAddr 修改的代码段将插入appendAddr之后
 * @param addrmin 修改后的代码段的起始地址
 * @param addrmax 修改后代码段终止地址
 */
void BackStepper::backwardFunctionIntegrate(const Addr &appendAddr,
        const Addr &addrmin, const Addr &addrmax) {

//这一步要完成bfb的完全整合到codeTable以及更新codeTable的scopeTable。
// 1 重置地址（不重置Z_Dz）
// 2 修改newarg（因为重置地址改变了部分S_Dz）
// 3 将重置的地址整合到bfb中
// 4 将bfb从头到尾扫描，修改scopeTable和templateFunctionTable
// 5 将bfb插入到codeTable中
// 最后请在创建BackSteper的地方修改currentCodeAddr和nextCodeAddr（如果需要的话）的地址

//1
    map<Addr, Addr> map_original_new = bfbScopeCdt->resetAddrRange(addrmin,
            addrmax);
//2
    set<Arg> newarg_temp = newarg;
#if debug
    {
        static int ignorecount = 0;
        cout
                << "BackStepper::backwardFunctionIntegrate(const Addr&, const Addr&, const Addr&)"
                << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
    for (auto &arg_ : newarg) {
        if (arg_.argFlag == S_Dz && map_original_new.count(arg_.arg_a)) {
            newarg_temp.erase(arg_);
            newarg_temp.insert(Arg(S_Dz, map_original_new[arg_.arg_a]));

        }
    }
#if debug
    {
        static int ignorecount = 0;
        cout
                << "BackStepper::backwardFunctionIntegrate(const Addr&, const Addr&, const Addr&)"
                << " " << "\tignorecount:[" << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
//3 将重置的地址整合到bfb中
//3.1 创建重置地址的cdt
    CodeTable cdt_newarg;
    Code cd_newarg;
    cd_newarg.assemblyFormula.flagBit.type = C;
    cd_newarg.setArg(1, Arg(X_Bs, "new"));
    cd_newarg.setArg(2, Arg(M_Bs, ":"));
    Addr ad = Addr(1);
    for (auto arg__ = newarg.begin(); arg__ != newarg.end(); ++arg__) {
#if debug
        {
            static int ignorecount = 0;
            cout
                    << "BackStepper::backwardFunctionIntegrate(const Addr&, const Addr&, const Addr&)"
                    << " arg__:[" << (*arg__).toStrg() << "]"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        cd_newarg.setArg(0, *arg__);
        cd_newarg.setArg(3, *arg__);
        cdt_newarg.addrdq.push_back(++ad);
        cdt_newarg.codedq.push_back(cd_newarg);
    }
//3.2 获取所要插入的位置，亦即之前在作用域内部开头处所插入的连续ME。现将之删除，并获取其上下地址边界
    Addr cdtNewArgLowBoundAddr = bfbScopeCdt->addrdq[0];
    Addr cdtNewArgUpBoundAddr = bfbScopeCdt->getNextNot(ME,
            cdtNewArgLowBoundAddr);
    bfbScopeCdt->eraseCode(cdtNewArgLowBoundAddr, +1, cdtNewArgUpBoundAddr, -1);
    cdt_newarg.resetAddrRange(cdtNewArgLowBoundAddr, cdtNewArgUpBoundAddr);
//3 将重置的地址整合到bfb中
    bfbScopeCdt->insertCodeTable(cdtNewArgUpBoundAddr, cdt_newarg);
//4 将bfb从头到尾扫描，修改scopeTable和templateFunctionTable(同时修改Z_Dz)
    map<Addr, Addr> scope_org_new_map;
    deque<Addr> currentScopeStack_;
    Addr currentScopeAddr_;
    for (Intg codePos = 0, len = bfbScopeCdt->size(); codePos < len;
            codePos++) {
        Code &code_ = (*bfbScopeCdt)[codePos];
        if (code_.assemblyFormula.flagBit.type == QS) {
            //获取currentScopeAddr(这里直接使用codeTable中代码地址作为scope地址)
            currentScopeAddr_ = bfbScopeCdt->addrdq[codePos];

            //获取原Z_Dz
            Addr scopeAddrOrg = code_.assemblyFormula.quaternion.result_a;

            //修改Code的地址
            code_.assemblyFormula.quaternion.result_a = currentScopeAddr_;
            code_.scopeAddr = currentScopeAddr_;
            code_.assemblyFormula.flagBit.res = currentScopeAddr_;

            //修改scopeTable
            scopeTable->addrdq.push_back(currentScopeAddr_);
            scopeTable->scopedq.push_back((*scopeTable)[scopeAddrOrg]);
            (*scopeTable)[currentScopeAddr_].scopeCoor1 =
                    (*bfbScopeCdt).addrdq[codePos];

            if (currentScopeStack_.size() == 0) {
                (*scopeTable)[currentScopeAddr_].requireScopeAddr =
                        bfnScopeAddr;
            } else {
                (*scopeTable)[currentScopeAddr_].requireScopeAddr =
                        currentScopeStack_.back();
                (*scopeTable)[currentScopeAddr_].parentScopeAddr =
                        currentScopeStack_.back();
            }
            (*scopeTable)[currentScopeAddr_].scopeCoor1 =
                    (*bfbScopeCdt).addrdq[codePos];

            currentScopeStack_.push_back(currentScopeAddr_);

            scope_org_new_map[scopeAddrOrg] = currentScopeAddr_;
        } else if (code_.assemblyFormula.flagBit.type == QE) {

            code_.scopeAddr = currentScopeAddr_;

            //修改Code的地址
            code_.assemblyFormula.quaternion.result_a = currentScopeAddr_;
            code_.scopeAddr = currentScopeAddr_;
            code_.assemblyFormula.flagBit.res = currentScopeAddr_;

            //修改scope
            (*scopeTable)[currentScopeAddr_].scopeCoor2 =
                    (*bfbScopeCdt).addrdq[codePos];

            currentScopeStack_.pop_back();
            if (currentScopeStack_.size() > 0) {
                currentScopeAddr_ = currentScopeStack_.back();
            } else {
                currentScopeAddr_ = currentAR->scopeStructureAddr;
            }

        } else {
            code_.scopeAddr = currentScopeAddr_;
            Addr &a1s_ = code_.assemblyFormula.flagBit.a1s;
            Addr &a2s_ = code_.assemblyFormula.flagBit.a2s;
            Addr &ops_ = code_.assemblyFormula.flagBit.ops;
            Addr &res_ = code_.assemblyFormula.flagBit.res;
            if (scope_org_new_map.count(a1s_)) {
                a1s_ = scope_org_new_map[a1s_];
            }
            if (scope_org_new_map.count(a2s_)) {
                a2s_ = scope_org_new_map[a2s_];
            }
            if (scope_org_new_map.count(ops_)) {
                ops_ = scope_org_new_map[ops_];
            }
            if (scope_org_new_map.count(res_)) {
                res_ = scope_org_new_map[res_];
            }
        }
    }
//修改templateFunctionTable
    TemplateFunction &bfntf = (*templateFunctionTable)[bfnScopeAddr];
    bfntf.isBackwardFunction = true;
    bfntf.bodyStartAddr = bfbScopeCdt->addrdq.front();
    bfntf.bodyEndAddr = bfbScopeCdt->addrdq.back();
    bfntf.bodyScopeAddr = (*bfbScopeCdt)[0].scopeAddr;
    bfntf.returnHandle.clear();

// 5 将bfb插入到codeTable中
    codeTable->insertCodeTable(addrmax, *bfbScopeCdt);

//如此的狠角色跟我比还是不够看

}
const bool Integrator::integrate() {
    globalRefTable.clear();
    globalRefTable.mode = MODE_PREEXE;
    while (getNewCode() == true) {
#if debug
        cout << "|||||||||||||||||||||" << endl;
#endif
#if debug
        {
            static int ignorecount = 0;
            cout << "Integrator::integrate()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t](" << "vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        Intg pos = 0;
        for (Code &cd : codeTable->codedq) {
#if debug
            cout << "Addr: " << codeTable->addrdq[pos].toStrg() << "   \t"
                    << "Code: " << cd.toStrg() << endl;
#endif
            pos++;

        }
#if debug
        {
            static int ignorecount = 0;
            cout << "Integrator::integrate()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
        if ((*currentCode).assemblyFormula.flagBit.type == C) {
            setArgScopeAddr();
            addArgsToCurrentAR();

        } else if ((*currentCode).assemblyFormula.flagBit.type == M
                && (*currentCode).assemblyFormula.flagBit.execute != F_) {
            setArgScopeAddr();
            addArgsToCurrentAR();

            if (nextCode == NULL
                    || (*nextCode).assemblyFormula.flagBit.type != M) {
                //如果表达式位于返回表达式的函数体中，则不进行整合
                Scope &currentScope = (*scopeTable)[currentCode->scopeAddr];
                if ((currentScope.functionFlag == FB
                        || currentScope.functionFlag == RFB)) {
                    TemplateFunction &tf =
                            (*templateFunctionTable)[currentScope.requireScopeAddr];
                    if (tf.integrate == true) {

                        continue;
                    }

                }
                //如果不位于返回表达式的函数体中，则进行整合
//                CodeTable cdtr = codeTable->copyTree(currentCodeAddr);
                MultiTFIntegrator mtfi(codeTable, currentCodeAddr,
                        templateFunctionTable, scopeTable, systemTable,
                        currentAR);
                mtfi.setMaxCapacity(matchStateSiloMaxCapacity,
                        codeTableSiloMaxCapacity, maxSwitchTurn);
#if debug
                cout << "Integrator::integrate 2s" << endl;
#endif
                if (mtfi.integrate() == false) {
#if debug
                    {
                        static int ignorecount = 0;
                        cout << "Integrator::integrate()"
                                << " integrate failed,exit(-1)"
                                << "\tignorecount:[" << ignorecount++ << "\t]("
                                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_integrate.hpp:"
                                << __LINE__ << ":0" << ")" << endl;
                    }
#endif
                    exit(-1);
                };
#if debug
                cout << "Integrator::integrate 2e" << endl;
#endif
            }

        } else if ((*currentCode).assemblyFormula.flagBit.type == Y
                && (*currentCode).assemblyFormula.flagBit.execute != F_) {
            setArgScopeAddr();
            addArgsToCurrentAR();
        }

        else if ((*currentCode).assemblyFormula.flagBit.type == M

        && (*currentCode).assemblyFormula.flagBit.execute == F_) {
            addArgsToCurrentAR();
            setArgScopeAddr();

        } else if ((*currentCode).assemblyFormula.flagBit.type == Y
                && (*currentCode).assemblyFormula.flagBit.execute == F_) {
            addArgsToCurrentAR();
            setArgScopeAddr();

        } else if ((*currentCode).assemblyFormula.flagBit.type == QS
                && (*currentCode).assemblyFormula.flagBit.execute == F_) {
            setArgScopeAddr();
            addArgsToCurrentAR();

        } else if ((*currentCode).assemblyFormula.flagBit.type == QE
                && (*currentCode).assemblyFormula.flagBit.execute == F_) {
            setArgScopeAddr();
            addArgsToCurrentAR();

        } else if ((*currentCode).assemblyFormula.flagBit.type == QS
                && (*currentCode).assemblyFormula.flagBit.execute == R) {

            setArgScopeAddr();
            addArgsToCurrentAR();

        } else if ((*currentCode).assemblyFormula.flagBit.type == QE
                && (*currentCode).assemblyFormula.flagBit.execute == R) {
            setArgScopeAddr();
            addArgsToCurrentAR();

        } else if ((*currentCode).assemblyFormula.flagBit.type == F) {
            setArgScopeAddr();
            addArgsToCurrentAR();
        } else if ((*currentCode).assemblyFormula.flagBit.type == TD) {
            setArgScopeAddr();
            //在这里进行逆函数的推导
            addArgsToCurrentAR();
        } else if ((*currentCode).assemblyFormula.flagBit.type == R) {
            setArgScopeAddr();
            addArgsToCurrentAR();
        }

        else {
            setArgScopeAddr();
            addArgsToCurrentAR();
        }

    }
    return true;

}

}  // namespace VM

#endif /* VM_INTEGRATE_HPP_ */
