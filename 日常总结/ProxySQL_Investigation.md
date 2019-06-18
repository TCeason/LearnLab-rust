# ProxySQL V2.0.2

## ProxySQL 架构介绍

### 多级配置系统介绍

ProxySQL 是一个拥有复杂多级配置系统（Multi layer configuration system），但是易于使用的 Proxy。正是因为有一个复杂的多级配置系统，所以，可以满足以下需求

> * 允许对配置进行简单的动态更新，允许业务零停机时间配置
> * 允许动态修改尽可能多的配置项，且无需重启 ProxySQL 进程
> * 可以实现回滚无效配置

 **Note：**配置层分为以下三个层级
```
 +-------------------------+
 |         RUNTIME         |
 +-------------------------+
       /|\          |
        |           |
    [1] |       [2] |
        |          \|/
 +-------------------------+
 |         MEMORY          |
 +-------------------------+ 
       /|\          |      |\
        |           |        \
    [3] |       [4] |         \ [5]
        |          \|/         \
 +-------------------------+  +-------------------------+
 |          DISK           |  |       CONFIG FILE       |
 +-------------------------+  +-------------------------+

```

* RUNTIME 代表 ProxySQL 运行时，其线程使用的内存中的数据结构。运维人员是不能直接休干 RUNTIME配置部分的内容，只能通过底层然后去 load 配置。RUNTIME 变量（runtime variables）包括：
  * global variables 中的实际值
  * hostgroups 中的 后端服务（backend servers）的实际使用值
  * mysql_users 中可以链接到 ProxySQL 和 backend servers 的用户信息
* MEMORY（也可以表示为 main），代表通过 MySQL 兼容的公开接口的内存数据库，运维人员可以通过 `MySQL Client` 连接到 ProxySQL 的管理界面以查询各种 ProxySQL 的配置参数等信息。通过该接口可配置的表是：
  * mysql_servers: ProxySQL连接到的后端服务器列表
  * mysql_users: 连接到ProxySQL的用户及其认证列表。ProxySQL也将使用相同的认证信息连接到后端服务器。
  * mysql_query_rules:  将流量路由到各种后端服务器时评估的查询规则列表，读写分离将依赖此配置
  * global_variables:  ProxySQL 使用的全局变量以及变量的值
* DISK & CONFIG FILE
  * DISK: 磁盘上的存于 SQLite3 的数据。重启 ProxySQL 时，内存中（RUNTIME+MEMORY）配置将丢失。
  * CONFIG FILE: 配置文件，默认在 /etc/proxysql.cnf

### ProxySQL 实例生命周期

#### 启动

正常启动时，会通过该命令 `proxysql -c /etc/proxysql.cnf`  读取配置文件，确定 `datadir`。
> 所以，datadir 是否存在将产生根本差别：
> * datadir 不存在：
> 解析配置文件，并将内容加载到内存数据库（in-memory database），同时将其保存在磁盘数据库（on-disk database）中，运行时加载
> * datadir 存在：
> ProxySQL将从持久存储的磁盘数据库初始化其内存中配置。因此，磁盘被加载到内存中，然后传播到RUNTIME。

#### 初始化启动方式

使用 `--initial` 标识，或者，datadir 不存在 的时候，都会解析配置文件，并加载到内存，并传播到RUNTIME。

#### 重载启动

使用 `--reload` 标识，会尝试 merge CONFIG FILE 和 DISK 的配置，如果有冲突会报错。

#### 在 RUNTIME 修改配置

修改 RUNTIME 相关 tables 后，通过支持的命令集合进行 load configuration。详尽方法参见下一节

#### 修改配置后传到其他配置层

修改方法可以是从 DISK TO RUNTIME/MEMORY 或者 MEMORY TO RUNTIME/DISK ，这里只介绍如何从 MEMORY TO RUNTIME/DISK。

