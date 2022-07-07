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
- 不常使用

3. io复用  
- select
- 用select改进回射客户端程序  
4. 信号驱动io
- 不常使用
5. 异步io
- 效率最高
### 14p
- select
- 读、写、异常事件发生条件
- 用select改写回射服务器程序


### 15p
- close 与 shutdown 区别
- 进一步改进回射服务器

### 16p
- 套接字i/o超时的设置
- 用select实现超时设置的方法
    - alarm
    - setsockopt
    - select

### 17p
- select 并发的限制
- poll

### 18p
- epoll 的使用
- epoll 与 select 、poll的区别
- epoll LT/ET模式

### 19p
- udp特点
- udp客户/服务基本模型
- udp回射客户/服务器
- udp注意点





