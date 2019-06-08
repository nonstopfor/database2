#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include "column.h"
using namespace std;


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
	int getrowsize();//获得表格行数的接口
	int count(string& s);//获得所有行的count
	int count(int i,string& s);//获得某一行的count
	vector<string> getall_columnname();
	Column* operator[](const int i);//重载[]，便于以列指标访问列的指针
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
	vector<bool> whereClauses(const string& str); //把一个wherecluause处理为bool向量，其中check[i]为true表示该行符合条件，反之同理,这里的str不带分号
	bool whereclauses_work(const int& i, const string& str); //为实现whereClauses函数定义的辅助函数，详见table.cpp
	string gettype(vector<string>& t);//获得表达式的类型
	vector<string> getdata(int i,vector<string>& t);//将表达式中的属性名字换成真正的值，同时去掉双引号
	vector<vector<string>> groupit(vector<vector<string>>& need_group,vector<string>& group);//返回分组后的结果，包括表头和数据（数据最后一列为行号）
	//need_group第一行为要展示的列名，下面的行为数据（每行数据最后为行号，如果是count等非表格中原来的列，对应的数据值为NULL）
	//groupit同时承担对count的计算
	//group为group by之后的各个列
	vector<string> deal_function(const vector<string>& columnname,vector<vector<string>>&t);
	//columnname为需要展示的列，t中含有一组的数据(若干行)
	string num_function(const int i,string need,vector<vector<string>>&t);
	//i为该数字函数在columnname中出现的位置,need为对应的数字函数的语句,t为一组数据
	string judge_function(string need);
	//判断need对应什么数字函数
	vector<vector<string>> orderit(vector<vector<string>>& need_order,string order);//返回分组后的结果，包括表头和数据（数据最后一列为行号）
	//need_group第一行为要展示的列名，下面的行为数据（每行数据最后为行号）
	//orderit中默认count已经计算完毕
	//order为order by之后的指标
	//返回值第一行是表头，下面的行是数据（带有行号）
	


	vector<vector<string>> takeout(vector<vector<string>>& need_group,vector<int>& want);//返回取完特定列之后的结果，只有数据与行号，无表名
	//need_group第一行为要展示的列名，下面的行为数据（每行数据最后为行号）
	//want是想要的列对应的下标

	static bool cmp_vector_string(const vector<string>&t,const vector<string>&u);
	bool equal(const vector<string>&t,const vector<string>&u);
	vector<vector<string>> combine(vector<string>columnname,vector<vector<string>> a,vector<vector<string>> b,int mode);
	//mode=1表示需要去重，mode=0表示不需要去重
	//传进来的数据无表头，但每行最后有行号
};

struct forsort{
	vector<string>data;
	Table* ptable;
	vector<string>selected;
	vector<string>want;//想要比较的项
	string mode;//比较模式，如string\int\double类型的比较
	int row;//在原table中的行号
	bool operator<(const forsort& u) const{
		for(int i=0;i<want.size();++i){
			if(mode=="string"){
				int q=find_pos(selected,Tolower(want[i]));
				if(q!=-1){
					if(data[q]<u.data[q]) return true;
					else if(data[q]>u.data[q]) return false;
				}
				else{
					int x=stoi(data.back()),y=stoi(u.data.back());
					if((*(*ptable)[want[i]])[x]<(*(*ptable)[want[i]])[y]) return true;
					if((*(*ptable)[want[i]])[x]>(*(*ptable)[want[i]])[y]) return false;
				}
			}
			else if(mode=="int"){
				int q=find_pos(selected,Tolower(want[i]));
				if(q!=-1){
					if(stoi(data[q])<stoi(u.data[q])) return true;
					else if(stoi(data[q])>stoi(u.data[q])) return false;
				}
				else{
					int x=stoi(data.back()),y=stoi(u.data.back());
					if(stoi((*(*ptable)[want[i]])[x])<stoi((*(*ptable)[want[i]])[y])) return true;
					if(stoi((*(*ptable)[want[i]])[x])>stoi((*(*ptable)[want[i]])[y])) return false;
				}
			}
			else if(mode=="double"){
				int q=find_pos(selected,Tolower(want[i]));
				if(q!=-1){
					if(stof(data[q])<stof(u.data[q])) return true;
					else if(stof(data[q])>stof(u.data[q])) return false;
				}
				else{
					int x=stof(data.back()),y=stof(u.data.back());
					if(stof((*(*ptable)[want[i]])[x])<stof((*(*ptable)[want[i]])[y])) return true;
					if(stof((*(*ptable)[want[i]])[x])>stof((*(*ptable)[want[i]])[y])) return false;
				}
			}
		}
		return false;
	}
	
	bool operator==(const forsort& u) const{
		return !((*this)<u)&&(!(u<(*this)));
	}

	forsort(vector<string>&_selected,Table* _ptable,vector<string>&_data,vector<string>& _want,string _mode="string"):
		selected(_selected),ptable(_ptable),data(_data),want(_want),mode(_mode)
	{
		row=stoi(data.back());
	}
};


#endif
