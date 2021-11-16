/**
 * @name IZP Projekt 1 - Prace s textem
 * @author Josef Kuchar - xkucha28
 * 2021
 */

#include <stdio.h> // IO functions
#include <stdbool.h> // Bool type
#include <limits.h> // Min and max values of variable types
#include <stdlib.h> // EXIT macros

/**
 * Define password hard limits
 */
#define MAX_PASSWORD_LENGTH 100
#define PASSWORD_BUFFER_LENGTH MAX_PASSWORD_LENGTH + 3 // 2 (\r)\n, 1 \0

// Define unused param macro to achieve same arguments across all rules
#define UNUSED(x) (void)(x)

/**
 * Struct for keeping track of password stats
 */
struct stats {
    int min_length; // Minimum password length
    bool used_chars[256]; // Array of used characters
    int total_length; // Total length of all passwords
    int password_count; // Number of passwords
};

/**
 * Struct for argument definitions
 */
struct argument {
    char *key; // Argument key, for example "-l"
    bool has_value; // Flag that states if argument has value
    int value; // Resulting argument value
    int min_val; // Min argument value (including)
    int max_val; // Max argument value (including)
    char *error_msg; // Error message when argument is invalid
};

/**
 * Find if character is lower case letter
 * @param c character that is checked
 * @return true if character is lower case
 */
bool is_lower_case(char c) {
    return c >= 'a' && c <= 'z';
}

/**
 * Find if character is upper case letter
 * @param c character that is checked
 * @return true if character is upper case
 */
bool is_upper_case(char c) {
    return c >= 'A' && c <= 'Z';
}

/**
 * Find if character is digit
 * @param c character that is checked
 * @return true if character is digit
 */
bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

/**
 * Find if character is a special character
 * @param c character that is checked
 * @return true if character is special character
 */
bool is_special_char(char c) {
    return !(is_lower_case(c) || is_upper_case(c) || is_digit(c)) &&
            c >= 32 && c <= 126; // Bounds defined in assigment
}

/**
 * Check rule 1.
 * @param password Password string to check
 * @param x PARAM input argument
 * @return true if password meets rule one
 */
bool meets_rule_one(char *password, int x) {
    UNUSED(x);

    bool lower_case_letter_present = false;
    bool upper_case_letter_present = false;

    // Iterate through the whole string
    for (int i = 0; password[i] != '\0'; i++) {
        // Check if char is lower case letter
        if (!lower_case_letter_present && is_lower_case(password[i])) {
            lower_case_letter_present = true;
            continue;
        }

        // Check if char is upper case letter
        if (!upper_case_letter_present && is_upper_case(password[i])) {
            upper_case_letter_present = true;
            continue;
        }
    }

    // If both types of letters are found rule is met
    return lower_case_letter_present && upper_case_letter_present;
}

/**
 * Check rule 2.
 * @param password Password string to check
 * @param x PARAM input argument
 * @return true if password meets rule two
 */
bool meets_rule_two(char *password, int x) {
    int check_pass_count = 0;

    // If param > 4 then only require 4 groups
    if (x > 4) {
        x = 4;
    }

    // Create array of all checks
    typedef bool (*f)(char);
    f checks[] = { is_lower_case, is_upper_case, is_digit, is_special_char };
    // Array of all check results
    bool check_results[4] = { false };

    // Iterate through the whole string
    for (int i = 0; password[i] != '\0'; i++) {
        // Iterate through all checks
        for (int j = 0; j < 4; j++) {
            // Check if check wasn't already passed and if check is true
            if (!check_results[j] && checks[j](password[i])) {
                check_results[j] = true;
                check_pass_count++;
                // We don't have to search for more checks
                break;
            }
        }
    }

    // If number of met rules is bigger or equal than x rule is met
    return check_pass_count >= x;
}

/**
 * Check rule 3.
 * @param password Password string to check
 * @param x PARAM input argument
 * @return true if password meets rule three
 */
