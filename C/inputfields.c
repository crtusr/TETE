#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> // Define el número máximo de campos de entrada
#include <ctype.h>
#include "inputfields.h"

void init_input_field(InputField* field, const char* prompt, int max_length, bool password_mode, int start_x, int start_y, int type) {
    field->prompt = strdup(prompt); // Usar strdup para asignar memoria al prompt
    field->max_length = max_length;
    field->password_mode = password_mode;
    field->start_x = start_x;
    field->start_y = start_y;
    memset(field->input_buffer, 0, sizeof(field->input_buffer));
    field->count = 0;
    field->cursor_pos = 0;
    field->type = type;
}

void draw_input_field(const InputField* field) {
    	move(field->start_y, field->start_x);
    	clrtoeol();
   	mvprintw(field->start_y, field->start_x, "%s", field->prompt);
	attron(A_REVERSE);
	for (int i = 0; i < field->max_length; i++) {
		addch(' ');
	}
	move(field->start_y, field->start_x + strlen(field->prompt));
	for (int i = 0; i < field->count; i++) {
        	if (field->password_mode) {
            		addch('*');
        	} else {
            		addch(field->input_buffer[i]);
        	}
	
    	}
attroff(A_REVERSE);
    move(field->start_y, field->start_x + strlen(field->prompt) + field->cursor_pos); // Posicionar el cursor
}

static void insert_char(InputField* field, char ch) {
    if (field->count < field->max_length) {
        // Use memmove for efficient shifting of the buffer
        memmove(&field->input_buffer[field->cursor_pos + 1],
                &field->input_buffer[field->cursor_pos],
                field->count - field->cursor_pos);

        field->input_buffer[field->cursor_pos] = ch;
        field->count++;
        field->cursor_pos++;
        field->input_buffer[field->count] = '\0'; // Ensure null termination
    }
}

void handle_input_char(InputField* field, int ch) {
    // Check for valid input types and character ranges *before* inserting
    switch (field->type) {
        case STRING:
            if (isprint(ch)) { // Use isprint for printable characters
                insert_char(field, (char)ch);
            }
            break;
        case INTEGER:
		if (field->cursor_pos == 0 && ch == '-') {
			insert_char(field, (char)ch);
		}
            	if (isdigit(ch)) {  // Use isdigit for digits
                	insert_char(field, (char)ch);
            	}
            break;
        case FLOAT:
            // Improved FLOAT handling (allows digits, '.', '+', '-')
            if (isdigit(ch) || ch == '.' || ch == '+' || ch == '-') {
                // Add more sophisticated validation for FLOAT here if needed
                // (e.g., only one decimal point, sign only at the beginning).

                // rudimentary validation for float input
                if (ch == '.' && strchr(field->input_buffer, '.') != NULL) {
                  break; // only allow one decimal point
                }
                if ((ch == '+' || ch == '-') && field->cursor_pos != 0 ) {
                   break;
                }
                 insert_char(field, (char)ch);
            }
            break;
        default:
            // Handle invalid field type (optional, but good practice)
            break;
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
		if (field->cursor_pos < field->count) { // Fixed: changed field->max_length to field->count
        field->cursor_pos++;
    }
}

void input_fields_loop(InputField fields[], int num_fields) {
    int current_field_index = 0;
    while (1) {
        clear();

        for (int i = 0; i < num_fields; i++) {
            draw_input_field(&fields[i]);
        }

        refresh();

        move(fields[current_field_index].start_y, fields[current_field_index].start_x + strlen(fields[current_field_index].prompt) + fields[current_field_index].cursor_pos);

        int ch = getch();

        if (ch == KEY_ENTER || ch == '\n' || ch == '\r' || ch == PADENTER) {
            if (current_field_index == num_fields - 1) {
                break; // Exit loop after last field
            } else {
                current_field_index++;
            }
        } else if (ch == KEY_UP) {
            current_field_index--;
            if (current_field_index < 0) {
                current_field_index = num_fields - 1;
            }
        } else if (ch == KEY_DOWN) {
            current_field_index++;
            if (current_field_index >= num_fields) {
                current_field_index = 0;
            }
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            handle_backspace(&fields[current_field_index]);
        } else if (ch == KEY_LEFT) {
            handle_cursor_left(&fields[current_field_index]);
        } else if (ch == KEY_RIGHT) {
            handle_cursor_right(&fields[current_field_index]);
        } else if (ch >= 32 && ch <= 126) {
            handle_input_char(&fields[current_field_index], ch);
        }
    }
}
