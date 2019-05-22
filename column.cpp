#include "column.h"
#include <iostream> 
using namespace std;

bool str_com(const string& a,const string& b) { //忽略大小写的字符串比较函数 
	if (a.length()!=b.length()) return false;
	else {
		bool c;
		for (int i=0; i<a.length();i++) {
			if (isalpha(a[i])&&isalpha(b[i])) 
				c = (a[i]==b[i]) || (a[i]-b[i]=='A'-'a') || (a[i]-b[i]=='a'-'A');
			else c = (a[i]==b[i]);
			if (!c) return false;
		}
		return true;
	}
}

Column::Column(string a,string b,bool c): cname(a),can_be_null(c) {
    if (str_com(b,"INT")) type = "int(11)";
    else if (str_com(b,"CHAR")) type = "char(1)";
    else type = "double";
}

Column::~Column() {}

string Column::getname() const {return cname;} //获得列名的接口 

string Column::gettype() const {return type;} //获得列所对应变量类型的接口 

bool Column::can_null() const {return can_be_null;} //获得列所存储变量是否可为空的接口 

int Column::getsize() {return cvalue.size();} //获得列所存储数据的数量，即行数的接口 

void Column::insert(const string& a) { //插入新数据 
	string value = a;
	if ((value[0]=='\"'&&value[value.length()-1]=='\"')||(value[0]=='\''&&value[value.length()-1]=='\'')) { //删除字符头尾的引号 
		value.erase(value.begin());
		value.erase(value.end()-1);
	}
	cvalue.push_back(value);
}

void Column::del(int s) { //删除行 
	cvalue.erase(cvalue.begin()+s);
}

void Column::update(int s,const string& a) { //修改某行数据 
	string value = a;
	if ((value[0]=='\"'&&value[value.length()-1]=='\"')||(value[0]=='\''&&value[value.length()-1]=='\'')) {
		value.erase(value.begin());
		value.erase(value.end()-1);
	}
	cvalue[s] = value;
}

string& Column::operator[] (int s) { //重载[]以便于直接访问某一行数据 
	return cvalue[s];
}

string Tolower(string x){
	string u="";
	for(int i=0;i<x.length();++i){
		u+=tolower(x[i]);
	}
	return u;
}
vector<string> cut(const string& s) {
    int l = s.length();
    string u = "";
    vector<string> temp;
    
    //切割string s, 并过滤空格
    for(int i = 0; i < l; i++) {
        if(i == l - 1) {
            if(s[i] != ' ') u.push_back(s[i]);
            if(u != "") temp.push_back(u);
            break;
        }
        if(s[i] != ' '&&s[i]!='\t') u.push_back(s[i]);
        else if(u != "") {
            temp.push_back(u);
            u = "";
        }
    }
    return temp;
}

bool compare(string data1,string data2,string type1,string type2,string opt){
	if(opt=="like"){
		bool front=false,back=false;//记录最前面和最后面是否有%
		if(data2[0]=='%') front=true;
		if(data2.back()=='%') back=true;
		vector<string>need;//需要含有的字符串
		string u="";
		for(int i=0;i<data2.size();++i){
			if(data2[i]=='%'){
				if(u!=""){
					need.push_back(u);
					u="";
				}

			}
			else {
				u+=data2[i];
				if(i==data2.size()-1){
					need.push_back(u);
				}
			}
		}
		int now_pos=0;
		for(int i=0;i<need.size();++i){
			if(data1.size()<need[i].size()) return false;
			if(i==need.size()-1&&!back){
				string& x=need[i];
				if(data1.substr(data1.size()-x.size(),x.size())!=x) return false;
			}
			int x=data1.find(need[i],now_pos);
			if(x==-1) return false;
			if(!i&&!front){
				if(x!=0) return false;
			}

			now_pos=x+need[i].size();
		}
		return true;
	}
	else{
		if(type1!="char(1)"){
			if(type1=="int(11)"){
				int x=atoi(data1.c_str());
				if(type2=="int(11)"){
					int y=atoi(data2.c_str());
					if(opt=="="){
						return x==y;
					}
					else if(opt==">"){
						return x>y;
					}
					else if(opt=="<"){
						return x<y;
					}
				}
				else if(type2=="double"){
					double y=atof(data2.c_str());
					if(opt=="="){
						return x==y;
					}
					else if(opt==">"){
						return x>y;
					}
					else if(opt=="<"){
						return x<y;
					}
				}
			}
			else if(type1=="double"){
				double x=atof(data1.c_str());
				if(type2=="int(11)"){
					int y=atoi(data2.c_str());
					if(opt=="="){
						return x==y;
					}
					else if(opt==">"){
						return x>y;
					}
					else if(opt=="<"){
						return x<y;
					}
				}
				else if(type2=="double"){
					double y=atof(data2.c_str());
					if(opt=="="){
						return x==y;
					}
					else if(opt==">"){
						return x>y;
					}
					else if(opt=="<"){
						return x<y;
					}
				}
			}
			return false;
		}
		else{
			if(opt=="="){
				return data1==data2;
			}
			else if(opt==">"){
				return data1>data2;
			}
			else if(opt=="<"){
				return data1<data2;
			}
			return false;
		}
	}
}

void clear_space(string& data){
	while(data.front()==' '){
		data.erase(data.begin());
	}
	while(data.back()==' '){
		data.erase(data.begin()+data.size()-1);
	}
}
