//2026FA-ENGR-2304-3-GUFFEY-ADAM-P02
 /* ENGR 2304 - Number Conversion Program
 * This program converts numbers between decimal, binary, and hexadecimal formats.
 */

#include <stdio.h>
#include <string.h>

void decimalToBinary(int n);
void binaryToDecimal(char b[]);
void binaryToHex(char b[]);
void hexToBinary(char h[]);
void printHexAsBinary(char c);
void printFirstHexAsBinary(char c);

int main()
{
    int menu, choice;
    char number[100];

    do
    {
        printf("\n==============================\n");
        printf(" Number Conversion Program\n");
        printf("==============================\n");
        printf("1. Decimal <-> Binary\n");
        printf("2. Binary <-> Hexadecimal\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &menu);

        switch(menu)
        {
            case 1:
                printf("\n1. Decimal to Binary\n");
                printf("2. Binary to Decimal\n");
                printf("Enter choice: ");
                scanf("%d", &choice);

                if(choice == 1)
                {
                    int decimal;

                    printf("Enter decimal (-127 to 127): ");
                    scanf("%d", &decimal);
                    decimalToBinary(decimal);
                }
                else if(choice == 2)
                {
                    printf("Enter 8-bit signed-magnitude binary: ");
                    scanf("%s", number);
                    binaryToDecimal(number);
                }
                else
                {
                    printf("Invalid choice.\n");
                }
                break;

            case 2:
                printf("\nEnter a positive binary or hexadecimal number.\n");
                printf("Hexadecimal numbers must start with 0x.\n");
                printf("Examples: 101101.1011 or 0x2D.B\n");
                printf("Enter number: ");
                scanf("%s", number);

                if(number[0] == '0' &&
                   (number[1] == 'x' || number[1] == 'X'))
                {
                    hexToBinary(number);
                }
                else
                {
                    binaryToHex(number);
                }
                break;

            case 3:
                printf("\nProgram ended.\n");
                break;

            default:
                printf("\nInvalid menu choice.\n");
        }

    } while(menu != 3);

    return 0;
}

/* Decimal -> 8-bit signed magnitude */
void decimalToBinary(int n)
{
    int bits[8];
    int magnitude;
    int i;

    if(n < -127 || n > 127)
    {
        printf("Error: number must be from -127 to 127.\n");
        return;
    }

    if(n < 0)
    {
        bits[0] = 1;       /* sign bit */
        magnitude = -n;
    }
    else
    {
        bits[0] = 0;       /* sign bit */
        magnitude = n;
    }

    for(i = 7; i >= 1; i--)
    {
        bits[i] = magnitude % 2;
        magnitude = magnitude / 2;
    }

    printf("Binary: ");

    for(i = 0; i < 8; i++)
    {
        printf("%d", bits[i]);
    }

    printf("\n");
}

/* 8-bit signed magnitude -> decimal */
void binaryToDecimal(char b[])
{
    int sign;
    int decimal = 0;
    int i;
    int valid = 1;

    if(strlen(b) != 8)
    {
        printf("Error: enter exactly 8 bits.\n");
        return;
    }

    for(i = 0; i < 8; i++)
    {
        if(b[i] != '0' && b[i] != '1')
        {
            valid = 0;
        }
    }

    if(valid == 0)
    {
        printf("Error: binary can only contain 0 and 1.\n");
        return;
    }

    sign = b[0] - '0';

    for(i = 1; i < 8; i++)
    {
        decimal = decimal * 2 + (b[i] - '0');
    }

    if(sign == 1)
    {
        decimal = -decimal;
    }

    printf("Decimal: %d\n", decimal);
}

/* Binary -> hexadecimal */
void binaryToHex(char b[])
{
    char integer[70];
    char fraction[70];
    int intLen = 0;
    int fracLen = 0;
    int dot = 0;
    int i, j, value, start, zeros;

    /* Separate the two sides of the binary point. */
    for(i = 0; i < strlen(b); i++)
    {
        if(b[i] == '.')
        {
            if(dot == 1)
            {
                printf("Error: too many decimal points.\n");
                return;
            }
            dot = 1;
        }
        else if(dot == 0)
        {
            integer[intLen] = b[i];
            intLen++;
        }
        else
        {
            fraction[fracLen] = b[i];
            fracLen++;
        }
    }

    integer[intLen] = '\0';
    fraction[fracLen] = '\0';

    if(intLen > 32 || fracLen > 32 || intLen == 0)
    {
        printf("Error: maximum is 32 bits on each side.\n");
        return;
    }

    for(i = 0; i < intLen; i++)
    {
        if(integer[i] != '0' && integer[i] != '1')
        {
            printf("Error: binary can only contain 0 and 1.\n");
            return;
        }
    }

    for(i = 0; i < fracLen; i++)
    {
        if(fraction[i] != '0' && fraction[i] != '1')
        {
            printf("Error: binary can only contain 0 and 1.\n");
            return;
        }
    }

    /* Remove unnecessary leading zeroes. */
    start = 0;
    while(start < intLen - 1 && integer[start] == '0')
    {
        start++;
    }

    printf("Hexadecimal: 0x");

    /*
     * Convert integer bits four at a time.
     * Add zeroes to the LEFT of the first group when needed.
     */
    zeros = (4 - (intLen - start) % 4) % 4;

    value = 0;

    for(j = 0; j < zeros; j++)
    {
        value = value * 2;
    }

    i = start;

    while(i < intLen)
    {
        value = value * 2 + (integer[i] - '0');

        if((i - start + 1 + zeros) % 4 == 0)
        {
            printf("%X", value);
            value = 0;
        }

        i++;
    }

    /* Convert fractional bits four at a time. */
    if(fracLen > 0)
    {
        printf(".");

        i = 0;

        while(i < fracLen)
        {
            value = 0;

            for(j = 0; j < 4; j++)
            {
                value = value * 2;

                if(i < fracLen)
                {
                    value = value + (fraction[i] - '0');
                    i++;
                }
            }

            printf("%X", value);
        }
    }

    printf("\n");
}

/* Hexadecimal -> binary */
void hexToBinary(char h[])
{
    int i;
    int dot = -1;
    int valid = 1;
    int lastFraction;

    if(strlen(h) < 3)
    {
        printf("Error: invalid hexadecimal number.\n");
        return;
    }

    /* Find the hexadecimal point. */
    for(i = 2; i < strlen(h); i++)
    {
        if(h[i] == '.')
        {
            dot = i;
        }
    }

    /* Check the hexadecimal digits. */
    for(i = 2; i < strlen(h); i++)
    {
        if(i == dot)
        {
            continue;
        }

        if(!((h[i] >= '0' && h[i] <= '9') ||
             (h[i] >= 'A' && h[i] <= 'F') ||
             (h[i] >= 'a' && h[i] <= 'f')))
        {
            valid = 0;
        }
    }

    if(valid == 0)
    {
        printf("Error: invalid hexadecimal digit.\n");
        return;
    }

    if(dot == -1)
    {
        if(strlen(h) - 2 > 8)
        {
            printf("Error: maximum is 8 hex digits.\n");
            return;
        }
    }
    else
    {
        if(dot - 2 > 8 || strlen(h) - dot - 1 > 8)
        {
            printf("Error: maximum is 8 hex digits on each side.\n");
            return;
        }
    }

    printf("Binary: ");

    /* Print the integer portion, suppressing leading zeroes. */
    i = 2;

    while(i < strlen(h) && h[i] == '0' && i != dot)
    {
        i++;
    }

    if(i == dot || i == strlen(h))
    {
        printf("0");
    }
    else
    {
        /* The first digit does not need unnecessary leading zeroes. */
        printFirstHexAsBinary(h[i]);
        i++;

        while(i < strlen(h) && h[i] != '.')
        {
            printHexAsBinary(h[i]);
            i++;
        }
    }

    /* Print fractional portion, suppressing trailing zero hex digits. */
    if(dot != -1 && dot < strlen(h) - 1)
    {
        lastFraction = strlen(h) - 1;

        while(lastFraction > dot && h[lastFraction] == '0')
        {
            lastFraction--;
        }

        if(lastFraction > dot)
        {
            printf(".");

            for(i = dot + 1; i <= lastFraction; i++)
            {
                printHexAsBinary(h[i]);
            }
        }
    }

    printf("\n");
}

/* One hexadecimal digit -> four binary bits */
void printHexAsBinary(char c)
{
    switch(c)
    {
        case '0': printf("0000"); break;
        case '1': printf("0001"); break;
        case '2': printf("0010"); break;
        case '3': printf("0011"); break;
        case '4': printf("0100"); break;
        case '5': printf("0101"); break;
        case '6': printf("0110"); break;
        case '7': printf("0111"); break;
        case '8': printf("1000"); break;
        case '9': printf("1001"); break;
        case 'A': case 'a': printf("1010"); break;
        case 'B': case 'b': printf("1011"); break;
        case 'C': case 'c': printf("1100"); break;
        case 'D': case 'd': printf("1101"); break;
        case 'E': case 'e': printf("1110"); break;
        case 'F': case 'f': printf("1111"); break;
        default: printf("????");
    }
}


/* Print the first hex digit without leading zeroes. */
void printFirstHexAsBinary(char c)
{
    switch(c)
    {
        case '0': printf("0"); break;
        case '1': printf("1"); break;
        case '2': printf("10"); break;
        case '3': printf("11"); break;
        case '4': printf("100"); break;
        case '5': printf("101"); break;
        case '6': printf("110"); break;
        case '7': printf("111"); break;
        case '8': printf("1000"); break;
        case '9': printf("1001"); break;
        case 'A': case 'a': printf("1010"); break;
        case 'B': case 'b': printf("1011"); break;
        case 'C': case 'c': printf("1100"); break;
        case 'D': case 'd': printf("1101"); break;
        case 'E': case 'e': printf("1110"); break;
        case 'F': case 'f': printf("1111"); break;
        default: printf("????");
    }
}
