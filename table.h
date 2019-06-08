#ifndef TABLE_H
#define TABLE_H
#include <string>
#include <vector>
#include "column.h"
using namespace std;


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
	int getrowsize();//��ñ�������Ľӿ�
	int count(string& s);//��������е�count
	int count(int i,string& s);//���ĳһ�е�count
	vector<string> getall_columnname();
	Column* operator[](const int i);//����[]����������ָ������е�ָ��
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
	vector<bool> whereClauses(const string& str); //��һ��wherecluause����Ϊbool����������check[i]Ϊtrue��ʾ���з�����������֮ͬ��,�����str�����ֺ�
	bool whereclauses_work(const int& i, const string& str); //Ϊʵ��whereClauses��������ĸ������������table.cpp
	string gettype(vector<string>& t);//��ñ��ʽ������
	vector<string> getdata(int i,vector<string>& t);//�����ʽ�е��������ֻ���������ֵ��ͬʱȥ��˫����
	vector<vector<string>> groupit(vector<vector<string>>& need_group,vector<string>& group);//���ط����Ľ����������ͷ�����ݣ��������һ��Ϊ�кţ�
	//need_group��һ��ΪҪչʾ���������������Ϊ���ݣ�ÿ���������Ϊ�кţ������count�ȷǱ����ԭ�����У���Ӧ������ֵΪNULL��
	//groupitͬʱ�е���count�ļ���
	//groupΪgroup by֮��ĸ�����
	vector<string> deal_function(const vector<string>& columnname,vector<vector<string>>&t);
	//columnnameΪ��Ҫչʾ���У�t�к���һ�������(������)
	string num_function(const int i,string need,vector<vector<string>>&t);
	//iΪ�����ֺ�����columnname�г��ֵ�λ��,needΪ��Ӧ�����ֺ��������,tΪһ������
	string judge_function(string need);
	//�ж�need��Ӧʲô���ֺ���
	vector<vector<string>> orderit(vector<vector<string>>& need_order,string order);//���ط����Ľ����������ͷ�����ݣ��������һ��Ϊ�кţ�
	//need_group��һ��ΪҪչʾ���������������Ϊ���ݣ�ÿ���������Ϊ�кţ�
	//orderit��Ĭ��count�Ѿ��������
	//orderΪorder by֮���ָ��
	//����ֵ��һ���Ǳ�ͷ��������������ݣ������кţ�
	


	vector<vector<string>> takeout(vector<vector<string>>& need_group,vector<int>& want);//����ȡ���ض���֮��Ľ����ֻ���������кţ��ޱ���
	//need_group��һ��ΪҪչʾ���������������Ϊ���ݣ�ÿ���������Ϊ�кţ�
	//want����Ҫ���ж�Ӧ���±�

	static bool cmp_vector_string(const vector<string>&t,const vector<string>&u);
	bool equal(const vector<string>&t,const vector<string>&u);
	vector<vector<string>> combine(vector<string>columnname,vector<vector<string>> a,vector<vector<string>> b,int mode);
	//mode=1��ʾ��Ҫȥ�أ�mode=0��ʾ����Ҫȥ��
	//�������������ޱ�ͷ����ÿ��������к�
};

struct forsort{
	vector<string>data;
	Table* ptable;
	vector<string>selected;
	vector<string>want;//��Ҫ�Ƚϵ���
	string mode;//�Ƚ�ģʽ����string\int\double���͵ıȽ�
	int row;//��ԭtable�е��к�
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
