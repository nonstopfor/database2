g++ -std=c++11 -o server_frame socket_server_frame.cpp -lpthread
g++ -std=c++11 -o client_frame socket_client_frame.cpp -lpthread
if [ "$1" != "" ];then
    if [ "$1" = "server" ];then
        ./server_frame 5111
    elif [ "$1" = "client" ];then
        ./client_frame "127.0.0.1" 5111
    else echo "wrong para"
    fi
else echo "specify server or client"
fi
