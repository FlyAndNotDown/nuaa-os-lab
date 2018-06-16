#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

// 读入一行进入缓冲区
int readLine(char *, int);
// 伪造system函数
int mysys(char *);
// 清空缓冲区
void bufferClear(char *, int);

int main(int argc, char *argv[]) {
    char *p;
    char buffer[BUFFER_LEN];
    char temp[BUFFER_LEN];

    // 开始主循环
    while (1) {
        // 输出shell标识符
        printf("$");
        // 清空缓冲区
        bufferClear(buffer, BUFFER_LEN);
        bufferClear(temp, BUFFER_LEN);
        // 读取一行，如果溢出了就报错
        if (readLine(buffer, BUFFER_LEN)) {
            // 拷贝原始输入
            strcpy(temp, buffer);
            // 分割参数
            p = strtok(temp, " ");
            // 根据指令的不同做不同的事情
            if (!strcmp(p, "echo")) {
                p = strtok(NULL, "");
                printf("%s\n", p);
            } else if (!strcmp(p, "ls")) {
                mysys(buffer);
            } else if (!strcmp(p, "cd")) {
                // TODO
            } else if (!strcmp(p, "pwd")) {
                // TODO
            } else if (!strcmp(p, "exit")) {
                // 退出程序
                return 0;
            }
        } else {
            printf("You can't input so much char at one time!\n");
        }
    }

    return 0;
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
        wait(NULL);
        return 0;
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
