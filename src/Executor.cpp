/**
 * @file Executor.cpp
 * @author 邱日宏 (3200105842@zju.edu.cn)
 * @brief 
 * @version 0.1
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "common.h"
#include "myshell.h"
#include "Console.h"
#include "Display.h"
#include "Executor.h"
#include "ProcessManager.h"

#include <vector>
#include <sstream>

#include <fcntl.h>
#include <assert.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

/** 测试终端联系 */
static inline bool test_tty(const char * file_name);

/** 定义命令字符串数组 */
static const char* OperandArray[] = 
{
    "bg", "cd", "clr", "dir", "echo", "exec", "exit", "fg",
    "help", "jobs", "myshell", "pwd", "set", "test", "time", "umask"
};

Executor::Executor(Console *model, Display *view)
: console_(model), display_(view)
{
    assert(console_ != nullptr);
    assert(display_ != nullptr);

    /** 定义函数指针数组 */
    int i = 0;

    FunctionArray[i] = &Executor::execute_bg;       ++i;
    FunctionArray[i] = &Executor::execute_cd;       ++i;
    FunctionArray[i] = &Executor::execute_clr;      ++i;
    FunctionArray[i] = &Executor::execute_dir;      ++i;
    FunctionArray[i] = &Executor::execute_echo;     ++i;
    FunctionArray[i] = &Executor::execute_exec;     ++i;
    FunctionArray[i] = &Executor::execute_exit;     ++i;
    FunctionArray[i] = &Executor::execute_fg;       ++i;

    FunctionArray[i] = &Executor::execute_help;     ++i;
    FunctionArray[i] = &Executor::execute_jobs;     ++i;
    FunctionArray[i] = &Executor::execute_myshell;  ++i;
    FunctionArray[i] = &Executor::execute_pwd;      ++i;
    FunctionArray[i] = &Executor::execute_set;      ++i;
    FunctionArray[i] = &Executor::execute_test;     ++i;
    FunctionArray[i] = &Executor::execute_time;     ++i;
    FunctionArray[i] = &Executor::execute_umask;    ++i;

    return;
}

Executor::~Executor()
{
}

sh_err_t Executor::execute(const int argc, char * const argv[], char * const env[]) const
{
    if (argc == 0)
        return SH_SUCCESS;  // 没有输入命令则无需处理
    else if (argv == nullptr || argv[0] == nullptr)
    {
        assert(false);
        return SH_FAILED;   // 解析可能产生了错误
    }

    /* 挂起命令处理 */
    int& argc_ = const_cast<int&>(argc);
    if (strcmp(argv[argc - 1], "&") == 0)    // 后台挂起
    {
        --argc_;
        if (argc == 0)  // 参数错误
            return SH_ARGS;

        pid_t pid;
        pid_t child_pid = getpid(); // 子进程pid

        if ((pid = fork()) < 0)
        { 
            /* 错误处理 */
            throw "Fork Error, 错误终止";
        }
        else if (pid == 0)
        {
            /* 子进程 */  
            setenv("parent", console_->shell_path_env, 1);  // 设置调用子进程的父进程

            #ifdef _DEBUG_
            printf("child pid: %d\n", console_->process_id);
            #endif

            setpgid(0, 0);

            char **&argv_ = const_cast<char **&>(argv);
            argv_[argc] = NULL;

            #ifdef _DEBUG_
            Argument_Display(argc, argv);
            #endif

            // 执行命令
            shell_function(argc, argv, env);

            // 执行完成时退出
            return SH_EXIT;
        }
        else
        {
            /* 父进程 */
            #ifdef _DEBUG_
            printf("parent pid: %d\n", pid);
            #endif

            // 添加进程列表
            char **&argv_ = const_cast<char **&>(argv);
            argv_[argc] = NULL;
            unsigned int jobid = console_->AddJob(child_pid, Running, argc_, argv_);
            console_->process_id = pid;
            
            // 打印当前进程
            char buffer[32];
            snprintf(buffer, 32, "[%u] %d\n", jobid, pid);
            if (write(console_->output_std_fd, buffer, strlen(buffer)));

            

            return SH_SUCCESS;
        }
    }
    
    return shell_function(argc, argv, env);
}

