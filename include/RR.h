#include "gantt_chart.h"
#include "Process.h"

#ifndef RR_H
#define RR_H

struct RR{
    int pid;
    int remaining_time;
    struct RR* next;
};



void RR(struct process* head,int quantum_number,struct gantt_chart *chart);

#endif