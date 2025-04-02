
#ifndef TEST
    #include <linux/string.h>
#else
    #include <string.h>
    #include <stdio.h>
    #define pr_info printf
    #define pr_err printf
#endif

#include "variables.h"

int get_string_between(const char* start, const char end, const char* in_buffer, char* out_buffer) {

    char *token_start = strstr(in_buffer, start);
    if (token_start) {
        token_start += strlen(start);
        char *token_end = strchr(token_start, end);
        if (token_end) {
            size_t token_len = token_end - token_start;
            strncpy(out_buffer, token_start, token_len);
            out_buffer[token_len] = '\0';
            pr_info(PRINT_FMT "Found string: %s\n", out_buffer);
        }
    } else {
        pr_err(PRINT_FMT "Failed to find string!\n");
        return 1;
    }
    return 0;
}
