#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // Define el número máximo de campos de entrada
#include "inputfields.h"

void init_input_field(InputField* field, const char* prompt, int max_length, bool password_mode, int start_x, int start_y) {
    field->prompt = strdup(prompt); // Usar strdup para asignar memoria al prompt
    field->max_length = max_length;
    field->password_mode = password_mode;
    field->start_x = start_x;
    field->start_y = start_y;
    memset(field->input_buffer, 0, sizeof(field->input_buffer));
    field->count = 0;
    field->cursor_pos = 0;
}

void draw_input_field(const InputField* field) {
    move(field->start_y, field->start_x);
    clrtoeol();
    mvprintw(field->start_y, field->start_x, "%s", field->prompt);
    for (int i = 0; i < field->count; i++) {
        if (field->password_mode) {
            addch('*');
        } else {
            addch(field->input_buffer[i]);
        }
    }
    move(field->start_y, field->start_x + strlen(field->prompt) + field->cursor_pos); // Posicionar el cursor
}

void handle_input_char(InputField* field, int ch) {
    if (field->count < field->max_length && ch >= 32 && ch <= 126) {
        // Insertar el carácter en la posición actual del cursor.
        for (int i = field->count; i > field->cursor_pos; i--) {
            field->input_buffer[i] = field->input_buffer[i - 1];
        }
        field->input_buffer[field->cursor_pos] = (char)ch;
        field->count++;
        field->cursor_pos++;
    }
}

void handle_backspace(InputField* field) {
    if (field->cursor_pos > 0) {
        for (int i = field->cursor_pos; i < field->count; i++) {
            field->input_buffer[i - 1] = field->input_buffer[i];
        }
        field->input_buffer[field->count - 1] = '\0';
        field->count--;
        field->cursor_pos--;
    }
}

void handle_cursor_left(InputField* field) {
    if (field->cursor_pos > 0) {
        field->cursor_pos--;
    }
}

void handle_cursor_right(InputField* field) {
    if (field->cursor_pos < field->count) {
        field->cursor_pos++;
    }
}