#ifndef FILE_H
#define FILE_H
#include <stdio.h>

void F_check_and_create_data(const char *file_name);
FILE *F_open_file(const char *file_name, const char *mode);
void F_write_file(FILE *file, const char *data);
void F_close_file(FILE *file);
void F_delete_data(const char *filename);
void F_delete_file(const char *file_path);
#endif