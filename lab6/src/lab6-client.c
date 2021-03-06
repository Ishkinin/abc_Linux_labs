#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>

#include "shared.h"

int main(int argc, char *argv[]);

void proc_client(shm_block_struct *m, int proc_pid, char* msg);

int main(int argc, char *argv[])
{
    // Инициализация блока разделяемой памяти
    key_t key;
    int shm_id;
    shm_block_struct *shm_block;

    key = load_key();
    if (key == -1) {
        printf(
            "Не удалось загрузить ключ общей памяти\n"
            "Сервер, вероятно, не работает\n\n");
        return 1;
    }
    shm_id = shmget(key, sizeof(shm_block_struct), 0666);
    if (shm_id == -1) {
        printf("Сервер не работает\n");
        return 1;
    }
    shm_block = (shm_block_struct *) shmat(shm_id, 0, 0);

    // Подготовка к отправке сообщения
    int proc_pid = getpid();
    char *msg = (char*) malloc(sizeof(char)*SHMB_MSG_LEN);

    if (1 < argc) {
        if (strlen(argv[1]) >= SHMB_MSG_LEN) {
            printf("Сообщение слишком длинное. Максимальная длина %i\n\n", SHMB_MSG_LEN);
            return 1;
        }
        strcpy(msg, argv[1]);
    } else {
        sprintf(msg, "Это сообщение от процесса с PID %i", proc_pid);
    }

    // Отправка сообщения
    proc_client(shm_block, proc_pid, msg);

    return 0;
}

void proc_client(shm_block_struct *m, int proc_pid, char* msg)
{
    sem_wait(&m->sem);
    printf("Процесс: Првет, Я PID - %i, посмотри на меня!\n", proc_pid);
    sem_post(&m->sem);

    // Отправка сообщения
    // Жду разрешения на отправку сообщения
    sem_wait(&m->sem_send_msg);
    m->sender_proc_pid = proc_pid;
    strcpy((char*) &m->msg, msg);
    // Даю разрешение на получение
    sem_post(&m->sem_new_msg);

    sem_wait(&m->sem);
    printf("Процесс: отправлено сообщение.\n\n");
    sem_post(&m->sem);

    return;
}
