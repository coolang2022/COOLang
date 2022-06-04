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

#include <iostream>
//#include <map>
//#
#include <vm.hpp>
//#define table map
using namespace VM;
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <iterator>
#include <algorithm>
#include <vm_execute.hpp>

using namespace std;
using namespace VM;
//class Print{
//  public:
//       void operator() (const Addr &ad){
//             cout<<ad.toStrg()<<" ";
//             return;
//         }
//  };

template<typename T>
class Print {
public:
    void operator()(const T &ad) {
        cout << ad.toStrg() << " ";
        return;
    }
};
int main(int argc, char *argv[]) {
    ///r
//    string s = "I  have  a  dream!";
//    vector<string> res = split(s, " ");
//    for(int i=0;i<(int)res.size();i++){
//        puts(res.at(i).c_str());
//
//    }
//    puts(s.c_str());
//    printf("%d\n",atoi(string("123").c_str()));  //r

///r
//    Addr a;
//    a="123.234.345";
//    a=a++;//"123.234.346";
//    puts(a.toStrg().c_str());
//
//    Addr b=a.createNext();//"123.234.347"
//    puts(b.toStrg().c_str());
//
//    Addr c=a.createSub();//"123.234.346.1"
//    puts(c.toStrg().c_str());
//
//    Addr d=a.createSub("2.1");//"123.234.346.2.1"
//    puts(d.toStrg().c_str());
//
//    Addr e=d.createNext(4);// "123.234.346.2.5"
//    puts(e.toStrg().c_str());
//
//    e=e+6; // "123.234.346.2.11"
//    puts(e.toStrg().c_str());

///r
//    Addr a;
//    a = "12.23.45";
//    puts(a.toStrg().c_str());
//
//    a = 1;
//    puts(a.toStrg().c_str());
//
//    if (a == 1 && a == "1") {
//        puts(a.toStrg().c_str());
//    }

///r
//    CodeTable cd;
//    Addr a;
//    Addr b;
//    a="12.34.56";
//    for(int i=0;i<30;i++){
//        a=a++;
//        cd.addrdq.push_back(a);
//        if(i==29){b=a;}
//    }
//    puts(b.toStrg().c_str());
//
//Addr c = cd.getCodeAddrByOffset(b, -10);
//c = cd.getNextCodeAddr(c);
//c = cd.getAboveCodeAddr(c);
//puts("result");
//puts(c.toStrg().c_str());

///r
//    CodeTable cd;
//    Addr a;
//    Addr b;
//    a = "12.34.56";
//    for (int i = 0; i < 30; i++) {
//        a = a++;
//        deque<Addr>::iterator ita=(cd.addrdq).begin()+i;
//            cd.addrdq.insert(ita, a);
//    }
//
//    for (int i = 0; i < 30; i++) {
//        a = a++;
//        deque<Addr>::iterator ita=(cd.addrdq).begin()+1;
//            cd.addrdq.insert(ita, a);
//    }
//    //Print p;
//    for_each(cd.addrdq.begin(),cd.addrdq.end(),Print());

///r
//        CodeTable cd;
//        Addr a;
//        Addr b;
//        a = "12.34.56";
//        for (int i = 0; i < 30; i++) {
//            a = a++;
//            deque<Addr>::iterator ita=(cd.addrdq).begin()+i;
//                cd.addrdq.insert(ita, a);
//        }
//
//        for (int i = 0; i < 30; i++) {
//            a = a++;
//            deque<Addr>::iterator ita=(cd.addrdq).begin()+1;
//                cd.addrdq.insert(ita, a);
//        }
//        //Print p;
//        for_each(cd.addrdq.begin(),cd.addrdq.end(),Print<Addr>());
//        puts("");
//        cd.addrdq.erase(cd.addrdq.begin()+2);
//        for_each(cd.addrdq.begin(),cd.addrdq.end(),Print<Addr>());
//

    CodeTable codeTable;
    ScopeTable scopeTable;
    TemplateFunctionTable templateFunctionTable;
    DataTable dataTable_scan;
    DataTable dataTable_preexe;
    DataTable dataTable_execute;
    SystemTable systemTable;

    Scanner s(&codeTable, &scopeTable, &templateFunctionTable,&dataTable_scan, &systemTable);
    Intg matchStateSiloMaxCapacity = 5;
    Intg codeTableSiloMaxCapacity = 10;
    Intg maxSwitchTurn = 15;
#if debug
    s.loadAssemblyCode("assemblyCode.txt");
#else
    s.loadAssemblyCode(Strg(argv[1]));

    if (argc == 4) {
        matchStateSiloMaxCapacity = atoi(argv[2]);
        codeTableSiloMaxCapacity = atoi(argv[3]);
    }
#endif
//    cout << "begin" << endl;
//    cout << s.assemblyCodeTable.toStrg() << endl;
//    cout << "end" << endl;
    s.scan();
    cout<<"scan后codeTable状态"<<endl;
    codeTable.toStrg();

    Integrator itg(&codeTable, &templateFunctionTable, &scopeTable, &dataTable_preexe,
            &systemTable);

//    cout << "matchStateSiloMaxCapacity:\r\n";
//    cin >> matchStateSiloMaxCapacity;
//    cout << "codeTableSiloMaxCapacity:\r\n";
//    cin >> codeTableSiloMaxCapacity;
    itg.setMaxCapacity(matchStateSiloMaxCapacity, codeTableSiloMaxCapacity,
            maxSwitchTurn);
    itg.integrate();
#if debug
    Intg pos = 0;
    cout << "*****************************************************************"
            << endl;
    cout << "*****************************************************************"
            << endl;
    for (Code &cd : codeTable.codedq) {
        cout << "Addr: " << codeTable.addrdq[pos].toStrg() << "   \t"
                << "Code: " << cd.toStrg() << endl;
        pos++;

    }
    cout << "*****************************************************************"
            << endl;
    cout << "*****************************************************************"
            << endl;
#endif
    Executer executer(&codeTable, &templateFunctionTable, &scopeTable,
            &dataTable_execute, &systemTable);
    executer.execute();

    return 0;

}
//int main() {
//  cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//  return 0;
//}
