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

// 信号量
typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

// 信号量初始化
void sema_init(sema_t *, int);
// 等待条件变量
void sema_wait(sema_t *);
// 唤醒等待条件变量的线程
void sema_signal(sema_t *);

// 三个子线程
void *producer_func(void *);
void *calculator_func(void *);
void *consumer_func(void *);

// 定义信号量
sema_t mutex_sema1, mutex_sema2;
sema_t empty_buffer_sema1, empty_buffer_sema2;
sema_t full_buffer_sema1, full_buffer_sema2;

int main(int argc, char *argv[]) {
    // 线程标识符
    pthread_t calculator_thread, consumer_thread;

    // 初始化信号量
    sema_init(&mutex_sema1, 1);
    sema_init(&empty_buffer_sema1, 1);
    sema_init(&full_buffer_sema1, 0);
    sema_init(&mutex_sema2, 1);
    sema_init(&empty_buffer_sema2, 1);
    sema_init(&full_buffer_sema2, 0);

    // 创建计算者和消费者进程
    pthread_create(&calculator_thread, NULL, &calculator_func, NULL);
    pthread_create(&consumer_thread, NULL, &consumer_func, NULL);
    // 执行生产者线程
    producer_func(NULL);

    // 等待线程执行完毕
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

void sema_init(sema_t *sema, int value) {
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema) {
    pthread_mutex_lock(&sema->mutex);
    sema->value--;
    while (sema->value < 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema) {
    pthread_mutex_lock(&sema->mutex);
    sema->value++;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

void *producer_func(void *args) {
    for (int i = 0; i < CHAR_NUM; i++) {
        sema_wait(&empty_buffer_sema1);
        sema_wait(&mutex_sema1);
        char item = START_CHAR + i;
        put_item(buffer1, &in1, item);
        sema_signal(&mutex_sema1);
        sema_signal(&full_buffer_sema1);
    }
    return NULL;
}

void *calculator_func(void *args) {
    for (int i = 0; i < CHAR_NUM; i++) {
        sema_wait(&full_buffer_sema1);
        sema_wait(&mutex_sema1);
        char item = get_item(buffer1, &out1);
        sema_signal(&mutex_sema1);
        sema_signal(&empty_buffer_sema1);

        sema_wait(&empty_buffer_sema2);
        sema_wait(&mutex_sema2);
        put_item(buffer2, &in2, item - 32);
        sema_signal(&mutex_sema2);
        sema_signal(&full_buffer_sema2);
    }
    return NULL;
}

void *consumer_func(void *args) {
    for (int i = 0; i < CHAR_NUM; i++) {
        sema_wait(&full_buffer_sema2);
        sema_wait(&mutex_sema2);
        char item = get_item(buffer2, &out2);
        printf("%c ", item);
        sema_signal(&mutex_sema2);
        sema_signal(&empty_buffer_sema2);
    }
    return NULL;
}
