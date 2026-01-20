#include "Process.h"
#include <stdlib.h>
#include "merge_Sort.h"

// field: 1 - pid, 2 - arrival time, 3 - burst time, 4 - priority
struct process* sorted_merge(int field, struct process* a, struct process* b) {
    if (!a) return b;
    if (!b) return a;

    int compare_a, compare_b;
    if (field == 1) {
        compare_a = a->pid; compare_b = b->pid;
    } else if (field == 2) {
        compare_a = a->arrival_time; compare_b = b->arrival_time;
    } else if (field == 3) {
        compare_a = a->burst_time; compare_b = b->burst_time;
    } else {
        compare_a = a->priority; compare_b = b->priority;
    }

    int a_first = 0;
    if (compare_a < compare_b) {
        a_first = 1;
    } else if (compare_a == compare_b) {
        // TIE BREAKER: arrange based on process id
        if (a->pid < b->pid) a_first = 1;
        else a_first = 0;
    } else {
        a_first = 0;
    }

    if (a_first) {
        a->next = sorted_merge(field, a->next, b);
        return a;
    } else {
        b->next = sorted_merge(field, a, b->next);
        return b;
    }
}

static void split_list(struct process* source, struct process** front_ref, struct process** back_ref) {
    if (!source || !source->next) {
        *front_ref = source;
        *back_ref = NULL;
        return;
    }
    struct process* slow = source;
    struct process* fast = source->next;
    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }
    *front_ref = source;
    *back_ref = slow->next;
    slow->next = NULL;
}

void merge_sort(int field, struct process** head_ref) {
    if (!head_ref || !*head_ref || !(*head_ref)->next) return;
    struct process* a;
    struct process* b;
    split_list(*head_ref, &a, &b);
    merge_sort(field, &a);
    merge_sort(field, &b);
    *head_ref = sorted_merge(field, a, b);
}