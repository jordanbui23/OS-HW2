#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"

// Add and comment your analysis code in this function.
// THIS IS NOT FINISHED.
int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        printf("%s <pcb file> <schedule algorithm> [quantum]\n", argv[0]);
        return EXIT_FAILURE;
    }

    //open the file (this is the second thing passed into the argument vector)
    fp* file = fopen(argv[1], "rb");
    //make sure that the passed file is not null
    if(file == NULL)
    {
        return EXIT_FAILURE;
    }

    //Make the dyn_array ready_queue (I have no opinion on minimum size, that is why it is zero)
    dyn_array *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t));

    //Execute scheduling algorithm to collect time statistics
    //TODO?
    first_come_first_serve(ready_queue);

    //Clean up allocations (free dyn_array? & close file?)
    fclose(file);
    dyn_array_destroy(ready_queue);

    //Report times to STDOUT the readme file
    //TODO

    return EXIT_SUCCESS;
}
