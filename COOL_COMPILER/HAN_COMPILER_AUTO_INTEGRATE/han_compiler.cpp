/*
 * han_compiler.cpp
 *
 *  Created on: 2021年11月5日
 *      Author: dell
 */

/*
 *interface
 */
#include "han_compiler.hpp"

using namespace HAN_COMPILER;
using namespace std;
void output() {
    return HAN_COMPILER::output();
}
void HAN_COMPILER::output() {
    cout << "HAN_COMPILER::output" << endl;
}
HAN_COMPILER::Compiler compiler;

void setOutputCodePath(const char *filePath) {
    try {
        compiler.setOutputCodePath(filePath);
    } catch (exception &e) {
        cout << "setOutputCodePath err" << e.what() << endl;
    }
}
Intg matchToken(Intg createNew, Intg tokenType, Intg argFlag,
        char *tokenString) {
    try {
        return compiler.matchToken(createNew, tokenType, argFlag, tokenString);
    } catch (exception &e) {
        cout << "matchToken err" << e.what() << endl;
        exit(1);
    }

}
void matchRule(Intg createNew, Intg ruleType, const Intg *tokenNumber1,
        const Intg *tokenNumber2, const Intg *tokenNumber3,
        const Intg *tokenNumber4) {
    try {
        Intg * tokenNumber1_;
        Intg * tokenNumber2_;
        Intg * tokenNumber3_;
        Intg * tokenNumber4_;
        tokenNumber1_ = const_cast<Intg*>(tokenNumber1);
        tokenNumber2_ = const_cast<Intg*>(tokenNumber2);
        tokenNumber3_ = const_cast<Intg*>(tokenNumber3);
        tokenNumber4_ = const_cast<Intg*>(tokenNumber4);
        compiler.matchRule(createNew, ruleType, tokenNumber1_, tokenNumber2_,
                tokenNumber3_, tokenNumber4_);
    } catch (exception &e) {
        cout << "matchRule err" << e.what() << endl;
    }

}
void matchFinish() {
    try {
        compiler.matchFinish();
    } catch (exception &e) {
        cout << "matchFinish err" << e.what() << endl;
    }
    compiler.matchFinish();
}
void flushMap() {
    try {
        compiler.flushMap();
    } catch (exception &e) {
        cout << "flushMap err" << e.what() << endl;
    }
    compiler.flushMap();
}
Intg HAN_COMPILER::toIntg(Strg s) {
    return atoi(s.c_str());
}
HAN_COMPILER::Strg HAN_COMPILER::toStrg(const Intg& i) {
    stringstream ss;
    ss << i;
    return ss.str();
}
bool HAN_COMPILER::isNumber(const string &str){
    stringstream ss(str);
    double d;
    char c;
    if (!(ss >> d))
        return false;
    if (ss >> c)
        return false;
    return true;
}

