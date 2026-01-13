#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t db_write_lock;    // 数据库写锁
pthread_mutex_t mutex;  // 互斥锁
int read_count = 0;     // 正在读的读者数量
int shared_data = 0;    // 共享数据

void show_status(const char* role, int id, const char* action) {
    printf(">> [%s-%d] %s (当前读者数: %d, 数据值: %d)\n", 
           role, id, action, read_count, shared_data);
}

void* reader(void* arg) {
    int id = *(int*)arg;
    free(arg);

    // 1. 申请进入
    pthread_mutex_lock(&mutex);
    read_count++;
    if (read_count == 1) {
        sem_wait(&db_write_lock); // 如果是第一个读者，锁住写者
    }
    pthread_mutex_unlock(&mutex);

    // 2. 执行读取 (模拟耗时)
    show_status("读者", id, "开始读取...");
    sleep(2); // 模拟读取需要2秒
    show_status("读者", id, "读取完成");

    // 3. 离开
    pthread_mutex_lock(&mutex);
    read_count--;
    if (read_count == 0) {
        sem_post(&db_write_lock); // 如果是最后一个读者，释放写者
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    free(arg);

    show_status("写者", id, "等待写入权限...");

    // 1. 申请独占写入
    sem_wait(&db_write_lock); 

    // 2. 执行写入 (模拟耗时)
    shared_data++; // 修改数据
    show_status("写者", id, "正在修改数据"); 
    sleep(3);      // 假设写入需要3秒
    show_status("写者", id, "写入完成，离开");

    // 3. 释放锁
    sem_post(&db_write_lock);

    return NULL;
}

int main() {
    sem_init(&db_write_lock, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    int id_counter = 1;
    char choice;

    printf("   读者-写者  \n");
    printf("'r' 启动一个读者\n");
    printf("'w' 启动一个写者\n");
    printf("'q' 退出程序\n");
    printf("==============================\n");

    while (1) {
        scanf(" %c", &choice);

        if (choice == 'q') break;

        // 动态分配ID，传给线程
        int* id = malloc(sizeof(int));
        *id = id_counter++;

        pthread_t tid;
        if (choice == 'r') {
            printf("> 指令: 创建读者 %d\n", *id);
            pthread_create(&tid, NULL, reader, id);
            pthread_detach(tid);
        } 
        else if (choice == 'w') {
            printf("> 指令: 创建写者 %d\n", *id);
            pthread_create(&tid, NULL, writer, id);
            pthread_detach(tid);
        }
    }

    // 清理资源
    sem_destroy(&db_write_lock);
    pthread_mutex_destroy(&mutex);
    return 0;
}