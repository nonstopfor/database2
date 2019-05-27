#include "table.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

Table::Table(string a,string b): tname(a),primary_key(b) {}

Table::~Table() { //析构函数释放内存 
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)) delete (*t);
	}
}

string Table::getname() const {return tname;}  //获取表格名的接口 

string Table::getprime () const {return primary_key;} //获取主键的接口 

int Table::getsize() {return tvalue.size();} //获取表格列数的接口
int Table::getrowsize(){return (*this)[primary_key]->getsize();}

int Table::count(string& s){
	if(Tolower(s)=="count(*)"){
		return getrowsize();
	}
	else{
		string columnname=s.substr(6,s.size()-7);
		int result=0;
		for(int i=0;i<(*this)[columnname]->getsize();++i){
			if((*((*this)[columnname]))[i]!="NULL"){
							++result;
			}
		}
		return result;
	}
}
int Table::count(int i,string& s){
	if(s=="") return 0;
	if(Tolower(s)=="count(*)") return 1;
	else{
		string columnname=s.substr(6,s.size()-7);
		if((*(*this)[columnname])[i]!="NULL"){
			return 1;
		}
		else return 0;
	}
}
Column* Table::operator[](const int i){
	return tvalue[i];
}
Column* Table::operator[] (const string& a) { //重载[]以便于用列名访问到列的指针
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)->getname()==a) return (*t);
	}
	return NULL;
}

void Table::create(const string& a,const string& b,bool c) {//在表格中添加新列，a b c分别表示列名、变量类型和是否可为空 
	tvalue.push_back(new Column(a,b,c));
}

void Table::show_all(const vector<bool>& check) { //展示表格全部信息
	int len = check.size(); int num = 0;
	for (int i=0; i<len; i++) {
		if (check[i]) num++;
	}
	if (num!=0) { //满足whereclauses的行数为0则不输出 
		for (auto t=tvalue.begin();t!=tvalue.end()-1;t++) {
			cout << (*t)->getname() << '\t';
		}
		cout << (*(tvalue.end()-1))->getname() << endl;
		for (int i=0; i<len; i++) {
			if (check[i]) {
				for (auto t=tvalue.begin();t!=tvalue.end()-1;t++) {
					if ((*t)->gettype()!="double")
		        		cout << (*(*t))[i] << '\t';
		        	else
		        		cout << fixed << setprecision(4) << stod((*(*t))[i]) << '\t';
	        	}
	        	if ((*(tvalue.end()-1))->gettype()!="double")
	        		cout << (*(*(tvalue.end()-1)))[i] << endl;
	        	else
	        		cout << fixed << setprecision(4) << stod((*(*(tvalue.end()-1)))[i]) << endl;
			}
		}
	}
}

void Table::show_one(const string& cname,const vector<bool>& check) { //打印表格特定列 
	int len = check.size(); int num = 0;
	for (int i=0; i<len; i++) {
		if (check[i]) num++;
	}
	if (num!=0) { //满足whereclauses的行数为0则不输出  
		cout << (*this)[cname]->getname() << endl;
		for (int i=0; i<len; i++) {
			if (check[i]) {
				if ((*this)[cname]->gettype()!="double")
					cout << (*((*this)[cname]))[i] << endl;
				else
					cout << fixed << setprecision(4) << stod((*((*this)[cname]))[i]) << endl;
			}
		}
	}
}

void Table::show_column() { //输出制表信息
	cout << "Field\tType\tNull\tKey\tDefault\tExtra\n";
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		cout << (*t)->getname() << "\t" << (*t)->gettype() << "\t";
		if ((*t)->can_null()) cout << "YES\t";
		else cout << "NO\t";
		if ((*t)->getname()==primary_key) cout << "PRI\tNULL\t" << endl;
		else cout << "\tNULL\t" << endl;
	}
}

bool Table::find_column(const string& a) { //给出列名判断表格中是否有这一列
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)->getname()==a) return true;
	}
	return false;
}

bool Table::null_check(const string& a) { //给出一个to_check字符串判断是否有not null变量没有被赋值 
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if (!((*t)->can_null()) && (a.find((*t)->getname())==-1)) {
			return true;
		}
	}
	return false;
}

