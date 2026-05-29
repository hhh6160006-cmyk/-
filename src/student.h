#ifndef STUDENT_H
#define STUDENT_H

#define MAX_NAME    32
#define MAX_COURSE  5
#define MAX_STUDENT 100
#define DATA_FILE   "data/students.dat"

typedef struct {
    char  id[12];
    char  name[MAX_NAME];
    char  gender;
    int   age;
    float scores[MAX_COURSE];
    float total;
    float average;
} Student;

extern const char *COURSE_NAMES[MAX_COURSE];

void calc_total_avg(Student *s);
int  add_student(Student *arr, int *n, Student new_one);
int  find_by_id(const Student *arr, int n, const char *id);
int  delete_by_index(Student *arr, int *n, int index);
void modify_by_index(Student *arr, int index);
int  input_student(Student *s, const Student *arr, int n, int is_new);

#endif
