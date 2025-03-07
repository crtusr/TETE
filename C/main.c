#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "fondos.h"
#include "menu.h"

int main() {

	MenuItem cliOp_menu[] = {
	{"Registrar", NULL, NULL, 		1, 21, 6, 0, 0, NULL , operaciones},
        {"Consultas", NULL, NULL, 		2, 21, 22, 0, 1, NULL , NULL},
	{"Menu anterior", NULL, menprov, 	3, 21, 37, 0, 2, NULL , NULL},
	{"Modificaciones", NULL, NULL, 		4, 21, 52, 0, 3, NULL , NULL},
        {NULL, NULL, NULL, 			0, 0, 0, 0, 0, NULL , NULL}
	};
	
	MenuItem ProvOp_menu[] = {
	{"Altas", NULL, NULL, 			1, 21, 6, 0, 0, NULL , operaciones},
        {"Consultas", NULL, NULL, 		2, 21, 22, 0, 1, NULL , NULL},
	{"Menu anterior", NULL, menprov, 	3, 21, 37, 0, 2, NULL , NULL},
	{"Modificaciones", NULL, NULL, 		4, 21, 52, 0, 3, NULL , NULL},
        {NULL, NULL, NULL, 			0, 0, 0, 0, 0, NULL , NULL}
	};
	
	MenuItem Compras_menu[] = {
        {"Lista de proveedores", NULL, NULL, 	1, 2, 27, 0, 0, NULL , menprov},
        {"Registro de compras", ProvOp_menu, NULL, 	2, 4, 27, 1, 0, NULL , NULL},
	{"Balance de compras", NULL, NULL, 	3, 6, 29, 2, 0, NULL , NULL},
	{"Menu anterior", NULL, menu_principal, 		4, 8, 30, 3, 0, NULL , NULL},
        {NULL, NULL, NULL, 			0, 0, 0, 0, 0, NULL , NULL}
	};

	MenuItem Clientes_menu[] = {
        {"Altas", NULL, NULL, 			1, 4, 3, 0, 0, recuadro , NULL},
        {"Bajas", NULL, NULL, 			2, 5, 3, 1, 0, NULL , NULL},
	{"Modificaciones", NULL, NULL, 		3, 6, 3, 2, 0, NULL , NULL},
	{"Consultas", NULL, NULL, 		4, 7, 3, 3, 0, NULL , NULL},
	{"Deudores", NULL, NULL, 		5, 8, 3, 4, 0, NULL , NULL},
	{"Listado gral.", NULL, NULL, 		6, 9, 3, 5, 0, NULL , NULL},
	{"Retroceder", NULL, menu_principal, 	7, 10, 3, 6, 0, NULL , NULL},
        {NULL, NULL, NULL, 			0, 0, 0, 0, 0, NULL , NULL}
	};

	MenuItem Stock_menu[] = {
        {"Altas", NULL, NULL, 			1, 4, 14, 0, 0, recuadro , NULL},
        {"Bajas", NULL, NULL, 			2, 5, 14, 1, 0, NULL , NULL},
	{"Modificaciones", NULL, NULL, 		3, 6, 14, 2, 0, NULL , NULL},
	{"Consultas", NULL, NULL, 		4, 7, 14, 3, 0, NULL , NULL},
	{"Deudores", NULL, NULL, 		5, 8, 14, 4, 0, NULL , NULL},
	{"Listado gral.", NULL, NULL, 		6, 9, 14, 5, 0, NULL , NULL},
	{"Grupo de precios", NULL, NULL, 	7, 10, 14, 6, 0, NULL , NULL},
	{"Grupo de plantas", NULL, NULL, 	8, 11, 14, 7, 0, NULL , NULL},
	{"Grupo por prove.", NULL, NULL, 	9, 12, 14, 8, 0, NULL , NULL},
	{"Retroceder", NULL, menu_principal, 	10, 13, 14, 9, 0, NULL , NULL},
        {NULL, NULL, NULL, 			0, 0, 0, 0, 0, NULL , NULL}
	};

	MenuItem Main_menu[] = {
        {"Clientes", Clientes_menu, NULL, 	0, 3, 2, 0 ,0 , NULL, menu_principal},
        {"Stock", Stock_menu, NULL, 		1, 3, 14, 0 ,1 , NULL, NULL},
        {"Compras", Compras_menu, NULL, 	2, 3, 25,0 ,2 , NULL, NULL},
	{"Ctas.ctes.", NULL, NULL, 		3, 5, 2, 1 ,0 , NULL, operaciones},
	{"Parametros", NULL, NULL, 		4, 5, 14, 1 ,1 , NULL, NULL},
	{"Cheques", NULL, NULL, 		5, 5, 25, 1 ,2 , NULL, NULL},
	{"DOS", NULL, NULL, 			6, 5, 35, 1 ,3 , NULL, NULL},
        {NULL, NULL, NULL, 			0, 0, 0, 0 ,0 , NULL , NULL}
	};

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	p_inicio();

	getch();

	menu_principal();

	dibujar_menu(Main_menu ,0 ,0 ,0);

	refresh();
	endwin();
	
};