void Table::default_fill() { //给为被赋值的变量用缺省值NULL填充 
	int len = (*this)[primary_key]->getsize();
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)->getsize()<len) ((*t)->insert("NULL"));
	}
}

void Table::del_row(const vector<bool>& check) { //删除满足whereclauses的行 
	int len = check.size(); int p = 0;
	for (int i=0; i<len; i++) {
		if (check[i]) {
			for (auto t=tvalue.begin();t!=tvalue.end();t++) {
				(*t)->del(p);
			}
		}
		else p++;
	}
}

void Table::update_row(string cname,string value,const vector<bool>& check) { //修改满足whereclauses的行 
	int len = check.size();
	for (int i=0; i<len; i++) {
		if (check[i]) {
			(*this)[cname]->update(i,value);
		}
	}
	sort_prime();
}

void Table::swap_row(int a,int b) {//给定两个行号，交换这两行的数据 
	string temp;
	for (int i=0; i<tvalue.size(); i++) {
		temp = (*(tvalue[i]))[a];
		(*(tvalue[i]))[a] = (*(tvalue[i]))[b];
		(*(tvalue[i]))[b] = temp;
	}
}

bool cmp_int(const string& a,const string& b) {
	return stoi(a) > stoi(b);
}

bool cmp_char(const string& a,const string& b) {
	return a > b;
}

bool cmp_double(const string& a,const string& b) {
	return stod(a) > stod(b);
}

void Table::sort_prime() {  //根据主键给各行排序 
	int len = (*this)[primary_key]->getsize(); bool cc;
	string ctype = (*this)[primary_key]->gettype();
	for (int i=0; i<len-1; i++) {
		for (int j=0; j<len-1-i; j++) {
			if (ctype=="int(11)") cc = cmp_int((*((*this)[primary_key]))[j],(*((*this)[primary_key]))[j+1]);
			else if (ctype=="char(1)") cc = cmp_char((*((*this)[primary_key]))[j],(*((*this)[primary_key]))[j+1]);
			else cc = cmp_double((*((*this)[primary_key]))[j],(*((*this)[primary_key]))[j+1]);
			if (cc) swap_row(j,j+1);
		}
	}
}

vector<bool> Table::whereClauses(const string& str) { //whereclauses的判断，返回一个vector，表示那些是true，那些是false
	int len = (*this)[primary_key]->getsize();
	vector<bool> check(len,true);
	if (str=="") return check;
	else {
        int i = 0;
        for (auto checking:check) {
            if (!whereclauses_work(i, str)) checking = false;
            //std::cout << '\n' << tvalue[0]->cvalue[i] << " " << check[i];
            ++i;
        }
        //std::cout << '\n';
		return check;
	}
}