sh_err_t Executor::shell_function(const int argc, char * const argv[], char * const env[]) const
{
    const char *op = argv[0];

#ifdef _DEBUG_
    Argument_Display(argc, argv);

    // 以下命令不要求实现，仅供练习使用
    if (strcmp(op, "date") == 0)
    {
        return execute_date(argc, argv, env);
    }
    else if (strcmp(op, "clear") == 0)
    {
        return execute_clear(argc, argv, env);
    }
    else if (strcmp(op, "env") == 0)
    {
        return execute_env(argc, argv, env);
    }
    else if (strcmp(op, "who") == 0)
    {
        return execute_who(argc, argv, env);
    }
    else if (strcmp(op, "mkdir") == 0)
    {
        return execute_mkdir(argc, argv, env);
    }
    else if (strcmp(op, "rmdir") == 0)
    {
        return execute_rmdir(argc, argv, env);
    }
#endif

    /** 二分查找匹配内部命令 */
    int index = Binary_Search(0, sizeof(OperandArray)/sizeof(OperandArray[0]), op, OperandArray, strcmp);
#ifdef _DEBUG_
    printf("index: %d op: %s\n", index, OperandArray[index>=0?index:0]);
#endif

    if (index >= 0 && index < FunctionNumber)   // 找到了
    {
        MemFuncPtr FunctionPointer = FunctionArray[index];  // 找到对应的函数指针
        return (*this.*FunctionPointer)(argc, argv, env);   // 执行内部函数
    }
    

    // 其他的命令行输入被解释为程序调用，
    // shell 创建并执行这个程序，并作为自己的子进程
    pid_t pid = getpid(); // 获取当前进程id，用于处理父进程行为
    if ((pid = vfork()) < 0)
    { 
        /* 错误处理 */
        throw "Fork Error, 错误终止";
    }
    else if (pid == 0)
    {
        /* 子进程 */  
        setenv("parent", console_->shell_path_env, 1);  // 设置调用子进程的父进程
        int status_code = execvp(argv[0], argv);        // 在子进程之中执行

        if (status_code == -1)
        {
            throw "Execvp Error, terminated incorrectly";
        }

        return SH_UNDEFINED; // 未识别的命令
    }
    else
    {
        /* 父进程 */
        wait(NULL); // 等待子进程结束后再继续执行，保证执行顺序不混乱
        return SH_SUCCESS;
    }

    return SH_FAILED;
}

sh_err_t Executor::execute_cd(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "cd")==0 && "unexpected node type");

    std::string path;
    if (argc == 1)
    {
        // 默认无参数时为主目录
        path = console_->home;
    }
    else if (argc == 2)
    {
        path = argv[1];
        
        #ifdef _DEBUG_
        printf("char: %c %d\n", path[0], (path[0] == '~'));
        #endif

        if (path[0] == '~') // 对于~目录需要特殊判断
        {
            // 将~替换为主目录
            path.replace(0, 1, console_->home);
        }

        #ifdef _DEBUG_
        printf("Argv: %s\nHome: %s\nPath: %s\n", argv[1], console_->home, path.c_str());
        #endif
    }
    else
    {
        return SH_ARGS; // 参数错误
    }

    // 更改目录
    int ret = chdir(path.c_str());
    if (ret != 0)   // 打开目录异常
    {
        throw ((std::string)"cd: 无法打开路径 " + path);
    }

    // 重新设置控制台环境与系统环境变量
    if (getcwd(console_->current_working_dictionary, BUFFER_SIZE) != nullptr )
        setenv("PWD", console_->current_working_dictionary, 1);
    else
        throw "get cwd error";

    return SH_SUCCESS;
}

