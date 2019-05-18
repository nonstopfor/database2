#ifndef DATABASE_H
#define DATABASE_H
#include <vector>
#include <string>
#include <algorithm>
#include "table.h"
using namespace std;


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
	Table* get_table(const string& a);//ͨ������������Ӧ����ָ��
	void del(const string& a); //ͨ�������ɾ�����
	vector<vector<int>> where_multiple(vector<string>& tablename,string condition);//���ض������Щ�з�������
	bool where_multiple_work(vector<string>& tablename, vector<int>&data, string condition);
	//�����������ж�ĳһ������Ƿ���������    data�б����˸����Ӧ������,����˳����tablename��һ��
	//���Ľ���1.����С�ڵ��ں�ǰ������޿ո�(�ѽ��)
	//2.���ܱȽ϶���Ϊĳ��������Ժͳ���
	void enumerate(int& num_table,int step,vector<int>v,vector<string>& tablename,
	vector<vector<int>>& result,string condition);//��������---����ϵ�ö��
	vector<vector<string>> simple_select(string todo);//select������һ�����ԣ�����todoĩβ�޷ֺţ��޿ո�
	vector<vector<string>> multiple_select(string todo);//select�����ж�/1�����ԣ�����
	//todoĩβ�޷ֺţ��޿ո�
};

#endif
