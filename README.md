# Artshell

MyShell, version 1.0.0-release (x86_64-pc-linux-gnu)
These shell commands are defined internally.

1)	bg <job> ——将被挂起的作业<job>转到后台。如果没有<job>参数，则默认将当前进程放到后台运行。

2)	cd <directory>  ——把当前默认目录改变为<directory>。如果没有<directory>参数，则显示主目录。

3)	clr  ——清屏。

4)	dir <directory>  ——列出目录<directory>的内容。如果没有<directory>参数，则显示当前目录内容。

5)	echo <comment>  ——在屏幕上显示<comment>并换行，多个空格和制表符将被缩减为一个空格。

6)	exec <command> ——执行命令<command>替换当前运行这个命令的进程。

7)	exit  ——退出shell。

8)	fg <job> ——将后台运行或挂起的作业<job>切换到前台运行。如果没有<job>参数，则默认将当前进程放到后台运行。

9)	help ——显示用户手册，即本文档。

10)	jobs <job> ——显示所有挂起的和后台进程的作业号及状态。如果没有列表，则显示当前进程的状态。

11)	pwd ——显示当前目录。

12)	set  ——列出所有的环境变量。

13)	test <expression> ——检测表达式<expression>返回true还是false。

14)	time ——显示当前时间。

15)	umask <mask> ——设定新创建文件或目录的访问特权。如果没有<mask>参数，则显示当前设置的掩码。

其他的命令行输入被解释为程序调用



，<mask>是位的掩码，用八进制表示。掩码位为1表示新创建的文件相应的访问特权应该被关闭