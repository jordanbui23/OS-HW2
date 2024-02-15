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

    //Load PCBs into ready_queue
    ProcessControlBlock_t pcb;
    while (fread(&pcb, sizeof(ProcessControlBlock_t), 1, file) == 1) 
    {
        dyn_array_push_back(ready_queue, &pcb);
    }

    //Execute scheduling algorithm to collect time statistics
    //TODO?
    if(argv[2] == FCFS)
    {
        first_come_first_serve(ready_queue);
    }
    else if(argv[2] == P)
    {
        //TODO
        return EXIT_FAILURE;
    }
    else if(argv[2] == RR)
    {
        //TODO
        return EXIT_FAILURE;
    }
    else if(argv[2] == SJF)
    {
        //TODO
        return EXIT_FAILURE;
    }
    else
    {
        //Should this be shortest time remaining?
    }
    

    //Clean up allocations (free dyn_array? & close file?)
    fclose(file);
    dyn_array_destroy(ready_queue);

    //Report times to STDOUT the readme file
    //TODO

    return EXIT_SUCCESS;
}