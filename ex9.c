#include <stdio.h>
#include <omp.h>

double f(double x) {
    return x * x;
}

int main() {
    int i, n;
    double a, b, h;
    double sum_red = 0.0, integral_red;
    double sum_crit = 0.0, integral_crit;

    printf("Enter lower limit a: ");
    if (scanf("%lf", &a) != 1) return 1;

    printf("Enter upper limit b: ");
    if (scanf("%lf", &b) != 1) return 1;

    printf("Enter number of trapezoids n: ");
    if (scanf("%d", &n) != 1) return 1;

    h = (b - a) / n;

    #pragma omp parallel for reduction(+:sum_red)
    for(i = 1; i < n; i++) {
        sum_red += f(a + i * h);
    }
    integral_red = (h / 2.0) * (f(a) + f(b) + 2.0 * sum_red);


    #pragma omp parallel
    {
        double local_sum = 0.0;


        #pragma omp for
        for(i = 1; i < n; i++) {
            local_sum += f(a + i * h);
        }

        #pragma omp critical
        {
            sum_crit += local_sum;
        }
    }

    integral_crit = (h / 2.0) * (f(a) + f(b) + 2.0 * sum_crit);

    printf("\nResults:\n");
    printf("Reduction Method: %lf\n", integral_red);
    printf("Critical Method:  %lf\n", integral_crit);

    return 0;
}

