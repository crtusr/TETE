#include <curses.h>
#include <string.h>
#include <stdlib.h>  // Agrega esta línea
#include <stdbool.h>

char* get_limited_input(int max_length, const char* prompt, bool password_mode, int start_x, int start_y) {
    static char input_buffer[256];
    int ch;
    int count = 0;
    int row, col;
    int cursor_pos = 0; // Posición relativa dentro del buffer de entrada.

     // --- Inicialización ---
    if (max_length >= sizeof(input_buffer)) {
      endwin(); // En caso de error
      fprintf(stderr, "Error: max_length es demasiado grande para el buffer.\n");
      exit(1); // o return NULL;
    }

    // Limpiar el buffer al inicio.
    memset(input_buffer, 0, sizeof(input_buffer));

 // Obtener la fila actual (la columna no importa).
    col = start_x;
    row = start_y;          // Usar la columna inicial proporcionada.
    mvprintw(row, col, "%s", prompt); // Mostrar el prompt en la posición especificada.
    refresh();
    move(row, col + strlen(prompt));  // Mover el cursor al final del prompt.


    while (1) {
        ch = getch();

        if (ch == KEY_ENTER || ch == '\n' || ch == '\r') {
            break;
        } else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (cursor_pos > 0) {
                // Mover los caracteres a la izquierda.
                for (int i = cursor_pos; i < count; i++) {
                    input_buffer[i - 1] = input_buffer[i];
                }
                input_buffer[count - 1] = '\0';
                count--;
                cursor_pos--;

                // Redibujar toda la línea (desde el prompt).
                move(start_y, start_x);
                clrtoeol(); // Borrar desde la posición actual hasta el final de la línea
                mvprintw(start_y, start_x, "%s", prompt);
                for(int i = 0; i< count; i++){
                    if(password_mode){
                        addch('*');
                    } else {
                        addch(input_buffer[i]);
                    }
                }
                move(start_y, start_x + strlen(prompt) + cursor_pos); // Restaurar posición del cursor.
            }
        } else if (ch == KEY_LEFT) {
            if (cursor_pos > 0) {
                cursor_pos--;
                move(start_y, start_x + strlen(prompt) + cursor_pos);
            }
        } else if (ch == KEY_RIGHT) {
            if (cursor_pos < count) {
                cursor_pos++;
                move(start_y, start_x + strlen(prompt) + cursor_pos);
            }
        } else if (count < max_length && ch >= 32 && ch <= 126) {
              // Insertar el carácter en la posición actual del cursor.

               // Mover los caracteres a la derecha (si es necesario).
                for (int i = count; i > cursor_pos; i--) {
                    input_buffer[i] = input_buffer[i - 1];
                }

                input_buffer[cursor_pos] = (char)ch;
                count++;

                // Redibujar la línea
                move(start_y, start_x);
                clrtoeol();
                mvprintw(start_y, start_x, "%s", prompt);

                for(int i = 0; i< count; i++){
                    if(password_mode){
                         addch('*');
                    } else{
                        addch(input_buffer[i]);
                    }
                }
                cursor_pos++;
                move(start_y, start_x + strlen(prompt) + cursor_pos);
        }
        refresh();
    }

    input_buffer[count] = '\0';
    return input_buffer;
}