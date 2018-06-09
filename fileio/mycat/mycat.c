#include <stdio.h>

int main(int argc, char *argv[]) {
	FILE *file;
	char buffer[100];
    
    file = fopen(argv[1], "r");
	if (file == NULL)
		printf("Can't open the file %s", argv[1]);

    while (feof(file) == 0) {
        fgets(buffer, 100, file);
        printf("%s", buffer);
    }
	fclose(file);
    return 0;
}
