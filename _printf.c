#include "my_printf.h"
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

/* Function to print binary representation of an unsigned integer */
void print_binary(unsigned int num)
{
    if (num > 1)
        print_binary(num / 2);

    putchar('0' + (num % 2));
}

/* Function to apply ROT13 encoding to a string */
void apply_rot13(char *str)
{
    while (*str)
    {
        if ((*str >= 'A' && *str <= 'Z') || (*str >= 'a' && *str <= 'z'))
        {
            char base = (*str >= 'A' && *str <= 'Z') ? 'A' : 'a';
            *str = (char)(((*str - base + 13) % 26) + base);
        }
        str++;
    }
}

/**
 * _printf - produces output according to a format.
 * @format: a character string containing zero or more directives.
 *
 * Return: the number of characters printed (excluding the null byte used to
 * end output to strings).
 */
int _printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    int count = 0; /* To keep track of the number of characters printed */
    char buffer[BUFFER_SIZE];
    char *buf_ptr = buffer;

    while (*format)
    {
        if (*format == '%')
        {
            format++; /* Move to the character after '%' */

            /* Flags to track the usage of +, space, 0, and - flags */
            int flag_plus = 0;
            int flag_space = 0;
            int flag_hash = 0;
            int flag_zero = 0;
            int flag_minus = 0;

            /* Read flags */
            while (*format == '+' || *format == ' ' || *format == '#' || *format == '0' || *format == '-')
            {
                if (*format == '+')
                    flag_plus = 1;
                else if (*format == ' ')
                    flag_space = 1;
                else if (*format == '#')
                    flag_hash = 1;
                else if (*format == '0')
                    flag_zero = 1;
                else if (*format == '-')
                    flag_minus = 1;

                format++;
            }

            /* Field width */
            int field_width = 0;
            if (*format == '*')
            {
                field_width = va_arg(args, int);
                format++;
            }
            else
            {
                while (*format >= '0' && *format <= '9')
                {
                    field_width = field_width * 10 + (*format - '0');
                    format++;
                }
            }

            /* Precision */
            int precision = -1;
            if (*format == '.')
            {
                format++;
                if (*format == '*')
                {
                    precision = va_arg(args, int);
                    format++;
                }
                else
                {
                    precision = 0;
                    while (*format >= '0' && *format <= '9')
                    {
                        precision = precision * 10 + (*format - '0');
                        format++;
                    }
                }
            }

            /* Length modifiers */
            int length_modifier = 0;
            while (*format == 'l' || *format == 'h')
            {
                if (*format == 'l')
                    length_modifier++;
                else if (*format == 'h')
                    length_modifier--;

                format++;
            }

            /* Handle different format specifiers */
            switch (*format)
            {
                case 'c':
                {
                    int ch = va_arg(args, int); /* Note: char is promoted to int in va_arg */

                    if (flag_minus)
                    {
                        *buf_ptr = ch;
                        buf_ptr++;
                        count++;
                        if (field_width > 1)
                        {
                            int padding = field_width - 1;
                            while (padding--)
                            {
                                *buf_ptr = ' ';
                                buf_ptr++;
                                count++;
                            }
                        }
                    }
                    else
                    {
                        if (field_width > 1)
                        {
                            int padding = field_width - 1;
                            while (padding--)
                            {
                                *buf_ptr = flag_zero ? '0' : ' ';
                                buf_ptr++;
                                count++;
                            }
                        }
                        *buf_ptr = ch;
                        buf_ptr++;
                        count++;
                    }
                    break;
                }
                case 's':
                {
                    char *str = va_arg(args, char *);
                    int str_len = 0;
                    while (str[str_len])
                        str_len++;

                    if (precision >= 0 && precision < str_len)
                        str_len = precision;

                    if (flag_minus)
                    {
                        int i;
                        for (i = 0; i < str_len; i++)
                        {
                            *buf_ptr = str[i];
                            buf_ptr++;
                            count++;
                        }

                        if (field_width > str_len)
                        {
                            int padding = field_width - str_len;
                            while (padding--)
                            {
                                *buf_ptr = ' ';
                                buf_ptr++;
                                count++;
                            }
                        }
                    }
                    else
                    {
                        if (field_width > str_len)
                        {
                            int padding = field_width - str_len;
                            while (padding--)
                            {
                                *buf_ptr = flag_zero ? '0' : ' ';
                                buf_ptr++;
                                count++;
                            }
                        }

                        int i;
                        for (i = 0; i < str_len; i++)
                        {
                            *buf_ptr = str[i];
                            buf_ptr++;
                            count++;
                        }
                    }
                    break;
                }
                case 'd':
                case 'i':
                {
                    if (length_modifier == 2)
                    {
                        long long int num = va_arg(args, long long int);
                        int num_len = snprintf(NULL, 0, "%lld", num);

                        if (flag_plus && num >= 0)
                        {
                            num_len++; /* For the '+' sign */
                        }
                        else if (flag_space && num >= 0)
                        {
                            num_len++; /* For the space */
                        }

                        if (precision > num_len - (flag_plus || flag_space))
                        {
                            int padding = precision - (num_len - (flag_plus || flag_space));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_plus && num >= 0)
                        {
                            *buf_ptr = '+';
                            buf_ptr++;
                            count++;
                        }
                        else if (flag_space && num >= 0)
                        {
                            *buf_ptr = ' ';
                            buf_ptr++;
                            count++;
                        }
                        count += sprintf(buf_ptr, "%lld", num);
                        buf_ptr += count;
                    }
                    else if (length_modifier == 1)
                    {
                        long int num = va_arg(args, long int);
                        int num_len = snprintf(NULL, 0, "%ld", num);

                        if (flag_plus && num >= 0)
                        {
                            num_len++; /* For the '+' sign */
                        }
                        else if (flag_space && num >= 0)
                        {
                            num_len++; /* For the space */
                        }

                        if (precision > num_len - (flag_plus || flag_space))
                        {
                            int padding = precision - (num_len - (flag_plus || flag_space));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_plus && num >= 0)
                        {
                            *buf_ptr = '+';
                            buf_ptr++;
                            count++;
                        }
                        else if (flag_space && num >= 0)
                        {
                            *buf_ptr = ' ';
                            buf_ptr++;
                            count++;
                        }
                        count += sprintf(buf_ptr, "%ld", num);
                        buf_ptr += count;
                    }
                    else
                    {
                        int num = va_arg(args, int);
                        int num_len = snprintf(NULL, 0, "%d", num);

                        if (flag_plus && num >= 0)
                        {
                            num_len++; /* For the '+' sign */
                        }
                        else if (flag_space && num >= 0)
                        {
                            num_len++; /* For the space */
                        }

                        if (precision > num_len - (flag_plus || flag_space))
                        {
                            int padding = precision - (num_len - (flag_plus || flag_space));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_plus && num >= 0)
                        {
                            *buf_ptr = '+';
                            buf_ptr++;
                            count++;
                        }
                        else if (flag_space && num >= 0)
                        {
                            *buf_ptr = ' ';
                            buf_ptr++;
                            count++;
                        }
                        count += sprintf(buf_ptr, "%d", num);
                        buf_ptr += count;
                    }
                    break;
                }
                case 'u':
                {
                    if (length_modifier == 2)
                    {
                        unsigned long long int num = va_arg(args, unsigned long long int);
                        int num_len = snprintf(NULL, 0, "%llu", num);

                        if (precision > num_len)
                        {
                            int padding = precision - num_len;
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        count += sprintf(buf_ptr, "%llu", num);
                        buf_ptr += count;
                    }
                    else if (length_modifier == 1)
                    {
                        unsigned long int num = va_arg(args, unsigned long int);
                        int num_len = snprintf(NULL, 0, "%lu", num);

                        if (precision > num_len)
                        {
                            int padding = precision - num_len;
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        count += sprintf(buf_ptr, "%lu", num);
                        buf_ptr += count;
                    }
                    else
                    {
                        unsigned int num = va_arg(args, unsigned int);
                        int num_len = snprintf(NULL, 0, "%u", num);

                        if (precision > num_len)
                        {
                            int padding = precision - num_len;
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        count += sprintf(buf_ptr, "%u", num);
                        buf_ptr += count;
                    }
                    break;
                }
                case 'o':
                {
                    if (length_modifier == 2)
                    {
                        unsigned long long int num = va_arg(args, unsigned long long int);
                        int num_len = snprintf(NULL, 0, "%llo", num);

                        if (flag_hash && num != 0)
                        {
                            num_len++; /* For the '0' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            count++;
                        }
                        count += sprintf(buf_ptr, "%llo", num);
                        buf_ptr += count;
                    }
                    else if (length_modifier == 1)
                    {
                        unsigned long int num = va_arg(args, unsigned long int);
                        int num_len = snprintf(NULL, 0, "%lo", num);

                        if (flag_hash && num != 0)
                        {
                            num_len++; /* For the '0' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            count++;
                        }
                        count += sprintf(buf_ptr, "%lo", num);
                        buf_ptr += count;
                    }
                    else
                    {
                        unsigned int num = va_arg(args, unsigned int);
                        int num_len = snprintf(NULL, 0, "%o", num);

                        if (flag_hash && num != 0)
                        {
                            num_len++; /* For the '0' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            count++;
                        }
                        count += sprintf(buf_ptr, "%o", num);
                        buf_ptr += count;
                    }
                    break;
                }
                case 'x':
                {
                    if (length_modifier == 2)
                    {
                        unsigned long long int num = va_arg(args, unsigned long long int);
                        int num_len = snprintf(NULL, 0, "%llx", num);

                        if (flag_hash && num != 0)
                        {
                            num_len += 2; /* For the '0x' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            *buf_ptr = 'x';
                            buf_ptr++;
                            count += 2;
                        }
                        count += sprintf(buf_ptr, "%llx", num);
                        buf_ptr += count;
                    }
                    else if (length_modifier == 1)
                    {
                        unsigned long int num = va_arg(args, unsigned long int);
                        int num_len = snprintf(NULL, 0, "%lx", num);

                        if (flag_hash && num != 0)
                        {
                            num_len += 2; /* For the '0x' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            *buf_ptr = 'x';
                            buf_ptr++;
                            count += 2;
                        }
                        count += sprintf(buf_ptr, "%lx", num);
                        buf_ptr += count;
                    }
                    else
                    {
                        unsigned int num = va_arg(args, unsigned int);
                        int num_len = snprintf(NULL, 0, "%x", num);

                        if (flag_hash && num != 0)
                        {
                            num_len += 2; /* For the '0x' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            *buf_ptr = 'x';
                            buf_ptr++;
                            count += 2;
                        }
                        count += sprintf(buf_ptr, "%x", num);
                        buf_ptr += count;
                    }
                    break;
                }
                case 'X':
                {
                    if (length_modifier == 2)
                    {
                        unsigned long long int num = va_arg(args, unsigned long long int);
                        int num_len = snprintf(NULL, 0, "%llX", num);

                        if (flag_hash && num != 0)
                        {
                            num_len += 2; /* For the '0X' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            *buf_ptr = 'X';
                            buf_ptr++;
                            count += 2;
                        }
                        count += sprintf(buf_ptr, "%llX", num);
                        buf_ptr += count;
                    }
                    else if (length_modifier == 1)
                    {
                        unsigned long int num = va_arg(args, unsigned long int);
                        int num_len = snprintf(NULL, 0, "%lX", num);

                        if (flag_hash && num != 0)
                        {
                            num_len += 2; /* For the '0X' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            *buf_ptr = 'X';
                            buf_ptr++;
                            count += 2;
                        }
                        count += sprintf(buf_ptr, "%lX", num);
                        buf_ptr += count;
                    }
                    else
                    {
                        unsigned int num = va_arg(args, unsigned int);
                        int num_len = snprintf(NULL, 0, "%X", num);

                        if (flag_hash && num != 0)
                        {
                            num_len += 2; /* For the '0X' prefix */
                        }

                        if (precision > num_len - (flag_hash && num != 0))
                        {
                            int padding = precision - (num_len - (flag_hash && num != 0));
                            while (padding--)
                            {
                                *buf_ptr = '0';
                                buf_ptr++;
                                count++;
                            }
                        }

                        if (flag_hash && num != 0)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            *buf_ptr = 'X';
                            buf_ptr++;
                            count += 2;
                        }
                        count += sprintf(buf_ptr, "%X", num);
                        buf_ptr += count;
                    }
                    break;
                }
                case 'b':
                {
                    unsigned int num = va_arg(args, unsigned int);
                    int num_len = snprintf(NULL, 0, "%u", num);

                    if (precision > num_len)
                    {
                        int padding = precision - num_len;
                        while (padding--)
                        {
                            *buf_ptr = '0';
                            buf_ptr++;
                            count++;
                        }
                    }

                    print_binary(num);
                    break;
                }
                case 'S':
                {
                    char *str = va_arg(args, char *);
                    int str_len = 0;
                    while (str[str_len])
                        str_len++;

                    if (precision >= 0 && precision < str_len)
                        str_len = precision;

                    if (field_width > str_len)
                    {
                        int padding = field_width - str_len;
                        while (padding--)
                        {
                            *buf_ptr = flag_zero ? '0' : ' ';
                            buf_ptr++;
                            count++;
                        }
                    }

                    while (*str && str_len > 0)
                    {
                        if (*str < 32 || *str >= 127)
                        {
                            // Print non-printable characters in the \x format
                            count += sprintf(buf_ptr, "\\x%02X", (unsigned char)*str);
                            buf_ptr += count;
                        }
                        else
                        {
                            *buf_ptr = *str;
                            buf_ptr++;
                            count++;
                        }
                        str++;
                        str_len--;
                    }
                    break;
                }
                case 'p':
                {
                    void *ptr = va_arg(args, void *);
                    int ptr_len = snprintf(NULL, 0, "%p", ptr);

                    if (field_width > ptr_len)
                    {
                        int padding = field_width - ptr_len;
                        while (padding--)
                        {
                            *buf_ptr = flag_zero ? '0' : ' ';
                            buf_ptr++;
                            count++;
                        }
                    }

                    count += sprintf(buf_ptr, "%p", ptr);
                    buf_ptr += count;
                    break;
                }
                case 'r':
                {
                    char *str = va_arg(args, char *);
                    int str_len = 0;
                    while (str[str_len])
                        str_len++;

                    if (field_width > str_len)
                    {
                        int padding = field_width - str_len;
                        while (padding--)
                        {
                            *buf_ptr = flag_zero ? '0' : ' ';
                            buf_ptr++;
                            count++;
                        }
                    }

                    while (str_len > 0)
                    {
                        *buf_ptr = str[str_len - 1];
                        buf_ptr++;
                        count++;
                        str_len--;
                    }
                    break;
                }
                case 'R':
                {
                    char *str = va_arg(args, char *);
                    int str_len = 0;
                    while (str[str_len])
                        str_len++;

                    if (field_width > str_len)
                    {
                        int padding = field_width - str_len;
                        while (padding--)
                        {
                            *buf_ptr = flag_zero ? '0' : ' ';
                            buf_ptr++;
                            count++;
                        }
                    }

                    apply_rot13(str);

                    while (*str)
                    {
                        *buf_ptr = *str;
                        buf_ptr++;
                        count++;
                        str++;
                    }
                    break;
                }
                case '%':
                {
                    *buf_ptr = '%';
                    buf_ptr++;
                    count++;
                    break;
                }
                default:
                    /* If the format specifier is not recognized, simply print the character */
                    *buf_ptr = *format;
                    buf_ptr++;
                    count++;
            }
        }
        else
        {
            *buf_ptr = *format;
            buf_ptr++;
            count++;
        }

        format++;

        if (buf_ptr - buffer >= BUFFER_SIZE - 1 || *format == '\0')
        {
            // Buffer full or format string processing complete, write to stdout
            *buf_ptr = '\0'; // Null-terminate the buffer
            write(1, buffer, buf_ptr - buffer);
            buf_ptr = buffer; // Reset the buffer pointer
        }
    }

    va_end(args);
    return count;
}

