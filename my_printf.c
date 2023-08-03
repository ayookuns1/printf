#include "my_printf.h"
#include <unistd.h>

int _printf(const char *format, ...)
{
   char buffer[BUFFER_SIZE];
    char *buf_ptr = buffer;
    int count = 0;

    va_list args;
    va_start(args, format);

    while (*format)
    {
        if (*format == '%')
        {
            format++; /* Move past the '%' */

            /* Handle the conversion specifiers */
            switch (*format)
            {
                case 'c':
                {
                    char ch = va_arg(args, int);
                    *buf_ptr = ch;
                    buf_ptr++;
                    count++;
                    break;
                }
                case 's':
                {
                    char *str = va_arg(args, char *);
                    while (*str)
                    {
                        *buf_ptr = *str;
                        buf_ptr++;
                        count++;
                        str++;
                    }
                    break;
                }
                case 'd':
                case 'i':
                {
                    int num = va_arg(args, int);
                    char num_buffer[12]; 
                    int num_len = snprintf(num_buffer, sizeof(num_buffer), "%d", num);
                    memcpy(buf_ptr, num_buffer, num_len);
                    buf_ptr += num_len;
                    count += num_len;
                    break;
                }
                case 'u':
                {
                    unsigned int num = va_arg(args, unsigned int);
                    char num_buffer[12]; 
                    int num_len = snprintf(num_buffer, sizeof(num_buffer), "%u", num);
                    memcpy(buf_ptr, num_buffer, num_len);
                    buf_ptr += num_len;
                    count += num_len;
                    break;
                }
                case 'o':
                {
                    unsigned int num = va_arg(args, unsigned int);
                    char num_buffer[12]; 
                    int num_len = snprintf(num_buffer, sizeof(num_buffer), "%o", num);
                    memcpy(buf_ptr, num_buffer, num_len);
                    buf_ptr += num_len;
                    count += num_len;
                    break;
                }
                case 'x':
                {
                    unsigned int num = va_arg(args, unsigned int);
                    char num_buffer[12]; 
                    int num_len = snprintf(num_buffer, sizeof(num_buffer), "%x", num);
                    memcpy(buf_ptr, num_buffer, num_len);
                    buf_ptr += num_len;
                    count += num_len;
                    break;
                }
                case 'X':
                {
                    unsigned int num = va_arg(args, unsigned int);
                    char num_buffer[12]; 
                    int num_len = snprintf(num_buffer, sizeof(num_buffer), "%X", num);
                    memcpy(buf_ptr, num_buffer, num_len);
                    buf_ptr += num_len;
                    count += num_len;
                    break;
                }
                case 'b':
                {
                    unsigned int num = va_arg(args, unsigned int);
                    print_binary(num, &buf_ptr, &count);
                    break;
                }
                case 'S':
                {
                    char *str = va_arg(args, char *);
                    print_hex_string(str, &buf_ptr, &count);
                    break;
                }
                case 'p':
                {
                    void *ptr = va_arg(args, void *);
                    print_pointer(ptr, &buf_ptr, &count);
                    break;
                }
                case 'r':
                {
                    char *str = va_arg(args, char *);
                    print_reversed_string(str, &buf_ptr, &count);
                    break;
                }
                case 'R':
                {
                    char *str = va_arg(args, char *);
                    print_rot13_string(str, &buf_ptr, &count);
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
                {
                    
                    *buf_ptr = *format;
                    buf_ptr++;
                    count++;
                }
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
            /* Buffer full or format string processing complete, write to stdout */
            *buf_ptr = '\0'; /* Null-terminate the buffer */
            write(1, buffer, buf_ptr - buffer);
            buf_ptr = buffer; /* Reset the buffer pointer */
        }
    }

    va_end(args);
    return count;
}

