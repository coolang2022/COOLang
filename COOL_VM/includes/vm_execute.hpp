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

#ifndef VM_EXECUTE_HPP_
#define VM_EXECUTE_HPP_
#include "vm_integrate.hpp"
#include <algorithm>
#include <math.h>
using namespace std;
namespace VM {
typedef Addr AddrFormal;
typedef Addr AddrReal;
class FunctionHandleMatcher {
public:
    CodeTable *cdtreal = NULL;

    CodeTable *cdtformal = NULL;

    map<ArgReal, deque<ArgFormal> > am;
    map<ArgFormal, ArgReal> frmap;
    map<AddrReal, AddrFormal> cdm;
    bool sameOneArg(const ArgReal &argreal, const ArgFormal &argformal) {
#if debug
        cout << "FunctionHandleMatcher::sameOneArg argreal:["
                << argreal.toStrg() << "]\targformal:[" << argformal.toStrg()
                << "]\t" << endl;
#endif
        if (argformal.formalArg == argreal.formalArg
                && argreal.asc == argformal.asc && argreal == argformal) {
            return true;
        }
        return false;
    }
    FunctionHandleMatcher(CodeTable *const cdtreal,
            CodeTable *const cdtformal) {
        this->cdtreal = cdtreal;

        this->cdtformal = cdtformal;

    }
    const bool add(const ArgReal &argreal, const ArgFormal &argformal) {
#if debug
        cout << "FunctionHandleMatcher::add \n\targreal:[" << argreal.toStrg()
                << "]\n\targformal:[" << argformal.toStrg() << "]" << endl;
#endif
        if (argformal.changeable != A_
                && argreal.changeable != argformal.changeable) {
#if debug
            cout << "FunctionHandleMatcher::add false1\n\targreal:["
                    << argreal.toStrg() << "]\n\targformal:["
                    << argformal.toStrg() << "]" << endl;
#endif
            return false;
        }
        if (argreal == argnull && argformal == argnull) {
            return true;
        }

        if (am[argreal].size() == 0) {
            am[argreal].push_back(Arg());
        }

        if (argreal.argFlag == Sz) {
            if (argreal.changeable == true) {
#if debug
                cerr
                        << "FunctionHandleMatcher::add err, Sz & changeable==true ,exit -1"
                        << endl;
#endif
                exit(-1);
            }
            if (argformal.changeable == true) {
#if debug
                cout << "FunctionHandleMatcher::add false2\n\targreal:["
                        << argreal.toStrg() << "]\n\targformal:["
                        << argformal.toStrg() << "]" << endl;
#endif
                return false;
            }
            if (argformal.argFlag == Sz) {
                return argreal == argformal;
            } else if (argformal.argFlag == S_Dz || argformal.argFlag == S_Bs) {
                for (const ArgFormal &a : am[argreal]) {
                    if (a == argformal) {
                        return true;
                    }
                }
                if (am[argreal][0] == argnull) {
                    am[argreal][0] = argformal;
                } else {
                    am[argreal].push_back(argformal);
                }
                return true;
            }

        }

        if (argreal.argFlag == X_Bs) {
#if debug
            cout
                    << "FunctionHandleMatcher::add compare SHUXING,jump and return true. argreal:["
                    << argreal.toStrg() << "]\targformal:[" << argreal.toStrg()
                    << "]" << endl;
#endif
            return true;
        }

        if (argreal.argFlag == M_Bs) {
            return argreal == argformal;
        }
        if (am[argreal][0] == argformal) {
            return true;
        } else if (argformal.formalArg == true && am[argreal][0] == argnull) {
            am[argreal][0] = argformal;
            return true;
        } else if (argformal.formalArg == false
                && sameOneArg(argreal, argformal) == true) {
            am[argreal][0] = argformal;
            return true;
        } else {
#if debug
            cout << "FunctionHandleMatcher::add false3\n\targreal:["
                    << argreal.toStrg() << "]\n\targformal:["
                    << argformal.toStrg() << "]" << endl;
#endif
            return false;
        }
    }
    const bool add(const AddrReal &adreal, const AddrFormal &adformal) {
#if debug
        cout << "FunctionHandleMatcher::add(addr,addr) adreal:["
                << adreal.toStrg() << "]\tadformal:[" << adformal.toStrg()
                << "]\t" << endl;
#endif
        if (cdm[adreal] == adformal) {
            return true;
        } else if (cdm[adreal] == addrnull) {
            cdm[adreal] = adformal;
            return true;
        } else {
            return false;
        }
    }
    const Addr getSameTrBranchLastArgAddr(CodeTable *codeTable, const Addr &ad,
            const Arg &a) {
#if debug
        cout << "FunctionHandleMatcher::getSameTrBranchLastArgAddr ad:["
                << ad.toStrg() << "]\ta:[" << a.toStrg() << "]\t" << endl;
#endif
        if (a.argFlag == M_Bs) {
            return addrnull;
        }
        Addr addr = (*codeTable).getAboveCodeAddr(ad);
        while ((*codeTable)[addr].assemblyFormula.flagBit.type == M
                || (*codeTable)[addr].assemblyFormula.flagBit.execute == Du) {
            if ((*codeTable)[addr].includeArg(
                    a) && (*codeTable)[addr].assemblyFormula.flagBit.execute != Du) {
                return addr;
            }
            addr = (*codeTable).getAboveCodeAddr(addr);
        }
        return Addr();

    }
    const bool reset(CodeTable *cdreal, CodeTable *cdformal) {
        this->cdtreal = cdtreal;
        this->cdtformal = cdtformal;
        am.clear();
        cdm.clear();
        return true;
    }
    const bool match(const AddrReal &adreal, const AddrFormal &adformal) {
#if debug
        cout << "FunctionHandleMatcher::match adreal:[" << adreal.toStrg()
                << "]\tadformal:[" << adformal.toStrg() << "]\t" << endl;
#endif
        if (adformal == addrnull) {
            getFRMap();
            return true;
        }
        if (cdm.find(adreal) != cdm.end()) {
            if (cdm[adreal] == adformal) {
                getFRMap();
                return true;
            } else {
#if debug
                cerr << "FunctionHandleMatcher::match err,adreal:["
                        << adreal.toStrg() << "]\tadformal:["
                        << adformal.toStrg() << "]" << endl;
#endif
            }

        } else if (add(adreal, adformal) == false) {
#if debug
            cout << "FunctionHandleMatcher::match err,adreal:["
                    << adreal.toStrg() << "]\tadformal:[" << adformal.toStrg()
                    << "]" << endl;
#endif
            return false;
        }
        ArgReal argreal1 = (*cdtreal)[adreal][0];
        ArgReal argreal2 = (*cdtreal)[adreal][1];
        ArgReal operatorreal = (*cdtreal)[adreal][2];
        ArgReal resultreal = (*cdtreal)[adreal][3];

        if (adformal < addrnull) {
#if debug
            cout << "FunctionHandleMatcher::match adformal>addrnull,adreal:["
                    << adreal.toStrg() << "]\tadformal:[" << adformal.toStrg()
                    << "]" << endl;
#endif
            switch (adformal[0]) {
            case S_S_JIA:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "+");
                break;
            case S_S_JIAN:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "-");
                break;
            case S_S_CHENG:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "*");
                break;
            case S_S_CHU:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "/");
                break;
            case S_S_FUZHI:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "=");
                break;
            case S_S_DENGYU:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "==");
                break;
            case S_S_DAYU:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == ">");
                break;
            case S_S_XIAOYU:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "<");
                break;
            case S_S_MI:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "^");
                break;
            case CALL:
                return ((argreal1.argFlag == S_Bs || argreal1.argFlag == Zf)
                        && (argreal2.argFlag == S_Dz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == Sz
                                || argreal2.argFlag == Zf)
                        && (operatorreal.arg_s == "call"
                                || operatorreal.arg_s == "ca"));
                break;
            case SHUCHU:
                return (argreal1.argFlag == Sz || argreal1.argFlag == S_Bs
                        || argreal1.argFlag == S_Dz)
                        && (argreal2.argFlag == Sz || argreal2.argFlag == S_Bs
                                || argreal2.argFlag == S_Dz)
                        && (operatorreal.argFlag == M_Bs
                                && operatorreal.arg_s == "-->");
                break;
            default:
                break;
            }

        }
        ArgReal argformal1 = (*cdtformal)[adformal][0];
        ArgReal argformal2 = (*cdtformal)[adformal][1];
        ArgReal operatorformal = (*cdtformal)[adformal][2];
        ArgReal resultformal = (*cdtformal)[adformal][3];

        if (adreal != addrnull && adformal != addrnull) {
            bool add1 = add(argreal1, argformal1);
            if (add1 == false) {
#if debug
                cout << "add1false:adreal:[" << adreal.toStrg()
                        << "]\tadformal:[" << adformal.toStrg() << "]"
                        << "\n\targreal1:[" << argreal1.toStrg() << "]"
                        << "\n\targformal1:[" << argformal1.toStrg() << "]"

                        << endl;
#endif
            }

            bool add2 = add(argreal2, argformal2);
            if (add2 == false) {
#if debug
                cout << "add2false:adreal:[" << adreal.toStrg()
                        << "]\tadformal:[" << adformal.toStrg() << "]"
                        << "\n\targreal2:[" << argreal2.toStrg() << "]"
                        << "\n\targformal2:[" << argformal2.toStrg() << "]"
                        << endl;
#endif
            }

            bool addop = add(operatorreal, operatorformal);

            if (addop == false) {
#if debug
                cout << "addopfalse:adreal:[" << adreal.toStrg()
                        << "]\tadformal:[" << adformal.toStrg() << "]"
                        << "\n\toperatorreal:[" << operatorreal.toStrg() << "]"
                        << "\n\toperatorformal:[" << operatorformal.toStrg()
                        << "]" << endl;
#endif
            }
            bool addre = add(resultreal, resultformal);
            if (addre == false) {
#if debug
                cout << "addformalalse:adreal:[" << adreal.toStrg()
                        << "]\tadformal:[" << adformal.toStrg() << "]"

                        << "\n\tresultreal:[" << resultreal.toStrg() << "]"
                        << "\n\tresultformal:[" << resultformal.toStrg() << "]"
                        << endl;
#endif
            }
            if (add1 && add2 && addop && addre) {
                Addr adreal1;
                Addr adformal1;
                adreal1 = cdtreal->getAboveCodeAddr(adreal);
                adformal1 = cdtformal->getAboveCodeAddr(adformal);
                if ((*cdtreal)[adreal1].assemblyFormula.flagBit.type != M
                        || (*cdtreal)[adreal1].assemblyFormula.flagBit.functionHandleFlag
                                == T_) {
                    getFRMap();
                    return true;
                } else if ((*cdtreal)[adreal1].assemblyFormula.flagBit.type == M
                        && (*cdtreal)[adreal1].assemblyFormula.flagBit.functionHandleFlag
                                == F_) {

                    return match(adreal1, adformal1);
                } else {
#if debug
                    cerr << "FunctionHandleMatcher::match err,exit(-1)" << endl;
#endif
                    exit(-1);
                }

            }
            return false;
        } else if (adformal == addrnull) {
            getFRMap();
            return true;
        } else {
            return false;
        }
    }
    map<ArgFormal, ArgReal>& getFRMap() {
#if debug
        cout << "FunctionHandleMatcher::getFRMap in" << endl;
#endif

        for (auto iter = am.begin(); iter != am.end(); iter++) {
            for (ArgFormal &af : iter->second) {
                if (iter->first.formalArg == false) {
                    Arg argreal(iter->first);
                    argreal.arg_ar = iter->first.arg_ar;
                    argreal.formalArg = true;
                    this->frmap.insert(pair<ArgFormal, ArgReal>(af, argreal));
                } else {
                    this->frmap.insert(
                            pair<ArgFormal, ArgReal>(af, iter->first));
                }
            }
        }
#if debug
        cout << "FunctionHandleMatcher::getFRMap out" << endl;
#endif

        return frmap;
    }

};

