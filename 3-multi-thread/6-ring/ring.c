#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int send;

typedef struct {
    pthread_t *pre, *start;
    int no;
} thread_args, *thread_args_ptr;

void *thread_func(void *);

int main(int argc, char *argv[]) {
    // 参数判断
    if (argc < 2) {
        printf("too few arguments\nusage: ./ring.run n\n");
        return 0;
    }

    // 输入参数
    int n;
    sscanf(argv[1], "%d", &n);
    if (n <= 1) {
        printf("the n is too small\n");
        return 0;
    }

    // 创建线程
    pthread_t *thread_array = (pthread_t *) malloc(sizeof(pthread_t) * n);
    thread_args_ptr thread_args_array = (thread_args_ptr) malloc(sizeof(thread_args) * n);
    for (int i = 1; i < n; i++) {
        thread_args_array[i].pre = &thread_array[i - 1];
        thread_args_array[i].start = &thread_array[0];
        thread_args_array[i].no = i;
    }
    thread_args_array[0].pre = &thread_array[n - 1];
    thread_args_array[0].start = &thread_array[0];
    thread_args_array[0].no = 0;

    for (int i = 0; i < n; i++)
        pthread_create(&thread_array[i], NULL, &thread_func, (void *) &thread_args_array[i]);

    pthread_join(thread_array[n - 1], NULL);

    return 0;
}

void *thread_func(void *args) {
    thread_args_ptr temp = (thread_args_ptr) args;

    // 如果是第一个线程
    if (temp->no == 0) {
        send = 1;
        printf("thread %d send: %d\n", temp->no + 1, send);
    // 如果不是第一个线程
    } else {
        pthread_join(*temp->pre, NULL);
        printf("thread %d recv: %d\n", temp->no + 1, send);
        send += 1;
        printf("thread %d send: %d\n", temp->no + 1, send);
    }
    return NULL;
}