sh_err_t Executor::execute_pwd(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "pwd")==0 && "unexpected node type");
    display_->message(console_->current_working_dictionary);
    display_->message("\n");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_time(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "time")==0 && "unexpected node type");

    // 相当于env命令，从兼容性的角度出发，这里选择调用env命令以兼容Linux用户需求以及后续开发
    return execute_date(argc, argv, env);
}

sh_err_t Executor::execute_clr(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "clr")==0 && "unexpected node type");
    
    // 相当于clear命令，从兼容性的角度出发，这里选择调用env命令以兼容Linux用户需求以及后续开发
    return execute_clear(argc, argv, env);
}

sh_err_t Executor::execute_dir(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "dir")==0 && "unexpected node type");
    
    std::string real_path;
    if (argc == 1)
    {
        // 默认无参数时为当前目录
        real_path = console_->current_working_dictionary;
    }
    else if (argc == 2)
    {
        real_path = argv[1];
        if (real_path[0] == '~') // 对于~目录需要特殊判断
        {
            // 将~替换为主目录
            real_path.replace(0, 1, console_->home);
        }
    }
    else
    {
        return SH_ARGS; // 参数错误
    }
    
    int ret;                // 用于接受返回值
    DIR *direction_pointer; // 目录指针
    if ((direction_pointer = opendir(real_path.c_str())) == NULL)
    {
        throw ((std::string)"dir: 无法打开路径 " + real_path);
    }

    // 临时将进程目录调整为指定目录
    ret = chdir(real_path.c_str());
    if (ret != 0)   // 打开目录异常
    {
        throw ((std::string)"dir: 无法打开路径 " + real_path);
    }

    struct dirent *entry;   // 目录内容
    while ((entry = readdir(direction_pointer)) != NULL)
    {
        struct stat stat_buffer;    // 存储stat结构
        lstat(entry->d_name, &stat_buffer); // 根据文件名获得文件stat结构

        char buffer[BUFFER_SIZE];
        if (S_ISDIR(stat_buffer.st_mode))   // 检测该数据项是否是一个目录
        {
            // 数据项是一个目录

            if (strcmp(".", entry->d_name) == 0 ||
                strcmp("..", entry->d_name) == 0)
            {
                // 如果是.或者..目录，则不显示
                continue;
            }

            // 目录用蓝色显示
            snprintf(buffer, BUFFER_SIZE, "\033[34m%s\033[0m  ", entry->d_name);
            if (console_->redirect_output == false) 
                display_->message(buffer);
            else
            {
                display_->message(entry->d_name);
                display_->message("  ");
            }
        }
        else
        {
            // 普通文件
            switch (entry->d_type)
            {
                case DT_UNKNOWN:    // 目录文件未知文件用红色
                    snprintf(buffer, BUFFER_SIZE, "\033[31m%s\033[0m  ", entry->d_name);
                    break;
                
                case DT_REG:        // 目录文件普通文件用白色
                    if (access(entry->d_name, X_OK) == 0)    // 可执行文件除外，用绿色
                        snprintf(buffer, BUFFER_SIZE, "\033[32m%s\033[0m  ", entry->d_name);
                    else
                        snprintf(buffer, BUFFER_SIZE, "\033[37m%s\033[0m  ", entry->d_name);
                    break;

                default:            // 其他文件用青色
                    snprintf(buffer, BUFFER_SIZE, "\033[36m%s\033[0m  ", entry->d_name);
                    break;
            }
            if (console_->redirect_output == false) 
                display_->message(buffer);
            else
            {
                display_->message(entry->d_name);
                display_->message("  ");
            }
        }
    }
    display_->message("\n");

    // 结束时将目录更改回当前目录
    ret = chdir(console_->current_working_dictionary);
    if (ret != 0)   // 打开目录异常
    {
        throw ((std::string)"dir: 无法打开路径 " + real_path);
    }

    ret = closedir(direction_pointer);
    if (ret == -1)  // 关闭目录流异常
    {
        throw "dir: 关闭目录流异常";
    }

    return SH_SUCCESS;
}