class Executer {
public:
    ScopeTable *scopeTable = NULL;
    TemplateFunctionTable *templateFunctionTable = NULL;
    DataTable *currentAR = NULL;
    CodeTable *codeTable = NULL;
    SystemTable *systemTable = NULL;

    Intg currentCodePos = -1;
    Addr currentCodeAddr;
    Code *currentCode = NULL;

    Intg nextCodePos = 0;
    Addr nextCodeAddr;
    Code *nextCode = NULL;

    Arg argexe1;
    Arg argexe2;
    Arg operatorexe;
    Arg resultexe;
    Addr getScopeQEAddr(const Addr &codeAddr);
    Addr getScopeQSAddr(const Addr &codeAddr);
    Addr getNextAssociateBranchAddr(const Addr &currentBranchCodeAddr);
    const bool getNewCode();
    const bool setNextCodePos(Intg &pos);
    const bool execute();
    const bool executeCurrentCode();
    const bool addArgsToCurrentAR();
    map<ArgFormal, ArgReal> getFRMap();
    Executer(CodeTable *codeTable, TemplateFunctionTable *templateFunctionTable,
            ScopeTable *scopeTable, DataTable *dataTable,
            SystemTable *systemTable) {

        this->codeTable = codeTable;
        this->templateFunctionTable = templateFunctionTable;
        this->scopeTable = scopeTable;
        this->currentAR = dataTable;
        this->systemTable = systemTable;

    }
    DataTable* getParentAR(const Addr &scopeAddr);

};
/**
 * @brief 在创建函数调用ar时，利用此函数获得函数的parentAR
 * @attention 此函数考虑了类函数的调用
 * @param functionBodyScopeAddr
 * @return
 */
DataTable* Executer::getParentAR(const Addr &functionBodyScopeAddr) {

#if debug
    cout << "Executer::getParentAR in, functionBodyScopeAddr:["
            << functionBodyScopeAddr.toStrg() << "] in" << endl;
#endif

    if (functionBodyScopeAddr <= addrnull) {
        //内置函数不考虑
        return NULL;
    }
    Addr &parentScopeAddr = (*scopeTable)[functionBodyScopeAddr].parentScopeAddr;
    Scope &parentScope = (*scopeTable)[parentScopeAddr];
    DataTable *parentAR = NULL;
    if (parentScope.functionFlag != SYSB) {
        //非成员函数，直接递推寻找父ar
        parentAR = this->currentAR;
        while (parentAR && parentAR->scopeStructureAddr != parentScopeAddr) {
            parentAR = parentAR->parentAR;
        }
        return parentAR;
    } else {
        //是成员函数，有可能是成员函数间内部互相调用，此时可以访问到的与父scope同addr的ar就是父ar
        parentAR = currentAR->getAccessibleAR(parentScopeAddr);
        if (parentAR) {
            return parentAR;
        } else {
            //是成员函数，且不是类内部互相调用的情况，需要在当前ar中寻找参与函数调用的类实例
            //从根节点向前推算，第一个遇到的引用将被作为函数调用的父ar
            Intg pos = getAddrPosition(codeTable->addrdq, currentCodeAddr);
            Intg pos_start = pos;
            Addr &tfaddr = currentCode->assemblyFormula.flagBit.tfAddr;
            while (true) {
                Code &cd = codeTable->codedq[pos];
                if (cd.assemblyFormula.flagBit.tfAddr != tfaddr
                        || (cd.assemblyFormula.flagBit.functionHandleFlag
                                == true && pos != pos_start)) {
                    break;
                }
                Arg &&arg1 = cd[0];
                Arg &&arg2 = cd[1];
                Arg &&argop = cd[2];
                Arg &&argres = cd[3];
                if (arg1.argFlag == Y_Dz) {
                    arg1.arg_ar = currentAR;
                    ReferenceData &&ref = globalRefTable.getReferenceData(arg1);
                    parentAR = ref.getDestAR();
                    return parentAR;
                }
                if (arg2.argFlag == Y_Dz) {
                    arg2.arg_ar = currentAR;
                    ReferenceData &&ref = globalRefTable.getReferenceData(arg2);
                    parentAR = ref.getDestAR();
                    return parentAR;
                }
                if (argop.argFlag == Y_Dz) {
                    argop.arg_ar = currentAR;
                    ReferenceData &&ref = globalRefTable.getReferenceData(
                            argop);
                    parentAR = ref.getDestAR();
                    return parentAR;
                }
                if (argres.argFlag == Y_Dz) {
                    argres.arg_ar = currentAR;
                    ReferenceData &&ref = globalRefTable.getReferenceData(
                            argres);
                    parentAR = ref.getDestAR();
                    return parentAR;
                }
                pos--;

            }
            return NULL;

        }
    }

#if debug
    cout << "Executer::getParentAR in, functionBodyScopeAddr:["
            << functionBodyScopeAddr.toStrg() << "] out" << endl;
#endif

}

/**
 *@brief 根据当前的@arg  currentCodeAddr 和 @arg 对应的tfaddr生成形参实参对照表
 * @return 形式参数、实际参数对照表
 */
map<ArgFormal, ArgReal> Executer::getFRMap() {
    {
#if debug
        static Intg ignorecount = 0;
        cout << "Executer::getFRMap in, currentCodeAddr:["
                << currentCodeAddr.toStrg() << "] in , ignorecount:["
                << ignorecount++ << "]" << endl;
#endif
    }
    FunctionHandleMatcher fhm(codeTable, codeTable);
    const Addr &tfaddr =
            (*codeTable)[currentCodeAddr].assemblyFormula.flagBit.tfAddr;
    const Addr &addrformal = codeTable->getLast(M,
            (*scopeTable)[tfaddr].scopeCoor2);
#if debug
    cout << "Executer::getFRMap 1, currentCodeAddr:["
            << currentCodeAddr.toStrg() << "]\taddrformal:["
            << addrformal.toStrg() << "]" << endl;
#endif
    if (fhm.match(currentCodeAddr, addrformal) == T_) {
#if debug
        cout << "Executer::getFRMap out, currentCodeAddr:["
                << currentCodeAddr.toStrg() << "] in" << endl;
#endif
        return fhm.frmap;

    } else {
#if debug
        cerr << "Executer::getFRMap err,exit(-1)" << endl;
#endif
        exit(-1);
    }

}
/**
 * @attention 注意，此函数只是用于验证给定位置pos的代码是否可以成为一个有效的nextCode。
 * 如果输入的pos是有效的，那么pos的值不会改变，否则，此函数会在此pos的基础上寻找下一个有效的pos
 * @attention 有效的pos指的是对应代码可以被顺序执行！（因此不能被直接执行的代码会被跳过）
 * @param pos
 * @return
 */
