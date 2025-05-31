#include <stdio.h>
#include <stdlib.h>
#include "dbftool.h"
#include <string.h>
#include <inttypes.h>
#include <errno.h>

#define FIELD_SIZE 32L
#define FIELD_NAME 11L
#define FIELD_TYPE 1L
#define RESERVED_BYTES 4L
#define FIELD_LENGTH 1L
#define DECIMAL_COUNT 1L
#define WORK_AREA 2L
#define EXAMPLE 1L
#define RESERVED_BYTES2 10L
#define INDEX_FLAG 1L
#define MAX_DBF_FIELDS 255

#define DBF_VER 1L
#define DATE 3L
#define N_OF_RECORDS 1L
#define HEADER_BYTES 4L
#define RECORD_BYTES 1L
#define H_RES_1 2L
#define INC_TRANS 1L
#define ENCR_FLAG 1L
#define H_RES_2 12L
#define LANG_DRIV_ID 1L
#define H_RES_3 2L
#define TERMINATOR 0x0D
#define MAX_FIELD_LENGTH 254


/* This function is for parsing the header of the .dbf and loading it into a struct (header). 
I'm using trial and error to load the correct bytes for every field.

Esta funcion es para leer el encaezado del archivo .dbf y cargarlo en una estructura de datos (header).
Estoy haciendo por prueba y error para cargar los bytes correctos en cada campo.
*/

void store_header_data(header *head, FILE *file, int i) 
{


	unsigned char buffer[FIELD_SIZE];
	memset(buffer, 0, FIELD_SIZE);

	fseek(file, 0, SEEK_SET);

	//fread(variable donde vas a guardar, tamaño del dato que vas a leer, cantidad de datos que vas a leer, puntero al archivo)

	fread(buffer, 1, sizeof(head->dbfversion), file); //this should be interpreted as 8 1bit flags
	memcpy((&head[i].dbfversion), buffer, sizeof(head->dbfversion));

	fread(buffer, 1, sizeof(head->date), file); //last modified
	memcpy((&head[i].date), buffer, sizeof(head->date));

	fread(buffer, sizeof(head->nofrecords), 1, file); //number of records in the file
	memcpy((&head[i].nofrecords), buffer, sizeof(head->nofrecords)); 

	fread(buffer, sizeof(head->header_bytes), 1, file); //number of bytes in the header
	memcpy((&head[i].header_bytes), buffer, sizeof(head->header_bytes)); 

	fread(buffer, sizeof(head->record_bytes), 1, file); //number of bytes in each record
	memcpy((&head[i].record_bytes), buffer, sizeof(head->record_bytes));

	fread(buffer, sizeof(head->res1), 1, file); //2 reserved bytes
	memcpy((&head[i].res1), buffer, sizeof(head->res1));

	fread(buffer, sizeof(head->inctransaction), 1, file); //flag for an incomplete transaction
	memcpy((&head[i].inctransaction), buffer, sizeof(head->inctransaction));

	fread(buffer, sizeof(head->encryption), 1, file); //entription flag
	memcpy((&head[i].encryption), buffer, sizeof(head->encryption));

	fread(buffer, sizeof(head->res2), 1, file); //12 reserved bytes
	memcpy((&head[i].res2), buffer, sizeof(head->res2));

	fread(buffer, sizeof(head->mdxflag), 1, file); // ???
	memcpy((&head[i].mdxflag), buffer, sizeof(head->mdxflag));

	fread(buffer, sizeof(head->langdrivID), 1, file); // language driver ID
	memcpy((&head[i].langdrivID), buffer, sizeof(head->langdrivID));

	fread(buffer, sizeof(head->res3), 1, file); //2 reserved bytes
	memcpy((&head[i].res3), buffer, sizeof(head->res3));

}

/* This function is for parsing the field descriptors of the .dbf and loading it into a struct (descriptor). 
I'm using trial and error to load the correct bytes for every field.

Esta funcion es para leer el descriptor de campos del .dbf y cargarlo en una estructura de datos (descriptor).
Estoy haciendo por prueba y error para cargar los bytes correctos en cada campo.
*/

