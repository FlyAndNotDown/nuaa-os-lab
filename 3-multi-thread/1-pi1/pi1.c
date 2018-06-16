#include <stdio.h>
#include <pthread.h>

// 级数深度
int depth = 0;
// 前半部分的结果
float left = 0;
// 后半部分的结果
float right = 0;

// 子线程函数
void *thread_func(void *);
// 计算莱布尼茨级数深度从start到end的一部分
float calculate(int, int);

int main(int argc, char *argv[]) {
    // 输入参数判断
    if (argc < 2) {
        printf("too few arguments\nusage: ./pi1.run depth\n");
        return 0;
    }
    sscanf(argv[1], "%d", &depth);
    if (depth <= 0) {
        printf("the depth is too small\n");
        return 0;
    }
    if (depth == 1) {
        printf("1\n");
        return 0;
    }

    // 创建子线程
    pthread_t child_thread;
    pthread_create(&child_thread, NULL, &thread_func, NULL);

    // 计算莱布尼茨技术前半部分
    left = calculate(1, depth / 2);

    // 等待子线程执行完毕
    pthread_join(child_thread, NULL);

    // 输出结果
    printf("%f\n", left + right);
    return 0;
}

void *thread_func(void *args) {
    right = calculate(depth / 2 + 1, depth);
    return NULL;
}

float calculate(int start, int end) {
    float result;
    for (int i = start; i <= end; i++)
        result += i % 2 == 0 ? (0 - 1.0 / (i * 2 - 1)) : 1.0 / (i * 2 - 1);
    return result;
}