sh_err_t Executor::execute_set(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "set")==0 && "unexpected node type");
    
    // 相当于env命令，从兼容性的角度出发，这里选择调用env命令以兼容Linux用户需求
    return execute_env(argc, argv, env);
}

sh_err_t Executor::execute_echo(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "echo")==0 && "unexpected node type");

    for (int i = 1; i < argc; ++i)
    {
        // 多个空格和制表符被缩减为一个空格
        if (i > 1)
            display_->message(" ");

        display_->message(argv[i]);
    }
    display_->message("\n");

    return SH_SUCCESS;
}

sh_err_t Executor::execute_help(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "help")==0 && "unexpected node type");

    FILE* fp = fopen("README.md", "r");
    if (fp == nullptr)
    {
        return SH_FAILED;
    }

    char buffer[BUFFER_SIZE*2];
    if (fgets(buffer, BUFFER_SIZE, fp) == nullptr)
        return SH_FAILED;    // 忽略首行

    size_t size = fread(buffer, 1, BUFFER_SIZE*2, fp);
    if (size < 0)
    {
        return SH_FAILED;
    }

    if (fclose(fp) == -1)
    {
        throw std::exception();
    }

    display_->message(buffer);
    display_->message("\n");

    return SH_SUCCESS;
}

sh_err_t Executor::execute_exit(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "exit")==0 && "unexpected node type");
    return SH_EXIT;
}

sh_err_t Executor::execute_date(const int argc, char * const argv[], char * const env[]) const
{
    // assert(strcmp(argv[0], "date")==0 && "unexpected node type");

    //获取当前时间
    time_t t = time(NULL);
    struct tm *ptr = localtime(&t);

    //生成返回信息
    // char weekday[16], month[16];
    char date[256];
    // strftime(weekday, 16, "%A", ptr);
    // strftime(month, 16, "%B", ptr);
    strftime(date, 256, "%c", ptr);

    // char buffer[BUFFER_SIZE];
    // snprintf(buffer, BUFFER_SIZE, "%s %s %s\n", weekday, month, date);

    // display_->message(buffer);
    display_->message(date);
    display_->message("\n");

    return SH_SUCCESS;
}

sh_err_t Executor::execute_clear(const int argc, char * const argv[], char * const env[]) const
{
    display_->message("\x1b[H\x1b[2J");    // 输出清屏控制 \x1b[H\x1b[2J

    return SH_SUCCESS;
}

sh_err_t Executor::execute_env(const int argc, char * const argv[], char * const env[]) const
{
    extern char **environ; //env variables
    char ***update_env = const_cast<char ***>(&env);
    *update_env = environ;

    while(*env)
    {
        char buffer[BUFFER_SIZE];
        snprintf(buffer, BUFFER_SIZE, "%s\n", *env++);
        display_->message(buffer);
    }

    return SH_SUCCESS;
}

sh_err_t Executor::execute_who(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "who")==0 && "unexpected node type");
    display_->message(console_->user_name);
    display_->message("\n");
    return SH_SUCCESS;
}

sh_err_t Executor::execute_mkdir(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "mkdir")==0 && "unexpected node type");
    
    const char * path = argv[1];
    if (mkdir(path, S_IRWXU) == 0)
    {
        return SH_SUCCESS;
    }
    else
    {
        return SH_FAILED;
    }

}

sh_err_t Executor::execute_rmdir(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "rmdir")==0 && "unexpected node type");

    if (rmdir(argv[1]) == 0)
    {
        return SH_SUCCESS;
    }
    else
    {
        return SH_FAILED;
    }
}

sh_err_t Executor::execute_bg(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "bg")==0 && "unexpected node type");

    return SH_SUCCESS;
}

sh_err_t Executor::execute_fg(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "fg")==0 && "unexpected node type");

    return SH_SUCCESS;
}

