#include "database.h"
#include <iostream>
using namespace std;

bool com(Table* t1,Table* t2) { //为对dvalue使用sort而写的比较函数 
	return (t1->getname()) < (t2->getname());
}

Database::Database(string a): dname(a) {}

Database::~Database() { //析构函数释放内存 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)) delete (*t);
	}
}

Table* Database::operator[] (const string& a) { //重载[]以方便用表格名访问表格指针 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) return (*t);
	}
	return NULL;
}

string Database::getname() const {return dname;} //获得数据库名的接口 

int Database::getsize() {return dvalue.size();} //获得数据库内表格数量的接口 

void Database::create(const string& a,const string& b) { //在库里新建表格的接口 
	dvalue.push_back(new Table(a,b));
	sort(dvalue.begin(),dvalue.end(),com); //表格按字典序排序 
}

void Database::show() { //打印数据库包含的表格 
	if (!dvalue.empty()) { //数据库为空则不输出 
		cout << "Tables_in_" << dname << endl;
		for (auto t=dvalue.begin();t!=dvalue.end();t++) {
			cout << ((*t)->getname()) << endl;
		}
	}
}

bool Database::find_table(const string& a) { //给出表格名判断库里是否有这个表格 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) return true;
	}
	return false;
	
}

Table* Database::get_table(const string& a){//通过表格名获得相应表格的指针
	for(auto t=dvalue.begin();t!=dvalue.end();t++){
		if((*t)->getname()==a) return *t;
	}
	//return nullptr;
}

void Database::del(const string& a) { //给出表格名删除表格 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) {
			delete (*t);
			dvalue.erase(t);
			break;
		}
	}
}