const bool Executer::setNextCodePos(Intg &pos) {
#if debug
    {
        static int ignorecount = 0;
        cout << "Executer::setNextCodePos(int&)" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_execute.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
#if debug
    cout << "Executer::setNextCodePos 0, nextCodeAddr:["
            << nextCodeAddr.toStrg() << "]" << endl;
#endif
    if (nextCodePos >= (Intg) codeTable->addrdq.size() || nextCodePos < 0) {
        nextCodeAddr = addrnull;
#if debug
        cout << "Executer::setNextCodePos 0.5, nextCodeAddr:["
                << nextCodeAddr.toStrg() << "]" << endl;
#endif
        return true;
    }
    if ((*codeTable)[pos].assemblyFormula.flagBit.execute == T_) {
        nextCodeAddr = (*codeTable).addrdq[pos];
#if debug
        cout << "Executer::setNextCodePos 1, nextCodeAddr:["
                << nextCodeAddr.toStrg() << "]" << endl;
#endif

        return true;
    }
    if (pos >= (Intg) codeTable->addrdq.size()) {
        nextCodeAddr = addrnull;
#if debug
        cout << "Executer::setNextCodePos 2, nextCodeAddr:["
                << nextCodeAddr.toStrg() << "]" << endl;
#endif
        return false;
    }
    if ((*codeTable)[pos].assemblyFormula.flagBit.execute == Du) {
#if debug
        cout << "Executer::setNextCodePos 3, nextCodeAddr:["
                << nextCodeAddr.toStrg() << "]" << endl;
#endif
        pos++;
        return setNextCodePos(pos);
    } else if ((*codeTable)[pos].assemblyFormula.flagBit.execute == F_
            || ((*codeTable)[pos].assemblyFormula.flagBit.execute == R
                    && (*codeTable)[pos].assemblyFormula.flagBit.type == QS)) {
#if debug
        cout << "Executer::setNextCodePos 4, nextCodeAddr:["
                << nextCodeAddr.toStrg() << "]" << endl;
#endif
        Addr &scopeAddrJump = (*codeTable)[pos].scopeAddr;
        Addr &coor2 = (*scopeTable)[scopeAddrJump].scopeCoor2;
        pos = getAddrPosition(codeTable->addrdq, coor2);
        ++pos;
#if debug
        cout << "Executer::setNextCodePos 5, nextCodeAddr:["
                << nextCodeAddr.toStrg() << "]" << endl;
#endif
        return setNextCodePos(pos);
    }
    nextCodeAddr = (*codeTable).addrdq[pos];
#if debug
    cout << "Executer::setNextCodePos 5, nextCodeAddr:["
            << nextCodeAddr.toStrg() << "]" << endl;
#endif
    return true;
}
Addr Executer::getScopeQEAddr(const Addr &codeAddr) {
    return (*scopeTable)[(*codeTable)[codeAddr].scopeAddr].scopeCoor2;
}
Addr Executer::getScopeQSAddr(const Addr &codeAddr) {
    return (*scopeTable)[(*codeTable)[codeAddr].scopeAddr].scopeCoor1;
}
Addr Executer::getNextAssociateBranchAddr(const Addr &currentBranchCodeAddr) {
    Addr possibleNextBranchCodeAddr;
    Addr nextQSAddr = codeTable->getNext(QS, currentBranchCodeAddr);
    Addr nextQEAddr;
    Addr currentBranchCodeQEAddr = getScopeQEAddr(currentBranchCodeAddr);

    if (nextQSAddr == addrnull
            || (currentBranchCodeQEAddr != addrnull
                    && nextQSAddr > currentBranchCodeQEAddr)) {
        return Addr();
    } else {
        nextQEAddr = getScopeQEAddr(nextQSAddr);
        possibleNextBranchCodeAddr = codeTable->getNext(B, nextQEAddr);
        if (possibleNextBranchCodeAddr != addrnull
                && (*codeTable)[possibleNextBranchCodeAddr][0]
                        == (*codeTable)[currentBranchCodeAddr][3]) {
            return possibleNextBranchCodeAddr;
        } else {
            return Addr();
        }
    }

}
/**
 * @attention 这个函数将根据（用户设定的）nextCodePos更新currentCode和Addr，
 * 同时，会自动寻找下一个可以被自动执行的代码将其设为新的nextCodePos以及对应的addr与code供用户参考。
 * 用户可以对其重新赋值来确定下一步执行的代码。
 * @return
 */
const bool Executer::getNewCode() {
#if debug
    cout << "Executer::getNewCode, currentCodeAddr:["
            << currentCodeAddr.toStrg() << "] in" << endl;
#endif

    if (nextCodePos >= (Intg) codeTable->addrdq.size()) {
#if debug
        cout << "//////////////////////////////////////////////////\n"
                "///Executer::getNewCode program finish! exit(0)///\n"
                "//////////////////////////////////////////////////" << endl;
#endif
        exit(0);

    } else {
        currentCodePos = nextCodePos;
    }

    if (currentCodePos >= (Intg) codeTable->addrdq.size()) {
        return false;
    }
    ++nextCodePos;
    setNextCodePos(nextCodePos);

#if debug
    cout << "Executer::getNewCode nextCodeAddr 1:" << nextCodeAddr.toStrg()
            << endl;
#endif

    if (currentCodePos < (Intg) codeTable->addrdq.size() - 1) {
#if debug
        cout << "Executer::getNewCode nextCodeAddr 2:" << nextCodeAddr.toStrg()
                << endl;
#endif
        currentCodeAddr = codeTable->addrdq[currentCodePos];
        currentCode = &(*codeTable)[currentCodePos];
#if debug
        cout << "Executer::getNewCode nextCodeAddr 3:" << nextCodeAddr.toStrg()
                << endl;
#endif
        if (nextCodePos < (Intg) codeTable->addrdq.size()) {
#if debug
            cout << "Executer::getNewCode nextCodeAddr 4:"
                    << nextCodeAddr.toStrg() << endl;
#endif
            nextCode = &(*codeTable)[nextCodePos];
#if debug
            cout << "Executer::getNewCode nextCodeAddr 5:"
                    << nextCodeAddr.toStrg() << endl;
#endif
        }

        return true;

    } else if (currentCodePos == (Intg) codeTable->addrdq.size() - 1) {
#if debug
        cout << "Executer::getNewCode nextCodeAddr 6:" << nextCodeAddr.toStrg()
                << endl;
#endif
        currentCodeAddr = codeTable->addrdq[currentCodePos];
        currentCode = &(*codeTable)[currentCodePos];
        nextCodeAddr = addrnull;
        nextCode = NULL;
#if debug
        cout << "Executer::getNewCode nextCodeAddr 7:" << nextCodeAddr.toStrg()
                << endl;
#endif
        return true;
    } else {
#if debug
        cerr << "Executer::getNewCode err,exit(-1)" << endl;
#endif
        exit(-1);
    }
    return false;
}
const bool Executer::addArgsToCurrentAR() {
#if debug
    {
        static int ignorecount = 0;
        cout << "Executer::addArgsToCurrentAR()" << " " << "\tignorecount:["
                << ignorecount++ << "\t]("
                << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_execute.hpp:"
                << __LINE__ << ":0" << ")" << endl;
    }
#endif
#if debug
    cout << "Executer::addArgsToCurrentAR, currentCodeAddr:["
            << currentCodeAddr.toStrg() << "] in" << endl;
#endif
    if ((*currentCode).assemblyFormula.flagBit.type == C
            && argexe1.argFlag == S_Bs && argexe2.argFlag == X_Bs
            && argexe2.arg_s == "new") {

        (*currentAR).add(argexe1);

    } else if ((*currentCode).assemblyFormula.flagBit.type == C
            && argexe1.argFlag == S_Bs && argexe2.argFlag == X_Bs
            && argexe2.arg_s != "new" && argexe2.arg_s != "out") {
        //此部分处理创建类对象实例
        (*currentAR).add(argexe1);
        Data &sysData = (*currentAR)[argexe1];
        safe_delete(sysData.content_ar);
        System &sys = (*systemTable)[pair<Strg, Addr>(argexe2.arg_s,
                currentCode->scopeAddr)];
        sysData.dataFlagBit = S_AR;
        Addr &systemScopeAddr = sys.systemAddr;
        DataTable *sysAR = sysData.content_ar = new DataTable(currentAR,
                currentAR, currentAR, (*currentCode).scopeAddr, SYSB);
        sysAR->meetAddr = nextCodeAddr;
        sysAR->scopeStructureAddr = sys.systemAddr;
        //执行所继承的类
        nextCodeAddr = (*scopeTable)[systemScopeAddr].scopeCoor1;
        nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
        currentAR = sysAR;

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
                    cout << "Executer::addArgsToCurrentAR()"
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
            && (*currentCode).assemblyFormula.flagBit.execute == true) {
        if (currentCode->assemblyFormula.flagBit.arg1_flag == S_Dz) {
            (*currentAR).add((*currentCode)[0]);
        } else if (currentCode->assemblyFormula.flagBit.arg1_flag == Y_Dz) {
            globalRefTable.addRefData(this->currentAR, codeTable,
                    currentCodeAddr, 0);
            (*currentAR).add((*currentCode)[0]);
        }
        if (currentCode->assemblyFormula.flagBit.arg2_flag == S_Dz) {
            (*currentAR).add((*currentCode)[1]);
        } else if (currentCode->assemblyFormula.flagBit.arg2_flag == Y_Dz) {
            globalRefTable.addRefData(this->currentAR, codeTable,
                    currentCodeAddr, 1);
            (*currentAR).add((*currentCode)[1]);
        }
        if (currentCode->assemblyFormula.flagBit.operator_flag == S_Dz) {
            (*currentAR).add((*currentCode)[2]);
        } else if (currentCode->assemblyFormula.flagBit.operator_flag == Y_Dz) {
            globalRefTable.addRefData(this->currentAR, codeTable,
                    currentCodeAddr, 2);
            (*currentAR).add((*currentCode)[2]);
        }
        if (currentCode->assemblyFormula.flagBit.result_flag == S_Dz) {
            (*currentAR).add((*currentCode)[3]);
        } else if (currentCode->assemblyFormula.flagBit.result_flag == Y_Dz) {
            globalRefTable.addRefData(this->currentAR, codeTable,
                    currentCodeAddr, 3);
            (*currentAR).add((*currentCode)[3]);
        }
    } /*else if ((*currentCode).assemblyFormula.flagBit.type == M
     && (*currentCode).assemblyFormula.flagBit.execute == F_) {
     (*currentAR).add(argexe1);
     (*currentAR).add(argexe2);
     (*currentAR).add(operatorexe);
     (*currentAR).add(resultexe);

     }*/
    else if ((*currentCode).assemblyFormula.flagBit.type == QS
            && (*currentCode).assemblyFormula.flagBit.execute == T_) {
        (*currentAR).add(resultexe);
        (*currentAR)[(*currentCode).scopeAddr].dataFlagBit = S_AR;
        safe_delete((*currentAR)[(*currentCode).scopeAddr].content_ar);
        DataTable *scopeAR = (*currentAR)[(*currentCode).scopeAddr].content_ar =
                new DataTable(currentAR, currentAR, (*currentCode).scopeAddr);
        currentAR = scopeAR;

    } else if ((*currentCode).assemblyFormula.flagBit.type == QE
            && (*currentCode).assemblyFormula.flagBit.execute == T_) {

//        (*currentAR)[resultexe].dataFlagBit = S_AR;
        //todo: delete heap content in this AR

        currentAR = currentAR->parentAR;

    } else if ((*currentCode).assemblyFormula.flagBit.type == QS
            && (*currentCode).assemblyFormula.flagBit.execute == R
            && currentAR->type == SYSB
            && currentAR->scopeStructureAddr == currentCode->scopeAddr) {
        //此部分仅仅考虑类继承中的处理，
        //无需考虑因函数调用或条件语句执行引起的QS R的情况，因为其已经在其他部分处理过了
        Scope &currentScope = (*scopeTable)[currentCode->scopeAddr];
        System &currentSystem = (*systemTable)[currentCode->scopeAddr];
        set<Addr> decedentSystemAddrSet1;        //应该被继承的类
        set<Addr> decedentSystemAddrSet2;        //已经被继承的类
        for (auto &id : currentSystem.decedentSystemIDList) {
            decedentSystemAddrSet1.insert((*systemTable)[id].systemAddr);
        }
        for (auto &decedentAR : currentAR->queryARList) {

            decedentSystemAddrSet2.insert(decedentAR->scopeStructureAddr);
        }

        decedentSystemAddrSet1 -= decedentSystemAddrSet2;
        if (decedentSystemAddrSet1.size() == 0) {
            //所有类均已继承，直接return，后续将自动执行此类中的代码
            return 0;

        } else {
            //还有类没被继承，那么创建这个类的AR添加到当前AR的queryARList中
            Addr decedentSysAddr = *decedentSystemAddrSet1.begin();
            Scope &decedentSysScope = (*scopeTable)[decedentSysAddr];
            System &decedentSystem = (*systemTable)[decedentSysAddr];
            DataTable *parentARofDecedent = currentAR->getAccessibleAR(
                    decedentSysScope.parentScopeAddr);
            DataTable *decedentAR = new DataTable(parentARofDecedent,
                    parentARofDecedent, currentAR, decedentSysAddr, SYSB);
            decedentAR->meetAddr = currentCodeAddr;
            currentAR->queryARList.push_front(decedentAR);

            nextCodeAddr = (*scopeTable)[decedentSystem.systemAddr].scopeCoor1;
            nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
            currentAR = decedentAR;
        }

        /*  (*currentAR).add(resultexe);
         (*currentAR)[resultexe.arg_a].dataFlagBit = S_AR;
         (*currentAR)[resultexe.arg_a].scopeStructureAddr =
         currentCode->scopeAddr;
         safe_delete((*currentAR)[(*currentCode).scopeAddr].content_ar);
         DataTable *queryAR_ = nullptr;
         if ((*scopeTable)[(*currentCode).scopeAddr].requireScopeAddr
         != addrnull) {
         queryAR_ =
         (*currentAR)[(*scopeTable)[(*currentCode).scopeAddr].requireScopeAddr].content_ar;
         } else {
         queryAR_ = currentAR;
         }

         (*currentAR)[(*currentCode).scopeAddr].content_ar = new DataTable(
         currentAR, queryAR_, (*currentCode).scopeAddr);
         currentAR = (*currentAR)[(*currentCode).scopeAddr].content_ar;*/

    } else if ((*currentCode).assemblyFormula.flagBit.type == QE
            && (*currentCode).assemblyFormula.flagBit.execute == R
            && (*scopeTable)[currentCode->scopeAddr].conditionFlag == 0) {
        //处理非条件语句作用域结尾
#if debug
        {
            static int ignorecount = 0;
            cout << "Executer::addArgsToCurrentAR()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_execute.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif

        //(*currentAR)[resultexe].dataFlagBit = S_AR;
        nextCodeAddr = currentAR->meetAddr;

        if (nextCodeAddr == addrnull) {
            nextCodePos = codeTable->addrdq.size();
#if debug
            cout << "Executer::addArgsToCurrentAR 1,meetAddr == addrnull ,"
                    "program will finish\n" << endl;
#endif
        } else {
            nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
        }
        DataTable *ar_delete = currentAR;
        currentAR = currentAR->returnAR;
        if (ar_delete->type != SYSB) {
            //todo: delete heap content in ar_delete

        }

    } else if ((*currentCode).assemblyFormula.flagBit.type == QE
            && (*currentCode).assemblyFormula.flagBit.execute == R
            && (*scopeTable)[currentCode->scopeAddr].conditionFlag == BRB) {
#if debug
        {
            static int ignorecount = 0;
            cout << "Executer::addArgsToCurrentAR()" << " QE R BRB"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "vm_execute.hpp:" << __LINE__ << ":0" << ")" << endl;
        }
#endif

//        (*currentAR)[resultexe].dataFlagBit = S_AR;

        Scope &conditionalBodyScope = (*scopeTable)[currentCode->scopeAddr];
        Addr &conditionCodeAddr = conditionalBodyScope.conditionCodeAddr;
        Addr &&possibleNextBranchCodeAddr = getNextAssociateBranchAddr(
                conditionCodeAddr);

        if (possibleNextBranchCodeAddr != addrnull
                && (*codeTable)[possibleNextBranchCodeAddr][0]
                        == (*codeTable)[currentCodeAddr][3]) {
            nextCodeAddr = possibleNextBranchCodeAddr;
            nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);

        } else {
            nextCodePos = currentCodePos + 1;
            setNextCodePos(nextCodePos);
            nextCodeAddr = codeTable->addrdq[nextCodePos];
        }
        DataTable *ar_delete = currentAR;
        currentAR = currentAR->parentAR;
        //todo: delete heap content in ar_delete

    } else if ((*currentCode).assemblyFormula.flagBit.type == QE
            && (*currentCode).assemblyFormula.flagBit.execute == R
            && (*scopeTable)[currentCode->scopeAddr].conditionFlag == LOB) {
#if debug
        {
            static int ignorecount = 0;
            cout << "Executer::addArgsToCurrentAR()" << " QE R LOB"
                    << "\tignorecount:[" << ignorecount++ << "\t]("
                    << "vm_execute.hpp:" << __LINE__ << ":0" << ")" << endl;
        }
#endif

//        (*currentAR)[resultexe].dataFlagBit = S_AR;

        Scope &conditionalBodyScope = (*scopeTable)[currentCode->scopeAddr];
        Addr &conditionCodeAddr = conditionalBodyScope.conditionCodeAddr;
        Code &conditionCode = (*codeTable)[conditionCodeAddr];
        Arg &&conditionResult = conditionCode[1];

        if (conditionResult.argFlag == S_Dz) {
            const Addr &&lastM = codeTable->getLast(M,
                    conditionalBodyScope.scopeCoor1);
            if ((*codeTable)[lastM][3] == conditionResult) {
                nextCodeAddr = codeTable->getLastNot(M, lastM);
                nextCodeAddr = codeTable->getNext(M, nextCodeAddr);
                nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
            } else {
#if debug
                {
                    static int ignorecount = 0;
                    cerr << "Executer::addArgsToCurrentAR()" << " err"
                            << "\tignorecount:[" << ignorecount++ << "\t]("
                            << "vm_execute.hpp:" << __LINE__ << ":0" << ")"
                            << endl;
                }
#endif
                exit(-1);
            }
        } else {
            nextCodePos = currentCodePos + 1;
            setNextCodePos(nextCodePos);
            nextCodeAddr = codeTable->addrdq[nextCodePos];
        }
        DataTable *ar_delete = currentAR;
        currentAR = currentAR->parentAR;
        //todo: delete heap content in ar_delete
        //delete ar_delete;

    } else if ((*currentCode).assemblyFormula.flagBit.type == R
            && (*currentCode).assemblyFormula.flagBit.execute == T_) {

        (*currentAR)["ans"] = (*currentAR)[argexe1];

        DataTable *ar_delete;
        while (currentAR->returnAR == NULL) {
            ar_delete = currentAR;
            currentAR = currentAR->parentAR;
            //todo:delete heap content in ar_delete

        }
        nextCodeAddr = currentAR->meetAddr;
        if (nextCodeAddr == addrnull) {
            nextCodePos = codeTable->addrdq.size();
#if debug
            cout << "Executer::addArgsToCurrentAR 2,meetAddr == addrnull ,"
                    "program will finish\n" << endl;
#endif
        } else {
            nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
        }
        ar_delete = currentAR;
        currentAR = currentAR->returnAR;
        //todo:delete heap content in ar_delete

    } else if ((*currentCode).assemblyFormula.flagBit.type == B
            && (*currentCode).assemblyFormula.flagBit.execute == T_) {
        bool exitBranch = false;
        if (argexe1.argFlag == S_Dz) {
            exitBranch = (*currentAR)[argexe1].content_i;

        } else if (argexe1.argFlag == Sz) {
            exitBranch = argexe1.arg_i;

        } else {
#if debug
            cerr
                    << "Executer::addArgsToCurrentAR err ,B type Code invalid argexe1, exit(-1)"
                    << endl;
#endif
            exit(-1);
        }
        bool conditionResult = false;
        if (exitBranch == false) {
            if (argexe2.argFlag == S_Dz || argexe2.argFlag == S_Bs) {
                conditionResult = (*currentAR)[argexe2].content_i;

            } else if (argexe2.argFlag == Sz) {
                conditionResult = argexe2.arg_i;

            } else {
#if debug
                cerr
                        << "Executer::addArgsToCurrentAR err ,B type Code invalid argexe2, exit(-1)"
                        << endl;
#endif
                exit(-1);
            }

        } else {
            currentAR->add(resultexe);
            (*currentAR)[resultexe] = Data(true);
            Addr &&nextAssociateBranchAddr = getNextAssociateBranchAddr(
                    currentCodeAddr);

            if (nextAssociateBranchAddr != addrnull
                    && (*codeTable)[nextAssociateBranchAddr][0] == resultexe) {

                nextCodeAddr = nextAssociateBranchAddr;
                nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
            } else {

                nextCodePos = currentCodePos + 1;
                setNextCodePos(nextCodePos);
                nextCodeAddr = codeTable->addrdq[nextCodePos];

            }
            return true;

        };
        bool exitNextBranch = false;
        if (conditionResult == true) {
            exitNextBranch = true;
            (*currentAR)[argexe1] = Data(true);
            Scope &conditionalBodyScope = (*scopeTable)[operatorexe.arg_a];
            nextCodeAddr = conditionalBodyScope.scopeCoor1;
            nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
        } else {
            exitNextBranch = false;
        }
        currentAR->add(resultexe);
        (*currentAR)[resultexe] = Data(exitNextBranch);
        return true;

    } else if ((*currentCode).assemblyFormula.flagBit.type == L
            && (*currentCode).assemblyFormula.flagBit.execute == T_) {
        bool exitLoop = false;
        if (argexe1.argFlag == S_Dz) {
            if (currentAR->findcurrent(argexe1)) {
                exitLoop = (*currentAR)[argexe1].content_i;
            } else {
                exitLoop = argexe1.arg_i;
            }

        } else {
#if debug
            cerr
                    << "Executer::addArgsToCurrentAR err ,L type Code invalid argexe1, exit(-1)"
                    << endl;
#endif
            exit(-1);
        }
        bool conditionResult = false;
        if (exitLoop == false) {
            if (argexe2.argFlag == S_Dz || argexe2.argFlag == S_Bs) {
                conditionResult = (*currentAR)[argexe2].content_i;

            } else if (argexe2.argFlag == Sz) {
                conditionResult = argexe2.arg_i;

            } else {
#if debug
                cerr
                        << "Executer::addArgsToCurrentAR err ,L type Code invalid argexe2, exit(-1)"
                        << endl;
#endif
                exit(-1);
            }

        }

        if (exitLoop == false && conditionResult == true) {

            Scope &conditionalBodyScope = (*scopeTable)[operatorexe.arg_a];
            nextCodeAddr = conditionalBodyScope.scopeCoor1;
            nextCodePos = getAddrPosition(codeTable->addrdq, nextCodeAddr);
        }
    }
    /*else if ((*currentCode).assemblyFormula.flagBit.type == QE
     && (*currentCode).assemblyFormula.flagBit.execute == R) {

     (*currentAR)[resultexe].dataFlagBit = S_AR;

     currentAR = currentAR->parentAR;

     }*/

    return true;
}
const bool Executer::executeCurrentCode() {
    {
#if debug
        static Intg ignorecount = 0;
        cout << "Executer::executeCurrentCode, currentCodeAddr:["
                << currentCodeAddr.toStrg() << "] in,ignorecount:["
                << ignorecount++ << "]" << endl;
#endif
#if debug
        cout << "Executer::executeCurrentCode 1" << endl;
#endif
    }
    if (currentCode->assemblyFormula.flagBit.type == B
            || currentCode->assemblyFormula.flagBit.type == L) {
#if debug
        cout << "Executer::executeCurrentCode B L " << endl;
#endif
        Code &nextCode = (*codeTable)[nextCodeAddr];
        if (operatorexe.arg_a == nextCode.scopeAddr) {

            DataTable *conditionalBodyAR = new DataTable();
            conditionalBodyAR->parentAR = currentAR;
            conditionalBodyAR->returnAR = currentAR;
            conditionalBodyAR->meetAddr = currentCodeAddr;

            conditionalBodyAR->queryARList.push_back(currentAR);
            conditionalBodyAR->scopeStructureAddr = nextCode.scopeAddr;

            setNextCodePos(++nextCodePos);
            nextCodeAddr = codeTable->addrdq[nextCodePos];

            currentAR = conditionalBodyAR;
        } else {

            return true;
        }

    }
    if (currentCode->assemblyFormula.flagBit.type == C) {
        //创建函数不做处理
    }
//todo::从此处开始处理函数类型F
    if (currentCode->assemblyFormula.flagBit.functionHandleFlag == F_) {
#if debug
        cout << "Executer::executeCurrentCode 2 out" << endl;
#endif

        return true;
    }
//todo::从此处开始处理函数调用的根节点 functionHandleFlag == true
    if (currentCode->assemblyFormula.flagBit.tfAddr == addrnull) {
#if debug
        cout << "Executer::executeCurrentCode 3 out" << endl;
#endif

        return true;
    } else if (currentCode->assemblyFormula.flagBit.tfAddr < addrnull) {
#if debug
        //todo::从此处开始处理函数调用的根节点 functionHandleFlag == true 内置函数
        cout << "Executer::executeCurrentCode 4,tfaddr<addrnull" << endl;
#endif

        bool compareresult = false;
        Numb *a1_i = NULL;
        Numb *a2_i = NULL;
        Numb *res_i = NULL;
        Strg *a1_s = NULL;
        Strg *a2_s = NULL;
        Strg *res_s = NULL;
        Data *d1 = NULL;
        Data *d2 = NULL;
        Data *dres = NULL;
        bool isImmediateData_a1 = false;
        bool isImmediateData_a2 = false;
        bool isImmediateData_res = false;
        if (argexe1.argFlag == Sz) {
            a1_i = &argexe1.arg_i;
        } else if (argexe1.argFlag == Zf) {
            a1_s = &argexe1.arg_s;
        } else if (argexe1.isVar()) {
            if (argexe1.argFlag == Y_Dz) {
                argexe1.arg_ar = currentAR;
                Arg argtmp = globalRefTable[argexe1];
                DataTable *artmp =
                        globalRefTable.getReferenceData(argexe1).getDestAR();
                if (argtmp.isVar()) {
                    d1 = &(*artmp)[argtmp];
                } else {

                    d1 = NULL;
                    isImmediateData_a1 = true;
                    if (argtmp.argFlag == Sz) {
#if debug
                        {
                            static int ignorecount = 0;
                            cout << "Executer::executeCurrentCode()" << " "
                                    << "\tignorecount:[" << ignorecount++
                                    << "\t](" << "vm_execute.hpp:" << __LINE__
                                    << ":0" << ")" << endl;
                        }
#endif
                        a1_i = new Numb(argtmp.arg_i);
                        d1 = new Data(argtmp.arg_i);

                    } else if (argtmp.argFlag == Zf) {
                        a1_s = new Strg(argtmp.arg_s);
                        d1 = new Data(argtmp.arg_s);
                    }

                }

            } else {
                try {
                    d1 = &(*currentAR)[argexe1];
                } catch (Arg &argr) {
                    d1 = NULL;
                    isImmediateData_a1 = true;
                    if (argr.argFlag == Sz) {
#if debug
                        {
                            static int ignorecount = 0;
                            cout << "Executer::executeCurrentCode()" << " "
                                    << "\tignorecount:[" << ignorecount++
                                    << "\t](" << "vm_execute.hpp:" << __LINE__
                                    << ":0" << ")" << endl;
                        }
#endif
                        a1_i = new Numb(argr.arg_i);
                        d1 = new Data(argr.arg_i);

                    } else if (argr.argFlag == Zf) {
                        a1_s = new Strg(argr.arg_s);
                        d1 = new Data(argr.arg_s);
                    }
                }
            }
            if (d1 && !isImmediateData_a1) {
                if (d1->dataFlagBit == Sz) {
                    a1_i = &d1->content_i;
                } else if (d1->dataFlagBit == Zf) {
                    a1_s = &d1->content_s;
                } else if (d1->dataFlagBit == S_Bs || d1->dataFlagBit == S_Dz) {
                    a1_i = &d1->content_i;
                    a1_s = &d1->content_s;

                } //todo: a1_ar.....
            }

        }
        if (argexe2.argFlag == Sz) {
            a2_i = &argexe2.arg_i;
        } else if (argexe2.argFlag == Zf) {
            a2_s = &argexe2.arg_s;
        } else if (argexe2.isVar()) {
            if (argexe2.argFlag == Y_Dz) {
                argexe2.arg_ar = currentAR;
                Arg argtmp = globalRefTable[argexe2];
                DataTable *artmp =
                        globalRefTable.getReferenceData(argexe2).getDestAR();
                if (argtmp.isVar()) {
                    d2 = &(*artmp)[argtmp];
                } else {

                    d2 = NULL;
                    isImmediateData_a2 = true;
                    if (argtmp.argFlag == Sz) {
#if debug
                        {
                            static int ignorecount = 0;
                            cout << "Executer::executeCurrentCode()" << " "
                                    << "\tignorecount:[" << ignorecount++
                                    << "\t](" << "vm_execute.hpp:" << __LINE__
                                    << ":0" << ")" << endl;
                        }
#endif
                        a2_i = new Numb(argtmp.arg_i);
                        d2 = new Data(argtmp.arg_i);

                    } else if (argtmp.argFlag == Zf) {
                        a2_s = new Strg(argtmp.arg_s);
                        d2 = new Data(argtmp.arg_s);
                    }

                }

            } else {
                try {
                    d2 = &(*currentAR)[argexe2];
                } catch (Arg &argr) {
                    d2 = NULL;
                    isImmediateData_a2 = true;
                    if (argr.argFlag == Sz) {
                        a2_i = new Numb(argr.arg_i);
                        d2 = new Data(argr.arg_i);

                    } else if (argr.argFlag == Zf) {
                        a2_s = new Strg(argr.arg_s);
                        d2 = new Data(argr.arg_s);
                    }
                }
            }

            if (d2 && !isImmediateData_a2) {
                if (d2->dataFlagBit == Sz) {
                    a2_i = &d2->content_i;
                } else if (d2->dataFlagBit == Zf) {
                    a2_s = &d2->content_s;
                } else if (d2->dataFlagBit == S_Bs || d2->dataFlagBit == S_Dz) {
                    a2_i = &d2->content_i;
                    a2_s = &d2->content_s;

                } //todo: a2_ar.....
            }

        }
        if (resultexe.argFlag == Sz) {
            res_i = &resultexe.arg_i;
        } else if (resultexe.argFlag == Zf) {
            res_s = &resultexe.arg_s;
        } else if (resultexe.isVar()) {
            if (resultexe.argFlag == Y_Dz) {
                resultexe.arg_ar = currentAR;
                Arg argtmp = globalRefTable[resultexe];
                DataTable *artmp =
                        globalRefTable.getReferenceData(resultexe).getDestAR();
                if (argtmp.isVar()) {
                    dres = &(*artmp)[argtmp];
                } else {

                    dres = NULL;
                    isImmediateData_res = true;
                    if (argtmp.argFlag == Sz) {
#if debug
                        {
                            static int ignorecount = 0;
                            cout << "Executer::executeCurrentCode()" << " "
                                    << "\tignorecount:[" << ignorecount++
                                    << "\t](" << "vm_execute.hpp:" << __LINE__
                                    << ":0" << ")" << endl;
                        }
#endif
                        res_i = new Numb(argtmp.arg_i);
                        dres = new Data(argtmp.arg_i);

                    } else if (argtmp.argFlag == Zf) {
                        res_s = new Strg(argtmp.arg_s);
                        dres = new Data(argtmp.arg_s);
                    }

                }

            } else {
                try {
                    dres = &(*currentAR)[resultexe];
                } catch (Arg &argr) {
                    dres = NULL;
                    isImmediateData_res = true;
                    if (argr.argFlag == Sz) {
                        res_i = new Numb(argr.arg_i);
                        dres = new Data(argr.arg_i);

                    } else if (argr.argFlag == Zf) {
                        res_s = new Strg(argr.arg_s);
                        dres = new Data(argr.arg_s);
                    }
                }
            }
            if (dres && !isImmediateData_res) {
                if (dres->dataFlagBit == Sz) {
                    res_i = &dres->content_i;
                } else if (dres->dataFlagBit == Zf) {
                    res_s = &dres->content_s;
                } else if (dres->dataFlagBit == S_Bs
                        || dres->dataFlagBit == S_Dz) {
                    res_i = &dres->content_i;
                    res_s = &dres->content_s;

                } //todo: res_ar.....
            }

        }
        {
#if debug
            static Intg ignorecount = 0;
            cout << "Executer::executeCurrentCode 5,ignorecount:["
                    << ignorecount++ << "]" << endl;
#endif
        }
        if (d1 != NULL) {
#if debug
            cout << "\td1:" << d1->toStrg() << endl;
#endif
        }
        if (d2 != NULL) {
#if debug
            cout << "\td2:" << d2->toStrg() << endl;
#endif
        }
        if (dres != NULL) {
#if debug
            cout << "\tdres:" << dres->toStrg() << endl;
#endif
        }

        switch (currentCode->assemblyFormula.flagBit.tfAddr[0]) {
        case S_S_JIA:
#if debug
            cout << "Executer::executeCurrentCode 5.1 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == "+");
            if (compareresult == true) {
                if (a1_i && a2_i) {
                    (*res_i) = (*a1_i) + (*a2_i);
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Sz) {
                        d2->dataFlagBit = Sz;
                    }
                    if (dres && dres->dataFlagBit != Sz) {
                        dres->dataFlagBit = Sz;
                    }

                } else if (a1_s && a2_s) {
                    (*res_s) = (*a1_s) + (*a2_s);
                    if (d1 && d1->dataFlagBit != Zf) {
                        d1->dataFlagBit = Zf;
                    }
                    if (d2 && d2->dataFlagBit != Zf) {
                        d2->dataFlagBit = Zf;
                    }
                    if (dres && dres->dataFlagBit != Zf) {
                        dres->dataFlagBit = Zf;
                    }
                } else {
#if debug
                    cerr
                            << "Executer::executeCurrentCode ,S_S_JIA err,exit(-1),currentCode:["
                            << currentCode->toStrg() << "]" << endl;
#endif
                    exit(-1);
                }

            }
#if debug
            cout << "Executer::executeCurrentCode 5.1 out" << endl;
#endif

            break;
        case S_S_JIAN:
#if debug
            cout << "Executer::executeCurrentCode 5.2 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == "-");
            if (compareresult == true) {
                (*res_i) = (*a1_i) - (*a2_i);
                if (d1 && d1->dataFlagBit != Sz) {
                    d1->dataFlagBit = Sz;
                }
                if (d2 && d2->dataFlagBit != Sz) {
                    d2->dataFlagBit = Sz;
                }
                if (dres && dres->dataFlagBit != Sz) {
                    dres->dataFlagBit = Sz;
                }
                /*(*res_s) = (*a1_s) - (*a2_s);*/
            }
#if debug
            cout << "Executer::executeCurrentCode 5.2 out" << endl;
#endif
            break;
        case S_S_CHENG:
#if debug
            cout << "Executer::executeCurrentCode 5.3 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == "*");
            if (compareresult == true) {
                (*res_i) = (*a1_i) * (*a2_i);
                if (d1 && d1->dataFlagBit != Sz) {
                    d1->dataFlagBit = Sz;
                }
                if (d2 && d2->dataFlagBit != Sz) {
                    d2->dataFlagBit = Sz;
                }
                if (dres && dres->dataFlagBit != Sz) {
                    dres->dataFlagBit = Sz;
                }

            }
#if debug
            cout << "Executer::executeCurrentCode 5.3 out" << endl;
#endif
            break;
        case S_S_CHU:
#if debug
            cout << "Executer::executeCurrentCode 5.4 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == "/");
            if (compareresult == true) {
                if ((*a2_i) == 0) {
#if debug
                    cerr << "divide 0 err,exit(-1)" << endl;
#endif
                    exit(-1);
                }
                (*res_i) = (*a1_i) / (*a2_i);
                if (d1 && d1->dataFlagBit != Sz) {
                    d1->dataFlagBit = Sz;
                }
                if (d2 && d2->dataFlagBit != Sz) {
                    d2->dataFlagBit = Sz;
                }
                if (dres && dres->dataFlagBit != Sz) {
                    dres->dataFlagBit = Sz;
                }

            }
#if debug
            cout << "Executer::executeCurrentCode 5.4 out" << endl;
#endif
            break;
        case S_S_FUZHI:
#if debug
            cout << "Executer::executeCurrentCode 5.5 in" << endl;
#endif
            compareresult =
                    argexe1.isVar()
                            && (operatorexe.argFlag == M_Bs
                                    && operatorexe.arg_s == "=");
            if (compareresult == true) {
                if (a1_i && a2_i) {
                    (*res_i) = (*a2_i);
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Sz) {
                        d2->dataFlagBit = Sz;
                    }
                    if (dres && dres->dataFlagBit != Sz) {
                        dres->dataFlagBit = Sz;
                    }
                } else if (a1_s && a2_s) {
                    (*res_s) = (*a2_s);
                } else {
#if debug
                    cerr
                            << "Executer::executeCurrentCode ,S_S_FUZHI err,exit(-1),currentCode:["
                            << currentCode->toStrg() << "]" << endl;
#endif
                    exit(-1);
                }

            }
#if debug
            cout << "Executer::executeCurrentCode 5.5 out" << endl;
#endif
            break;
        case S_S_DENGYU:
#if debug
            cout << "Executer::executeCurrentCode 5.6 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == "==");
            if (compareresult == true) {
                if (a1_i && a2_i) {
                    (*res_i) = ((*a1_i) == (*a2_i));
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Sz) {
                        d2->dataFlagBit = Sz;
                    }
                    if (dres && dres->dataFlagBit != Sz) {
                        dres->dataFlagBit = Sz;
                    }
                } else if (a1_s && a2_s) {
                    (*res_i) = ((*a1_s) == (*a2_s));
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Zf) {
                        d2->dataFlagBit = Zf;
                    }
                    if (dres && dres->dataFlagBit != Zf) {
                        dres->dataFlagBit = Zf;
                    }
                } else {
#if debug
                    cerr
                            << "Executer::executeCurrentCode ,S_S_DENGYU err,exit(-1),currentCode:["
                            << currentCode->toStrg() << "]" << endl;
#endif
                    exit(-1);
                }
            }
