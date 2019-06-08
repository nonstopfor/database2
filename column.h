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

using namespace std;
const int monthd[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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



bool str_com(const string& a,const string& b);
string Tolower(string x);
vector<string> cut(const string& s);
bool compare(string data1,string data2,string type1,string type2,string opt);
void clear_space(string& data);//��ȥdataǰ��Ŀո�
void clear_qua(string& data);//��ȥdataǰ���˫����
int find_pos(const vector<string>& t,string need,bool strict=false,bool care_big_small=false);
template<class T>
vector<T> get_vecstr(vector<T>u,int x,int y){
	vector<T>result;
	for(int i=x;i<=y;++i) result.push_back(u[i]);
	return result;
}
string to_date(string t);
string to_time(string t);
string adddate(string s1, string s2);
string addtime(string s1, string s2);
string getvalid_string(string t);//ȥ����string��ǰ��ո���߶���
vector<string> get_show_columnname(vector<string>& t);//��select...from...����еõ���Ҫչʾ������(����count)
vector<string> split_string(string s);//�������������>,<,=��string���зָͬʱ��ȥ�ո�
bool isopt(char c);
bool iscmp(string x);//�Ƿ��ǱȽϷ�
bool iscountopt(string x);//�Ƿ��������
string putvector_tostring(vector<string>& t,int start,int end);
vector<string>clear_tablename(vector<string>& u);//ȥ���������
bool check_null(vector<string>& u);
void print_vector(vector<string>& u);

int floor_val(string &s);
int ceil_val(string &s);
double cos_val(string &s);
double sin_val(string &s);
double ln(string &s);
    
#endif
