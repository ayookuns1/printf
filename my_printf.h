#ifndef MY_PRINTF_H
#define MY_PRINTF_H

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define BUFFER_SIZE 1024

int _printf(const char *format, ...);
void print_binary(unsigned int num, char **buf_ptr, int *count);
void print_hex_string(char *str, char **buf_ptr, int *count);
void print_pointer(void *ptr, char **buf_ptr, int *count);
void print_reversed_string(char *str, char **buf_ptr, int *count);
void print_rot13_string(char *str, char **buf_ptr, int *count);


#endif /* MY_PRINTF_H */