#if debug
            cout << "Executer::executeCurrentCode 5.6 out" << endl;
#endif
            break;
        case S_S_DAYU:
#if debug
            cout << "Executer::executeCurrentCode 5.6.1 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == ">");
            if (compareresult == true) {
                if (a1_i && a2_i) {
                    (*res_i) = ((*a1_i) > (*a2_i));
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Sz) {
                        d2->dataFlagBit = Sz;
                    }
                    if (dres && dres->dataFlagBit != Sz) {
                        dres->dataFlagBit = Sz;
                    }
                } else if (a1_s && a2_s) {
                    (*res_i) = ((*a1_s) > (*a2_s));
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Zf) {
                        d2->dataFlagBit = Zf;
                    }
                    if (dres && dres->dataFlagBit != Zf) {
                        dres->dataFlagBit = Zf;
                    }
                } else {
#if debug
                    cerr
                            << "Executer::executeCurrentCode ,S_S_DAYU err,exit(-1),currentCode:["
                            << currentCode->toStrg() << "]" << endl;
#endif
                    exit(-1);
                }
            }
#if debug
            cout << "Executer::executeCurrentCode 5.6.1 out" << endl;
#endif
            break;
        case S_S_XIAOYU:
#if debug
            cout << "Executer::executeCurrentCode 5.6.2 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == "<");
            if (compareresult == true) {
                if (a1_i && a2_i) {
                    (*res_i) = ((*a1_i) < (*a2_i));
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Sz) {
                        d2->dataFlagBit = Sz;
                    }
                    if (dres && dres->dataFlagBit != Sz) {
                        dres->dataFlagBit = Sz;
                    }
                } else if (a1_s && a2_s) {
                    (*res_i) = ((*a1_s) < (*a2_s));
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Zf) {
                        d2->dataFlagBit = Zf;
                    }
                    if (dres && dres->dataFlagBit != Zf) {
                        dres->dataFlagBit = Zf;
                    }
                } else {
#if debug
                    cerr
                            << "Executer::executeCurrentCode ,S_S_XIAOYU err,exit(-1),currentCode:["
                            << currentCode->toStrg() << "]" << endl;
#endif
                    exit(-1);
                }
            }
