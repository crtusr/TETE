#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "fondos.h"
#include "menu.h"
#include "protete.h"

int main() 
{
  int x = 0;
	int y = 0;
  
  y = 2; 
  x = 20;
	MenuItem ConsProv_menu[] = 
  {
	  {"OPERACION INDIVIDUAL (por numero de operacion)", NULL, consulta_compra, 	1, y, x, 0, 0, NULL , consultas_op},
    {"LISTADO POR IMPRESORA (por cliente)", NULL, NULL, 			                  2, y + 2, x, 0, 1, NULL , NULL},
	  {"Menu anterior", NULL, operaciones, 					                              3, y + 4, x, 0, 2, NULL , NULL},
	  {"BALANCE DE DEUDAS", NULL, NULL, 					                                4, y + 6, x, 0, 3, NULL , NULL},
	  {"RESUMEN DE FACTURACION MES A MES", NULL, NULL, 			                      5, y + 8, x, 0, 4, NULL , NULL},
	  {"OPERACIONES POR FECHA", NULL, NULL, 					                            6, y + 10, x, 0, 5, NULL , NULL},
	  {"OPERACIONES ENTRE DOS FECHAS", NULL, NULL, 				                        7, y + 12, x, 0, 6, NULL , NULL},
	  {"ULTIMAS 23 OPERACIONES (por poveedor)", NULL, ultimas_op_com, 	          8, y + 14, x, 0, 7, NULL , NULL},
    {NULL, NULL, NULL, 			0, 0, 0, 0, 0, NULL , NULL}
	};

	MenuItem ConsCli_menu[] = {
	  {"OPERACION INDIVIDUAL (por numero de operacion)", NULL, consulta_operacion,  1, y, x, 0, 0, NULL , consultas_op},
    {"LISTADO POR IMPRESORA (por cliente)", NULL, NULL, 			                    2, y + 2, x, 0, 1, NULL , NULL},
	  {"Menu anterior", NULL, operaciones, 					                                3, y + 4, x, 0, 2, NULL , NULL},
	  {"BALANCE DE DEUDAS", NULL, NULL, 					                                  4, y + 6, x, 0, 3, NULL , NULL},
	  {"RESUMEN DE FACTURACION MES A MES", NULL, NULL, 			                        5, y + 8, x, 0, 4, NULL , NULL},
	  {"OPERACIONES POR FECHA", NULL, NULL, 					                              6, y + 10, x, 0, 5, NULL , NULL},
	  {"OPERACIONES ENTRE DOS FECHAS", NULL, NULL, 				                          7, y + 12, x, 0, 6, NULL , NULL},
	  {"ULTIMAS 23 OPERACIONES (por cliente)", NULL, ultimas_op_cli, 		            8, y + 14, x, 0, 7, NULL , NULL},
    {NULL, NULL, NULL, 							0, 0, 0, 0, 0, NULL , NULL}
	};

	MenuItem CliOp_menu[] = {
    {"Registrar", NULL, agregarCtacte, 		  1, 22, 6, 0, 0, NULL , operaciones},
    {"Consultas", ConsCli_menu, NULL, 	    2, 22, 22, 0, 1, NULL , NULL},
    {"Menu anterior", NULL, menprov, 	      3, 22, 37, 0, 2, NULL , NULL},
    {"Modificaciones", NULL, modCtacte, 		4, 22, 52, 0, 3, NULL , NULL},
    {NULL, NULL, NULL, 			0, 0, 0, 0, 0, NULL , NULL}
	};
	
	MenuItem ProvOp_menu[] = {
    {"Altas", NULL, agregarOrdCom, 			1, 22, 6, 0, 0, NULL , operaciones},
    {"Consultas", ConsProv_menu, NULL, 	2, 22, 22, 0, 1, NULL , NULL},
    {"Menu anterior", NULL, menprov, 	  3, 22, 37, 0, 2, NULL , NULL},
    {"Modificaciones", NULL, modCom, 		4, 22, 52, 0, 3, NULL , NULL},
    {NULL, NULL, NULL, 			0, 0, 0, 0, 0, NULL , NULL}
	};
	
  y = 5;
  
  MenuItem ListaProv_menu[] = 
  {
    {"Altas", NULL, agregarPro, 			    1, y, 40, 0, 0, recuadro , NULL},
    {"Bajas", NULL, NULL, 			          2, y+1, 40, 1, 0, NULL , NULL},
	  {"Modificaciones", NULL, modPro, 		  3, y+2, 40, 2, 0, NULL , NULL},
	  {"Consultas", NULL,  consPro, 		    4, y+3, 40, 3, 0, NULL , NULL},
    {"Listado de acreedores", NULL, NULL, 5, y+4, 40, 4, 0, NULL , NULL},
	  {"Retroceder", NULL, menu_principal, 	6, y+5, 40, 6, 0, NULL , NULL},
    {NULL, NULL, NULL, 			              0, 0, 0, 0, 0, NULL , NULL}
	};
  
	MenuItem Compras_menu[] = {
    {"Lista de proveedores", ListaProv_menu, NULL,  1, 2, 27, 0, 0, NULL , menprov},
    {"Registro de compras", ProvOp_menu, NULL,      2, 4, 27, 1, 0, NULL , NULL},
    {"Balance de compras", NULL, NULL,              3, 6, 29, 2, 0, NULL , NULL},
    {"Menu anterior", NULL, menu_principal,         4, 8, 30, 3, 0, NULL , NULL},
    {NULL, NULL, NULL,                              0, 0, 0, 0, 0, NULL , NULL}
	};

	MenuItem Clientes_menu[] = 
  {
    {"Altas", NULL, agregarCli, 			          1, 4, 3, 0, 0, recuadro , NULL},
    {"Bajas", NULL, NULL, 			          2, 5, 3, 1, 0, NULL , NULL},
	  {"Modificaciones", NULL, modCli, 		    3, 6, 3, 2, 0, NULL , NULL},
	  {"Consultas", NULL,  consCli, 		    4, 7, 3, 3, 0, NULL , NULL},
	  {"Deudores", NULL, NULL, 		          5, 8, 3, 4, 0, NULL , NULL},
	  {"Listado gral.", NULL, NULL, 		    6, 9, 3, 5, 0, NULL , NULL},
	  {"Retroceder", NULL, menu_principal, 	7, 10, 3, 6, 0, NULL , NULL},
    {NULL, NULL, NULL, 			              0, 0, 0, 0, 0, NULL , NULL}
	};

	MenuItem Stock_menu[] = {
    {"Altas", NULL, agregarStock, 			    1, 4, 14, 0, 0, recuadro , NULL},
    {"Bajas", NULL, NULL, 			            2, 5, 14, 1, 0, NULL , NULL},
    {"Modificaciones", NULL, modStock, 		  3, 6, 14, 2, 0, NULL , NULL},
    {"Consultas", NULL, sto_consulta, 		  4, 7, 14, 3, 0, NULL , NULL},
    {"Listado gral.", NULL, NULL, 		      5, 8, 14, 4, 0, NULL , NULL},
    {"Grupo de precios", NULL, NULL, 		    6, 9, 14, 5, 0, NULL , NULL},
    {"Grupo de plantas", NULL, NULL, 	      7, 10, 14, 6, 0, NULL , NULL},
    {"Grupo por prove.", NULL, grupoPorProv, 	      8, 11, 14, 7, 0, NULL , NULL},
    {"Ingreso de mercaderia", NULL, NULL, 	9, 12, 14, 8, 0, NULL , NULL},
    {"Retroceder", NULL, menu_principal, 	  10, 13, 14, 9, 0, NULL , NULL},
    {NULL, NULL, NULL, 			                0, 0, 0, 0, 0, NULL , NULL}
	};
  
  y = 7;
  x = 25;
  
	MenuItem Cheques_menu[] = 
  {
    {"Altas", NULL, agregarCheque, 			          1, y, x, 0, 0, recuadro , NULL},
    {"Bajas", NULL, NULL, 			          2, y+1, x, 1, 0, NULL , NULL},
	  {"Modificaciones", NULL, modCheque, 		    3, y+2, x, 2, 0, NULL , NULL},
	  {"Consultas", NULL,  consCheque, 		        4, y+3, x, 3, 0, NULL , NULL},
	  {"Listado", NULL, NULL, 		          5, y+4, x, 4, 0, NULL , NULL},
	  {"Pantalla", NULL, chequesNoEntregados, 		          6, y+5, x, 5, 0, NULL , NULL},
	  {"Retroceder", NULL, menu_principal, 	7, y+6, x, 6, 0, NULL , NULL},
    {"Cheques en fecha", NULL, NULL, 		  8, y+7, x, 7, 0, NULL , NULL},
    {NULL, NULL, NULL, 			              0, 0, 0, 0, 0, NULL , NULL}
	};

	MenuItem Main_menu[] = 
  {
    {"Clientes", Clientes_menu, NULL, 0, 3, 2, 0 ,0 , NULL, menu_principal},
    {"Stock", Stock_menu, NULL, 		  1, 3, 14, 0 ,1 , NULL, NULL},
    {"Compras", Compras_menu, NULL, 	2, 3, 25,0 ,2 , NULL, NULL},
	  {"Ctas.ctes.", CliOp_menu, NULL, 	3, 5, 2, 1 ,0 , NULL, operaciones},
	  {"Parametros", NULL, NULL, 		    4, 5, 14, 1 ,1 , NULL, NULL},
	  {"Cheques", Cheques_menu, NULL, 	5, 5, 25, 1 ,2 , NULL, NULL},
	  {"DOS", NULL, NULL, 			        6, 5, 35, 1 ,3 , NULL, NULL},
	  {"DEBUG", NULL,  funcTest,			7, 6, 35, 1 ,4 , NULL, NULL},
    {NULL, NULL, NULL, 			          0, 0, 0, 0 ,0 , NULL , NULL}
	};

	initscr();
	//resize_term(30, 100);
	//is_termresized();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	p_inicio();

	getch();

	menu_principal();

	dibujar_menu(Main_menu ,0 ,0 ,0);

	refresh();
	endwin();
	
}
