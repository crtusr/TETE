#include "menu.h"
#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include "fondos.h"

// recuadro function (no changes)
void recuadro(int start_x, int start_y, int width, int height) {
    mvhline(start_y, start_x, 0, width);
    mvhline(start_y + height - 1, start_x, 0, width);
    mvvline(start_y, start_x, 0, height);
    mvvline(start_y, start_x + width - 1, 0, height);
    mvaddch(start_y, start_x, ACS_ULCORNER);
    mvaddch(start_y, start_x + width - 1, ACS_URCORNER);
    mvaddch(start_y + height - 1, start_x, ACS_LLCORNER);
    mvaddch(start_y + height - 1, start_x + width - 1, ACS_LRCORNER);
}

static void redibujar_menu_items(
    MenuItem *menu,
    int current_row,
    int current_col,
    int max_width,
    int num_rows, // Reemplaza a menu_height, ya que eran iguales
    int *num_cols_per_row
);

static void redibujar_menu_items(
    MenuItem *menu,
    int current_row,
    int current_col,
    int max_width,
    int num_rows, // Reemplaza a menu_height
    int *num_cols_per_row
) {
    if (menu == NULL) return; // Seguridad

    int start_x = menu->x - 2;
    int start_y = menu->y - 1;
    int i = 0;
    int current_row_index = 0;
    int col_count = 0;
    int last_y = -1;

    // 1. Limpiar el área del menú (¡importante antes de redibujar!)
    // Usa num_rows donde antes usabas menu_height
    for (int r = 0; r < num_rows + 2; r++) {
        // Asegúrate de que las coordenadas no sean negativas
        if (start_y + r >= 0 && start_x >= 0) {
             mvprintw(start_y + r, start_x, "%*s", max_width + 4, "");
        }
    }

    // 2. Dibujar el fondo específico del (sub)menú si existe
    if (menu->draw_background != NULL) {
        menu->draw_background(start_x, start_y, max_width + 4, num_rows + 2);
    }
   
    if (menu->pre_background != NULL) {
             menu->pre_background();
    }

    // 3. Dibujar los items del menú
    i = 0;
    current_row_index = 0;
    col_count = 0;
    last_y = -1;
    while (menu[i].text != NULL) {
        if (menu[i].y != last_y) {
            if (i != 0) current_row_index++;
            last_y = menu[i].y;
            col_count = 0; // Reiniciar contador de columna para la nueva fila
        }

        // Resaltar el item seleccionado
        if (current_row_index == current_row && col_count == current_col) {
            attron(A_REVERSE);
        }

        // Dibujar texto y posible indicador de submenú
        // Asegúrate de que las coordenadas no sean negativas
        if (menu[i].y >= 0 && menu[i].x >= 0) {
            mvprintw(menu[i].y, menu[i].x, "%s", menu[i].text);
            if (menu[i].submenu != NULL) {
                // Asegura no escribir fuera de límites si el texto es muy largo
                 int text_len = strlen(menu[i].text);
                 if (menu[i].x + text_len >= 0) { // Verifica la posición x del ">"
                    mvprintw(menu[i].y, menu[i].x + text_len, " >");
                 }
            }
        }

        // Quitar resaltado para el siguiente item
        attroff(A_REVERSE);

        i++;
        col_count++;
    }
    // refresh() se llamará después de esta función en dibujar_menu
}


