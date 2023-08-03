#include "my_printf.h"
#include <stdio.h>
#include <string.h>

void print_binary(unsigned int num, char **buf_ptr, int *count)
{
    int i;
    int num_len = 32;
    char num_buffer[33]; /* Assuming a 32-bit binary representation (+ 1 for '\0') */

    for (i = 31; i >= 0; i--)
    {
        num_buffer[i] = (num & 1) ? '1' : '0';
        num >>= 1;
    }

    num_buffer[32] = '\0';
    memcpy(*buf_ptr, num_buffer, num_len);
    *buf_ptr += num_len;
    *count += num_len;
}

void print_hex_string(char *str, char **buf_ptr, int *count)
{
    int num_len = 4;
    char buffer[4]; /* Assuming the longest possible '\xXX' representation (+ 1 for '\0') */
    while (*str)
    {
        if (*str < 32 || *str >= 127)
        {
            sprintf(buffer, "\\x%02X", (unsigned char)*str);
            memcpy(*buf_ptr, buffer, num_len);
            *buf_ptr += num_len;
            *count += num_len;
        }
        else
        {
            **buf_ptr = *str;
            *buf_ptr += 1;
            *count += 1;
        }
        str++;
    }
}
void print_pointer(void *ptr, char **buf_ptr, int *count)
{
    char buffer[20]; 
    int num_len = sprintf(buffer, "%p", ptr);
    memcpy(*buf_ptr, buffer, num_len);
    *buf_ptr += num_len;
    *count += num_len;
}

void print_reversed_string(char *str, char **buf_ptr, int *count)
{
    int len = strlen(str);
    int i;
    for (i = len - 1; i >= 0; i--)
    {
        **buf_ptr = str[i];
        *buf_ptr += 1;
        *count += 1;
    }
}

void print_rot13_string(char *str, char **buf_ptr, int *count)
{
    while (*str)
    {
        char ch = *str;
        if (('a' <= ch && ch <= 'm') || ('A' <= ch && ch <= 'M'))
            ch += 13;
        else if (('n' <= ch && ch <= 'z') || ('N' <= ch && ch <= 'Z'))
            ch -= 13;

        **buf_ptr = ch;
        *buf_ptr += 1;
        *count += 1;
        str++;
    }
}
