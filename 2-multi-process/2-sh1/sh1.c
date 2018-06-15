#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 1024

int readLine(char *, int);

int main(int argc, char *argv[]) {
    char *p;
    char buffer[BUFFER_LEN];

    // 开始主循环
    while (1) {
        // 输出shell标识符
        printf("$");
        // 读取一行，如果溢出了就报错
        if (readLine(buffer, BUFFER_LEN)) {
            // 分割参数
            p = strtok(buffer, " ");
            // 根据指令的不同做不同的事情
            if (!strcmp(p, "echo")) {
                printf("%s\n", buffer);
            } else if (!strcmp(p, "ls")) {

            } else if (!strcmp(p, "cd")) {

            } else if (!strcmp(p, "pwd")) {

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
