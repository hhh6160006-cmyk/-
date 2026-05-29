#include "student.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

const char *COURSE_NAMES[MAX_COURSE] = {"语文", "数学", "英语", "物理", "化学"};

void calc_total_avg(Student *s) {
    float sum = 0.0f;
    for (int i = 0; i < MAX_COURSE; i++) {
        sum += s->scores[i];
    }
    s->total = sum;
    s->average = sum / MAX_COURSE;
}

int add_student(Student *arr, int *n, Student new_one) {
    if (*n >= MAX_STUDENT) {
        printf("错误：学生数量已达上限 %d，无法继续添加。\n", MAX_STUDENT);
        return -1;
    }
    if (find_by_id(arr, *n, new_one.id) >= 0) {
        printf("错误：学号 %s 已存在，请重新录入。\n", new_one.id);
        return -1;
    }
    calc_total_avg(&new_one);
    arr[*n] = new_one;
    (*n)++;
    return 0;
}

int find_by_id(const Student *arr, int n, const char *id) {
    for (int i = 0; i < n; i++) {
        if (strcmp(arr[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}

int delete_by_index(Student *arr, int *n, int index) {
    if (index < 0 || index >= *n) {
        return -1;
    }
    for (int i = index; i < *n - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*n)--;
    return 0;
}

static char read_gender(const char *prompt, char default_gender) {
    char buf[16];
    while (1) {
        read_string(prompt, buf, sizeof(buf));
        if (buf[0] == '\0') {
            return default_gender;
        }
        char g = (char)toupper((unsigned char)buf[0]);
        if (g == 'M' || g == 'F') {
            return g;
        }
        printf("性别输入无效，请输入 M（男）或 F（女）。\n");
    }
}

static int read_age(const char *prompt, int default_age) {
    char buf[16];
    while (1) {
        read_string(prompt, buf, sizeof(buf));
        if (buf[0] == '\0') {
            return default_age;
        }
        int age = 0;
        if (sscanf(buf, "%d", &age) == 1 && age > 0 && age < 150) {
            return age;
        }
        printf("年龄输入无效，请输入 1-149 之间的整数。\n");
    }
}

int input_student(Student *s, const Student *arr, int n, int is_new) {
    (void)arr;
    (void)n;

    if (is_new) {
        while (1) {
            read_string("请输入学号: ", s->id, sizeof(s->id));
            if (s->id[0] == '\0') {
                printf("学号不能为空。\n");
                continue;
            }
            if (find_by_id(arr, n, s->id) >= 0) {
                printf("学号 %s 已存在，请重新输入。\n", s->id);
                continue;
            }
            break;
        }
        read_string("请输入姓名: ", s->name, sizeof(s->name));
        if (s->name[0] == '\0') {
            printf("姓名不能为空。\n");
            return -1;
        }

        char gbuf[16];
        while (1) {
            read_string("请输入性别 (M/F): ", gbuf, sizeof(gbuf));
            if (gbuf[0] == '\0') {
                printf("性别不能为空。\n");
                continue;
            }
            char g = (char)toupper((unsigned char)gbuf[0]);
            if (g == 'M' || g == 'F') {
                s->gender = g;
                break;
            }
            printf("性别输入无效，请输入 M 或 F。\n");
        }

        while (1) {
            s->age = read_int("请输入年龄: ", 1, 149);
            break;
        }
    }

    for (int i = 0; i < MAX_COURSE; i++) {
        char prompt[64];
        snprintf(prompt, sizeof(prompt), "请输入%s成绩 [0-100]: ", COURSE_NAMES[i]);
        if (is_new) {
            s->scores[i] = read_float(prompt, 0.0f, 100.0f);
        } else {
            snprintf(prompt, sizeof(prompt), "请输入%s成绩 [0-100] (回车保留 %.1f): ",
                     COURSE_NAMES[i], s->scores[i]);
            s->scores[i] = read_float_optional(prompt, s->scores[i], 0.0f, 100.0f);
        }
    }

    calc_total_avg(s);
    return 0;
}

void modify_by_index(Student *arr, int index) {
    Student *s = &arr[index];
    printf("\n--- 修改学生信息（回车保留原值）---\n");
    printf("学号: %s（不可修改）\n", s->id);

    read_string_optional("姓名: ", s->name, sizeof(s->name), s->name);
    s->gender = read_gender("性别 (M/F): ", s->gender);
    s->age = read_age("年龄: ", s->age);

    for (int i = 0; i < MAX_COURSE; i++) {
        char prompt[80];
        snprintf(prompt, sizeof(prompt), "%s成绩 (回车保留 %.1f): ",
                 COURSE_NAMES[i], s->scores[i]);
        s->scores[i] = read_float_optional(prompt, s->scores[i], 0.0f, 100.0f);
    }

    calc_total_avg(s);
    printf("修改成功！\n");
}
