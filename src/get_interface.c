#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "link_file.h"
#include "get_interface.h"
#include "log.h"

void GI_get_network_info(interface_info interfaces[], int *count)
{
    FILE *fp;
    char buffer[BUFFER_SIZE];
    char *token;
    char buffer_copy[256];
    char mac_copy[64];
    char *mac_pointer;
    // *rx_bytes_pointer, *tx_bytes_pointer;
    int iface_index = -1;

    LOG(LOG_LVL_DEBUG, "%s, %d: Start", __func__, __LINE__);
    // fp = popen(RUN, "r");
    fp = fopen(PATH_DATA_INTERFACE_TEST, "r");
    if (!fp)
    {
        LOG(LOG_LVL_ERROR, "%s, %d: popen failed", __func__, __LINE__);
        exit(1);
    }

    while (fgets(buffer, BUFFER_SIZE, fp))
    {
        // buffer[strcspn(buffer, "\n")];
        buffer[strcspn(buffer, "\n")] = '\0';

        // Bắt đầu interface mới
        if (buffer[0] != ' ' && strstr(buffer, "Link encap:"))
        {
            iface_index++;

            // Lấy tên interface
            strncpy(buffer_copy, buffer, sizeof(buffer_copy) - 1);
            buffer_copy[sizeof(buffer_copy) - 1] = '\0';

            // Lấy tên interface (br0)
            token = strtok(buffer_copy, " ");
            if (token)
            {
                size_t name_len = sizeof(interfaces[iface_index].name) - 1;
                strncpy(interfaces[iface_index].name, token, name_len);
                interfaces[iface_index].name[name_len] = '\0';
            }

            // Tìm HWaddr (Địa chỉ MAC)
            mac_pointer = strstr(buffer, "HWaddr");
            if (mac_pointer)
            {
                mac_pointer += strlen("HWaddr"); // Dịch con trỏ sau "HWaddr"

                // Bỏ qua khoảng trắng
                while (*mac_pointer == ' ' || *mac_pointer == '\t')
                {
                    mac_pointer++;
                }

                // Sao chép chuỗi để tách riêng biệt
                strncpy(mac_copy, mac_pointer, sizeof(mac_copy) - 1);
                mac_copy[sizeof(mac_copy) - 1] = '\0';

                // Tách lấy địa chỉ MAC
                token = strtok(mac_copy, " \t\n");
                if (token)
                {
                    strncpy(interfaces[iface_index].mac, token, sizeof(interfaces[iface_index].mac) - 1);
                    interfaces[iface_index].mac[sizeof(interfaces[iface_index].mac) - 1] = '\0';
                }
                else
                {
                    strcpy(interfaces[iface_index].mac, "*");
                }
            }
            else
            {
                strcpy(interfaces[iface_index].mac, "*");
            }

            // Khởi tạo mặc định
            strcpy(interfaces[iface_index].ipv4, "*");
            strcpy(interfaces[iface_index].ipv6, "*");
            strcpy(interfaces[iface_index].status, "UNKNOWN");
            strcpy(interfaces[iface_index].bcast, "*");
            strcpy(interfaces[iface_index].mask, "*");
            // interfaces[iface_index].rx_bytes = 0;
            // interfaces[iface_index].tx_bytes = 0;
        }

        else if (strstr(buffer, "inet addr:"))
        {
            char *token = strstr(buffer, "inet addr:") + strlen("inet addr:");
            token = strtok(token, " "); // Lấy IPv4

            if (token)
            {
                strncpy(interfaces[iface_index].ipv4, token, sizeof(interfaces[iface_index].ipv4) - 1);
                interfaces[iface_index].ipv4[sizeof(interfaces[iface_index].ipv4) - 1] = '\0';
            }

            // Duyệt tiếp để tìm Bcast và Mask
            while ((token = strtok(NULL, " ")) != NULL)
            {
                if (strncmp(token, "Bcast:", 6) == 0)
                {
                    token += 6; // Bỏ qua "Bcast:"
                    strncpy(interfaces[iface_index].bcast, token, sizeof(interfaces[iface_index].bcast) - 1);
                    interfaces[iface_index].bcast[sizeof(interfaces[iface_index].bcast) - 1] = '\0';
                }
                else if (strncmp(token, "Mask:", 5) == 0)
                {
                    token += 5; // Bỏ qua "Mask:"
                    strncpy(interfaces[iface_index].mask, token, sizeof(interfaces[iface_index].mask) - 1);
                    interfaces[iface_index].mask[sizeof(interfaces[iface_index].mask) - 1] = '\0';
                }
            }
        }

        // IPv6
        else if (strstr(buffer, "inet6 addr: "))
        {
            token = strstr(buffer, "inet6 addr: ") + strlen("inet6 addr: ");
            token = strtok(token, " \n");
            if (token)
            {
                strncpy(interfaces[iface_index].ipv6, token, sizeof(interfaces[iface_index].ipv6) - 1);
                interfaces[iface_index].ipv6[sizeof(interfaces[iface_index].ipv6) - 1] = '\0';
            }
        }
        // Status
        else if (strstr(buffer, "MTU:"))
        {
            strcpy(interfaces[iface_index].status, "DOWN"); // Mặc định là DOWN

            if (strstr(buffer, "UP"))
            {
                strcpy(interfaces[iface_index].status, "UP");
            }
        }
    }

    *count = iface_index + 1;
    // pclose(fp);
    fclose(fp);
    LOG(LOG_LVL_DEBUG, "%s, %d: End, found %d interfaces", __func__, __LINE__, *count);
}

void GI_write_network_info_to_file(interface_info interfaces[], int count)
{
    LOG(LOG_LVL_DEBUG, "%s, %d: Start", __func__, __LINE__);
    FILE *file = fopen(PATH_FILE_INTERFACE_INFO, "w");

    if (!file)
    {
        LOG(LOG_LVL_ERROR, "%s, %d: Cannot open file %s", __func__, __LINE__, PATH_FILE_INTERFACE_INFO);
        return;
    }

    // Header
    if (fprintf(file, "%-5s %-15s %-18s %-15s %-15s %-15s %-50s %-8s \n",                    // %-12s %-12s
                "ID", "Name", "MAC Address", "IPv4", "Bcast", "Mask", "IPv6", "Status") < 0) // , "RX Bytes", "TX Bytes"
    {
        LOG(LOG_LVL_ERROR, "%s, %d: Failed to write header", __func__, __LINE__);
        fclose(file);
        return;
    }
    if (fprintf(file, "--------------------------------------------------------------------------------------------------------------------------------------------\n") < 0)
    {
        LOG(LOG_LVL_ERROR, "%s, %d: Failed to write separator", __func__, __LINE__);
        fclose(file);
        return;
    }

    // Data
    for (int i = 0; i < count; i++)
    {

        if (fprintf(file, "%-5d %-15s %-18s %-15s %-15s %-15s %-50s %-8s \n", i + 1,
                    interfaces[i].name, interfaces[i].mac, interfaces[i].ipv4, interfaces[i].bcast, interfaces[i].mask,
                    interfaces[i].ipv6, interfaces[i].status) < 0)
        {
            LOG(LOG_LVL_ERROR, "%s, %d: Failed to write interface %d", __func__, __LINE__, i);
            break;
        }
    }

    if (fclose(file) != 0)
    {
        LOG(LOG_LVL_ERROR, "%s, %d: Failed to close file", __func__, __LINE__);
    }
    LOG(LOG_LVL_DEBUG, "%s, %d: End", __func__, __LINE__);
}
