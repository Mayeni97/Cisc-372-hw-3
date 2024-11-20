/* File:    trap.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  Estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -o trap trap.c
 * Usage:   ./trap
 *
 * Note:    The function f(x) is hardwired.
 *
 * IPP2:    Section 3.2.1 (pp. 101 and ff.) and 5.2 (p. 228)
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

double f(double x);  // Function to integrate
void *Trap(void *rank);  // Thread function to perform trapezoidal calculations

// Global variables for integration
double a, b, h;
int n, thread_count;
double total_integral = 0.0;
pthread_mutex_t mutex;  // Mutex for synchronizing access to total_integral

int main(int argc, char *argv[]) {
    long thread;
    pthread_t *thread_handles;

    // Read number of threads from command line
    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = malloc(thread_count * sizeof(pthread_t));

    // Read integration limits and number of trapezoids
    printf("Enter a, b, and n\n");
    scanf("%lf", &a);
    scanf("%lf", &b);
    scanf("%d", &n);

    h = (b - a) / n;  // Calculate height of trapezoids

    pthread_mutex_init(&mutex, NULL);

    // Create threads
    for (thread = 0; thread < thread_count; thread++) {
        pthread_create(&thread_handles[thread], NULL, Trap, (void *)thread);
    }

    // Join threads
    for (thread = 0; thread < thread_count; thread++) {
        pthread_join(thread_handles[thread], NULL);
    }

    // Output result
    printf("With n = %d trapezoids, our estimate\n", n);
    printf("of the integral from %f to %f = %.15e\n", a, b, total_integral);

    // Clean up
    pthread_mutex_destroy(&mutex);
    free(thread_handles);

    return 0;
}

double f(double x) {
    // Define the function to integrate here, e.g., f(x) = x*x
    return x * x;
}

void *Trap(void *rank) {
    long my_rank = (long)rank;
    int local_n = n / thread_count;  // Number of trapezoids per thread
    double local_a = a + my_rank * local_n * h;  // Start of local interval
    double local_b = local_a + local_n * h;  // End of local interval
    double my_integral = (f(local_a) + f(local_b)) / 2.0;
    
    // Compute trapezoidal estimate in each thread's interval
    for (int i = 1; i < local_n; i++) {
        double x = local_a + i * h;
        my_integral += f(x);
    }
    my_integral *= h;

    // Accumulate result in the global integral, protected by mutex
    pthread_mutex_lock(&mutex);
    total_integral += my_integral;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