#if debug
            cout << "Executer::executeCurrentCode 5.6.2 out" << endl;
#endif
            break;
        case S_S_MI:
#if debug
            cout << "Executer::executeCurrentCode 5.7 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == "^");
            if (compareresult == true) {
                if (a1_i && a2_i) {
                    float a1 = (*a1_i);
                    float a2 = (*a2_i);
                    (*res_i) = pow(a1, a2);
                    if (d1 && d1->dataFlagBit != Sz) {
                        d1->dataFlagBit = Sz;
                    }
                    if (d2 && d2->dataFlagBit != Sz) {
                        d2->dataFlagBit = Sz;
                    }
                    if (dres && dres->dataFlagBit != Sz) {
                        dres->dataFlagBit = Sz;
                    }
                } else {
#if debug
                    cerr
                            << "Executer::executeCurrentCode ,S_S_MI err,exit(-1),currentCode:["
                            << currentCode->toStrg() << "]" << endl;
#endif
                    exit(-1);
                }
            }
#if debug
            cout << "Executer::executeCurrentCode 5.7 out" << endl;
#endif
            break;
        case CALL:
#if debug
            cout << "Executer::executeCurrentCode 5.8 in" << endl;
#endif
            compareresult =
                    (operatorexe.argFlag == M_Bs
                            && (operatorexe.arg_s == "call"
                                    || operatorexe.arg_s == "ca"));
