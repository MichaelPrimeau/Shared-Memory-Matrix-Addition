/**
 * matrixshm.c
 * Lab 4 Shared Memory Matrix Addition
 * Written by: Michael Primeau
 * This program makes use of multi programming to complete matrix addition
 * and times the execution
 * 
 **/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

/**
 * addRow()
 *      Takes two rows from two matrices and adds them together and stores the results in memory
 *      Parameters: matrixA, pointer to first matrix for addition
 *                  matrixB, pointer to second matrix for addition
 *                  result, pointer to location to store results
 *                  row, The row index to be used to calculate where to store the results
 *                  width, The width of the row
 **/
void addRow(int* matrixA, int* matrixB, int* result, int row, int width);

int main(int argc, char* argv[]) {
    // Time execution
    pid_t clockPid = getpid();
    clockid_t clockId;
    struct timespec timeSpec;
    long startTime;
    long endTime;

    clock_getcpuclockid(clockPid, &clockId);
    clock_gettime(clockId, &timeSpec);
    startTime = (timeSpec.tv_sec*1000000000)+timeSpec.tv_nsec;

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

    // Create shared memory space and map
    int mapSize = sizeof(int) * yA * xA;

    int shmfd = shm_open("/CS3841Lab4", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(shmfd == -1) {
        printf("Failed to create shared memory segment\n");
        exit(EXIT_FAILURE);
    }
    ftruncate(shmfd, mapSize);
    int* result = mmap(NULL, mapSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if(result == MAP_FAILED) {
        printf("Failed to mmap\n");
        exit(EXIT_FAILURE);  
    }

    // Check that matrices are the same size
    if(xA != xB || yA != yB) {
        printf("Matrices are two different sizes, cannot complete addition\n");
        exit(EXIT_FAILURE);
    }

    // Create memory to store child pids
    int* pids = malloc(sizeof(int) * yA);

    // Create number of child processes equal to number of rows
    int pid;
    for(int i = 0; i < yA; i++) {
        pid = fork();
        // Add child process to pids[] array in parent
        pids[i] = pid;
        // If in child process, complete row addition
        if(pids[i] == 0) {
            // Create temporary arrays for row in matrices A and B then add
            int a[xA];
            int b[xA];
            // Fill temp arrays
            for(int j = 0; j < xA; j++) {
                a[j] = matrixA[i * xA + j];
                b[j] = matrixB[i * xA + j];
            }
            addRow(a, b, (int*)result, i, xA);

            // Free mallocs in child process before exiting
            // And end child process
            i = yA;
            munmap(result, mapSize);
            close(shmfd);
            
            // Free memory allocations in children to remove leaks
            free(matrixA);
            free(matrixB);
            free(pids);
            exit(0);
        }
    }

    // Wait for all child processes to end
    for(int i = 0; i < yA; i++) {
        waitpid(pids[i], NULL, 0);
    }

    // Print out the resulting matrix
    for(int y = 0; y < yA; y++) {
        for(int x = 0; x < xA; x++) {
            printf("%u ", result[y*xA + x]);
        }
        printf("\n");
    }

    // Get rid of shared memory space
    munmap(result, mapSize);
    close(shmfd);
    shm_unlink("/CS3841Lab4");

    // Free all allocated memory
    free(matrixA);
    free(matrixB);
    free(pids);

    // Print time to complete
    clock_gettime(clockId, &timeSpec);
    endTime = (timeSpec.tv_sec*1000000000)+timeSpec.tv_nsec;
    printf("Time to complete: %lu nanoseconds\n", endTime);
    return 0;
}

void addRow(int* matrixA, int* matrixB, int* result, int row, int width) {
    // Add matrices
    for(int i = 0; i < width; i++) {
        int index = row * width + i;
        result[index] = matrixA[i] + matrixB[i];
    }
}