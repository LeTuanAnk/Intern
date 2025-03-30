#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include "link_file.h"
#include "process_file.h"
#include "file.h"
#include "log.h"
#include "create_mail_from_name.h"
#include "get_interface.h"
#include "data_swap_json.h"
#include "linked_list.h"

// Chuan hoa
char *PO_get_normalized_path(const char *file_name)
{
    char *full_path = malloc(PO_MAX_PATH_LENGTH);
    if (!full_path)
    {
        perror("Malloc failed");
        exit(1);
    }
    if (file_name[0] == '/')
    {
        strncpy(full_path, file_name, PO_MAX_PATH_LENGTH - 1);
    }
    else
    {
        snprintf(full_path, PO_MAX_PATH_LENGTH, "%s%s", PO_DEFAULT_PATH, file_name);
    }
    full_path[PO_MAX_PATH_LENGTH - 1] = '\0';
    return full_path;
}

void PO_parse_combined_options(int argc, char *argv[])
{
    static struct option long_options[] = {
        {"open-file", required_argument, 0, 'f'},
        {"mode", required_argument, 0, 'm'},
        {"data", required_argument, 0, 'd'},
        {"close", no_argument, 0, 'c'},
        {"delete-data", required_argument, 0, 'x'},
        {"create", required_argument, 0, 'r'},

        {"delete-file", required_argument, 0, 'y'},
        {"linked-list", no_argument, 0, 'l'},
        {"get-interface", no_argument, 0, 'g'},
        {"swap-data-json", no_argument, 0, 'j'},
        {"push-data", no_argument, 0, 'p'},
        {"extract", no_argument, 0, 'e'},
        {"help", no_argument, 0, 'h'},

        {0, 0, 0, 0} // End array
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
    bool data_json = false;
    bool linked_list = false;
    interface_info interfaces[MAX_INTERFACES];
    struct interface_node* head = NULL;
    int count = 0;
    opterr = 0;

    while ((opt = getopt_long(argc, argv, "f:m:d:r:x:y:cphegjl", long_options, &option_index)) != -1)
    {
        switch (opt)
        {
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
            if (file != NULL)
            {
                F_close_file(file);
                file = NULL;
            }
            else
            {
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
        case 'g':
            interface = true;
            break;
        case 'j':
            data_json = true;
            break;
        case 'l':
            linked_list = true;
            break;
        case 'h':
            printf("Cách dùng: %s [tùy chọn] [tên...]\n", argv[0]);
            printf("Tùy chọn:\n");
            printf("  -f [file_name]          Name file\n");
            printf("  -m [mode]               Mode file (default: w)\n");
            printf("  -d [data]               Write data in file\n");
            printf("  -y [file]               Delete file\n");
            printf("  -x [file]               Delete datafile\n");
            printf("  -p, --push-data         Push data from name_data to mail_data\n");
            printf("  -h, --help              Show this notification help\n");
            printf("  -r, --create            Create file \n");
            printf("  -c, --close             Close file present\n");
            printf("  -j, --swap-data-json    Data interface swap json\n");
            printf("  -g, --get-interface     Get interface\n");
            printf("  -e,  --extract          Extract file_name\n");
            exit(0);
        case '?':
            if (optopt == 'f' || optopt == 'm' || optopt == 'd')
            {
                fprintf(stderr, "option requires arguments-%c.\n", optopt);
            }
            else
            {
                fprintf(stderr, "options Invalid -%c\n", optopt);
            }
            exit(1);
        default:
            break;
        }
    }
    if (file_name != NULL)
    {
        file = F_open_file(PO_get_normalized_path(file_name), mode);
        if (data != NULL)
        {
            F_write_file(file, data);
        }
        else if (file != NULL)
        {
            F_close_file(file);
            file = NULL;
        }
    }
    else if (file_data != NULL)
    {
        F_check_and_create_data(PO_get_normalized_path(file_data));
        free(PO_get_normalized_path(file_data));
    }
    else if (delete_data_file != NULL)
    {
        F_delete_data(PO_get_normalized_path(delete_data_file));
        free(PO_get_normalized_path(delete_data_file));
    }
    else if (delete_file != NULL)
    {
        F_delete_file(PO_get_normalized_path(delete_file));
        free(PO_get_normalized_path(delete_file));
    }
    else if (extract_name)
    {

        CMFN_extract_names(PATH_INPUT_NAME, PATH_OUTPUT_NAME);
        if (do_push)
        {
            CMFN_push_mail_data(PATH_OUTPUT_NAME, PATH_OUTPUT_EMAIL);
        }
    }
    else if (do_push)
    {
        CMFN_push_mail_data(PATH_OUTPUT_NAME, PATH_OUTPUT_EMAIL);
    }
    else if (interface)
    {
        GI_get_network_info(interfaces, &count);
        GI_write_network_info_to_file(interfaces, count);
        system(GET_INTERFACE);
    }
    else if (data_json)
    {
        DSJ_convert_to_json(PATH_JSON_INPUT, PATH_JSON_OUTPUT);
        system(DATA_SWAP_JSON);
    }
    else if (linked_list)
    {
        GI_get_network_info(interfaces, &count);      
        LL_build_list_from_array(&head, interfaces, count); 
        LL_print_list(head,PATH_LINKED_LIST);
        LL_free_list(head);
        system(OPEN_PATH_LINKED_LIST);
    }
    else
    {
        LOG(LOG_LVL_WARN, "%s, %d: Usage: get_info [-R] MODE[,MODE]...", __func__, __LINE__);
        fprintf(stderr, "Usage: get_info [-R] MODE[,MODE]...\n");
        exit(1);
    }
}
