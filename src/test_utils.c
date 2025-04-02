#include <stdio.h>

#define TEST 1

#include "lib/utils.h"

// This file just tests utils

int main(int argc, char* argv[]) {
    printf("Testing utils...\n");

    char* expected = "my_expected_string";
    char* start_str = "<h4>";
    char* end_str = "</h4>";
    char target[128];
    snprintf(target, 128, "%s%s%c", start_str, expected, end_str[0]);
    char result[128];
    get_string_between(start_str, end_str[0], target, result);
    printf("Expected: %s\n", expected);
    printf("Got:      %s\n", result);
    if (strcmp(result, expected)) {
        printf("Error!\n");
        return 1;
    }
    printf("Passed!\n");
    return 0;
}
