#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

static int ensure_data_dir(void) {
    MKDIR("data");
    return 0;
}

int save_to_file(const Student *arr, int n, const char *path) {
    ensure_data_dir();
    FILE *fp = fopen(path, "wb");
    if (!fp) {
        printf("错误：无法打开文件 %s 进行写入。\n", path);
        return -1;
    }
    if (fwrite(&n, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }
    if (n > 0 && fwrite(arr, sizeof(Student), (size_t)n, fp) != (size_t)n) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    printf("成功保存 %d 条记录到 %s\n", n, path);
    return 0;
}

int load_from_file(Student *arr, int *n, const char *path) {
    FILE *fp = fopen(path, "rb");
    if (!fp) {
        return -1;
    }
    int count = 0;
    if (fread(&count, sizeof(int), 1, fp) != 1) {
        fclose(fp);
        return -1;
    }
    if (count < 0 || count > MAX_STUDENT) {
        fclose(fp);
        printf("错误：文件数据损坏（学生数量异常）。\n");
        return -1;
    }
    if (count > 0) {
        if (fread(arr, sizeof(Student), (size_t)count, fp) != (size_t)count) {
            fclose(fp);
            return -1;
        }
    }
    fclose(fp);
    *n = count;
    return 0;
}

int export_csv(const Student *arr, int n, const char *path) {
    ensure_data_dir();
    FILE *fp = fopen(path, "w");
    if (!fp) {
        printf("错误：无法创建 CSV 文件 %s\n", path);
        return -1;
    }

    fprintf(fp, "学号,姓名,性别,年龄");
    for (int i = 0; i < MAX_COURSE; i++) {
        fprintf(fp, ",%s", COURSE_NAMES[i]);
    }
    fprintf(fp, ",总分,平均分\n");

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%s,%s,%c,%d",
                arr[i].id, arr[i].name, arr[i].gender, arr[i].age);
        for (int j = 0; j < MAX_COURSE; j++) {
            fprintf(fp, ",%.1f", arr[i].scores[j]);
        }
        fprintf(fp, ",%.1f,%.2f\n", arr[i].total, arr[i].average);
    }

    fclose(fp);
    printf("成功导出 %d 条记录到 %s\n", n, path);
    return 0;
}

static char *trim(char *s) {
    while (*s == ' ' || *s == '\t' || *s == '\r') {
        s++;
    }
    char *end = s + strlen(s) - 1;
    while (end > s && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n')) {
        *end = '\0';
        end--;
    }
    return s;
}

int import_csv(Student *arr, int *n, const char *path) {
    FILE *fp = fopen(path, "r");
    if (!fp) {
        printf("错误：无法打开 CSV 文件 %s\n", path);
        return -1;
    }

    char line[512];
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        return -1;
    }

    int imported = 0;
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (line[0] == '\n' || line[0] == '\r') {
            continue;
        }

        Student s;
        memset(&s, 0, sizeof(s));

        char *token = strtok(line, ",\n\r");
        if (!token) continue;
        strncpy(s.id, trim(token), sizeof(s.id) - 1);

        token = strtok(NULL, ",\n\r");
        if (!token) continue;
        strncpy(s.name, trim(token), sizeof(s.name) - 1);

        token = strtok(NULL, ",\n\r");
        if (!token) continue;
        s.gender = token[0];

        token = strtok(NULL, ",\n\r");
        if (!token) continue;
        s.age = atoi(token);

        for (int i = 0; i < MAX_COURSE; i++) {
            token = strtok(NULL, ",\n\r");
            if (!token) break;
            s.scores[i] = (float)atof(token);
        }

        calc_total_avg(&s);

        if (*n >= MAX_STUDENT) {
            printf("警告：已达上限，停止导入。\n");
            break;
        }
        if (find_by_id(arr, *n, s.id) >= 0) {
            printf("跳过重复学号: %s\n", s.id);
            continue;
        }
        arr[*n] = s;
        (*n)++;
        imported++;
    }

    fclose(fp);
    printf("成功从 CSV 导入 %d 条记录。\n", imported);
    return 0;
}
