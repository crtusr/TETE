#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "fondos.h"
#include "menu.h"
#include "dbftool.h"
#include "inputfields.h"

//macros for dbf fields of ctasctes.dbf used for clarity

#define OPERAC ctasctes_descr[0].length
#define FECHA ctasctes_descr[1].length
#define FACTUR ctasctes_descr[2].length
#define NROCLI ctasctes_descr[3].length
#define DENOMI ctasctes_descr[4].length
#define ACREE ctasctes_descr[5].length
#define DEUDO ctasctes_descr[6].length
#define ECHEQ ctasctes_descr[7].length
#define EEFEC ctasctes_descr[8].length
#define RCHEQ ctasctes_descr[9].length
#define REFEC ctasctes_descr[10].length
#define OBSERV ctasctes_descr[11].length



void sto_consulta(){
	
	header stockhead[1];
	descriptor stockdescr[MAX_DBF_FIELDS];
	InputField entrada[1];
	int entradas = 1;
	FILE *stockptr = NULL;
	int indice = 0;
	char buffer[MAX_FIELD_LENGTH + 1];
	memset(buffer, 0, MAX_FIELD_LENGTH + 1);

	int descriptor_index = 1;
	
	init_input_field(&entrada[0], "CODIGO: ", 4, false, 10, 5, STRING);

	input_fields_loop(entrada, entradas);

	stockptr = fopen("STOCK1.dbf", "rb");
	
	if (stockptr == NULL) {
		perror("error abriendo STOCK1.dbf");

		return;
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
		
		return;
	
	}
	
	getch(); //wait for input

	return;

}

