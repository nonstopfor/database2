#include "column.h"
#include <cmath>
#include <iostream> 
using namespace std;



Column::Column(string a,string b,bool c): cname(a),can_be_null(c) {
    if (str_com(b,"INT")) type = "int(11)";
    else if (str_com(b,"CHAR")) type = "char(1)";
    else if (str_com(b,"TIME")) type = "time";
    else if (str_com(b,"DATE")) type = "date";
    else type = "double";
}

Column::~Column() {}

string Column::getname() const {return cname;} //获得列名的接口 

string Column::gettype() const {return type;} //获得列所对应变量类型的接口 

bool Column::can_null() const {return can_be_null;} //获得列所存储变量是否可为空的接口 

int Column::getsize() {return cvalue.size();} //获得列所存储数据的数量，即行数的接口 

void Column::insert(const string& a) { //插入新数据 
	string value = a;
	if ((value[0]=='"'&&value[value.length()-1]=='"')||(value[0]=='\''&&value[value.length()-1]=='\'')) { //删除字符头尾的引号 
		value.erase(value.begin());
		value.erase(value.end()-1);
	}
    if(type == "date"){
        string t = to_date(value);
        if(t.find('-') != 4) value = "NULL";
        else{
            int m = stoi(t.substr(5,2)), y = stoi(t.substr(0, t.find('-'))), d = stoi(t.substr(t.length() - 2));
            if(m == 2){
                if(y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)){if(d > 29) value = "NULL";}
                else if(d > 28) value = "NULL";
            }else if(d > monthd[m]) value = "NULL";
        }
        if(value != "NULL") value = t;
    }else if(type == "time"){
        string t = to_time(value);
        if(t[0] != '-' && t.find(':') > 3) value = "NULL";
        else if(t[0] == '-' && t.find(':') > 4) value = "NULL";
        int m = stoi(t.substr(t.length() - 5)), s = stoi(t.substr(t.length() - 2));
        if(m > 59 || s > 59) value = "NULL";
        if(value != "NULL") value = t;
    }
	cvalue.push_back(value);
}

void Column::del(int s) { //删除行 
	cvalue.erase(cvalue.begin()+s);
}

void Column::update(int s,const string& a) { //修改某行数据 
	string value = a;
	if ((value[0]=='"'&&value[value.length()-1]=='"')||(value[0]=='\''&&value[value.length()-1]=='\'')) {
		value.erase(value.begin());
		value.erase(value.end()-1);
	}
	cvalue[s] = value;
}

string& Column::operator[] (int s) { //重载[]以便于直接访问某一行数据 
	return cvalue[s];
}