void store_descriptor_data(descriptor *fields, FILE *file) 
{

	int index = 0;

	unsigned char buffer[FIELD_SIZE];

	fseek(file, FIELD_SIZE, SEEK_SET);

	//buffer[0] != 0x0d

	while (index < MAX_DBF_FIELDS) 
	{
		memset(buffer, 0, FIELD_SIZE);
	


		fread(buffer, sizeof(fields->fieldname), 1, file);
		memcpy((&fields[index].fieldname), buffer, sizeof(fields->fieldname));

		if (buffer[0] == TERMINATOR) 
		{
			break;
		}

		// ENCONTRAR 0x0d!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


		fread(buffer, sizeof(fields->type), 1, file);
		memcpy((&fields[index].type), buffer, sizeof(fields->type));
	
		fread(buffer, sizeof(fields->res1), 1, file);
		memcpy((&fields[index].res1), buffer, sizeof(fields->res1));
	
		fread(buffer, sizeof(fields->length), 1, file);
		memcpy((&fields[index].length), buffer, sizeof(fields->length));

		fread(buffer, sizeof(fields->decimal), 1, file);
		memcpy((&fields[index].decimal), buffer, sizeof(fields->decimal));

		fread(buffer, sizeof(fields->workID), 1, file);
		memcpy((&fields[index].workID), buffer, sizeof(fields->workID));

		fread(buffer, sizeof(fields->example), 1, file);
		memcpy((&fields[index].example), buffer, sizeof(fields->example));

		fread(buffer, sizeof(fields->res2), 1, file);
		memcpy((&fields[index].res2), buffer, sizeof(fields->res2));

		fread(buffer, sizeof(fields->index_flag), 1, file);
		memcpy((&fields[index].index_flag), buffer, sizeof(fields->index_flag));

		index++;
	
	}

}

//search_index(campo en donde buscar, contenido que buscar, archivo que se abre, puntero al encabezado (es un struct), puntero al descriptor (struct))

int get_index(char* campo, char* string, FILE* file, header* head, descriptor* descr)
{
	//busco el indice del campo

	if (strlen(string) > MAX_FIELD_LENGTH) 
	{

		perror("used more than 254 characters...");

		return -1;
		
	}

	int i = 0;
	int j = 0;
	int offset = 0;
	int rindex = 0;
	char buffer[MAX_FIELD_LENGTH];
	memset(buffer, 0, 254);
	
	fseek(file, FIELD_SIZE, SEEK_SET);
	while(strncmp(descr[i].fieldname,campo,strlen(campo))) 
		{
		i++;
		}


	//pongo el cursor en el lugar donde compienzan los registros

	fseek(file, head[0].header_bytes, SEEK_SET);
	
	//offseteo el puntero al valor decriptor[0].length + ... + descriptor[indice].length

	while(j < i) 
		{
		offset += descr[j].length; 
		j++;
		}

	fseek(file, offset + 1, SEEK_CUR);

	//loopeo 

	//leo el valor del campo y comparo con el string que quiero buscar

	while(strncmp(string, buffer, strlen(string)) != 0 && rindex < head[0].nofrecords) 
	{

		fread(buffer, descr[i].length, 1, file);

		//avanzo en header[n].record_bytes (tamaño del registro) - (descriptor[indice del campo].length)

		fseek(file, head[0].record_bytes - (descr[i].length) , SEEK_CUR);

		rindex++;
	
	}
	
	return rindex - 1;
		
}

int get_data(char* buffer, int indice, char* campo, FILE* file, header* head, descriptor* descr)
{
	
	if (indice == -1) 
	{
		perror("indice invalido");
		return -1;
	}
	
	//busco el indice del campo

	int i = 0;
	int j = 0;
	int offset = 0;
	fseek(file, FIELD_SIZE, SEEK_SET);
	while(strncmp(descr[i].fieldname,campo,strlen(campo))) 
		{
		i++;
		}


	//pongo el cursor en el lugar donde compienzan los registros

	fseek(file, head[0].header_bytes, SEEK_SET);
	
	//offseteo el puntero al valor decriptor[0].length + ... + descriptor[indice].length

	while(j < i) 
		{
		offset += descr[j].length; 
		j++;
		}

	fseek(file, offset + 1, SEEK_CUR);

	//avanzo en los registros en (tamaño en bytes del registro) * el indice obtenido 
	
	fseek(file, head[0].record_bytes * indice , SEEK_CUR);

	fread(buffer, descr[i].length, 1, file);

	return 0;
	
}

