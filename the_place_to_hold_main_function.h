#ifndef MIAMIAFUNC
#define MIAMIAFUNC
#include <string>
#include <sstream>
#include <stack>
#include <iostream>
#include <map>
#include <cctype>
#include <vector>
#include "database.h"

string ordername;//调用sort之前更新
vector<string>overall_columnname;
int order_sub;//UNION操作中排序指标对应的下标
map<vector<string>,int>overall_mcount;//调用sort之前更新
string data_keep_filename="data_keep.txt";
string todo; Database* now = NULL; //now表示现在正在使用的数据库，便于切换操作 
map<string,Database*> database; //从数据库名映射至相应的数据库指针 
vector<string>all_database;
stringstream result_out;
//便于测试，先不从存档中读入数据
//read_data(database,all_database);
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
bool cmp2(vector<string>& u,vector<string>& v){//用于UNION实现中sort，去重
	for(int i=0;i<u.size();++i){
		if(u[i]<v[i]) return true;
		else if(u[i]>v[i]) return false;
	}
	return false;
}
bool cmp3(vector<string>& u,vector<string>& v){//用于UNION实现中的ORDER BY语句
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
		result_out<<result[0][i]<<'\t';
	}
	result_out<<endl;
	for(int i=1;i<result.size();++i){
		for(int j=0;j<result[i].size()-1;++j){
			result_out<<result[i][j]<<'\t';
		}
		result_out<<endl;
	}
}
void print_result_norows(vector<vector<string>>& result){
	if(result.size()<2) return;
	for(int i=0;i<result[0].size();++i){
		result_out<<result[0][i]<<'\t';
	}
	result_out<<endl;
	for(int i=1;i<result.size();++i){
		for(int j=0;j<result[i].size();++j){
			result_out<<result[i][j]<<'\t';
		}
		result_out<<endl;
	}
}
//this function servers as the main function
//it will receive a string as input
//and return the reply to be printed
//the following is an example, which should be replaced by the real function
string miamia(string A){
<<<<<<< HEAD
	result_out.str("");
    string todo=A;
    //stringstream ss(todo);
    string result="";
    if (todo=="") return "OK";
=======
	result_out.clear();
    string todo=A;
    //stringstream ss(todo);
    string result="";
    if (todo=="") return "";
>>>>>>> 0b87a9b137417d7f7ca828a4b8bf631b20e29dee
    if (todo[todo.length()-1]==' ') todo.erase(todo.end()-1);
    todo+=';';
    
    /*stack<string> holdi;
    string result="";

    while(IN>>result)holdi.push(result);
    result="";
    while(holdi.size()>0){
        result+=holdi.top();
        result+=" ";
        holdi.pop();
    }*/
	int loop_control=1;
	while(loop_control--){
        if (str_com(todo.substr(0,8).c_str(),"CREATE D")) { //创建数据库 
    		string dname = todo.substr(16,todo.length()-17); //截取数据库名dname，下同 
    		database[dname] = new Database(dname);
			all_database.push_back(dname);
		}
		else if (str_com(todo.substr(0,6),"DROP D")) { //删除数据库 
			string dname = todo.substr(14,todo.length()-15);
			if (database.find(dname)==database.end()) { //讨论待删除的数据库不存在的情况 
				result_out << "Database Not Found!\n";
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
			
			delete database[dname]; //释放内存 
			database.erase(dname);
			

		}
		else if (str_com(todo.substr(0,4),"USE ")) { //利用now指针切换数据库 
			string dname = todo.substr(4,todo.length()-5);
			if (database.find(dname)==database.end()) {
				result_out << "Database Not Found!\n";
				continue;
			} 
			now = database[dname];
		}
		else if (str_com(todo.substr(0,6),"SHOW D")) { //打印现有数据库名 
			if (!database.empty()) { //没有数据库则不输出 
				result_out << "Database\n";
				for (auto t=database.begin();t!=database.end();t++) {
					result_out << (t->second->getname()) << endl; 
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
				result_out << "Table Not Found!\n";
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
				result_out << "Table Not Found!\n";
				continue;
			}
			(*now)[tname]->show_column();
		}
		else if (str_com(todo.substr(0,8),"INSERT I")) { //在表格中插入数据 
			int p = todo.find("(",0);
			string tname = todo.substr(12,p-12);
			if (!(now->find_table(tname))) {
				result_out << "Table Not Found!\n";
				continue;
			}
			p++; int q = todo.find("(",p); q++; //用两套指针，分别截取表头变量名cname和其数值value 
			int pt = todo.find(")",p); int qt = todo.find(")",q);
			int pp = todo.find(",",p); int qq = todo.find(",",q);
			string cname,value; string to_check = todo.substr(p,pt-p); //to_check为tname(...)括号中的内容 
			if ((*now)[tname]->null_check(to_check)) {  //检查是否有not null变量没有赋值，有则报错 
				result_out << "Wrong Insert! Some Not Null field is not filled!\n";
				continue;
			}
			while ((pp<pt)&&(qq<qt)&&(pp!=-1)&&(qq!=-1)) {
				cname = todo.substr(p,pp-p);
				value = todo.substr(q,qq-q);
				if ((*now)[tname]->find_column(cname)) (*((*now)[tname]))[cname]->insert(value);
				else result_out << "Column " << cname << " Is Not Found! Filled As Default!\n"; //讨论所指示的表头变量名不存在的情况，此时它将用缺省值填充 
				if (todo[pp+1]==' ') p=pp+2; else p=pp+1;
				if (todo[qq+1]==' ') q=qq+2; else q=qq+1;
				pp = todo.find(",",p); qq = todo.find(",",q);
			}
			cname = todo.substr(p,pt-p);
			value = todo.substr(q,qt-q);
			if ((*now)[tname]->find_column(cname)) (*((*now)[tname]))[cname]->insert(value);
			else result_out << "Column " << cname << " Is Not Found! Filled As Default!\n";
			(*now)[tname]->default_fill(); //对于该次赋值，如果表中有某些变量没有赋值，则调用default_fill函数用缺省值NULL填充 
			(*now)[tname]->sort_prime(); //根据主键给各行排序 
		} 
		else if (str_com(todo.substr(0,7),"DELETE ")) { //删除行 
			int p = todo.find(' ',12);
			if (p==-1) p = todo.length()-1;
			string tname = todo.substr(12,p-12);
			if (!(now->find_table(tname))) {
				result_out << "Table Not Found!\n";
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
				result_out << "Table Not Found!\n";
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
				result_out << "Table Not Found!\n";
				continue;
			}
			string cname = todo.substr(7,p-7);
			if ((!((*now)[tname]->find_column(cname)))&&(cname!="*")) {
				result_out << "Column Not Found!\n";
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
							result_out<<"getin";
							for(int i=0;i<ptable->getrowsize();++i){
								result_out<<"get";
								for(int j=0;j<ptable->getsize();++j){
									fout<<(*((*ptable)[j]))[i]<<"\t";
									result_out<<"write";
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
			else if(Tolower(todo).find("join")!=-1){//实现join功能

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
				vector<vector<pair<int,bool>>>r;//bool为true表示需要显示，否则为NULL
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



				/*int pos_join=find_pos(t,"join");
				string mode=Tolower(t[pos_join-1]);//left\right\inner
				int pos_on=find_pos(t,"on");
				int pos_from=find_pos(t,"from");
<<<<<<< HEAD



				/*vector<string>columnname,ltable,rtable,alltable;
				for(int i=pos_from;i<pos_join-1;++i) ltable.push_back(getvalid_string(t[i]));
				for(int i=pos_join+1;i<pos_on;++i) rtable.push_back(getvalid_string(t[i]));
				for(int i=0;i<ltable.size();++i) alltable.push_back(ltable[i]);
				for(int i=0;i<rtable.size();++i) alltable.push_back(rtable[i]);
				if(t[1]=="*"){
					for(int i=0;i<alltable.size();++i){
						vector<string>temp=(*now)[alltable[i]]->getall_columnname();
						for(int j=0;j<temp.size();++j) {
							string u=alltable[i]+".";u+=temp[j];
							columnname.push_back(u);
						}
					}
				}
				else{
					for(int i=1;i<pos_from;++i) columnname.push_back(t[i]);
				}
				/*string condition="";
				if(pos_on!=-1) condition=putvector_tostring(t,pos_on+1,l-1);
				map<string,int>sub_table;
				for(int i=0;i<alltable.size();++i) sub_table[alltable[i]]=i;
				vector<vector<int>>rows=now->where_multiple(alltable,condition);
				vector<vector<string>>result;
				result.push_back(clear_tablename(columnname));
				for(int i=0;i<rows.size();++i){
					vector<string>temp;
					for(int j=0;j<columnname.size();++j){
						int t=columnname[j].find(".");
						string _tablename=columnname[j].substr(0,t);
						string _columnname=columnname[j].substr(t+1,columnname[j].size()-t-1);
						temp.push_back((*(*(*now)[_tablename])[_columnname])[rows[i][sub_table[_tablename]]]);
					}
					result.push_back(temp);
				}
				if(mode=="left"){

				}*/


			}
			else if(Tolower(todo).find("union")!=-1){//实现union操作符
				//先实现UNION连接两个结果集，有需要再改进(已实现连接多个结果集)
				//之前组只实现了select之后有一个属性，这里已改进为可以有多个属性（单表）
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
				int pos_start;//在unions中"union"第一次出现的位置
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
				
				/*int pos_union=Tolower(todo).find("union");
				int pos_select2=Tolower(todo).find("select",pos_union);
				int pos_order=Tolower(todo).find("order");

				if(pos_order==-1){
					pos_order=todo.size()+1;
				}
				string todo1=todo.substr(0,pos_union-1);
				vector<string>columnname;
				auto cut_todo1=cut(todo1);
				for(int i=1;i<cut_todo1.size()-2;++i){
					string x;
					if(i!=cut_todo1.size()-3){
						x=cut_todo1[i].substr(0,cut_todo1[i].size()-1);
					}
					else{
						x=cut_todo1[i];
					}
					columnname.push_back(x);
				}
				vector<vector<string>> result1=now->multiple_select(todo1);
				string todo2=todo.substr(pos_select2,pos_order-pos_select2-1);
				
				vector<vector<string>> result2=now->multiple_select(todo2);
				
				
				vector<vector<string>> result=result1;
				
				result.insert(result.end(),result2.begin(),result2.end());
				
				if(Tolower(todo).find("union all")!=-1){
					sort(result.begin(),result.end(),cmp2);
					auto end_unique=unique(result.begin(),result.end());
					result.erase(end_unique,result.end());
				}
				
				if(Tolower(todo).find("order")!=-1){
					ordername=t[l-1];
					auto& w=columnname;//找排序指标对应的下标
					for(int i=0;i<w.size();++i){
						string x;
						if(i!=w.size()-3){
							x=w[i].substr(0,w[i].size()-1);
						}
						else x=w[i];
						if(x==ordername) {
							order_sub=i;
							break;
						}
					}
					sort(result.begin(),result.end(),cmp3);
				}
				//result_out<<todo1<<endl<<todo2<<endl;
				for(int i=0;i<columnname.size();++i){
					result_out<<columnname[i]<<'\t';
				}
				result_out<<endl;
				for(int i=0;i<result.size();++i){
					for(int j=0;j<result[i].size();++j){
						result_out<<result[i][j]<<'\t';
					}
					result_out<<endl;
				}*/
				
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
				int pos_count,pos_group,pos_order;//记录count和group在t中的下标
				pos_count=find_pos(t,"count");
				pos_group=find_pos(t,"group");
				pos_order=find_pos(t,"order");
				string todo="";
				for(int i=0;i<(pos_group==-1?pos_order:pos_group);++i){
					if(!i) todo+=t[i];
					else{
						if(i==pos_count) continue;
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


				/*vector<string>group;
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
					result_out<<group[i]<<'\t';
				}
				if(mode){
					string u="COUNT(";
					u+=countname;u+=")";
					result_out<<u;
				}
				result_out<<endl;
				overall_columnname.clear();
				for(int i=0;i<group.size();++i){
					overall_columnname.push_back(group[i]);
				}
				ordername=t[l-1];
				overall_mcount=mcount;
				sort(result.begin(),result.end(),cmp);
				for(int i=0;i<result.size();++i){
					for(int j=0;j<result[i].size();++j){
						result_out<<result[i][j]<<'\t';
					}
					if(mode) result_out<<mcount[result[i]];
					result_out<<endl;
				}*/
			}
			else if(Tolower(todo).find("group by")!=-1){//分组语句
				Table* ptable=nullptr;
				int i;//i对应from出现的下标
				for(i=0;i<t.size();++i){
					if(Tolower(t[i])=="from"){
						ptable=now->get_table(t[i+1]);
						break;
					}
				}
				if(ptable==nullptr) continue;
				int pos_count,pos_group;//记录count和group在t中的下标
				pos_count=find_pos(t,"count");
				pos_group=find_pos(t,"group");
				string todo="";
				for(int i=0;i<pos_group;++i){
					if(!i) todo+=t[i];
					else{
						if(i==pos_count) continue;
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
				
				/*vector<string>group;
				
				vector<vector<string>>result;
				string countname="*";
				int mode=0;//mode=1表示有count,mode=0表示无count
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
					result_out<<group[i]<<'\t';
				}
				if(mode){
					string u="COUNT(";
					u+=countname;u+=")";
					result_out<<u;
				}
				result_out<<endl;
				for(int i=0;i<result.size();++i){
					for(int j=0;j<result[i].size();++j){
						result_out<<result[i][j]<<'\t';
					}
					if(mode) result_out<<mcount[result[i]];
					result_out<<endl;
				}*/
			}
			else if(Tolower(todo).find("count")!=-1){//实现COUNT()函数
				result_out<<t[1]<<endl;
				Table* ptable=(*now)[t[l-1]];
				result_out<<ptable->count(t[1])<<endl;
			}
			else if(Tolower(t[0])=="select"){//查询表格信息,包括单表和多表
				
				int pos_from=0,pos_where=0;
				for(int i=0;i<l;++i){
					if(Tolower(t[i])=="from") pos_from=i;
					if(Tolower(t[i])=="where") {
						pos_where=i;break;
					}
				}
				if(pos_where-pos_from>2||(!pos_where&&pos_from<l-2)){//多表
					vector<string>tablename;//存储from后面的表名
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
					vector<string>ctablename;//存储输出时每个列对应的表格的名字
					map<string,int>table_sub;//存储输出时的表名在v中的vector中的第几个
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
						result_out<<columnname[i]<<'\t';
					}
					result_out<<endl;
					for(int i=0;i<v.size();++i){
						for(int j=0;j<columnname.size();++j){
							int x=table_sub[ctablename[j]];
							result_out<<(*(*(*now)[ctablename[j]])[columnname[j]])[v[i][x]]<<'\t';
						}
						result_out<<endl;
					}
				} 
				else{//单表
					
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
					/*for(int i=0;i<columnname.size();++i){
						result_out<<columnname[i]<<'\t';
					}			
					result_out<<endl;
					for(int i=0;i<u.size();++i){
						for(int j=0;j<u[i].size()-1;++j){
							result_out<<u[i][j]<<'\t';
						}
						result_out<<endl;
					}*/
				}
			}
			
			

=======



				/*vector<string>columnname,ltable,rtable,alltable;
				for(int i=pos_from;i<pos_join-1;++i) ltable.push_back(getvalid_string(t[i]));
				for(int i=pos_join+1;i<pos_on;++i) rtable.push_back(getvalid_string(t[i]));
				for(int i=0;i<ltable.size();++i) alltable.push_back(ltable[i]);
				for(int i=0;i<rtable.size();++i) alltable.push_back(rtable[i]);
				if(t[1]=="*"){
					for(int i=0;i<alltable.size();++i){
						vector<string>temp=(*now)[alltable[i]]->getall_columnname();
						for(int j=0;j<temp.size();++j) {
							string u=alltable[i]+".";u+=temp[j];
							columnname.push_back(u);
						}
					}
				}
				else{
					for(int i=1;i<pos_from;++i) columnname.push_back(t[i]);
				}
				/*string condition="";
				if(pos_on!=-1) condition=putvector_tostring(t,pos_on+1,l-1);
				map<string,int>sub_table;
				for(int i=0;i<alltable.size();++i) sub_table[alltable[i]]=i;
				vector<vector<int>>rows=now->where_multiple(alltable,condition);
				vector<vector<string>>result;
				result.push_back(clear_tablename(columnname));
				for(int i=0;i<rows.size();++i){
					vector<string>temp;
					for(int j=0;j<columnname.size();++j){
						int t=columnname[j].find(".");
						string _tablename=columnname[j].substr(0,t);
						string _columnname=columnname[j].substr(t+1,columnname[j].size()-t-1);
						temp.push_back((*(*(*now)[_tablename])[_columnname])[rows[i][sub_table[_tablename]]]);
					}
					result.push_back(temp);
				}
				if(mode=="left"){

				}*/


			}
			else if(Tolower(todo).find("union")!=-1){//实现union操作符
				//先实现UNION连接两个结果集，有需要再改进(已实现连接多个结果集)
				//之前组只实现了select之后有一个属性，这里已改进为可以有多个属性（单表）
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
				int pos_start;//在unions中"union"第一次出现的位置
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
				
				/*int pos_union=Tolower(todo).find("union");
				int pos_select2=Tolower(todo).find("select",pos_union);
				int pos_order=Tolower(todo).find("order");

				if(pos_order==-1){
					pos_order=todo.size()+1;
				}
				string todo1=todo.substr(0,pos_union-1);
				vector<string>columnname;
				auto cut_todo1=cut(todo1);
				for(int i=1;i<cut_todo1.size()-2;++i){
					string x;
					if(i!=cut_todo1.size()-3){
						x=cut_todo1[i].substr(0,cut_todo1[i].size()-1);
					}
					else{
						x=cut_todo1[i];
					}
					columnname.push_back(x);
				}
				vector<vector<string>> result1=now->multiple_select(todo1);
				string todo2=todo.substr(pos_select2,pos_order-pos_select2-1);
				
				vector<vector<string>> result2=now->multiple_select(todo2);
				
				
				vector<vector<string>> result=result1;
				
				result.insert(result.end(),result2.begin(),result2.end());
				
				if(Tolower(todo).find("union all")!=-1){
					sort(result.begin(),result.end(),cmp2);
					auto end_unique=unique(result.begin(),result.end());
					result.erase(end_unique,result.end());
				}
				
				if(Tolower(todo).find("order")!=-1){
					ordername=t[l-1];
					auto& w=columnname;//找排序指标对应的下标
					for(int i=0;i<w.size();++i){
						string x;
						if(i!=w.size()-3){
							x=w[i].substr(0,w[i].size()-1);
						}
						else x=w[i];
						if(x==ordername) {
							order_sub=i;
							break;
						}
					}
					sort(result.begin(),result.end(),cmp3);
				}
				//result_out<<todo1<<endl<<todo2<<endl;
				for(int i=0;i<columnname.size();++i){
					result_out<<columnname[i]<<'\t';
				}
				result_out<<endl;
				for(int i=0;i<result.size();++i){
					for(int j=0;j<result[i].size();++j){
						result_out<<result[i][j]<<'\t';
					}
					result_out<<endl;
				}*/
				
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
				int pos_count,pos_group,pos_order;//记录count和group在t中的下标
				pos_count=find_pos(t,"count");
				pos_group=find_pos(t,"group");
				pos_order=find_pos(t,"order");
				string todo="";
				for(int i=0;i<(pos_group==-1?pos_order:pos_group);++i){
					if(!i) todo+=t[i];
					else{
						if(i==pos_count) continue;
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


				/*vector<string>group;
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
					result_out<<group[i]<<'\t';
				}
				if(mode){
					string u="COUNT(";
					u+=countname;u+=")";
					result_out<<u;
				}
				result_out<<endl;
				overall_columnname.clear();
				for(int i=0;i<group.size();++i){
					overall_columnname.push_back(group[i]);
				}
				ordername=t[l-1];
				overall_mcount=mcount;
				sort(result.begin(),result.end(),cmp);
				for(int i=0;i<result.size();++i){
					for(int j=0;j<result[i].size();++j){
						result_out<<result[i][j]<<'\t';
					}
					if(mode) result_out<<mcount[result[i]];
					result_out<<endl;
				}*/
			}
			else if(Tolower(todo).find("group by")!=-1){//分组语句
				Table* ptable=nullptr;
				int i;//i对应from出现的下标
				for(i=0;i<t.size();++i){
					if(Tolower(t[i])=="from"){
						ptable=now->get_table(t[i+1]);
						break;
					}
				}
				if(ptable==nullptr) continue;
				int pos_count,pos_group;//记录count和group在t中的下标
				pos_count=find_pos(t,"count");
				pos_group=find_pos(t,"group");
				string todo="";
				for(int i=0;i<pos_group;++i){
					if(!i) todo+=t[i];
					else{
						if(i==pos_count) continue;
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
				
				/*vector<string>group;
				
				vector<vector<string>>result;
				string countname="*";
				int mode=0;//mode=1表示有count,mode=0表示无count
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
					result_out<<group[i]<<'\t';
				}
				if(mode){
					string u="COUNT(";
					u+=countname;u+=")";
					result_out<<u;
				}
				result_out<<endl;
				for(int i=0;i<result.size();++i){
					for(int j=0;j<result[i].size();++j){
						result_out<<result[i][j]<<'\t';
					}
					if(mode) result_out<<mcount[result[i]];
					result_out<<endl;
				}*/
			}
			else if(Tolower(todo).find("count")!=-1){//实现COUNT()函数
				result_out<<t[1]<<endl;
				Table* ptable=(*now)[t[l-1]];
				result_out<<ptable->count(t[1])<<endl;
			}
			else if(Tolower(t[0])=="select"){//查询表格信息,包括单表和多表
				
				int pos_from=0,pos_where=0;
				for(int i=0;i<l;++i){
					if(Tolower(t[i])=="from") pos_from=i;
					if(Tolower(t[i])=="where") {
						pos_where=i;break;
					}
				}
				if(pos_where-pos_from>2||(!pos_where&&pos_from<l-2)){//多表
					vector<string>tablename;//存储from后面的表名
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
					vector<string>ctablename;//存储输出时每个列对应的表格的名字
					map<string,int>table_sub;//存储输出时的表名在v中的vector中的第几个
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
						result_out<<columnname[i]<<'\t';
					}
					result_out<<endl;
					for(int i=0;i<v.size();++i){
						for(int j=0;j<columnname.size();++j){
							int x=table_sub[ctablename[j]];
							result_out<<(*(*(*now)[ctablename[j]])[columnname[j]])[v[i][x]]<<'\t';
						}
						result_out<<endl;
					}
				} 
				else{//单表
					
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
					/*for(int i=0;i<columnname.size();++i){
						result_out<<columnname[i]<<'\t';
					}			
					result_out<<endl;
					for(int i=0;i<u.size();++i){
						for(int j=0;j<u[i].size()-1;++j){
							result_out<<u[i][j]<<'\t';
						}
						result_out<<endl;
					}*/
				}
			}
			
			

>>>>>>> 0b87a9b137417d7f7ca828a4b8bf631b20e29dee
		}
		fstream fout;
		fout.open(data_keep_filename,ios::out|ios::ate);
		for(int i=0;i<all_database.size();++i){
			database[all_database[i]]->keep_data(data_keep_filename,fout);
		}
		//fout<<"nowdatabase: "<<now->getname()<<endl;
		fout.close();
		/*else {
			result_out << "WRONG INPUT!\n"; //防止错误输入使程序崩溃 
			continue;
		}*/
		
	}
	string oop=result_out.str();
	result=oop;
<<<<<<< HEAD
	if(result=="") result="OK";
=======
	if(result=="")result="OK";
//	else result="OK";
>>>>>>> 0b87a9b137417d7f7ca828a4b8bf631b20e29dee
    return result;
}

#endif

