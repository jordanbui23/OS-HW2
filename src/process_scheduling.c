#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dyn_array.h"
#include "processing_scheduling.h"


// You might find this handy.  I put it around unused parameters, but you should
// remove it before you submit. Just allows things to compile initially.
#define UNUSED(x) (void)(x)

// private function
void virtual_cpu(ProcessControlBlock_t *process_control_block) 
{
    // decrement the burst time of the pcb
    --process_control_block->remaining_burst_time;
}

bool first_come_first_serve(dyn_array_t *ready_queue, ScheduleResult_t *result)  {
    if (ready_queue == NULL || dyn_array_size(ready_queue) == 0) {
        return false; 
    }

    // Local vars
    float total_wait_time = 0.0;
    float total_turnaround_time = 0.0;
    unsigned long total_clock_time = 0;

    for (size_t i = 0; i < dyn_array_size(ready_queue); ++i) {
        ProcessControlBlock_t *pcb = dyn_array_at(ready_queue, i);

        // Update times
        total_turnaround_time += total_clock_time + pcb->burst_time; //Should you subtract arrival time from these? JO
        total_wait_time += total_clock_time;

        //total_turnaround_time += (total_clock_time + pcb->burst_time) - pcb->arrival_time;
        //total_wait_time += total_clock_time - pcb->arrival_time;

        // Update total clock time
        total_clock_time += pcb->burst_time;
    }

    // Calculate all the times
    result->average_waiting_time = total_wait_time / dyn_array_size(ready_queue);
    result->average_turnaround_time = total_turnaround_time / dyn_array_size(ready_queue);
    result->total_run_time = total_clock_time;

    return true; 
}

bool shortest_job_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

//Do not have to do this one this time
bool priority(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;   
}

bool round_robin(dyn_array_t *ready_queue, ScheduleResult_t *result, size_t quantum) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    UNUSED(quantum);
    return false;
}

dyn_array_t *load_process_control_blocks(const char *input_file) {
    if (input_file == NULL) {
        return NULL;
    }

    FILE *file = fopen(input_file, "rb");
    if (file == NULL) {
        return NULL; 
    }

    uint32_t num_blocks;
    fread(&num_blocks, sizeof(uint32_t), 1, file);
    // Create a dyn_array to store the ProcessControlBlocks
    dyn_array_t *pcb_array = dyn_array_create(num_blocks, sizeof(ProcessControlBlock_t));

    // Read burst time, priority, and arrival time for each process control block
    for (uint32_t i = 0; i < num_blocks; i++) {
        ProcessControlBlock_t pcb;
        // A lot of error checking for all the times. fread will return 1 if successful
        if (fread(&pcb.burst_time, sizeof(uint32_t), 1, file) != 1 ||
            fread(&pcb.priority, sizeof(uint32_t), 1, file) != 1 ||
            fread(&pcb.arrival_time, sizeof(uint32_t), 1, file) != 1) {
            fclose(file);
            dyn_array_destroy(pcb_array);
            return NULL; 
        }
        dyn_array_push_back(pcb_array, &pcb);
    }

    fclose(file);
    return pcb_array;
}

bool shortest_remaining_time_first(dyn_array_t *ready_queue, ScheduleResult_t *result) 
{
    UNUSED(ready_queue);
    UNUSED(result);
    return false;
}
