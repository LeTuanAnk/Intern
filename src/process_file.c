#include <stdio.h>
#include <sys/stat.h> 
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include "link_file.h"


void PF_extract_substring(const char *input_str, int start_position, int sub_length) {
    // I. define
    int input_length;
    char *substring;

    // II. function
    if (!input_str || start_position < 0 || sub_length <= 0) {
        printf("Tham so khong hop le.\n");
        return;
    }

    input_length = strlen(input_str);
    if (start_position >= input_length) {
        printf("Vi tri ban dau vuot qua do dai chuoi.\n");
        return;
    }

    if (start_position + sub_length > input_length) {
        sub_length = input_length - start_position;
    }

    substring = (char *)malloc(sub_length + 1);
    if (!substring) {
        printf("khong the cap phat bo nho");
        return; 
    }

    strncpy(substring, input_str + start_position, sub_length);
    substring[sub_length] = '\0'; 
    printf("Chuoi con: %s \n", substring);

    free(substring);
}

void PF_replace_word(const char *input_str, const char *old_word, const char *new_word) {
    // I. Define
    char *result;
    int input_length, old_length, new_length;
    int count = 0;
    const char *ptr;

    // II.Function
    if (!input_str || !old_word || !new_word) {
        printf("Lỗi: Tham số không hợp lệ.\n");
        return;
    }

    input_length = strlen(input_str);
    old_length = strlen(old_word);
    new_length = strlen(new_word);

    if (old_length == 0) {
        printf("Lỗi: Từ cần thay thế không hợp lệ.\n");
        return;
    }

    ptr = input_str;
    while ((ptr = strstr(ptr, old_word)) != NULL) {
        count++;
        ptr += old_length;
    }

   
    result = (char *)malloc(input_length + count * (new_length - old_length) + 1);
    if (!result) {
        printf("Lỗi: Không thể cấp phát bộ nhớ.\n");
        return;
    }

    
    char *current = result;
    while (*input_str) {
        if (strstr(input_str, old_word) == input_str) { 
            strcpy(current, new_word);
            current += new_length;
            input_str += old_length;
        } else {
            *current++ = *input_str++;
        }
    }
    *current = '\0';

    printf("Chuỗi sau khi thay thế: \"%s\"\n", result);
    free(result);
}



