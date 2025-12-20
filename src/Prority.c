#include <stdlib.h>
#include "gantt_chart.h"
#include "Process.h"
#include "merge_Sort.h"
#include "Prority.h"

void Priority(struct process* head, struct gantt_chart* chart) {
    struct gantt_chart* tail = chart;
    
    int current_time = 0;
    struct process* p = head;

    while(p) {
        struct gantt_chart* new_node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
        new_node->next = NULL;

        if(current_time >= p->arrival_time) {
            new_node->process_id = p->pid;
            new_node->start_time = current_time;
            current_time += p->burst_time;
            new_node->end_time = current_time;
        }
        else {  
            new_node->process_id = p->pid;
            current_time = p->arrival_time;
            new_node->start_time = current_time;
            current_time += p->burst_time;
            new_node->end_time = current_time;
        }

        tail->next = new_node;
        tail = new_node;

        // Data Computation
        p->Completion_time = current_time;
        p->Turn_Around_time = p->Completion_time - p->arrival_time;
        p->Waiting_time = p->Turn_Around_time - p->burst_time;
        
        if(p->next && current_time > p->next->arrival_time) {
            struct process* unsorted_list = p->next;
            struct process* tail_proc = p->next;
            while(tail_proc->arrival_time <= current_time && tail_proc->next)
                tail_proc = tail_proc->next;

            struct process* next_iteration = tail_proc->next;
            tail_proc->next = NULL;

            merge_sort(4, &unsorted_list); // Sort by Priority
            p->next = unsorted_list;

            while(unsorted_list->next)
                unsorted_list = unsorted_list->next;
            unsorted_list->next = next_iteration;
            p = p->next;
        }
        else {
            p = p->next;
        }
    }
}