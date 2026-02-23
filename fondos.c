#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "fondos.h"

void p_inicio() {

	printw(      
        "                 ||||||||||   ||||||||||   ||||||||||   ||||||||||\n"
        "                     ||       ||     2026      ||       ||\n"
        "                     ||       ||    2 0 2 6    ||       ||\n"
        "                     ||       ||||||  2 0 2 6  ||       ||||||\n"
        "                     ||       ||  2  0  2 6    ||       ||\n"
        "                     ||       ||  2  0  2  6   ||       ||\n"
        "                     ||       ||||||||||       ||       ||||||||||\n");
	printw(
	
        "           +------------------------------------------------------------+\n"
        "           |          Sistema creado por Jose Ernesto Gasperi           |\n"
        "           |     esta prohibido su copiado para la comercializacion     |\n");
	printw(
        "           |                                                            |\n"
        "           |                     Cap. Giachino 415                      |\n"
        "           |                     1613 Ing. Pablo Nogues                 |\n"
        "           |                     Prov. Bs. As. Argentina                |\n");
	printw(
	
        "           |                                                            |\n"
        "           |        HOME PAGE:  www.cultivogasperi.com.ar               |\n"
        "           |        E-MAIL   : cultivosgasperi@yahoo.com.ar             |\n"
        "           |                                                            |\n"
        "           |                                                            |\n");
	printw(       
	      "           |                                                            |\n"
        "           +------------------------------------------------------------+\n"
        "                        Pulse alguna tecla para continuar"
	);
}

void menu_principal() {
	clear();
	
	printw(

	"+----------------------VIVERO *** menu principal  2  0  2  6------------------+\n"
	"|                                                                             |\n"
	"|                                                                             |\n"
	"| Clientes >  Stock >    Compras >                                            |\n"
	"|                                                                             |\n");
	printw(
	"| Ctas.ctes. >Parametros Cheques > DOS                                        |\n"
	"|                                  DEBUG                                      |\n"
	"|                                                                             |\n"
	"|                                                                             |\n"
	"|                                                                             |\n");
	printw(
	"|                                                                             |\n"
	"|                                                                             |\n"
	"|                                                                             |\n"
	"|                   ****** CULTIVO GASPERI hermanos SRL ******                |\n"
	"|                                                                             |\n");
	printw(
	"| (Fecha)                                                                     |\n"
	"+-----------------------------------------------------------------------------+\n"
	"          +----------------------------------------------------------+\n"
	"          |                    Stock de mercaderia                   |\n"
	"          +----------------------------------------------------------+\n");

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	mvprintw(15, 3, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900); //Se añade la fecha.

}


void menprov() {

	clear();

printw( "                     +-------------------------------+\n"
        "                     |||||||||||||||||||||||||||||||||\n"
        "                     ||    Lista de proveedores >   ||\n"
        "                     |||||||||||||||||||||||||||||||||\n");
printw( "                     ||    Registro de compras >    ||\n"
        "                     |||||||||||||||||||||||||||||||||\n"
        "                     ||      Balance de compras     ||\n"
        "                     |||||||||||||||||||||||||||||||||\n");
printw( "                     ||       Menu anterior         ||\n"
        "                     |||||||||||||||||||||||||||||||||\n"
        "                     +-------------------------------+\n"
        "\n");
printw( "\n"
        "\n"
        "\n"
        "\n");
printw( "\n"
        "                   informacion sobre proveedores y saldos\n");

}

void operaciones() {
	
	clear();

printw( "\n"
	"   +------------------------------------------------------------------------+\n"
	"   | operacion :[      ]    fecha:[        ]  factura nro.:[             ]  |\n"
	"   +------------------------------------------------------------------------|\n"
	"   |nro.cliente  :[   ]                 nombre:[                         ]  |\n");
printw( "   |direccion:[                         ] localidad:[                    ]  |\n"
	"   |cpostal:[    ]           telefono1:[         ]  telefono2:[          ]  |\n"
	"   +------------------------------------------------------------------------|\n"
	"   |DETALLE:                          |      S A L D O   P A R C I A L      |\n");
printw( "   |                                  |credito [              ]             |\n"
	"   |                                  |factura$[              ]             |\n"
	"   |                                  +-------------------------------------|\n"
	"   |                                  |      S A L D O    G E N E R A L     |\n");
printw( "   |                                  |acreedor[         ]                  |\n"
	"   |                                  |deudor  [         ]                  |\n"
	"   +----------------------------------+-------------------------------------|\n"
	"   |           VALORES ENTREGADOS     |           VALORES RECIBIDOS         |\n");
printw( "   |cheque[         ] efec.[         ]|cheque[         ] efectivo[         ]|\n"
	"   +------------------------------------------------------------------------+\n"
	"\n"
	"\n");
printw( "+------------------Registro de Cuentas Corrientes ******* menu de opciones****+\n"
	"|                                                                             |\n"
	"+-----------------------------------------------------------------------------+\n"
	"          registro de deuda en cta cte\n");

}

void consultas_op() {

	clear();

printw( "          +-----------------------------------------------------------+\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n"
	      "          ||||||||||                                              |||||\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
printw( "          ||||||||||                                   ||||||||||||||||\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n"
	      "          ||||||||||             ||||||||||||||||||||||||||||||||||||||\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
printw( "          ||||||||||                 ||||||||||||||||||||||||||||||||||\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n"
	      "          ||||||||||                                |||||||||||||||||||\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
printw( "          ||||||||||                     ||||||||||||||||||||||||||||||\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n"
	      "          ||||||||||                            |||||||||||||||||||||||\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n");
printw( "          ||||||||||                                    |||||||||||||||\n"
	      "          |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n"
	      "          +-----------------------------------------------------------+\n");

}

void draw_background_submenu(int x, int y, int width, int height) {
     for (int i = y; i < y + height; i++) {
        for (int j = x; j < x + width; j++) {
            mvaddch(i, j, ACS_CKBOARD);
        }
    }
}
