#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <vector>
#include "database.h"
using namespace std;
string ordername;//����sort֮ǰ����
vector<string>overall_columnname;
map<vector<string>,int>overall_mcount;//����sort֮ǰ����
bool cmp(vector<string> u,vector<string> v){
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
int main() {
    string todo; Database* now = NULL; //now��ʾ��������ʹ�õ����ݿ⣬�����л����� 
    map<string,Database*> database; //�����ݿ���ӳ������Ӧ�����ݿ�ָ�� 
    while(getline(cin,todo)) {
    	if (todo=="") continue; 
    	if (todo[todo.length()-1]==' ') todo.erase(todo.end()-1); //ɾ����ĩ�ո� 
    	if (str_com(todo.substr(0,8).c_str(),"CREATE D")) { //�������ݿ� 
    		string dname = todo.substr(16,todo.length()-17); //��ȡ���ݿ���dname����ͬ 
    		database[dname] = new Database(dname);
		}
		else if (str_com(todo.substr(0,6),"DROP D")) { //ɾ�����ݿ� 
			string dname = todo.substr(14,todo.length()-15);
			if (database.find(dname)==database.end()) { //���۴�ɾ�������ݿⲻ���ڵ���� 
				cout << "Database Not Found!\n";
				continue;
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
		/*else if (str_com(todo.substr(0,7),"SELECT ")) { //��ѯ�����Ϣ 
			int p = todo.find(' ',7);
			int q = todo.find(' ',p+6);
			if (q==-1) q = todo.length()-1;
			string tname = todo.substr(p+6,q-p-6);
			if (!(now->find_table(tname))) {
				cout << "Table Not Found!\n";
				continue;
			}
			string cname = todo.substr(7,p-7);
			if ((!((*now)[tname]->find_column(cname)))&&(cname!="*")) {
				cout << "Column Not Found!\n";
				continue;
			}
			string clause;
			int pp = todo.find(" WHERE ");
			if (pp==-1) pp = todo.find(" where ");
			if (pp==-1) clause = "";
			else clause = todo.substr(pp+7,todo.length()-pp-8);
			if (cname=="*") (*now)[tname]->show_all((*now)[tname]->whereClauses(clause)); //��select��Ϊ * �������show_all��ʾȫ�� 
			else (*now)[tname]->show_one(cname,(*now)[tname]->whereClauses(clause));
		}*/
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
							cout<<"getin";
							for(int i=0;i<ptable->getrowsize();++i){
								cout<<"get";
								for(int j=0;j<ptable->getsize();++j){
									fout<<(*((*ptable)[j]))[i]<<"\t";
									cout<<"write";
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
			else if(Tolower(todo).find("order by")!=-1){//�������
				Table* ptable;
				int i;//i��Ӧfrom���ֵ��±�
				for(i=0;i<t.size();++i){
					if(Tolower(t[i])=="from"){
						ptable=now->get_table(t[i+1]);
						break;
					}
				}
				vector<string>group;
				
				vector<vector<string>>result;
				string countname="*";
				int mode=0;
				map<vector<string>,int>m;//������ֵĴ���
				map<vector<string>,int>mcount;//����countֵ
				if(Tolower(todo).find("count")!=-1){
					mode=1;
					for(int i=0;i<l;++i){
						if(Tolower(t[i]).substr(0,5)=="count"){
							countname=t[i].substr(6,t[i].size()-7);
							break;
						}
					}
					
				}
				int j;//j��ӦGROUP BY�е�BY���ֵ��±�
				for(j=0;j<l;++j){
					if(Tolower(t[j])=="by") break;
				}
				for(int q=j+1;q<l-4;++q){
					group.push_back(t[q].substr(0,t[q].size()-1));
				}
				group.push_back(t[l-4]);
				for(int i=0;i<ptable->getrowsize();++i){
					vector<string>temp;
					for(int j=0;j<group.size();++j){
						temp.push_back((*(*ptable)[group[j]])[i]);
					}					
					if(m[temp]){
						++m[temp];
					}
					else{
						result.push_back(temp);
						++m[temp];
					}
					if(countname=="*") ++mcount[temp];
					else {
						if((*(*ptable)[countname])[i]!="NULL") ++mcount[temp];
					}
				}
				for(int i=0;i<group.size();++i){
					cout<<group[i]<<'\t';
				}
				if(mode){
					string u="COUNT(";
					u+=countname;u+=")";
					cout<<u;
				}
				cout<<endl;
				overall_columnname.clear();
				for(int i=0;i<group.size();++i){
					overall_columnname.push_back(group[i]);
				}
				ordername=t[l-1];
				overall_mcount=mcount;
				sort(result.begin(),result.end(),cmp);
				for(int i=0;i<result.size();++i){
					for(int j=0;j<result[i].size();++j){
						cout<<result[i][j]<<'\t';
					}
					if(mode) cout<<mcount[result[i]];
					cout<<endl;
				}
			}
			else if(Tolower(todo).find("group by")!=-1){//�������
				Table* ptable;
				int i;//i��Ӧfrom���ֵ��±�
				for(i=0;i<t.size();++i){
					if(Tolower(t[i])=="from"){
						ptable=now->get_table(t[i+1]);
						break;
					}
				}
				vector<string>group;
				
				vector<vector<string>>result;
				string countname="*";
				int mode=0;
				map<vector<string>,int>m;//������ֵĴ���
				map<vector<string>,int>mcount;//����countֵ
				if(Tolower(todo).find("count")!=-1){
					mode=1;
					for(int i=0;i<l;++i){
						if(Tolower(t[i]).substr(0,5)=="count"){
							countname=t[i].substr(6,t[i].size()-7);
							break;
						}
					}
					
				}
				int j;//j��ӦBY���ֵ��±�
				for(j=0;j<l;++j){
					if(Tolower(t[j])=="by") break;
				}
				for(int q=j+1;q<l-1;++q){
					group.push_back(t[q].substr(0,t[q].size()-1));
				}
				group.push_back(t[l-1]);
				for(int i=0;i<ptable->getrowsize();++i){
					vector<string>temp;
					for(int j=0;j<group.size();++j){
						temp.push_back((*(*ptable)[group[j]])[i]);
					}					
					if(m[temp]){
						++m[temp];
					}
					else{
						result.push_back(temp);
						++m[temp];
					}
					if(countname=="*") ++mcount[temp];
					else {
						if((*(*ptable)[countname])[i]!="NULL") ++mcount[temp];
					}
				}
				for(int i=0;i<group.size();++i){
					cout<<group[i]<<'\t';
				}
				if(mode){
					string u="COUNT(";
					u+=countname;u+=")";
					cout<<u;
				}
				cout<<endl;
				for(int i=0;i<result.size();++i){
					for(int j=0;j<result[i].size();++j){
						cout<<result[i][j]<<'\t';
					}
					if(mode) cout<<mcount[result[i]];
					cout<<endl;
				}
			}
			else if(Tolower(todo).find("count")!=-1){//ʵ��COUNT()����
				cout<<t[1]<<endl;
				Table* ptable=(*now)[t[l-1]];
				cout<<ptable->count(t[1])<<endl;
			}
			else if(Tolower(t[0])=="select"&&Tolower(t[l-2])=="from"){//��ѯ�����Ϣ
				//����ȥ�˷ֺţ�����Ҫ�ӻ�������Ӧ֮ǰ�Ĵ��룩
				todo+=';';
				int p = todo.find(' ',7);
				int q = todo.find(' ',p+6);
				if (q==-1) q = todo.length()-1;
				string tname = todo.substr(p+6,q-p-6);
				if (!(now->find_table(tname))) {
					cout << "Table Not Found!\n";
					continue;
				}
				string cname = todo.substr(7,p-7);
				if ((!((*now)[tname]->find_column(cname)))&&(cname!="*")) {
					cout << "Column Not Found!\n";
					continue;
				}
				string clause;
				int pp = todo.find(" WHERE ");
				if (pp==-1) pp = todo.find(" where ");
				if (pp==-1) clause = "";
				else clause = todo.substr(pp+7,todo.length()-pp-8);
				if (cname=="*") (*now)[tname]->show_all((*now)[tname]->whereClauses(clause)); //��select��Ϊ * �������show_all��ʾȫ�� 
				else (*now)[tname]->show_one(cname,(*now)[tname]->whereClauses(clause));
			}


		}
		

		/*else {
			cout << "WRONG INPUT!\n"; //��ֹ��������ʹ������� 
			continue;
		}*/
	}
	return 0;
}
