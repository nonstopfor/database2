#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <vector>
#include "database.h"
using namespace std;
string ordername;//调用sort之前更新
vector<string>overall_columnname;
map<vector<string>,int>overall_mcount;//调用sort之前更新
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
    string todo; Database* now = NULL; //now表示现在正在使用的数据库，便于切换操作 
    map<string,Database*> database; //从数据库名映射至相应的数据库指针 
    while(getline(cin,todo)) {
    	if (todo=="") continue; 
    	if (todo[todo.length()-1]==' ') todo.erase(todo.end()-1); //删除行末空格 
    	if (str_com(todo.substr(0,8).c_str(),"CREATE D")) { //创建数据库 
    		string dname = todo.substr(16,todo.length()-17); //截取数据库名dname，下同 
    		database[dname] = new Database(dname);
		}
		else if (str_com(todo.substr(0,6),"DROP D")) { //删除数据库 
			string dname = todo.substr(14,todo.length()-15);
			if (database.find(dname)==database.end()) { //讨论待删除的数据库不存在的情况 
				cout << "Database Not Found!\n";
				continue;
			} 
			delete database[dname]; //释放内存 
			database.erase(dname);
		}
		else if (str_com(todo.substr(0,4),"USE ")) { //利用now指针切换数据库 
			string dname = todo.substr(4,todo.length()-5);
			if (database.find(dname)==database.end()) {
				cout << "Database Not Found!\n";
				continue;
			} 
			now = database[dname];
		}
		else if (str_com(todo.substr(0,6),"SHOW D")) { //打印现有数据库名 
			if (!database.empty()) { //没有数据库则不输出 
				cout << "Database\n";
				for (auto t=database.begin();t!=database.end();t++) {
					cout << (t->second->getname()) << endl; 
				}
			} 
		}
		else if (str_com(todo.substr(0,8),"CREATE T")) { //创建表格 
			int p = todo.find("(",13);
			string tname = todo.substr(13,p-13);  //截取表格名tname，下同 
			int q = todo.find("PRIMARY KEY",p); //寻找主键 
			if (q==-1) q = todo.find("primary key",p);
			int s = todo.find(")",q);
			string prime = todo.substr(q+12,s-q-12); //截取主键名 
			now->create(tname,prime); //调用成员函数建表 
			if (todo[s+1]==')') { //将定义主键的子串移除 
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
				string cname = todo.substr(p,s-p); //截取表头变量名cname，下同 
				int t = todo.find(" ",s+1);
				int w = todo.find(",",s+1);
				if (w==-1) w = todo.find(")",s+1);
				n = true; string type;
				if (t>w||t==-1) { //判断该表头变量是否not null，用布尔值n表示 
					type = todo.substr(s+1,w-s-1);
					(*now)[tname]->create(cname,type,n); //创建表格的一列 
				}
				else {
					n = false;
					type = todo.substr(s+1,t-s-1);
					(*now)[tname]->create(cname,type,n);
				}
				p = todo.find(",",p); //继续建立下一列 
				if (p==-1) break;
				if (todo[p+1]==' ') p += 2;
				else p++;
			}
		}
		else if (str_com(todo.substr(0,6),"DROP T")) { //删除表格 
			string tname = todo.substr(11,todo.length()-12);
			if (!(now->find_table(tname))) { //讨论表格不存在的情况 
				cout << "Table Not Found!\n";
				continue;
			}
			now->del(tname);
		}
		else if (str_com(todo.substr(0,6),"SHOW T")) { //展示现有数据库的表格 
			now->show();
		}
		else if (str_com(todo.substr(0,6),"SHOW c")) { //展示建表信息 
			string tname = todo.substr(18,todo.length()-19);
			if (!(now->find_table(tname))) {
				cout << "Table Not Found!\n";
				continue;
			}
			(*now)[tname]->show_column();
		}
		else if (str_com(todo.substr(0,8),"INSERT I")) { //在表格中插入数据 
			int p = todo.find("(",0);
			string tname = todo.substr(12,p-12);
			if (!(now->find_table(tname))) {
				cout << "Table Not Found!\n";
				continue;
			}
			p++; int q = todo.find("(",p); q++; //用两套指针，分别截取表头变量名cname和其数值value 
			int pt = todo.find(")",p); int qt = todo.find(")",q);
			int pp = todo.find(",",p); int qq = todo.find(",",q);
			string cname,value; string to_check = todo.substr(p,pt-p); //to_check为tname(...)括号中的内容 
			if ((*now)[tname]->null_check(to_check)) {  //检查是否有not null变量没有赋值，有则报错 
				cout << "Wrong Insert! Some Not Null field is not filled!\n";
				continue;
			}
			while ((pp<pt)&&(qq<qt)&&(pp!=-1)&&(qq!=-1)) {
				cname = todo.substr(p,pp-p);
				value = todo.substr(q,qq-q);
				if ((*now)[tname]->find_column(cname)) (*((*now)[tname]))[cname]->insert(value);
				else cout << "Column " << cname << " Is Not Found! Filled As Default!\n"; //讨论所指示的表头变量名不存在的情况，此时它将用缺省值填充 
				if (todo[pp+1]==' ') p=pp+2; else p=pp+1;
				if (todo[qq+1]==' ') q=qq+2; else q=qq+1;
				pp = todo.find(",",p); qq = todo.find(",",q);
			}
			cname = todo.substr(p,pt-p);
			value = todo.substr(q,qt-q);
			if ((*now)[tname]->find_column(cname)) (*((*now)[tname]))[cname]->insert(value);
			else cout << "Column " << cname << " Is Not Found! Filled As Default!\n";
			(*now)[tname]->default_fill(); //对于该次赋值，如果表中有某些变量没有赋值，则调用default_fill函数用缺省值NULL填充 
			(*now)[tname]->sort_prime(); //根据主键给各行排序 
		} 
		else if (str_com(todo.substr(0,7),"DELETE ")) { //删除行 
			int p = todo.find(' ',12);
			if (p==-1) p = todo.length()-1;
			string tname = todo.substr(12,p-12);
			if (!(now->find_table(tname))) {
				cout << "Table Not Found!\n";
				continue;
			}
			string clause; //截取whereclause 
			int pp = todo.find(" WHERE ");
			if (pp==-1) pp = todo.find(" where ");
			if (pp==-1) clause = ""; //若没有whereclause，则设为空，下同 
			else clause = todo.substr(pp+7,todo.length()-pp-8);
			(*now)[tname]->del_row((*now)[tname]->whereClauses(clause)); //调用函数删除行 
		}
		else if (str_com(todo.substr(0,7),"UPDATE ")) { //修改表格信息 
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
		/*else if (str_com(todo.substr(0,7),"SELECT ")) { //查询表格信息 
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
			if (cname=="*") (*now)[tname]->show_all((*now)[tname]->whereClauses(clause)); //若select后为 * ，则调用show_all显示全部 
			else (*now)[tname]->show_one(cname,(*now)[tname]->whereClauses(clause));
		}*/
		else{
			todo.erase(todo.size()-1);//为了适应之前的代码读入时把分号也读入进来
			auto t=cut(todo);int l=t.size();
			if (Tolower(todo).find("outfile")!=-1){//数据导出
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
			else if(Tolower(todo).find("infile")!=-1){//数据导入
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
			else if(Tolower(todo).find("order by")!=-1){//排序语句
				Table* ptable;
				int i;//i对应from出现的下标
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
				map<vector<string>,int>m;//计算出现的次数
				map<vector<string>,int>mcount;//计算count值
				if(Tolower(todo).find("count")!=-1){
					mode=1;
					for(int i=0;i<l;++i){
						if(Tolower(t[i]).substr(0,5)=="count"){
							countname=t[i].substr(6,t[i].size()-7);
							break;
						}
					}
					
				}
				int j;//j对应GROUP BY中的BY出现的下标
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
			else if(Tolower(todo).find("group by")!=-1){//分组语句
				Table* ptable;
				int i;//i对应from出现的下标
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
				map<vector<string>,int>m;//计算出现的次数
				map<vector<string>,int>mcount;//计算count值
				if(Tolower(todo).find("count")!=-1){
					mode=1;
					for(int i=0;i<l;++i){
						if(Tolower(t[i]).substr(0,5)=="count"){
							countname=t[i].substr(6,t[i].size()-7);
							break;
						}
					}
					
				}
				int j;//j对应BY出现的下标
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
			else if(Tolower(todo).find("count")!=-1){//实现COUNT()函数
				cout<<t[1]<<endl;
				Table* ptable=(*now)[t[l-1]];
				cout<<ptable->count(t[1])<<endl;
			}
			else if(Tolower(t[0])=="select"&&Tolower(t[l-2])=="from"){//查询表格信息
				//由于去了分号，这里要加回来（适应之前的代码）
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
				if (cname=="*") (*now)[tname]->show_all((*now)[tname]->whereClauses(clause)); //若select后为 * ，则调用show_all显示全部 
				else (*now)[tname]->show_one(cname,(*now)[tname]->whereClauses(clause));
			}


		}
		

		/*else {
			cout << "WRONG INPUT!\n"; //防止错误输入使程序崩溃 
			continue;
		}*/
	}
	return 0;
}
