#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_QUEUE 100
#define MAX_LINE 256
#define NUM_THREADS 4
#define NUM_PRODUCERS 2

// Shared Queue
char queue[MAX_QUEUE][MAX_LINE];
int front = 0, rear = 0, count = 0;

// File list - Ensure these files exist in your directory!
const char *files[] = {"file1.txt", "file2.txt", "file3.txt"};
int file_index = 0;
int total_files = 3;

int producers_done = 0;

// Function to add line to queue
void enqueue(char *line) {
    int added = 0;
    while (!added) {
        #pragma omp critical(queue_access)
        {
            if (count < MAX_QUEUE) {
                strncpy(queue[rear], line, MAX_LINE - 1);
                queue[rear][MAX_LINE - 1] = '\0'; // Safety null terminator
                rear = (rear + 1) % MAX_QUEUE;
                count++;
                added = 1;
            }
        }
        if (!added) {
            // Small sleep to prevent CPU spiking if queue is full
            #pragma omp taskyield
        }
    }
}

// Function to remove line from queue
int dequeue(char *line) {
    int success = 0;
    #pragma omp critical(queue_access)
    {
        if (count > 0) {
            strcpy(line, queue[front]);
            front = (front + 1) % MAX_QUEUE;
            count--;
            success = 1;
        }
    }
    return success;
}

// Producer function
void producer() {
    while (1) {
        const char *filename = NULL;
        #pragma omp critical(file_access)
        {
            if (file_index < total_files) {
                filename = files[file_index++];
            }
        }

        if (filename == NULL) break;

        FILE *fp = fopen(filename, "r");
        if (!fp) {
            printf("Error opening file %s\n", filename);
            continue;
        }

        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, fp)) {
            enqueue(line);
        }
        fclose(fp);
    }

    #pragma omp atomic
    producers_done++;
}

// Consumer function
void consumer() {
    char line[MAX_LINE];
    char *saveptr; // Required for thread-safe strtok_r

    while (1) {
        if (dequeue(line)) {
            // Use strtok_r for thread safety
            char *token = strtok_r(line, " \t\n\r", &saveptr);
            while (token != NULL) {
                printf("Thread %d: %s\n", omp_get_thread_num(), token);
                token = strtok_r(NULL, " \t\n\r", &saveptr);
            }
        } else {
            // Atomic read of producers_done to ensure visibility
            int done;
            #pragma omp atomic read
            done = producers_done;

            if (done == NUM_PRODUCERS && count == 0)
                break;

            #pragma omp taskyield
        }
    }
}

int main() {
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        int id = omp_get_thread_num();
        if (id < NUM_PRODUCERS) {
            producer();
        } else {
            consumer();
        }
    }
    return 0;
}


