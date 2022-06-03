/*
 * vm_debug.hpp
 *
 *  Created on: Aug 8, 2021
 *      Author: han
 */

#ifndef VM_DEBUG_HPP_
#define VM_DEBUG_HPP_

#include <string>
#include <sstream>
#include <deque>
#include <map>

//#define Strg string
#define debug true
#define Intg int
#define Numb double

using namespace std;
namespace VM {
template<typename T_list>
class ListPrinter {
public:
    stringstream ss;
    ListPrinter(const T_list &list) {
        Intg j = list.size();
        for (int i = 0; i < j; i++) {
            ss << list.at(i).toStrg();
        }
    }

};

//use this variable to certainfy jump turns
//static Intg debugturns = 0;
}

#endif /* VM_DEBUG_HPP_ */
