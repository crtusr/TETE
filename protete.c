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
  input_fields_loop(entrada, entradas, operaciones);

  stockptr = fopen("STOCK1.dbf", "rb");
  
  if (stockptr == NULL) {
    perror("error abriendo STOCK1.dbf");

    return;
  }

  store_header_data(stockhead, stockptr, 0);

  store_descriptor_data(stockdescr, stockptr);

  indice = get_index("CODIGO", entrada[0].input_buffer, stockptr, stockhead, stockdescr);

  while(stockdescr[descriptor_index].fieldname[0] != TERMINATOR) 
  {

    get_data(buffer , indice, stockdescr[descriptor_index].fieldname, stockptr, stockhead, stockdescr);
  
    mvprintw(descriptor_index + 5, 10 + (6 - strlen(stockdescr[descriptor_index].fieldname)), "%s: %s", stockdescr[descriptor_index].fieldname, buffer);
    
    descriptor_index++;
  
  }

  refresh();

  free(entrada[0].prompt);

  if (fclose(stockptr) != 0) {
    mvprintw(0, 0, "Error al cerrar el archivo: %s", strerror(errno));
    getch();
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

  init_input_field(&entrada[0], "Operacion: ", 4, false, 5, 2, STRING);

  input_fields_loop(entrada, entradas, operaciones);

  operaciones();

  refresh();

  ctasctes_ptr = fopen("CTASCTES.DBF", "rb");
  
  if (ctasctes_ptr == NULL) {

    perror("error abriendo CTASCTES.dbf");

    return;
  }

  store_header_data(ctasctes_head, ctasctes_ptr, 0);
  store_descriptor_data(ctasctes_descr, ctasctes_ptr);

  rightAlign(entrada[0].input_buffer, ctasctes_descr[0].length);

  indice = get_index("OPERAC", entrada[0].input_buffer, ctasctes_ptr, ctasctes_head, ctasctes_descr);

  get_data(buffer, indice, ctasctes_descr[0].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(2, 17, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[1].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(2, 35, "%c%c-%c%c-%c%c", buffer[6], buffer[7], buffer[4], buffer[5], buffer[2], buffer[3]);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[2].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(2, 60, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[3].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(4, 19, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[4].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(4, 48, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[5].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(9, 48, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[6].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(10, 48, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[7].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(17, 11, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[8].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(17, 28, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[9].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(17, 46, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[10].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(17, 66, "%s", buffer);

  

  mvprintw(0, 0, "Representacion en hexadecimal: %x %x %x %x %x %x ",
    entrada[0].input_buffer[0], 
    entrada[0].input_buffer[1], 
    entrada[0].input_buffer[2], 
    entrada[0].input_buffer[3], 
    entrada[0].input_buffer[4], 
    entrada[0].input_buffer[5]);
  
  free(entrada[0].prompt);
  if (fclose(ctasctes_ptr) != 0) 
  {
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


  char espacios[6] = "      "; //just to align with the field

  init_input_field(&entrada[0], "Operacion: ", 4, false, 5, 2, STRING);


  input_fields_loop(entrada, entradas, operaciones);



  operaciones();


  refresh();



  int espacios_vacios = 6 - strlen(entrada[0].input_buffer); //NO ENTIENDO PORQUE 6, TENDRIA QUE SER 4



  if(strlen(entrada[0].input_buffer) < 6)
    {


    memmove(&entrada[0].input_buffer[6 - strlen(entrada[0].input_buffer)],
 
      &entrada[0].input_buffer[0],
 
      strlen(entrada[0].input_buffer));



    memmove(&entrada[0].input_buffer[0], espacios, espacios_vacios);
    
} //YESSSSSSSSSS!!!!!!!

 


  compras_ptr = fopen("COMPRA.DBF", "rb");

  
  if (compras_ptr == NULL) 
    {


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



  free(entrada[0].prompt);
  if (fclose(compras_ptr) != 0) 
  {

    mvprintw(0, 0, "Error al cerrar el archivo: %s", strerror(errno));
    getch();
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
  input_fields_loop(entrada, entradas, operaciones);

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
    
    //I might have to add one to printOffset since I want the fields to have a space of separation 
   
    for(int j = 4; (current) < (i) && current < end; j++) 
      {
      int offset = 1;
      int printOffset = 1;
  
      //mvprintw(j, 0, "the loop executes"); // for debugging
      
      //mvprintw(j, 0, "%s", dbfData); //for debugging
  
      mvprintw(j, 0, "%.4s", &dbfData[(current) * ctasctes_head[0].record_bytes] + offset);
      offset += OPERAC;
      printOffset += OPERAC;
      
      mvprintw(j, printOffset, "%c%c/%c%c",   dbfData[(current) * ctasctes_head[0].record_bytes + offset + 6],
                dbfData[(current) * ctasctes_head[0].record_bytes + offset + 7],
                dbfData[(current) * ctasctes_head[0].record_bytes + offset + 4],
                dbfData[(current) * ctasctes_head[0].record_bytes + offset + 5]);
      offset += FECHA;
      printOffset += FECHA - 2;
      
      //mvprintw(j, printOffset, "%.13s", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
      offset += FACTUR;
      offset += NROCLI;  // not printed
      offset += DENOMI;  // not printed
  
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
  if (fclose(ctasctes_ptr) != 0) 
  {
    mvprintw(0, 0, "Error al cerrar el archivo: %s", strerror(errno));
    return;
  }
  
  refresh();
  free(entrada[0].prompt);
  free(dbfData);
  return;
}

void agregarCtacte()
{ 

  //file names

  char cuentasCtes[15] = "CTASCTES.DBF";
  char cliPro[15] = "CLIPRO.DBF";

  //file pointers

  FILE *ctasctes_ptr = NULL;
  header ctasctes_head[1];
  descriptor ctasctes_descr[15]; // I chose 15 because i Know the file I will be opening has less than 15 fields,
  FILE *cli_ptr = NULL;
  header cli_head[1];
  descriptor cli_descr[20]; // MAKE SURE YOU DECLARE MORE DESCRIPTORS THAN THERE ALREADY ARE, IF NOT, IT WILL BE THE END OF THE WORLD
  
  //variables and structs related to CTASCTES.dbf
  
  InputField cabecera[4];
  int cabeceras = 4;
  InputField operacion[6];
  int noperaciones = 6;
  char lastOp[4 + 1] = {0};
  char buffer[118 + 50] = {0};
  char memo [10 + 1] = "          ";
  
  //Variables related to CLIPRO.dbf
  
  int cliente = 0;
  char nombreCli[35] = {0};
  char cliBuffer[35] = {0}; 

  ctasctes_ptr = fopen(cuentasCtes, "rb");
  if (ctasctes_ptr == NULL) 
  {
    mvprintw(0, 0, "Error abriendo CTASCTES.dbf: %s", strerror(errno));
    mvprintw(1, 0, "Presione cualquier tecla para salir...");
    refresh();
    getch();
    return;
  }
  
  store_header_data(ctasctes_head, ctasctes_ptr, 0);
  store_descriptor_data(ctasctes_descr, ctasctes_ptr);

  fseek(ctasctes_ptr, -ctasctes_head[0].record_bytes, SEEK_END);
  fread(lastOp, ctasctes_descr[0].length, 1, ctasctes_ptr);

  if (fclose(ctasctes_ptr) != 0) 
  {
    mvprintw(0, 0, "Error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }

  int iLastOp = atoi(lastOp);
  iLastOp++;
  snprintf(lastOp, 5,"%d", iLastOp); //I wasn't aware snprintf truncated the input to ensure null termination so size has to be 5
  getch();

  
  init_input_field(&cabecera[0], "Operacion:[", ctasctes_descr[0].length , false, 5, 2, INTEGER); //automatically write last op. + 1
  init_input_field(&cabecera[1], "Fecha: ", ctasctes_descr[1].length, false, 28, 2, INTEGER); //should make a date parser
  init_input_field(&cabecera[2], "factura no.:", ctasctes_descr[2].length, false, 46, 2, STRING); 
  init_input_field(&cabecera[3], "cliente: ", ctasctes_descr[3].length, false, 8, 4, INTEGER);


  memcpy(cabecera[0].input_buffer, lastOp, ctasctes_descr[0].length);
  cabecera[0].count = ctasctes_descr[0].length;
  cabecera[0].cursor_pos = cabecera[0].count;
  rightAlign(cabecera[0].input_buffer, ctasctes_descr[0].length);



  operaciones();
  input_fields_loop(cabecera, cabeceras, operaciones);
  if (strncmp(cabecera[0].input_buffer, "   0", 4) == 0)
  {
    return;
  }  

  //TODO get client data and print it on the screen

  cli_ptr = fopen(cliPro, "rb");

  if (cli_ptr == NULL) 
  {
    mvprintw(0, 0, "Error abriendo CLIPRO.dbf: %s", strerror(errno));
    mvprintw(1, 0, "Presione cualquier tecla para salir...");
    refresh();
    getch();
    return;
  }

  store_header_data(cli_head, cli_ptr, 0);
  store_descriptor_data(cli_descr, cli_ptr);
  rightAlign(cabecera[3].input_buffer, ctasctes_descr[3].length);

  cliente = get_index(cli_descr[0].fieldname, cabecera[3].input_buffer, cli_ptr, cli_head, cli_descr);
  get_data(nombreCli, cliente, cli_descr[1].fieldname,cli_ptr, cli_head, cli_descr);
  mvprintw(4, 48, "%.23s", nombreCli);
  

  init_input_field(&operacion[0], "Credito ", ctasctes_descr[5].length , false, 39, 9, FLOAT); //automatically write last op. + 1
  init_input_field(&operacion[1], "Factura$", ctasctes_descr[6].length, false, 39, 10, FLOAT);
  init_input_field(&operacion[2], "Cheque", ctasctes_descr[7].length, false, 4, 17, FLOAT); 
  init_input_field(&operacion[3], "Efec.", ctasctes_descr[8].length, false, 22, 17, FLOAT);
  init_input_field(&operacion[4], "Cheque", ctasctes_descr[9].length, false, 39, 17, FLOAT); 
  init_input_field(&operacion[5], "Efectivo", ctasctes_descr[10].length, false, 57, 17, FLOAT);

refresh();

  input_fields_loop(operacion, noperaciones, operaciones);

  //this sets up the fields (right aligns integers and floats, fills null characters in strings with empty spaces)

  for(int i = 0; i < 4; i++)
  {
    if(cabecera[i].type == 0)
    {
      spaceFill(cabecera[i].input_buffer, ctasctes_descr[i].length);
    }
    else
    {
      rightAlign(cabecera[i].input_buffer, ctasctes_descr[i].length);
    }
  }
  
    for(int i = 0; i < 6; i++)
  {
    if(operacion[i].type == 0)
    {
      spaceFill(operacion[i].input_buffer, ctasctes_descr[i + 5].length);
    }
    else
    {
      rightAlign(operacion[i].input_buffer, ctasctes_descr[i + 5].length);
    }
  }

  int off = 0;
  for(int i = 0; i < 4; i++)
  {
    memcpy(&buffer[off], cabecera[i].input_buffer, ctasctes_descr[i].length);
    off += ctasctes_descr[i].length;
  }

  memcpy(&buffer[off], nombreCli, ctasctes_descr[4].length);
  off += ctasctes_descr[4].length;

  for(int i = 0; i < 6; i++)
  {
    memcpy(&buffer[off], operacion[i].input_buffer, ctasctes_descr[i+5].length);
    off += ctasctes_descr[i+5].length;
  }
  
  memcpy(&buffer[off], memo, ctasctes_descr[11].length);
  off += ctasctes_descr[11].length;
  
  buffer[off] = 0x1A;

  //the last byte of buffer should be 0x1A since it is the .dbf file's terminator
  

mvprintw(0, 0, "%s", buffer); //for debugging purposes I'm checking data integrity (and alignment) here
mvprintw(2, 2, "%x%s%d", buffer[off], memo,ctasctes_descr[11].length);

  int check = yesNoMenu("CONFIRMA", 19, 30, "REPITE", 19, 45);
  if(check == 1) 
  {
    addRecord(buffer, cuentasCtes, 118);
  }
  
  getch();

  free(cabecera[0].prompt);
  free(cabecera[1].prompt);
  free(cabecera[2].prompt);
  free(cabecera[3].prompt);

  free(operacion[0].prompt);
  free(operacion[1].prompt);
  free(operacion[2].prompt);
  free(operacion[3].prompt);
  free(operacion[4].prompt);
  free(operacion[5].prompt);



  if (fclose(cli_ptr) != 0) 
  {
    mvprintw(0, 0, "Error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }
}

void modCtacte()
{

}
