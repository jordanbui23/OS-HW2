#include <stdio.h>
#include <stdlib.h>

#include "dyn_array.h"
#include "processing_scheduling.h"

#define FCFS "FCFS"
#define P "P"
#define RR "RR"
#define SJF "SJF"
#define SRJF "SRJF"

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
    FILE *file = fopen(argv[1], "rb");
    //make sure that the passed file is not null
    if(file == NULL)
    {
        return EXIT_FAILURE;
    }

    //Make the dyn_array ready_queue (I have no opinion on minimum size, that is why it is zero)
    dyn_array_t *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);

    //Load PCBs into ready_queue
    ProcessControlBlock_t pcb;
    while (fread(&pcb, sizeof(ProcessControlBlock_t), 1, file) == 1) 
    {
        dyn_array_push_back(ready_queue, &pcb);
    }

    //Execute scheduling algorithm to collect time statistics
    //TODO
    if(strncmp(argv[2], FCFS, 2))
    {
        // first_come_first_serve(ready_queue, XXX); NEED ANOTHER ARGUMENT IN FUNCTION CALL
    }
    else if(strncmp(argv[2], RR, 2))
    {
        //round_robin(ready_queue, XXX, YYY);
    }
    else if(strncmp(argv[2], SRJF, 2))
    {
        ScheduleResult_t result;
        shortest_remaining_time_first(ready_queue, result);
    }
    else if(strncmp(argv[2], SJF, 2))
    {
        //shortest_job_first(ready_queue, XXX);
    }
    else
    {
        printf("Invalid scheduling algorithm");
        fclose(file);
        dyn_array_destroy(ready_queue);
        return EXIT_FAILURE;
    }
    

    //Clean up allocations (free dyn_array? & close file?)
    fclose(file);

    //Report times to STDOUT
    printf("Time Statistics:\n");
    //I used ChatGPT for this next section (next 5 lines) It did a very clever printf with formatting
    printf("PID\tArrival Time\tPriority\tRemaining Burst Time\tStarted\n");
    for (size_t i = 0; i < dyn_array_size(ready_queue); i++) {
        ProcessControlBlock_t *pcb1 = (ProcessControlBlock_t *)dyn_array_at(ready_queue, i);
        printf("%zu\t%u\t\t%u\t\t%u\t\t\t%s\n", i + 1, pcb1->arrival, pcb1->priority, pcb1->remaining_burst_time, pcb1->started ? "Yes" : "No");
    }

    //This next section will report the times to the readme (feel free to delete this section for better performance)
    FILE *readme_file = fopen("readme.md", "w");
    if (readme_file == NULL) {
        printf("Error: Unable to open readme.md for writing.\n");
        dyn_array_destroy(ready_queue);
        return EXIT_FAILURE;
    }

    //Write general data format to README.md
    fprintf(readme_file, "## Simulation Statistics\n");
    fprintf(readme_file, "| PID | Arrival Time | Priority | Remaining Burst Time | Started |\n");
    fprintf(readme_file, "| --- | ------------ | -------- | -------------------- | ------- |\n");

    //Collect statistics and report times (just like before but with slightly different formatting)
    for (size_t i = 0; i < dyn_array_size(ready_queue); i++) {
        ProcessControlBlock_t *pcb2 = (ProcessControlBlock_t *)dyn_array_at(ready_queue, i);
        fprintf(readme_file, "| %zu | %u | %u | %u | %s |\n", i + 1, pcb2->arrival, pcb2->priority, pcb2->remaining_burst_time, pcb2->started ? "Yes" : "No");
    }

    //close the readme
    fclose(readme_file);

    //clean up allocations part 2
    dyn_array_destroy(ready_queue);

    return EXIT_SUCCESS;
}