#include <stdio.h>
#include <stdlib.h>
#include "dbftool.h"
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <ctype.h>
//#include <curses.h> // for debugging

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

//All formatting functions should be at the top of the file

void spaceFill(char *string, size_t size)
{
  for (int i = 0; i < size; i++)
  {
    if(string[i] == '\0')
    {
      string[i] = ' ';
    }
  }
  return;
}

void rightAlign(char *string, const size_t size)
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
  if(counter > 0)
  {
    memmove(&string[counter], string, size - counter);

    for(int i = 0; i < counter; i++)
    {
      string[i] = ' ';
    }
  }
  return;
}

int addDecimals(char *string, const size_t size, const size_t decimals)
{
  if(size <= decimals) return -1;
  int dotCheck = 0;

  int numPos = 0;
  for(int i = 0; i < size; i++)
  {
    if(string[i] != ' ' && string[i] != '\0')
    {
      numPos = i;
      break;
    }
  }
  // Check if parentheses are correct
  if(numPos == 0 && (string[0] == ' ' || string[0] == '\0'))
  { 
    string[0] = '0';
  }
  for(int i = numPos; i < size; i++)
  {
    if((string[i] == ' ' || string[i] == '\0') && i < size - decimals && dotCheck == 0)
    {
      dotCheck = 1;
      string[i] = '.';
      for(int j = 0; j < decimals; j++)
      {
        string[i + j + 1] = '0';
      }
    }
    if(string[i] == '.' && i < size - decimals && dotCheck == 0)
    {
      dotCheck = 1;
      for(int j = 0; j < decimals; j++)
      {
        if(string[i] == ' ')
        {
          string[i + j + 1] = '0';
        }
      }
    }
    
  }
  if(dotCheck == 0)
  {
    string[size - decimals - 1] = '.';
    for(int j = 0; j < decimals; j++)
    {
      string[size - (decimals - j)] = '0';
    }
  }
  dotCheck = 0;
  int dot = 0;
  while(string[dot] != '.')
  {
    dot++;
  }
  dot++;
  dot += decimals;
  if(dot < size)
  {
    while(dot < size)
    {
      string[dot] = ' ';
      dot++;
    }
  }
  string[dot] = '\0';
  return 0;
}



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

//get_index(campo en donde buscar, contenido que buscar, archivo que se abre, puntero al encabezado (es un struct), puntero al descriptor (struct))

