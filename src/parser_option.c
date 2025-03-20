#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "link_file.h"
#include "process_file.h"
#include "file.h"
#include "create_mail_from_name.h"
#include "interface.h"

// Chuan hoa 
char *PO_get_normalized_path(const char *file_name) {
    char *full_path = malloc(PO_MAX_PATH_LENGTH);
    if(!full_path){
        perror("Malloc failed");
        exit(1);
    }
    if (file_name[0] == '/') {
        strncpy(full_path, file_name, PO_MAX_PATH_LENGTH - 1);
    } else {
        snprintf(full_path, PO_MAX_PATH_LENGTH, "%s%s", PO_DEFAULT_PATH, file_name);
    }
    full_path[PO_MAX_PATH_LENGTH - 1] = '\0';
    return full_path;
}


void PO_parse_combined_options(int argc, char *argv[]) {
    static struct option long_options[] = {
        {"open-file", required_argument, 0, 'f'},
        {"mode", required_argument, 0, 'm'},
        {"data", required_argument, 0, 'd'},
        {"close", no_argument, 0, 'c'},
        {"delete-data", required_argument, 0, 'x'},  
        {"create", required_argument, 0, 'r'},
        
        {"delete-file", required_argument, 0, 'y'},
        {"show-interface", no_argument, 0, 's'},
        {"push-data", no_argument, 0, 'p'},
        {"extract", no_argument, 0, 'e'},
        {"help", no_argument, 0, 'h'},
        
        {0, 0, 0, 0} // Kết thúc mảng
    };

    int opt;
    int option_index = 0;
    const char *delete_data_file = NULL;
    const char *delete_file = NULL;
    const char *file_name = NULL;
    const char *file_data = NULL;
    const char *mode = "r";
    const char *data = NULL;
    FILE *file = NULL;
    bool do_push = false;
    bool extract_name = false;
    bool interface = false;
    opterr = 0;

    while ((opt = getopt_long(argc, argv, "f:m:d:r:x:y:cphes", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'f':
                file_name = optarg;
                break;
            case 'r':
                file_data = optarg;
                break;
            case 'm':
                mode = optarg;
                break;
            case 'd':
                data = optarg;
                break;
            case 'c':
                if (file != NULL) {
                    F_close_file(file);
                    file = NULL;
                } else {
                    printf("No file open to close.\n");
                }
                break;
            case 'y':
                delete_file = optarg;
                break;
            case 'x':
                delete_data_file = optarg;
                break;
            case 'p':
                do_push = true;
                break;
            case 'e':
                extract_name = true;
                break;
            case 's':
                interface = true;
                break;
            case 'h':
                printf("Cách dùng: %s [tùy chọn] [tên...]\n", argv[0]);
                printf("Tùy chọn:\n");
                printf("  -f file_name          Chỉ định tên file\n");
                printf("  -m mode               Chỉ định chế độ mở file (mặc định: w)\n");
                printf("  -d data               Dữ liệu để ghi vào file\n");
                printf("  -c                    Đóng file hiện tại\n");
                printf("  -x                    Xóa file\n");
                printf("  -p, --push-data       Đẩy dữ liệu từ name_data đến mail_data\n");
                printf("  -h, --help            Hiển thị thông báo trợ giúp này\n");
                printf("  -r, --create          Tao file trong thu muc con \n");
                exit(0);
            case '?':
                if (optopt == 'f' || optopt == 'm' || optopt == 'd') {
                    fprintf(stderr, "Tùy chọn -%c yêu cầu đối số.\n", optopt);
                } else {
                    fprintf(stderr, "Tùy chọn không hợp lệ: -%c\n", optopt);
                }
                exit(1);
            default:
                break;
        }
    }
    if (file_name != NULL) {
        file = F_open_file(PO_get_normalized_path(file_name), mode);
        if (data != NULL) {
            F_write_file(file, data);
        }
        else if (file != NULL) {     
            F_close_file(file);
            file = NULL;
        }
    
    }
    if(file_data !=NULL) {
        F_check_and_create_data(PO_get_normalized_path(file_data));
        free(PO_get_normalized_path(file_data));
    }
    if(delete_data_file != NULL){
        F_delete_data(PO_get_normalized_path(delete_data_file));
        free(PO_get_normalized_path(delete_data_file));
    }
    if(delete_file != NULL){
        PF_delete_file(PO_get_normalized_path(delete_file));
        free(PO_get_normalized_path(delete_file));
    }
    if (do_push) {
        if (optind + 1 < argc) { 
            CMFN_push_mail_data(PO_get_normalized_path(argv[optind]), PO_get_normalized_path(argv[optind + 1]));        
            free(PO_get_normalized_path(argv[optind]));
            free(PO_get_normalized_path(argv[optind+1]));       
        } 
        else {
            fprintf(stderr, "Thiếu đối số cho -p: cần name_file và mail_file\n");
            exit(1); 
        }
    }
    if (extract_name){
        if (optind + 1 < argc){
            CMFN_extract_names(PO_get_normalized_path(argv[optind]),PO_get_normalized_path(argv[optind+1]));
            free(PO_get_normalized_path(argv[optind]));
            free(PO_get_normalized_path(argv[optind+1]));
        }
        else {
            fprintf(stderr, "Thiếu đối số cho -e: cần input_file và output_file\n");
            exit(1); 
        }
    }
    if (interface){
        if (optind + 1 < argc){
            ITF_process_interface(PO_get_normalized_path(argv[optind]),PO_get_normalized_path(argv[optind+1]));
            free(PO_get_normalized_path(argv[optind]));
            free(PO_get_normalized_path(argv[optind+1]));
        }
        else {
            fprintf(stderr, "Thiếu đối số cho -s: cần input_interface và output_interface\n");
            exit(1); 
        }
    }
}




















// // parser option cho open_file
// void parse_options(int argc, char *argv[]) {
//     int opt;
//     int delete_file = 0;
//     const char *file_name = NULL;
//     const char *mode = "w"; 
//     const char *data = NULL;
//     FILE *file = NULL;
//     char full_path[MAX_PATH_LENGTH];



//     while ((opt = getopt(argc, argv, "f:m:d:cx")) != -1) {
//         switch (opt) {
//             case 'f':
//                 //opttarg la gia tri tiep theo sau cac tuy chon
//                 file_name = optarg;
//                 get_full_path(full_path, file_name);
//                 break;
//             case 'm':
//                 mode = optarg;
//                 break;
//             case 'd':
//                 data = optarg;
//                 break;
//             case 'c':
//                 if (file != NULL) {
//                     close_file(file);
//                 } else {
//                     printf("No file open to close.\n");
//                 }
//                 return; 
//             case 'x':
//                 delete_file = 1;
//                 break;
//             default:
//                 fprintf(stderr, "Usage: %s -f file_name -m mode -d data [-c]\n", argv[0]);
//                 //exit(EXIT_FAILURE);
//         }
//     }

//     if (file_name == NULL) {
//         fprintf(stderr, "Usage: %s -f file_name -m mode -d data [-c]\n", argv[0]);
//         //exit(EXIT_FAILURE);
//     }

//     file = open_file(full_path, mode);
//     if (data != NULL) {
//         write_file(file, data);
//     }
//     if(delete_file){
//         delete_data(full_path);
//         //return;
//     }

//     close_file(file);
// }


// void get_full_path(char *full_path, const char *file_name) {
//     snprintf(full_path, MAX_PATH_LENGTH, "/path/to/%s", file_name);
// }

// FILE* open_file(const char *full_path, const char *mode) {
//     return fopen(full_path, mode);
// }

// void write_file(FILE *file, const char *data) {
//     fprintf(file, "%s", data);
// }

// void close_file(FILE *file) {
//     if (file) fclose(file);
// }

// void delete_data(const char *full_path) {
//     remove(full_path);
// }

// void generate_email(const char *name) {
//     printf("Tạo email cho: %s\n", name);
// }

// void push_mail_data() {
//     printf("Đẩy dữ liệu từ name_data đến mail_data\n");
// }