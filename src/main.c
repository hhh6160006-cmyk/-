#include <stdio.h>
#include <string.h>
#include "student.h"
#include "ui.h"
#include "file_io.h"
#include "stats.h"

static void handle_add(Student *students, int *count) {
    if (*count >= MAX_STUDENT) {
        printf("错误：学生数量已达上限 %d。\n", MAX_STUDENT);
        return;
    }
    Student s;
    memset(&s, 0, sizeof(s));
    if (input_student(&s, students, *count, 1) == 0) {
        if (add_student(students, count, s) == 0) {
            printf("录入成功！\n");
        }
    }
}

static void handle_find(const Student *students, int count) {
    char id[12];
    read_string("请输入学号: ", id, sizeof(id));
    int idx = find_by_id(students, count, id);
    if (idx < 0) {
        printf("未找到学号 %s 的学生。\n", id);
        return;
    }
    printf("\n找到学生：\n");
    show_all_students(&students[idx], 1);
}

static void handle_modify(Student *students, int count) {
    char id[12];
    read_string("请输入要修改的学号: ", id, sizeof(id));
    int idx = find_by_id(students, count, id);
    if (idx < 0) {
        printf("未找到学号 %s 的学生。\n", id);
        return;
    }
    modify_by_index(students, idx);
}

static void handle_delete(Student *students, int *count) {
    char id[12];
    read_string("请输入要删除的学号: ", id, sizeof(id));
    int idx = find_by_id(students, *count, id);
    if (idx < 0) {
        printf("未找到学号 %s 的学生。\n", id);
        return;
    }
    printf("确认删除 %s [%s]? (Y/N): ", students[idx].name, students[idx].id);
    char confirm[8];
    read_string("", confirm, sizeof(confirm));
    if (confirm[0] == 'Y' || confirm[0] == 'y') {
        delete_by_index(students, count, idx);
        printf("删除成功！\n");
    } else {
        printf("已取消删除。\n");
    }
}

static void handle_sort(Student *students, int count) {
    if (count <= 1) {
        printf("学生数量不足，无需排序。\n");
        return;
    }

    printf("\n排序方式：\n");
    printf("  1. 按总分降序\n");
    printf("  2. 按学号升序\n");
    printf("  3. 按姓名字典序\n");
    int key = read_int("请选择 [1-3]: ", 1, 3);

    CmpFunc cmp = cmp_by_total;
    if (key == 2) cmp = cmp_by_id;
    if (key == 3) cmp = cmp_by_name;

    Student backup[MAX_STUDENT];
    memcpy(backup, students, sizeof(Student) * (size_t)count);

    memcpy(students, backup, sizeof(Student) * (size_t)count);
    bubble_sort_timed(students, count, cmp);

    memcpy(students, backup, sizeof(Student) * (size_t)count);
    selection_sort_timed(students, count, cmp);

    printf("\n已使用选择排序结果展示：\n");
    show_all_students(students, count);
}

static void handle_stats(const Student *students, int count) {
    show_stats(students, count);

    printf("\n是否查看分数段直方图？(Y/N): ");
    char buf[8];
    read_string("", buf, sizeof(buf));
    if (buf[0] == 'Y' || buf[0] == 'y') {
        printf("\n选择课程：\n");
        for (int i = 0; i < MAX_COURSE; i++) {
            printf("  %d. %s\n", i + 1, COURSE_NAMES[i]);
        }
        int c = read_int("请选择 [1-5]: ", 1, MAX_COURSE);
        show_histogram(students, count, c - 1);
    }
}

static int read_menu_choice(void) {
    char buf[16];
    read_string("请输入选项[0-9/a/b/c]: ", buf, sizeof(buf));
    if (buf[0] == '\0') return -1;

    if (buf[0] >= '0' && buf[0] <= '9' && buf[1] == '\0') {
        return buf[0] - '0';
    }
    if ((buf[0] == 'a' || buf[0] == 'A') && buf[1] == '\0') return 10;
    if ((buf[0] == 'b' || buf[0] == 'B') && buf[1] == '\0') return 11;
    if ((buf[0] == 'c' || buf[0] == 'C') && buf[1] == '\0') return 12;

    return -1;
}

int main(void) {
    Student students[MAX_STUDENT];
    int count = 0;

    if (load_from_file(students, &count, DATA_FILE) == 0) {
        printf("已从 %s 加载 %d 条历史记录。\n", DATA_FILE, count);
    } else {
        printf("无历史数据，从空开始。\n");
        count = 0;
    }

    while (1) {
        show_menu();
        int choice = read_menu_choice();

        if (choice < 0) {
            printf("输入无效，请输入 0-9 或 a/b/c。\n");
            pause_and_clear();
            continue;
        }

        switch (choice) {
        case 1:  handle_add(students, &count); break;
        case 2:  show_all_students(students, count); break;
        case 3:  handle_find(students, count); break;
        case 4:  handle_modify(students, count); break;
        case 5:  handle_delete(students, &count); break;
        case 6:  handle_sort(students, count); break;
        case 7:  handle_stats(students, count); break;
        case 8:  save_to_file(students, count, DATA_FILE); break;
        case 9:
            if (load_from_file(students, &count, DATA_FILE) == 0) {
                printf("成功读取 %d 条记录。\n", count);
            } else {
                printf("读取失败，文件可能不存在。\n");
            }
            break;
        case 10: {
            char keyword[MAX_NAME];
            read_string("请输入姓名关键词: ", keyword, sizeof(keyword));
            fuzzy_search(students, count, keyword);
            break;
        }
        case 11: {
            char path[128];
            read_string("请输入导出路径 [data/students.csv]: ", path, sizeof(path));
            if (path[0] == '\0') strcpy(path, "data/students.csv");
            export_csv(students, count, path);
            break;
        }
        case 12: {
            char path[128];
            read_string("请输入导入路径 [data/students.csv]: ", path, sizeof(path));
            if (path[0] == '\0') strcpy(path, "data/students.csv");
            import_csv(students, &count, path);
            break;
        }
        case 0:
            save_to_file(students, count, DATA_FILE);
            printf("感谢使用，再见！\n");
            return 0;
        default:
            printf("选项 %d 无效，请重新输入。\n", choice);
            break;
        }

        pause_and_clear();
    }

    return 0;
}