bool Table::whereclauses_work(const int& i, const string& str) { //对单行(第i行)进行判断是否符合whereclause的要求，采用递归来分段处理，层层分割，到仅剩一个比较运算符时再进行比较，最后将所有结果采用逻辑运算符连接得到结果
    bool ans = true;
    const bool default_ans = false;
    int now_space=0, f=0;
    now_space = str.find(" OR "); //按照优先级先处理OR
    if (now_space == -1) now_space = str.find(" or ");
    if (!f && now_space != -1) {
        f = 1;
        ans = whereclauses_work(i, str.substr(0, now_space)) || whereclauses_work(i, str.substr(now_space+4, str.length()-now_space-4));
    }
	now_space = str.find(" XOR ");
    if (now_space == -1) now_space = str.find(" xor ");
    if (!f && now_space != -1) {
        f = 1;
        ans = (whereclauses_work(i, str.substr(0, now_space))== whereclauses_work(i, str.substr(now_space+4, str.length()-now_space-4)) ? false : true);
    }
    now_space = str.find(" AND "); 
    if (now_space == -1) now_space = str.find(" and ");
    if (!f && now_space != -1) {
        f = 1;
        ans = whereclauses_work(i, str.substr(0, now_space)) && whereclauses_work(i, str.substr(now_space+5, str.length()-now_space-5));
    }
	now_space = str.find(" NOT "); 
    if (now_space == -1) now_space = str.find(" not ");
    if (!f && now_space != -1) {
        f = 1;
        ans = whereclauses_work(i, str.substr(0, now_space)) && ! (whereclauses_work(i, str.substr(now_space+5, str.length()-now_space-5)));
    }
    if (!f) { //如果在处理的片段中没有OR或者AND，即没有递归过，仅仅含有>、<、=的判断，则对其进行判断处理；处理思路为找到比较运算符的位置，然后将其前后分割，并转化为可以比较的类型，最后进行比较，并将结果返回
        //这里的修改并未影响功能，只是为了增加对更多操作符的支持，比如like,==等
		
		vector<string>sstr=split_string(str);
		int pos_cmp;//比较运算符的位置
		for(pos_cmp=0;pos_cmp<sstr.size();++pos_cmp) if(iscmp(sstr[pos_cmp])) break;
		vector<string>exp1,exp2;string opt=sstr[pos_cmp];
		for(int i=0;i<pos_cmp;++i) exp1.push_back(sstr[i]);		
		for(int i=pos_cmp+1;i<sstr.size();++i) exp2.push_back(sstr[i]);
		string tp1=gettype(exp1),tp2=gettype(exp2);
		vector<string> _data1=getdata(i,exp1),_data2=getdata(i,exp2);
		string data1=CAL_alg(_data1,tp1),data2=CAL_alg(_data2,tp2);

		/*auto t=cut(str);string opt;
		int x=str.find("=");int y=x+1;opt="=";
		if(x==-1) x=str.find("<"),y=x+1,opt="<";
		if(x==-1) x=str.find(">"),y=x+1,opt=">";
		if(x==-1) x=Tolower(str).find("like"),y=x+4,opt="like";
		string data1=str.substr(0,x);
		string data2=str.substr(y,str.size()-y);
		clear_space(data1);
		clear_space(data2);
		data2=data2.substr(1,data2.size()-2);
		string type1,type2;
		//temporary handle expresseions




		if(data1.find(" ")!=-1){
			type1="int(11)";
			string temp=data1;
			string tp_hold;
			vector<string> hold_strs;
			stringstream IN(data1);
			bool wrong_flag=false;
			while(IN>>tp_hold){
				if(tp_hold=="+"||tp_hold=="-"||tp_hold=="*"||tp_hold=="/"||tp_hold=="%%")hold_strs.push_back(tp_hold);
				else if(isdigit(tp_hold[0])){
					hold_strs.push_back(tp_hold);
				}
				else{
					if((*this)[tp_hold]==NULL){
						wrong_flag=true;
					}else{
						if(type1=="char(1)")wrong_flag=true;
						else{
							if(type1=="double")type1="double";
							tp_hold=(*(*this)[tp_hold])[i];
							hold_strs.push_back(tp_hold);
						}
					}
				}

				if(wrong_flag){
					data1="NULL";
					type1="char(1)";
					break;
				}
			}
			if(data1!="NULL"){
				data1=CAL_alg(hold_strs,type1);
				if(data1=="NULL")type1="char(1)";
			}

		}






		//above handle
		else if((*this)[data1]!=NULL){//是属性
			type1=(*this)[data1]->gettype();
			data1=(*(*this)[data1])[i];
		}
		else {//是常数
			if(isdigit(data1[0])){
				if(data1.find(".")!=-1){
					type1="int(11)";
				}
				else{
					type1="double";
				}
			}
			else{
				type1="char(1)";
			}
		}


		if(data2.find(" ")!=-1){
			type2="int(11)";
			string temp=data2;
			string tp_hold;
			vector<string> hold_strs;
			stringstream IN(data2);
			bool wrong_flag=false;
			while(IN>>tp_hold){
				if(tp_hold=="+"||tp_hold=="-"||tp_hold=="*"||tp_hold=="/"||tp_hold=="%%")hold_strs.push_back(tp_hold);
				else if(isdigit(tp_hold[0])){
					hold_strs.push_back(tp_hold);
				}
				else{
					if((*this)[tp_hold]==NULL){
						wrong_flag=true;
					}else{
						if(type2=="char(1)")wrong_flag=true;
						else{
							if(type2=="double")type2="double";
							tp_hold=(*(*this)[tp_hold])[i];
							hold_strs.push_back(tp_hold);
						}
					}
				}

				if(wrong_flag){
					data2="NULL";
					type2="char(1)";
					break;
				}
			}
			if(data2!="NULL")data2=CAL_alg(hold_strs,type2);

		}


		else if((*this)[data2]!=NULL){//是属性
			type2=(*this)[data2]->gettype();
			data2=(*(*this)[data2])[i];
		}
		else {//是常数
			if(isdigit(data2[0])){
				if(data2.find(".")!=-1){
					type2="int(11)";
				}
				else{
					type2="double";
				}
			}
			else{
				type2="char(1)";
			}
		}*/
		
		ans=compare(data1,data2,tp1,tp2,opt);
		
    }
    return ans;
}
string Table::gettype(vector<string>& t){
	for(int i=0;i<t.size();++i){
		if(iscountopt(t[i])) continue;
		if(find_column(t[i])) return (*this)[t[i]]->gettype();
		if(isdigit(t[i][0])) {
			if(t[i].find(".")!=-1) return "double";
			return "int(11)";
		}
		else return "char(1)";
	}
	return "NULL";
}
vector<string> Table::getdata(int i,vector<string>& t){
	vector<string>result;
	for(int j=0;j<t.size();++j){
		if(this->find_column(t[j])) {
			result.push_back((*(*this)[t[j]])[i]);
		}
		else{
			clear_qua(t[j]);
			result.push_back(t[j]);
		}
	}
	return result;
}
vector<vector<string>> Table::groupit(vector<vector<string>>& need_group,vector<string>& group){
	bool needcount;
	int pos_count=find_pos(need_group[0],"count");
	if(pos_count==-1) needcount=false;
	else needcount=true;
	string count_thing="";
	if(needcount) count_thing=need_group[0][pos_count];
	/*vector<int>want;
	for(int i=0;i<group.size();++i){
		int o=find_pos(need_group[0],group[i]);
		want.push_back(o);
	}
	want.push_back(need_group[1].size()-1);*/
	

	/*auto _takeout=takeout(need_group,want);
	sort(_takeout.begin(),_takeout.end(),cmp_vector_string);*/
	vector<vector<string>>result;
	result.push_back(need_group[0]);
	vector<forsort>u;
	vector<string>selected=need_group[0];
	for(int i=1;i<need_group.size();++i){
		u.push_back(forsort(selected,this,need_group[i],group,"string"));
	}
	sort(u.begin(),u.end());
	int _count=0;
	for(int i=0;i<u.size();++i){
		int j=i;_count=0;
		while(j<u.size()&&u[j]==u[i]) {
			if(needcount) _count+=count(u[j].row,count_thing);
			++j;
		}
		vector<string>temp;
		if(!needcount) temp=u[i].data;
		else{
			for(int q=0;q<pos_count;++q) temp.push_back(u[i].data[q]);
			temp.push_back(to_string(_count));
			for(int q=pos_count;q<u[i].data.size();++q) temp.push_back(u[i].data[q]);
		}
		result.push_back(temp);
		i=j-1;
	}
	/*int _count=0;
	for(int i=0;i<_takeout.size();++i){
		if(!i){
			_count+=count(stoi(_takeout[i].back()),count_thing);
			if(i==_takeout.size()-1){
				int sub=0;//对应到need_group中的下标
				for(sub=1;sub<need_group.size();++sub){
					if(need_group[sub].back()==_takeout[i].back()){
						break;
					}
				}
				if(!needcount) {
					result.push_back(need_group[sub]);
				}
				else{
					vector<string>temp;
					for(int o=0;o<count_pos;++o){
						temp.push_back(need_group[sub][o]);
					}
					temp.push_back(to_string(_count));
					for(int o=count_pos;o<need_group[sub].size();++o){
						temp.push_back(need_group[sub][o]);
					}
					result.push_back(temp);
				}
			}
		}
		else{
			if(equal(_takeout[i],_takeout[i-1])){
				_count+=count(stoi(_takeout[i].back()),count_thing);
				if(i==_takeout.size()-1){
					int sub=0;//对应到need_group中的下标
					for(sub=1;sub<need_group.size();++sub){
						if(need_group[sub].back()==_takeout[i].back()){
							break;
						}
					}
					if(!needcount) {
						result.push_back(need_group[sub]);
					}
					else{
						vector<string>temp;
						for(int o=0;o<count_pos;++o){
							temp.push_back(need_group[sub][o]);
						}
						temp.push_back(to_string(_count));
						for(int o=count_pos;o<need_group[sub].size();++o){
							temp.push_back(need_group[sub][o]);
						}
						result.push_back(temp);
					}
				}
			}
			else{
				int sub=0;//对应到need_group中的下标
				for(sub=1;sub<need_group.size();++sub){
					if(need_group[sub].back()==_takeout[i-1].back()){
						break;
					}
				}
				if(!needcount) {
					result.push_back(need_group[sub]);
				}
				else{
					vector<string>temp;
					for(int o=0;o<count_pos;++o){
						temp.push_back(need_group[sub][o]);
					}
					temp.push_back(to_string(_count));
					for(int o=count_pos;o<need_group[sub].size();++o){
						temp.push_back(need_group[sub][o]);
					}
					result.push_back(temp);
					
				}
				_count=0;
				_count+=count(stoi(_takeout[i].back()),count_thing);
				if(i==_takeout.size()-1){
					int sub=0;//对应到need_group中的下标
					for(sub=1;sub<need_group.size();++sub){
						if(need_group[sub].back()==_takeout[i].back()){
							break;
						}
					}
					if(!needcount) {
						result.push_back(need_group[sub]);
					}
					else{
						vector<string>temp;
						for(int o=0;o<count_pos;++o){
							temp.push_back(need_group[sub][o]);
						}
						temp.push_back(to_string(_count));
						for(int o=count_pos;o<need_group[sub].size();++o){
							temp.push_back(need_group[sub][o]);
						}
						result.push_back(temp);
					}
				}
			}
		}
	}*/
	return result;
}
vector<vector<string>> Table::orderit(vector<vector<string>>& need_order,string order){
	vector<vector<string>>result;
	result.push_back(need_order[0]);
	string cmp_type;
	if(Tolower(order).find("count(")!=-1) cmp_type="int(11)";
	else{
		cmp_type=(*this)[order]->gettype();
	}
	vector<string>selected=need_order[0];
	vector<string>want;want.push_back(order);
	vector<forsort>u;
	if(cmp_type=="char(1)"){
		for(int i=1;i<need_order.size();++i){
			u.push_back(forsort(selected,this,need_order[i],want,"string"));
		}
		sort(u.begin(),u.end());
	}
	else if(cmp_type=="int(11)"){
		for(int i=1;i<need_order.size();++i){
			u.push_back(forsort(selected,this,need_order[i],want,"int"));
		}
		sort(u.begin(),u.end());
	}
	else if(cmp_type=="double"){
		for(int i=1;i<need_order.size();++i){
			u.push_back(forsort(selected,this,need_order[i],want,"double"));
		}
		sort(u.begin(),u.end());
	}

	for(int i=0;i<u.size();++i){
		result.push_back(u[i].data);
	}
	return result;
}
vector<vector<string>> Table::takeout(vector<vector<string>>& need_group,vector<int>& want){
	vector<vector<string>> result;
	for(int i=1;i<need_group.size();++i){
		vector<string>temp;
		for(int j=0;j<want.size();++j){
			temp.push_back(need_group[i][j]);
		}
		result.push_back(temp);
	}
	return result;
}
bool Table::cmp_vector_string(const vector<string>&t,const vector<string>&u){
	for(int i=0;i<min(t.size(),u.size())-1;++i){
		if(t[i]<u[i]) return true;
		if(t[i]>u[i]) return false;
	}
	return false;
}
bool Table::equal(const vector<string>&t,const vector<string>&u){
	return (!cmp_vector_string(t,u))&&(!cmp_vector_string(u,t));
}

vector<vector<string>> Table::combine(vector<string>columnname,vector<vector<string>> a,vector<vector<string>> b,int mode){
	vector<vector<string>>result;
	if(!mode){
		a.insert(a.end(),b.begin(),b.end());
		return a;
	}
	else{
		vector<forsort>u;vector<string>want;
		for(int i=0;i<columnname.size();++i) want.push_back(columnname[i]);
		for(int i=0;i<a.size();++i){
			u.push_back(forsort(columnname,this,a[i],want,"string"));
		}
		for(int i=0;i<b.size();++i){
			u.push_back(forsort(columnname,this,b[i],want,"string"));
		}
		sort(u.begin(),u.end());
		auto new_end=unique(u.begin(),u.end());
		u.erase(new_end,u.end());
		for(int i=0;i<u.size();++i){
			result.push_back(u[i].data);
		}
		return result;
	}
}	
