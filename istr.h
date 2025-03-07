#ifndef ISTR_H
#define ISTR_H

#include <stdbool.h>

char* get_limited_input(int max_length, const char* prompt, bool password_mode, int start_x, int start_y);

#endif