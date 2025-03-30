#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "link_file.h"
#include "log.h"


// Check => create file 
void F_check_and_create_data(const char *file_name) {
    FILE *new_file;
    
    new_file = fopen(file_name, "w"); 
    LOG(LOG_LVL_DEBUG, "%s, %d: Start ", __func__, __LINE__);
    if (new_file != NULL) {
        //printf("File %s da duoc tao .\n", file_name);
        LOG(LOG_LVL_DEBUG, "%s, %d: Done create file", __func__, __LINE__);
        fclose(new_file);
    } else {
        //perror("Loi khi tao file ");
        LOG(LOG_LVL_ERROR, "%s, %d: Loi khi tao file ", __func__, __LINE__);
    }
    LOG(LOG_LVL_DEBUG, "%s, %d: END ", __func__, __LINE__);
}


// Open file

FILE *F_open_file(const char *file_name, const char *mode) {
    FILE *file = fopen(file_name, mode);
    LOG(LOG_LVL_DEBUG, "%s, %d: Start ", __func__, __LINE__);
    if (file == NULL) {
        //printf("No open file %s\n", file_name);
        LOG(LOG_LVL_ERROR, "%s, %d: No Open file", __func__, __LINE__);
        return NULL;
    }
    //printf("Open file successfully\n", file_name);
    LOG(LOG_LVL_DEBUG, "%s, %d: Open file successfully ", __func__, __LINE__);



    LOG(LOG_LVL_DEBUG, "%s, %d: END ", __func__, __LINE__);
    return file;

}


// write file

void F_write_file(FILE *file, const char *data) {
    LOG(LOG_LVL_DEBUG, "%s, %d: Start ", __func__, __LINE__);
    if (file == NULL) {
        LOG(LOG_LVL_ERROR, "%s, %d: Dont write to file", __func__, __LINE__);
    } 

    else if (fprintf(file, "%s", data) < 0){
        LOG(LOG_LVL_ERROR, "%s, %d: Write file to failed", __func__, __LINE__);
    }

    else {
        LOG(LOG_LVL_DEBUG, "%s, %d: DONE WRITE ", __func__, __LINE__);
    }
    LOG(LOG_LVL_DEBUG, "%s, %d: END ", __func__, __LINE__);
}


// Close file 

void F_close_file(FILE *file) {
    if (file != NULL) {

        if (fclose(file) == 0) {
            //printf("File closed successfully.\n");
            LOG(LOG_LVL_DEBUG, "%s, %d: File closed successfully ", __func__, __LINE__);
        } 
        else {
            //perror("Error closing file");
            LOG(LOG_LVL_ERROR, "%s, %d: Error closing file ", __func__, __LINE__);
        }
    } 
    else {
        //printf("Warning: Attempted to close a NULL file pointer.\n");
        LOG(LOG_LVL_WARN, "%s, %d: Attempted to close a NULL file pointer ", __func__, __LINE__);
    }
}


//Delete data file

void F_delete_data(const char *filename) {
    //   I. Define
    FILE *file;

    //   I Function
    file = fopen(filename, "r");  
    if (file == NULL) {
        LOG(LOG_LVL_ERROR, "%s, %d: Does not exit", __func__, __LINE__);
        return;
    }
    fclose(file);

    file = fopen(filename,"w");
    if(file == NULL){
        LOG(LOG_LVL_ERROR, "%s, %d: No open file", __func__, __LINE__);
        return;
    }
    fclose(file);
    LOG(LOG_LVL_DEBUG, "%s, %d: DONE DELETE DATA FILE ", __func__, __LINE__);
}
    // delete_file
void F_delete_file(const char *file_path) {
    if (remove(file_path) == 0) {
        LOG(LOG_LVL_DEBUG, "%s, %d: DONE DELETE FILE ", __func__, __LINE__);
    } else {
        LOG(LOG_LVL_ERROR, "%s, %d: Dont delete file", __func__, __LINE__);
    }
}