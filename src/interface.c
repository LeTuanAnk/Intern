#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link_file.h"





void ITF_run_ifconfig_with_sudo(const char *options) {
    char command[256];

    if (options == NULL || options[0] == '\0') {
        snprintf(command, sizeof(command), "echo '1\n' | sudo -S ifconfig > ../database/tmp_ifconfig.txt");
    } else {
        snprintf(command, sizeof(command), "echo '1\n' | sudo -S ifconfig %s > ../database/tmp_ifconfig.txt", options);
    }

    system(command);
}

void ITF_process_interface(const char *input_file_interface, const char *output_file_interface) {
    FILE *PI_input_file_interface = fopen(input_file_interface, "r");
    FILE *PI_output_file_interface = fopen(output_file_interface, "w");
    
    char line[ITF_MAX_LINE];
    char interface[50] = "";

    while (fgets(line, sizeof(line), PI_input_file_interface)) {
        if (line[0] == '\n') continue; 
        if (line[0] != ' ') {
            sscanf(line, "%s", interface);
            fprintf(PI_output_file_interface, "\nThông tin của %s:\n", interface);
        } 
        else {
            char *pos;
            char buffer[ITF_MAX_LINE];

           
            if ((pos = strstr(line, "Link encap:"))) {
                sscanf(pos, "Link encap:%s", buffer);
                fprintf(PI_output_file_interface, "Link encap: %s\n", buffer);
            }
            if ((pos = strstr(line, "inet addr:"))) {
                sscanf(pos, "inet addr:%s", buffer);
                fprintf(PI_output_file_interface, "inet addr: %s\n", buffer);
            }
            if ((pos = strstr(line, "inet6 addr:"))) {
                sscanf(pos, "inet6 addr:%s", buffer);
                fprintf(PI_output_file_interface, "inet6 addr: %s\n", buffer);
            }
            if ((pos = strstr(line, "Bcast:"))) {
                sscanf(pos, "Bcast:%s", buffer);
                fprintf(PI_output_file_interface, "Bcast: %s\n", buffer);
            }
            if ((pos = strstr(line, "Mask:"))) {
                sscanf(pos, "Mask:%s", buffer);
                fprintf(PI_output_file_interface, "Mask: %s\n", buffer);
            }
        } 
    }
    fclose(PI_input_file_interface);
    fclose(PI_output_file_interface);
    printf("Da trich xuat thong tin interface tu %s va ghi vao %s\n", input_file_interface, output_file_interface);
}