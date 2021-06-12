#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>


union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
};

#define PATH "/home/"

typedef struct __counter_t {
    int value;
    key_t key;
    int semid;
    union semun arg;
} counter_t;

unsigned int loop_cnt;
counter_t counter;

void init(counter_t *c, char *argv[]) {
    c->value = 0;
    c->key = ftok(PATH, 'z');
    if (c->key < 0) {
        perror(argv[0]);
        exit(1);
    }
    c->semid = semget(c->key, 1, 0600 | IPC_CREAT);
    if (c->semid < 0) {
        perror(argv[0]);
        exit(1);
    }

    c->arg.val = 1;
    semctl(c->semid, 0, SETVAL, c->arg); 
}

void increment(counter_t *c) {
    struct sembuf s;
    
    /* Lock */
    s.sem_num = 0;
    s.sem_op = -1; 
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
    
    c->value++;
    
    /* Unlock */
    s.sem_num = 0;
    s.sem_op = 1;
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
}

void decrement(counter_t *c) {
    struct sembuf s;
    
    /* Lock */
    s.sem_num = 0;
    s.sem_op = -1; 
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
    
    c->value--;
    
    /* Unlock */
    s.sem_num = 0;
    s.sem_op = 1;
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
}

int get(counter_t *c) {
    struct sembuf s;
    
    /* Lock */
    s.sem_num = 0;
    s.sem_op = -1; 
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
    
    int rc = c->value;
    
    /* Unlock */
    s.sem_num = 0;
    s.sem_op = 1;
    s.sem_flg = 0;
    semop(c->semid, &s, 1);
    
    return rc;
}

void *mythread(void *arg)
{
    char *letter = arg;
    int i;

    printf("%s: begin\n", letter);
    
    for (i = 0; i < loop_cnt; i++) {
        increment(&counter);
    }
    printf("%s: done\n", letter);
    return NULL;
}
                                                                             
int main(int argc, char *argv[])
{             
      
    loop_cnt = atoi(argv[1]);

    init(&counter, argv);
    
    clock_t start, end;
    float time;

    pthread_t p1, p2;
    printf("main: begin [counter = %d]\n", get(&counter));
    pthread_create(&p1, NULL, mythread, "A"); 
    pthread_create(&p2, NULL, mythread, "B");
    // join waits for the threads to finish
    start = clock();
    pthread_join(p1, NULL); 
    pthread_join(p2, NULL);
    end = clock();
    
    time = (float)(end - start)/CLOCKS_PER_SEC;
    
    printf("main: done [counter: %d] [should be: %d]\n", get(&counter), loop_cnt * 2);
    printf("Time : %.3f\n", time);
    return 0;
}
