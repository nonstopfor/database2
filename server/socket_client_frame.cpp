#include <iostream>
#include <cstring>
#include <cstdio>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
using namespace std;
#define WRONG "wrong match, re enter please!"
int main(int argc,char** argv){
    int ask_sock=0;
    sockaddr_in server_address;
    memset(&server_address,0,sizeof(server_address));
    string recv_msg;
    string sent_msg;
    char recv_buff[1024];

    ask_sock=socket(AF_INET,SOCK_STREAM,0);
    if(ask_sock<0){
        cout<<"error : cannot create client socket";
        return 1;
    }

    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(atoi(argv[2]));
    server_address.sin_addr.s_addr=inet_addr(argv[1]);

    //try to connect
    if(connect(ask_sock,(const sockaddr*)&server_address,sizeof(server_address))<0){
        cout<<"error : connection failure!"<<endl;
        return 1;
    }
    cout<<"connected to ip : "<<inet_ntoa(server_address.sin_addr)<<":"<<ntohs(server_address.sin_port)<<endl;

    //now already connected
    //do what you like below
    //answer questions
    int pass_flag=0;
    while(1){
        //listen to the question
        int len_pos=read(ask_sock,recv_buff,sizeof(recv_buff)-1);
        if(len_pos<=0){
            cout<<"server error"<<endl;
            break;
        }
        recv_buff[len_pos]='\0';
        printf("server > %s  ",recv_buff);
        if(strcmp("pass",recv_buff)==0){
            printf("\n");
            pass_flag=1;
            sleep(1);
            break;
        }else{

            //other idea?
        }
        memset(recv_buff,0,sizeof(recv_buff));

        //then answer
        string ans;
        getline(cin,ans);

        int re=write(ask_sock,ans.c_str(),sizeof(ans));
        if(re<0){
            cout<<"lose connection accidently!"<<endl;
            break;
        }

    }
    memset(recv_buff,0,sizeof(recv_buff));



    bool quit_flag=false;
    while(pass_flag){
        //client will send first
        cout<<">";
        getline(cin,sent_msg);
        if(sent_msg=="")sent_msg="exit";
        int re=write(ask_sock,sent_msg.c_str(),sizeof(sent_msg));
        if(re<0){
            cout<<"lose connection accidently!"<<endl;
            break;
        }else if(sent_msg=="exit"){
            cout<<"exit successfully"<<endl;
            break;
        }

        //then receive
        int len_pos=read(ask_sock,recv_buff,sizeof(recv_buff)-1);
        if(len_pos<=0){
            cout<<"reply error"<<endl;
            break;
        }
        recv_buff[len_pos]='\0';
        if(strcmp("exit",recv_buff)==0){
            cout<<"server rejects you!"<<endl;
            break;
        }
        cout<<"REPLY : "<<endl;
        printf("\t%s\n",recv_buff);
        memset(recv_buff,0,sizeof(recv_buff));
    }

    //do what you like above
    close(ask_sock);
    return 0;
}