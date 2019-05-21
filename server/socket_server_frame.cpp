#include <sys/socket.h>
#include <sys/types.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <netinet/in.h>
#include <unistd.h>
#include <bitset>
#include <vector>
#include <arpa/inet.h>
#include <pthread.h>
using namespace std;
#define MAX_CLIENT 10
#define EXI "exit"
#define PASS "pass"
#define NAME "your name please : "
#define PASSWD "your password please : "
#define PARDON "non valid input"
#define SUCC "data received"
#define WRONG "wrong match, try again ? (Y/N) "
#define ERR "EEOoFf"
pthread_mutex_t lokk;
char recv_buff[1024];

string msg;
struct ID_sock{
    int sockke;
    int seq;
};

struct name_and_code{
    string nami;
    string code;
};
vector<name_and_code> main_check;
long IDS[MAX_CLIENT];
//write pattern
void write_pattern(int sock,string to_write,int& ju_flag){
    pthread_mutex_lock(&lokk);
    if(to_write==""){
        cout<<"wrong para"<<endl;
        exit(1);
    }
    int rt=write(sock,to_write.c_str(),sizeof(to_write));
    if(rt<0){
        cout<<"lose client!"<<endl;
        ju_flag=1;
    }
    pthread_mutex_unlock(&lokk);

}
//read pattern
string read_pattern(int sock,int& ju_flag){
    string resu;
    memset(recv_buff,0,sizeof(recv_buff));
    int kk=read(sock,recv_buff,sizeof(recv_buff)-1);
    pthread_mutex_lock(&lokk);
    if(kk<=0){
        cout<<"client problem!"<<endl;
        ju_flag=1;
        pthread_mutex_unlock(&lokk);
        return ERR;
    }else{
        recv_buff[kk]='\0';
        resu=recv_buff;
    }
    memset(recv_buff,0,sizeof(recv_buff));
    pthread_mutex_unlock(&lokk);
    return resu;
}
void* destroy(void * threadarg){
    string SHr;
    while(1){
        cin>>SHr;
        if(SHr!="shut")continue;
        else{
            for(auto i : IDS)pthread_cancel(i);
            pthread_exit(NULL);
            break;
        }
    }
}
void* accept_func(void* threadarg){

    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    int client_sock=0;
    sockaddr_in client_address;
    memset(&client_address,0,sizeof(client_address));
    ID_sock datas=*(ID_sock *)threadarg;
    int listeningSock=datas.sockke;
    int seq=datas.seq;
    IDS[seq]=pthread_self();
    socklen_t SI=(socklen_t)sizeof(client_address);
    while(1){
        int len_pos;
        client_sock=accept(listeningSock,(sockaddr*)&client_address,&SI);
        pthread_mutex_lock(&lokk);
        std::cout<<"-----------one connection !--------------"<<endl;
        getpeername(client_sock,(sockaddr*)&client_address,&SI);
        std::cout<<"client ip : "<<inet_ntoa(client_address.sin_addr)<<":"<<ntohs(client_address.sin_port)<<endl;
        pthread_mutex_unlock(&lokk);
        //start asking

        string namie;
        string codie;
        int rt;
        int exit_flag=0;
        int break_flag=0;
        while(1){
            if(exit_flag||break_flag)break;
            memset(recv_buff,0,sizeof(recv_buff));
            write_pattern(client_sock,NAME,exit_flag);
            msg=read_pattern(client_sock,exit_flag);
            if(msg==ERR||msg==EXI){
                exit_flag=true;
                continue;
            }
            namie=msg;
            memset(recv_buff,0,sizeof(recv_buff));
            write_pattern(client_sock,PASSWD,exit_flag);
            msg=read_pattern(client_sock,exit_flag);
            if(msg==ERR){
                exit_flag=true;
                continue;
            }
            codie=msg;
            memset(recv_buff,0,sizeof(recv_buff));

            //find flag
            bool find_flag=false;
            for(auto &i : main_check){
                if(i.nami==namie&&i.code==codie){
                    write_pattern(client_sock,PASS,exit_flag);
                    find_flag=true;
                    break_flag=1;
                    break;
                }
            }
            if(!find_flag){
                memset(recv_buff,0,sizeof(recv_buff));
                write_pattern(client_sock,WRONG,exit_flag);
                msg=read_pattern(client_sock,exit_flag);
                if(msg=="Y"){
                    continue;
                }else{
                    exit_flag=1;
                    continue;
                }
                memset(recv_buff,0,sizeof(recv_buff));

            }

        }

        //above asking sentences

        //now start processing client problems
        
        while(1){
            memset(recv_buff,0,sizeof(recv_buff));
            if(exit_flag)break;
            msg=read_pattern(client_sock,exit_flag);
            if(msg==ERR||msg==EXI){
                exit_flag=true;
                continue;
            }else{
                //how will you deal with it?
                write_pattern(client_sock,SUCC,exit_flag);

            }
        }

        close(client_sock);
        pthread_mutex_lock(&lokk);
        std::cout<<"connection closed!"<<endl;
        pthread_mutex_unlock(&lokk);
        sleep(1);

    }

    //do what you like above
    
}

int main(int argc,char** argv){
    //name and code datas
    ifstream IN("name_and_code.txt");
    string namie;string codie;
    while(IN>>namie>>codie){
        main_check.push_back(name_and_code{namie,codie});
    }
    IN.close();




    int listening_sock=0,connect_sock=0,client_sock=0;
    sockaddr_in listening_address,connecting_address,client_address;
    memset(&listening_address,0,sizeof(listening_address));
    memset(&connecting_address,0,sizeof(connecting_address));
    memset(&client_address,0,sizeof(client_address));
    int CL_SIZE=sizeof(client_address);
    string msg;
    

    listening_sock=socket(AF_INET,SOCK_STREAM,0);
    if(listening_sock<0){
        std::cout<<"error : cannot get the server sock"<<endl;
        return 1;
    }

    listening_address.sin_family=AF_INET;
    listening_address.sin_port=htons(atoi(argv[1]));
    listening_address.sin_addr.s_addr=htonl(INADDR_ANY);    //ip address to be defined

    bind(listening_sock,(const sockaddr*)&listening_address,sizeof(listening_address));
    if(listen(listening_sock,MAX_CLIENT)==-1){
        std::cout<<"fail to start listening process"<<endl;
        return 1;
    }

    std::cout<<"start listening on ip : "<<inet_ntoa(listening_address.sin_addr)<<":"<<ntohs(listening_address.sin_port)<<endl;

    pthread_t threads[MAX_CLIENT+1];
//    cout<<bitset<MAX_CLIENT+2>(full)<<endl;

    ID_sock nubber[MAX_CLIENT];
    for(int i=0;i<=MAX_CLIENT-1;i++){
        nubber[i].seq=i;
        nubber[i].sockke=listening_sock;
        pthread_create(&threads[i],NULL,accept_func,(void*)&nubber[i]);
    }
    pthread_create(&threads[MAX_CLIENT],NULL,destroy,NULL);
    pthread_exit(NULL);

    close(listening_sock);
    return 0;
}