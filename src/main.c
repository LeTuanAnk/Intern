#include <stdio.h>
#include "file.h"
#include "process_file.h"
#include "create_mail_from_name.h"
#include "parser_option.h"
#include "link_file.h"
#include "interface.h"
int main(int argc, char *argv[]){
    

    //check_and_create_data("data.txt");
    //file_push_to_data("../bin/output.txt","../database/data.txt");
    //push_mail_data("../bin/output.txt","../database/data.txt");
    //extract_names("../bin/input.txt","../bin/output.txt");

    ITF_run_ifconfig_with_sudo("");
    // FILE *input = fopen("data_before_extract.txt", "r");
    // FILE *output = fopen("output.txt", "w");
    PO_parse_combined_options(argc, argv);
    //PF_extract_substring("Le Tuan Anh", 3, 4);
    //PF_replace_word("le tuan anh, le duc trung, le thi nhu quynh","le" , "nguyen" );
    // process_interface(input, output);

    return 0;
}