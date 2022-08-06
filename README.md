# Artshell

MyShell, version 0.0.1-release (x86_64-pc-linux-gnu)
These shell commands are defined internally.

    1)	cd <directory>  ——把当前默认目录改变为<directory>。如果没有<directory>参数，则显示当前目录。如该目录不存在，会出现合适的错误信息。这个命令也可以改变PWD 环境变量。
    2)	pwd ——显示当前目录。
    3)	time ——显示当前时间
    4)	clr  ——清屏。
    5)	dir <directory>  ——列出目录<directory>的内容。
    6)	set  ——列出所有的环境变量。
    7)	echo <comment>  ——在屏幕上显示<comment>并换行（多个空格和制表符可能被缩减为一个空格）。
    8)	help ——显示用户手册，并且使用more 命令过滤。
    9)	exit  ——退出shell。
    其他的命令行输入被解释为程序调用