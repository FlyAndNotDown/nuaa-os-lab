#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int src, dst;
    char buffer[1024];
    int count;

    // 打开文件
    src = open(argv[1], O_RDONLY);
    dst = open(argv[2], O_WRONLY | O_CREAT, 0666);

    if (src < 0 || dst < 0) printf("can't open the file\n");

    // 一遍从源文件读，一边向目标文件写
    while ((count = read(src, buffer, 1023)) > 0) {
        write(dst, buffer, count);
    }

    // 关闭文件
    close(src);
    close(dst);

    return 0;
}
