#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_LEN 1024

#define RE_NO 0
#define RE_OUT 1
#define RE_IN 2
#define RE_ALL 3

// 读入一行进入缓冲区
int readLine(char *, int);
// 清空缓冲区
void bufferClear(char *, int);
// 伪造system函数
int mysys(char *);
// 判断是否要重定向
int needRedirect(char *);
// 执行指令
void doSomething(char *);
// 处理字符串中的输出重定向
char *dealReOutStr(char *);
// 处理字符串中的输入重定向
char *dealReInStr(char *);
// 寻找特定字符在字符串中的位置
int findCharInStr(char *, char);
// 删除字符串的一部分
void deleteFromStr(char *, int, int);

int main(int argc, char *argv[]) {
    char buffer[BUFFER_LEN];

    // 开始主循环
    while (1) {
        // 输出shell标识符
        printf("$");
        // 清空缓冲区
        bufferClear(buffer, BUFFER_LEN);
        // 读取一行，如果溢出了就报错
        if (readLine(buffer, BUFFER_LEN)) {
            // 执行指令
            doSomething(buffer);
        } else {
            printf("You can't input so much char at one time!\n");
        }
    }

    return 0;
}

int readLine(char *buffer, int length) {
    char c;
    int count = 0;
    c = getchar();
    while (c != '\n') {
        count++;
        if (count < length - 1) {
            buffer[count - 1] = c;
        } else return 0;
        c = getchar();
    }
    buffer[count] = '\0';
    return 1;
}

void bufferClear(char *buffer, int length) {
    for (int i = 0; i < length; i++) buffer[i] = '\0';
}

int needRedirect(char *str) {
    // 寻找 < 符号和 > 符号
    int findLeft = 0;
    int findRight = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '<') findLeft = 1;
        if (str[i] == '>') findRight = 1;
    }
    if (!findLeft && findRight) return RE_OUT;
    if (findLeft && !findRight) return RE_IN;
    if (findLeft && findRight) return RE_ALL;
    return RE_NO;
}

void doSomething(char *str) {
    // 缓冲区
    char buffer[BUFFER_LEN];
    char str2[BUFFER_LEN];
    strcpy(str2, str);
    char path[BUFFER_LEN];
    char *p = NULL;
    char *name = NULL;
    // 文件描述符
    int fd;
    // 子进程
    pid_t pid;

    if (strcmp(str, "")) {
        // 判断指令类型
        int type = needRedirect(str);
        // 根据指令情况进行处理
        switch (type) {
            case RE_NO:
                // 拷贝输入
                strcpy(buffer, str);
                // 分割参数
                p = strtok(buffer, " ");
                // 根据指令的不同做不同的事情
                if (p) {
                    if (!strcmp(p, "cd")) {
                        p = strtok(NULL, "");
                        if (chdir(p) < 0) printf("no such directory\n");
                    } else if (!strcmp(p, "pwd")) {
                        getcwd(path, BUFFER_LEN);
                        printf("%s\n", path);
                    } else if (!strcmp(p, "exit")) {
                        // 退出程序
                        exit(0);
                    } else {
                        mysys(str);
                    }
                }
                break;
            case RE_ALL:
            case RE_OUT:
                // 获取重定向文件名
                name = dealReOutStr(str2);
                if (name) {
                    pid = fork();
                    if (pid == 0) {
                        // 执行重定向
                        fd = open(name, O_CREAT | O_RDWR, 0666);
                        dup2(fd, 1);
                        close(fd);
                        // 递归
                        doSomething(str2);
                        exit(0);
                    } else waitpid(pid, NULL, 0);
                }
                break;
            case RE_IN:
                name = dealReInStr(str2);
                if (name) {
                    pid = fork();
                    if (pid == 0) {
                        fd = open(name, O_CREAT | O_RDWR, 0666);
                        dup2(fd, 0);
                        close(fd);
                        doSomething(str2);
                        exit(0);
                    } else waitpid(pid, NULL, 0);
                }
                break;
        }
    }
}

char *dealReOutStr(char *str) {
    char *name = (char *) malloc(sizeof(char) * BUFFER_LEN);
    int end;

    int symbol = findCharInStr(str, '>');
    if (symbol < 0 || symbol == strlen(str) - 1) return NULL;

    // 寻找 > 符号后面的第一个单词
    if (str[symbol + 1] == ' ') {
        end = symbol + 2;
        for (int i = symbol + 2; i < strlen(str); i++) {
            if (str[i] == ' ') break;
            end++;
        }
        // 拷贝到name
        strncpy(name, &str[symbol + 2], end - (symbol + 2));
        // 从原来的指令中删除重定向
        deleteFromStr(str, symbol, end);
    } else {
        end = symbol + 1;
        for (int i = symbol + 1; i < strlen(str); i++) {
            if (str[i] == ' ') break;
            end++;
        }
        strncpy(name, &str[symbol + 1], end - (symbol + 1));
        deleteFromStr(str, symbol, end);
    }
    return name;
}

char *dealReInStr(char *str) {
    char *name = (char *) malloc(sizeof(char) * BUFFER_LEN);
    int end;

    int symbol = findCharInStr(str, '<');
    if (symbol < 0 || symbol == strlen(str) - 1) return NULL;

    // 寻找 < 符号后面的第一个单词
    if (str[symbol + 1] == ' ') {
        end = symbol + 2;
        for (int i = symbol + 2; i < strlen(str); i++) {
            if (str[i] == ' ') break;
            end++;
        }
        // 拷贝到name
        strncpy(name, &str[symbol + 2], end - (symbol + 2));
        // 从原来的指令中删除重定向
        deleteFromStr(str, symbol, end);
    } else {
        end = symbol + 1;
        for (int i = symbol + 1; i < strlen(str); i++) {
            if (str[i] == ' ') break;
            end++;
        }
        strncpy(name, &str[symbol + 1], end - (symbol + 1));
        deleteFromStr(str, symbol, end);
    }
    return name;
}

void deleteFromStr(char *str, int start, int end) {
    int length = end - start;
    if (length > 0) {
        for (int i = start; i < strlen(str) - 1; i++) str[i] = str[i + 1];
        str[strlen(str) - 1] = '\0';
        deleteFromStr(str, start, end - 1);
    }
}

int findCharInStr(char *str, char c) {
    int symbol = -1;
    int i, j, end;
    char *name = (char *) malloc(sizeof(char) * BUFFER_LEN);
    for (i = 0; i < strlen(str); i++) {
        if (str[i] == c) {
            symbol = i;
            break;
        }
    }
    return symbol;
}

int mysys(char *arg) {
    // fork一个子进程
    pid_t fpid = fork();
    if (fpid < 0) {
        // 如果获取子进程失败
        return -1;
    } else if (fpid == 0) {
        // 如果是子进程
        if (execl("/bin/sh", "sh", "-c", arg, (char *) 0) < 0) {
            return 127;
        }
    } else {
        // 等待子进程结束
        waitpid(fpid, NULL, 0);
        return 0;
    }
    return 0;
}