```sql
LOAD MYSQL USERS TO RUNTIME;
SAVE MYSQL USERS TO DISK;

LOAD MYSQL SERVERS TO RUNTIME;
SAVE MYSQL SERVERS TO DISK;

LOAD MYSQL QUERY RULES TO RUNTIME;
SAVE MYSQL QUERY RULES TO DISK;

LOAD MYSQL VARIABLES TO RUNTIME;
SAVE MYSQL VARIABLES TO DISK;

LOAD ADMIN VARIABLES TO RUNTIME;
SAVE ADMIN VARIABLES TO DISK;

```

## ProxySQL 配置

开始此章节的时候，假定 proxysql 以及 后端 MySQL 主从服务（ backend service） 已经安装完毕。如果 ProxySQL 安装有问题，参考[下节](#ProxySQL 源码探析)。接下来开始 ProxySQL 配置（均采用默认配置项）。

### 配置 Backend Service

* 根据 proxysql.cnf 默认给定的 monitor 账户（监管 backend service 状态）在 backend service 建立相关监控账户。
```
mysql -uroot -hbackend_master_ip --prompt='backend_service> '
backend_service> CREATE USER 'monitor'@'%' IDENTIFIED BY 'monitor';
backend_service> GRANT USAGE ON *.* TO 'monitor'@'%';
```

### 配置 ProxySQL

* 同步 backend service users 到 ProxySQL 的 mysql_users 表中。后端以 MySQL 5.7.20-18 为例：

> ```
> mysql -uroot -hbackend_master_ip --prompt='backend_service> '
> backend_service> select User, authentication_string from mysql.user where host not in ('127.0.0.1', 'localhost', '::1')";
> 
> -- 得到下面两行数据，将用户信息插入到 ProxySQL 的 mysql_users 表中。
> 
> +-------+-------------------------------------------+
> | User  | authentication_string                     |
> +-------+-------------------------------------------+
> | test  | *94BDCEBE19083CE2A1F959FD02F964C7AF4CFC29 |
> | admin | *4ACFE3202A5FF5CF467898FC58AAB1D615029441 |
> +-------+-------------------------------------------+
> 
> mysql -uadmin -padmin  -h127.0.0.1 -P6032 --prompt='proxysql> '
> proxysql> insert into mysql_users(username, password) values('test', '*94BDCEBE19083CE2A1F959FD02F964C7AF4CFC29'), ('admin', '*4ACFE3202A5FF5CF467898FC58AAB1D615029441');
> 
> proxysql> LOAD MYSQL USERS TO RUNTIME;
> 
> proxysql> SAVE MYSQL USERS TO DISK;
> 
> ```

* 配置 mysql_services

>```sql
> mysql -uadmin -padmin  -h127.0.0.1 -P6032 --prompt='proxysql> '
> proxysql> select * from mysql_servers;
> +--------------+---------------+------+-----------+--------+--------+-------------+-----------------+---------------------+---------+----------------+---------+
> | hostgroup_id | hostname      | port | gtid_port | status | weight | compression | max_connections | max_replication_lag | use_ssl | max_latency_ms | comment |
> +--------------+---------------+------+-----------+--------+--------+-------------+-----------------+---------------------+---------+----------------+---------+
> | 1            | 192.168.0.250 | 3306 | 0         | ONLINE | 1      | 0           | 1000            | 20                  | 1       | 0              |         |
> | 0            | 192.168.0.250 | 3306 | 0         | ONLINE | 1      | 0           | 1000            | 20                  | 1       | 0              |         |
> | 1            | 192.168.0.249 | 3306 | 0         | ONLINE | 4      | 0           | 1000            | 20                  | 1       | 0              |         |
> +--------------+---------------+------+-----------+--------+--------+-------------+-----------------+---------------------+---------+----------------+---------+
> 
> ```
> **Note:** 
> * max_replication_lag 表示复制延迟超过该值后，将把 该 hostgroup_id 设置为 shunning 状态。max_replication_lag 设置为 0 则不会检测延迟落后。
> * 写IP 设置到 hostgroup_id 为 读组别 中是防止 从库 shunning 状态时，访问失败。但是，需要调低 weight。
> * max_connections 表示该 backend 允许最大链接数。建议和 backend service 的 max_connections 值相同。

* mysql_replication_hostgroups

>```sql
> proxysql> select * from runtime_mysql_replication_hostgroups;
> +------------------+------------------+-----------------+---------+
> | writer_hostgroup | reader_hostgroup | check_type      | comment |
> +------------------+------------------+-----------------+---------+
> | 0                | 1                |   read_only     |         |
> +------------------+------------------+-----------------+---------+
> 
>```
> **Note:** 该表填写后，通过 monitor 账户去获取该值，会以 read_only 的值为 读/写 库判断依据，将 mysql_servers 的 hostname 对应的 hostgroup_id 设置为 0 或 1。建议采用 LVS 分配 VIP，方便管理。

## ProxySQL 读写分离

ProxySQL 是通过`自定义sql路由规则`就可以实现读写分离。定义路由规则，如：除select * from tb for update的select全部发送到slave，其他的的语句发送到master。

* mysql_query_rules

> ```sql
> proxysql> select rule_id,active,match_digest,destination_hostgroup,apply from mysql_query_rules;
> +---------+--------+----------------------+-----------------------+-------+
> | rule_id | active | match_digest         | destination_hostgroupProxySQL V2.0.2 | apply |
> +---------+--------+----------------------+-----------------------+-------+
> | 1       | 1      | ^SELECT.*FOR UPDATE$ | 0                     | 1     |
> | 2       | 1      | ^SELECT              | 1                     | 1     |
> +---------+--------+----------------------+-----------------------+-------+
> ```

## ProxySQL V2.0.2 源码探析

源码是基于 C/C++ 编写的。Makefile 是硬编码，所以，需要 make && sudo make install 将命令安装到 /usr/bin/proxysql。

### 源代码组织结构

| 目录 | 作用 |
| -- | :--: |
| deps | 依赖，如lz4，ssl，jemalloc 等 |
| include | ProxySQL 代码包含的所有头文件 |
| lib | 有所有 ProxySQL 生成 libproxysql.a 和ProxySQL 核心 API |
| src | ProxySQL Main 调用 |


### src/

* src/proxysql_global.cpp:

```cpp
#define PROXYSQL_EXTERN
#include "proxysql.h"
//#include "proxysql_glovars.hpp"
#include "cpp.h"
//ProxySQL_GlobalVariables GloVars;

```

proxysql_global.cpp 定义了 PROXYSQL_EXTERN，对应 include/proxysql_structs.h:

```cpp
583:#ifdef PROXYSQL_EXTERN
...
```

* main.cpp:

> (1) 定义了用于返回定义为 plugin 的全部变量
> (2) 内存管理机制
> 
> ```cpp
> const char *malloc_conf = "xmalloc:true,lg_tcache_max:16,purge:decay,prof:true,prof_leak:true,lg_prof_sample:20,lg_prof_interval:30,prof_active:false";
>
> ```
> * xmalloc:true  --> Abort-on-out-of-memory enabled
> * lg_tcache_max:16 --> Maximum size class to cache in the thread-specific cache 2^16=64KB
> * prof_leak:true --> Leak reporting enabled
> * prof:true --> Memory profiling enabled
> * lg_prof_sample:20 --> sample interval 1MB, decreases the computational overhead.
> 
> (3) 定义 mysql_worker_thread_func() ，每个 worker_thread 都会调用
> 
> ```cpp
> void * mysql_worker_thread_func(void *arg) {

> //	__thr_sfp=l_mem_init();
> // 监听 TCP 和 Unix socket
>	pthread_attr_t thread_attr;
>	size_t tmp_stack_size=0;
>	if (!pthread_attr_init(&thread_attr)) {
>		if (!pthread_attr_getstacksize(&thread_attr , &tmp_stack_size )) {
>			__sync_fetch_and_add(&GloVars.statuses.stack_memory_mysql_threads,tmp_stack_size);
>		}
>	}
>
>	proxysql_mysql_thread_t *mysql_thread=(proxysql_mysql_thread_t *)arg;
>	MySQL_Thread *worker = new MySQL_Thread();
>	mysql_thread->worker=worker;
>	worker->init();
> //	worker->poll_listener_add(listen_fd);
> //	worker->poll_listener_add(socket_fd);
> // 等待所有 load_ 开头的函数
> 	__sync_fetch_and_sub(&load_,1);
> 	do { usleep(50); } while (load_);
> 
> 	worker->run();
> 	//delete worker;
> // worker->run() 函数可能会收到 shutdown = 1 ，如果不删除会破坏内存分配。
> 	delete worker;
> //	l_mem_destroy(__thr_sfp);
> 	__sync_fetch_and_sub(&GloVars.statuses.stack_memory_mysql_threads,tmp_stack_size);
> 	return NULL;
> }
> ...
> // num_threads 由 参数 mysql-threads 的值决定，default: 4 
> 	for (i=0; i<GloMTH->num_threads; i++) {
>		GloMTH->create_thread(i,mysql_worker_thread_func, false);
> 
> ```
> 
> (4) 优先读配置文件，然后在读 datadir
> ```cpp
> void ProxySQL_Main_process_global_variables(int argc, const char **argv) {
> ...
>    if (GloVars.confFile->OpenFile(GloVars.config_file) == true) {}
> ...
>     if (GloVars.__cmd_proxysql_datadir==NULL) {
>        // datadir was not specified , try to read config file
>         if (GloVars.configfile_open==true) {...}
>	  }
>    else {
>        GloVars.datadir=GloVars.__cmd_proxysql_datadir;
>    }
> ```

### include/

* proxysql_struct.h

> 定义了 global variables

### lib/

* mysql_session.cpp

> 实现 mysql_session.h， 基于一个 session 状态确定执行路径。抽取 mysql_errcode 进行分类分析，并决定如何处理该链接。
> ```cpp
> switch (myerr) {
    case 1317:  // Query execution was interrupted
        if (killed==true) { // this session is being kiled
            handler_ret = -1;
            return handler_ret;
        }
        if (myds->killed_at) {
            // we intentionally killed the query
            break;
        }
    case 1290: // read-only
    case 1047: // WSREP has not yet prepared node for application use
    case 1053: // Server shutdown in progress
        myconn->parent->connect_error(myerr);
        if (myds->query_retries_on_failure > 0) {
            myds->query_retries_on_failure--;
            if ((myds->myconn->reusable==true) && myds->myconn->IsActiveTransaction()==false && myds->myconn->MultiplexDisabled()==false) {
                retry_conn=true;
                proxy_warning("Retrying query.\n");
            }
        }
        switch (myerr) {
            case 1047: // WSREP has not yet prepared node for application use
            case 1053: // Server shutdown in progress
                myds->destroy_MySQL_Connection_From_Pool(false);
                break;
            default:
                if (mysql_thread___reset_connection_algorithm == 2) {
                    create_new_session_and_reset_connection(myds);
                } else {
                    myds->destroy_MySQL_Connection_From_Pool(true);
                }
                break;
        }
        myconn = myds->myconn; // re-initialize
        myds->fd=0;
        if (retry_conn) {
            myds->DSS=STATE_NOT_INITIALIZED;
            //previous_status.push(PROCESSING_QUERY);
        switch(status) { // this switch can be replaced with a simple previous_status.push(status), but it is here for readibility
            case PROCESSING_QUERY:
                previous_status.push(PROCESSING_QUERY);
                break;
            case PROCESSING_STMT_PREPARE:
                previous_status.push(PROCESSING_STMT_PREPARE);
                break;
            default:
                assert(0);
                break;
            }
            if (errmsg) {
                free(errmsg);
                errmsg = NULL;
            }
            NEXT_IMMEDIATE(CONNECTING_SERVER);
        }
        //handler_ret = -1;
        //return handler_ret;
        break;
    case 1153: // ER_NET_PACKET_TOO_LARGE
        proxy_warning("Error ER_NET_PACKET_TOO_LARGE during query on (%d,%s,%d): %d, %s\n", myconn->parent->myhgc->hid, myconn->parent->address, myconn->parent->port, myerr, mysql_error(myconn->mysql));
        break;
    default:
        break; // continue normally
}
> ```
>
> * MySQL_HostGroups_Manager.cpp
>  shunned 状态的 HostGroup 不会被清理。链接可用
>  ```cpp
>  bool MySQL_HostGroups_Manager::shun_and_killall(char *hostname, int port) {
>      ...
>    switch (mysrvc->status) {
>    case MYSQL_SERVER_STATUS_SHUNNED:
>       if (mysrvc->shunned_automatic==false) {
>            break;
>       }
>   case MYSQL_SERVER_STATUS_ONLINE:
>        if (mysrvc->status == MYSQL_SERVER_STATUS_ONLINE) {
>            ret = true;
>        }
>        mysrvc->status=MYSQL_SERVER_STATUS_SHUNNED;
>    case MYSQL_SERVER_STATUS_OFFLINE_SOFT:
>        mysrvc->shunned_automatic=true;
>        mysrvc->shunned_and_kill_all_connections=true;
>        mysrvc->ConnectionsFree->drop_all_connections();
>        break;
>    default:
>        break;
> }
> 
>  ```

### 重点线程

Admin thread

MySQL workers -> mysql-threads

Monitor thread -> monitor scheme 中的表即为监控项





## ProxySQL VS Other Proxy

https://proxysql.com/compare

https://www.percona.com/blog/2016/05/12/proxysql-versus-maxscale-for-oltp-ro-workloads/

https://proxysql.com/blog/point-select-benchmark

https://proxysql.com/blog/proxy-wars

https://proxysql.com/blog/proxysql-vs-maxscale-persistent-connection-response-time

https://www.percona.com/live/17/sessions/mysql-load-balancers-maxscale-proxysql-haproxy-mysql-router-nginx-close-look

## ProxySQL 性能测试（SSL ON/OFF）

| CPU | MEMORY | ROLE | THREADS NUM | RUNTIME |
| :--: | :--: | :--: | :--: | :--: |
| 4C | 8GB | MySQL Instance | 100 | 1H |
| 4C | 4GB | ProxySQL | 100 | 1H |

性能比：
(28695.79−25054.72)/28695.79*100% = 12.69%

### 开启 SSL 认证

```
    queries performed:
        read:                            63140070
        write:                           18039754
        other:                           9019925
        total:                           90199749
    transactions:                        4509920 (1252.72 per sec.)
    queries:                             90199749 (25054.72 per sec.)
    ignored errors:                      85     (0.02 per sec.)
    reconnects:                          0      (0.00 per sec.)

Throughput:
    events/s (eps):                      1252.7172
    time elapsed:                        3600.1102s
    total number of events:              4509920

Latency (ms):
         min:                                   16.64
         avg:                                   79.82
         max:                               104651.22
         95th percentile:                      130.13
         sum:                            359993465.21

Threads fairness:
    events (avg/stddev):           45099.2000/8625.57
    execution time (avg/stddev):   3599.9347/0.04

```


### 关闭 SSL 认证加密
```
SQL statistics:
    queries performed:
        read:                            72316454
        write:                           20661536
        other:                           10330817
        total:                           103308807
    transactions:                        5165356 (1434.77 per sec.)
    queries:                             103308807 (28695.79 per sec.)
    ignored errors:                      105    (0.03 per sec.)
    reconnects:                          0      (0.00 per sec.)

Throughput:
    events/s (eps):                      1434.7661
    time elapsed:                        3600.1380s
    total number of events:              5165356

Latency (ms):
         min:                                   14.91
         avg:                                   69.69
         max:                                 1962.88
         95th percentile:                      123.28
         sum:                            359992116.60

Threads fairness:
    events (avg/stddev):           51653.5600/1571.66
    execution time (avg/stddev):   3599.9212/0.03

```
