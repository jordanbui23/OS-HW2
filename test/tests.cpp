#include <fcntl.h>
#include <stdio.h>
#include "gtest/gtest.h"
#include <pthread.h>
#include "../include/processing_scheduling.h"

// Using a C library requires extern "C" to prevent function mangling
extern "C"
{
#include <dyn_array.h>
}

/*
#define NUM_PCB 30
#define QUANTUM 5 // Used for Robin Round for process as the run time limit

unsigned int score;
unsigned int total;

class GradeEnvironment : public testing::Environment
{
 public:
   virtual void SetUp()
   {
      score = 0;
      total = 210;
   }
   virtual void TearDown()
   {
      ::testing::Test::RecordProperty("points_given", score);
      ::testing::Test::RecordProperty("points_total", total);
      std::cout << "SCORE: " << score << '/' << total << std::endl;
   }
};
*/


TEST(load_process_control_blocks, InvalidInputFile) {
    dyn_array_t *pcb_array = load_process_control_blocks("nonexistent_file.bin");
    EXPECT_EQ(pcb_array, nullptr);
}


TEST(first_come_first_serve, EmptyReadyQueue) {
    dyn_array_t *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ScheduleResult_t result;
    bool success = first_come_first_serve(ready_queue, &result);

    EXPECT_FALSE(success);
}

TEST (first_come_first_serve, NullQueue) {
    ScheduleResult_t res;
    bool success = first_come_first_serve(NULL, &res);
    EXPECT_FALSE(success);
}

TEST (first_come_first_serve, NonEmptyReadyQueue) {
    dyn_array_t *ready_queue = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);
    
    //Example values
    ProcessControlBlock_t pcb1 = {10, 2, 0, true};  //burst time, priority, arrival 
    ProcessControlBlock_t pcb2 = {8, 1, 5, true};   
    ProcessControlBlock_t pcb3 = {15, 3, 3, true};

    dyn_array_push_back(ready_queue, &pcb1);
    dyn_array_push_back(ready_queue, &pcb2);
    dyn_array_push_back(ready_queue, &pcb3);

    ScheduleResult_t result;
    bool success = first_come_first_serve(ready_queue, &result);

    EXPECT_TRUE(success);
}

TEST(round_robin, EmptyReadyQueue) {
    dyn_array_t *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ScheduleResult_t result;
    bool success = round_robin(ready_queue, &result, 5);

    EXPECT_FALSE(success);
}

TEST(round_robin, NonEmptyReadyQueue) {
    dyn_array_t *ready_queue = dyn_array_create(2, sizeof(ProcessControlBlock_t), NULL);

    ProcessControlBlock_t pcb1 = {10, 1, 0, true};  
    ProcessControlBlock_t pcb2 = {8, 2, 2, true};
    dyn_array_push_back(ready_queue, &pcb1);
    dyn_array_push_back(ready_queue, &pcb2);

    ScheduleResult_t result;
    bool success = round_robin(ready_queue, &result, 5); 

    EXPECT_TRUE(success);
}

TEST(round_robin, VaryingBurstTimesAndPriorities) {
    dyn_array_t *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ProcessControlBlock_t pcb1 = {6, 1, 0, true};
    ProcessControlBlock_t pcb2 = {10, 2, 2, true};
    ProcessControlBlock_t pcb3 = {8, 1, 3, true};
    dyn_array_push_back(ready_queue, &pcb1);
    dyn_array_push_back(ready_queue, &pcb2);
    dyn_array_push_back(ready_queue, &pcb3);

    ScheduleResult_t result;
    bool success = round_robin(ready_queue, &result, 4);

    EXPECT_TRUE(success);
    EXPECT_FLOAT_EQ(result.average_waiting_time, 26);
    EXPECT_FLOAT_EQ(result.average_turnaround_time, 34);
    EXPECT_EQ(result.total_run_time, 24ul);
}

TEST(shortest_remaining_time_first, EmptyReadyQueue) {
    dyn_array_t *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ScheduleResult_t result;
    bool success = shortest_remaining_time_first(ready_queue, &result);

    EXPECT_TRUE(success);
}

TEST (shortest_remaining_time_first, NullQueue) {
    ScheduleResult_t res;
    bool success = shortest_remaining_time_first(NULL, &res);
    EXPECT_FALSE(success);
}

TEST (shortest_remaining_time_first, NonEmptyReadyQueue) {
    dyn_array_t *ready_queue = dyn_array_create(3, sizeof(ProcessControlBlock_t), NULL);
    
    //Example values
    ProcessControlBlock_t pcb1 = {10, 2, 0, true};  //burst time, priority, arrival 
    ProcessControlBlock_t pcb2 = {8, 1, 5, true};   
    ProcessControlBlock_t pcb3 = {15, 3, 3, true};

    dyn_array_push_back(ready_queue, &pcb1);
    dyn_array_push_back(ready_queue, &pcb2);
    dyn_array_push_back(ready_queue, &pcb3);

    ScheduleResult_t result;
    bool success = first_come_first_serve(ready_queue, &result);

    EXPECT_TRUE(success);
}

TEST(shortest_job_first, EmptyReadyQueue) {
    dyn_array_t *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ScheduleResult_t result;
    bool success = shortest_job_first(ready_queue, &result);

    EXPECT_FALSE(success);
}

TEST(shortest_job_first, MultipleProcesses) {
    dyn_array_t *ready_queue = dyn_array_create(0, sizeof(ProcessControlBlock_t), NULL);
    ProcessControlBlock_t pcb1 = {8, 1, 0, false};
    ProcessControlBlock_t pcb2 = {12, 2, 2, false};
    ProcessControlBlock_t pcb3 = {6, 1, 3, false};
    dyn_array_push_back(ready_queue, &pcb1);
    dyn_array_push_back(ready_queue, &pcb2);
    dyn_array_push_back(ready_queue, &pcb3);

    ScheduleResult_t result;
    bool success = shortest_job_first(ready_queue, &result);

    EXPECT_TRUE(success);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    //::testing::AddGlobalTestEnvironment(new GradeEnvironment);
    return RUN_ALL_TESTS();
}