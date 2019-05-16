#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include <algorithm>
#include "table.h"
using namespace std;
//�����ľ��幦�ܺ�ʵ�־���cpp�ļ��� 

class Database {
	string dname; //���ݿ��� 
	vector<Table*> dvalue; //�������ݿ����б��ָ�������
public:
	Database(string a); //��ʼ���б��ַ���aΪ���ݿ���
	~Database(); //����������ɾ��dvalue�е�ָ���ͷ��ڴ�
	Table* operator[] (const string& a); //[]�����أ��ñ�������ʱ��ָ��
	string getname() const; //������ݿ����Ľӿ�
	int getsize(); //������ݿ��������Ľӿ�
	void create(const string& a,const string& b); //�ڿ��ﴴ�����ĺ�����a��ʾ�������bΪ��������
	void show(); //չʾ���ݿ��������ı��
	bool find_table(const string& a); //���������жϿ����Ƿ��иñ�
	void del(const string& a); //ͨ�������ɾ�����
};

#endif
