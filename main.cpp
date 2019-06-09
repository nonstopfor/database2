#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <vector>
#include "database.h"
using namespace std;
string ordername;//����sort֮ǰ����
vector<string>overall_columnname;
int order_sub;//UNION����������ָ���Ӧ���±�
map<vector<string>,int>overall_mcount;//����sort֮ǰ����
string data_keep_filename="data_keep.txt";
bool cmp(vector<string>& u,vector<string>& v){
	if(Tolower(ordername.substr(0,5))=="count"){
		return overall_mcount[u]<overall_mcount[v];
	}
	else{
		int k;
		for(k=0;k<overall_columnname.size();++k){
			if(overall_columnname[k]==ordername) break;
		}
		return u[k]<v[k];
	}
}
bool cmp2(vector<string>& u,vector<string>& v){//����UNIONʵ����sort��ȥ��
	for(int i=0;i<u.size();++i){
		if(u[i]<v[i]) return true;
		else if(u[i]>v[i]) return false;
	}
	return false;
}
bool cmp3(vector<string>& u,vector<string>& v){//����UNIONʵ���е�ORDER BY���
	return u[order_sub]<v[order_sub];
}

void read_data(map<string,Database*>& database,vector<string>& all_database){
	fstream fin;
	fin.open(data_keep_filename,ios::in);
	if(!fin.is_open()) return;
	string x;
	Database* now_database;
	Table* now_table;
	while(getline(fin,x)){
		auto t=cut(x);
		if(t[0]=="database:"){
			database[t[1]]=new Database(t[1]);
			now_database=database[t[1]];
			all_database.push_back(t[1]);
			
		}
		else if(t[0]=="table:"){
			string tablename=t[1],prime_key=t[2];
			now_database->create(tablename,prime_key);
			now_table=(*now_database)[tablename];
		}
		else if(t[0]=="column:"){
			for(int i=1;i<t.size();i+=3){
				bool q;
				if(t[i+2]=="1") q=true;
				else q=false;
				if(t[i+1][0]=='i') t[i+1]="INT";
				else if(t[i+1][0]=='c') t[i+1]="CHAR";
				now_table->create(t[i],t[i+1],q);
			}
		}
		else if(t[0]=="data:"){
			for(int i=1;i<t.size();++i){
				(*now_table)[i-1]->insert(t[i]);
			}
		}
		/*else if(t[0]=="nowdatabase:"){
			now_database=database[t[1]];
		}*/
	}
	fin.close();
}
void print_result(vector<vector<string>>& result){
	if(result.size()<2) return;
	for(int i=0;i<result[0].size();++i){
		cout<<result[0][i]<<'\t';
	}
	cout<<endl;
	for(int i=1;i<result.size();++i){
		for(int j=0;j<result[i].size()-1;++j){
			cout<<result[i][j]<<'\t';
		}
		cout<<endl;
	}
}
void print_result_norows(vector<vector<string>>& result){
	if(result.size()<2) return;
	for(int i=0;i<result[0].size();++i){
		cout<<result[0][i]<<'\t';
	}
	cout<<endl;
	for(int i=1;i<result.size();++i){
		for(int j=0;j<result[i].size();++j){
			cout<<result[i][j]<<'\t';
		}
		cout<<endl;
	}
}
int main() {
	
    string todo; Database* now = NULL; //now��ʾ��������ʹ�õ����ݿ⣬�����л����� 
    map<string,Database*> database; //�����ݿ���ӳ������Ӧ�����ݿ�ָ�� 
	vector<string>all_database;
	//���ڲ��ԣ��Ȳ��Ӵ浵�ж�������
	//read_data(database,all_database);
    while(getline(cin,todo)) {
    	if (todo=="") continue; 
    	if (todo[todo.length()-1]==' ') todo.erase(todo.end()-1); //ɾ����ĩ�ո� 
    	if (str_com(todo.substr(0,8).c_str(),"CREATE D")) { //�������ݿ� 
    		string dname = todo.substr(16,todo.length()-17); //��ȡ���ݿ���dname����ͬ 
    		database[dname] = new Database(dname);
			all_database.push_back(dname);
		}
		else if (str_com(todo.substr(0,6),"DROP D")) { //ɾ�����ݿ� 
			string dname = todo.substr(14,todo.length()-15);
			if (database.find(dname)==database.end()) { //���۴�ɾ�������ݿⲻ���ڵ���� 
				cout << "Database Not Found!\n";
				continue;
			} 
			int i;
			for(i=0;i<all_database.size();++i){
				if(all_database[i]==dname){
					//all_database.erase(all_database.begin()+i);
					all_database.erase(all_database.begin()+i);
					break;
				}
			}		
			
			delete database[dname]; //�ͷ��ڴ� 
			database.erase(dname);
			

		}
		else if (str_com(todo.substr(0,4),"USE ")) { //����nowָ���л����ݿ� 
			string dname = todo.substr(4,todo.length()-5);
			if (database.find(dname)==database.end()) {
				cout << "Database Not Found!\n";
				continue;
			} 
			now = database[dname];
		}
		else if (str_com(todo.substr(0,6),"SHOW D")) { //��ӡ�������ݿ��� 
			if (!database.empty()) { //û�����ݿ������ 
				cout << "Database\n";
				for (auto t=database.begin();t!=database.end();t++) {
					cout << (t->second->getname()) << endl; 
				}
			} 
		}
		else if (str_com(todo.substr(0,8),"CREATE T")) { //������� 
			int p = todo.find("(",13);
			string tname = todo.substr(13,p-13);  //��ȡ�����tname����ͬ 
			int q = todo.find("PRIMARY KEY",p); //Ѱ������ 
			if (q==-1) q = todo.find("primary key",p);
			int s = todo.find(")",q);
			string prime = todo.substr(q+12,s-q-12); //��ȡ������ 
			now->create(tname,prime); //���ó�Ա�������� 
			if (todo[s+1]==')') { //�������������Ӵ��Ƴ� 
				if (todo[q-1]==' ') q-=2; else q--; s++;
				todo.erase(todo.begin()+q,todo.begin()+s);
			}
			else {
				if (todo[q-1]==' ') q--; s+=2;
				todo.erase(todo.begin()+q,todo.begin()+s);
			}
			p++; bool n;
			while (true) {
				s = todo.find(" ",p);
				string cname = todo.substr(p,s-p); //��ȡ��ͷ������cname����ͬ 
				int t = todo.find(" ",s+1);
				int w = todo.find(",",s+1);
				if (w==-1) w = todo.find(")",s+1);
				n = true; string type;
				if (t>w||t==-1) { //�жϸñ�ͷ�����Ƿ�not null���ò���ֵn��ʾ 
					type = todo.substr(s+1,w-s-1);
					(*now)[tname]->create(cname,type,n); //��������һ�� 
				}
				else {
					n = false;
					type = todo.substr(s+1,t-s-1);
					(*now)[tname]->create(cname,type,n);
				}
				p = todo.find(",",p); //����������һ�� 
				if (p==-1) break;
				if (todo[p+1]==' ') p += 2;
				else p++;
			}
		}
		else if (str_com(todo.substr(0,6),"DROP T")) { //ɾ����� 
			string tname = todo.substr(11,todo.length()-12);
			if (!(now->find_table(tname))) { //���۱�񲻴��ڵ���� 
				cout << "Table Not Found!\n";
				continue;
			}
			now->del(tname);
		}
		else if (str_com(todo.substr(0,6),"SHOW T")) { //չʾ�������ݿ�ı�� 
			now->show();
		}
		else if (str_com(todo.substr(0,6),"SHOW c")) { //չʾ������Ϣ 
			string tname = todo.substr(18,todo.length()-19);
			if (!(now->find_table(tname))) {
				cout << "Table Not Found!\n";
				continue;
			}
			(*now)[tname]->show_column();
		}
		else if (str_com(todo.substr(0,8),"INSERT I")) { //�ڱ���в������� 
			int p = todo.find("(",0);
			string tname = todo.substr(12,p-12);
			if (!(now->find_table(tname))) {
				cout << "Table Not Found!\n";
				continue;
			}
			p++; int q = todo.find("(",p); q++; //������ָ�룬�ֱ��ȡ��ͷ������cname������ֵvalue 
			int pt = todo.find(")",p); int qt = todo.find(")",q);
			int pp = todo.find(",",p); int qq = todo.find(",",q);
			string cname,value; string to_check = todo.substr(p,pt-p); //to_checkΪtname(...)�����е����� 
			if ((*now)[tname]->null_check(to_check)) {  //����Ƿ���not null����û�и�ֵ�����򱨴� 
				cout << "Wrong Insert! Some Not Null field is not filled!\n";
				continue;
			}
			while ((pp<pt)&&(qq<qt)&&(pp!=-1)&&(qq!=-1)) {
				cname = todo.substr(p,pp-p);
				value = todo.substr(q,qq-q);
				if ((*now)[tname]->find_column(cname)) (*((*now)[tname]))[cname]->insert(value);
				else cout << "Column " << cname << " Is Not Found! Filled As Default!\n"; //������ָʾ�ı�ͷ�����������ڵ��������ʱ������ȱʡֵ��� 
				if (todo[pp+1]==' ') p=pp+2; else p=pp+1;
				if (todo[qq+1]==' ') q=qq+2; else q=qq+1;
				pp = todo.find(",",p); qq = todo.find(",",q);
			}
			cname = todo.substr(p,pt-p);
			value = todo.substr(q,qt-q);
			if ((*now)[tname]->find_column(cname)) (*((*now)[tname]))[cname]->insert(value);
			else cout << "Column " << cname << " Is Not Found! Filled As Default!\n";
			(*now)[tname]->default_fill(); //���ڸôθ�ֵ�����������ĳЩ����û�и�ֵ�������default_fill������ȱʡֵNULL��� 
			(*now)[tname]->sort_prime(); //������������������ 
		} 
		else if (str_com(todo.substr(0,7),"DELETE ")) { //ɾ���� 
			int p = todo.find(' ',12);
			if (p==-1) p = todo.length()-1;
			string tname = todo.substr(12,p-12);
			if (!(now->find_table(tname))) {
				cout << "Table Not Found!\n";
				continue;
			}
			string clause; //��ȡwhereclause 
			int pp = todo.find(" WHERE ");
			if (pp==-1) pp = todo.find(" where ");
			if (pp==-1) clause = ""; //��û��whereclause������Ϊ�գ���ͬ 
			else clause = todo.substr(pp+7,todo.length()-pp-8);
			(*now)[tname]->del_row((*now)[tname]->whereClauses(clause)); //���ú���ɾ���� 
		}
		else if (str_com(todo.substr(0,7),"UPDATE ")) { //�޸ı����Ϣ 
			int p = todo.find(' ',7);
			string tname = todo.substr(7,p-7);
			if (!(now->find_table(tname))) {
				cout << "Table Not Found!\n";
				continue;
			}
			int q = todo.find('=',p+5);
			int s = todo.find(' ',p+5);
			if (s==-1) s = todo.length()-1;
			string clause;
			int pp = todo.find(" WHERE ");
			if (pp==-1) pp = todo.find(" where ");
			if (pp==-1) clause = "";
			else clause = todo.substr(pp+7,todo.length()-pp-8);
			string cname = todo.substr(p+5,q-p-5);
			string value = todo.substr(q+1,s-q-1);
			(*now)[tname]->update_row(cname,value,(*now)[tname]->whereClauses(clause));
		}
		else{
			todo.erase(todo.size()-1);//Ϊ����Ӧ֮ǰ�Ĵ������ʱ�ѷֺ�Ҳ�������
			auto t=cut(todo);int l=t.size();
			if (Tolower(todo).find("outfile")!=-1){//���ݵ���
				fstream fout;
				string filename=t[l-3].substr(1,t[l-3].size()-2);
				ifstream check_if_exist(filename);
				if(check_if_exist.is_open()){
					continue;
				}
				fout.open(filename,ios::out);
				if(now->find_table(t[l-1])){
						Table* ptable=now->get_table(t[l-1]);
						if(t[1]=="*"){
							//cout<<"getin";
							for(int i=0;i<ptable->getrowsize();++i){
								//cout<<"get";
								for(int j=0;j<ptable->getsize();++j){
									fout<<(*((*ptable)[j]))[i]<<"\t";
									//cout<<"write";
								}
								fout<<endl;
							}
						}
						else{
							vector<string>columnname;
							for(int i=1;i<=l-6;++i){
								if(i!=l-6){
									columnname.push_back(t[i].erase(t[i].size()-1));
								}
								else{
									columnname.push_back(t[i]);
								}
							}
							for(int i=0;i<ptable->getrowsize();++i){
								for(int j=0;j<columnname.size();++j){
									fout<<(*(*ptable)[columnname[j]])[i]<<"\t";
								}
							}
						}
					}
				fout.close();	
			}
			else if(Tolower(todo).find("infile")!=-1){//���ݵ���
				string filename;
				for(int i=0;i<l;++i){
					if(Tolower(t[i])=="infile"){
						filename=t[i+1].substr(1,t[i+1].size()-2);
						break;
					}
				}
				fstream fin;
				fin.open(filename,ios::in);
				if(t[l-1].back()!=')'){
					string s;
					Table* ptable=now->get_table(t[l-1]);
					while(getline(fin,s)){
						auto u=cut(s);
						for(int i=0;i<u.size();++i){
							(*ptable)[i]->insert(u[i]);
						}
					}
				}
				else{
					string tablename="";
					int i;int j;
					for(i=0;i<l;++i){
						if(Tolower(t[i])=="table"){
							for(j=0;j<t[i+1].size();++j){
								if(t[i+1][j]=='(') break;
								tablename+=t[i+1][j];
							}						
							break;
						}
					}
					Table* ptable=now->get_table(tablename);
					vector<string>columnname;
					columnname.push_back(t[i+1].substr(j+1,t[i+1].size()-j-2));
					for(int u=i+2;u<l;++u){
						columnname.push_back(t[u].substr(0,t[u].size()-1));
					}
					string s;
					while(getline(fin,s)){
						
						auto u=cut(s);
						for(int i=0;i<u.size();++i){
							
							(*ptable)[columnname[i]]->insert(u[i]);
						}
						ptable->default_fill();

					}
					ptable->sort_prime();
				}
				fin.close();
			}
			else if(find_pos(t,"join",true)!=-1){//ʵ��join����

				int pos_from=find_pos(t,"from");
				vector<string>joins,alltable;
				vector<vector<string>>needs;
				vector<string>temp;
				vector<string>nowtables;
				for(int i=pos_from+1;i<l;++i){
					string x=Tolower(t[i]);
					if(x=="inner"||x=="left"||x=="right"){
						string u=x+' ';
						u+="join";++i;
						joins.push_back(u);
						needs.push_back(temp);
						temp.clear();
					}
					else if(x=="join"){
						string u="inner join";
						joins.push_back(u);
						needs.push_back(temp);
						temp.clear();
					}
					else{
						if(temp.empty()) alltable.push_back(t[i]);
						temp.push_back(t[i]);
						if(i==l-1) needs.push_back(temp);
					}
				}
				vector<vector<pair<int,bool>>>r;//boolΪtrue��ʾ��Ҫ��ʾ������ΪNULL
				for(int i=0;i<(*now)[needs[0][0]]->getrowsize();++i){
					vector<pair<int,bool>>temp;
					temp.push_back(make_pair(i,true));
					r.push_back(temp);
				}
				nowtables.push_back(needs[0][0]);
				for(int i=0;i<joins.size();++i){
					nowtables.push_back(needs[i+1][0]);
					r=now->join_it(r,nowtables,needs[i+1],joins[i]);
				}
				vector<string>_columnname=get_show_columnname(t);
				map<string,int>sub_table;
				for(int i=0;i<alltable.size();++i) sub_table[alltable[i]]=i;
				vector<vector<string>>result;result.push_back(clear_tablename(_columnname));
				for(int i=0;i<r.size();++i){
					vector<string>temp;
					for(int j=0;j<_columnname.size();++j){
						int u=_columnname[j].find(".");
						string x=_columnname[j].substr(0,u);
						string y=_columnname[j].substr(u+1,_columnname[j].size()-u-1);
						int o=sub_table[x];
						string str="NULL";
						if(r[i][o].second){
							str=(*(*(*now)[x])[y])[r[i][o].first];
						}
						temp.push_back(str);
					}
					result.push_back(temp);
				}
				print_result_norows(result);
			}
			else if(Tolower(todo).find("union")!=-1){//ʵ��union������
				//��ʵ��UNION�������������������Ҫ�ٸĽ�(��ʵ�����Ӷ�������)
				//֮ǰ��ֻʵ����select֮����һ�����ԣ������ѸĽ�Ϊ�����ж�����ԣ�����
				auto columnname=get_show_columnname(t);
				vector<string>selects,unions;int last_pos=0;
				int pos_order=find_pos(t,"order");
				string ordername="";
				vector<vector<vector<string>>>results;
				
				for(int i=0;i<l;++i){
					if(Tolower(t[i])=="union"){
						selects.push_back(putvector_tostring(t,last_pos,i-1));
						if(Tolower(t[i+1])=="all"){
							unions.push_back("union all");
							last_pos=i+2;++i;
						}
						else{
							unions.push_back("union");
							last_pos=i+1;
						}
					}
					else if(Tolower(t[i])=="order"){
						selects.push_back(putvector_tostring(t,last_pos,i-1));
						ordername=t[i+2];break;
					}
					if(i==l-1){
						selects.push_back(putvector_tostring(t,last_pos,i));
					}
				}
				for(int i=0;i<selects.size();++i){
					results.push_back(now->multiple_select(selects[i]));
				}
				vector<vector<string>>result=results[0];
				int pos_start;//��unions��"union"��һ�γ��ֵ�λ��
				for(pos_start=unions.size()-1;pos_start>=0;pos_start--){
					if(unions[pos_start]=="union") break;
				}
				for(int i=0;i<=pos_start;++i){
					result=(*now)[0]->combine(columnname,result,results[i+1],1);
				}
				for(int i=pos_start+1;i<unions.size();++i){
					result=(*now)[0]->combine(columnname,result,results[i+1],0);
				}
				result.insert(result.begin(),columnname);
				auto final_result=result;
				if(pos_order!=-1){
					final_result=(*now)[0]->orderit(result,ordername);
				}
				print_result(final_result);
			}
			else if(Tolower(todo).find("order by")!=-1){//�������
				Table* ptable;
				int i;//i��Ӧfrom���ֵ��±�
				for(i=0;i<t.size();++i){
					if(Tolower(t[i])=="from"){
						ptable=now->get_table(t[i+1]);
						break;
					}
				}
				int pos_group,pos_order;//��¼order��group��t�е��±�
				pos_group=find_pos(t,"group");
				pos_order=find_pos(t,"order");
				string todo="";
				for(int i=0;i<(pos_group==-1?pos_order:pos_group);++i){
					if(!i) todo+=t[i];
					else{
						todo+=' ';todo+=t[i];
					}
				}
				auto need_group=now->multiple_select(todo);
				auto columnname=get_show_columnname(t);
				need_group.insert(need_group.begin(),columnname);
				vector<vector<string>>need_order;
				if(pos_group!=-1){
					vector<string>group;
					for(int i=pos_group+2;i<pos_order;++i){
						group.push_back(getvalid_string(t[i]));
					}
					need_order=ptable->groupit(need_group,group);
				}
				else{
					need_order=need_group;
				}
				string order=t[pos_order+2];

				auto result=ptable->orderit(need_order,order);
				print_result(result);
			}
			else if(Tolower(todo).find("group by")!=-1){//�������
				Table* ptable=nullptr;
				int i;//i��Ӧfrom���ֵ��±�
				for(i=0;i<t.size();++i){
					if(Tolower(t[i])=="from"){
						ptable=now->get_table(t[i+1]);
						break;
					}
				}
				if(ptable==nullptr) continue;
				int pos_group=find_pos(t,"group");//��¼group��t�е��±�
				string todo="";
				for(int i=0;i<pos_group;++i){
					if(!i) todo+=t[i];
					else{
						todo+=' ';todo+=t[i];
					}
				}
				auto need_group=now->multiple_select(todo);
				auto columnname=get_show_columnname(t);
				need_group.insert(need_group.begin(),columnname);
				vector<string>group;
				for(int i=pos_group+2;i<l;++i){
					group.push_back(getvalid_string(t[i]));
				}
				auto result=ptable->groupit(need_group,group);
				print_result(result);
			}
			else if(Tolower(todo).find("count")!=-1 || Tolower(todo).find("sum")!=-1 || Tolower(todo).find("max")!=-1
                    || Tolower(todo).find("min")!=-1 || Tolower(todo).find("ave")!=-1 || Tolower(todo).find("var_samp")!=-1){//ʵ�־ۺϺ���
				Table* ptable=nullptr;
				int i;//i��Ӧfrom���ֵ��±�
				for(i=0;i<t.size();++i){
					if(Tolower(t[i])=="from"){
						ptable=now->get_table(t[i+1]);
						break;
					}
				}
				if(ptable==nullptr) continue;
                string todo="";
				for(int i=0;i<t.size();++i){
					if(!i) todo+=t[i];
					else{
						todo+=' ';todo+=t[i];
					}
				}
				auto need_group=now->multiple_select(todo);
				auto columnname=get_show_columnname(t);
				need_group.insert(need_group.begin(),columnname);
				vector<string>group;
				auto result=ptable->groupit(need_group,group);
				print_result(result);
			}
			else if(Tolower(t[0])=="select"){//��ѯ�����Ϣ,��������Ͷ��
				
				int pos_from=0,pos_where=0;
				for(int i=0;i<l;++i){
					if(Tolower(t[i])=="from") pos_from=i;
					if(Tolower(t[i])=="where") {
						pos_where=i;break;
					}
				}
				if(pos_where-pos_from>2||(!pos_where&&pos_from<l-2)){//���
					vector<string>tablename;//�洢from����ı���
					int pos_from=0,pos_where=0;
					for(int i=0;i<l;++i){
						if(Tolower(t[i])=="from"){
							pos_from=i;
						}
						if(Tolower(t[i])=="where"){
							pos_where=i;
							break;
						}
					}
					string condition;
					if(pos_where){
						for(int i=pos_from+1;i<pos_where;++i){
							if(i==pos_where-1){
								tablename.push_back(t[i]);
							}
							else{
								tablename.push_back(t[i].substr(0,t[i].size()-1));
							}
						}
						int u=todo.find(" WHERE ");
						if(u==-1) u=todo.find(" where ");
						condition=todo.substr(u+7,todo.size()-u-7);					
					}
					else{
						condition="*";
					}
					vector<string>columnname;
					vector<string>ctablename;//�洢���ʱÿ���ж�Ӧ�ı�������
					map<string,int>table_sub;//�洢���ʱ�ı�����v�е�vector�еĵڼ���
					for(int i=0;i<tablename.size();++i){
						table_sub[tablename[i]]=i;
					}
					for(int i=1;i<pos_from;++i){
						int x=t[i].find('.');
						ctablename.push_back(t[i].substr(0,x));
						if(i==pos_from-1){
							columnname.push_back(t[i].substr(x+1,t[i].size()-x-1));
						}
						else{
							columnname.push_back(t[i].substr(x+1,t[i].size()-x-2));
						}
					}

					vector<vector<int>>v=now->where_multiple(tablename,condition);
					for(int i=0;i<columnname.size();++i){
						cout<<columnname[i]<<'\t';
					}
					cout<<endl;
					for(int i=0;i<v.size();++i){
						for(int j=0;j<columnname.size();++j){
							int x=table_sub[ctablename[j]];
							cout<<(*(*(*now)[ctablename[j]])[columnname[j]])[v[i][x]]<<'\t';
						}
						cout<<endl;
					}
				} 
				else{//����
					
					vector<string>columnname;
					
					if(t[1]!="*"){
						
						for(int i=1;i<pos_from;++i){
							columnname.push_back(getvalid_string(t[i]));
						}
					}
					else{
						
						string tablename=t[pos_from+1];
						for(int i=0;i<(*now)[tablename]->getsize();++i){
							columnname.push_back((*((*now)[tablename]))[i]->getname());
						}
					}
					auto u=now->multiple_select(todo);
					u.insert(u.begin(),columnname);

					print_result(u);
				}
			}
			
			

		}
		fstream fout;
		fout.open(data_keep_filename,ios::out|ios::ate);
		for(int i=0;i<all_database.size();++i){
			database[all_database[i]]->keep_data(data_keep_filename,fout);
		}
		//fout<<"nowdatabase: "<<now->getname()<<endl;
		fout.close();
		/*else {
			cout << "WRONG INPUT!\n"; //��ֹ��������ʹ������� 
			continue;
		}*/
		
	}
	return 0;
}
