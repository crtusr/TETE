#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "inputfields.h"

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    InputField fields[MAX_FIELDS];
    int num_fields = MAX_FIELDS; // Explicitly set number of fields

    // Initialize input fields
    init_input_field(&fields[0], "STRING: ", 20, false, 10, 5, STRING);
    init_input_field(&fields[1], "INTEGER: ", 20, false, 10, 7, INTEGER);
    init_input_field(&fields[2], "FLOAT: ", 15, false, 10, 9, FLOAT);

    input_fields_loop(fields, num_fields); // Call the input loop function from inputfields.c

    clear();
    mvprintw(15, 10, "Datos introducidos:");
    for (int i = 0; i < num_fields; i++) {
        mvprintw(16 + i, 10, "%s: %s", fields[i].prompt, fields[i].input_buffer);
    }
    refresh();
    getch();

    endwin();

    // Free allocated memory for prompts
    for (int i = 0; i < num_fields; i++) {
        free(fields[i].prompt);
    }

    return 0;
}
