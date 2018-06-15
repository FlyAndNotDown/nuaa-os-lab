#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int mysys(char *);

int main(int argc, char *argv[]) {
    char buffer[500];
    
    // 事件主循环
    while (1) {
        // 提示
        printf("kbash$ ");
        // 读入指令
        fgets(buffer, 500, stdin);
        if (strcmp(buffer, "exit") == 0)
            break;
        // 执行指令
        mysys(buffer);
    }

    return 0;
}

int mysys(char *arg) {
    pid_t fpid = fork();
    if (fpid < 0) {
        return -1;
    } else if (fpid == 0) {
        if (execl("/bin/sh", "sh", "-c", arg, (char *) 0) < 0) {
            return 127;
        }
    } else {
        wait(NULL);
        return 0;
    }
}