#include <stdio.h>

int main(int argc, char *argv[]) {
    // 输出所有参数
    while (argc --> 1) {
    	printf("%s ", *++argv);
    }
    // 输出一个换行
    printf("\n");

    return 0;
}
