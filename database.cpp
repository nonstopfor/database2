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

Table* Database::operator[] (const int& i){
	if(i>=dvalue.size()) return nullptr;
	else{
		return dvalue[i];
	}
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
		bool NULL_flag=false;
		if(where_multiple_work(tablename,v,condition,NULL_flag)){
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
string Database::gettype(vector<string>t){
	for(int i=0;i<t.size();++i){
		if(iscountopt(t[i])) continue;
		//clear_qua(t[i]);
		if(t[i][0]=='"'||t[i][0]=='\'') return "char(1)";
		int w=t[i].find('.');
		if(w!=-1&&this->find_table(t[i].substr(0,w))) {
			int w=t[i].find('.');
			string tablename=t[i].substr(0,w);
			string columnname=t[i].substr(w+1,t[i].size()-w-1);
			return (*(*this)[tablename])[columnname]->gettype();
			//if(find_column(t[i])) return (*this)[t[i]]->gettype();
		}
		if(t[i].find('-') != string::npos && t[i].find('-')>0) return "date";
		if(t[i].find(':') != string::npos) return "time";
		if(isdigit(t[i][0])) {
			if(t[i].find(".")!=-1) return "double";
			return "int(11)";
		}
		else return "char(1)";
	}
	return "NULL";
	
}
vector<string> Database::getdata(vector<string>& tablename, vector<int>&data,vector<string> t){
	vector<string>result;
	for(int j=0;j<t.size();++j){
		int w=t[j].find('.');
		if(w!=-1&&this->find_table(t[j].substr(0,w))){
			string table=t[j].substr(0,w);
			string columnname=t[j].substr(w+1,t[j].size()-w-1);
			int x=find_pos(tablename,table);
			result.push_back((*(*(*this)[table])[columnname])[data[x]]);
		}
		else{
			clear_qua(t[j]);
			result.push_back(t[j]);
		}
	}
	return result;
}
bool Database::where_multiple_work(vector<string>& tablename, vector<int>&data, string condition,bool& NULL_flag){
	if(NULL_flag)return false;
	if(condition=="*") return true;
	bool ans = true;
    const bool default_ans = false;
    int this_space=0, f=0;
    this_space = condition.find(" OR "); //按照优先级先处理OR
    if (this_space == -1) this_space = condition.find(" or ");
    if (!f && this_space != -1) {
        f = 1;
		where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4),NULL_flag);
        ans = where_multiple_work(tablename,data, condition.substr(0, this_space),NULL_flag) || where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4),NULL_flag);
		if(NULL_flag){
//cout<<"also get a NULL flag!"<<endl;
			return false;
		}
		return ans;
    }
	this_space = condition.find(" XOR ");
	if (this_space == -1) this_space = condition.find(" xor ");
    if (!f && this_space != -1) {
        f = 1;
        ans = (where_multiple_work(tablename,data, condition.substr(0, this_space),NULL_flag)==where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4),NULL_flag) ? false : true);
		if(NULL_flag)return false;
		return ans;
    }
    this_space = condition.find(" AND "); //再处理AND
    if (this_space == -1) this_space = condition.find(" and ");
    if (!f && this_space != -1) {
        f = 1;
		ans = where_multiple_work(tablename,data, condition.substr(0, this_space),NULL_flag) && where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4),NULL_flag);
		if(NULL_flag)return false;
		return ans;
    }
	this_space = condition.find(" NOT "); 
    if (this_space == -1) this_space = condition.find(" not ");
    if (!f && this_space != -1) {
        f = 1;
		ans = where_multiple_work(tablename,data, condition.substr(0, this_space),NULL_flag) && ! (where_multiple_work(tablename, data,condition.substr(this_space+4, condition.length()-this_space-4),NULL_flag));
		if(NULL_flag)return false;
		return ans;
    }
	string str=condition;
	vector<string>sstr=split_string(str);
	int pos_cmp;//比较运算符的位置
	for(pos_cmp=0;pos_cmp<sstr.size();++pos_cmp) if(iscmp(sstr[pos_cmp])) break;
	vector<string>exp1,exp2;string opt=sstr[pos_cmp];
	for(int i=0;i<pos_cmp;++i) exp1.push_back(sstr[i]);	
	for(int i=pos_cmp+1;i<sstr.size();++i) exp2.push_back(sstr[i]);
	string tp1=gettype(exp1),tp2=gettype(exp2);
	if(tp1 == "date" || tp1 == "time") tp2 = tp1;
	if(tp2 == "date" || tp2 == "time") tp1 = tp2;
	vector<string> _data1=getdata(tablename,data,exp1),_data2=getdata(tablename,data,exp2);
	if(check_null(_data1)||check_null(_data2)) return false;
	string data1=CAL_alg(_data1,tp1,NULL_flag),data2=CAL_alg(_data2,tp2,NULL_flag);
	if(NULL_flag)return false;
	ans=compare(data1,data2,tp1,tp2,opt);
	return ans;

	//先获得需要比较的数据
	/*vector<string>t;
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
        if(!c2 && ((*(*(*this)[tablename2])[columnname2]).gettype() == "date" || (*(*(*this)[tablename2])[columnname2]).gettype() == "time")){
            if(t[0][0] == '\'' || t[0][0] == '"'){
                data1=t[0].substr(1,t[0].size()-2);
            } else data1=t[0];
            type1 = (*(*(*this)[tablename2])[columnname2]).gettype();
        } else if(isdigit(t[0][0])){
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
        if(!c1 && ((*(*(*this)[tablename1])[columnname1]).gettype() == "date" || (*(*(*this)[tablename1])[columnname1]).gettype() == "time")){
            if(t[2][0] == '\'' || t[2][0] == '"'){
                data2=t[2].substr(1,t[2].size()-2);
            } else data2=t[2];
            type2 = (*(*(*this)[tablename1])[columnname1]).gettype();
        } else if(isdigit(t[2][0])){
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
	return compare(data1,data2,type1,type2,opt);*/
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
		columnname.push_back(getvalid_string(t[i]));
	}
	auto check=(*this)[tablename]->whereClauses(clause);
	for(int i=0;i<check.size();++i){
		if(check[i]){
			vector<string>temp;
			for(int j=0;j<columnname.size();++j){
				if(columnname[j]!="*"){
					if((*this)[tablename]->find_column(columnname[j]))
					temp.push_back((*(*(*this)[tablename])[columnname[j]])[i]);
					else temp.push_back("NULL");
				}
				else{
					for(int j=0;j<(*this)[tablename]->getsize();++j){
						temp.push_back((*(*(*this)[tablename])[j])[i]);
					}
				}
			}
			temp.push_back(to_string(i));
			result.push_back(temp);
		}
	}
	return result;
}
vector<vector<pair<int,bool>>> Database::join_it(vector<vector<pair<int,bool>>>r,vector<string>tablename,vector<string>need,string join){
	string _table=need[0];
	Table* table=(*this)[_table];
	need.erase(need.begin());need.erase(need.begin());
	vector<vector<pair<int,bool>>> result;
	map<vector<pair<int,bool>>,int>m;
	for(int i=0;i<r.size();++i){
		for(int j=0;j<table->getrowsize();++j){
			r[i].push_back(make_pair(j,true));
			bool NULL_flag=false;
			if(join_ok(tablename,r[i],need,NULL_flag)){
				result.push_back(r[i]);
				m[get_vecstr(r[i],0,r[i].size()-2)]++;
				m[get_vecstr(r[i],r[i].size()-1,r[i].size()-1)]++;
				r[i].pop_back();
			}
			else{
				r[i].pop_back();
			}
		}
		if(join=="left join"){
			if(!m[r[i]]){
				r[i].push_back(make_pair(0,false));
				result.push_back(r[i]);
				r[i].pop_back();
			}
		}
	}
	if(join=="right join"){
		for(int j=0;j<table->getrowsize();++j){
			auto u=make_pair(j,true);
			vector<pair<int,bool>>temp;temp.push_back(u);
			if(!m[temp]){
				vector<pair<int,bool>>q;
				for(int i=0;i<r[0].size();++i){
					auto o=r[0][i];o.second=false;
					q.push_back(o);
				}
				q.push_back(u);
				result.push_back(q);
			}
		}
	}
	return result;
}
bool Database::join_ok(const vector<string>&tablename,const vector<pair<int,bool>>& r,vector<string>need,bool& NULL_flag){
	if(NULL_flag)return false;
	if(find_pos(need,"or")!=-1){
		join_ok(tablename,r,get_vecstr(need,find_pos(need,"or")+1,need.size()-1),NULL_flag);
		bool ans=join_ok(tablename,r,get_vecstr(need,0,find_pos(need,"or")-1),NULL_flag) || join_ok(tablename,r,get_vecstr(need,find_pos(need,"or")+1,need.size()-1),NULL_flag);
		if(NULL_flag)return false;
		return ans;
	}
	if(find_pos(need,"xor")!=-1){
		bool ans= !(join_ok(tablename,r,get_vecstr(need,0,find_pos(need,"xor")-1),NULL_flag) == join_ok(tablename,r,get_vecstr(need,find_pos(need,"xor")+1,need.size()-1),NULL_flag));
		if(NULL_flag)return false;
		return ans;
	}
	if(find_pos(need,"and")!=-1){
		bool ans= join_ok(tablename,r,get_vecstr(need,0,find_pos(need,"and")-1),NULL_flag) && join_ok(tablename,r,get_vecstr(need,find_pos(need,"and")+1,need.size()-1),NULL_flag);
		if(NULL_flag)return false;
		return ans;
	}
	if(find_pos(need,"not")!=-1){
		bool ans=join_ok(tablename,r,get_vecstr(need,0,find_pos(need,"not")-1),NULL_flag) && !join_ok(tablename,r,get_vecstr(need,find_pos(need,"not")+1,need.size()-1),NULL_flag);
		if(NULL_flag)return false;
		return ans;
	}
	string temp=putvector_tostring(need,0,need.size()-1);
	//cout<<temp<<endl;
	vector<string>sstr=split_string(temp);
	//print_vector(sstr);
	int pos_cmp;//比较运算符的位置
	for(pos_cmp=0;pos_cmp<sstr.size();++pos_cmp) if(iscmp(sstr[pos_cmp])) break;
	vector<string>exp1=get_vecstr(sstr,0,pos_cmp-1),exp2=get_vecstr(sstr,pos_cmp+1,sstr.size()-1);string opt=sstr[pos_cmp];
	string tp1=gettype(tablename,exp1,r),tp2=gettype(tablename,exp2,r);
	if(tp1=="NULL"||tp2=="NULL") return false;
	vector<string> _data1=getdata(tablename,exp1,r),_data2=getdata(tablename,exp2,r);
	if(check_null(_data1)||check_null(_data2)) return false;
	string data1=CAL_alg(_data1,tp1,NULL_flag),data2=CAL_alg(_data2,tp2,NULL_flag);
	if(NULL_flag)return false;
	bool ans=compare(data1,data2,tp1,tp2,opt);
	return ans;
}
string Database::gettype(const vector<string>&tablename,const vector<string>&t,const vector<pair<int,bool>>& r){
	for(int i=0;i<t.size();++i){
		if(iscountopt(t[i])) continue;
        if(t[i].find("-") != string::npos && t[i].find("-")>0) return "date";
        if(t[i].find(":") != string::npos) return "time";
		int u=t[i].find(".");
		if(u==-1){
			if(isdigit(t[i][0])) return "int(11)";
			else return "char(1)";
		}
		else{
			string x=t[i].substr(0,u);
			if(find_table(x)){
				string y=t[i].substr(u+1,t[i].size()-u-1);
				int pos_table=find_pos(tablename,x);
				if(r[pos_table].second==false) return "NULL";
				return (*(*this)[x])[y]->gettype();
			}
			else{
				return "double";
			}
		}
		
	}
	return "NULL";
}
vector<string> Database::getdata(const vector<string>&tablename,const vector<string>&t,const vector<pair<int,bool>>& r){
	vector<string>result;
	for(int i=0;i<t.size();++i){
		if(t[i].find(".")!=-1 && this->find_table(t[i].substr(0,t[i].find(".")))){
			int u=t[i].find(".");
			string x=t[i].substr(0,u);
			int pos_table=find_pos(tablename,x);

			string y=t[i].substr(u+1,t[i].size()-u-1);
			result.push_back((*(*(*this)[x])[y])[r[pos_table].first]);
		}
		else{
			string x=t[i];
			clear_qua(x);
			result.push_back(x);
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
