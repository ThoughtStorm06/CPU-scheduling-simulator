#include <stdlib.h>
#include "gantt_chart.h"
#include "Process.h"
#include "merge_Sort.h"
#include "Prority.h"

/* Non-preemptive Priority Scheduling
 * Processes are executed in arrival order but when multiple processes
 * have arrived and the CPU is free we sort the arrived list by priority
 * (lower value = higher priority) using merge_sort(field=4).
 */
void Priority(struct process* head, struct gantt_chart* chart) {
    struct gantt_chart* tail = chart;
    int current_time = 0;
    struct process* p = head;

    while (p) {
        struct gantt_chart* new_node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
        new_node->next = NULL;

        if (current_time >= p->arrival_time) {
            new_node->process_id = p->pid;
            new_node->start_time = current_time;
            current_time += p->burst_time;
            new_node->end_time = current_time;
        } else {
            new_node->process_id = p->pid;
            current_time = p->arrival_time;
            new_node->start_time = current_time;
            current_time += p->burst_time;
            new_node->end_time = current_time;
        }

        tail->next = new_node;
        tail = new_node;

        /* Compute metrics */
        p->Completion_time = current_time;
        p->Turn_Around_time = p->Completion_time - p->arrival_time;
        p->Waiting_time = p->Turn_Around_time - p->burst_time;

        /* If more processes have arrived while we were running, sort them by priority */
        if (p->next && current_time > p->next->arrival_time) {
            struct process* unsorted_list = p->next;
            struct process* tail_proc = p->next;
            while (tail_proc->arrival_time <= current_time && tail_proc->next)
                tail_proc = tail_proc->next;

            struct process* next_iteration = tail_proc->next;
            tail_proc->next = NULL;

            merge_sort(4, &unsorted_list); // Sort by Priority (field=4)
            p->next = unsorted_list;

            while (unsorted_list->next)
                unsorted_list = unsorted_list->next;
            unsorted_list->next = next_iteration;
            p = p->next;
        } else {
            p = p->next;
        }
    }
}

/* Preemptive Priority Scheduling (new)
 * Time-unit simulation (similar to SRTF implementation):
 * - At each time unit pick the arrived process with smallest `priority` value.
 * - Execute one time unit, update remaining time and gantt chart (coalesce same-pid units).
 * - On completion, compute Completion/Turnaround/Waiting based on original burst.
 */
void Priority_preemptive(struct process* head, struct gantt_chart* chart) {
    if (!head) return;

    /* Count processes */
    int n = 0;
    for (struct process* t = head; t; t = t->next) n++;

    struct process** procs = malloc(sizeof(struct process*) * n);
    int* rem = malloc(sizeof(int) * n); /* remaining times */
    int* OBT = malloc(sizeof(int) * n); /* original burst times */
    int* prio = malloc(sizeof(int) * n); /* priority values */

    int idx = 0;
    for (struct process* t = head; t; t = t->next) {
        procs[idx] = t;
        rem[idx] = t->burst_time;
        OBT[idx] = t->burst_time;
        prio[idx] = t->priority;
        idx++;
    }

    int completed = 0;
    int current_time = 0;
    int last_pid = -1;
    struct gantt_chart* tail = chart;

    while (completed < n) {
        /* select arrived process with highest priority (smallest prio value)
         * tie-breaker: smaller pid */
        int chosen = -1;
        for (int i = 0; i < n; ++i) {
            if (procs[i]->arrival_time <= current_time && rem[i] > 0) {
                if (chosen == -1 || prio[i] < prio[chosen] || (prio[i] == prio[chosen] && procs[i]->pid < procs[chosen]->pid)) {
                    chosen = i;
                }
            }
        }

        if (chosen == -1) {
            /* no arrived process — advance time to next arrival */
            int next_arr = -1;
            for (int i = 0; i < n; ++i) if (rem[i] > 0) {
                if (next_arr == -1 || procs[i]->arrival_time < next_arr) next_arr = procs[i]->arrival_time;
            }
            if (next_arr > current_time) current_time = next_arr;
            continue;
        }

        /* Append or extend gantt chart node for this time unit */
        if (last_pid != procs[chosen]->pid) {
            struct gantt_chart* node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
            node->next = NULL;
            node->process_id = procs[chosen]->pid;
            node->start_time = current_time;
            node->end_time = current_time + 1;
            tail->next = node;
            tail = node;
        } else {
            tail->end_time += 1;
        }

        /* execute one time unit */
        rem[chosen] -= 1;
        current_time += 1;
        last_pid = procs[chosen]->pid;

        if (rem[chosen] == 0) {
            procs[chosen]->Completion_time = current_time;
            procs[chosen]->Turn_Around_time = procs[chosen]->Completion_time - procs[chosen]->arrival_time;
            procs[chosen]->Waiting_time = procs[chosen]->Turn_Around_time - OBT[chosen];
            completed++;
        }
    }

    free(procs);
    free(rem);
    free(OBT);
    free(prio);
}