sh_err_t Executor::execute_jobs(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "jobs")==0 && "unexpected node type");

    console_->ConsoleJobList();

    return SH_SUCCESS;
}

sh_err_t Executor::execute_exec(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "exec")==0 && "unexpected node type");

    // 只有一个参数时不做处理
    if (argc == 1)
        return SH_SUCCESS;

    int status_code = execvp(argv[1], argv+1);        // 在子进程之中执行

    if (status_code == -1)
    {
        throw "Execvp Error, terminated incorrectly";
    }

    return SH_UNDEFINED; // 未识别的命令
}

sh_err_t Executor::execute_test(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "test")==0 && "unexpected node type");

    bool ret = false;
    if (argc == 1)  // 空字符串，false
    {
        ret = false;
    } 
    else if (argc == 2) // 单目运算
    {
        if (strcmp(argv[1], "!") == 0 || strcmp(argv[1], "-z") == 0)
            ret = true;
        else
            ret = false;
    }
    else 
    {
        if (strcmp(argv[1], "!"))   // 第二个参数不是！
        {
            if (argc == 3 || argc == 4) // 双目运算
            {
                // 文件测试 与 部分字符串测试
                ret = Executor::test_file_state(argc, argv)
                    | Executor::test_number_compare(argc, argv)
                    | Executor::test_string_compare(argc, argv);
            }
            else
            {
                return SH_ARGS;
            }
        }
    }

    if (console_->GetOutputRedirect() == false) // 如果是在终端显示就产生正误提示
    {
        if (ret)
            display_->message("true\n");
        else
            display_->message("false\n");
    }

    return SH_SUCCESS;
}

sh_err_t Executor::execute_umask(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "umask")==0 && "unexpected node type");

    if (argc == 1)
    {
        // 没有参数，显示当前掩码
        char buffer[16];
        snprintf(buffer, 16, "%04o\n", console_->umask_);   // 以八进制显示，0补齐
        display_->message(buffer);
    }
    else if (argc == 2) // 有一个输入的参数
    {
        // 使用函数模板实现进制转换
        console_->umask_ = String_to_Number<mode_t>(argv[1]);
        
        if (argv[1][0] == '0')
        {
            if (strlen(argv[1]) >= 2 && argv[1][1] == 'x')  // 十六进制
                console_->umask_ = Hexadecimal_to_Decimal(console_->umask_);
            else                                            // 八进制
                console_->umask_ = Octal_to_Decimal(console_->umask_);
        }
        
        #ifdef _DEBUG_
        printf("mask: %04u %04o\n", console_->umask_, console_->umask_);
        #endif
        umask(console_->umask_);
    }
    else
    {
        return SH_ARGS; // 参数错误
    }

    return SH_SUCCESS;
}

