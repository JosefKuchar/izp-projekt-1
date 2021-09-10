#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <stdlib.h>

// Define password hard limits
#define MAX_PASSWORD_LENGTH 102 // 100 chars + 1 newline + 1 terminator
#define MAX_PASSWORD_COUNT  43 // 42 Passwords + 1 for EOF

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

// Check if password meets required security level
int meets_security_level(char* password, int x, int level) {
    // Create array of all rules
    typedef bool (*f)(char*, int);
    f rules[] = {
        meets_rule_one,
        meets_rule_two,
        meets_rule_three,
        meets_rule_four
    };

    // Iterate through all rules
    for (int i = 0; i < level; i++) {
        bool is_met = rules[i](password, x);

        // If rule isn't met the password doesn't meet security level
        if (!is_met) {
            return false;
        }
    }

    // If all rules were met it means we have met required security level
    return true;
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

/* Prepare used chars array for counting
 * It modifies used_chars array
 */
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
bool string_to_int(char *string, int *num) {
    char *pEnd;
    long number = strtol(string, &pEnd, 10);

    /* Check if string starts with number (sign)
     * Also check if it ends with number
     */
    if (pEnd == string || *pEnd != '\0') {
        return false;
    }

    // Safely clamp long to int
    if (number > (long) INT_MAX) {
        number = INT_MAX;
    }
    else if (number < (long) INT_MIN) {
        number = INT_MIN;
    }

    // Return result by reference
    *num = number;

    return true;
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

// Read all passwords from STDIN and put in into supplied array
bool read_passwords(char passwords[][MAX_PASSWORD_LENGTH],
                    int* password_count) {
    for (int i = 0; i < MAX_PASSWORD_COUNT; i++) {
        // Get one line from stdin
        char* ret = fgets(passwords[i], MAX_PASSWORD_LENGTH, stdin);

        // If we hit EOF then end reading
        if (ret == NULL) {
            return true;
        }

        // If there is no newline it means the password is too long
        if (!is_newline_present(passwords[i])) {
            fprintf(stderr, "Heslo cislo %d je prilis dlouhe!\n", i + 1);
            return false;
        }

        // Update password count
        (*password_count)++;
    }

    // If we didn't already hit EOF it means the password list is too long
    fprintf(stderr, "Prilis dlouhy seznam hesel!\n");
    return false;
}

// Function for printing final stats
void print_stats(char passwords[][MAX_PASSWORD_LENGTH], int password_count) {
    // Variables for tracking totals
    int min_password_length = INT_MAX;
    int total_length = 0;
    bool used_chars[256] = { false }; // 256 for all ASCII codes

    // Iterate through all passwords
    for (int i = 0; i < password_count; i++) {
        // For average length
        int password_length = get_password_length(passwords[i]);
        total_length += password_length;

        // Update min password length
        if (password_length < min_password_length) {
            min_password_length = password_length;
        }

        // For counting unique chars
        populate_used_chars(passwords[i], used_chars);
    }

    // Fix stats when there are 0 passwords
    if (password_count == 0) {
        // Set password count to 1 to fix division by 0
        password_count = 1;
        // Set min password length to 0 from INT_MAX
        min_password_length = 0;
    }

    // Calculate other final stats
    int unique_chars = get_different_char_count(used_chars);
    double average_length = (double) total_length / (double) password_count;

    // Finally print everything
    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", unique_chars);
    printf("Minimalni delka: %d\n", min_password_length);
    printf("Prumerna delka: %.2f\n", average_length);
}

// Function to print valid passwords
void print_valid_passwords(char passwords[][MAX_PASSWORD_LENGTH],
                           int password_count,
                           int level,
                           int x) {
    // Iterate through all passwords
    for (int i = 0; i < password_count; i++) {
        if (meets_security_level(passwords[i], x, level)) {
            printf("%s", passwords[i]);
        }
    }
}

struct argument {
    char *key; // Argument key, for example "-l"
    bool has_value; // Flag that states if argument has value
    int value; // Resulting argument value
    int min_val; // Min argument value (including)
    int max_val; // Max argument value (including)
    char *error_msg; // Error message when argument is invalid
};

// Function to parse arguments from command line
// TODO combine bonus with standard solution
bool parse_arguments(int argc, char *argv[], struct argument arguments[]) {
    // Iterate through all supplied arguments except the program name
    for (int i = 1; i < argc; i++) {
        bool key_found = false;

        // Iterate through all defined arguments
        for (int j = 0; j < 3; j++) {
            struct argument *a = &arguments[j];
            if (str_cmp(a->key, argv[i])) {
                // If argument has value then parse it
                if (a->has_value) {
                    // Check if value is in bounds (exists)
                    if (i + 1 < argc) {
                        // Check and parse string to int
                        if (!string_to_int(argv[i + 1], &a->value) ||
                                a->value < a->min_val || // Check lower bounds
                                a->value > a->max_val) { // Check upper bounds
                            fprintf(stderr, "%s", a->error_msg);
                            return false;
                        }
                        // Skip to next argument key
                        i++;
                    } else {
                        fprintf(stderr, "%s", a->error_msg);
                        return false;
                    }
                /* If argument doesn't have value just set the value to 1
                 * to indicate that argument has been used
                 */
                } else {
                    a->value = 1;
                }

                // Because we already found one key we can skip
                key_found = true;
                break;
            }
        }

        // If we didn't find the key it means the argument is invalid
        if (!key_found) {
            fprintf(stderr, "Neexistující argument: %s\n", argv[i]);
            return false;
        }
    }

    // If everything went well we idicate it
    return true;
}

// Entry
int main(int argc, char *argv[]) {
    // Define all arguments
    struct argument arguments[] = {
        // LEVEL argument
        {
            "-l", // Key
            true, // Has value
            1, // Default value
            1, // Min value
            4, // Max value
            "Argument LEVEL musi byt cele cislo mezi 1 a 4!\n" }, // Error msg
        // PARAM argument
        {
            "-p", // Key
            true, // Has value
            1, // Default value
            0, // Min value
            INT_MAX, // Max value
            "Argument PARAM musi byt nezaporne cele cislo!\n" }, // Error msg
        // Stats argument
        {
            "--stats", // Key
            false, // Has value
            0, // Default value
            0, // Min value
            0, // Max value
            "" // Error msg
        }
    };

    // Parse arguments and error check
    if (!parse_arguments(argc, argv, arguments)) {
        return EXIT_FAILURE;
    }

    // Get parsed values from arguments
    int level = arguments[0].value;
    int x = arguments[1].value;

    // Create buffer for all passwords
    char passwords[MAX_PASSWORD_COUNT][MAX_PASSWORD_LENGTH];
    int password_count = 0;

    // Read passwords from STDIN
    if (!read_passwords(passwords, &password_count)) {
        return EXIT_FAILURE;
    }

    // Finally print all valid passwords
    print_valid_passwords(passwords, password_count, level, x);

    // Print stats if relevant argument was supplied
    if (arguments[2].value) {
        print_stats(passwords, password_count);
    }

    return EXIT_SUCCESS;
}
