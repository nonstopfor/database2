#include "math_cal.h"

string CAL_alg(vector<string> &hold_strs, string type){
	//if any error occurs, return "NULL"
	if(type!="int(11)"&&type!="double")return "NULL";
	else if(type=="int(11)")return CAA<int>(hold_strs);
	else return CAA<double>(hold_strs);
	
}