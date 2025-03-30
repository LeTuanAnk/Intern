#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <log.h> 
#include <link_file.h>



//check valid
int CMFN_is_valid_name(const char *line) {
    LOG(LOG_LVL_DEBUG, "%s, %d: START ", __func__, __LINE__);
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
    LOG(LOG_LVL_DEBUG, "%s, %d: END ", __func__, __LINE__);
    return (word_count >= MIN_WORDS && word_count <= MAX_WORDS);
}


//  filter name from file, write to file  
void CMFN_extract_names(const char *input_filename, const char *output_filename) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Start ", __func__, __LINE__);
    FILE *input_file = fopen(PATH_INPUT_NAME , "r");
    FILE *output_file = fopen(PATH_OUTPUT_NAME , "w");

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
    LOG(LOG_LVL_DEBUG, "%s, %d: DONE filter valid names", __func__, __LINE__);
    //printf("Đã lọc %d tên hợp lệ vào %s\n", count, output_filename);
    fclose(input_file);
    fclose(output_file);
    LOG(LOG_LVL_DEBUG, "%s, %d: END ", __func__, __LINE__);
}



//      create email from name
void CMFN_generate_email(char *name, char *email, int email_size) {
    LOG(LOG_LVL_DEBUG, "%s, %d: START ", __func__, __LINE__);

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

    LOG(LOG_LVL_DEBUG, "%s, %d: END ", __func__, __LINE__);
}


void CMFN_push_mail_data(const char *name_file, const char *mail_file) {
    //  I. define
    FILE *source = fopen(PATH_OUTPUT_NAME, "r");
    FILE *destination = fopen(PATH_OUTPUT_EMAIL, "w");
    char line[100];
    char email[100];

    //  II. function

    // 1. Open file => read
    if (source == NULL) {
        //printf("Khong the mo file nguon %s\n", name_file);
        LOG(LOG_LVL_ERROR, "%s, %d: NO open file SRC ", __func__, __LINE__);
        return;
    }

    // 2. Open file => write
    if (destination == NULL) {
        //printf("Khong the mo file dich %s\n", mail_file);
        LOG(LOG_LVL_ERROR, "%s, %d: No open file DEST ", __func__, __LINE__);
        fclose(source);
        return;
    }

    // 3.  read lline, create email
    while (fgets(line, sizeof(line), source) != NULL) {
        // remote charater line break 
        line[strcspn(line, "\n")] = 0;

        // create mail form nmae
        CMFN_generate_email(line, email, sizeof(email));

        // read email to dst file
        fprintf(destination, "%s\n", email);
    }

    // 4. check real error
    if (ferror(source)) {
        //printf("Loi khi doc du lieu tu %s\n", name_file);
        LOG(LOG_LVL_ERROR, "%s, %d: error write data ", __func__, __LINE__);
    }

    // 5. Close file
    fclose(source);
    fclose(destination);
    LOG(LOG_LVL_DEBUG, "%s, %d: Done create email and write to file", __func__, __LINE__);
    //printf("Da tao danh sach email tu %s va ghi vao %s\n", name_file, mail_file);
    LOG(LOG_LVL_DEBUG, "%s, %d: END ", __func__, __LINE__);
}



