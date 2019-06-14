#ifndef BASE_FUNCTION
#define BASE_FUNCTION
#include<iostream>
#include<string>
#include<vector>
using namespace std;
const int monthd[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};//每个月有多少天

bool str_com(const string& a,const string& b);//忽略大小写的字符串比较函数
string Tolower(string x);//将string字符串转为小写，返回转为小写后的结果
vector<string> cut(const string& s);//将string字符串进行切割，过滤空格和制表符等
bool compare(string data1,string data2,string type1,string type2,string opt);//用于where子句的两个数据之间的比较
void clear_space(string& data);//除去data前后的空格
void clear_qua(string& data);//除去data前后的双引号
int find_pos(const vector<string>& t,string need,bool strict=false,bool care_big_small=false);//找一个string字符串在vector<string>中的下标,strict代表是否严格相等
template<class T>
vector<T> get_vecstr(vector<T>u,int x,int y){
	vector<T>result;
	for(int i=x;i<=y;++i) result.push_back(u[i]);
	return result;
}//截取vector<T>的从x到y下标的部分
string to_date(string t);
string to_time(string t);
string adddate(string s1, string s2);
string addtime(string s1, string s2);
string getvalid_string(string t);//去除该string的前后空格或者逗号
vector<string> get_show_columnname(vector<string>& t);//从select...from...语句中得到需要展示的列名(包括count)
vector<string> split_string(string s);//根据运算符或者>,<,=对string进行分割，同时除去空格
bool isopt(char c);//判断一个字符c是否是运算符或者比较符的一部分
bool iscmp(string x);//是否是比较符
bool iscountopt(string x);//是否是运算符
string putvector_tostring(vector<string>& t,int start,int end);//将vector<string>拼接成string
vector<string>clear_tablename(vector<string>& u);//去除表格名字
bool check_null(vector<string>& u);//检查vector<string>中是否有"null"
void print_vector(vector<string>& u);//打印一个vector<string>的内容

#endif