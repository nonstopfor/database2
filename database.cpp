#include "database.h"
#include <iostream>
using namespace std;

bool com(Table* t1,Table* t2) { //Ϊ��dvalueʹ��sort��д�ıȽϺ��� 
	return (t1->getname()) < (t2->getname());
}

Database::Database(string a): dname(a) {}

Database::~Database() { //���������ͷ��ڴ� 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)) delete (*t);
	}
}

Table* Database::operator[] (const string& a) { //����[]�Է����ñ�������ʱ��ָ�� 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) return (*t);
	}
	return NULL;
}

string Database::getname() const {return dname;} //������ݿ����Ľӿ� 

int Database::getsize() {return dvalue.size();} //������ݿ��ڱ�������Ľӿ� 

void Database::create(const string& a,const string& b) { //�ڿ����½����Ľӿ� 
	dvalue.push_back(new Table(a,b));
	sort(dvalue.begin(),dvalue.end(),com); //����ֵ������� 
}

void Database::show() { //��ӡ���ݿ�����ı�� 
	if (!dvalue.empty()) { //���ݿ�Ϊ������� 
		cout << "Tables_in_" << dname << endl;
		for (auto t=dvalue.begin();t!=dvalue.end();t++) {
			cout << ((*t)->getname()) << endl;
		}
	}
}

bool Database::find_table(const string& a) { //����������жϿ����Ƿ��������� 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) return true;
	}
	return false;
	
}

Table* Database::get_table(const string& a){//ͨ������������Ӧ����ָ��
	for(auto t=dvalue.begin();t!=dvalue.end();t++){
		if((*t)->getname()==a) return *t;
	}
	//return nullptr;
}

void Database::del(const string& a) { //���������ɾ����� 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) {
			delete (*t);
			dvalue.erase(t);
			break;
		}
	}
}
