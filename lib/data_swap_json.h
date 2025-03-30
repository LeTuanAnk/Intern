#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdio.h>


void DSJ_printf_json_in_file(const char *filename, const char *json_string);
void DSJ_trim_whitespace(char *str);
void DSJ_convert_to_json(const char *input_file, const char *output_file);

#endif