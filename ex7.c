#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define DATA_SIZE 10

int num1[DATA_SIZE];
int num2[DATA_SIZE];

FILE *fp;
sem_t file_sem;

void *thread_add(void *arg) {
    fprintf(fp,"--------Pthread synchronization using semaphore--------\n");
    for (int i = 0; i < DATA_SIZE; i++) {
        int result = num1[i] + num2[i];

        sem_wait(&file_sem);
        fprintf(fp, "Addition operation : %d + %d = %d\n", num1[i], num2[i], result);
        sem_post(&file_sem);
    }
    return NULL;
}

void *thread_sub(void *arg) {
    for (int i = 0; i < DATA_SIZE; i++) {
        int result = num1[i] - num2[i];

        sem_wait(&file_sem);
        fprintf(fp, "Subtraction operation %d - %d = %d\n", num1[i], num2[i], result);
        sem_post(&file_sem);
    }
    return NULL;
}

void *thread_mul(void *arg) {
    for (int i = 0; i < DATA_SIZE; i++) {
        long result = (long)num1[i] * num2[i];

        sem_wait(&file_sem);
        fprintf(fp, "Multiplication operation%d * %d = %ld\n", num1[i], num2[i], result);
        sem_post(&file_sem);
    }
    return NULL;
}

void *thread_div(void *arg) {
    for (int i = 0; i < DATA_SIZE; i++) {
        sem_wait(&file_sem);

        if (num2[i] == 0) {
            fprintf(fp, "Division operation%d / %d = Undefined (Divide by Zero)\n", num1[i], num2[i]);
        } else {
            double result = (double)num1[i] / num2[i];
            fprintf(fp, "Division operation %d / %d = %.2f\n", num1[i], num2[i], result);
        }

        sem_post(&file_sem);
    }
    return NULL;
}

// --- Main ---

int main() {
    pthread_t t_add, t_sub, t_mul, t_div;

    srand(time(NULL));

    printf("Generating %d pairs of random numbers\n", DATA_SIZE);
    for (int i = 0; i < DATA_SIZE; i++) {
        num1[i] = rand() % 100;
        num2[i] = rand() % 100;

        if(num2[i] == 0) num2[i] = 1;
    }

    fp = fopen("results.txt", "w");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }
    //printf("File opened. Starting calculation threads...\n");

    sem_init(&file_sem, 0, 1);

    pthread_create(&t_add, NULL, thread_add, NULL);
    pthread_create(&t_sub, NULL, thread_sub, NULL);
    pthread_create(&t_mul, NULL, thread_mul, NULL);
    pthread_create(&t_div, NULL, thread_div, NULL);

    pthread_join(t_add, NULL);
    pthread_join(t_sub, NULL);
    pthread_join(t_mul, NULL);
    pthread_join(t_div, NULL);

    fclose(fp);
    sem_destroy(&file_sem);

    return 0;
}

