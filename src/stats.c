#include "stats.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>

int cmp_by_total(const Student *a, const Student *b) {
    if (a->total < b->total) return 1;
    if (a->total > b->total) return -1;
    return 0;
}

int cmp_by_id(const Student *a, const Student *b) {
    return strcmp(a->id, b->id);
}

int cmp_by_name(const Student *a, const Student *b) {
    return strcmp(a->name, b->name);
}

static void swap_student(Student *a, Student *b) {
    Student tmp = *a;
    *a = *b;
    *b = tmp;
}

void sort_students(Student *arr, int n, CmpFunc cmp) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (cmp(&arr[j], &arr[j + 1]) > 0) {
                swap_student(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void bubble_sort_timed(Student *arr, int n, CmpFunc cmp) {
    clock_t start = clock();
    sort_students(arr, n, cmp);
    clock_t end = clock();
    double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("冒泡排序完成，耗时: %.3f 毫秒\n", ms);
}

void selection_sort_timed(Student *arr, int n, CmpFunc cmp) {
    clock_t start = clock();
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (cmp(&arr[j], &arr[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            swap_student(&arr[i], &arr[min_idx]);
        }
    }
    clock_t end = clock();
    double ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("选择排序完成，耗时: %.3f 毫秒\n", ms);
}

void show_stats(const Student *arr, int n) {
    if (n == 0) {
        printf("\n当前没有学生记录，无法统计。\n");
        return;
    }

    printf("\n========== 成绩统计分析 ==========\n");

    for (int c = 0; c < MAX_COURSE; c++) {
        float min_s = arr[0].scores[c];
        float max_s = arr[0].scores[c];
        float sum = 0.0f;
        int pass = 0, excellent = 0;

        for (int i = 0; i < n; i++) {
            float sc = arr[i].scores[c];
            sum += sc;
            if (sc < min_s) min_s = sc;
            if (sc > max_s) max_s = sc;
            if (sc >= 60.0f) pass++;
            if (sc >= 85.0f) excellent++;
        }

        float avg = sum / n;
        printf("\n【%s】\n", COURSE_NAMES[c]);
        printf("  最高分: %.1f  最低分: %.1f  平均分: %.2f\n", max_s, min_s, avg);
        printf("  及格率: %.1f%% (%d/%d)  优秀率(≥85): %.1f%% (%d/%d)\n",
               100.0f * pass / n, pass, n,
               100.0f * excellent / n, excellent, n);
    }

    printf("\n【总分统计】\n");
    float min_t = arr[0].total, max_t = arr[0].total, sum_t = 0.0f;
    int fail_count = 0;
    for (int i = 0; i < n; i++) {
        sum_t += arr[i].total;
        if (arr[i].total < min_t) min_t = arr[i].total;
        if (arr[i].total > max_t) max_t = arr[i].total;
        if (arr[i].average < 60.0f) fail_count++;
    }
    printf("  最高总分: %.1f  最低总分: %.1f  平均总分: %.2f\n",
           max_t, min_t, sum_t / n);
    printf("  不及格人数（平均分<60）: %d / %d\n", fail_count, n);
}

void show_histogram(const Student *arr, int n, int course) {
    if (n == 0) {
        printf("\n当前没有学生记录。\n");
        return;
    }
    if (course < 0 || course >= MAX_COURSE) {
        printf("课程编号无效。\n");
        return;
    }

    int buckets[5] = {0, 0, 0, 0, 0};
    const char *labels[5] = {"[0,60)", "[60,70)", "[70,80)", "[80,90)", "[90,100]"};

    for (int i = 0; i < n; i++) {
        float sc = arr[i].scores[course];
        if (sc < 60.0f)       buckets[0]++;
        else if (sc < 70.0f)  buckets[1]++;
        else if (sc < 80.0f)  buckets[2]++;
        else if (sc < 90.0f)  buckets[3]++;
        else                  buckets[4]++;
    }

    int max_count = 0;
    for (int i = 0; i < 5; i++) {
        if (buckets[i] > max_count) max_count = buckets[i];
    }

    printf("\n【%s 分数段分布】\n", COURSE_NAMES[course]);
    for (int i = 0; i < 5; i++) {
        printf("%-10s: ", labels[i]);
        int bar_len = max_count > 0 ? (buckets[i] * 20 / max_count) : 0;
        if (buckets[i] > 0 && bar_len == 0) bar_len = 1;
        for (int j = 0; j < bar_len; j++) printf("#");
        printf("  (%d)\n", buckets[i]);
    }
}

static int str_contains_ci(const char *haystack, const char *needle) {
    if (!needle || needle[0] == '\0') return 1;
    if (!haystack) return 0;

    size_t nlen = strlen(needle);
    size_t hlen = strlen(haystack);
    if (nlen > hlen) return 0;

    for (size_t i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (size_t j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i + j]) !=
                tolower((unsigned char)needle[j])) {
                match = 0;
                break;
            }
        }
        if (match) return 1;
    }
    return 0;
}

void fuzzy_search(const Student *arr, int n, const char *keyword) {
    if (keyword[0] == '\0') {
        printf("关键词不能为空。\n");
        return;
    }

    int found = 0;
    printf("\n--- 模糊查询结果（关键词: %s）---\n", keyword);
    for (int i = 0; i < n; i++) {
        if (str_contains_ci(arr[i].name, keyword) ||
            str_contains_ci(arr[i].id, keyword)) {
            printf("%-12s %-10s %-4c 总分: %.1f  平均: %.2f\n",
                   arr[i].id, arr[i].name, arr[i].gender,
                   arr[i].total, arr[i].average);
            found++;
        }
    }
    if (found == 0) {
        printf("未找到匹配 \"%s\" 的学生。\n", keyword);
    } else {
        printf("共找到 %d 条记录。\n", found);
    }
}