void consulta_operacion(){

	header ctasctes_head[1];
	descriptor ctasctes_descr[MAX_DBF_FIELDS];
	InputField entrada[1];
	int entradas = 1;
	FILE *ctasctes_ptr = NULL;
	int indice = 0;
	char buffer[MAX_FIELD_LENGTH + 1];
	memset(buffer, 0, MAX_FIELD_LENGTH + 1);

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

		return;
	}

	store_header_data(ctasctes_head, ctasctes_ptr, 0);

	store_descriptor_data(ctasctes_descr, ctasctes_ptr);

	indice = get_index("OPERAC", entrada[0].input_buffer, ctasctes_ptr, ctasctes_head, ctasctes_descr);

	get_data(buffer, indice, ctasctes_descr[0].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(2, 17, "%s", buffer);

	get_data(buffer, indice, ctasctes_descr[1].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(2, 35, "%c%c-%c%c-%c%c", buffer[6], buffer[7], buffer[4], buffer[5], buffer[2], buffer[3]);

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

	if (fclose(ctasctes_ptr) != 0) {
	
		perror("error al cerrar el archivo");
		
		return;
	}

	getch();

	return;
	
}

void consulta_compra(){

	

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

			return;
	
	}



	store_header_data(compras_head, compras_ptr, 0);


	store_descriptor_data(compras_descr, compras_ptr);


	indice = get_index("ORDCOM", entrada[0].input_buffer, compras_ptr, compras_head, 

compras_descr);



	get_data(buffer, indice, compras_descr[0].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(2, 17, "%s", buffer);



	get_data(buffer, indice, compras_descr[1].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(2, 35, "%c%c-%c%c-%c%c", buffer[6], buffer[7], buffer[4], buffer[5], buffer[2], buffer

[3]);



	get_data(buffer, indice, compras_descr[2].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(2, 60, "%s", buffer);



	get_data(buffer, indice, compras_descr[3].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(4, 19, "%s", buffer);



	get_data(buffer, indice, compras_descr[4].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(4, 48, "%s", buffer);



	get_data(buffer, indice, compras_descr[11].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(9, 48, "%s", buffer);



	get_data(buffer, indice, compras_descr[12].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(10, 48, "%s", buffer);



	get_data(buffer, indice, compras_descr[13].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(17, 11, "%s", buffer);



	get_data(buffer, indice, compras_descr[14].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(17, 28, "%s", buffer);



	get_data(buffer, indice, compras_descr[15].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(17, 46, "%s", buffer);



	get_data(buffer, indice, compras_descr[16].fieldname, compras_ptr, compras_head, 

compras_descr);

	mvprintw(17, 66, "%s", buffer);



	

mvprintw(0, 0, "Representacion en hexadecimal: %x %x %x %x %x %x ",
		 entrada[0].input_buffer[0],
		 

entrada[0].input_buffer[1],
		 entrada[0].input_buffer[2],
		 entrada[0].input_buffer[3],
		 entrada

[0].input_buffer[4],
		 entrada[0].input_buffer[5]);



	if (fclose(compras_ptr) != 0) {

	
		perror("error al cerrar el archivo");

		
	return;
	
	
	
	}
	getch();


	return;

}

void ultimas_op_cli(){
	header ctasctes_head[1];
	descriptor ctasctes_descr[MAX_DBF_FIELDS];
	InputField entrada[1];
	int entradas = 1;
	FILE *ctasctes_ptr = NULL;
	int indice = 0;
	char buffer[254] = {0};
	memset(buffer, 0, 254);


	float acreedor = 0;
	float deudor = 0;
	float pagueCheque = 0;
	float pagueEfectivo = 0;
	float recibiCheque = 0;
	float recibiEfectivo = 0;
	float Saldo = 0;


	char espacios[4] = "    "; //has to be equal to field size

	init_input_field(&entrada[0], "Cliente: ", 4, false, 0, 0, STRING);
	input_fields_loop(entrada, entradas);

	ctasctes_ptr = fopen("CTASCTES.DBF", "rb");

	int espacios_vacios = 4 - strlen(entrada[0].input_buffer);

	if(strlen(entrada[0].input_buffer) < 4)
	{



		memmove(&entrada[0].input_buffer[4 - strlen(entrada[0].input_buffer)],
 
			&entrada[0].input_buffer[0],
 
			strlen(entrada[0].input_buffer));



		memmove(&entrada[0].input_buffer[0], espacios, espacios_vacios);


	}
	
	if (ctasctes_ptr == NULL) 
	{

		perror("error abriendo CTASCTES.dbf");

		return;
	}

	store_header_data(ctasctes_head, ctasctes_ptr, 0);
	store_descriptor_data(ctasctes_descr, ctasctes_ptr);

	char *dbfData = (char *)malloc((ctasctes_head[0].record_bytes) * 500); // basically assuming no more than 500 records for 1 client 

	int i = 0; //originally set to 2
	
	indice = get_index(ctasctes_descr[3].fieldname, entrada[0].input_buffer, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	
	get_data(buffer, indice, ctasctes_descr[4].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	mvprintw(0, 25, "Cliente: %s", buffer);
	

	
	while(indice < ctasctes_head->nofrecords)
	{


		get_data(buffer, indice, ctasctes_descr[3].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
	
		int comparison = strncmp(buffer, entrada[0].input_buffer, 4); //strncmp didn't work inside of the if statement
		
//mvprintw(4, i, "%d %s %s %d", indice, buffer, entrada[0].input_buffer, comparison); 
//used for logging
			
refresh();
		
		if(comparison == 0)
		{
			
			memset(buffer, 0, 254);
			get_record(buffer, indice, ctasctes_ptr, ctasctes_head, ctasctes_descr);
			memcpy(&dbfData[(i) * ctasctes_head[0].record_bytes], buffer, ctasctes_head[0].record_bytes);
			
			i++;
		
		}
		indice++;
	}

		// dbfData[i * ctasctes_head[0].record_bytes + offset] where i are the rows and j are the columns
		// offset is the sum of the previous fields in bytes

		mvprintw(0, 50, "Numero de operaciones: %d", i);
		
		int scrollSize = 19;
		int begin = 0; //scroll begin
		int current = begin;
		int end = begin + scrollSize; //scroll end
		int key = 0;
		
    	while(1) 
	{
		current = begin;
		end = begin + scrollSize;
		
	
		for(int j = 4; (current) < (i) && current < end; j++) 
			{
			int offset = 1;
			int printOffset = 1;
	
			//mvprintw(j, 0, "the loop executes"); // for debugging
			
			//mvprintw(j, 0, "%s", dbfData); //for debugging
	
			mvprintw(j, 0, "%.4s", &dbfData[(current) * ctasctes_head[0].record_bytes] + offset);
			offset += OPERAC;
			printOffset += OPERAC;
			
			mvprintw(j, printOffset, "%c%c/%c%c", 	dbfData[(current) * ctasctes_head[0].record_bytes + offset + 6],
								dbfData[(current) * ctasctes_head[0].record_bytes + offset + 7],
								dbfData[(current) * ctasctes_head[0].record_bytes + offset + 4],
								dbfData[(current) * ctasctes_head[0].record_bytes + offset + 5]);
			offset += FECHA;
			printOffset += FECHA - 2;
			
			//mvprintw(j, printOffset, "%.13s", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
			offset += FACTUR;
			offset += NROCLI;	// not printed
			offset += DENOMI;	// not printed
	
			mvprintw(j, printOffset, "%.10s", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
			offset += ACREE;
			printOffset += ACREE;
	
			mvprintw(j, printOffset, "%.10s", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
			offset += DEUDO;
			printOffset += DEUDO;
	
			mvprintw(j, printOffset, "%.10s", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
			offset += ECHEQ;
			printOffset += ECHEQ;
	
			mvprintw(j, printOffset, "%.10s", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
			offset += EEFEC;
			printOffset += EEFEC;	

			mvprintw(j, printOffset, "%.10s", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
			offset += RCHEQ;
			printOffset += RCHEQ;	

			mvprintw(j, printOffset, "%.10s", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
			offset += REFEC;
			printOffset += REFEC;
			
			current++;
			}

		key = getch();
    
		if(key == KEY_UP)
		{
			if(begin > 0) 
			{
				begin--;
			}
			else
			{
				begin = 0;
			}
		}
		else if(key == KEY_DOWN)
		{
			if(end < i) 
			{
				begin++;
			}
			else
			{
				begin = i - scrollSize;
			}
		}
		
		if(key == KEY_ENTER || key == '\n')
		{
			break;
		}
	

	}
	
	refresh();
	free(dbfData);
	return;
}
