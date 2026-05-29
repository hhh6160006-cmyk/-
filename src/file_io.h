#ifndef FILE_IO_H
#define FILE_IO_H

#include "student.h"

int save_to_file(const Student *arr, int n, const char *path);
int load_from_file(Student *arr, int *n, const char *path);
int export_csv(const Student *arr, int n, const char *path);
int import_csv(Student *arr, int *n, const char *path);

#endif
