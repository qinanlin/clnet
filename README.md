# clnet
一个简单的网络框架，有连接处理，协议解封，共享内存，日志处理等功能。部分功能还未完善  

## 主要文件
* 逻辑服: logic_server.cpp
* 连接服: conn_server.cpp
* 日志服: log_server.cpp
* 模拟客户端: main_client.cpp
* 共享内存: sharemem.cpp

## 主要功能
### 共享内存
* 采用无锁循环队列  

### 连接服
* 单线程epoll模型
* 内存池
* TCP


### 逻辑服
* 连接服与逻辑服之间用共享内存交换数据
* 数据序列化采用protocolbuffer

### 日志系统
* 日志服与逻辑服之间用共享内存交换数据  
* 日志以文本的形式呈现  


## 待开发的功能
* 支持UDP
* 数据库
* 配置文件
