/**
 * matrix.c
 * Lab 4 Shared Memory Matrix Addition
 * Written by: Michael Primeau
 * This program uses a single process to complete matrix addition
 * and times the execution
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    // Time execution
    pid_t clockPid = getpid();
    clockid_t clockId;
    struct timespec timeSpec;
    long startTime;
    long endTime;

    clock_getcpuclockid(clockPid, &clockId);
    clock_gettime(clockId, &timeSpec);
    startTime = (timeSpec.tv_sec*1000000000) + timeSpec.tv_nsec;

    int yA, xA, yB, xB;
    FILE* inputA = fopen(argv[1], "r");
    FILE* inputB = fopen(argv[2], "r");

    // Get xA and yA values from file
    fscanf(inputA, "%d", &yA);
    fscanf(inputA, "%d", &xA);

    printf("%d xA %d\n", xA, yA);

    // Malloc space for matrixA 
    int* matrixA = malloc(sizeof(int) * yA * xA);

    // Parse through matrixA 
    for(int i = 0; i < yA; i++) {
        for(int j = 0; j < xA; j++) {
            int value;
            fscanf(inputA, "%d", &value);
            matrixA[i*xA+j] = value;
        }
    }
    fclose(inputA);

    // Get xB and yB values from file
    fscanf(inputB, "%d", &yB);
    fscanf(inputB, "%d", &xB);

    printf("%d xB %d\n", xB, yB);

    // Malloc space for matrixB
    int* matrixB = malloc(sizeof(int) * yB * xB);

    // Parse through matrixB
    for(int i = 0; i < yB; i++) {
        for(int j = 0; j < xB; j++) {
            int value;
            fscanf(inputB, "%d", &value);
            matrixB[i*xB+j] = value;
        }
    }
    fclose(inputB);

    if(xA != xB || yA != yB) {
        printf("Matrices are two different sizes, cannot complete addition\n");
        return 0;
    }

    // Add matrices
    int* result = malloc(sizeof(int) * xA * yA);
    if(xA == xB && yA == yB) {
        for(int i = 0; i < yA; i++) {
            for(int j = 0; j < xA; j++) {
                int index = i * xA + j;
                result[index] = matrixA[index] + matrixB[index];
                printf("%d ", result[index]);
            }
            printf("\n");
        }
    } else {

    }

    free(matrixA);
    free(matrixB);
    free(result);

    // Print time to complete
    clock_gettime(clockId, &timeSpec);
    endTime = (timeSpec.tv_sec*1000000000) + timeSpec.tv_nsec;
    printf("Time to complete: %lu nanoseconds\n", endTime);

    return 0;
}