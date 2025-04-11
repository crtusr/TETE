#include <stdio.h>
#include <stdlib.h>
#include "dbftool.h"
#include <string.h>
#include <inttypes.h>
#include <errno.h>


/*
I will test the functions here

Probare las funciones aquí
*/

int main() {

header header[5]; //5 is an arbitrary value 

descriptor descriptor[MAX_DBF_FIELDS];

FILE *stockptr;
unsigned char buffer[MAX_FIELD_LENGTH];
int cant_campos = 0;

memset(buffer, 0, 254);

printf("  %d  ", sizeof(header));

stockptr = fopen("STOCK1.dbf", "rb");

if (stockptr == NULL) {
	perror("error abriendo STOCK1.dbf");

	return 1;
	}


	store_header_data(header, stockptr, 0);

	store_descriptor_data(descriptor, stockptr);

	get_data(buffer , get_index("NOMBRE", "ABUTI", stockptr, header, descriptor), "PRECIOB", stockptr, header, 

descriptor);	

printf(" esta en indice %d\ndato obtenido: %s", get_index("NOMBRE", "ABUTI", stockptr, header, descriptor), buffer);

//printf("%x %x %x %x %d %d %d", header[0].dbfversion, header[0].date[0], header[0].date[1], header[0].date[2], header

[0].nofrecords, header[0].header_bytes, header[0].record_bytes);


printf("STOCK1.dbf abierto correctamente\n\n");

 int i = 0;

while (descriptor[i].fieldname[0] != 0X0d) {
	

	printf("%s\n", descriptor[i].fieldname);

	
	i++;
	cant_campos++;
	
}

printf("%d campos encontrados", cant_campos);






if (fclose(stockptr) == 0) {
	
	printf("\n\nSTOCK1.dbf Cerrado correctamente");
	
	} else {
	
	perror("error al cerrar el archivo");
	
	}

return 0;

}
