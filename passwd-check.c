#include <stdio.h>
#include <stdbool.h>

// Check rule 1.
bool meets_rule_one(char *password) {
    bool small_char_present = false;
    bool big_char_present = false;

    // Iterate through the whole string
    for (int i = 0; password[i] != '\0'; i++) {
        // Check if char is lower case letter
        if (!small_char_present && password[i] >= 'a' && password[i] <= 'z') {
            small_char_present = true;
            continue;
        }

        // Check if char is upper case letter
        if (!big_char_present && password[i] >= 'A' && password[i] <= 'Z') {
            big_char_present = true;
            continue;
        }
    }

    // If both types of letters is found rule is met
    return small_char_present && big_char_present;
}

// Check rule 2.
bool meets_rule_two(char *password, int x) {
    int rule_pass_count = 0;

    bool small_char_present = false;
    bool big_char_present = false;
    bool number_present = false;
    bool special_char_present = false;

    // Iterate through the whole string
    for (int i = 0; password[i] != '\0'; i++) {
        char c = password[i];

        // Check if char is lower case letter
        if (!small_char_present && c >= 'a' && c <= 'z') {
            small_char_present = true;
            rule_pass_count++;
            continue;
        }

        // Check if char is upper case letter
        if (!big_char_present && c >= 'A' && c <= 'Z') {
            big_char_present = true;
            rule_pass_count++;
            continue;
        }

        // Check if char is number
        if (!number_present && c >= '0' && c <= '9') {
            number_present = true;
            rule_pass_count++;
            continue;
        }

        // Check if char is special character
        if (!special_char_present &&
            // Check if character isn't alphanumeric
            !(
                (c >= 'A' && c <= 'Z') ||
                (c >= 'a' && c <= 'z') ||
                (c >= '0' && c <= '9')
            ) &&
            // Check if character is in special char bounds
            c >= 33 && c <= 126)
        {
            special_char_present = true;
            rule_pass_count++;
            continue;
        }
    }

    // If number of met rules is bigger or equal than x rule is met
    return rule_pass_count >= x;
}

// Check rule 3.
bool meets_rule_three(char *password, int x) {
    char last_char = '\0';
    int current_seq_len = 1;

    // Iterate through the whole string
    for (int i = 0; password[i] != '\0'; i++) {
        // If last char matches current then increase current seq length
        if (last_char == password[i]) {
            current_seq_len++;

            /* If current seq length is already
             * bigger then x we can stop searching
             */
            if (current_seq_len >= x) {
                return false;
            }
        // If last char doesn't match current then reset current length
        } else {
            current_seq_len = 1;
        }

        last_char = password[i];
    }

    // If seq with length x wasn't found rule is met
    return true;
}

// Check rule 4.
bool meets_rule_four(char *password, int x) {
    // Iterate through the whole string
    for (int i = 0; password[i] != '\0'; i++) {
        /* Iterate through remaining part of string
         * Substrings can overlap, so we have to start directly after current
         * char in main (outside) loop
         */
        for (int j = i + 1; password[j] != '\0'; j++) {
            bool matching_substrings = true;

            // Check if current substrings with length x are matching
            for (int k = 0; k < x; k++) {
                if (password[i + k] == '\0' || // Check if we are in bounds
                    password[j + k] == '\0' || // Check if we are in bounds
                    password[i + k] != password[j + k])
                {
                    matching_substrings = false;
                }
            }

            // If matching substrings were found we can stop searching
            if (matching_substrings) {
                return false;
            }
        }
    }

    // If matching substrings weren't found rule is met
    return true;
}

int main(int argc, char *argv[]) {
    for (int i = 0; i < argc; ++i) {
        printf("%s\n", argv[i]);
    }

    if (argc < 3) {
        printf("Nedostatecny pocet argumetu!\n");
        return 0;
    }

    printf("%d\n", meets_rule_one("azz"));
    printf("%d\n", meets_rule_two("azz!", 3));
    printf("%d\n", meets_rule_three("azzzzzz!asdfsdf", 6));
    printf("%d\n", meets_rule_four("hahaha", 4));

    return 0;

}
