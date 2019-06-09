#include "math_cal.h"

string CAL_alg(vector<string> &hold_strs, string type, bool& NULL_flag){
//cout<<"meet cal"<<endl;
	//if any error occurs, return "NULL"
    if(type == "date" || type == "time") return hold_strs[0];
	if(type=="char(1)"){
        if(CAA_2(hold_strs,NULL_flag)=="NULL")NULL_flag=true;
        return CAA_2(hold_strs,NULL_flag);
    }
    
	else if(type!="int(11)"&&type!="double"){
        NULL_flag=true;
        return "NULL";
    }
	else if(type=="int(11)"){
        if(CAA<int>(hold_strs,NULL_flag)=="NULL"){
//cout<<"one NULL"<<endl;
            NULL_flag=true;
        }
        return CAA<int>(hold_strs,NULL_flag);
    }
	else{
        if(CAA<double>(hold_strs,NULL_flag)=="NULL")NULL_flag=true;
        return CAA<double>(hold_strs,NULL_flag);
    }
	
}

string CAA_2(vector<string> & hold_strs,bool& NULL_flag){
    if(NULL_flag)return "NULL";
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
