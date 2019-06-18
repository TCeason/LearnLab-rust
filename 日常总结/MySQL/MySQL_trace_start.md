MySQL (percona server 5.7.24-26)启动过程
---

MySQL 入口函数位于 sql/main.cc 。

```cpp
     17 /* 
     18   main() for mysqld.
     19   Calls mysqld_main() entry point exported by sql library.
     20 */
     21 extern int mysqld_main(int argc, char **argv);
     22 
     23 int main(int argc, char **argv)
     24 {
     25   return mysqld_main(argc, argv);
     26 }
```

下面这段函数是 mysqld_main 精简模式：

```cpp
int mysqld_main(int argc, char **argv)
  {
    pre_initialize_performance_schema(); // 函数位于 pfs_server.cc 初始化 performance
    my_init(); // 函数位于 my_init.c ，判断路径权限等
    load_defaults(MYSQL_CONFIG_NAME, load_default_groups, &argc, &argv); //加载配置文件
    init_pfs_instrument_array();  // 
    ho_error= handle_early_options(); // 初始化 mysqld 依赖项
    init_sql_statement_names();  // 初始化 sql 语句，status 值
    sys_var_init();   // 函数位于 set_var.cc ，初始化 参数值
    adjust_related_options(ulong *requested_open_files); // grant table, 设置 open_file_limit, max_conn, table_cache_size, table_def_size
    init_error_log();
    mysql_audit_initialize(); //函数位于 sql_adudit.cc 初始化审计。
    query_logger.init(); // safe_mutex 依赖于 初始化，需要 my_init() 执行。
    my_thread_attr_setstacksize(&connection_attrib,
                            my_thread_stack_size + guardize); //位于 my_thread.h,检查堆栈大小和溢出情况
    (user_info= check_user(mysqld_user)) // check user
    opt_bin_log && !(server_id_supplied) // 开启binlog 同时，需要有 server_id
    init_server_components() //初始化服务核心组件，mdl，partition，timer，query_cache, binlog，engine等
    init_server_auto_options() //初始化 server uuid。由于 innodb inited，所以，如果 system、redo 加密，那么 初始化的时候，是 empty uuid。但是，该函数完成后，uuid 存在。下面的函数会解决该问题。
    innodb_hton != NULL && innodb_hton->fix_tablespaces_empty_uuid // 具体位于 handler.h 该函解决上述问题
    global_sid_lock->wrlock();
    int gtid_ret= gtid_state->init();
    global_sid_lock->unlock(); // 初始化 gtid 后解锁
    if (opt_bin_log) {} // 初始化 gtid_executed 表内容，gtid_executed gtid_purged
    init_ssl() 
    network_init()
    /* If running with bootstrap, do not start replication. */
    if (!opt_bootstrap)
    {
      // Make @@slave_skip_errors show the nice human-readable value.
      set_slave_skip_errors(&opt_slave_skip_errors);

      /*
        init_slave() must be called after the thread keys are created.
      */
      if (server_id != 0)
        init_slave(); /* Ignoring errors while configuring replication. */
    }
    execute_ddl_log_recovery();  // recovery ddl
    start_handle_manager();
    DBUG_PRINT("info", ("Waiting for shutdown proceed"));
    int ret= 0;
  #ifdef _WIN32
    if (shutdown_thr_handle.handle) 
      ret= my_thread_join(&shutdown_thr_handle, NULL);
    shutdown_thr_handle.handle= NULL;
    if (0 != ret)
      sql_print_warning("Could not join shutdown thread. error:%d", ret);
  #else
    if (signal_thread_id.thread != 0)
      ret= my_thread_join(&signal_thread_id, NULL);
    signal_thread_id.thread= 0;
    if (0 != ret)
      sql_print_warning("Could not join signal_thread. error:%d", ret);
  #endif

    clean_up(1);
    mysqld_exit(MYSQLD_SUCCESS_EXIT);
  }
```

在 MySQL 启动过程中，需要找配置文件。这里调用了函数 load_defaults() ，该函数的作用就是 找到配置文件并从中解析参数key， value。
```cpp
int load_defaults(const char *conf_file, const char **groups, int *argc, char ***argv)                                      
{
  return my_load_defaults(conf_file, groups, argc, argv, &default_directories);
}  
可以看到 load_defaults() 函数调用了 my_load_defaults() ，两个函数均位于 mysys_ssl/my_default.cc my_load_defaults() 有这样一段调用，dirs= init_default_directories(&alloc)，在 init_default_directories 中指定了 config_file:

errors += add_directory(alloc, "/etc/", dirs);
errors += add_directory(alloc, "/etc/mysql/", dirs);
my_load_defaults() 简要 code

if ((dirs= init_default_directories(&alloc)) == NULL) //如果没有指定cnf 就要找到 默认 config_file


/* 
my_search_option_files() 调用 search_default_file_with_ext()函数，逐行判断 group 以及每个 group 参数和 参数值  。group_name 由 mysqld.cc 的 array load_default_groups[] 控制。
const char *load_default_groups[]= {
#ifdef WITH_NDBCLUSTER_STORAGE_ENGINE
"mysql_cluster",
#endif
"mysqld","server", MYSQL_BASE_VERSION, 0, 0};
其中，MYSQL_BASE_VERSION 是由 当前 mysqld 版本决定的。可以查看 mysql_version.h 
*/
if ((error= my_search_option_files(conf_file, argc, argv,
                                     &args_used, handle_default_option,
                                     (void *) &ctx, dirs, false, found_no_defaults))) 
```
