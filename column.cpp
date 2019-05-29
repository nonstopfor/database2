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
	if(Tolower(opt)=="like"){
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
		if(type1=="char(1)"){
			if(opt=="<") return data1<data2;
			else if(opt==">") return data1>data2;
			else if(opt=="=") return data1==data2;
			else if(opt==">=") return data1>=data2;
			else if(opt=="<=") return data1<=data2;
			else if(opt=="!=") return data1!=data2;
			else{
				cout<<"wrong!";return false;
			}
		}
		else{
			if(type1=="int(11)") {
				if(type2=="int(11)") {
					int d1=stoi(data1),d2=stoi(data2);
					if(opt=="<") return d1<d2;
					else if(opt==">") return d1>d2;
					else if(opt=="=") return d1==d2;
					else if(opt==">=") return d1>=d2;
					else if(opt=="<=") return d1<=d2;
					else if(opt=="!=") return d1!=d2;
					else{
						cout<<"wrong!";return false;
					}
				}
				else if(type2=="double"){
					int d1=stoi(data1);double d2=stof(data2);
					if(opt=="<") return d1<d2;
					else if(opt==">") return d1>d2;
					else if(opt=="=") return d1==d2;
					else if(opt==">=") return d1>=d2;
					else if(opt=="<=") return d1<=d2;
					else if(opt=="!=") return d1!=d2;
					else{
						cout<<"wrong!";return false;
					}
				}
			}
			else if(type1=="double"){
				if(type2=="double"){
					double d1=stof(data1),d2=stof(data2);
					if(opt=="<") return d1<d2;
					else if(opt==">") return d1>d2;
					else if(opt=="=") return d1==d2;
					else if(opt==">=") return d1>=d2;
					else if(opt=="<=") return d1<=d2;
					else if(opt=="!=") return d1!=d2;
					else{
						cout<<"wrong!";return false;
					}
				}
				else if(type2=="int(11)"){
					double d1=stof(data1);int d2=stoi(data2);
					if(opt=="<") return d1<d2;
					else if(opt==">") return d1>d2;
					else if(opt=="=") return d1==d2;
					else if(opt==">=") return d1>=d2;
					else if(opt=="<=") return d1<=d2;
					else if(opt=="!=") return d1!=d2;
					else{
						cout<<"wrong!";return false;
					}
				}
			}
			double d1=stof(data1),d2=stof(data2);
			if(opt=="<") return d1<d2;
			else if(opt==">") return d1>d2;
			else if(opt=="=") return d1==d2;
			else if(opt==">=") return d1>=d2;
			else if(opt=="<=") return d1<=d2;
			else if(opt=="!=") return d1!=d2;
			else{
				cout<<"wrong!";return false;
			}
		}
		/*if(type1!="char(1)"){
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
		}*/
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

void clear_qua(string& data){
	while(data.front()=='"'){
		data.erase(data.begin());
	}
	while(data.back()=='"'){
		data.erase(data.begin()+data.size()-1);
	}
}

int find_pos(const vector<string>& t,string need,bool care_big_small){
	for(int i=0;i<t.size();++i){
		if(!care_big_small){
			if(Tolower(t[i]).find(Tolower(need))==0){
				return i;
			}
		}
	}
	return -1;
}
/*vector<string> get_vecstr(vector<string>u,int x,int y){
	vector<string>result;
	for(int q=x;q<=y;++q) result.push_back(u[q]);
	return result;
}*/
string getvalid_string(string t){
	clear_space(t);
	if(t.back()==',') t.erase(t.size()-1);
	return t;
}

vector<string> get_show_columnname(vector<string>& t){
	vector<string>result;
	for(int i=1;i<t.size();++i){
		if(Tolower(t[i])=="from") break;
		result.push_back(getvalid_string(t[i]));
	}
	return result;
}
vector<string> split_string(string s){
	vector<string>result;
	string u="";
	for(int i=0;i<s.size();++i){
		if(s[i]==' ') {
			if(u!="") result.push_back(u);
			u="";
		}
		else if(isopt(s[i])){
			if(u!="") result.push_back(u);
			u="";u.push_back(s[i]);
			if(isopt(s[i+1])){
				u.push_back(s[i+1]);
				result.push_back(u);
				++i;u="";
			}
			else{
				result.push_back(u);u="";
			}
		}
		else{
			u.push_back(s[i]);

		}
		if(i==s.size()-1){
			if(u!="") result.push_back(u);
		}
	}
	return result;
}
bool isopt(char c){
	if(c=='<'||c=='>'||c=='='||c=='!'||c=='+'||c=='-'||c=='*'||c=='/'||c=='%') return true;
	return false;
}

bool iscmp(string x){
	if(x==">"||x=="<"||x=="="||x=="!="||x=="<="||x==">="||Tolower(x)=="like") return true;
	return false;
}

bool iscountopt(string x){
	if(x=="+"||x=="-"|x=="*"||x=="/"||x=="%") return true;
	return false;
}

string putvector_tostring(vector<string>& t,int start,int end){
	string result=t[start];
	for(int i=start+1;i<=end;++i){
		result+=' ';result+=t[i];
	}
	return result;
}

vector<string>clear_tablename(vector<string>& u){
	vector<string>result;
	for(int i=0;i<u.size();++i){
		int t=u[i].find(".");string x;
		if(t!=-1){
			x=u[i].substr(t+1,u[i].size()-t-1);
		}
		else{
			x=u[i];
		}
		result.push_back(x);
	}
	return result;
}

bool check_null(vector<string>& u){
	if(find_pos(u,"null")!=-1) return true;
	return false;
}

void print_vector(vector<string>& u){
	for(int i=0;i<u.size();++i) cout<<u[i]<<endl;
}
