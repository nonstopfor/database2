#ifndef COLUMN_H
#define COLUMN_H
#include <vector>
#include <string>
#include<fstream>
#include <cctype>
#include<algorithm>
#include"math_cal.h"

using namespace std;


class Column {
	string cname; //列名 
	string type; //列存储的变量类型名 
	bool can_be_null; //表示该列数据是否可为空 
	vector<string> cvalue; //包含列所有行数据的向量 
public:
	Column(string a,string b,bool c); //构造函数，a为列名，b为列存储的变量类型名，c表示该列数据是否可为NULL
	~Column(); //析构函数
	string getname() const; //获得列名的接口
	string gettype() const; //获得列存储变量类型的接口
	bool can_null() const; //获得该列元素是否可为NULL的接口
	int getsize(); //获得该列元素个数，即表格行数的接口
	void insert(const string& a); //在列中插入元素，a为待插入的值
	void del(int s); //删除列中元素，s为行数
	void update(int s,const string& a); //修改列中元素，s为行数，a为目标值
	friend class Table; //便于Table访问Column数据
	string& operator[] (int s); //重载[]，便于用Column对象+[行数]访问相应位置的元素
};



bool str_com(const string& a,const string& b);
string Tolower(string x);
vector<string> cut(const string& s);
bool compare(string data1,string data2,string type1,string type2,string opt);
void clear_space(string& data);//除去data前后的空格
void clear_qua(string& data);//除去data前后的双引号
int find_pos(const vector<string>& t,string need,bool care_big_small=false);
string getvalid_string(string t);//去除该string的前后空格或者逗号
vector<string> get_show_columnname(vector<string>& t);//从select...from...语句中得到需要展示的列名(包括count)
vector<string> split_string(string s);//根据运算符或者>,<,=对string进行分割，同时除去空格
bool isopt(char c);
bool iscmp(string x);//是否是比较符
bool iscountopt(string x);//是否是运算符
string putvector_tostring(vector<string>& t,int start,int end);
#endif
