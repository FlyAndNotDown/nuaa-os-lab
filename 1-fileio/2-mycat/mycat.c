#include <stdio.h>

int main(int argc, char *argv[]) {
	FILE *file;
	char buffer[1024];

	// 打开文件
    file = fopen(argv[1], "r");
	if (file == NULL)
		printf("Can't open the file %s", argv[1]);

	// 不断输出
    while (feof(file) == 0) {
        fgets(buffer, 1024, file);
        printf("%s", buffer);
    }

	// 关闭文件
	fclose(file);
    return 0;
}
