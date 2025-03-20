#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link_file.h"



// Check => create file 
void F_check_and_create_data(const char *file_name) {
    FILE *new_file;

    new_file = fopen(file_name, "w"); 
    if (new_file != NULL) {
        printf("File %s da duoc tao .\n", file_name);
        fclose(new_file);
    } else {
        perror("Loi khi tao file ");
    }
}


// Open file

FILE *F_open_file(const char *file_name, const char *mode) {
    FILE *file = fopen(file_name, mode);

    if (file == NULL) {
        printf("No open file %s\n", file_name);
        return NULL;
    }
    printf("Open file %s successfully\n", file_name);
    return file;
}


// write file

void F_write_file(FILE *file, const char *data) {
    if (file == NULL) {
        printf("File khong hop le, khong the ghi\n");
    } 

    else if (fprintf(file, "%s", data) < 0){
        printf("Ghi du lieu vao file that bai\n");
    }

    else {
        printf("Da ghi du lieu vao file\n");
    }
}


// Close file 

void F_close_file(FILE *file) {
    if (file != NULL) {

        if (fclose(file) == 0) {
            printf("File closed successfully.\n");
        } 
        else {
            perror("Error closing file");
        }
    } 
    else {
        printf("Warning: Attempted to close a NULL file pointer.\n");
    }
}


//Delete data file

void F_delete_data(const char *filename) {
    //   I. Khai bao dau ham

    FILE *file;
    file = fopen(filename, "w"); 

    //   I Chuc nang 
    if (file == NULL) {
        printf("Khong the mo file %s de xoa du lieu\n", filename);
        return;
    }
    fclose(file);
    printf("Da xoa noi dung trong file %s\n", filename);
}