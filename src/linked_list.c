#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "get_interface.h"

//   Define structure to linked list
struct interface_node
{
    char name[10];
    char mac[18];
    char ipv4[16];
    char bcast[16];
    char mask[16];
    char ipv6[64];
    char status[5];
    struct interface_node *next;
};

// Function create node form struct interface_info
struct interface_node *LL_create_node_from_struct(interface_info *data)
{
    struct interface_node *newNode = (struct interface_node *)malloc(sizeof(struct interface_node));
    if (newNode == NULL)
    {
        printf("Dont dymamic memory\n");
        exit(1);
    }
    strncpy(newNode->name, data->name, sizeof(newNode->name) - 1);
    newNode->name[sizeof(newNode->name) - 1] = '\0';
    strncpy(newNode->mac, data->mac, sizeof(newNode->mac) - 1);
    newNode->mac[sizeof(newNode->mac) - 1] = '\0';
    strncpy(newNode->ipv4, data->ipv4, sizeof(newNode->ipv4) - 1);
    newNode->ipv4[sizeof(newNode->ipv4) - 1] = '\0';
    strncpy(newNode->bcast, data->bcast, sizeof(newNode->bcast) - 1);
    newNode->bcast[sizeof(newNode->bcast) - 1] = '\0';
    strncpy(newNode->mask, data->mask, sizeof(newNode->mask) - 1);
    newNode->mask[sizeof(newNode->mask) - 1] = '\0';
    strncpy(newNode->ipv6, data->ipv6, sizeof(newNode->ipv6) - 1);
    newNode->ipv6[sizeof(newNode->ipv6) - 1] = '\0';
    strncpy(newNode->status, data->status, sizeof(newNode->status) - 1);
    newNode->status[sizeof(newNode->status) - 1] = '\0';
    newNode->next = NULL;
    return newNode;
}

//  function add node to end of the list
void append(struct interface_node **head, interface_info *data)
{
    struct interface_node *newNode = LL_create_node_from_struct(data);
    if (*head == NULL)
    {
        *head = newNode;
        return;
    }
    struct interface_node *current = *head;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = newNode;
}

// Function to build a list from the interface_info array
void LL_build_list_from_array(struct interface_node **head, interface_info interfaces[], int count)
{
    for (int i = 0; i < count; i++)
    {
        append(head, &interfaces[i]); // Add each element from the array to the list
    }
}

// printf to screen
// void LL_print_list(struct interface_node* head) {
//     struct interface_node* current = head;
//     printf("Name            MAC Address        IPv4            Bcast           Mask            IPv6                                               Status\n");
//     printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
//     while (current != NULL) {
//         printf("%-15s %-18s %-15s %-15s %-15s %-50s %-5s\n",
//                current->name, current->mac, current->ipv4, current->bcast,
//                current->mask, current->ipv6, current->status);
//         current = current->next;
//     }
// }
// printf to file

void LL_print_list(struct interface_node *head, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        return;
    }

    struct interface_node *current = head;
    fprintf(file, "Name            MAC Address        IPv4            Bcast           Mask            IPv6                                               Status\n");
    fprintf(file, "--------------------------------------------------------------------------------------------------------------------------------------------\n");

    while (current != NULL)
    {
        fprintf(file, "%-15s %-18s %-15s %-15s %-15s %-50s %-5s\n",
                current->name, current->mac, current->ipv4, current->bcast,
                current->mask, current->ipv6, current->status);
        current = current->next;
    }

    fclose(file);
}

// functions memory free
void LL_free_list(struct interface_node *head)
{
    struct interface_node *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}