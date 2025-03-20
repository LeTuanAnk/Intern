#ifndef PARSER_OPTION_H
#define PARSER_OPTION_H


char *PO_get_normalized_path(const char *file_name);

void PO_parse_combined_options(int argc, char *argv[]);

#endif