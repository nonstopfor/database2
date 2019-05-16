#ifndef COLUMN_H
#define COLUMN_H
#include <vector>
#include <string>
using namespace std;
//函数的具体功能和实现均在cpp文件中 

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

#endif
