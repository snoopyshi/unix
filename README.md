## 学习unix系统编程的一些代码





## Socket

### 10p

- read, write 与 recv, send
- readline 实现
- 用 readline 实现回射客户/服务器
- getsocketname, getpeername
- gethostname, gethostbyname, gethostbyaddr


### 11p
- tcp 回射客户/服务器  
- tcp 是个流协议  
- 僵进程与SIGCHLD信号  


### 12p  

- tcp 11种状态

服务器端: 
1. 服务器创建一个socke, close 状态. 
2. 服务端调用 bind listen, close -> listen 状态
3.  

- 连接建立三次握手,连接终止四次挥手
- TIME_WAIT 与 SO_REUSEADDR  
- SIGPIPE  

### 13p
- 五种IO模型
1. 阻塞io
2. 非阻塞io
- select
- 用select改进回射客户端程序  


### 14p
- 


