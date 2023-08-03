#ifndef MY_PRINTF_H
#define MY_PRINTF_H

#include <stdarg.h>

int _printf(const char *format, ...);

/* Function prototypes for custom conversion specifiers */
void print_binary(unsigned int num);
void apply_rot13(char *str);

#endif /* MY_PRINTF_H */

