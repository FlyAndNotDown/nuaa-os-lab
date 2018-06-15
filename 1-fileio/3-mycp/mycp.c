#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *src, *dst;
    char buffer[1024];

    // 打开文件
    src = fopen(argv[1], "r");
    dst = fopen(argv[2], "w+");

    // 一遍从源文件读，一边向目标文件写
    while (feof(src) == 0) {
        fgets(buffer, 1024, src);
        fprintf(dst, "%s", buffer);
    }

    // 关闭文件
    fclose(src);
    fclose(dst);

    return 0;
}
