#include <stdio.h>
#include <stdlib.h>
#include "Process.h"
#include "merge_Sort.h"
#include "gantt_chart.h"
#include "FCFS.h"
#include "RR.h"
#include "SJF.h"
#include "Prority.h" 
#include "display.h"


struct process* create_process(int pid, int at, int bt, int prio) {
    struct process* new_node = (struct process*)malloc(sizeof(struct process));
    new_node->pid = pid;
    new_node->arrival_time = at;
    new_node->burst_time = bt;
    new_node->priority = prio;
    new_node->Completion_time = 0;
    new_node->Turn_Around_time = 0;
    new_node->Waiting_time = 0;
    new_node->next = NULL;
    return new_node;
}

void append_process(struct process** head, int pid, int at, int bt, int prio) {
    struct process* new_node = create_process(pid, at, bt, prio);
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    struct process* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = new_node;
}

int main() {
    struct process* head = NULL;
    struct gantt_chart* chart_head = (struct gantt_chart*)malloc(sizeof(struct gantt_chart));
    chart_head->next = NULL;

    int n, choice, quantum = 0;
    int at, bt, prio;

    printf("==========================================\n");
    printf("     CPU SCHEDULING ALGORITHM SIMULATOR   \n");
    printf("==========================================\n");

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("\nProcess P%d:\n", i + 1);
        printf("  Arrival Time: ");
        scanf("%d", &at);
        printf("  Burst Time:   ");
        scanf("%d", &bt);
        printf("  Priority (0 if N/A): ");
        scanf("%d", &prio);
        
        append_process(&head, i + 1, at, bt, prio);
    }

    // Initial Sort by Arrival Time
    merge_sort(2, &head);

    printf("\nSelect Algorithm:\n");
    printf("1. FCFS (First Come First Serve)\n");
    printf("2. SJF (Shortest Job First)\n");
    printf("3. Priority Scheduling\n");
    printf("4. Round Robin\n");
    printf("Enter choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            FCFS(head, chart_head);
            break;
        case 2:
            SJS(head, chart_head); 
            break;
        case 3:
            Priority(head, chart_head);
            break;
        case 4:
            printf("Enter Time Quantum: ");
            scanf("%d", &quantum);
            RR(head, quantum, chart_head);
            break;
        default:
            printf("Invalid Choice!\n");
            return 1;
    }

    // Calculate Averages
    float total_wt = 0, total_tat = 0;
    struct process* temp = head;
    while (temp != NULL) {
        total_wt += temp->Waiting_time;
        total_tat += temp->Turn_Around_time;
        temp = temp->next;
    }

    // Display Results
    printf("\n\n");
    chart_display(chart_head); 
    printf("\n");
    display(head, total_wt / n, total_tat / n); 

    return 0;
}