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
	//由于表的数目不确定，导致for循环层数不确定，故考虑用递归枚举
	vector<int>temp;
	enumerate(num_table,1,temp,tablename,result,condition);
	return result;
}

bool Database::where_multiple_work(vector<string>& tablename, vector<int>&data, string condition){
	if(condition=="*") return true;
	bool ans = true;
    const bool default_ans = false;
    int this_space=0, f=0;
    this_space = condition.find(" OR "); //按照优先级先处理OR
    if (this_space == -1) this_space = condition.find(" or ");
    if (!f && this_space != -1) {
        f = 1;
        return ans = where_multiple_work(tablename,data, condition.substr(0, this_space)) || where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4));
    }
    this_space = condition.find(" AND "); //再处理AND
    if (this_space == -1) this_space = condition.find(" and ");
    if (!f && this_space != -1) {
        f = 1;
		return ans = where_multiple_work(tablename,data, condition.substr(0, this_space)) && where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4));
    }
	//先获得需要比较的数据
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
	//完成切割

	int i;
	bool c1=false,c2=false;//第一个数据和第二个数据是否是常数
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
	todo+=';';//为了适应之前组的代码
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


	//if (cname=="*") (*this)[tname]->show_all((*this)[tname]->whereClauses(clause)); //若select后为 * ，则调用show_all显示全部 
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
