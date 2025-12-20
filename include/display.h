#include "Process.h"
#include "gantt_chart.h"

#ifndef DISPLAY_H
#define DISPLAY_H

void display(struct process* head,float waittime_avg,float turnaround_avg);

#endif