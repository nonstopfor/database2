#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include <algorithm>
#include "table.h"
using namespace std;
//函数的具体功能和实现均在cpp文件中 

class Database {
	string dname; //数据库名 
	vector<Table*> dvalue; //包含数据库所有表格指针的向量
public:
	Database(string a); //初始化列表，字符串a为数据库名
	~Database(); //析构函数，删除dvalue中的指针释放内存
	Table* operator[] (const string& a); //[]的重载，用表格名访问表格指针
	string getname() const; //获得数据库名的接口
	int getsize(); //获得数据库里表格数的接口
	void create(const string& a,const string& b); //在库里创建表格的函数，a表示表格名，b为表格的主键
	void show(); //展示数据库所包含的表格
	bool find_table(const string& a); //传入表格名判断库里是否有该表
	void del(const string& a); //通过表格名删除表格
};

#endif
