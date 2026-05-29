#ifndef UI_H
#define UI_H

#include "student.h"

void show_menu(void);
int  read_int(const char *prompt, int min, int max);
void read_string(const char *prompt, char *buf, int size);
float read_float(const char *prompt, float min, float max);
void read_string_optional(const char *prompt, char *buf, int size, const char *default_val);
float read_float_optional(const char *prompt, float default_val, float min, float max);
void show_all_students(const Student *arr, int n);
void pause_and_clear(void);
void clear_input_buffer(void);

#endif
