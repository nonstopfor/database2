#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include "column.h"
using namespace std;
//函数的具体功能和实现均在cpp文件中 

class Table {
	string tname; //表格名 
	string primary_key; //表格主键名 
	vector<Column*> tvalue; //包含表格所有列指针的向量 
public:
	Table(string a,string b); //构造函数，a为表名，b为主键名
	~Table(); //析构函数，释放tvalue内指针的内存
	string getname() const; //获得表格名的接口
	string getprime () const; //获得表格主键名的接口
	int getsize(); //获得表格列数的接口
	friend class Database; //方便Database类访问Table数据
	Column* operator[] (const string& a); //重载[]，便于以列名访问列的指针
	void create(const string& a,const string& b,bool c); //在表格中添加列，a为列名，b为列变量的类型名，c表示该列元素是否可为NULL
	void show_all(const vector<bool>& check); //展示整个表格数据，向量vector由whereClauses生成，表示哪些列符合条件，下同
	void show_one(const string& cname,const vector<bool>& check); //给出列名展示某些列的符合条件的数据
	void show_column(); //展示制表信息
	bool find_column(const string& a); //给定列名判断一个列是否在表中
	bool null_check(const string& a); //在插入操作中，给定一个列名，在该列为Not Null时，判断它是否被插入，否则报错
	void default_fill(); //在插入操作中，对没有插入数据的列用缺省值NULL填充
	void del_row(const vector<bool>& check); //删除符合条件的行
	void swap_row(int a,int b); //给行数交换某两行（行数从0开始）
	void sort_prime(); //根据主键从大到小给表格的行排序
	void update_row(string cname,string value,const vector<bool>& check); //修改表中数据，cname表示列名，value表示要填入的值
	vector<bool> whereClauses(const string& str); //把一个wherecluause处理为bool向量，其中check[i]为true表示该行符合条件，反之同理
	bool whereclauses_work(const int& i, const string& str); //为实现whereClauses函数定义的辅助函数，详见table.cpp
};


#endif