int get_record(char* buffer, int indice, FILE* file, header* head, descriptor* descr)
{
	
	if (indice == -1) 
	{
		perror("indice invalido");

		return -1;
	}
	
	//busco el indice del campo

	int i = 0;
	int j = 0;
	int offset = 0;
	//buffer[MAX_FIELD_LENGTH];

	//pongo el cursor en el lugar donde compienzan los registros

	fseek(file, head[0].header_bytes, SEEK_SET);

	//avanzo en los registros en (tamaño en bytes del registro) * el indice obtenido 
	
	fseek(file, head[0].record_bytes * indice , SEEK_CUR);

	fread(buffer, head[0].record_bytes, 1, file);

	return 0;
	
}

int readMemo(char *block, size_t size)
{
  return 0;
}

void rightAlign(char *string, size_t size)
{
  //determine amount of spaces to the right
  int counter = 0;
  int check = 0;

  for(int i = 0; i < size; i++)
  { 
    if(check == 0 && (string[i] != '\0' && string[i] != ' ')) check = 1; 
    if((string[i] == '\0' || string[i] == ' ') && check == 1) counter++;
    if(string[i] == '\0') string[i] = ' ';
  }

  memmove(&string[counter], string, size - counter);

  for(int i = 0; i < counter; i++)
  {
    string[i] = ' ';
  }

  return;
}

void spaceFill(char *string, size_t size)
{
  int counter = 0;
  for (int i = 0; i < size; i++)
  {
    if(string[i] == '\0')
    {
      string[i] = ' ';
    }
  }
  return;
}

int addDecimals(char *string, const size_t size, const size_t decimals)
{
  if(size < decimals) return -1;
  int dotCheck = 0;
  int decimalCount = 0;
  /*si hay coma y no llega a la cantidad de decimales entonces string[size - decimals - 1] = '.' 
    y un for para todos string[size - (decimals - i) - 1] hasta que i sea igual a la cantidad de decimales
    rellenar con ceros hasta el final
  */
  /*
    si no hay coma truncar los enteros hasta string[size - decimals - 2] agregar coma y rellenar con ceros
    hasta i = decimals 
  */
  /*si hay una coma
//
  for(int i = 0; i < size; i++)
  {
    if(string[i] == ' ' && i < size - decimals && dotcheck == 0)
    {
      dotcheck = 1;
      string[i] = '.';
      for(int j = 0; j < decimals; j++)
      {
        string[size - (decimals - j) -1] = '0';
      }
      break();
    }
    if(string[i] == '.' && i < size - decimals && dotcheck == 0)
    {
      dotcheck = 1;
      for(int j = 0; j < decimals; j++)
      {
        if(string[i] == ' ')
        {
          string[size - (decimals - j) -1] = '0';
        }
      break();
      }
    }
  }
  if(dotCheck == 0)
  {
    string[size - decimals - 1] = '.';
    for(int j = 0; j < decimals; j++)
    {
      string[size - (decimals - j) -1] = '0';
    }
  }
  dotCheck = 0;
  int dot = 0;
  while(string[dot] != '.')
  {
    dot++;
  }
  dot += 
  while(dot < size)
		{
		  
		}
}

int replaceField()
{
  return 0;
}

int addRecord(char* buffer, char* fname, size_t size)
{
  // I have to append the buffer into the file as a register, so I have to copy the amount of bytes of head->register_bytes.
  FILE *fPtr = NULL;
  fPtr = fopen(fname, "r+b");
  
  if(fPtr == NULL)
  {
    return -1;
  }

  header head[1];
  store_header_data(head, fPtr, 0);
  
  char *space = " "; //I'm changing the last byte from 0x1A (file terminator) to 0x20 (which is the marker for a valid register)
  
  fseek(fPtr, -1, SEEK_END);
  fwrite(space, sizeof(char), 1, fPtr);  
  
  if(fclose(fPtr) != 0)
  {
    return -2;
  }
  
  fPtr = fopen(fname, "ab");
  if(fPtr == NULL)
  {
    return -3;
  }
  if(size == head->record_bytes)
  {
    fwrite(buffer, sizeof(char), head->record_bytes, fPtr);
    
    if(fclose(fPtr) != 0)
    {
      return -4;
    }
  }
  else
  {
    return -5; // the size claimed to be on the buffer is different than the size of the record
  }
  return 0;
}

