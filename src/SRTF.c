/*
 * Shortest Remaining Time First (preemptive) implementation.
 * This implementation simulates time in discrete units and picks the
 * arrived process with the smallest remaining time at each unit.
 *
 * It builds gantt chart nodes by coalescing consecutive time units
 * executed by the same process.
 */
#include <stdlib.h>
#include <string.h>
#include "gantt_chart.h"
#include "Process.h"
#include "SRTF.h"

void SRTF(struct process* head, struct gantt_chart* chart) {
    if (!head) return;

    /* Count processes */
    int n = 0;
    for (struct process* t = head; t; t = t->next) n++;

    /* Arrays for quick indexed access during simulation */
    struct process** procs = malloc(sizeof(struct process*) * n);
    int* rem = malloc(sizeof(int) * n); /* remaining times */
    int* OBT = malloc(sizeof(int) * n); /* original burst times */

    int idx = 0;
    for (struct process* t = head; t; t = t->next) {
        procs[idx] = t;
        rem[idx] = t->burst_time;
        OBT[idx] = t->burst_time;
        idx++;
    }

    int completed = 0;
    int current_time = 0;
    int last_pid = -1; /* pid of last executing process */
    struct gantt_chart* tail = chart;
    struct gantt_chart* node = NULL;

    while (completed < n) {
        /* Choose the arrived process with minimum remaining time */
        int chosen = -1;
        for (int i = 0; i < n; ++i) {
            if (procs[i]->arrival_time <= current_time && rem[i] > 0) {
                if (chosen == -1 || rem[i] < rem[chosen] || (rem[i] == rem[chosen] && procs[i]->pid < procs[chosen]->pid)) {
                    chosen = i;
                }
            }
        }

        if (chosen == -1) {
            /* No arrived process: advance time to the next arrival */
            int next_arr = -1;
            for (int i = 0; i < n; ++i) {
                if (rem[i] > 0) {
                    if (next_arr == -1 || procs[i]->arrival_time < next_arr) next_arr = procs[i]->arrival_time;
                }
            }
            if (next_arr > current_time) current_time = next_arr;
            continue;
        }

        /* Append or extend a gantt chart node for this time unit */
        if (last_pid != procs[chosen]->pid) {
            node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
            node->next = NULL;
            node->process_id = procs[chosen]->pid;
            node->start_time = current_time;
            node->end_time = current_time + 1;
            tail->next = node;
            tail = node;
        } else {
            /* extend last node's end_time by 1 */
            tail->end_time += 1;
        }

        /* Execute one time unit */
        rem[chosen] -= 1;
        current_time += 1;
        last_pid = procs[chosen]->pid;

        if (rem[chosen] == 0) {
            /* Process finished: set completion metrics */
            procs[chosen]->Completion_time = current_time;
            procs[chosen]->Turn_Around_time = procs[chosen]->Completion_time - procs[chosen]->arrival_time;
            procs[chosen]->Waiting_time = procs[chosen]->Turn_Around_time - OBT[chosen];
            completed++;
        }
    }

    free(procs);
    free(rem);
    free(OBT);
}