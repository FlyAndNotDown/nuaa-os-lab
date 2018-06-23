#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	char buffer[1024];
	int file, count;

	// 打开文件
    file = open(argv[1], O_RDONLY);
	if (file < 0)
		printf("Can't open the file %s", argv[1]);

	// 不断输出
    while ((count = read(file, buffer, 1023)) > 0) {
    	write(1, buffer, count);
    }

	// 关闭文件
	close(file);
    return 0;
}
