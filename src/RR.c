#include "gantt_chart.h"
#include "Process.h"
#include <stdlib.h>
#include "RR.h"
 
/* Create a ready-queue node for Round-Robin scheduling.
 * pid: process id
 * remaining_time: time left for this process
 * Returns: newly allocated `struct RR*` (caller frees)
 */
struct RR* create_RR_Node(int pid, int remaining_time) {
    struct RR* node = (struct RR*)malloc(sizeof(struct RR));
    node->pid = pid;
    node->remaining_time = remaining_time;
    node->next = NULL;
    return node;
}

void enqueue(struct RR** head, struct RR** tail,
             int pid, int remaining_time) {
    /* Append node at queue tail (maintain head/tail pointers). */
    struct RR* node = create_RR_Node(pid, remaining_time);

    if (*tail == NULL) {
        *head = *tail = node;
    } else {
        (*tail)->next = node;
        *tail = node;
    }
}

struct RR* dequeue(struct RR** head, struct RR** tail) {
    /* Pop head from queue; update tail if queue becomes empty. */
    if (*head == NULL)
        return NULL;

    struct RR* temp = *head;
    *head = (*head)->next;

    if (*head == NULL)
        *tail = NULL;

    temp->next = NULL;
    return temp;
}

void RR(struct process* head, int quantum_number, struct gantt_chart *chart) {
    /* Round-Robin scheduler using a FIFO ready queue.
     * - `head` is the linked list of processes (sorted by arrival time)
     * - `quantum_number` is the time quantum
     * - `chart` is the gantt chart dummy head; we append nodes to it
     */
    struct RR* ready_head = NULL;
    struct RR* ready_tail = NULL;

    struct gantt_chart* tail = chart;

    int current_time = 0;
    struct process* p = head;

    while (p || ready_head) {

        /* Enqueue all processes that have arrived up to current_time */
        while (p && p->arrival_time <= current_time) {
            enqueue(&ready_head, &ready_tail, p->pid, p->burst_time);
            p = p->next;
        }

        /* If no process is ready, advance time to next arrival */
        if (ready_head == NULL) {
            if (p) {
                current_time = p->arrival_time;
                continue;
            } else {
                break;
            }
        }

        /* Dequeue next ready process and run up to `quantum_number` */
        struct RR* current = dequeue(&ready_head, &ready_tail);
        int exec_time = (current->remaining_time > quantum_number) ? quantum_number : current->remaining_time;

        /* Append Gantt chart node for this execution slice */
        struct gantt_chart* new_node = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
        new_node->process_id = current->pid;
        new_node->start_time = current_time;

        current_time += exec_time;
        new_node->end_time = current_time;
        new_node->next = NULL;

        tail->next = new_node;
        tail = new_node;

        /* Decrease remaining time and enqueue if not finished */
        current->remaining_time -= exec_time;

        /* Enqueue any processes that arrived during this time slice */
        while (p && p->arrival_time <= current_time) {
            enqueue(&ready_head, &ready_tail, p->pid, p->burst_time);
            p = p->next;
        }

        if (current->remaining_time > 0) {
            enqueue(&ready_head, &ready_tail, current->pid, current->remaining_time);
        } else {
            /* Find the original process struct and set completion metrics */
            struct process* proc = head;
            while (proc && proc->pid != current->pid) proc = proc->next;
            if (proc) {
                proc->Completion_time = current_time;
                proc->Turn_Around_time = current_time - proc->arrival_time;
                proc->Waiting_time = proc->Turn_Around_time - proc->burst_time;
            }
        }

        free(current);
    }
}