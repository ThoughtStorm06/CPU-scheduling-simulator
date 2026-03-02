#ifndef Priority_H
#define Priority_H

#include "Process.h"
#include "gantt_chart.h"


void Priority(struct process* head, struct gantt_chart* chart);
void Priority_preemptive(struct process* head, struct gantt_chart* chart);

#endif