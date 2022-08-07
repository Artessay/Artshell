#include "myshell.h"

int main(int argc, char *argv[], char **env)
{
    // 开头输出判断程序是否正常开始，仅在调试时使用
    // puts("Welcome to MyShell ! \n");

    // 创建模型
    Console *model = new Console;
    if (model == nullptr)
    {
        fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), "Out of Space for Console model");
        return 1;
    }

    // 创建视图
    Display *view = new Display(model);
    if (view == nullptr)
    {
        fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), "Out of Space for Display view");
        return 1;
    }

    // 创建控制
    Executor *controller = new Executor(model, view);
    if (controller == nullptr)
    {
        fprintf(stderr, "\e[1;31m[ERROR]\e[0m %s: %s\n", strerror(errno), "Out of Space for Executor controller");
        return 1;
    }

    _SHELL_::shell_loop(model, view, controller, env);
    
    // 回收内存，MVC模型
    delete model;
    delete view;
    delete controller; 

    // 末尾输出判断程序是否正常结束，仅在调试时使用
    // puts("Bye~");

    return 0;
}