// --- Función principal del menú ---
int dibujar_menu(MenuItem *menu, int selected_row, int selected_col, int level) {
    int i = 0;
    int ch;
    int current_row = selected_row;
    int current_col = selected_col;
    int max_width = 0;
    int num_rows = 0;
    int *num_cols_per_row = NULL;
    // menu_height ya no es necesaria, usaremos num_rows

    if (menu == NULL) {
        return -1; // Error: menú inválido
    }

    // --- Pre-cálculo de dimensiones (sin cambios) ---
    while (menu[i].text != NULL) {
        int text_width = strlen(menu[i].text);
        if (menu[i].submenu != NULL) {
            text_width += 2; // Space for " >"
        }
        if (text_width > max_width) {
            max_width = text_width;
        }
        i++;
    }

    int last_y = -1;
    for (int k = 0; menu[k].text != NULL; k++) {
        if (menu[k].y != last_y) {
            num_rows++;
            last_y = menu[k].y;
        }
    }

    num_cols_per_row = (int *)malloc(num_rows * sizeof(int));
    if (num_cols_per_row == NULL) {
        perror("Failed to allocate memory for num_cols_per_row");
        return -1; // Error de memoria
    }
    // Inicializar a cero
    for (int j = 0; j < num_rows; j++) {
        num_cols_per_row[j] = 0;
    }

    i = 0;
    int current_row_index = 0;
    last_y = -1;
    while (menu[i].text != NULL) {
        if (menu[i].y != last_y) {
            if (i != 0) current_row_index++;
            last_y = menu[i].y;
        }
        // Asegurarse de no escribir fuera de los límites del array
        if (current_row_index < num_rows) {
             num_cols_per_row[current_row_index]++;
        } else {
            // Esto indicaría un problema en la lógica de cálculo de num_rows
            // o en la estructura del menú (items con 'y' fuera de secuencia)
            // Considera añadir un mensaje de error o manejarlo
        }
        i++;
    }
    // menu_height = num_rows; // Ya no se necesita esta variable separada

    // --- Ya NO está la definición de redibujar_completo aquí ---


    // --- Dibujo inicial ---
    if (level == 0) { // Solo para el menú principal (nivel 0)
        clear(); // Limpia toda la pantalla
        if (menu->pre_background != NULL) {
            menu->pre_background(); // Dibuja el fondo principal ANTES de los items
        }
    }
    // Llama a la función auxiliar externa para dibujar los items (y el fondo del submenú si aplica)
    redibujar_menu_items(menu, current_row, current_col, max_width, num_rows, num_cols_per_row);
    refresh(); // Actualiza la pantalla después del dibujo inicial

    // --- Bucle de manejo de entrada ---
    while (1) {
        ch = getch();
        bool needs_redraw = false; // Para optimizar redibujos si no cambia la selección

        switch (ch) {
            case KEY_UP:
                if (num_rows > 0) { // Evitar división por cero si no hay filas
                    int prev_row = current_row;
                    current_row = (current_row - 1 + num_rows) % num_rows;
                    // Ajustar columna si la nueva fila tiene menos columnas
                    if (current_col >= num_cols_per_row[current_row]) {
                        current_col = num_cols_per_row[current_row] - 1;
                    }
                    if (prev_row != current_row) needs_redraw = true;
                }
                break;
            case KEY_DOWN:
                 if (num_rows > 0) {
                    int prev_row = current_row;
                    current_row = (current_row + 1) % num_rows;
                    if (current_col >= num_cols_per_row[current_row]) {
                        current_col = num_cols_per_row[current_row] - 1;
                    }
                     if (prev_row != current_row) needs_redraw = true;
                 }
                break;
            case KEY_RIGHT:
                 if (num_rows > 0 && num_cols_per_row[current_row] > 0) { // Evitar mod 0
                    int prev_col = current_col;
                    current_col = (current_col + 1) % num_cols_per_row[current_row];
                    if (prev_col != current_col) needs_redraw = true;
                 }
                break;
            case KEY_LEFT:
                 if (num_rows > 0 && num_cols_per_row[current_row] > 0) {
                    int prev_col = current_col;
                    current_col = (current_col - 1 + num_cols_per_row[current_row]) % num_cols_per_row[current_row];
                    if (prev_col != current_col) needs_redraw = true;
                 }
                break;
            case '\n': // Enter
            case '\r': // Enter en algunas terminales
            {
                int item_index = 0;
                int row_count = 0;
                // Calcular el índice lineal del item seleccionado
                while (row_count < current_row && row_count < num_rows) {
                    item_index += num_cols_per_row[row_count];
                    row_count++;
                }
                item_index += current_col;

                // Verificar que el índice calculado es válido
                // Contar el número total de items para seguridad
                int total_items = 0;
                while(menu[total_items].text != NULL) total_items++;

                if (item_index < total_items) { // Asegurarse que el índice es válido
                    MenuItem *selected_item = &menu[item_index];

                    if (selected_item->action != NULL) {
                        selected_item->action(); // Ejecutar acción
                        // Comprobar si la acción fue salir del submenú
                        if (strcmp(selected_item->text, "Retroceder") == 0 || strcmp(selected_item->text, "Menu anterior") == 0)
                        {
                            free(num_cols_per_row);
                            // No limpiar ni redibujar aquí, eso lo hará el nivel superior
                            return 0; // Código para indicar retroceso normal
                        }
                        // Si no fue retroceder, necesitamos redibujar el menú actual
                        needs_redraw = true;

                    } else if (selected_item->submenu != NULL) {
                        // Entrar al submenú recursivamente
                        int sub_result = dibujar_menu(selected_item->submenu, 0, 0, level + 1);
                        // Después de salir del submenú (sea por ESC o "Retroceder"),
                        // necesitamos redibujar el menú actual (padre).
                        if (sub_result == -2 && level == 0) {
                            // Si se presionó ESC en el submenú y estamos en el nivel principal, salir
                             free(num_cols_per_row);
                             return -2; // Propagar salida de la aplicación
                        }
                         needs_redraw = true; // Siempre redibujar al volver de un submenú
                    } else {
                        // Item sin acción ni submenú, no hacer nada o dar feedback?
                        beep(); // Podría ser útil indicar que no hace nada
                    }
                } else {
                     // Índice inválido, probablemente un error lógico anterior
                     beep();
                }

                // --- Redibujar DESPUÉS de acción o volver de submenú ---
                if (needs_redraw) {
                     if (level == 0) { // Menú principal
                        clear(); // Limpiar todo
                        if (menu->pre_background != NULL) {
                             menu->pre_background(); // Redibujar fondo principal
                        }
                        redibujar_menu_items(menu, current_row, current_col, max_width, num_rows, num_cols_per_row);
                     } else { // Submenú
                        // Solo redibuja los items (y su fondo específico si lo tiene)
                        // sin llamar a clear() para no borrar el menú padre
                        redibujar_menu_items(menu, current_row, current_col, max_width, num_rows, num_cols_per_row);
                     }
                    refresh(); // Actualizar pantalla
                    needs_redraw = false; // Ya se redibujó
                }

            } // Fin bloque case Enter
            break; // ¡Importante! Faltaba este break

            case 27: // ESC
                free(num_cols_per_row);
                if (level > 0) {
                    return 0; // Salir del submenú (código 0 para indicar salida normal por ESC)
                              // No redibujar aquí, el nivel superior lo hará.
                } else {
                    return -2; // Salir de la aplicación desde el menú principal
                }
                // No se necesita break después de return

            default:
                // Ignorar otras teclas o hacer beep()
                // beep();
                break; // Añadido por coherencia

        } // Fin switch(ch)

         // --- Redibujar si cambió la selección (movimiento) ---
        if (needs_redraw) {
            // Llama a la función auxiliar externa para redibujar solo los items
            redibujar_menu_items(menu, current_row, current_col, max_width, num_rows, num_cols_per_row);
            refresh(); // Actualiza la pantalla
        }

    } // Fin while(1)

    // Código inalcanzable debido al bucle infinito y los returns, pero por completitud:
    free(num_cols_per_row);
    return 0;
}

int yesNoMenu(char *yes, int yesX, int yesY, char* no, int noX, int noY)
{
  int selection = 1;
  int ch = 0;
  while(ch != '\n' && ch != '\r')
  {
    //Drawing loop
    for(int i = 0; i < 2; i++) 
    {
      if(selection == 1)
      {
        attron(A_REVERSE);
        mvprintw(yesX, yesY, "%s", yes);
        attroff(A_REVERSE);
        mvprintw(noX, noY, "%s", no);
      }
      else
      {
        mvprintw(yesX, yesY, "%s", yes);
        attron(A_REVERSE);
        mvprintw(noX, noY, "%s", no);
        attroff(A_REVERSE);
      }
    }
    ch = getch();
    switch(ch)
    {
      case KEY_UP:
        break;
      case KEY_DOWN:
        break;
      case KEY_LEFT:
        if(selection == 1) selection = 0;
        else if(selection == 0) selection = 1;
        break;
      case KEY_RIGHT:
        if(selection == 1) selection = 0;
        else if(selection == 0) selection = 1;
        break;
      case '\n':
        return selection;
        break;
    }
  }
  return selection;
}
