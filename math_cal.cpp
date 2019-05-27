#include "math_cal.h"

string CAL_alg(vector<string> &hold_strs, string type){
	//if any error occurs, return "NULL"
	if(type=="char(1)")return CAA_2(hold_strs);
	else if(type!="int(11)"&&type!="double")return "NULL";
	else if(type=="int(11)")return CAA<int>(hold_strs);
	else return CAA<double>(hold_strs);
	
}

string CAA_2(vector<string> & hold_strs){
    int str_turn=1;
    int cal_turn=0;
    string resu;
    for(int pos=0;pos<=hold_strs.size()-1;pos++,str_turn^=1,cal_turn^=1){
        if(str_turn){
            resu+=hold_strs[pos];
        }else{
            if(hold_strs[pos]!="+")return "NULL";
        }
    }
    return resu;
}