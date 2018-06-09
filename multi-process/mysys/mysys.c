#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

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

int main(int argc, char *argv[]) {
    int i;
    char buffer[200];
    buffer[0] = '\0';
    int count = 0;
    for (i = 1; i < argc; i++) {
        count += strlen(argv[i]);
        if (count < 200) {
            strcat(buffer, argv[i]);
            if (i < argc - 1)
                strcat(buffer, " ");
        }
    }

    mysys(buffer);

    return 0;
}