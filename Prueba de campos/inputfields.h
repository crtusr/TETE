#ifndef INPUTFIELDS_H
#define INPUTFIELDS_H

#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // Define el número máximo de campos de entrada

#define MAX_FIELDS 3

typedef struct {
    char* prompt;
    int max_length;
    bool password_mode;
    int start_x;
    int start_y;
    char input_buffer[256]; // Buffer para cada campo
    int count;
    int cursor_pos;
} InputField;

void init_input_field(InputField* field, const char* prompt, int max_length, bool password_mode, int start_x, int start_y);
void draw_input_field(const InputField* field);
void handle_input_char(InputField* field, int ch);
void handle_backspace(InputField* field);
void handle_cursor_left(InputField* field);
void handle_cursor_right(InputField* field);

#endif