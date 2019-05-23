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

Table* Database::operator[] (const string& a) { //����[]�Է����ñ��������ʱ���ָ�� 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) return (*t);
	}
	return NULL;
}

string Database::getname() const {return dname;} //������ݿ����Ľӿ� 

int Database::getsize() {return dvalue.size();} //������ݿ��ڱ��������Ľӿ� 

void Database::create(const string& a,const string& b) { //�ڿ����½�����Ľӿ� 
	dvalue.push_back(new Table(a,b));
	sort(dvalue.begin(),dvalue.end(),com); //�����ֵ������� 
}

void Database::show() { //��ӡ���ݿ�����ı��� 
	if (!dvalue.empty()) { //���ݿ�Ϊ������� 
		cout << "Tables_in_" << dname << endl;
		for (auto t=dvalue.begin();t!=dvalue.end();t++) {
			cout << ((*t)->getname()) << endl;
		}
	}
}

bool Database::find_table(const string& a) { //�����������жϿ����Ƿ���������� 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) return true;
	}
	return false;
	
}

Table* Database::get_table(const string& a){//ͨ�������������Ӧ�����ָ��
	for(auto t=dvalue.begin();t!=dvalue.end();t++){
		if((*t)->getname()==a) return *t;
	}
	//return nullptr;
}

void Database::del(const string& a) { //����������ɾ������ 
	for (auto t=dvalue.begin();t!=dvalue.end();t++) {
		if ((*t)->getname()==a) {
			delete (*t);
			dvalue.erase(t);
			break;
		}
	}
}
void Database::enumerate(int& num_table,int step,vector<int>v,vector<string>& tablename,
	vector<vector<int>>& result,string condition){
	if(step==num_table+1){
		if(where_multiple_work(tablename,v,condition)){
			result.push_back(v);
		}

		return;
	}
	else{
		for(int i=0;i<(*this)[tablename[step-1]]->getrowsize();++i){
			vector<int>u=v;
			u.push_back(i);
			enumerate(num_table,step+1,u,tablename,result,condition);
		}
	}
}
vector<vector<int>> Database::where_multiple(vector<string>& tablename,string condition){
	vector<vector<int>> result;
	int num_table=tablename.size();
	//���ڱ�����Ŀ��ȷ��������forѭ��������ȷ�����ʿ����õݹ�ö��
	vector<int>temp;
	enumerate(num_table,1,temp,tablename,result,condition);
	return result;
}

bool Database::where_multiple_work(vector<string>& tablename, vector<int>&data, string condition){
	if(condition=="*") return true;
	bool ans = true;
    const bool default_ans = false;
    int this_space=0, f=0;
    this_space = condition.find(" OR "); //�������ȼ��ȴ���OR
    if (this_space == -1) this_space = condition.find(" or ");
    if (!f && this_space != -1) {
        f = 1;
        return ans = where_multiple_work(tablename,data, condition.substr(0, this_space)) || where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4));
    }
	this_space = condition.find(" XOR ");
	if (this_space == -1) this_space = condition.find(" xor ");
    if (!f && this_space != -1) {
        f = 1;
        return ans = (where_multiple_work(tablename,data, condition.substr(0, this_space))==where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4)) ? false : true);
    }
    this_space = condition.find(" AND "); //�ٴ���AND
    if (this_space == -1) this_space = condition.find(" and ");
    if (!f && this_space != -1) {
        f = 1;
		return ans = where_multiple_work(tablename,data, condition.substr(0, this_space)) && where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4));
    }
	this_space = condition.find(" NOT "); //�ٴ���AND
    if (this_space == -1) this_space = condition.find(" not ");
    if (!f && this_space != -1) {
        f = 1;
		return ans = where_multiple_work(tablename,data, condition.substr(0, this_space)) && ! (where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4)));
    }
	//�Ȼ����Ҫ�Ƚϵ�����
	vector<string>t;
	string tablename1,tablename2,columnname1,columnname2;
	int x=condition.find('=');
	if(x==-1) x=condition.find('>');
	if(x==-1) x=condition.find('<');
	int lx=x-1,rx=x+1;
	while(condition[lx]==' ') --lx;
	while(condition[rx]==' ') ++rx;
	int ll=0,rr=condition.size()-1;
	while(condition[ll]==' ') ++ll;
	while(condition[rr]==' ') --rr;
	t.push_back(condition.substr(ll,lx+1-ll));
	string u="";u+=condition[x];
	t.push_back(u);
	t.push_back(condition.substr(rx,rr+1-rx));
	//����и�

	int i;
	bool c1=false,c2=false;//��һ�����ݺ͵ڶ��������Ƿ��ǳ���
	for(i=0;i<t[0].size();++i){
		if(t[0][i]=='.'){
			tablename1=t[0].substr(0,i);
			columnname1=t[0].substr(i+1,t[0].size()-i-1);
			break;
		}
	}
	if(i==t[0].size()){
		c1=true;
	}
	for(i=0;i<t[2].size();++i){
		if(t[2][i]=='.'){
			tablename2=t[2].substr(0,i);
			columnname2=t[2].substr(i+1,t[2].size()-i-1);
			break;
		}
	}
	if(i==t[2].size()) {
		c2=true;
	}
	string data1,data2,type1,type2;
	if(!c1){
		for(int i=0;i<tablename.size();++i){
			if(tablename[i]==tablename1){
				data1=(*(*(*this)[tablename1])[columnname1])[data[i]];
				type1=(*(*(*this)[tablename1])[columnname1]).gettype();
				break;
			}
		}
	}
	else{
		if(isdigit(t[0][0])){
			data1=t[0];
			if(t[0].find('.')!=-1){
				type1="int(11)";
			}
			else{
				type1="double";
			}
		}
		else{
			data1=t[0].substr(1,t[0].size()-2);
			type1="char(1)";
		}
	}
	if(!c2){
		for(int i=0;i<tablename.size();++i){
			if(tablename[i]==tablename2){
				data2=(*(*(*this)[tablename2])[columnname2])[data[i]];
				type2=(*(*(*this)[tablename2])[columnname2]).gettype();
				break;
			}
		}
	}
	else{
		if(isdigit(t[2][0])){
			data2=t[2];
			if(t[2].find('.')!=-1){
				type2="int(11)";
			}
			else{
				type2="double";
			}
		}
		else{
			data2=t[2].substr(1,t[2].size()-2);
			type2="char(1)";
		}
	}
	string opt=t[1];
	return compare(data1,data2,type1,type2,opt);
}

