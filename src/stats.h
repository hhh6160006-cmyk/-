#ifndef STATS_H
#define STATS_H

#include "student.h"

typedef int (*CmpFunc)(const Student *, const Student *);

int  cmp_by_total(const Student *a, const Student *b);
int  cmp_by_id(const Student *a, const Student *b);
int  cmp_by_name(const Student *a, const Student *b);

void sort_students(Student *arr, int n, CmpFunc cmp);
void bubble_sort_timed(Student *arr, int n, CmpFunc cmp);
void selection_sort_timed(Student *arr, int n, CmpFunc cmp);
void show_stats(const Student *arr, int n);
void show_histogram(const Student *arr, int n, int course);
void fuzzy_search(const Student *arr, int n, const char *keyword);

#endif
