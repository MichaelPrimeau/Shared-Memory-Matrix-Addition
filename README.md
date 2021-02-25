# Shared-Memory-Matrix-Addition

## Introduction
In this lab, we wrote two programs that completed matrix addition and then printed the results to the console. The matrices were stored in two separate files and had to be passed in as arguments and parsed. One of the programs used a single process to complete the addition and the second one used multi-process programming, one for each row in the matrices. The execution time of both programs was recorded and printed with the results.

## Design
The single process program was very straightforward. Once the files were parsed and the matrices were stored all that had to be done was create an algorithm for completing the addition and storing the result. The multi-process program required extra overhead to complete. 
	
Multiple processes had to be created, one for each row of the matrices. However, the number of rows is not known until the files are parsed which meant the processes had to be created in a loop. The pid of each child process had to be stored so the parent process could wait for each child to complete before printing the results. Extra overhead was also required to set up shared memory for the child processes to access and store the results of the addition.
I set up the shared memory location as a one-dimensional array and used index calculations to find where each process needed to store its results. The parent process was then able to read these results and print them to the console.

## Build Instructions
To build the single process program the command “gcc matrix.c” will compile the program and then “./a.out” plus the two source files of the matrices as arguments will run the program. The multi process program can be compiled with “gcc matrixshm.c -lrt” and then run with “./a.out” plus the two source file arguments.

## Analysis
From looking at the timing of the two applications the single process program runs faster than the multi-process program. From multiple executions the single process program sometimes runs over two times faster than the multi-process program. I believe the reason for this is because the time it takes two set up shared memory and create the individual processes takes time away from the actual execution of the addition. Creating the overhead to run multiple processes slows down execution significantly and makes using multiple processes impractical for this operation.
	
One way the computation could be sped up is by identifying patterns in the matrices. By doing this instead of doing the addition at every single point in the matrices, the results could be copied and pasted instead of redoing the calculation. For example, if there are a bunch of zeros in the source matrices we could avoid iterating through those positions and put zeros into the result matrix. This would allow the program to spend more time completing meaningful calculations.
	
From running valgrind with both the single process and multi process program, we can see that shared memory does not seem to affect the overall amount of memory that the program allocates. The programs still allocate very similar amounts of memory but slightly different because of the allocation of memory to store pids in the multi process program. From this we can conclude that using shared memory does not require overhead within memory itself to make and therefore does not require more space.

## Conclusion
The most challenging part of this lab for me was figuring out how to create child processes in a loop and how to keep track of them. It was difficult for me to figure out how to differentiate between the parent and the children as well as making sure only the parent process created children. However, it was fun creating a program that made use of multiple process to complete computation
