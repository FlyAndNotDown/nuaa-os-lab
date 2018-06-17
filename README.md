# NUAA 2018 os-tasks
2018年NUAA操作系统实践作业，作者161520311，欢迎参考、fork、点星星ヾ(◍°∇°◍)ﾉﾞ✨

# 使用说明
**所有代码均在unix系统上运行，请勿在windows上玩弄，MacOS上亲测能够运行，Linux应该也没问题，Clang和Gcc环境下均编译通过**

根目录下有三个大文件夹，分别为三种类型的题目：文件读写、多进程、多线程，每个大文件夹下有很多小文件夹，一个小文件夹对应一道题目，每个题目的文件夹下有一个与文件夹同名的源码文件和一个编译脚本，你只需要进入具体题目的文件夹然后运行我写的编译脚本即可产生.run格式的的可执行文件，然后enjoy即可☺

比如myecho:
```
// 从项目根目录开始
cd 1-fileio
cd 1-myecho
./build.sh
./myecho.run
```

如果无法执行编译脚本，尝试给编译脚本加上执行权限：
```
chmod 777 buils.sh
```

# 题目详情
参照Linuxmooc给出的操作系统编程练习题列表：
[Tasks - Linuxmooc](http://www.linuxmooc.com/task.html)
