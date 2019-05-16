#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include "column.h"
using namespace std;
//�����ľ��幦�ܺ�ʵ�־���cpp�ļ��� 

class Table {
	string tname; //����� 
	string primary_key; //��������� 
	vector<Column*> tvalue; //�������������ָ������� 
public:
	Table(string a,string b); //���캯����aΪ������bΪ������
	~Table(); //�����������ͷ�tvalue��ָ����ڴ�
	string getname() const; //��ñ�����Ľӿ�
	string getprime () const; //��ñ���������Ľӿ�
	int getsize(); //��ñ�������Ľӿ�
	friend class Database; //����Database�����Table����
	Column* operator[] (const string& a); //����[]�����������������е�ָ��
	void create(const string& a,const string& b,bool c); //�ڱ��������У�aΪ������bΪ�б�������������c��ʾ����Ԫ���Ƿ��ΪNULL
	void show_all(const vector<bool>& check); //չʾ����������ݣ�����vector��whereClauses���ɣ���ʾ��Щ�з�����������ͬ
	void show_one(const string& cname,const vector<bool>& check); //��������չʾĳЩ�еķ�������������
	void show_column(); //չʾ�Ʊ���Ϣ
	bool find_column(const string& a); //���������ж�һ�����Ƿ��ڱ���
	bool null_check(const string& a); //�ڲ�������У�����һ���������ڸ���ΪNot Nullʱ���ж����Ƿ񱻲��룬���򱨴�
	void default_fill(); //�ڲ�������У���û�в������ݵ�����ȱʡֵNULL���
	void del_row(const vector<bool>& check); //ɾ��������������
	void swap_row(int a,int b); //����������ĳ���У�������0��ʼ��
	void sort_prime(); //���������Ӵ�С������������
	void update_row(string cname,string value,const vector<bool>& check); //�޸ı������ݣ�cname��ʾ������value��ʾҪ�����ֵ
	vector<bool> whereClauses(const string& str); //��һ��wherecluause����Ϊbool����������check[i]Ϊtrue��ʾ���з�����������֮ͬ��
	bool whereclauses_work(const int& i, const string& str); //Ϊʵ��whereClauses��������ĸ������������table.cpp
};


#endif
