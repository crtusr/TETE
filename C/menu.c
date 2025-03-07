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
int dibujar_menu(MenuItem *menu, int selected_row, int selected_col, int level) {
    int i = 0;
    int ch;
    int current_row = selected_row;
    int current_col = selected_col;
    int max_width = 0;
    int num_rows = 0;
    int *num_cols_per_row = NULL;
    int menu_height;

    if (menu == NULL) {
        return -1;
    }

    // --- Pre-cálculo de dimensiones ---
    while (menu[i].text != NULL) {
        int text_width = strlen(menu[i].text);
        if (menu[i].submenu != NULL) {
            text_width += 2; // Space for ">"
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
        return -1;
    }
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
        num_cols_per_row[current_row_index]++;
        i++;
    }
    menu_height = num_rows; // Example height, adjust as needed

    // --- Function to redraw the entire menu ---
    void redibujar_completo() {
        int start_x = menu->x - 2;
        int start_y = menu->y - 1;

        // Clear *only* the menu area, not the entire screen
        for (int r = 0; r < menu_height + 2; r++) {
            mvprintw(start_y + r, start_x, "%*s", max_width + 4, "");
        }

        // Draw the background

        if (menu->draw_background != NULL) {
             menu->draw_background(start_x, start_y, max_width + 4, menu_height + 2);
        }

	if (menu->pre_background != NULL) {
             menu->pre_background();
        }


        i = 0;
        current_row_index = 0;
        int col_count = 0;
        last_y = -1;
        while (menu[i].text != NULL) {
            if (menu[i].y != last_y) {
                if (i != 0) current_row_index++;
                last_y = menu[i].y;
                col_count = 0;
            }
            if (current_row_index == current_row && col_count == current_col) {
                attron(A_REVERSE);
            }
            mvprintw(menu[i].y, menu[i].x, "%s", menu[i].text);
            if (menu[i].submenu != NULL) {
                mvprintw(menu[i].y, menu[i].x + strlen(menu[i].text), " >");
            }
            attroff(A_REVERSE);
            i++;
            col_count++;
        }
        refresh();
    }

    // --- Initial drawing ---
     // Draw pre_background if it exists (for the main menu)
    if (level == 0 && menu->pre_background != NULL) {
        clear();
        menu->pre_background();  // Draw main menu background
    }
    redibujar_completo(); // Draw the menu items (and submenu background if present)

    // --- Input handling loop ---
    while (1) {
        ch = getch();

        switch (ch) {
            case KEY_UP:
                current_row = (current_row - 1 + num_rows) % num_rows;
                if (current_col >= num_cols_per_row[current_row]) {
                    current_col = num_cols_per_row[current_row] - 1;
                }
                break;
            case KEY_DOWN:
                current_row = (current_row + 1) % num_rows;
                if (current_col >= num_cols_per_row[current_row]) {
                    current_col = num_cols_per_row[current_row] - 1;
                }
                break;
            case KEY_RIGHT:
                current_col = (current_col + 1) % num_cols_per_row[current_row];
                break;
            case KEY_LEFT:
                current_col = (current_col - 1 + num_cols_per_row[current_row]) % num_cols_per_row[current_row];
                break;
            case '\n':
            case '\r':
            {
                int item_index = 0;
                int row_count = 0;
                while (row_count < current_row) {
                    item_index += num_cols_per_row[row_count];
                    row_count++;
                }
                item_index += current_col;

                if (menu[item_index].action != NULL) {
                    menu[item_index].action();
                    if (strcmp(menu[item_index].text, "Retroceder") == 0 || strcmp(menu[item_index].text, "Menu anterior") == 0)
                    {
                         free(num_cols_per_row);
                        return -1;
                    }
                } else if (menu[item_index].submenu != NULL) {
                    dibujar_menu(menu[item_index].submenu, 0, 0, level + 1); // Enter submenu
                }
               // Redraw *after* action or submenu
                if(level > 0) //Submenu
                {
                    redibujar_completo(); // Redraw the current menu.
                }
                else //Main menu
                {
                    clear();
                    menu->pre_background(); // Redraw the main menu background.
                    redibujar_completo(); // Redraw on top.

                }
            }
            break;

            case 27: // ESC
                free(num_cols_per_row);
                if (level > 0) {
                    return -1; // Exit submenu, don't redraw here
                } else {
                    return -2; // Exit application
                }
                break;

        }
         // Redraw (only selection changes)
         redibujar_completo(); //Usar la funcion redibujar_completo
    }
    free(num_cols_per_row);
    return 0; // Should never reach here
}