bool meets_rule_three(char *password, int x) {
    char last_char = '\0';
    int current_seq_len = 1;

    // Iterate through the whole string
    for (int i = 0; password[i] != '\0'; i++) {
        // If last char matches current then increase current seq length
        if (last_char == password[i]) {
            current_seq_len++;

            /* If current seq length is already
             * bigger or equeal than x we can stop searching
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
    return current_seq_len < x;
}

/**
 * Check rule 4.
 * @param password Password string to check
 * @param x PARAM input argument
 * @return true if password meets rule four
 */
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
                    break;
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

/**
 * Check if password meets required security level
 * @param password Password string to check
 * @param x PARAM input argument
 * @param level LEVEL input argument
 * @return true if password meets desired security level
 */
bool meets_security_level(char *password, int x, int level) {
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

/**
 * Find password length - without newline character
 * @param password Password string
 * @return length of the password without newline character
 */
int get_password_length(char *password) {
    int i = 0;

    // Iterate through all characters
    while (password[i] != '\0') {
        // If we find newline we don't have to search anymore
        if (password[i] == '\n') {
            return i;
        }
        i++;
    }

    /* If we didn't find newline just return length of the whole string
     * without terminating character
     */
    return i;
}

// Find if password isn't longer than max length
bool is_password_length_ok(char *password) {
    return get_password_length(password) <= 100;
}

/**
 * Prepare used chars array for counting It modifies used_chars array
 * @param password Password string
 * @param used_chars Array of 256 bools for keeping track of used chars
 */
void populate_used_chars(char *password, bool *used_chars) {
    // Iterate through all characters
    for (int i = 0; password[i] != '\0'; i++) {
        // Populate array, exluding newline character
        if (password[i] != '\n') {
            used_chars[(int) password[i]] = true;
        }
    }
}

/**
 * Count different (unique) characters inside used chars array
 * @param used_chars Array of 256 bools for keeping track of used chars
 * @return Unique character count
 */
int get_different_char_count(bool used_chars[256]) {
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

/**
 * Convert string to int with error handling
 * @param string String argument
 * @param num Result
 * @return True if everything went ok
 */
bool string_to_int(char *string, int *num) {
    char *end;
    long number = strtol(string, &end, 10);

    /* Check if string starts with number (sign)
     * Also check if it ends with number
     */
    if (end == string || *end != '\0') {
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

/**
 * String compare function - strcmp alternative
 * @param s1 First string
 * @param s2 Second string
 * @return True if strings are equal
 */
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

/**
 * Update global password stats
 * @param password Password used to update stats
 * @param stats Stats struct
 */
void update_stats(char *password, struct stats *stats) {
    populate_used_chars(password, stats->used_chars);
    int password_length = get_password_length(password);
    if (password_length < stats->min_length) {
        stats->min_length = password_length;
    }
    stats->total_length += password_length;
    stats->password_count++;
}

/**
 * Process passwords from stdin and print them to stdout
 * @param level LEVEL input argument
 * @param x PARAM input argument
 * @param stats Stats struct
 * @return True if everything went well
 */
bool process_passwords(int level, int x, struct stats *stats) {
    // Read passwords from stdin until EOF
    for (int i = 1;; i++) {
        char password[PASSWORD_BUFFER_LENGTH];

        // Get one line from stdin
        char* ret = fgets(password, PASSWORD_BUFFER_LENGTH, stdin);

        // If we hit EOF then end reading
        if (ret == NULL) {
            return true;
        }

        // Check if password lenght is OK (defined by MAX_PASSWORD_LENGTH)
        if (!is_password_length_ok(password)) {
            fprintf(stderr, "Heslo cislo %d je prilis dlouhe!\n", i);
            return false;
        }

        // Print password if it meets required security level
        if (meets_security_level(password, x, level)) {
            printf("%s", password);
        }

        // Update password stats
        update_stats(password, stats);
    }
}

/**
 * Function for printing final stats
 * @param stats Stats struct
 */
void print_stats(struct stats *stats) {
    // First fix stats if 0 passwords were supplied
    if (stats->password_count == 0) {
        stats->password_count = 1; // So we don't divide by 0
        stats->min_length = 0;
    }

    // Calculate final stats from stats struct
    int unique_chars = get_different_char_count(stats->used_chars);
    double total_length = (double) stats->total_length;
    double password_count = (double) stats->password_count;
    double average_length = total_length / password_count;

    // Finally print everything
    printf("Statistika:\n");
    printf("Ruznych znaku: %d\n", unique_chars);
    printf("Minimalni delka: %d\n", stats->min_length);
    printf("Prumerna delka: %.1f\n", average_length);
}

/**
 * Parser for original assignment
 * @param argc Number of arguments
 * @param argv Program arguments
 * @param args Args struct with argument definitions
 * @return True if everything went well
 */
bool parse_arguments_classic(int argc, char *argv[], struct argument args[]) {
    // Check number of arguments
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "Neplatny pocet argumentu!\n");
        return false;
    }

    // Iterate through all supplied arguments except first one (program name)
    for (int i = 1; i < argc; i++) {
        struct argument *a = &args[i - 1];

        // Level and param arguments
        if (a->has_value) {
            if (!string_to_int(argv[i], &a->value) ||
                    a->value < a->min_val || // Check lower bounds
                    a->value > a->max_val) { // Check upper bounds
                fprintf(stderr, "%s", a->error_msg);
                return false;
            }
        // Stats argument
        } else {
            if (str_cmp(a->key, argv[i])) {
                a->value = 1;
            } else {
                fprintf(stderr, "Neplatny argument %s\n", argv[i]);
                return false;
            }
        }
    }

    return true;
}

/**
 * Function to parse arguments from command line
 * @param argc Number of arguments
 * @param argv Program arguments
 * @param arguments Args struct with argument definitions
 * @param ac Number of defined arguments
 * @return True if everything went well
 */
bool parse_arguments(int argc, char *argv[], struct argument args[], int ac) {
    // Iterate through all supplied arguments except the program name
    for (int i = 1; i < argc; i++) {
        bool key_found = false;

        // Iterate through all defined arguments
        for (int j = 0; j < ac; j++) {
            struct argument *a = &args[j];
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

        if (!key_found) {
            /* If we didn't find the key and it is the first argument
             * then fallback to original argument parser
             */
            if (i == 1) {
                return parse_arguments_classic(argc, argv, args);
            } else {
                fprintf(stderr, "Neplatny prepinac %s\n", argv[i]);
                return false;
            }
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    // Define all arguments
    struct argument arguments[] = {
        // LEVEL argument
        {
            .key = "-l", .has_value = true,
            .value = 1, .min_val = 1, .max_val = 4,
            .error_msg = "Argument LEVEL musi byt cele cislo mezi 1 a 4!\n" },
        // PARAM argument
        {
            .key = "-p", .has_value = true,
            .value = 1, .min_val = 1, .max_val = INT_MAX,
            .error_msg = "Argument PARAM musi byt nezaporne cele cislo!\n" },
        // Stats argument
        {
            .key = "--stats", .has_value = false,
            .value = 0, .min_val = 0, .max_val = 0,
            .error_msg = ""
        }
    };

    // Initialize stats
    struct stats stats = {
        .min_length = INT_MAX,
        .used_chars = {false},
        .total_length = 0,
        .password_count = 0
    };

    // Parse arguments and error check
    if (!parse_arguments(argc, argv, arguments, 3)) {
        return EXIT_FAILURE;
    }

    // Get parsed values from arguments
    int level = arguments[0].value;
    int x = arguments[1].value;

    // Read passwords from STDIN and print the valid ones
    if (!process_passwords(level, x, &stats)) {
        return EXIT_FAILURE;
    }

    // Print stats if relevant argument was supplied
    if (arguments[2].value) {
        print_stats(&stats);
    }

    return EXIT_SUCCESS;
}
