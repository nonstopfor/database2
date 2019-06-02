# database2

>## 其他目录

>## 服务器与客户端的使用
>>### 服务器
>>>1. 本机服务器启动（默认端口为5111） ：   
`./start server`  
>>>2. 本机服务器启动（$PROT指定为端口号） ：    
`./start server $PORT`  
示例 ： `./start server 1234`
>>>3. 本机服务器关闭 ： 在服务器窗口输入`shut`  

>>## 客户端
>>>1. 本机客户端启动（选用默认端口5111） ： 
`./start client`
>>>2. 本机客户端启动（$PORT为服务器监听端口） ：
`./start client $PORT`  
示例 ： `./start client 1234`   
>>>3. 客户端启动连接远程服务器（\$P为服务器地址，$PORT为服务器监听端口） ：     
`./start client $IP $PORT`  
示例 ： `./start client 192.168.0.5 5111`   
>>>4. 客户端断开连接 ： 在客户端窗口输入`exit`
