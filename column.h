#ifndef COLUMN_H
#define COLUMN_H
#include <vector>
#include <string>
#include<fstream>
#include <cctype>
#include<algorithm>
#include"math_cal.h"
#include<utility>
#include<map>
#include"base_function.h"
using namespace std;


class Column {
	string cname; //���� 
	string type; //�д洢�ı��������� 
	bool can_be_null; //��ʾ���������Ƿ��Ϊ�� 
	vector<string> cvalue; //���������������ݵ����� 
public:
	Column(string a,string b,bool c); //���캯����aΪ������bΪ�д洢�ı�����������c��ʾ���������Ƿ��ΪNULL
	~Column(); //��������
	string getname() const; //��������Ľӿ�
	string gettype() const; //����д洢�������͵Ľӿ�
	bool can_null() const; //��ø���Ԫ���Ƿ��ΪNULL�Ľӿ�
	int getsize(); //��ø���Ԫ�ظ���������������Ľӿ�
	void insert(const string& a); //�����в���Ԫ�أ�aΪ�������ֵ
	void del(int s); //ɾ������Ԫ�أ�sΪ����
	void update(int s,const string& a); //�޸�����Ԫ�أ�sΪ������aΪĿ��ֵ
	friend class Table; //����Table����Column����
	string& operator[] (int s); //����[]��������Column����+[����]������Ӧλ�õ�Ԫ��
};




    
#endif