sh_err_t Executor::execute_myshell(const int argc, char * const argv[], char * const env[]) const
{
    assert(strcmp(argv[0], "myshell")==0 && "unexpected node type");

    std::vector<std::string> FileList;
    if (argc == 1)
    {
        /*如果shell 被调用时没有使用参数，
        它会在屏幕上显示提示符请求用户输入*/
        while (1)   // 循环直到用户有输入
        {
            display_->prompt();

            int len;
            char input[BUFFER_SIZE];
            len = display_->InputCommand(input, BUFFER_SIZE);
            
            if (len == 1 || len < 0)
                continue;   // 非有效输入
            if (len == 0)
                return SH_EXIT; // EOF

            #ifdef _DEBUG_
            printf("len: %d\n", len);
            #endif
            input[len-1] = '\0'; // 去掉末尾的\n

            int& argc_ = const_cast<int&>(argc);        // 引用
            // char **argv_ = const_cast<char **>(argv);   // 指针

            std::istringstream line(input); // 字符串流
            std::string word;               // 分割出的字符串

            while (std::getline(line, word, ' '))
            {
                word = String_Trim(word);   // 裁剪
                if (word == "")
                    continue;
                
                ++argc_;
                FileList.emplace_back(word);  // 需要深拷贝
            }

            if (argc == 1)
                continue;   // 未能读到有效输入

            #ifdef _DEBUG_
            Argument_Display(argc, argv);   // 调试
            #endif

            break;
        }
    }
    else
    {
        for (int i = 1; i < argc; ++i) // 顺序执行
            FileList.push_back(argv[i]);    // 将参数加入向量列表
    }

    assert(argc > 1);   // 判断

    int input_fd = console_->input_file_descriptor; // 存储当前控制台的输入fd
    for (std::string File : FileList)
    {
        try
        {
            int fd = open(File.c_str(), O_RDONLY);   // 打开文件
            if (fd < 0) // 打开错误处理
            {
                throw std::exception();
            }

            #ifdef _DEBUG_
            fprintf(stdout, "FD: %d Input: %d Output: %d\n", fd, console_->input_file_descriptor, console_->output_file_descriptor);
            #endif

            console_->input_file_descriptor = fd;   // 更改输入
            
            // 执行循环
            SHELL::shell_loop(console_, display_, const_cast<Executor *>(this), const_cast<char **>(env));

            int state_code = close(fd); // 关闭文件
            if (state_code != 0)    // 关闭错误处理
            {
                throw std::exception();
            }
        }
        catch(...)
        {
            puts("every thing");
            std::string msg = "\e[1;31m[ERROR]\e[0m";
            msg = msg + "myshell" + ": (" + File + ") " + strerror(errno) + "\n";
            display_->message(msg.c_str());
        }
        
    }

    console_->input_file_descriptor = input_fd; // 恢复控制台的input fd

    return SH_SUCCESS;
}

static inline bool test_tty(const char * file_name)
{
    try
    {
        int fd = open(file_name, S_IREAD);  // 打开文件获取文件描述符
        bool tty = isatty(fd);              // 判断是否为终端
        close(fd);                          // 关闭文件
        return tty;
    }
    catch(...)
    {
        return false;                       // 如果有任何中断则返回false
    }
}

bool Executor::test_file_state(const int argc, const char * const argv[])
{
    assert(argc == 3 || argc == 4);

    if (argc == 3)
    {
        struct stat file_stat;

        if (lstat(argv[2], &file_stat) < 0) // 文件不存在
        {
            return false;   // 不存在一定是false
        }

        // 对文件测试参数进行判断
        switch (String_Hash(argv[1]))   // 为了形式上的优雅，使用switch语句
        {
            /* 存在性判断 */
            case String_Hash("-e"):  // 存在判断
                return true;
            
            /* 文件类型判断 */
            case String_Hash("-f"):  // 普通文件
                return S_ISREG(file_stat.st_mode);
            
            case String_Hash("-d"):  // 目录文件
                return S_ISDIR(file_stat.st_mode);
            
            case String_Hash("-c"):  // 字符特殊文件
                return S_ISCHR(file_stat.st_mode);
            
            case String_Hash("-b"):  // 块特殊文件
                return S_ISBLK(file_stat.st_mode);
            
            case String_Hash("-p"):  // 管道文件
                return S_ISFIFO(file_stat.st_mode);
            
            case String_Hash("-L"):  // 符号链接文件
                return S_ISLNK(file_stat.st_mode);
            
            case String_Hash("-S"):  // 套接字文件
                return S_ISSOCK(file_stat.st_mode);
            
            /* 文件权限判断 */
            case String_Hash("-r"):  // 只读文件
                return access(argv[1], R_OK);
            
            case String_Hash("-w"):  // 可写文件
                return access(argv[1], W_OK);
            
            case String_Hash("-x"):  // 可执行文件
                return access(argv[1], X_OK);

            case String_Hash("-O"):  // 所有者文件
                return file_stat.st_uid == getuid();
            
            case String_Hash("-G"):  // 组 文件
                return file_stat.st_gid == getgid();
            
            /* 文件属性判断 */
            case String_Hash("-u"):  // 用户位属性SUID
                return S_ISUID & file_stat.st_mode;
            
            case String_Hash("-g"):  // 组位属性GUID
                return S_ISGID & file_stat.st_mode;
            
            case String_Hash("-k"):  // Sticky bit属性
                return S_ISVTX & file_stat.st_mode;
            
            case String_Hash("-s"):  // 文件长度非0
                return file_stat.st_size > 0;
            
            case String_Hash("-t"):  // 文件描述符联系终端
                return test_tty(argv[2]);
            
            /* 其他情况返回错误 */
            default:
                return false;
        }
    }
    else
    {
        struct stat file_stat1, file_stat2;

        if (lstat(argv[1], &file_stat1) < 0) // 文件不存在
        {
            return false;   // 不存在一定是false
        }
        if (lstat(argv[3], &file_stat2) < 0) // 文件不存在
        {
            return false;   // 不存在一定是false
        }

        // 对文件测试参数进行判断
        switch (String_Hash(argv[2]))   // 为了形式上的优雅，使用switch语句
        {
            case String_Hash("-nt"):    // 判断file1是否比file2新
                return test_timespec_newer(file_stat1.st_mtim, file_stat2.st_mtim);
            
            case String_Hash("-ot"):    // 判断file1是否比file2旧
                return test_timespec_older(file_stat1.st_mtim, file_stat2.st_mtim);
            
            case String_Hash("-ef"):    // 判断file1与file2是否为同一个文件
                return file_stat1.st_ino == file_stat2.st_ino;
            
            default:                    /* 其他情况返回错误 */
                return false;
        }
    }
}

