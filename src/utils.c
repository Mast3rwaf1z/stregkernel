#ifndef TEST
    #include <linux/string.h>
#else
    #include <string.h>
    #include <stdio.h>
    #define pr_info printf
    #define pr_err printf
#endif

int get_string_between(const char *start_string, const char *end_string, const char *in_buffer, char *out_buffer) {
    const char *start_ptr = strstr(in_buffer, start_string);
    if (!start_ptr) {
        out_buffer[0] = '\0'; // Not found, return empty string
        return -1;
    }
    start_ptr += strlen(start_string); // Move past start_string

    const char *end_ptr = strstr(start_ptr, end_string);
    if (!end_ptr) {
        out_buffer[0] = '\0'; // End string not found, return empty string
        return -1;
    }

    size_t length = end_ptr - start_ptr;
    strncpy(out_buffer, start_ptr, length);
    out_buffer[length] = '\0'; // Null terminate the output buffer
    return 0;
}
