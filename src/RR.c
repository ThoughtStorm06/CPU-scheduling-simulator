#include "gantt_chart.h"
#include "Process.h"
#include <stdlib.h>
#include "RR.h"


struct RR* create_RR_Node(int pid, int remaining_time) {
    struct RR* new_node = (struct RR*)malloc(sizeof(struct RR));
    new_node->pid = pid;
    new_node->remaining_time = remaining_time;
    new_node->next = NULL;
    return new_node;
}

void circular_append(struct RR** head_ref, int pid, int remaining_time) {
    struct RR* new_node = create_RR_Node(pid, remaining_time);
    if (!(*head_ref)) {
        *head_ref = new_node;
        new_node->next = new_node; 
    } else {
        struct RR* temp = *head_ref;
        while (temp->next != *head_ref) {
            temp = temp->next;
        }
        temp->next = new_node;
        new_node->next = *head_ref; 
    }
}

void delete_RR_Node(struct RR** head_ref, struct RR* del_node) {
    if (*head_ref == NULL || del_node == NULL) return;
    struct RR* temp = *head_ref;
    if (*head_ref == del_node) {
        while (temp->next != *head_ref) {
            temp = temp->next;
        }
        if (*head_ref == (*head_ref)->next) {
            free(*head_ref);
            *head_ref = NULL;
            return;
        }
        temp->next = (*head_ref)->next;
        *head_ref = (*head_ref)->next;
        free(del_node);
        return;
    }
}

void RR(struct process* head, int quantum_number, struct gantt_chart *chart) {
    struct RR* rr_head = NULL;
    
    struct gantt_chart* tail = chart;
    
    int current_time = 0;
    struct process* p = head;

    while(p || rr_head) {
        struct gantt_chart* new_node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
        new_node->next = NULL;

        if(p && current_time >= p->arrival_time) {
            new_node->process_id = p->pid;
            new_node->start_time = current_time;
            if(p->burst_time > quantum_number) {
                current_time += quantum_number;
                circular_append(&rr_head, p->pid, p->burst_time - quantum_number);
                new_node->end_time = current_time;
            }
            else{
                current_time += p->burst_time;
                new_node->end_time = current_time;

                p->Completion_time = current_time;
                p->Turn_Around_time = p->Completion_time - p->arrival_time;
                p->Waiting_time = p->Turn_Around_time - p->burst_time;
            }
            p = p->next;
        }
        else { 
            // Processing from Queue
            if(rr_head) { 
                new_node->process_id = rr_head->pid;
                new_node->start_time = current_time;
                if(rr_head->remaining_time > quantum_number) {
                    current_time += quantum_number;
                    rr_head->remaining_time -= quantum_number;
                    rr_head = rr_head->next;
                    new_node->end_time = current_time;
                }
                else{
                    current_time += rr_head->remaining_time;
                    new_node->end_time = current_time;

                    struct process* proc = head;
                    while(proc && proc->pid != rr_head->pid) {
                        proc = proc->next;
                    }
                    if(proc) { 
                        proc->Completion_time = current_time;
                        proc->Turn_Around_time = proc->Completion_time - proc->arrival_time;
                        proc->Waiting_time = proc->Turn_Around_time - proc->burst_time;
                    }
                    delete_RR_Node(&rr_head, rr_head);
                }
            } else {
                 if (p) {
                    current_time = p->arrival_time;
                    free(new_node); // Discard unused node
                    continue; 
                 }
            }
        }
        
        if (new_node->end_time > 0) { // Simple check to see if we filled data
            tail->next = new_node;
            tail = new_node;
        }
    }
}