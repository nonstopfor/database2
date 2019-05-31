1. 运行方式 ：
	运行服务器 ： ./start server
	指定端口运行服务器 ： ./start server $PORT_NUM
	运行本机客户端 ： ./start client
	运行客户端连接远程服务器 ： ./start client $IP_ADDRESS $PORT_NUM
2. 运行指令 ：
	客户端 ： exit断开连接
	服务器 ： shut服务器关机
3. 文件 ： 
	name_and_code.txt ： 记录用户的用户名和密码
	the_place_to_hold_main_function.h ： 
		这一部分需要另外将当前main函数中的功能整合为一个新的函数 ： string miamia(string input)
		这个函数将读入mysql指令，并返回查找的结果
