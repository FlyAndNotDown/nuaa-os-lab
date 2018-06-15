#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

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

int main(int argc, char *argv[]) {
    int i;
    char buffer[200];
    buffer[0] = '\0';
    int count = 0;
    // 将参数拼成一个字符串传递给mysys函数
    for (i = 1; i < argc; i++) {
        count += strlen(argv[i]);
        if (count < 200) {
            strcat(buffer, argv[i]);
            if (i < argc - 1)
                strcat(buffer, " ");
        }
    }

    // 调用mysys
    mysys(buffer);

    return 0;
}
