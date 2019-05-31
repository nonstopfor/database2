#ifndef MIAMIAFUNC
#define MIAMIAFUNC
#include <string>
#include <sstream>
#include <stack>
#include <iostream>
using namespace std;

//this function servers as the main function
//it will receive a string as input
//and return the reply to be printed
//the following is an example, which should be replaced by the real function
string miamia(string A){
    stringstream IN(A);
    stack<string> holdi;
    string result="";

    while(IN>>result)holdi.push(result);
    result="";
    while(holdi.size()>0){
        result+=holdi.top();
        result+=" ";
        holdi.pop();
    }

    return result;
}

#endif