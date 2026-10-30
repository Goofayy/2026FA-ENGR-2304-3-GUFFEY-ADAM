/*
 * Decimal/Binary/Hexadecimal Conversion Program
 *
 * Menu Option 1:
 *   - Decimal (-127 to 127) <-> 8-bit signed-magnitude binary.
 *
 * Menu Option 2:
 *   - Positive binary <-> hexadecimal.
 *   - Binary input has no designator; hexadecimal input starts with 0x.
 *   - Binary output: up to 32 bits left and 32 bits right of the
 *     binary point (64 bits total).
 *   - Hex output: up to 8 hexadecimal digits left and 8 digits right
 *     of the hexadecimal point (16 hex digits total).
 *
 * The program continues until Exit is selected.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 256

/* ---------- Utility functions ---------- */

static void clear_input_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        /* discard remaining input */
    }
}

static int hex_value(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/* ---------- Menu Option 1 ---------- */

/*
 * Convert decimal -127..127 to 8-bit signed magnitude:
 *
 * bit 7 = sign (0 positive, 1 negative)
 * bits 6..0 = magnitude
 */
static void decimal_to_signed_magnitude(int value)
{
    unsigned int magnitude;
    unsigned int encoded;

    if (value < -127 || value > 127) {
        printf("Error: decimal input must be between -127 and 127.\n");
        return;
    }

    magnitude = (unsigned int)(value < 0 ? -value : value);
    encoded = magnitude | (value < 0 ? 0x80u : 0u);

    printf("8-bit signed-magnitude binary: ");
    for (int bit = 7; bit >= 0; --bit) {
        printf("%u", (encoded >> bit) & 1u);
    }
    printf("\n");
}

/*
 * Convert an 8-bit signed-magnitude binary string to decimal.
 * Example:
 *   00000101 = +5
 *   10000101 = -5
 */
static void signed_magnitude_to_decimal(const char *input)
{
    size_t len = strlen(input);
    unsigned int value = 0;
    int negative;

    if (len != 8) {
        printf("Error: signed-magnitude binary must contain exactly 8 bits.\n");
        return;
    }

    for (size_t i = 0; i < len; ++i) {
        if (input[i] != '0' && input[i] != '1') {
            printf("Error: binary input may contain only 0 and 1.\n");
            return;
        }
    }

    negative = (input[0] == '1');

    for (int i = 1; i < 8; ++i) {
        value = (value << 1) | (unsigned int)(input[i] - '0');
    }

    if (value > 127) {
        printf("Error: magnitude must be between 0 and 127.\n");
        return;
    }

    if (value == 0) {
        printf("Decimal: %d\n", 0);
    } else {
        printf("Decimal: %d\n", negative ? -(int)value : (int)value);
    }
}

static void option_one(void)
{
    int choice;
    char binary[MAX_INPUT];

    printf("\n--- Option 1: Decimal <-> Binary ---\n");
    printf("1. Decimal to 8-bit signed-magnitude binary\n");
    printf("2. 8-bit signed-magnitude binary to decimal\n");
    printf("Select conversion: ");

    if (scanf("%d", &choice) != 1) {
        printf("Error: invalid menu choice.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (choice == 1) {
        int decimal;

        printf("Enter a decimal integer (-127 to 127): ");
        if (scanf("%d", &decimal) != 1) {
            printf("Error: invalid decimal input.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();

        decimal_to_signed_magnitude(decimal);
    }
    else if (choice == 2) {
        printf("Enter an 8-bit signed-magnitude binary number: ");
        if (scanf("%255s", binary) != 1) {
            printf("Error: unable to read binary input.\n");
            clear_input_buffer();
            return;
        }
        clear_input_buffer();

        signed_magnitude_to_decimal(binary);
    }
    else {
        printf("Error: please select 1 or 2.\n");
    }
}

/* ---------- Menu Option 2 ---------- */

/*
 * Parse a positive binary value containing at most 32 integer bits
 * and 32 fractional bits.
 *
 * The integer and fractional parts are stored separately so the
 * program can represent the requested 64-bit binary format without
 * using floating-point arithmetic.
 */
static int parse_binary(const char *input,
                        uint32_t *integer_part,
                        uint32_t *fraction_part,
                        int *fraction_bits)
{
    const char *dot = strchr(input, '.');
    size_t int_len = dot ? (size_t)(dot - input) : strlen(input);
    const char *frac_start = dot ? dot + 1 : NULL;
    size_t frac_len = dot ? strlen(frac_start) : 0;

    if (int_len == 0 || int_len > 32 || frac_len > 32) {
        return 0;
    }

    *integer_part = 0;
    *fraction_part = 0;
    *fraction_bits = (int)frac_len;

    for (size_t i = 0; i < int_len; ++i) {
        if (input[i] != '0' && input[i] != '1') {
            return 0;
        }
        *integer_part = (*integer_part << 1) | (uint32_t)(input[i] - '0');
    }

    for (size_t i = 0; i < frac_len; ++i) {
        if (frac_start[i] != '0' && frac_start[i] != '1') {
            return 0;
        }
        *fraction_part =
            (*fraction_part << 1) | (uint32_t)(frac_start[i] - '0');
    }

    return 1;
}

/*
 * Print binary with up to 32 integer bits and up to 32 fractional bits.
 * Leading zeroes are suppressed on the integer side, but zero is shown
 * when necessary. Fractional trailing zeroes are suppressed.
 */
/*
 * Convert binary to hexadecimal. Each hexadecimal digit represents
 * four binary bits. Fractional binary digits are padded on the right
 * to a multiple of four before conversion.
 */
static void binary_to_hex(const char *input)
{
    uint32_t integer_part, fraction_part;
    int fraction_bits;

    if (!parse_binary(input, &integer_part, &fraction_part, &fraction_bits)) {
        printf("Error: binary must contain only 0/1, with at most "
               "32 bits on each side of the binary point.\n");
        return;
    }

    /* Integer portion */
    char int_hex[9];
    int int_digits = 0;

    if (integer_part == 0) {
        int_hex[int_digits++] = '0';
    } else {
        int started = 0;
        for (int shift = 28; shift >= 0; shift -= 4) {
            unsigned int digit = (integer_part >> shift) & 0xFu;
            if (digit != 0 || started) {
                int_hex[int_digits++] = "0123456789ABCDEF"[digit];
                started = 1;
            }
        }
    }
    int_hex[int_digits] = '\0';

    printf("Hexadecimal: 0x%s", int_hex);

    if (fraction_bits > 0) {
        int padded_bits = ((fraction_bits + 3) / 4) * 4;
        uint64_t padded_fraction = (uint64_t)fraction_part
                                   << (padded_bits - fraction_bits);
        int frac_digits = padded_bits / 4;

        /* Suppress trailing zero hex digits. */
        while (frac_digits > 0 &&
               (padded_fraction & 0xFu) == 0) {
            padded_fraction >>= 4;
            --frac_digits;
        }

        if (frac_digits > 0) {
            printf(".");
            for (int i = frac_digits - 1; i >= 0; --i) {
                unsigned int digit = (unsigned int)((padded_fraction >>
                                   (i * 4)) & 0xFu);
                putchar("0123456789ABCDEF"[digit]);
            }
        }
    }

    putchar('\n');
}

/*
 * Convert positive hexadecimal to binary.
 * Hexadecimal input starts with 0x or 0X.
 *
 * Up to 8 hex digits are allowed on each side of the hexadecimal point,
 * corresponding to 32 binary bits on each side.
 */
static void hex_to_binary(const char *input)
{
    const char *p = input + 2;
    const char *dot = strchr(p, '.');
    size_t int_len = dot ? (size_t)(dot - p) : strlen(p);
    size_t frac_len = dot ? strlen(dot + 1) : 0;

    if (int_len == 0 || int_len > 8 || frac_len > 8) {
        printf("Error: hexadecimal input may contain at most "
               "8 digits on each side of the point.\n");
        return;
    }

    for (size_t i = 0; i < int_len; ++i) {
        if (hex_value(p[i]) < 0) {
            printf("Error: invalid hexadecimal digit.\n");
            return;
        }
    }

    if (dot) {
        for (size_t i = 0; i < frac_len; ++i) {
            if (hex_value(dot[1 + i]) < 0) {
                printf("Error: invalid hexadecimal digit.\n");
                return;
            }
        }
    }

    printf("Binary: ");

    /* Integer hex digits -> binary. */
    int first_one_seen = 0;
    for (size_t i = 0; i < int_len; ++i) {
        int value = hex_value(p[i]);
        for (int bit = 3; bit >= 0; --bit) {
            int b = (value >> bit) & 1;
            if (b || first_one_seen || (i == int_len - 1 && bit == 0)) {
                putchar(b ? '1' : '0');
                first_one_seen = 1;
            }
        }
    }

    if (dot && frac_len > 0) {
        /* Find the last nonzero fractional hex digit. */
        size_t last_nonzero = frac_len;
        while (last_nonzero > 0 &&
               hex_value(dot[last_nonzero]) == 0) {
            --last_nonzero;
        }

        if (last_nonzero > 0) {
            putchar('.');
            for (size_t i = 0; i < last_nonzero; ++i) {
                int value = hex_value(dot[1 + i]);
                for (int bit = 3; bit >= 0; --bit) {
                    putchar((value >> bit) & 1 ? '1' : '0');
                }
            }
        }
    }

    putchar('\n');
}

static int is_hex_input(const char *input)
{
    return (input[0] == '0' &&
            (input[1] == 'x' || input[1] == 'X'));
}

static void option_two(void)
{
    char input[MAX_INPUT];

    printf("\n--- Option 2: Binary <-> Hexadecimal ---\n");
    printf("Enter a positive binary number or a hexadecimal number.\n");
    printf("Use 0x (or 0X) before hexadecimal input.\n");
    printf("Examples: 101101.1011   or   0x2D.B\n");
    printf("Input: ");

    if (scanf("%255s", input) != 1) {
        printf("Error: unable to read input.\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();

    if (is_hex_input(input)) {
        hex_to_binary(input);
    } else {
        binary_to_hex(input);
    }
}

/* ---------- Main program ---------- */

int main(void)
{
    int choice;

    printf("============================================\n");
    printf(" Decimal / Binary / Hex Conversion Program\n");
    printf("============================================\n");

    while (1) {
        printf("\nMenu\n");
        printf("1. Decimal <-> Binary\n");
        printf("2. Binary <-> Hexadecimal\n");
        printf("3. Exit\n");
        printf("Select an option: ");

        if (scanf("%d", &choice) != 1) {
            printf("Error: please enter 1, 2, or 3.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
            case 1:
                option_one();
                break;

            case 2:
                option_two();
                break;

            case 3:
                printf("Exiting program. Goodbye!\n");
                return 0;

            default:
                printf("Error: invalid menu option.\n");
        }
    }
}
