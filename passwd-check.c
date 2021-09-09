#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

// Define unused param macro to achieve same arguments across all rules
#define UNUSED(x) (void)(x)

// Check rule 1.
bool meets_rule_one(char *password, int x) {
    UNUSED(x);

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

// Get achieved level of password (0 - 4)
int get_achieved_level(char* password, int x) {
    // Create array of all rules
    typedef bool (*f)(char*, int);
    f rules[4] = {meets_rule_one, meets_rule_two, meets_rule_three, meets_rule_four};

    // Iterate through all rules
    for (int i = 0; i < 4; i++) {
        bool is_met = rules[i](password, x);

        // If rule isn't met return adequate level of security
        if (!is_met) {
            return i;
        }
    }

    // If all rules were met it means we have achieved highest level of security
    return 4;
}

// Find if newline is present in string to detect overflow
bool is_newline_present(char* string) {
    // Iterate through all characters
    for (int i = 0; string[i] != '\0'; i++) {
        // If we find newline we don't have to search anymore
        if (string[i] == '\n') {
            return true;
        }
    }

    // If we didn't find newline it isn't there
    return false;
}

// Find password length - without newline character
int get_password_length(char *password) {
    // Iterate through all characters
    for (int i = 0; password[i] != '\0'; i++) {
        // If we find newline we can return password length
        if (password[i] == '\n') {
            return i;
        }
    }

    // In case we didn't find newline character return -1 as error
    return -1;
}

// Prepare used chars array for counting
void populate_used_chars(char *password, bool *used_chars) {
    // Iterate through all characters
    for (int i = 0; password[i] != '\0'; i++) {
        // Populate array, including newline character
        used_chars[(int) password[i]] = true;
    }
    // Account for 0 (terminator) character
    used_chars[0] = true;
}

// Count different (unique) characters inside used chars array
int get_different_char_count(bool *used_chars) {
    int count = 0;

    /* Iterate through all ASCII character
     * codes inside array and just count them
     */
    for (int i = 0; i < 256; i++) {
        if (used_chars[i]) {
            count++;
        }
    }

    return count;
}

// Convert string to int with error handling
int string_to_int(char *string) {
    char * pEnd;
    int number = strtol(string, &pEnd, 10);

    /* Check if string starts with number (sign)
     * Also check if it ends with number
     */
    if (pEnd == string || *pEnd != '\0') {
        printf("Parametr neni cislo!\n");
    }

    return number;
}

// String compare function
bool str_cmp(char *s1, char *s2) {
    // Iterate through all characters
    for (int i = 0;; i++) {
        // When one of the strings ends
        if (s1[i] == '\0' || s2[i] == '\0') {
            // If both strings end then the strings are equal
            if (s1[i] == '\0' && s2[i] == '\0') {
                return true;
            // If not then they are different lengths thus not match
            } else {
                return false;
            }
        }

        // When chars doesn't equal we can immediately return
        if (s1[i] != s2[i]) {
            return false;
        }
    }
}

// Entry
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Nedostatecny pocet argumetu!\n");
        return 0;
    }

    int level = string_to_int(argv[1]);
    int x = string_to_int(argv[2]);

    int min_password_length = INT_MAX;
    int password_count = 0;
    int total_length = 0;
    bool used_chars[256] = { false };

    while (true) {
        /* Create buffer for password line
         * 100 chars for password + 1 char newline + 1 char terminator
         */
        char password[102];
        char* ret = fgets(password, 102, stdin);

        if (ret == NULL) {
            break;
        }

        if (!is_newline_present(password)) {
            printf("Heslo je prilis dlouhe!\n");
            return 0;
        }

        int password_length = get_password_length(password);
        total_length += password_length;
        if (password_length < min_password_length) {
            min_password_length = password_length;
        }
        password_count++;
        populate_used_chars(password, used_chars);

        int achieved_level = get_achieved_level(password, x);

        if (achieved_level >= level) {
            printf("%s", password);
        }
    }

    if (argc >= 4 && str_cmp(argv[3], "--stats")) {
        printf("Statistika:\n");
        printf("Ruznych znaku: %d\n", get_different_char_count(used_chars));
        printf("Minimalni delka: %d\n", min_password_length);
        printf("Prumerna delka: %.2f\n", (double) total_length / (double) password_count);
    }

    return 0;

}
