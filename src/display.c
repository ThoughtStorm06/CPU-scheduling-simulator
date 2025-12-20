#include <stdio.h>
#include "Process.h"
#include "gantt_chart.h"
#include "display.h"

void display(struct process* head, float waittime_avg, float turnaround_avg) {
    if (!head) {
        printf("No processes to display.\n");
        return;
    }

    printf("\nProcess List:\n");
    printf("-------------------------------------------------------------------------------------------------------\n");
    // Used negative numbers for left-alignment (e.g., %-10s)
    printf("%-6s | %-12s | %-10s | %-8s | %-15s | %-12s | %-15s\n", 
           "PID", "Arrival", "Burst", "Priority", "Completion", "Waiting", "Turnaround");
    printf("-------------------------------------------------------------------------------------------------------\n");

    struct process* current = head;
    while (current) {
        
        printf("%-6d | %-12d | %-10d | ", 
               current->pid, 
               current->arrival_time, 
               current->burst_time);

        // Handles Priority: If 0, print "N/A", otherwise prints the number
        if (current->priority != 0) {
            printf("%-8d | ", current->priority);
        } else {
            printf("%-8s | ", "N/A");
        }

        printf("%-15d | %-12d | %-15d\n", 
               current->Completion_time, 
               current->Waiting_time, 
               current->Turn_Around_time);

        current = current->next;
    }
    printf("-------------------------------------------------------------------------------------------------------\n");
    
    printf("\nAverage Waiting Time:    %.2f\n", waittime_avg);
    printf("Average Turnaround Time: %.2f\n", turnaround_avg);
}