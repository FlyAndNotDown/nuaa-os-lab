#include <stdio.h>

int main(int argc, char *argv[]) {
    FILE *src, *dst;
    char buffer[100];

    src = fopen(argv[1], "r");
    dst = fopen(argv[2], "w+");

    while (feof(src) == 0) {
        fgets(buffer, 100, src);
        fprintf(dst, "%s", buffer);
    }

    fclose(src);
    fclose(dst);

    return 0;
}
