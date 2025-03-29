#include <stdio.h>
#include <stdlib.h>
#include "dbftool.h"
#include <string.h>

#define FIELD_SIZE 32L
#define FIELD_NAME 10L
#define FIELD_TYPE 1L
#define RESERVED_BYTES 4L
#define FIELD_LENGTH 1L
#define DECIMAL_COUNT 1L

int main() {

FILE *stockptr;
unsigned char buffer[255];
int cant_campos = 0;

memset(buffer, 0, 255);




stockptr = fopen("STOCK1.dbf", "rb");

if (stockptr == NULL) {
	perror("error abriendo STOCK1.dbf");

	return 1;
	}




printf("STOCK1.dbf abierto correctamente\n\n");

fseek(stockptr, FIELD_SIZE, SEEK_SET);

int read = fread(buffer, 1, FIELD_NAME, stockptr);

while (buffer[0] != 0x0d) {

	printf("%s\n", buffer);

	fseek(stockptr, FIELD_SIZE - FIELD_NAME, SEEK_CUR);

	read = fread(buffer, 1, FIELD_NAME, stockptr);

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