#ifndef INPUTFIELDS_H
#define INPUTFIELDS_H

#include <stdbool.h>

#define MAX_FIELDS 3 

typedef enum {
    STRING,
    INTEGER,
    FLOAT,
    DATE,
    CAP
} InputFieldType;

typedef struct {
    char* prompt;
    char input_buffer[100]; // Buffer para la entrada del usuario
    int max_length;
    bool password_mode;
    int start_x;
    int start_y;
    int count;
    int cursor_pos;
    int type;
} InputField;

void init_input_field(InputField* field, const char* prompt, int max_length, bool password_mode, int start_x, int start_y, int type);
void draw_input_field(const InputField* field);
void draw_date_field(const InputField* field);
void handle_input_char(InputField* field, int ch, bool ins);
void handle_backspace(InputField* field);
void handle_cursor_left(InputField* field);
void handle_cursor_right(InputField* field);
void input_fields_loop(InputField fields[], int num_fields, void (*background)(void)); // Function to handle input loop for fields

#endif
