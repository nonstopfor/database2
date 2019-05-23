#ifndef MATH_CAL_H
#define MATH_CAL_H
#include <iostream>
#include <string>
#include <vector>
using namespace std;
//with elements input and result output
template <typename TY>
string CAA(vector<string> & hold_strs, string type_nami){
	
	
}


string CAL_alg(vector<string> &hold_strs, string type){
	//if any error occurs, return "NULL"
	if(type!="int(11)"&&type!="double")return "NULL";
	else if(type=="int(11)")return CAA<int>(hold_strs,type);
	else return CAA<double>(hold_strs,type);
	
}

#endif
