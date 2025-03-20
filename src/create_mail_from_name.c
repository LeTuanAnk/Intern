#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define MAX_LINE 256
#define MAX_WORDS 6
#define MIN_WORDS 3

// Kiểm tra xem một dòng có phải là tên hợp lệ không
int CMFN_is_valid_name(const char *line) {
    int word_count = 0;
    int i = 0;

    while (line[i] != '\0') {

        while (line[i] == ' ') i++;

        if (word_count == 0) {
            if (!isupper(line[i])) return 0;
        } else {
            if (!isupper(line[i])) return 0;
        }
        i++;

        while (line[i] != ' ' && line[i] != '\0') {
            if (!islower(line[i])) return 0;
            i++;
        }

        word_count++;

        if (word_count > MAX_WORDS) return 0; 
    }
    return (word_count >= MIN_WORDS && word_count <= MAX_WORDS);
}


// Lọc tên từ file và ghi vào file name_file.txt
void CMFN_extract_names(const char *input_filename, const char *output_filename) {
    FILE *input_file = fopen(input_filename, "r");
    FILE *output_file = fopen(output_filename, "w");

    if (!input_file || !output_file) {
        perror("Lỗi mở file");
        return;
    }

    char line[MAX_LINE];
    int count = 0;

    while (fgets(line, sizeof(line), input_file)) {
        line[strcspn(line, "\n")] = 0;
        if (CMFN_is_valid_name(line)) {
            fprintf(output_file, "%s\n", line); 
            count++;
        }
    }
    printf("Đã lọc %d tên hợp lệ vào %s\n", count, output_filename);
    fclose(input_file);
    fclose(output_file);
}



// Hàm tạo email từ tên
void CMFN_generate_email(char *name, char *email, int email_size) {
    char temp[100];
    char *tokens[10]; 
    int token_count = 0;
    char *token;
    char *last_name ="";
    char initials[10] = {0}; 
    int initial_count = 0;
    



    strcpy(temp, name); 
    for (int i = 0; temp[i]; i++) {
        temp[i] = tolower(temp[i]);
    }


    token = strtok(temp," ");
    while (token != NULL && token_count < 10) {
        tokens[token_count++] = token;
        token = strtok(NULL, " ");
    }

    if (token_count == 0) {
        snprintf(email, email_size, "@gmail.com");
        return;
    }
    

    last_name = (token_count > 0) ? tokens[token_count - 1] : ""; 
    for (int i = 0; i < token_count - 1 && initial_count < 9; i++) {
        if (tokens[i][0]) { 
            initials[initial_count++] = tokens[i][0];
        }
    }
    initials[initial_count] = '\0'; 
   
    snprintf(email, email_size, "%s%s@gmail.com", last_name, initials);
}


void CMFN_push_mail_data(const char *name_file, const char *mail_file) {
    //  I. KHAI BAO DAU HAM
    FILE *source = fopen(name_file, "r");
    FILE *destination = fopen(mail_file, "w");
    char line[100];
    char email[100];

    //  II. CHUC NANG

    // 1. Mở file tên để đọc
    if (source == NULL) {
        printf("Khong the mo file nguon %s\n", name_file);
        return;
    }

    // 2. Mở file email để ghi
    if (destination == NULL) {
        printf("Khong the mo file dich %s\n", mail_file);
        fclose(source);
        return;
    }

    // 3. Đọc từng dòng và tạo email
    while (fgets(line, sizeof(line), source) != NULL) {
        // Xóa ký tự xuống dòng nếu có
        line[strcspn(line, "\n")] = 0;

        // Tạo email từ tên
        CMFN_generate_email(line, email, sizeof(email));

        // Ghi email vào file đích
        fprintf(destination, "%s\n", email);
    }

    // 4. Kiểm tra lỗi đọc
    if (ferror(source)) {
        printf("Loi khi doc du lieu tu %s\n", name_file);
    }

    // 5. Đóng file
    fclose(source);
    fclose(destination);
    printf("Da tao danh sach email tu %s va ghi vao %s\n", name_file, mail_file);
}



