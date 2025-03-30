#ifndef GET_INTERFACE_H
#define GET_INTERFACE_H

typedef struct
{
    char name[16];
    char mac[18];
    char ipv4[16];
    char ipv6[50];
    char status[50];
    char bcast[16]; 
    char mask[16];  
} interface_info;

void GI_get_network_info(interface_info interfaces[], int *count);
void GI_write_network_info_to_file(interface_info interfaces[], int count);
#endif