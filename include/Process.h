#ifndef PROCESS_H
#define PROCESS_H

struct process {
    int pid;
    int arrival_time;
    int burst_time;
    int Completion_time;
    int Turn_Around_time;
    int Waiting_time;
    int priority;
    struct process* next;
};

#endif