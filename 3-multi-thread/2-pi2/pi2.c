#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

// 线程参数结构体
typedef struct {
    int start;
    int end;
    float result;
} thread_args, *thread_args_ptr;

// 计算莱布尼茨级数深度从start到end的一部分
void *thread_func(void *);
float calculate(int, int);

int main(int argc, char *argv[]) {
    // 参数校验
    if (argc < 3) {
        printf("too few argument\nusage: ./pi2.run depth thread_num\n");
    }
    // 读入参数
    int depth = 0;
    int thread_num = 0;
    sscanf(argv[1], "%d", &depth);
    sscanf(argv[2], "%d", &thread_num);

    // 一点判断
    if (depth <= 0) {
        printf("the depth is too small\n");
        return 0;
    }
    if (thread_num <= 1) {
        printf("the thread is too few\n");
        return 0;
    }
    if (thread_num > 100) {
        printf("you can't get so many thread at one time\n");
    }
    if (depth < thread_num) {
        printf("the depth can't small then thread_num\n");
        return 0;
    }

    // 创建进程
    pthread_t *tid_array = (pthread_t *) malloc(
        sizeof(pthread_t) * (thread_num - 1)
    );
    thread_args_ptr thread_args_array = (thread_args_ptr) malloc(
        sizeof(thread_args) * (thread_num - 1)
    );
    for (int i = 0; i < thread_num - 1; i++) {
        thread_args_array[i].start = i * depth / thread_num + 1;
        thread_args_array[i].end = (i + 1) * depth / thread_num;
        thread_args_array[i].result = 0;

        pthread_create(
            &tid_array[i], NULL, &thread_func, (void *) &thread_args_array[i]
        );
    }

    // 自己完成剩下的任务
    float result = calculate((thread_num - 1) * depth / thread_num + 1, depth);

    // 等待线程执行完毕
    for (int i = 0; i < thread_num - 1; i++)
        pthread_join(tid_array[i], NULL);

    // 计算结果并且输出
    for (int i = 0; i < thread_num - 1; i++) {
        result += thread_args_array[i].result;
    }
    printf("%f\n", result);
    return 0;
}

void *thread_func(void *args) {
    thread_args_ptr temp = (thread_args_ptr) args;
    temp->result = calculate(
        temp->start, temp->end
    );

    return NULL;
}

float calculate(int start, int end) {
    float result;
    for (int i = start; i <= end; i++)
        result += i % 2 == 0 ? (0 - 1.0 / (i * 2 - 1)) : 1.0 / (i * 2 - 1);
    return result;
}
