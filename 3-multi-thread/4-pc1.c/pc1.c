#include <stdio.h>
#include <pthread.h>

#define BUFFER_LEN 4
#define START_CHAR 'a'
#define CHAR_NUM 8

// 两个缓冲区
char buffer1[BUFFER_LEN];
char buffer2[BUFFER_LEN];
// 四个 io 指针
int in1 = 0, out1 = 0, in2 = 0, out2 = 0;

// buffer 是否为空
int buffer_is_empty(int, int);
// buffer 是否为满
int buffer_is_full(int, int);
// 获取一个 item 并且移动 out 指针
char get_item(char *, int *);
//放入一个 item 并且移动 in 指针
void put_item(char *, int *, char item);

// 临界区
pthread_mutex_t mutex1, mutex2;
// 条件变量
pthread_cond_t wait_empty_buffer1, wait_empty_buffer2;
pthread_cond_t wait_full_buffer1, wait_full_buffer2;

// 三个子线程
void *producer_func(void *);
void *calculator_func(void *);
void *consumer_func(void *);

int main(int argc, char *argv[]) {
    // 线程 tid
    pthread_t calculator_thread, consumer_thread;

    // 初始化条件变量
    pthread_mutex_init(&mutex1, NULL);
    pthread_cond_init(&wait_empty_buffer1, NULL);
    pthread_cond_init(&wait_full_buffer1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&wait_empty_buffer2, NULL);
    pthread_cond_init(&wait_full_buffer2, NULL);

    // 创建计算者和消费者线程
    pthread_create(&calculator_thread, NULL, &calculator_func, NULL);
    pthread_create(&consumer_thread, NULL, &consumer_func, NULL);

    // 执行生产者线程
    producer_func(NULL);

    // 等待计算者和消费者线程执行完毕
    pthread_join(calculator_thread, NULL);
    pthread_join(consumer_thread, NULL);

    printf("\n");

    return 0;
}

int buffer_is_empty(int in, int out) {
    return in == out;
}

int buffer_is_full(int in, int out) {
    return (in + 1) % BUFFER_LEN == out;
}

char get_item(char *buffer, int *out) {
    char item;
    item = buffer[*out];
    *out = (*out + 1) % BUFFER_LEN;
    return item;
}

void put_item(char *buffer, int *in, char item) {
    buffer[*in] = item;
    *in = (*in + 1) % BUFFER_LEN;
}

void *producer_func(void *args) {
    for (int i = 0; i < CHAR_NUM; i++) {
        // 加锁
        pthread_mutex_lock(&mutex1);
        // 如果缓冲区是满的，就等待一个空的缓冲区
        while (buffer_is_full(in1, out1))
            pthread_cond_wait(&wait_empty_buffer1, &mutex1);
        int item = START_CHAR + i;
        put_item(buffer1, &in1, item);
        // printf("produce item: %c\n", item);
        // 改变条件变量并且解锁
        pthread_cond_signal(&wait_full_buffer1);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}

void *calculator_func(void *args) {
    for (int i = 0; i < CHAR_NUM; i++) {
        pthread_mutex_lock(&mutex1);
        while (buffer_is_empty(in1, out1))
            pthread_cond_wait(&wait_full_buffer1, &mutex1);
        int item = get_item(buffer1, &out1);
        pthread_cond_signal(&wait_empty_buffer1);
        pthread_mutex_unlock(&mutex1);

        pthread_mutex_lock(&mutex2);
        while (buffer_is_full(in2, out2))
            pthread_cond_wait(&wait_empty_buffer2, &mutex2);
        put_item(buffer2, &in2, item - 32);
        // printf("calculate item: %c to %c\n", item, item - 21);
        pthread_cond_signal(&wait_full_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}

void *consumer_func(void *args) {
    for (int i = 0; i < CHAR_NUM; i++) {
        pthread_mutex_lock(&mutex2);
        while (buffer_is_empty(in2, out2))
            pthread_cond_wait(&wait_full_buffer2, &mutex2);
        int item = get_item(buffer2, &out2);
        printf("%c ", item);
        pthread_cond_signal(&wait_empty_buffer2);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}
