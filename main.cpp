#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <vector>
#include "database.h"
using namespace std;

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
		else if (str_com(todo.substr(0,7),"SELECT ")) { //��ѯ�����Ϣ 
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
		else {
			cout << "WRONG INPUT!\n"; //��ֹ��������ʹ������� 
			continue;
		}
	}
	return 0;
}
