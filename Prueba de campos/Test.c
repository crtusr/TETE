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
    int current_field_index = 0;

    // Inicializar los campos de entrada
    init_input_field(&fields[0], "Nombre: ", 20, false, 10, 5);
    init_input_field(&fields[1], "Apellido: ", 20, false, 10, 7);
    init_input_field(&fields[2], "Contraseña: ", 15, true, 10, 9);

    // Bucle principal de entrada
    while (1) {
        clear(); // Limpiar la pantalla en cada iteración (opcional, pero útil para redibujar todo)

        // Dibujar todos los campos de entrada
        for (int i = 0; i < MAX_FIELDS; i++) {
            draw_input_field(&fields[i]);
        }

        refresh();

	move(fields[current_field_index].start_y, fields[current_field_index].start_x + strlen(fields[current_field_index].prompt) + fields[current_field_index].cursor_pos);

        int ch = getch();
	

        if (ch == KEY_ENTER || ch == '\n' || ch == '\r') {
            if (current_field_index == MAX_FIELDS - 1) { // Si ENTER en el último campo, salir
                break;
            } else {
                current_field_index++; // Si ENTER, ir al siguiente campo (opcional)
            }
        } else if (ch == KEY_UP) {
            current_field_index--;
            if (current_field_index < 0) {
                current_field_index = MAX_FIELDS - 1; // Volver al último campo si llega al principio
            }
        } else if (ch == KEY_DOWN) {
            current_field_index++;
            if (current_field_index >= MAX_FIELDS) {
                current_field_index = 0; // Volver al primer campo si llega al final
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

    clear();
    mvprintw(15, 10, "Datos introducidos:");
    for (int i = 0; i < MAX_FIELDS; i++) {
        mvprintw(16 + i, 10, "%s: %s", fields[i].prompt, fields[i].input_buffer);
    }
    refresh();
    getch();

    endwin();

    // Liberar la memoria asignada para los prompts
    for (int i = 0; i < MAX_FIELDS; i++) {
        free(fields[i].prompt);
    }

    return 0;
}