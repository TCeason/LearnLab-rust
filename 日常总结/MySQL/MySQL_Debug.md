Trace MySQL Bug
---

当我们看到 error log 中出现下面的描述，一般是使用方法错误，或者真的遇到了 bug。这个时候，只需要根据 stack 信息进行 trace，就能进一步定位问题。

```
03:51:06 UTC - mysqld got signal 11 ;
This could be because you hit a bug. It is also possible that this binary
or one of the libraries it was linked against is corrupt, improperly built,
or misconfigured. This error can also be caused by malfunctioning hardware.
We will try our best to scrape up some info that will hopefully help
diagnose the problem, but since we have already crashed, 
something is definitely wrong and this may fail.
Please help us make Percona Server better by reporting any
bugs at https://bugs.percona.com/

key_buffer_size=8388608
read_buffer_size=131072
max_used_connections=0
max_threads=153
thread_count=0
connection_count=0
It is possible that mysqld could use up to 
key_buffer_size + (read_buffer_size + sort_buffer_size)*max_threads = 69061 K  bytes of memory
Hope that's ok; if not, decrease some variables in the equation.

Thread pointer: 0x0
Attempting backtrace. You can use the following information to find out
where mysqld died. If you see no messages after this, something went
terribly wrong...
stack_bottom = 0 thread_stack 0x40000
./bin/mysqld(my_print_stacktrace+0x3b)[0x8f7eab]
./bin/mysqld(handle_fatal_signal+0x49a)[0x66d55a]
/lib/x86_64-linux-gnu/libpthread.so.0(+0x11390)[0x7f6f44fc2390]
./bin/mysqld(_ZN9MYSQL_LOG17generate_new_nameEPcPKc+0x9d)[0x657c4d]
./bin/mysqld(_ZN9MYSQL_LOG26init_and_set_log_file_nameEPKcS1_13enum_log_type10cache_type+0x57)[0x657d07]
./bin/mysqld(_ZN13MYSQL_BIN_LOG11open_binlogEPKcS1_10cache_typembbbbP28Format_description_log_event+0x7b)[0x89a22b]
./bin/mysqld(_Z11mysqld_mainiPPc+0x1bab)[0x5acbfb]
/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf0)[0x7f6f44366830]
./bin/mysqld(_start+0x29)[0x59e659]
```


以上述 stacktrace 为例进行排查。
```
mysqld_path=$(which mysqld)

nm  -n $mysqld_path > /tmp/mysqld.sym
```


我将上述log存到了 /tmp/mysqld.stack 中

> resolve_stack_dump -s /tmp/mysqld.sym -n /tmp/mysqld.stack  # resolve_stack_dump 位于 MySQL basedir/bin

当解析完毕后，我们可以看到关键部分已经发生了变化，更具体的打印了函数名：

```
03:51:06 UTC - mysqld got signal 11 ;
This could be because you hit a bug. It is also possible that this binary...
Thread pointer: 0x0
Attempting backtrace. You can use the following information to find out
where mysqld died. If you see no messages after this, something went
terribly wrong...
stack_bottom = 0 thread_stack 0x40000
0x8f7eab my_print_stacktrace + 59
0x66d55a handle_fatal_signal + 1178
0x7f6f44fc2390 _end + 1136550664
0x657c4d _ZN9MYSQL_LOG17generate_new_nameEPcPKc + 157
0x657d07 _ZN9MYSQL_LOG26init_and_set_log_file_nameEPKcS1_13enum_log_type10cache_type + 87
0x89a22b _ZN13MYSQL_BIN_LOG11open_binlogEPKcS1_10cache_typembbbbP28Format_description_log_event + 123
0x5acbfb _Z11mysqld_mainiPPc + 7083
0x7f6f44366830 _end + 1123592104
0x59e659 _start + 41
```

如果想把他转换成熟悉的代码只需要稍稍改动上面的解析命令：

> resolve_stack_dump -s /tmp/mysqld.sym -n /tmp/mysqld.stack | c++filt

```
Thread pointer: 0x0
Attempting backtrace. You can use the following information to find out
where mysqld died. If you see no messages after this, something went
terribly wrong...
stack_bottom = 0 thread_stack 0x40000
0x8f7eab my_print_stacktrace + 59
0x66d55a handle_fatal_signal + 1178
0x7f6f44fc2390 _end + 1136550664
0x657c4d MYSQL_LOG::generate_new_name(char*, char const*) + 157
0x657d07 MYSQL_LOG::init_and_set_log_file_name(char const*, char const*, enum_log_type, cache_type) + 87
0x89a22b MYSQL_BIN_LOG::open_binlog(char const*, char const*, cache_type, unsigned long, bool, bool, bool, bool, Format_description_log_event*) + 123
0x5acbfb mysqld_main(int, char**) + 7083
0x7f6f44366830 _end + 1123592104
0x59e659 _start + 41
```
接下来去排查相关代码即可。
