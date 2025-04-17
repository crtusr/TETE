#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "fondos.h"
#include "menu.h"
#include "dbftool.h"
#include "inputfields.h"


int sto_consulta(){
	
	header stockhead[1];
	descriptor stockdescr[MAX_DBF_FIELDS];
	InputField entrada[1];
	int entradas = 1;
	FILE *stockptr = NULL;
	int indice = 0;
	char buffer[254];
	memset(buffer, 0, 254);

	int descriptor_index = 1;
	
	init_input_field(&entrada[0], "CODIGO: ", 4, false, 10, 5, STRING);

	input_fields_loop(entrada, entradas);

	stockptr = fopen("STOCK1.dbf", "rb");
	
	if (stockptr == NULL) {
		perror("error abriendo STOCK1.dbf");

		return 1;
	}

	store_header_data(stockhead, stockptr, 0);

	store_descriptor_data(stockdescr, stockptr);

	indice = get_index("CODIGO", entrada[0].input_buffer, stockptr, stockhead, stockdescr);

	while(stockdescr[descriptor_index].fieldname[0] != TERMINATOR) {

		get_data(buffer , indice, stockdescr[descriptor_index].fieldname, stockptr, stockhead, stockdescr);
	
		mvprintw(descriptor_index + 5, 10 + (6 - strlen(stockdescr[descriptor_index].fieldname)), "%s: %s", stockdescr[descriptor_index].fieldname, buffer);
		
		descriptor_index++;
	
	}

	refresh();

	free(entrada[0].prompt);

	if (fclose(stockptr) != 0) {
	
		perror("error al cerrar el archivo");
		
		return 1;
	
	}
	
	getch(); //wait for input

	return 0;

}

int consulta_operacion(){

	header ctasctes_head[1];
	descriptor ctasctes_descr[MAX_DBF_FIELDS];
	InputField entrada[1];
	int entradas = 1;
	FILE *ctasctes_ptr = NULL;
	int indice = 0;
	char buffer[254];
	memset(buffer, 0, 254);

	char espacios[6] = "      "; //???????????????????????????

	init_input_field(&entrada[0], "Operacion: ", 4, false, 5, 2, STRING);
	input_fields_loop(entrada, entradas);

	operaciones();

	refresh();

	int espacios_vacios = 6 - strlen(entrada[0].input_buffer); //NO ENTIENDO PORQUE 6 TENDRIA QUE SER 4

	if(strlen(entrada[0].input_buffer) < 6){
		
		memmove(&entrada[0].input_buffer[6 - strlen(entrada[0].input_buffer)], 
			&entrada[0].input_buffer[0], 
			strlen(entrada[0].input_buffer));

		memmove(&entrada[0].input_buffer[0], espacios, espacios_vacios);
		
	} //YESSSSSSSSSS!!!!!!! 


	ctasctes_ptr = fopen("CTASCTES.DBF", "rb");
	
	if (ctasctes_ptr == NULL) {
		perror("error abriendo CTASCTES.dbf");
		return 1;
	}

	store_header_data(ctasctes_head, ctasctes_ptr, 0);
	store_descriptor_data(ctasctes_descr, ctasctes_ptr);

	indice = get_index("OPERAC", entrada[0].input_buffer, ctasctes_ptr, ctasctes_head, ctasctes_descr);

	get_data(buffer, indice, ctasctes_descr[0].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(2, 17, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[1].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(2, 35, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[2].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(2, 60, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[3].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(4, 19, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[4].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(4, 48, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[5].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(9, 48, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[6].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(10, 48, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[7].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(17, 11, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[8].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(17, 28, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[9].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(17, 46, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[10].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(17, 66, "%s", buffer);

	

	mvprintw(0, 0, "Representacion en hexadecimal: %x %x %x %x %x %x ",
		entrada[0].input_buffer[0], 
		entrada[0].input_buffer[1], 
		entrada[0].input_buffer[2], 
		entrada[0].input_buffer[3], 
		entrada[0].input_buffer[4], 
		entrada[0].input_buffer[5]);

	if (fclose(ctactes_ptr) != 0) {
	
		perror("error al cerrar el archivo");
		
		return 1;
	}
	getch();

	return 0;
}

int consulta_compra(){

	header compras_head[1];
	descriptor compras_descr[MAX_DBF_FIELDS];
	InputField entrada[1];
	int entradas = 1;
	FILE *compras_ptr = NULL;
	int indice = 0;
	char buffer[254];
	memset(buffer, 0, 254);

	char espacios[6] = "      "; //???????????????????????????

	init_input_field(&entrada[0], "Operacion: ", 4, false, 5, 2, STRING);

	input_fields_loop(entrada, entradas);

	operaciones();

	refresh();

	int espacios_vacios = 6 - strlen(entrada[0].input_buffer); //NO ENTIENDO PORQUE 6, TENDRIA QUE SER 4

	if(strlen(entrada[0].input_buffer) < 6){

		memmove(&entrada[0].input_buffer[6 - strlen(entrada[0].input_buffer)], 
			&entrada[0].input_buffer[0], 
			strlen(entrada[0].input_buffer));

		memmove(&entrada[0].input_buffer[0], espacios, espacios_vacios);
		
	} //YESSSSSSSSSS!!!!!!! 


	compras_ptr = fopen("COMPRA.DBF", "rb");
	
	if (compras_ptr == NULL) {

		perror("error abriendo COMPRA.DBF");

		return 1;
	}

	store_header_data(compras_head, compras_ptr, 0);

	store_descriptor_data(compras_descr, compras_ptr);

	indice = get_index("OPERAC", entrada[0].input_buffer, compras_ptr, compras_head, compras_descr);

	get_data(buffer, indice, ctasctes_descr[0].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(2, 17, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[1].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(2, 35, "%c%c-%c%c-%c%c", 
		buffer[6], buffer[7], 
		buffer[4], buffer[5], 
		buffer[2], buffer[3]);

	get_data(buffer, indice, ctasctes_descr[2].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(2, 60, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[3].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(4, 19, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[4].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(4, 48, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[5].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(9, 48, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[6].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(10, 48, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[7].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(17, 11, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[8].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(17, 28, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[9].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(17, 46, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[10].fieldname, compras_ptr, compras_head, compras_descr);
	mvprintw(17, 66, "%s", buffer);

	

	mvprintw(0, 0, "Representacion en hexadecimal: %x %x %x %x %x %x ", entrada[0].input_buffer[0], 

entrada[0].input_buffer[1], entrada[0].input_buffer[2], entrada[0].input_buffer[3], entrada

[0].input_buffer[4], entrada[0].input_buffer[5]);

	if (fclose(compras_ptr) != 0) {
	
		perror("error al cerrar el archivo");
		
		return 1;
	}	
	getch();
	return 0;
}
