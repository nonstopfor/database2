#ifndef MATH_CAL_H
#define MATH_CAL_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <typeinfo>
using namespace std;
//with elements input and result output
//if TY==double then the operand % is not allowed



template <typename TY>
string CAA(vector<string> & hold_strs, bool& NULL_flag){
    if(NULL_flag)return "NULL";
	vector<string> oper_stack;
    vector<TY> data_stack;
    //correct datas and do goods
    bool wrong_flag=false;
    for(auto & i : hold_strs){
        if(i!="+"&&i!="-"&&i!="*"&&i!="/"&&i!="%"){
            if(typeid(TY)==typeid(int)){
                data_stack.push_back(atoi(i.c_str()));
            }
            else if(typeid(TY)==typeid(double)){
                data_stack.push_back(atof(i.c_str()));
            }else{
                NULL_flag=true;
                return "NULL";
            }
        }
        else{
            if(oper_stack.size()==0)oper_stack.push_back(i);
            else if(i=="+"||i=="-"){
                while(oper_stack.size()!=0){
                    string op_hold=oper_stack[oper_stack.size()-1];
                    oper_stack.pop_back();
                    if(data_stack.size()<2)return "NULL";
                    TY op2=data_stack[data_stack.size()-1];
                    data_stack.pop_back();
                    TY op1=data_stack[data_stack.size()-1];
                    data_stack.pop_back();
                    if(op_hold=="+")data_stack.push_back((TY)op1+(TY)op2);
                    else if(op_hold=="-")data_stack.push_back((TY)op1-(TY)op2);
                    else if(op_hold=="*")data_stack.push_back((TY)op1*(TY)op2);
                    else if(op_hold=="/"){
                        if(op2==0)return "NULL";
                        else data_stack.push_back((TY)op1/(TY)op2);
                    }
                    else if(op_hold=="%"){
                        if(typeid(TY)==typeid(int)&&op2!=0){
                            data_stack.push_back((int)op1%(int)op2);
                        }else return "NULL";
                    }
                    else return "NULL";

                }
                oper_stack.push_back(i);
                if(data_stack.size()!=1)return "NULL";
            }
            else if(i=="*"||i=="/"||i=="%"){
                while(oper_stack.size()>0&&oper_stack[oper_stack.size()-1]!="+"&&oper_stack[oper_stack.size()-1]!="-"){
                    string op_hold=oper_stack[oper_stack.size()-1];
                    oper_stack.pop_back();
                    if(data_stack.size()<2)return "NULL";
                    TY op2=data_stack[data_stack.size()-1];
                    data_stack.pop_back();
                    TY op1=data_stack[data_stack.size()-1];
                    data_stack.pop_back();
                    if(op_hold=="*")data_stack.push_back((TY)op1*(TY)op2);
                    else if(op_hold=="/"){
                        if(op2==0)return "NULL";
                        else data_stack.push_back((TY)op1/(TY)op2);
                    }
                    else if(op_hold=="%"){
                        if(typeid(TY)==typeid(int)&&op2!=0){
                            data_stack.push_back((int)op1%(int)op2);
                        }else return "NULL";
                    }
                    else return "NULL";
                }   
                oper_stack.push_back(i);
            }
            else return "NULL";
        }
    }
    if(oper_stack.size()==0&&data_stack.size()==1){
        string re;
        TY mid;
        //limited accuracy
        stringstream IN;
        IN<<data_stack[0];
        IN>>re;
        return re;
    }else if(oper_stack.size()>0){
        while(oper_stack.size()!=0){
            string op_hold=oper_stack[oper_stack.size()-1];
            oper_stack.pop_back();
            if(data_stack.size()<2)return "NULL";
            TY op2=data_stack[data_stack.size()-1];
            data_stack.pop_back();
            TY op1=data_stack[data_stack.size()-1];
            data_stack.pop_back();
            if(op_hold=="+")data_stack.push_back((TY)op1+(TY)op2);
            else if(op_hold=="-")data_stack.push_back((TY)op1-(TY)op2);
            else if(op_hold=="*")data_stack.push_back((TY)op1*(TY)op2);
            else if(op_hold=="/"){
                if(op2==0){
                //cout<<"typical wrong"<<endl;
                return "NULL";
                }
                else data_stack.push_back((TY)op1/(TY)op2);
            }
            else if(op_hold=="%"){
                if(typeid(TY)==typeid(int)&&op2!=0){
                    data_stack.push_back((int)op1%(int)op2);
                }else return "NULL";
            }
            else return "NULL";
            
        }
        if(data_stack.size()!=1)return "NULL";
        string re;
        TY mid;
        //limited accuracy
        stringstream IN;
        IN<<data_stack[0];
        IN>>re;
        return re;
    }else return "NULL";
	
}


string CAL_alg(vector<string> &hold_strs, string type, bool& NULL_flag);
//计算表达式的值

string CAA_2(vector<string> & hold_strs, bool& NULL_flag);

#endif
