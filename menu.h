#ifndef MENU_H
#define MENU_H


typedef struct MenuItem {
    char *text;
    struct MenuItem *submenu;
    void (*action)();
    int id;
    int y;
    int x;
    int row;
    int column;
    void (*draw_background)(int, int, int, int); // Puntero a subrutina, o NULL
    void (*pre_background)();
} MenuItem;


void recuadro (int start_x, int start_y, int width, int height);

int dibujar_menu(MenuItem *menu, int selected_row, int selected_col, int level);
int yesNoMenu(char *yes, int yesX, int yesY, char* no, int noX, int noY);
int printingMenu();

#endif
