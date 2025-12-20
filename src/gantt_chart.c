#include<stdio.h>
#include "gantt_chart.h"

void chart_display(struct gantt_chart* chart) {
    if (chart == NULL || chart->next == NULL) {
        printf("GANTT CHART EMPTY\n");
        return;
    }

    printf("GANTT CHART:\n");

    int counter = 0;
    struct gantt_chart* ptr = chart->next;

    while (ptr) {
        counter++;
        ptr = ptr->next;
    }

    /* ---------------- TOP BAR ---------------- */
    ptr = chart->next;
    printf("+");
    while (ptr) {
        printf("-----+");
        if (ptr->next && ptr->end_time != ptr->next->start_time) {
            printf("     +");
        }
        ptr = ptr->next;
    }
    printf("\n");

    /* --------------- PROCESS IDs ------------- */
    ptr = chart->next;
    printf("|");
    while (ptr) {
        printf("P%4d|", ptr->process_id);
        if (ptr->next && ptr->end_time != ptr->next->start_time) {
            printf("     |");
        }
        ptr = ptr->next;
    }
    printf("\n");

    /* --------------- BOTTOM BAR --------------- */
    ptr = chart->next;
    printf("+");
    while (ptr) {
        printf("-----+");
        if (ptr->next && ptr->end_time != ptr->next->start_time) {
            printf("-----+");
        }
        ptr = ptr->next;
    }
    printf("\n");

    /* ---------------- TIMELINE ---------------- */
    ptr = chart->next;
    printf("%-6d", ptr->start_time);

    while (ptr) {
        printf("%-6d", ptr->end_time);
        if (ptr->next && ptr->end_time != ptr->next->start_time) {
            printf("%-6d", ptr->next->start_time);
        }
        ptr = ptr->next;
    }

    printf("\n");
}