int get_index(const char* campo, const char* string, FILE* file, header* head, descriptor* descr)
{
	//busco el indice del campo

	if (strlen(string) > MAX_FIELD_LENGTH) 
	{
	  perror("used more than 254 characters...");
	  return -3;
	}

	int i = 0;
	int j = 0;
	int offset = 0;
	int rindex = 0;
	int check = 1;
	char buffer[MAX_FIELD_LENGTH];
	memset(buffer, 0, 254);
	
	//fseek(file, FIELD_SIZE, SEEK_SET);
	while(strncmp(descr[i].fieldname, campo, FIELD_NAME)) 
	{
	  i++;
	  if(descr[i].fieldname[0] == 0x0d)
	  {
	    return -2; //No such field
	  }
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

	while(check != 0 && rindex < head[0].nofrecords) 
	{
	  fread(buffer, descr[i].length, 1, file);
	  check = strncmp(string, buffer, descr[i].length);

	  //avanzo en header[n].record_bytes (tamaño del registro) - (descriptor[indice del campo].length)

	  fseek(file, head[0].record_bytes - (descr[i].length) , SEEK_CUR);
	  rindex++;
	}
	
        if(check == 0) return rindex - 1; //found
        else return -1; //not found
		
}

int get_indexes(int* indexes, const char* campo, const char* string, FILE* file, header* head, descriptor* descr)
{
	/*
    Indexes must not be shorter than the estimation of
    the number of records that one expects to find, the 
    safest would be to allocate for as much indexes as
    records, but if you know how much in advance, you 
    may lower the number
  */
  // change to something safer
  int len = strnlen(string, MAX_FIELD_LENGTH);
  
	if (len > MAX_FIELD_LENGTH) 
	{
	  perror("used more than 254 characters...");
	  return -3;
	}
  
  int k = 0;
	int i = 0;
	int j = 0;
	int offset = 0;
	int rindex = 0;
	int check = 1;
	char buffer[MAX_FIELD_LENGTH + 1];
  char buffer2[MAX_FIELD_LENGTH + 1];
	memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  memset(buffer2, 0, MAX_FIELD_LENGTH + 1);
	//fseek(file, FIELD_SIZE, SEEK_SET);
  
  //add upper bound to the loop (254 which is the max amount of fields)
	while(strncmp(descr[i].fieldname, campo, FIELD_NAME)) 
	{
	  i++;
	  if(descr[i].fieldname[0] == 0x0d)
	  {
	    return -2; //No such field
	  }
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
  memcpy(buffer2, string, len);
  
  
  //right align numbers
  switch(descr[i].type)
  {
    case 'N':
      rightAlign(buffer2, descr[i].length);
      break;
    case 'C':
      spaceFill(buffer2, descr[i].length);
      break;
  }

	while(rindex < head[0].nofrecords) 
	{
	  fread(buffer, descr[i].length, 1, file);
	  check = strncmp(buffer2, buffer, descr[i].length);
    
//mvprintw(0,0, "***%s %s %d %d***", buffer2, buffer, check, k);
//getch();
    
    if(!check)
    {
      indexes[k] = rindex;
      k++;
    }

	  //avanzo en header[n].record_bytes (tamaño del registro) - (descriptor[indice del campo].length)

	  fseek(file, head[0].record_bytes - (descr[i].length) , SEEK_CUR);
	  rindex++;
	}
        if(indexes[1]) return 0; //found
        else return -1; //not found
		
}

int get_indexes_gr_th(int* indexes, const char* campo, const char* string, FILE* file, header* head, descriptor* descr)
{
	/*
    Indexes must not be shorter than the estimation of
    the number of records that one expects to find, the 
    safest would be to allocate for as much indexes as
    records, but if you know how much in advance, you 
    may lower the number
  */
  // change to something safer
  int len = strnlen(string, MAX_FIELD_LENGTH);
  
	if (len > MAX_FIELD_LENGTH) 
	{
	  perror("used more than 254 characters...");
	  return -3;
	}
  
  int k = 0;
	int i = 0;
	int j = 0;
	int offset = 0;
	int rindex = 0;
	int check = 1;
	char buffer[MAX_FIELD_LENGTH + 1];
  char buffer2[MAX_FIELD_LENGTH + 1];
	memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  memset(buffer2, 0, MAX_FIELD_LENGTH + 1);
	//fseek(file, FIELD_SIZE, SEEK_SET);
  
  //add upper bound to the loop (254 which is the max amount of fields)
	while(strncmp(descr[i].fieldname, campo, FIELD_NAME)) 
	{
	  i++;
	  if(descr[i].fieldname[0] == 0x0d)
	  {
	    return -2; //No such field
	  }
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
  memcpy(buffer2, string, len);
  
  
  //right align numbers
  if(descr[i].type == 'N')
  {
    rightAlign(buffer2, descr[i].length);
  }
	while(rindex < head[0].nofrecords) 
	{
	  fread(buffer, descr[i].length, 1, file);
	  check = strncmp(buffer2, buffer, descr[i].length);

    if(check < 0)
    {
      indexes[k] = rindex;
      k++;
    }

	  //avanzo en header[n].record_bytes (tamaño del registro) - (descriptor[indice del campo].length)

	  fseek(file, head[0].record_bytes - (descr[i].length) , SEEK_CUR);
	  rindex++;
	}
        if(indexes[0]) return 0; //found
        else return -1; //not found
		
}

int get_indexes_lw_th(int* indexes, const char* campo, const char* string, FILE* file, header* head, descriptor* descr)
{
	/*
    Indexes must not be shorter than the estimation of
    the number of records that one expects to find, the 
    safest would be to allocate for as much indexes as
    records, but if you know how much in advance, you 
    may lower the number
  */
  // change to something safer
  int len = strnlen(string, MAX_FIELD_LENGTH);
  
	if (len > MAX_FIELD_LENGTH) 
	{
	  perror("used more than 254 characters...");
	  return -3;
	}
  
  int k = 0;
	int i = 0;
	int j = 0;
	int offset = 0;
	int rindex = 0;
	int check = 1;
	char buffer[MAX_FIELD_LENGTH + 1];
  char buffer2[MAX_FIELD_LENGTH + 1];
	memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  memset(buffer2, 0, MAX_FIELD_LENGTH + 1);
	//fseek(file, FIELD_SIZE, SEEK_SET);
  
  //add upper bound to the loop (254 which is the max amount of fields)
	while(strncmp(descr[i].fieldname, campo, FIELD_NAME)) 
	{
	  i++;
	  if(descr[i].fieldname[0] == 0x0d)
	  {
	    return -2; //No such field
	  }
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
  memcpy(buffer2, string, len);
  
  
  //right align numbers
  if(descr[i].type == 'N')
  {
    rightAlign(buffer2, descr[i].length);
  }
	while(rindex < head[0].nofrecords) 
	{
	  fread(buffer, descr[i].length, 1, file);
	  check = strncmp(buffer2, buffer, descr[i].length);

    if(check > 0)
    {
      indexes[k] = rindex;
      k++;
    }

	  //avanzo en header[n].record_bytes (tamaño del registro) - (descriptor[indice del campo].length)

	  fseek(file, head[0].record_bytes - (descr[i].length) , SEEK_CUR);
	  rindex++;
	}
        if(indexes[0]) return 0; //found
        else return -1; //not found
		
}

int get_indexes_betw(int* indexes, const char* campo, const char* low, const char* high, FILE* file, header* head, descriptor* descr)
{
	/*
    Indexes must not be shorter than the estimation of
    the number of records that one expects to find, the 
    safest would be to allocate for as much indexes as
    records, but if you know how much in advance, you 
    may lower the number
  */
  // change to something safer
  int lenH = strnlen(high, MAX_FIELD_LENGTH);
  int lenL = strnlen(low, MAX_FIELD_LENGTH);
  
	if (lenH > MAX_FIELD_LENGTH || lenL > MAX_FIELD_LENGTH) 
	{
	  perror("used more than 254 characters...");
	  return -3;
	}
  
  int k = 0;
	int i = 0;
	int j = 0;
	int offset = 0;
	int rindex = 0;
	int check = 1;
  int check2 = 1;
	char buffer[MAX_FIELD_LENGTH + 1];
  char buffer2[MAX_FIELD_LENGTH + 1];
  char buffer3[MAX_FIELD_LENGTH + 1];
	memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  memset(buffer2, 0, MAX_FIELD_LENGTH + 1);
  memset(buffer3, 0, MAX_FIELD_LENGTH + 1);
	//fseek(file, FIELD_SIZE, SEEK_SET);
  
  //add upper bound to the loop (254 which is the max amount of fields)
	while(strncmp(descr[i].fieldname, campo, FIELD_NAME)) 
	{
	  i++;
	  if(descr[i].fieldname[0] == 0x0d)
	  {
	    return -2; //No such field
	  }
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

	//leo el valor del campo y comparo con el high que quiero buscar
  memcpy(buffer2, high, lenH);
  memcpy(buffer3, low, lenL);
  
  //right align numbers
  if(descr[i].type == 'N')
  {
    rightAlign(buffer2, descr[i].length);
    rightAlign(buffer3, descr[i].length);
  }
	while(rindex < head[0].nofrecords) 
	{
	  fread(buffer, descr[i].length, 1, file);
	  check = strncmp(buffer2, buffer, descr[i].length);
    check2 = strncmp(buffer3, buffer, descr[i].length);

    if(check >= 0 && check2 <= 0)
    {
      indexes[k] = rindex;
      k++;
    }

	  //avanzo en header[n].record_bytes (tamaño del registro) - (descriptor[indice del campo].length)

	  fseek(file, head[0].record_bytes - (descr[i].length) , SEEK_CUR);
	  rindex++;
	}
        if(indexes[0]) return 0; //found
        else return -1; //not found
}

int get_data(char* buffer, int indice, char* campo, FILE* file, header* head, descriptor* descr)
{
	if (indice == -1) 
	{
		perror("Record not found");
		return -1;
	}
	
	//busco el indice del campo

	int i = 0;
	int j = 0;
	int offset = 0;
	fseek(file, FIELD_SIZE, SEEK_SET);
	while(strncmp(descr[i].fieldname,campo, FIELD_NAME)) 
	{
	  i++;
	  if(descr[i].fieldname[0] == 0x0d)
	  {
	    return -2; //No such field
	  }
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

/*
int get_bulk_data(char* buffer, int* indices, char* campo, FILE* file, header* head, descriptor* descr)
{
	if (indice == -1) 
	{
		perror("Record not found");
		return -1;
	}
	
	//busco el indice del campo

	int i = 0;
	int j = 0;
	int offset = 0;
	fseek(file, FIELD_SIZE, SEEK_SET);
	while(strncmp(descr[i].fieldname,campo, FIELD_NAME)) 
	{
	  i++;
	  if(descr[i].fieldname[0] == 0x0d)
	  {
	    return -2; //No such field
	  }
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
*/
int get_record(char* buffer, int indice, FILE* file, header* head, descriptor* descr)
{
	if (indice == -1) 
	{
		perror("Record Not found");
		return -1;
	}
	
	//buffer[MAX_FIELD_LENGTH];

	//pongo el cursor en el lugar donde compienzan los registros

	fseek(file, head[0].header_bytes, SEEK_SET);

	//avanzo en los registros en (tamaño en bytes del registro) * el indice obtenido 
	
	fseek(file, head[0].record_bytes * indice , SEEK_CUR);

	fread(buffer, head[0].record_bytes, 1, file);

	return 0;
	
}

int decodeMemoStr(char* buffer)
{
  int i = 0;
  while(buffer[i] != 0 && buffer[i] != 0x1A) 
  {
    if(buffer[i] == 0x8d || buffer[i] == 0x0d)
    {
      buffer[i] = ' ';
    }
    i++;
    if(buffer[i] == 0x1A)
    {
      buffer[i] = 0;
    }
  }
  return 0;
}

int encodeMemoStr(char* buffer, const unsigned int heigth, const unsigned int width)
{
  return 0; //TODO
}

int readMemo(char* buffer, char* file, const int block)
{
  if (block == 0 || block == 1) 
  {
    return -1; //invalid Block
  }

  FILE* memoptr = NULL;
  memoptr = fopen(file, "rb");

  if (memoptr == NULL) 
  {
    return -2; //File not found or in use
  }
  
  fseek(memoptr, 0x200 * block ,SEEK_SET);
  fread(buffer, 1, 512, memoptr);

  if (fclose(memoptr) != 0) 
  {
    return -3; //Could not close the File
  }

  decodeMemoStr(buffer);

  return 0;

//first block is number 2


    /*
DBT are storage for fulltext data. Data is stored in simple 512 byte blocks. If data is > 510 byte it will be stored in continuous blocks.
Only reference to the data is an pointer stored in the memo field in the datafile. The pointer is not displayed in dBASE.
If the pointer in a data file record is not empty dBASE will open a file with same name as the data file but with the extention .dbt.
The memo file header has only two values in its 512 bytes:

next available block
version number
Rest of the header is garbage.
Next available block will always point the end of the file. Data can only be appended.
    _______________________  _______
 0 | Number of next        |  ^  ^
 1 | available block       |  |  |
 2 | for appending data    |  | Header
 3 | (binary)              |  |  |
   |-----------------------| _|__v__
 4 | ( Reserved )          |  |
   |                       |  |
   |                       |  |
 7 |                       |  |
   |-----------------------|  |
 8 | ( Reserved )          |  |
   :                       :  |
15 |                       |  |
   |-----------------------|  |
16 | Version no.  (03h)    |  |
   |-----------------------|  |
17 | (i.e. garbage)        |  |
   :                       : First block
   :                       :  |
511|                       |  |
   |=======================| _v_____
512|                       |  ^
   |                       |  |
   |                       | 512 Bytes
   |                       | text blocks
   :                       :  |
   :                       :  |
   |                       |  |
   |-----------------------| _|_____
   | Field terminator (1Ah)|  |  ^
   |-----------------------|  |  |Terminating field
   | Field terminator (1Ah)|  |  |within the block *1
   |-----------------------| _|__v__
   : ( Unused )            :  |
1023                       :  |
   |=======================| _v_____
   |                       |  ^
   |                       |  |
   |                       | 512 Bytes
   |                       | text blocks
   :                       :  |
   :                       :  |
   |                       |  |
   |                       | _v_____
   |=======================|

field terminator Is reported to use only one field terminator (1Ah) - (FoxPro, Fox??)
A memo field can be longer than the 512 byte block. It simply continues through the next block. The field is logically terminated by two End-of-file marks in the field. The reminder of the block is unused.
	Every time you re-write a memo field to the memo file in dBASE the field is APPENDED to the memo file (i.e. the file expands for each update). dBASE V for DOS (and perhaps others) may reuse the space from the deleted text, if memo size <= allocated number of block in dbt file. 
There is NO marking of deleted data. Valid data can only be determined from the data file.
The Memo file itself tells nothing about it's use. You need the corresponding DBF file to interpret the content of the memo file. Many of the clones and dBASE 5 have binary elements stored in the memo file marked with file type B or G.
  */
}

int replaceField(char* buffer, int indice, char* campo, FILE* file, const header* head, const descriptor* descr)
{ 
  if (indice == -1) 
  {
    perror("indice invalido");
    return -1;
  }
  
  //make sure buffer is properly formatted so there is no data corruption

  char formatted[MAX_FIELD_LENGTH + 1] = {0};
  memset(formatted, ' ', MAX_FIELD_LENGTH);
  memcpy(formatted, buffer, strlen(buffer));
  	
  //busco el indice del campo

  int i = 0;
  int j = 0;
  int offset = 0;
  fseek(file, FIELD_SIZE, SEEK_SET);
  while(strncmp(descr[i].fieldname,campo, FIELD_NAME)) 
  {
    i++;
    if(descr[i].fieldname[0] == 0x0d)
    {
      return -2; //No such field
    }
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

  int written = fwrite(formatted, descr[i].length, 1, file);

  //fflush(file);
  //fseek(file, -descr[i].length , SEEK_CUR);
  //fread(buffer, descr[i].length, 1, file);

  return written;
}

int replaceRecord()
{
  return 0;
}

void dateFormatter(char* date)
{
  //This reformats the date form ddmmyyyy to yyyymmdd

  int charMap[8] = {4, 5, 6, 7, 2, 3, 0, 1};
  char buffer[16] = {0};
  
  memcpy(buffer, date, 8);

  for(int i = 0; i < 8; i++)
  {
    date[i] = buffer[charMap[i]];
  }
  return;
}
/*
converts a decimal array to a long long of at most 17 digits (to prevent overflow)
TODO add a version for arbitrary presicion arithmetic (might be slower)
*/
long long atonum(const char* number)
{
  int i = 0;
  int j = 0;
  int negcheck = 0;
  int numcheck = 0;
  long long retval = 0;
  char noDecimal[254] = {0};
  while(i < 18 || number[i] != 0)
  { 
    if(isdigit(number[i]))
    {
      noDecimal[j] = number[i];
      j++;
    }
    else if(negcheck == 0 && numcheck == 0 && number[i] == '-')
    {
      noDecimal[j] = '-';
      negcheck = 1;
      numcheck = 1;
      j++;
    }
    i++;
  }
  //transform nodecimal to a long long
  retval = atoll(noDecimal);
  return retval; 
}

int ntoa(char* buffer, const long long num, const size_t decimals, const size_t fieldSize)
{
  if(decimals >= fieldSize)
  {
    return -1; //too many decimal places
  }
  memset(buffer, 0, 21);//this is for long long, bigger integers should not work
  snprintf(buffer, 21, "%lld", num);
  int len = strlen(buffer);
//printf(" %d ", len);
//printf("\n%s\n", buffer);
  if(len + 2 > fieldSize)
  {
    return -2; //number too long
  }
  if(decimals != 0 && decimals < len)
  {
    memmove(&buffer[len - decimals + 1], &buffer[len - decimals], decimals);
    buffer[len - decimals] = '.';
  }
  else if (decimals >= len)
  {
    memmove(&buffer[decimals - len + 2], buffer, decimals);
    memset(buffer , 0x30, decimals - len + 2);
    buffer[1] = '.';
  }
  else if (decimals == len)
  {
    memmove(&buffer[decimals - len + 2], buffer, decimals);
    memset(buffer , 0x30, decimals - len + 1);
    buffer[1] = '.';
  }
  return 0;
}

//This is slow If i Have to sum more than 2 strings of numbers.
int sumFields(char *res, char* a, char* b)
{
  //Declarations
  int dotA = 0;
  int dotB = 0;
  int decA = 0;
  int decB = 0;
  int decTop = 0;
  int decBottom = 0;
  int diff = 0;
  long long int tenPow = 1;
  //check decimals
  for(int i = 0; i < 21 ; i++)
  {
    if(a[i] == 0)
    {
      dotA = 0;
    }
    else if(dotA == 1) 
    {
      decA++;
    }
    else if(a[i] == '.') 
    {
      dotA = 1;
    }
    if(b[i] == 0)
    {
      dotB = 0;
    }
    if(dotB == 1) 
    {
      decB++;
    }
    else if(b[i] == '.') 
    {
      dotB = 1;
    }
  }
  //conversion to int
  long long anum = atonum(a);
  long long bnum = atonum(b);
  //Which has more decimal places
  if(decA >= decB)
  {
    decTop = decA;
    decBottom = decB;
    diff = decTop - decBottom;
    for(int i = 0; i < diff; i++) tenPow *= 10;
    bnum *= tenPow;
  }
  else
  {
    decTop = decB;
    decBottom = decA;
    diff = decTop - decBottom;
    for(int i = 0; i < diff; i++) tenPow *= 10;
    anum *= tenPow;
  }
  long long resnum = bnum + anum;
  ntoa(res, resnum, decTop, 20);
  return 0;
}

int subFields(char *res, char* a, char* b)
{
  //the ordering is b - a = res
  //Declarations
  int dotA = 0;
  int dotB = 0;
  int decA = 0;
  int decB = 0;
  int decTop = 0;
  int decBottom = 0;
  int diff = 0;
  long long int tenPow = 1;
  //check decimals
  for(int i = 0; i < 21 ; i++)
  {
    if(a[i] == 0)
    {
      dotA = 0;
    }
    else if(dotA == 1) 
    {
      decA++;
    }
    else if(a[i] == '.') 
    {
      dotA = 1;
    }
    if(b[i] == 0)
    {
      dotB = 0;
    }
    if(dotB == 1) 
    {
      decB++;
    }
    else if(b[i] == '.') 
    {
      dotB = 1;
    }
  }
  //conversion to int
  long long anum = atonum(a);
  long long bnum = atonum(b);
  //Which has more decimal places
  if(decA >= decB)
  {
    decTop = decA;
    decBottom = decB;
    diff = decTop - decBottom;
    for(int i = 0; i < diff; i++) tenPow *= 10;
    bnum *= tenPow;
  }
  else
  {
    decTop = decB;
    decBottom = decA;
    diff = decTop - decBottom;
    for(int i = 0; i < diff; i++) tenPow *= 10;
    anum *= tenPow;
  }
  long long resnum = bnum - anum;
  ntoa(res, resnum, decTop, 20);
  return 0;
}

int addRecord(char* buffer, const char* fname, size_t size)
{
  // I have to append the buffer into the file as a register, so I have to copy the amount of bytes of head->register_bytes.
  FILE *fPtr = NULL;
  header head[1];
  int recCount[1];

  fPtr = fopen(fname, "r+b");
  
  if(fPtr == NULL)
  {
    return -1;
  }

  store_header_data(head, fPtr, 0);

  fseek(fPtr, 3, SEEK_SET);
  
  recCount[0] = head[0].nofrecords;
  recCount[0]++;

  fseek(fPtr, 4, SEEK_SET);
  fwrite(&recCount[0], sizeof(int), 1, fPtr);
  
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

int addMemo(const char* fileName, char* buffer)
{
  //variables

  FILE* dbtptr = NULL;
  int nOfMemos = 0;
  long int pos = 0;
  int padding = 0;
  char zeroes[512] = {0};
  zeroes[0] = 0x1a;
  zeroes[2] = 0x1a;

  //open .dbt file

  dbtptr = fopen(fileName, "r+b");
  if(dbtptr == NULL)
  {
    return -1; //file not found
  }

  //read the first two bytes of the .dbt file

  fread(&nOfMemos, 2, 1, dbtptr);
  nOfMemos++;
  fseek(dbtptr, 0, SEEK_SET);
  fwrite(&nOfMemos, 2, 1, dbtptr);
  fseek(dbtptr, 0, SEEK_END);
  pos = ftell(dbtptr);
  padding = 512 * (nOfMemos - 1) - pos;
  
  //append zeores until next block then append memo

  if(fclose(dbtptr) != 0)
  {
    return -2; //could not close file
  }
  dbtptr = fopen(fileName, "ab");
  if(dbtptr == NULL)
  {
    return -3; //file not found for appending
  }
  fwrite(zeroes, sizeof(char), padding, dbtptr);
  fwrite(buffer, sizeof(char), strlen(buffer) - 1, dbtptr);
  if(fclose(dbtptr) != 0)
  {
    return -4; //could not close appending file
  }
  return (nOfMemos - 1);
}

int replaceMemo(const char* fileName, char* buffer, int block)
{
  //variables

  FILE* dbtptr = NULL;
  int nOfMemos = 0;
  char zeroes[512] = {0};

  //open .dbt file

  dbtptr = fopen(fileName, "r+b");
  if(dbtptr == NULL)
  {
    return -1; //file not found
  }

  //read the first two bytes of the .dbt file

  fread(&nOfMemos, 2, 1, dbtptr);
  fseek(dbtptr, 512 * block, SEEK_SET);
  fwrite(zeroes, sizeof(char), 512, dbtptr);
  fseek(dbtptr, 512 * block, SEEK_SET);
  fwrite(buffer, sizeof(char), strlen(buffer) - 1, dbtptr);
  
  //append zeores until next block then append memo

  if(fclose(dbtptr) != 0)
  {
    return -2; //could not close file
  }
  return 0;
}