vector<vector<string>> Database::simple_select(string todo){
	vector<vector<string>>result;
	todo+=';';//Ϊ����Ӧ֮ǰ��Ĵ���
	int p = todo.find(' ',7);
	int q = todo.find(' ',p+6);
	if (q==-1) q = todo.length()-1;
	string tname = todo.substr(p+6,q-p-6);
	if (!(this->find_table(tname))) {
		cout << "Table Not Found!\n";
		
	}
	string cname = todo.substr(7,p-7);
	if ((!((*this)[tname]->find_column(cname)))&&(cname!="*")) {
		cout << "Column Not Found!\n";
		
	}
	string clause;
	int pp = todo.find(" WHERE ");
	if (pp==-1) pp = todo.find(" where ");
	if (pp==-1) clause = "";
	else clause = todo.substr(pp+7,todo.length()-pp-8);
	if(cname=="*"){
		auto u=(*this)[tname]->whereClauses(clause);
		for(int i=0;i<(*this)[tname]->getrowsize();++i){
			if(u[i]){
				vector<string>temp;
				for(int j=0;j<(*this)[tname]->getsize();++j){
					temp.push_back((*(*(*this)[tname])[j])[i]);
				}
				result.push_back(temp);
			}
			
		}
	}
	else{
		auto u=(*this)[tname]->whereClauses(clause);
		for(int i=0;i<(*this)[tname]->getrowsize();++i){
			if(u[i]){
				vector<string>temp;
				temp.push_back((*(*(*this)[tname])[cname])[i]);
				result.push_back(temp);
			}
			
		}
	}
	return result;


	//if (cname=="*") (*this)[tname]->show_all((*this)[tname]->whereClauses(clause)); //��select��Ϊ * �������show_all��ʾȫ�� 
	//else (*this)[tname]->show_one(cname,(*this)[tname]->whereClauses(clause));
}

vector<vector<string>> Database::multiple_select(string todo){
	vector<vector<string>>result;
	auto t=cut(todo);int l=t.size();
	int pos_where=0;
	for(int i=0;i<l;++i){
		if(Tolower(t[i])=="where"){
			pos_where=i;break;
		}
	}
	string clause="";
	if(pos_where){
		int pp=Tolower(todo).find("where");
		clause=todo.substr(pp+6,todo.size()-pp-6);
	}
	int pos_from=0;
	string tablename="";
	for(int i=0;i<l;++i){
		if(Tolower(t[i])=="from"){
			pos_from=i;
			tablename=t[i+1];
			break;
		}
	}
	vector<string>columnname;
	for(int i=1;i<pos_from;++i){
		if(i!=pos_from-1){
			columnname.push_back(t[i].substr(0,t[i].size()-1));
		}
		else{
			columnname.push_back(t[i]);
		}
	}
	auto check=(*this)[tablename]->whereClauses(clause);
	for(int i=0;i<check.size();++i){
		if(check[i]){
			vector<string>temp;
			if(t[1]!="*"){
				for(int j=0;j<columnname.size();++j){
					temp.push_back((*(*(*this)[tablename])[columnname[j]])[i]);
				}
			}
			else{
				for(int j=0;j<(*this)[tablename]->getsize();++j){
					temp.push_back((*(*(*this)[tablename])[j])[i]);
				}
			}
			result.push_back(temp);
		}
	}
	return result;
}

void Database::keep_data(string filename,fstream& fout){
	fout<<"database: "<<dname<<endl;
	for(int i=0;i<dvalue.size();++i){
		auto& t=*(dvalue[i]);
		fout<<"table: "<<t.getname()<<" "<<t.getprime()<<endl;
		fout<<"column: ";
		for(int i=0;i<t.getsize();++i){
			auto& u=*t[i];
			fout<<u.getname()<<" "<<u.gettype()<<" "<<u.can_null()<<" ";
		}
		fout<<endl;
		for(int j=0;j<t.getrowsize();++j){
			fout<<"data: ";
			for(int i=0;i<t.getsize();++i){
				auto& u=*t[i];
				fout<<u[j]<<" ";
			}
			fout<<endl;
		}
	}
}
