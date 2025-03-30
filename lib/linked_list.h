#ifndef LINKED_LIST
#define LINKED_LIST
#include "get_interface.h"

struct interface_node* LL_creat_node_from_struct(interface_info* data);
void LL_append(struct interface_node** head, interface_info* data);
void LL_build_list_from_array(struct interface_node** head, interface_info interfaces[], int count);
// void LL_print_list(struct interface_node* head);
void LL_print_list(struct interface_node *head, const char *filename);
void LL_free_list(struct interface_node* head);

#endif