#include <pthread.h>
#include <stdio.h>

#define ARR_LEN 1024

int length = 0;
int numbers[ARR_LEN];

// 子线程
void *thread_func(void *);
// 选择排序
void selectSort(int *, int);

int main(int argc, char *argv[]) {
    // 输入数据
    printf("array length: ");
    scanf("%d", &length);
    if (length <= 0) {
        printf("the length is too small\n");
        return 0;
    }
    printf("array: ");
    for (int i = 0; i < length; i++) scanf("%d", &numbers[i]);
    if (length == 1) {
        printf("result: %d\n", numbers[0]);
        return 0;
    }

    // 创建线程辅助运算
    pthread_t child;
    pthread_create(&child, NULL, &thread_func, NULL);

    // 选择排序前半部分
    selectSort(numbers, length / 2);

    // 等待线程执行完毕
    pthread_join(child, NULL);

    // 进行归并排序
    int result[ARR_LEN];
    int p = 0, q = length / 2, r = 0;
    while (p < length / 2 || q < length) {
        if (p >= length / 2 && q < length) result[r++] = numbers[q++];
        if (p < length / 2 && q >= length) result[r++] = numbers[p++];
        if (p < length / 2 || q < length)
            result[r++] = numbers[p] < numbers[q] ? numbers[p++] : numbers[q++];
    }

    printf("result: ");
    for (int i = 0; i < length; i++) printf("%d ", result[i]);
    printf("\n");

    return 0;
}

void *thread_func(void *args) {
    // 选择排序后半部分
    selectSort(&numbers[length / 2], length - length / 2);

    return NULL;
}

void selectSort(int *arr, int len) {
    for (int i = 0; i < len; i++) {
        int min = i;
        for (int j = i; j < len; j++)
            if (arr[j] < arr[min]) min = j;
        int temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }
}
