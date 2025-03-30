#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link_file.h"
#include "cJSON.h"
#include "file.h"
#include "log.h"

//  Function JSN to file
void DSJ_printf_json_in_file(const char *filename, const char *json_string)
{
    LOG(LOG_LVL_DEBUG, "%s, %d: Start ", __func__, __LINE__);
    FILE *file = fopen(filename, "w");
    if (file)
    {
        fprintf(file, "%s", json_string);
        fclose(file);
    }
    LOG(LOG_LVL_DEBUG, "%s, %d: End ", __func__, __LINE__);
}

//  Function remote whitespace
void DSJ_trim_whitespace(char *str)
{
    LOG(LOG_LVL_DEBUG, "%s, %d: Start ", __func__, __LINE__);
    char *end;
    while (*str == ' ')
        str++;
    end = str + strlen(str) - 1;
    while (end > str && *end == ' ')
        end--;
    *(end + 1) = '\0';
    LOG(LOG_LVL_DEBUG, "%s, %d: End ", __func__, __LINE__);
}

void DSJ_convert_to_json(const char *input_file, const char *output_file)
{
    // Define
    LOG(LOG_LVL_DEBUG, "%s, %d: Start ", __func__, __LINE__);
    FILE *file;
    cJSON *root;
    cJSON *interfaces;
    cJSON *interface;
    char ID[32];
    char line[1024];
    char name[32];
    char mac[32];
    char ipv4[32];
    char bcast[32];
    char mask[32];
    char ipv6[128];
    char status[16];
    int parsed;
    char *json_string;

    // Logic process
    file = F_open_file(input_file, "r");
    if (!file)
    {
        // printf("Failed to open file.\n");
        LOG(LOG_LVL_ERROR, "%s, %d: Failed to open file\n", __func__, __LINE__);
        return;
    }

    root = cJSON_CreateObject();
    interfaces = cJSON_CreateArray();

    //  break header, whitespace
    fgets(line, sizeof(line), file); // Bỏ qua "Name MAC Address ..."
    fgets(line, sizeof(line), file); // Bỏ qua "-------------------"

    while (fgets(line, sizeof(line), file))
    {
        parsed = sscanf(line, "%5s %15s %17s %15s %15s %15s %63s %15s", ID,
                        name, mac, ipv4, bcast, mask, ipv6, status);

        if (parsed >= 7)
        { 
            DSJ_trim_whitespace(ID);
            DSJ_trim_whitespace(name);
            DSJ_trim_whitespace(mac);
            DSJ_trim_whitespace(ipv4);
            DSJ_trim_whitespace(bcast);
            DSJ_trim_whitespace(mask);
            DSJ_trim_whitespace(ipv6);
            DSJ_trim_whitespace(status);

            //  Create object JSON for interface
            interface = cJSON_CreateObject();
            cJSON_AddStringToObject(interface, "ID", strcmp(ID, "*") == 0 ? "" : ID);
            cJSON_AddStringToObject(interface, "Name", strcmp(name, "*") == 0 ? "" : name);
            cJSON_AddStringToObject(interface, "MAC Address", strcmp(mac, "*") == 0 ? "" : mac);
            cJSON_AddStringToObject(interface, "IPv4", strcmp(ipv4, "*") == 0 ? "" : ipv4);
            cJSON_AddStringToObject(interface, "Bcast", strcmp(bcast, "*") == 0 ? "" : bcast);
            cJSON_AddStringToObject(interface, "Mask", strcmp(mask, "*") == 0 ? "" : mask);
            cJSON_AddStringToObject(interface, "IPv6", strcmp(ipv6, "*") == 0 ? "" : ipv6);
            cJSON_AddStringToObject(interface, "Status", strcmp(status, "*") == 0 ? "" : status);
            cJSON_AddItemToArray(interfaces, interface);
        }
    }

    cJSON_AddItemToObject(root, "Interfaces", interfaces);
    json_string = cJSON_Print(root);
    if (json_string)
    {
        DSJ_printf_json_in_file(output_file, json_string);
        //printf("%s\n", json_string);
        free(json_string);
    }

    cJSON_Delete(root);
    fclose(file);
    LOG(LOG_LVL_DEBUG, "%s, %d: End ", __func__, __LINE__);
}