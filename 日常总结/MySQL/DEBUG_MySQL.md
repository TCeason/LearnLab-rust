使用 debug 模式 （debug 模式 编译 mysqld ）
---

1、debug: mysqld

> Session1: mysqld-debug --initialize-insecure --debug=d,info,error,query,general,where:O,/tmp/mysqld.trace
> Session2: tail -f /tmp/mysqld.trace

**Note:** 不做任何 defaults-file 设置的话，datadir 位于 $basedir/data

2、debug: mysql client

> Session1: mysql -uusername --debug=d:t:O,/tmp/client.trace
> Session2: tail -f /tmp/client.trace
