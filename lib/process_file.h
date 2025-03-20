#ifndef PROCESS_FILE_H
#define PROCESS_FILE_H

void PF_delete_file(const char *file_path);
void PF_extract_substring(const char *input_str, int start_pos, int sub_length);
void PF_replace_word(const char *input_str, const char *old_word, const char *new_word);

#endif