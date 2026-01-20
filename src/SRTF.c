#include <stdlib.h>
#include "gantt_chart.h"
#include "Process.h"
#include "merge_Sort.h"
#include "SRTF.h"

void SRTF(struct process* head, struct gantt_chart* chart) {
    struct gantt_chart* tail = chart;
    int OBT[100]; // Assuming a maximum of 100 processes- Original Burst Time
    for(int i = 0; i < 100; i++)
        OBT[i] = -1;
    int current_time = 0;
    struct process* p = head;

    //creating a dummy pointer to traverse the list to complete processes.
    struct process* dummy=(struct process*)malloc(sizeof(struct process));
    dummy->next=head;
    head=dummy;

    //while executing through the incoming processes.
    while(p) {
        struct gantt_chart* new_node;

        if(current_time >= dummy->next->arrival_time) {
            if(OBT[dummy->next->pid-1]==-1) {
                OBT[dummy->next->pid-1]=dummy->next->burst_time;
                new_node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
                new_node->next = NULL;

                new_node->process_id = dummy->next->pid;
                new_node->start_time = current_time;
                current_time += 1;
                dummy->next->burst_time -= 1;
                new_node->end_time = current_time;

                tail->next = new_node;
                tail = new_node;
            }
            else {
                if(dummy->next->pid !=tail->process_id) {
                    new_node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
                    new_node->next = NULL;

                    new_node->process_id = dummy->next->pid;
                    new_node->start_time = current_time;
                    current_time += 1;
                    dummy->next->burst_time -= 1;
                    new_node->end_time = current_time;
                    tail->next = new_node;
                    tail = new_node;
                }
                else {
                  tail->end_time += 1;
                  current_time += 1;
                  dummy->next->burst_time -= 1;
                  new_node = NULL;
                }
            }
            
        }
        else {
            current_time = p->arrival_time;
            OBT[p->pid-1]=p->burst_time;
                new_node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
                new_node->next = NULL;

                new_node->process_id = p->pid;
                new_node->start_time = current_time;
                current_time += 1;
                p->burst_time -= 1;
                new_node->end_time = current_time;
        }
        
        if(dummy->next->burst_time==0) {
            dummy->next->Completion_time=current_time;
            dummy=dummy->next;
        }   
        
        
 
        if(p->next && current_time >= p->next->arrival_time) {
            
            struct process* arrival_boundary = p;
            while(arrival_boundary->next && arrival_boundary->next->arrival_time <= current_time) {
                arrival_boundary = arrival_boundary->next;
            }

            struct process* not_yet_arrived = arrival_boundary->next;
            arrival_boundary->next = NULL;

            struct process* arrived_head = dummy->next;
            merge_sort(3, &arrived_head);
            dummy->next = arrived_head;

            struct process* temp = arrived_head;
            while(temp->next) temp = temp->next;
            temp->next = not_yet_arrived;
            p = temp; 
        } else {
            if (p->next == NULL) {
                p  = NULL;
            }
        }
    }

    dummy=dummy->next;
    p=dummy;
    //after all processes have arrived, execute remaining processes in order of burst time.
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
            // Idle time logic
            new_node->process_id = p->pid;
            current_time = p->arrival_time;
            new_node->start_time = current_time;
            current_time += p->burst_time;
            new_node->end_time = current_time;
        }

        tail->next = new_node;
        tail = new_node;

        p->Completion_time = current_time;
        p = p->next;
    }


    //Restoring original burst times and calculating TAT and WT
    struct process* temp=head->next;
    while(temp){
        if (temp->pid > 0 && temp->pid <= 100 && OBT[temp->pid - 1] != -1) {
            int original_bt = OBT[temp->pid - 1];
            
            temp->Turn_Around_time = temp->Completion_time - temp->arrival_time;
            temp->Waiting_time = temp->Turn_Around_time - original_bt;
            
            temp->burst_time = original_bt;
        }
        temp = temp->next;
    } 
    
}