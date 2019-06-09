#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include <algorithm>
#include "table.h"
using namespace std;


class Database {
	string dname; //数据库名 
	vector<Table*> dvalue; //包含数据库所有表格指针的向量
public:
	Database(string a); //初始化列表，字符串a为数据库名
	~Database(); //析构函数，删除dvalue中的指针释放内存
	Table* operator[] (const string& a); //[]的重载，用表格名访问表格指针
	Table* operator[] (const int& i);//[]的重载，用表格下标访问表格指针
	string getname() const; //获得数据库名的接口
	int getsize(); //获得数据库里表格数的接口
	void create(const string& a,const string& b); //在库里创建表格的函数，a表示表格名，b为表格的主键
	void show(); //展示数据库所包含的表格
	bool find_table(const string& a); //传入表格名判断库里是否有该表
	Table* get_table(const string& a);//通过表格名获得相应表格的指针
	void del(const string& a); //通过表格名删除表格
	vector<vector<int>> where_multiple(vector<string>& tablename,string condition);//返回多表中哪些行符合条件
	string gettype(vector<string>s);
	vector<string>getdata(vector<string>& tablename, vector<int>&data,vector<string> t);
	bool where_multiple_work(vector<string>& tablename, vector<int>&data, string condition,bool& NULL_flag);
	//辅助函数，判断某一个组合是否满足条件    data中保存了各表对应的行数,保存顺序与tablename中一致
	//待改进：1.大于小于等于号前后可能无空格(已解决)
	//2.可能比较对象为某个表格属性和常数(已解决)
	
	void enumerate(int& num_table,int step,vector<int>v,vector<string>& tablename,
	vector<vector<int>>& result,string condition);//辅助进行---行组合的枚举
	vector<vector<string>> simple_select(string todo);//select后面有一个属性，单表，todo末尾无分号，无空格
	vector<vector<string>> multiple_select(string todo);//select后面有多/1个属性，单表，返回值是所有符合条件的数据（不包括表头）
	//todo末尾无分号，无空格,属性后面是否带逗号没有关系
	//返回的数据每一行最后是行号
	//todo不含有group或者order
	//count等本来不在表中的数据用NULL填充
	vector<vector<pair<int,bool>>> join_it(vector<vector<pair<int,bool>>>r,vector<string>tablename,vector<string>need,string join);
	bool join_ok(const vector<string>&tablename,const vector<pair<int,bool>>& r,vector<string>need,bool& NULL_flag);
	string gettype(const vector<string>&tablename,const vector<string>&t,const vector<pair<int,bool>>& r);
	vector<string>getdata(const vector<string>&tablename,const vector<string>&t,const vector<pair<int,bool>>& r);
	void keep_data(string filename,fstream& fout);
	
};

#endif
