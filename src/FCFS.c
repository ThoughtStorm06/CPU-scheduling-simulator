#include "gantt_chart.h"
#include "Process.h"
#include <stdlib.h>
#include "FCFS.h"

void FCFS(struct process* head, struct gantt_chart *chart) {
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
            // CPU will be idle until process arrives
            new_node->process_id = p->pid;
            current_time = p->arrival_time;
            new_node->start_time = current_time;
            current_time += p->burst_time;
            new_node->end_time = current_time;
        }

        tail->next = new_node;
        tail = new_node; // Move tail forward

        // Data Computation
        p->Completion_time = current_time;
        p->Turn_Around_time = p->Completion_time - p->arrival_time;
        p->Waiting_time = p->Turn_Around_time - p->burst_time;
        
        p = p->next;
    }
}