bool Executor::test_number_compare(const int argc, const char * const argv[])
{
    if (argc != 4)
        return false;

    
    int number1 = String_to_Number<int>(argv[1]);
    int number2 = String_to_Number<int>(argv[2]);

    // 对整数测试参数进行判断
    switch (String_Hash(argv[2]))   // 为了形式上的优雅，使用switch语句
    {
        case String_Hash("-eq"):
        case String_Hash("=="):     // 判断number1是否与number2相等
            return number1 == number2;
        
        case String_Hash("-ne"):
        case String_Hash("!="):     // 判断number1是否与number2不相等
            return number1 != number2;
        
        case String_Hash("-ge"):
        case String_Hash(">="):     // 判断number1是否大于等于number2
            return number1 >= number2;
        
        case String_Hash("-gt"):
        case String_Hash(">"):      // 判断number1是否大于number2
            return number1 > number2;
        
        case String_Hash("-le"):
        case String_Hash("<="):     // 判断number1是否小于等于number2
            return number1 <= number2;
        
        case String_Hash("-lt"):
        case String_Hash("<"):      // 判断number1是否小于number2
            return number1 < number2;
        
        default:                    /* 其他情况返回错误 */
            return false;
    }
    
}

bool Executor::test_string_compare(const int argc, const char * const argv[])
{
    assert(argc == 3 || argc == 4);

    if (argc == 3)
    {
        // 对字符串测试参数进行判断
        switch (String_Hash(argv[1]))   // 为了形式上的优雅，使用switch语句
        {
            /* 存在性判断 */
            case String_Hash("-n"):  // 存在判断
                return true;
                        
            /* 其他情况返回错误 */
            default:
                return false;
        }
    }
    else
    {
        // 对字符串测试参数进行判断
        switch (String_Hash(argv[2]))   // 为了形式上的优雅，使用switch语句
        {
            case String_Hash("="):     // 判断string1是否与string2相等
                return !strcmp(argv[1], argv[3]);
            
            case String_Hash("!="):     // 判断string1是否与string2不相等
                return strcmp(argv[1], argv[3]);
            
            case String_Hash("\\>"):      // 判断string1是否大于string2
                return strcmp(argv[1], argv[3]) > 0;
            
            case String_Hash("\\<"):      // 判断string1是否小于string2
                return strcmp(argv[1], argv[3]) < 0;
            
            default:
                return false;
        }
    }
}