#if debug
            cout << "Executer::executeCurrentCode 5.8 out" << endl;
#endif
            break;
        case SHUCHU:
#if debug
            cout << "Executer::executeCurrentCode 5.9 in" << endl;
#endif
            compareresult = (operatorexe.argFlag == M_Bs
                    && operatorexe.arg_s == "-->");
            if (compareresult == true) {
                if (a2_i) {
                    if ((*a2_i) == 0) {

                        cout
                                << "------------------------------------------"
                                        "debug output-----------------------------------------\n"

                                << d1->toStrg()
                                << "\n"
                                        "------------------------------------------"
                                        "-----------------------------------------------------"

                                << endl;

                    }

                } /*else if (a2_s) {
                 (*res_s) = (*a2_s);
                 } */else {
#if debug
                    cerr
                            << "Executer::executeCurrentCode ,SHUCHU err,exit(-1),currentCode:["
                            << currentCode->toStrg() << "]" << endl;
#endif
                    exit(-1);
                }

            }
#if debug
            cout << "Executer::executeCurrentCode 5.9 out" << endl;
#endif
            break;

        default:
            break;
        }
        if (isImmediateData_a1) {
#if debug
            {
                static int ignorecount = 0;
                cout << "Executer::executeCurrentCode()" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "vm_execute.hpp:" << __LINE__ << ":0" << ")" << endl;
            }
#endif
            safe_delete(a1_i);
#if debug
            {
                static int ignorecount = 0;
                cout << "Executer::executeCurrentCode()" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "vm_execute.hpp:" << __LINE__ << ":0" << ")" << endl;
            }
#endif
            delete a1_s;
#if debug
            {
                static int ignorecount = 0;
                cout << "Executer::executeCurrentCode()" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "vm_execute.hpp:" << __LINE__ << ":0" << ")" << endl;
            }
#endif
            safe_delete(d1);
#if debug
            {
                static int ignorecount = 0;
                cout << "Executer::executeCurrentCode()" << " "
                        << "\tignorecount:[" << ignorecount++ << "\t]("
                        << "vm_execute.hpp:" << __LINE__ << ":0" << ")" << endl;
            }
#endif
        }
        if (isImmediateData_a2) {
            delete a2_i;
            delete a2_s;
            safe_delete(d2);
        }
        if (isImmediateData_res) {
            delete res_i;
            delete res_s;
            safe_delete(dres);
        }
        if (compareresult == true) {
#if debug
            cout << "Executer::executeCurrentCode 6 out" << endl;
#endif
            return true;
        } else {
#if debug
            cerr << "Executer::executeCurrentCode tfaddr<addrnull err,exit(-1)"
                    << endl;
#endif
            exit(-1);
            return false;
        }

    } else if (currentCode->assemblyFormula.flagBit.tfAddr > addrnull) {
        //todo:: 从此处开始处理非内置模板函数调用的根节点
#if debug
        {
            static int ignorecount = 0;
            cout << "Executer::executeCurrentCode()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_execute.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif
#if debug
        cout << "Executer::executeCurrentCode 7 , tfaddr>addrnull" << endl;
#endif
        map<ArgFormal, ArgReal> &&frmap = getFRMap();

        Addr &functionNameScopeAddr =
                currentCode->assemblyFormula.flagBit.tfAddr;
        Addr &functionBodyScopeAddr =
                (*templateFunctionTable)[functionNameScopeAddr].bodyScopeAddr;
        TemplateFunction &tf = (*templateFunctionTable)[functionNameScopeAddr];

        auto &bfMeetAddrdq = currentAR->bfMeetAddrdq;
        auto &backwardExecute = currentAR->backwardExecute;
        Addr meetAddr = nextCodeAddr;
        //是否跳过当前function，（正向执行过程中遇到非末尾逆向函数时要跳过这个函数）
        bool skipfunction = false;

        //执行遇到末尾
        if (tf.isBackwardFunction == F_
                && (*codeTable)[nextCodeAddr].assemblyFormula.flagBit.type != M
                && bfMeetAddrdq.size() > 0) {
            bfMeetAddrdq.push_front(nextCodeAddr);
            meetAddr = bfMeetAddrdq.back();
            bfMeetAddrdq.pop_back();
            if (meetAddr != nextCodeAddr) {
                backwardExecute = T_;
            } else {
                backwardExecute = F_;
            }
        } else if (tf.isBackwardFunction == T_
                && (*codeTable)[nextCodeAddr].assemblyFormula.flagBit.type != M
                && bfMeetAddrdq.size() > 0) {
            bfMeetAddrdq.push_front(nextCodeAddr);
            meetAddr = bfMeetAddrdq.back();
            bfMeetAddrdq.pop_back();
            backwardExecute = T_;
        }
        //正向执行，未到末尾
        else if (tf.isBackwardFunction == F_
                && (*codeTable)[nextCodeAddr].assemblyFormula.flagBit.type == M) {
            meetAddr = nextCodeAddr;
            backwardExecute = F_;

        } else if (tf.isBackwardFunction == T_
                && (*codeTable)[nextCodeAddr].assemblyFormula.flagBit.type == M
                && backwardExecute == F_) {
            meetAddr = nextCodeAddr;
            bfMeetAddrdq.push_back(currentCodeAddr);
            //这里注意初始化逆向函数的返回值，因为逆推时其要作为上一步的实参出现。
            Arg &&argreturn = (*currentCode)[3];
            if (currentAR->findcurrent(argreturn) == false) {
                currentAR->add(argreturn);
            }
            //正向执行遇到非末尾逆向函数要跳过
            skipfunction = true;

        }
        //逆向执行
        else if (tf.isBackwardFunction == T_ && backwardExecute == T_) {
            meetAddr = bfMeetAddrdq.back();
            bfMeetAddrdq.pop_back();
            if (bfMeetAddrdq.size() > 0) {
                backwardExecute = T_;
            } else {
                //如果bfmeetAddrdq为空，说明此次函数执行完毕后跳转到了一非M类型code中，此时需要正向执行
                backwardExecute = F_;
            }

        }

        if (skipfunction) {
            return true;
        }
        //初始化函数调用生成的AR
        DataTable *functionBodyParentAR = getParentAR(functionBodyScopeAddr);
        DataTable *functionBodyAR = new DataTable();
        functionBodyAR->parentAR = functionBodyParentAR;
        functionBodyAR->returnAR = currentAR;
        functionBodyAR->meetAddr = meetAddr;
        functionBodyAR->queryARList.push_back(functionBodyParentAR);
        functionBodyAR->scopeStructureAddr = functionBodyScopeAddr;
        functionBodyAR->setFRMap(frmap);

        nextCodePos = getAddrPosition(codeTable->addrdq,
                (*templateFunctionTable)[functionNameScopeAddr].bodyStartAddr);

        //将函数调用的AR添加到当前AR，转移至当前AR运行
        Arg arg_(Z_Dz, functionBodyScopeAddr);
        arg_.formalArg = true;
        currentAR->add(arg_);
        (*currentAR)[arg_].dataFlagBit = S_AR;
        (*currentAR)[arg_].content_ar = functionBodyAR;
        currentAR = functionBodyAR;

        setNextCodePos(++nextCodePos);
        nextCodeAddr = codeTable->addrdq[nextCodePos];

    } else {
#if debug
        cerr << "Executer::executeCurrentCode err, exit(-1)" << endl;
#endif
        exit(-1);
    }

    return true;
}
const bool Executer::execute() {
    globalRefTable.clear();
    globalRefTable.mode = MODE_EXECUTE;
    while (getNewCode() == true) {
#if debug
        {
            static int ignorecount = 0;
            cout << "Executer::execute()" << " " << "\tignorecount:["
                    << ignorecount++ << "\t]("
                    << "D:/eclipseC&CPP-workspace/HAN_VM/includes/vm_execute.hpp:"
                    << __LINE__ << ":0" << ")" << endl;
        }
#endif

        argexe1 = (*currentCode)[0];
        argexe2 = (*currentCode)[1];
        operatorexe = (*currentCode)[2];
        resultexe = (*currentCode)[3];

        if ((*currentCode).assemblyFormula.flagBit.type == C) {
#if debug
            cout << "Executer::execute C in,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
            addArgsToCurrentAR();

#if debug
            cout << "Executer::execute C out,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif

        } else if ((*currentCode).assemblyFormula.flagBit.type == R) {
#if debug
            cout << "Executer::execute R in,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
            addArgsToCurrentAR();
            executeCurrentCode();
#if debug
            cout << "Executer::execute R out,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
        } else if ((*currentCode).assemblyFormula.flagBit.type == M) {
#if debug
            cout << "Executer::execute M in,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
            addArgsToCurrentAR();
            executeCurrentCode();
#if debug
            cout << "Executer::execute M out,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
        } else if ((*currentCode).assemblyFormula.flagBit.type == Y
                && (*currentCode).assemblyFormula.flagBit.execute != Du) {
            addArgsToCurrentAR();
            executeCurrentCode();

        } else if ((*currentCode).assemblyFormula.flagBit.type == QS
                && (*currentCode).assemblyFormula.flagBit.execute == R_
                && currentAR->type == SYSB) {
            //专门用于处理类继承问题
            addArgsToCurrentAR();
        } else if ((*currentCode).assemblyFormula.flagBit.type == QS
                && (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
            cout << "Executer::execute QS in,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
            addArgsToCurrentAR();
            executeCurrentCode();
#if debug
            cout << "Executer::execute QS out,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
        }

        else if ((*currentCode).assemblyFormula.flagBit.type == QE
                && (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
            cout << "Executer::execute QE T_ in,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
            addArgsToCurrentAR();
            executeCurrentCode();
#if debug
            cout << "Executer::execute QE T_ out,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
        } else if ((*currentCode).assemblyFormula.flagBit.type == QE
                && (*currentCode).assemblyFormula.flagBit.execute == R) {
#if debug
            cout << "Executer::execute QE R in,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
            addArgsToCurrentAR();
            executeCurrentCode();
#if debug
            cout << "Executer::execute QE R out,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
        } else if ((*currentCode).assemblyFormula.flagBit.type == B
                && (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
            cout << "Executer::execute B T_ in,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
            addArgsToCurrentAR();
            executeCurrentCode();
#if debug
            cout << "Executer::execute B T_ out,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
        } else if ((*currentCode).assemblyFormula.flagBit.type == L
                && (*currentCode).assemblyFormula.flagBit.execute == T_) {
#if debug
            cout << "Executer::execute L T_ in,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
            addArgsToCurrentAR();
            executeCurrentCode();
#if debug
            cout << "Executer::execute L T_ out,currentCode:["
                    << currentCode->toStrg() << "]" << endl;
#endif
        } else if ((*currentCode).assemblyFormula.flagBit.type == TD
                && (*currentCode).assemblyFormula.flagBit.execute == T_) {
        }
    }

    return true;
}
}

#endif /* VM_EXECUTE_HPP_ */
