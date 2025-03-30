#ifndef CREATE_MAIL_FROM_NAME_H
#define CREATE_MAIL_FROM_NAME_H

#include <stdio.h>

int CMFN_is_valid_name(const char *line);

void CMFN_extract_names(const char *input_filename, const char *output_filename);

void CMFN_generate_email(char *name, char *email, int email_size);

void CMFN_push_mail_data(const char *name_file, const char *mail_file);


#endif