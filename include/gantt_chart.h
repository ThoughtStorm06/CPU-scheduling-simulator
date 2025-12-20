#ifndef GANTT_CHART_H
#define GANTT_CHART_H

struct gantt_chart {
    int process_id;
    int start_time;
    int end_time;
    struct gantt_chart* next;
};

void chart_display(struct gantt_chart* chart);

#endif