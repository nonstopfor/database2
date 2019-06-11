# 数据库项目说明

## 1.整体架构

三个主要的类：

* `Database`:单个数据库，包含`vector<Table*> dvalue`，存储多个表
* `Table`：单个表，包含`vector<Column*> tvalue`，存储多个列
* `Column`：单个属性，包含`vector<string> cvalue`，存储每一行的数据

`main.cpp`中包含有`Database* now`,指向当前正在使用的数据库，以及`vector<string>all_database`，存储所有的数据库的名字，以及`map<string,Database*> database`,从名字到数据库指针的映射。

## 2.运行逻辑

在`main.cpp`中通过标准输入`cin`读入数据（以分号为分隔符），根据对输入语句的语法分类，调用不同的类中的函数进行相应的处理。

## 3.接口说明

对database.h,table.h,column.h三个类中的接口进行说明，math_cal.h中的函数只是为了计算where后的表达式的结果，此处略去。

`database.h`:

```class Database {
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
	string gettype(vector<string>s);//获得where后面的表达式的类型，包括int(11),char(1),double等
	vector<string>getdata(vector<string>& tablename, vector<int>&data,vector<string> t);//获得where后面的表达式的具体数据,如将属性名换成某一行的具体数据
	bool where_multiple_work(vector<string>& tablename, vector<int>&data, string condition,bool& NULL_flag);
	//辅助函数，判断某一个组合是否满足条件    data中保存了各表对应的行数,保存顺序与tablename中一致
	//待改进：1.大于小于等于号前后可能无空格(已解决)
	//2.可能比较对象为某个表格属性和常数(已解决)
	
	void enumerate(int& num_table,int step,vector<int>v,vector<string>& tablename,
	vector<vector<int>>& result,string condition);//辅助进行---行组合的枚举(在多表操作中)
	vector<vector<string>> simple_select(string todo);//select后面有一个属性，单表，todo末尾无分号，无空格
	vector<vector<string>> multiple_select(string todo);//select后面有多/1个属性，单表，返回值是所有符合条件的数据（不包括表头）
	//todo末尾无分号，无空格,属性后面是否带逗号没有关系
	//返回的数据每一行最后是行号
	//todo不含有group或者order
	//count等本来不在表中的数据用NULL填充
	vector<vector<pair<int,bool>>> join_it(vector<vector<pair<int,bool>>>r,vector<string>tablename,vector<string>need,string join);
	//bool为true表示需要显示，否则为NULL, int代表第几行
	bool join_ok(const vector<string>&tablename,const vector<pair<int,bool>>& r,vector<string>need,bool& NULL_flag);
	//判断on后面的条件对于某个行的组合是否满足
	string gettype(const vector<string>&tablename,const vector<string>&t,const vector<pair<int,bool>>& r);//获得on后面的表达式的类型，包括int(11),char(1),double等
	vector<string>getdata(const vector<string>&tablename,const vector<string>&t,const vector<pair<int,bool>>& r);//获得on后面的表达式的具体数据,如将属性名换成某一行的具体数据
	void keep_data(string filename,fstream& fout);//向存储数据的文件中写入数据(实现数据库存档功能)
	
};
```

​    `column.h`:(注：便于引用，在column.h中的Column类外声明了一些基本的功能函数)

``` 
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
    
```

`table.h`:(注：为了在group和order中进行排序操作，在这里定义了forsort结构体专门用于排序)

```
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
	vector<string> getall_columnname();//获得所有的列名
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
	bool whereclauses_work(const int& i, const string& str, bool& NULL_flag); //为实现whereClauses函数定义的辅助函数，详见table.cpp
	string gettype(vector<string> t);//获得表达式的类型
	vector<string> getdata(int i,vector<string> t);//将表达式中的属性名字换成真正的值，同时去掉双引号
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

	static bool cmp_vector_string(const vector<string>&t,const vector<string>&u);//比较两个vector<string>的大小
	bool equal(const vector<string>&t,const vector<string>&u);//判断两个vector<string>是否相等
	vector<vector<string>> combine(vector<string>columnname,vector<vector<string>> a,vector<vector<string>> b,int mode);
	//mode=1表示需要去重，mode=0表示不需要去重
	//传进来的数据无表头，但每行最后有行号
};

struct forsort{
	vector<string>data;//数据
	Table* ptable;//指向对应的表
	vector<string>selected;//需要展示的列名
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
```







