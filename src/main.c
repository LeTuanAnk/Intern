#include <stdio.h>
#include "log.h"
#include "file.h"
#include "process_file.h"
#include "create_mail_from_name.h"
#include "parser_option.h"
#include "link_file.h"
#include "data_swap_json.h"
#include "get_interface.h"
#include "linked_list.h"
int main(int argc, char *argv[])
{

    // PF_extract_substring("Le Tuan Anh", 3, 4);
    // log_enable = 1;PF_replace_word("le tuan anh, le duc trung, le thi nhu quynh","le" , "nguyen" );
    LOG_set_level(LOG_LVL_DEBUG);
    LOG(LOG_LVL_DEBUG, "%s, %d: Start ", __func__, __LINE__);

    // interface_info interfaces[MAX_INTERFACES];
    // int count = 0;
    // get_network_info(interfaces, &count);
    // write_network_info_to_file(interfaces, count);
    // system(GET_INTERFACE);
    // check_and_create_data("data.txt");
    // file_push_to_data("../bin/output.txt","../database/data.txt");
    // push_mail_data("../bin/output.txt","../database/data.txt");
    // extract_names("../bin/input.txt","../bin/output.txt");
    // ITF_run_ifconfig_with_sudo("");
    //  FILE *input = fopen("data_before_extract.txt", "r");
    //  FILE *output = fopen("output.txt", "w");
    // int num_interfaces = 0;
    // interface_info *interfaces = get_interface_info(&num_interfaces);

    PO_parse_combined_options(argc, argv);
    LOG(LOG_LVL_DEBUG, "%s, %d: End ", __func__, __LINE__);
    return 0;
}