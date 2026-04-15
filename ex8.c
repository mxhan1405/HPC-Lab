#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i;
    printf("Enter the size of the arrays: ");
    scanf("%d", &n);

    // Dynamic memory allocation
    int *a = (int*)malloc(n * sizeof(int));
    int *b = (int*)malloc(n * sizeof(int));
    int *c = (int*)malloc(n * sizeof(int));

    printf("Enter elements of Array A:\n");
    for(i = 0; i < n; i++) scanf("%d", &a[i]);

    printf("Enter elements of Array B:\n");
    for(i = 0; i < n; i++) scanf("%d", &b[i]);

    // Parallel addition with static scheduling
    #pragma omp parallel for schedule(static)
    for(i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
        printf("Thread %d working on index %d\n", omp_get_thread_num(), i);
    }

    printf("\nResultant Array C (A + B):\n");
    for(i = 0; i < n; i++) printf("%d ", c[i]);
    printf("\n");

    free(a); free(b); free(c);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
    int n, i, constant;
    printf("Enter the size of the array: ");
    scanf("%d", &n);

    int *a = (int*)malloc(n * sizeof(int));

    printf("Enter elements of the array:\n");
    for(i = 0; i < n; i++) scanf("%d", &a[i]);

    printf("Enter the constant value to add: ");
    scanf("%d", &constant);
    // chunk size 2 means each thread takes 2 iterations at a time
    #pragma omp parallel for schedule(dynamic, 2)
    for(i = 0; i < n; i++) {
        a[i] = a[i] + constant;
        printf("Thread %d processed index %d\n", omp_get_thread_num(), i);
    }

    printf("\nUpdated Array:\n");
    for(i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");

    free(a);
    return 0;
}


