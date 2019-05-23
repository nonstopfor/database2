#include "table.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include "math_cal.h"
using namespace std;

Table::Table(string a,string b): tname(a),primary_key(b) {}

Table::~Table() { //���������ͷ��ڴ� 
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)) delete (*t);
	}
}

string Table::getname() const {return tname;} //��ȡ�������Ľӿ� 

string Table::getprime () const {return primary_key;} //��ȡ�����Ľӿ� 

int Table::getsize() {return tvalue.size();} //��ȡ���������Ľӿ� 
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
Column* Table::operator[] (const string& a) { //����[]�Ա������������ʵ��е�ָ�� 
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)->getname()==a) return (*t);
	}
	return NULL;
}

void Table::create(const string& a,const string& b,bool c) { //�ڱ������������У�a b c�ֱ��ʾ�������������ͺ��Ƿ��Ϊ�� 
	tvalue.push_back(new Column(a,b,c));
}

void Table::show_all(const vector<bool>& check) { //չʾ����ȫ����Ϣ 
	int len = check.size(); int num = 0;
	for (int i=0; i<len; i++) {
		if (check[i]) num++;
	}
	if (num!=0) { //����whereclauses������Ϊ0����� 
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

void Table::show_one(const string& cname,const vector<bool>& check) { //��ӡ�����ض��� 
	int len = check.size(); int num = 0;
	for (int i=0; i<len; i++) {
		if (check[i]) num++;
	}
	if (num!=0) { //����whereclauses������Ϊ0����� 
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

void Table::show_column() { //����Ʊ���Ϣ 
	cout << "Field\tType\tNull\tKey\tDefault\tExtra\n";
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		cout << (*t)->getname() << "\t" << (*t)->gettype() << "\t";
		if ((*t)->can_null()) cout << "YES\t";
		else cout << "NO\t";
		if ((*t)->getname()==primary_key) cout << "PRI\tNULL\t" << endl;
		else cout << "\tNULL\t" << endl;
	}
}

bool Table::find_column(const string& a) { //���������жϱ������Ƿ�����һ�� 
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)->getname()==a) return true;
	}
	return false;
}

bool Table::null_check(const string& a) { //����һ��to_check�ַ����ж��Ƿ���not null����û�б���ֵ 
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if (!((*t)->can_null()) && (a.find((*t)->getname())==-1)) {
			return true;
		}
	}
	return false;
}

void Table::default_fill() { //��Ϊ����ֵ�ı�����ȱʡֵNULL��� 
	int len = (*this)[primary_key]->getsize();
	for (auto t=tvalue.begin();t!=tvalue.end();t++) {
		if ((*t)->getsize()<len) ((*t)->insert("NULL"));
	}
}

void Table::del_row(const vector<bool>& check) { //ɾ������whereclauses���� 
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

void Table::update_row(string cname,string value,const vector<bool>& check) { //�޸�����whereclauses���� 
	int len = check.size();
	for (int i=0; i<len; i++) {
		if (check[i]) {
			(*this)[cname]->update(i,value);
		}
	}
	sort_prime();
}

void Table::swap_row(int a,int b) { //���������кţ����������е����� 
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

void Table::sort_prime() { //������������������ 
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

vector<bool> Table::whereClauses(const string& str) { //whereclauses���жϣ�����һ��vector����ʾ��Щ��true����Щ��false
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

bool Table::whereclauses_work(const int& i, const string& str) { //�Ե���(��i��)�����ж��Ƿ����whereclause��Ҫ�󣬲��õݹ����ֶδ��������ָ����ʣһ���Ƚ������ʱ�ٽ��бȽϣ�������н�������߼���������ӵõ����
    bool ans = true;
    const bool default_ans = false;
    int now_space=0, f=0;
    now_space = str.find(" OR "); //�������ȼ��ȴ���OR
    if (now_space == -1) now_space = str.find(" or ");
    if (!f && now_space != -1) {
        f = 1;
        ans = whereclauses_work(i, str.substr(0, now_space)) || whereclauses_work(i, str.substr(now_space+4, str.length()-now_space-4));
    }
	now_space = str.find(" XOR "); //�������ȼ��ȴ���OR
    if (now_space == -1) now_space = str.find(" xor ");
    if (!f && now_space != -1) {
        f = 1;
        ans = (whereclauses_work(i, str.substr(0, now_space))== whereclauses_work(i, str.substr(now_space+4, str.length()-now_space-4)) ? false : true);
    }
    now_space = str.find(" AND "); //�ٴ���AND
    if (now_space == -1) now_space = str.find(" and ");
    if (!f && now_space != -1) {
        f = 1;
        ans = whereclauses_work(i, str.substr(0, now_space)) && whereclauses_work(i, str.substr(now_space+5, str.length()-now_space-5));
    }
	now_space = str.find(" NOT "); //�ٴ���AND
    if (now_space == -1) now_space = str.find(" not ");
    if (!f && now_space != -1) {
        f = 1;
        ans = whereclauses_work(i, str.substr(0, now_space)) && ! (whereclauses_work(i, str.substr(now_space+5, str.length()-now_space-5)));
    }
    if (!f) { //����ڴ�����Ƭ����û��OR����AND����û�еݹ������������>��<��=���жϣ����������жϴ���������˼·Ϊ�ҵ��Ƚ��������λ�ã�Ȼ����ǰ��ָ��ת��Ϊ���ԱȽϵ����ͣ������бȽϣ������������
        //������޸Ĳ�δӰ�칦�ܣ�ֻ��Ϊ�����ӶԸ����������֧�֣�����like,==��
		
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
			if(data1!="NULL")data1=CAL_alg(hold_strs,type1);

		}






		//above handle
		else if((*this)[data1]!=NULL){//������
			type1=(*this)[data1]->gettype();
			data1=(*(*this)[data1])[i];
		}
		else {//�ǳ���
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


		else if((*this)[data2]!=NULL){//������
			type2=(*this)[data2]->gettype();
			data2=(*(*this)[data2])[i];
		}
		else {//�ǳ���
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
		/*now_space = str.find(">"); //����>
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
        now_space = str.find("<"); //����<
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
        now_space = str.find("="); //����=
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
