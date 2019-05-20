#include "table.h"
#include <iostream>
#include <iomanip>
using namespace std;

Table::Table(string a,string b): tname(a),primary_key(b) {}

Table::~Table() { //析构函数释放内存 
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)) delete (*t);
	}
}

string Table::getname() const {return tname;} //获取表格名的接口 

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

Column* Table::operator[](const int i){
	return tvalue[i];
}
Column* Table::operator[] (const string& a) { //重载[]以便于用列名访问到列的指针 
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)->getname()==a) return (*t);
	}
	return NULL;
}

void Table::create(const string& a,const string& b,bool c) { //在表格中添加新列，a b c分别表示列名、变量类型和是否可为空 
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

void Table::swap_row(int a,int b) { //给定两个行号，交换这两行的数据 
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

void Table::sort_prime() { //根据主键给各行排序 
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
    now_space = str.find(" AND "); //再处理AND
    if (now_space == -1) now_space = str.find(" and ");
    if (!f && now_space != -1) {
        f = 1;
        ans = whereclauses_work(i, str.substr(0, now_space)) && whereclauses_work(i, str.substr(now_space+5, str.length()-now_space-5));
    }
    if (!f) { //如果在处理的片段中没有OR或者AND，即没有递归过，仅仅含有>、<、=的判断，则对其进行判断处理；处理思路为找到比较运算符的位置，然后将其前后分割，并转化为可以比较的类型，最后进行比较，并将结果返回
        //这里的修改并未影响功能，只是为了增加对更多操作符的支持，比如like,==等
		
		auto t=cut(str);string opt;
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
		if((*this)[data1]!=NULL){//是属性
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
		if((*this)[data2]!=NULL){//是属性
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
		}
		
		ans=compare(data1,data2,type1,type2,opt);
		/*now_space = str.find(">"); //处理>
        if (now_space != -1) {
            string l_string = str.substr(0, now_space);
            string r_string = str.substr(now_space+1, str.length()-now_space-1);
            string str_type = ((*this)[l_string])->gettype();
            if (str_type == "int(11)") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if ((l_string != "NULL") && (r_string != "NULL")) ans = stoi(l_string) > stoi(r_string);
                else ans = default_ans;
            }
            if (str_type == "char(1)") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if (l_string[0] == '\"') l_string = l_string[1];
                if (r_string[0] == '\"') r_string = r_string[1];
                if ((l_string != "NULL") && (r_string != "NULL")) ans = l_string > r_string;
                else ans = default_ans;
            }
            if (str_type == "double") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if ((l_string != "NULL") && (r_string != "NULL")) ans = stod(l_string) > stod(r_string);
                else ans = default_ans;
            }
        }
        now_space = str.find("<"); //处理<
        if (now_space != -1) {
            string l_string = str.substr(0, now_space);
            string r_string = str.substr(now_space+1, str.length()-now_space-1);
            string str_type = ((*this)[l_string])->gettype();
            if (str_type == "int(11)") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if ((l_string != "NULL") && (r_string != "NULL")) ans = stoi(l_string) < stoi(r_string);
                else ans = default_ans;
            }
            if (str_type == "char(1)") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if (l_string[0] == '\"') l_string = l_string[1];
                if (r_string[0] == '\"') r_string = r_string[1];
                if ((l_string != "NULL") && (r_string != "NULL")) ans = l_string < r_string;
                else ans = default_ans;
            }
            if (str_type == "double") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if ((l_string != "NULL") && (r_string != "NULL")) ans = stod(l_string) < stod(r_string);
                else ans = default_ans;
            }
        }
        now_space = str.find("="); //处理=
        if (now_space != -1) {
            string l_string = str.substr(0, now_space);
            string r_string = str.substr(now_space+1, str.length()-now_space-1);
            string str_type = ((*this)[l_string])->gettype();
            if (str_type == "int(11)") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if ((l_string != "NULL") && (r_string != "NULL")) ans = stoi(l_string) == stoi(r_string);
                else ans = default_ans;
            }
            if (str_type == "char(1)") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if (l_string[0] == '\"') l_string = l_string[1];
                if (r_string[0] == '\"') r_string = r_string[1];
                if ((l_string != "NULL") && (r_string != "NULL")) ans = l_string == r_string;
                else ans = default_ans;
            }
            if (str_type == "double") {
                if ((*this)[l_string] != NULL) l_string = (((*this)[l_string])->cvalue[i]);
                else r_string = (((*this)[r_string])->cvalue[i]);
                if ((l_string != "NULL") && (r_string != "NULL")) ans = stod(l_string) == stod(r_string);
                else ans = default_ans;
            }
        }*/
    }
    return ans;
}
