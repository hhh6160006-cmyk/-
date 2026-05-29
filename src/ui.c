#include "ui.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
#include <windows.h>
#define CLEAR_SCREEN() system("cls")
#else
#define CLEAR_SCREEN() system("clear")
#endif

void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard */
    }
}

void read_string(const char *prompt, char *buf, int size) {
    if (size <= 0) {
        return;
    }
    printf("%s", prompt);
    if (fgets(buf, size, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    int len = (int)strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
}

void read_string_optional(const char *prompt, char *buf, int size, const char *default_val) {
    char tmp[256];
    printf("%s", prompt);
    if (fgets(tmp, sizeof(tmp), stdin) == NULL) {
        strncpy(buf, default_val, (size_t)size - 1);
        buf[size - 1] = '\0';
        return;
    }
    int len = (int)strlen(tmp);
    if (len > 0 && tmp[len - 1] == '\n') {
        tmp[len - 1] = '\0';
    }
    if (tmp[0] == '\0') {
        strncpy(buf, default_val, (size_t)size - 1);
        buf[size - 1] = '\0';
    } else {
        strncpy(buf, tmp, (size_t)size - 1);
        buf[size - 1] = '\0';
    }
}

int read_int(const char *prompt, int min, int max) {
    char buf[64];
    while (1) {
        read_string(prompt, buf, sizeof(buf));
        char *end = NULL;
        long val = strtol(buf, &end, 10);
        if (end != buf && *end == '\0' && val >= min && val <= max) {
            return (int)val;
        }
        printf("输入无效，请输入 %d-%d 之间的整数。\n", min, max);
    }
}

float read_float(const char *prompt, float min, float max) {
    char buf[64];
    while (1) {
        read_string(prompt, buf, sizeof(buf));
        char *end = NULL;
        float val = (float)strtod(buf, &end);
        if (end != buf && *end == '\0' && val >= min && val <= max) {
            return val;
        }
        printf("输入无效，请输入 %.1f-%.1f 之间的数字。\n", min, max);
    }
}

float read_float_optional(const char *prompt, float default_val, float min, float max) {
    char buf[64];
    printf("%s", prompt);
    if (fgets(buf, sizeof(buf), stdin) == NULL) {
        return default_val;
    }
    int len = (int)strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    }
    if (buf[0] == '\0') {
        return default_val;
    }
    char *end = NULL;
    float val = (float)strtod(buf, &end);
    if (end != buf && *end == '\0' && val >= min && val <= max) {
        return val;
    }
    printf("输入无效，保留原值 %.1f。\n", default_val);
    return default_val;
}

void show_menu(void) {
    printf("\n");
    printf("============================================\n");
    printf("        欢迎使用学生成绩管理系统 v1.0\n");
    printf("============================================\n");
    printf("  1. 录入学生成绩\n");
    printf("  2. 显示全部学生\n");
    printf("  3. 按学号查找学生\n");
    printf("  4. 修改学生成绩\n");
    printf("  5. 删除学生\n");
    printf("  6. 排序（按总分/学号/姓名）\n");
    printf("  7. 统计（最高/最低/平均/不及格）\n");
    printf("  8. 保存到文件\n");
    printf("  9. 从文件读取\n");
    printf("  a. 模糊查询（按姓名）\n");
    printf("  b. 导出 CSV\n");
    printf("  c. 导入 CSV\n");
    printf("  0. 退出系统\n");
    printf("============================================\n");
}

void show_all_students(const Student *arr, int n) {
    if (n == 0) {
        printf("\n当前没有学生记录。\n");
        return;
    }

    printf("\n");
    printf("%-12s %-10s %-4s %-4s", "学号", "姓名", "性别", "年龄");
    for (int i = 0; i < MAX_COURSE; i++) {
        printf(" %-6s", COURSE_NAMES[i]);
    }
    printf(" %-8s %-8s\n", "总分", "平均");
    printf("------------------------------------------------------------------------"
           "----------------\n");

    for (int i = 0; i < n; i++) {
        printf("%-12s %-10s %-4c %-4d",
               arr[i].id, arr[i].name, arr[i].gender, arr[i].age);
        for (int j = 0; j < MAX_COURSE; j++) {
            printf(" %-6.1f", arr[i].scores[j]);
        }
        printf(" %-8.1f %-8.2f\n", arr[i].total, arr[i].average);
    }
    printf("共 %d 名学生。\n", n);
}

void pause_and_clear(void) {
    printf("\n按回车键继续...");
    clear_input_buffer();
    getchar();
    CLEAR_SCREEN();
}
