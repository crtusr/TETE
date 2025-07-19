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

//temporary function for writing memos

static void encodeMemo(unsigned char* encoded, InputField* memo)
{
  int lastLine = 4;
  int encodedIndex = 0;
  for(int i = 4; i >= 0; i--)
  {
    if(strlen(&memo[i].input_buffer[0]) != 0)
    {
      lastLine = i;
      break;
    }
  }
  //printf("%d *%x*", lastLine, memo[0].input_buffer[0]);
  for(int i = 0; i <= lastLine; i++)
  {
    for(int j = 0; j < 31; j++)
    {
      if(memo[i].input_buffer[j] != 0 && memo[i].input_buffer[j] != 0x0a)
      {
        encoded[encodedIndex] = memo[i].input_buffer[j];
        encodedIndex++;
      }
      else if(i < lastLine && memo[i].input_buffer[j] == 0)
      {
        encoded[encodedIndex] = 0x0d;
        encodedIndex++;
        encoded[encodedIndex] = 0x0a;
        encodedIndex++;
        break;
      }
      else if(i == lastLine && memo[i].input_buffer[j] == 0)
      {
        encoded[encodedIndex] = 0x1a;
        encodedIndex++;
        encoded[encodedIndex] = 0x1a;
        encodedIndex++;
        encoded[encodedIndex] = 0x00;
        encodedIndex++;
        encoded[encodedIndex] = 0x1a;
        encodedIndex++;
        break;
      }
    }
  }
  return;
}

static void edMemoTextbox(char* buffer)
{
  // for the sake of safety make the buffer 513 bytes long

  InputField memo[5];
  int boxHeight = 5;
  int lineNum = 0;
  int j = 0;
  int ib = 0;
  mvprintw(0, 0, "Check before loops");
  getch();
  for(int i = 0; i < 5; i++)
  {
    init_input_field(&memo[i], "", 31, FALSE, 4, 9 + i, STRING);
  }
  while(ib < 512)
  {
    if(buffer[ib] == 0x20 && buffer[ib + 1] == '\n')
    {
      memo[lineNum].count = j;
      memo[lineNum].cursor_pos = memo[lineNum].count;
      j = 0;
      lineNum++;
      ib += 2;
      //memo[lineNum].input_buffer[j] = buffer[i];
      //j++;
    }
    else if(buffer[ib] == 0x0)
    {
      memo[lineNum].count = j;
      memo[lineNum].cursor_pos = memo[lineNum].count;
      break;
    }
    else
    {
      memo[lineNum].input_buffer[j] = buffer[ib];
      j++;
      ib++;
    }
  }
  input_fields_loop(memo, boxHeight, NULL);
  //concatenate the input buffers
  encodeMemo(buffer, memo);
  return;
}

static void memoTextbox(char* buffer)
{
  // for the sake of safety make the buffer 513 bytes long
  InputField memo[5];
  int boxHeight = 5;
  for(int i = 0; i < 5; i++)
  {
    init_input_field(&memo[i], "", 31, FALSE, 4, 9 + i, STRING);
  }
  input_fields_loop(memo, boxHeight, NULL);
  //concatenate the input buffers
  encodeMemo(buffer, memo);
  return;
}

int isMemoEmpty(char* buffer)
{
  
  for(int i = 0; i < 5; i++)
  {
    for(int j = 0; j < 31; j++)
    {
      if(buffer[j + i * 31] != '\0' && buffer[j + i * 31] != 0x0a && buffer[j + i * 31] != 0x0d && buffer[j + i * 31] != 

0x1a) 
      {
        return 0;
      }
    }
  }
  return 1;
}

//temporary functions for saving memos

void sto_consulta(){
  
  header stockhead[1];
  descriptor stockdescr[MAX_DBF_FIELDS];
  InputField entrada[1];
  int entradas = 1;
  FILE *stockptr = NULL;
  int indice = 0;
  char buffer[MAX_FIELD_LENGTH + 1] = {0};
  memset(buffer, 0, MAX_FIELD_LENGTH + 1);

  int descriptor_index = 1;
  
  init_input_field(&entrada[0], "CODIGO: ", 4, false, 10, 5, STRING);
  input_fields_loop(entrada, entradas, NULL);

  stockptr = fopen("STOCK1.dbf", "rb");
  
  if (stockptr == NULL) 
  {
    mvprintw(0, 0, "error abriendo STOCK1.dbf: %s", strerror(errno));
    getch();
    return;
  }

  store_header_data(stockhead, stockptr, 0);

  store_descriptor_data(stockdescr, stockptr);

  indice = get_index("CODIGO", entrada[0].input_buffer, stockptr, stockhead, stockdescr);
  if(indice ==  -1)
  {
    mvprintw(0, 0, "not found");
    return;
  }

  while(stockdescr[descriptor_index].fieldname[0] != TERMINATOR) 
  {

    get_data(buffer , indice, stockdescr[descriptor_index].fieldname, stockptr, stockhead, stockdescr);
  
    mvprintw(descriptor_index + 5, 10 + (6 - strlen(stockdescr[descriptor_index].fieldname)), "%s: %s", stockdescr

[descriptor_index].fieldname, buffer);
    memset(buffer, 0, MAX_FIELD_LENGTH + 1);
    
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
  char buffer[MAX_FIELD_LENGTH + 1] = {0};
  char memoBuff[MEMOBLOCKSIZE + 1] = {0};

  init_input_field(&entrada[0], "Operacion: ", 4, false, 5, 2, STRING);

  input_fields_loop(entrada, entradas, operaciones);

  operaciones();

  refresh();

  ctasctes_ptr = fopen("CTASCTES.DBF", "rb");
  
  if (ctasctes_ptr == NULL) 
  {
    mvprintw(0, 0, "error abriendo CTASCTES.dbf: %s", strerror(errno));
    getch();
    return;
  }

  store_header_data(ctasctes_head, ctasctes_ptr, 0);
  store_descriptor_data(ctasctes_descr, ctasctes_ptr);

  rightAlign(entrada[0].input_buffer, ctasctes_descr[0].length);

  indice = get_index("OPERAC", entrada[0].input_buffer, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  if(indice ==  -1)
  {
    mvprintw(0, 0, "not found");
    return;
  }

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

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[11].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  int blockNum = atoi(buffer);
  
  int height = 5;
  int width = 31;
  int start_y = 9;
  int start_x = 4;

  /* 3. Create a derived window from stdscr */
  // This is functionally very similar to newwin() in this context.
  WINDOW *win = derwin(stdscr, height, width, start_y, start_x);
  if (win == NULL) 
  {
    mvprintw(0, 0, "win is NULL");
    endwin();
    return;
  }
    
  readMemo(memoBuff,"CTASCTES.DBT", blockNum);
  wprintw(win,"%s", memoBuff);
  wrefresh(win);

  endwin();

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
    mvprintw(0,0,"error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }

  getch();
  return;
  
}

void consulta_compra()
{

  
  header compras_head[1]; 
  descriptor compras_descr[MAX_DBF_FIELDS];

  InputField entrada[1];
  
  int entradas = 1;

  FILE *compras_ptr = NULL;

  int indice = 0;
  
  char buffer[254];

  memset(buffer, 0, 254);
  char memoBuff[MEMOBLOCKSIZE + 1] = {0};
  
  char espacios[6] = "      "; //just to align with the field

  init_input_field(&entrada[0], "Operacion: ", 4, false, 5, 2, STRING);


  input_fields_loop(entrada, entradas, operaciones);



  operaciones();


  refresh();





 


 compras_ptr = fopen("COMPRA.DBF", "rb");

  
if (compras_ptr == NULL) 
  {


    mvprintw(0,0,"error abriendo COMPRA.DBF: %s", strerror(errno));
    getch();
    return;
  }



  store_header_data(compras_head, compras_ptr, 0);


  store_descriptor_data(compras_descr, compras_ptr);

  rightAlign(entrada[0].input_buffer, compras_descr[0].length);

  indice = get_index("ORDCOM", entrada[0].input_buffer, compras_ptr, compras_head, 

compras_descr);

  if(indice ==  -1)
  {
    mvprintw(0, 0, "Registro no encontrado"); 
    getch();
    return;
  }
  
  get_data(buffer, indice, compras_descr[0].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(2, 17, "%s", buffer);




  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[1].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(2, 35, "%c%c-%c%c-%c%c", buffer[6], buffer[7], buffer[4], buffer[5], buffer[2], buffer

[3]);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[2].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(2, 60, "%s", buffer);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[3].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(4, 19, "%s", buffer);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[4].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(4, 48, "%s", buffer);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[11].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(9, 48, "%s", buffer);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[12].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(10, 48, "%s", buffer);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[13].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(17, 11, "%s", buffer);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[14].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(17, 28, "%s", buffer);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[15].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(17, 46, "%s", buffer);



  memset(buffer, 0, 254);

  get_data(buffer, indice, compras_descr[16].fieldname, compras_ptr, compras_head, 

compras_descr);

  mvprintw(17, 66, "%s", buffer);

  memset(buffer, 0, 254);
  get_data(buffer, indice, compras_descr[10].fieldname, compras_ptr, compras_head, compras_descr);
  int blockNum = atoi(buffer);
  
  int height = 5;
  int width = 31;
  int start_y = 9;
  int start_x = 4;

  /* 3. Create a derived window from stdscr */
  // This is functionally very similar to newwin() in this context.
  WINDOW *win = derwin(stdscr, height, width, start_y, start_x);
  if (win == NULL) 
  {
    mvprintw(0, 0, "win is NULL");
    endwin();
    return;
  }
    
  readMemo(memoBuff,"COMPRA.DBT", blockNum);
  wprintw(win,"%s", memoBuff);
  wrefresh(win);

  endwin();



  

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

   init_input_field(&entrada[0], "Cliente: ", 4, false, 0, 0, STRING);
  clear();
  input_fields_loop(entrada, entradas, operaciones);

  ctasctes_ptr = fopen("CTASCTES.DBF", "rb");

  if (ctasctes_ptr == NULL) 
  {

    perror("error abriendo CTASCTES.dbf");

    return;
  }

  store_header_data(ctasctes_head, ctasctes_ptr, 0);
  store_descriptor_data(ctasctes_descr, ctasctes_ptr);

  rightAlign(entrada[0].input_buffer, ctasctes_descr[3].length);

  char *dbfData = (char *)malloc((ctasctes_head[0].record_bytes) * 500); // basically assuming no more than 500 records for 1 

client 

  int i = 0; //originally set to 2
  
  indice = get_index(ctasctes_descr[3].fieldname, entrada[0].input_buffer, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  if(indice ==  -1)
  {
    mvprintw(0, 0, "not found");
    return;
  }
  
  get_data(buffer, indice, ctasctes_descr[4].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(0, 25, "Cliente: %s", buffer);
  
  while(indice < ctasctes_head->nofrecords)
  {


    get_data(buffer, indice, ctasctes_descr[3].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  
    int comparison = strncmp(buffer, entrada[0].input_buffer, 4); //strncmp didn't work inside of the if statement
    
//getch();
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
  
      mvprintw(j, 0, "%.4s ", &dbfData[(current) * ctasctes_head[0].record_bytes] + offset);
      offset += OPERAC;
      printOffset += OPERAC;
      
      mvprintw(j, printOffset, "%c%c/%c%c ",   dbfData[(current) * ctasctes_head[0].record_bytes + offset + 6],
                dbfData[(current) * ctasctes_head[0].record_bytes + offset + 7],
                dbfData[(current) * ctasctes_head[0].record_bytes + offset + 4],
                dbfData[(current) * ctasctes_head[0].record_bytes + offset + 5]);
      offset += FECHA;
      printOffset += FECHA - 2;
      
      //mvprintw(j, printOffset, "%.13s ", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
      offset += FACTUR;
      offset += NROCLI;  // not printed
      offset += DENOMI;  // not printed
  
      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
      offset += ACREE;
      printOffset += ACREE + 1;
  
      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
      offset += DEUDO;
      printOffset += DEUDO  + 1;
  
      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
      offset += ECHEQ;
      printOffset += ECHEQ  + 1;
  
      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
      offset += EEFEC;
      printOffset += EEFEC  + 1;  

      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
      offset += RCHEQ;
      printOffset += RCHEQ  + 1;  

      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * ctasctes_head[0].record_bytes + offset]);
      offset += REFEC;
      printOffset += REFEC  + 1;
      
      current++;
      }

    key = getch();
    
    if(key == KEY_UP && i > scrollSize)
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
    else if(key == KEY_DOWN && i > scrollSize)
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
    
    if(key == KEY_ENTER || key == '\n' || key == PADENTER || key == 27)
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

void ultimas_op_com(){
  header compras_head[1];
  descriptor compras_descr[MAX_DBF_FIELDS];
  InputField entrada[1];
  int entradas = 1;
  FILE *compras_ptr = NULL;
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

   init_input_field(&entrada[0], "Cliente: ", 4, false, 0, 0, STRING);
  input_fields_loop(entrada, entradas, operaciones);

  compras_ptr = fopen("COMPRA.DBF", "rb");

  if (compras_ptr == NULL) 
  {

    perror("error abriendo compras.dbf");

    return;
  }

  store_header_data(compras_head, compras_ptr, 0);
  store_descriptor_data(compras_descr, compras_ptr);

  rightAlign(entrada[0].input_buffer, compras_descr[3].length);

  char *dbfData = (char *)malloc((compras_head[0].record_bytes) * 500); // basically assuming no more than 500 records for 1 

client 

  int i = 0; //originally set to 2
  
  indice = get_index(compras_descr[3].fieldname, entrada[0].input_buffer, compras_ptr, compras_head, compras_descr);
  if(indice ==  -1)
  {
    mvprintw(0, 0, "not found");
    return;
  }
  
  get_data(buffer, indice, compras_descr[4].fieldname, compras_ptr, compras_head, compras_descr);
  mvprintw(0, 25, "Cliente: %s", buffer);
  
  while(indice < compras_head->nofrecords)
  {


    get_data(buffer, indice, compras_descr[3].fieldname, compras_ptr, compras_head, compras_descr);
  
    int comparison = strncmp(buffer, entrada[0].input_buffer, 3); //strncmp didn't work inside of the if statement
    
//getch();
//mvprintw(4, i, "%d %s %s %d", indice, buffer, entrada[0].input_buffer, comparison); 
//used for logging
      
refresh();
    
    if(comparison == 0)
    {
      
      memset(buffer, 0, 254);
      get_record(buffer, indice, compras_ptr, compras_head, compras_descr);
      memcpy(&dbfData[(i) * compras_head[0].record_bytes], buffer, compras_head[0].record_bytes);
      
      i++;
    
    }
    indice++;
  }

    // dbfData[i * compras_head[0].record_bytes + offset] where i are the rows and j are the columns
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
  
      mvprintw(j, 0, "%.6s ", &dbfData[(current) * compras_head[0].record_bytes] + offset);
      offset += compras_descr[0].length; //ORDCOM
      printOffset += compras_descr[0].length;
      
      mvprintw(j, printOffset, "%c%c/%c%c ",   dbfData[(current) * compras_head[0].record_bytes + offset + 6],
                dbfData[(current) * compras_head[0].record_bytes + offset + 7],
                dbfData[(current) * compras_head[0].record_bytes + offset + 4],
                dbfData[(current) * compras_head[0].record_bytes + offset + 5]);
      offset += compras_descr[1].length; //FECHA
      printOffset += compras_descr[1].length - 2;
      
      //mvprintw(j, printOffset, "%.13s ", &dbfData[(current) * compras_head[0].record_bytes + offset]);
      offset += compras_descr[2].length;  // NROFAC
      offset += compras_descr[3].length;  // NROPRO
      offset += compras_descr[4].length;  // NOMBRE
      offset += compras_descr[5].length;  // DIRECC
      offset += compras_descr[6].length;  // LOCALI
      offset += compras_descr[7].length;  // CPOSTA
      offset += compras_descr[8].length;  // TELEF1
      offset += compras_descr[9].length;  // TELEF2
      offset += compras_descr[10].length; // MEMO
  
      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * compras_head[0].record_bytes + offset]);
      offset += compras_descr[11].length;  // PACREE
      printOffset += compras_descr[11].length + 1;
  
      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * compras_head[0].record_bytes + offset]);
      offset += compras_descr[12].length;  // PDEUDO
      printOffset += compras_descr[12].length + 1;
  
      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * compras_head[0].record_bytes + offset]);
      offset += compras_descr[13].length;
      printOffset += compras_descr[13].length + 1;
  
      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * compras_head[0].record_bytes + offset]);
      offset += compras_descr[14].length;
      printOffset += compras_descr[14].length + 1;  

      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * compras_head[0].record_bytes + offset]);
      offset += compras_descr[15].length;
      printOffset += compras_descr[15].length + 1;  

      mvprintw(j, printOffset, "%.10s ", &dbfData[(current) * compras_head[0].record_bytes + offset]);
      offset += compras_descr[16].length;
      printOffset += compras_descr[16].length + 1;
      
      current++;
      }

    key = getch();
    
    if(key == KEY_UP && i > scrollSize)
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
    else if(key == KEY_DOWN && i > scrollSize)
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
    
    if(key == KEY_ENTER || key == '\n' || key == PADENTER || key == 27)
    {
      break;
    }
  

  }
  if (fclose(compras_ptr) != 0) 
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
  int err = 0;
  //file names

  char cuentasCtes[15] = "CTASCTES.DBF";
  char cliPro[15] = "CLIPRO.DBF";

  //file pointers

  FILE *ctasctes_ptr = NULL;
  header ctasctes_head[1];
  descriptor ctasctes_descr[15]; // I chose 15 because i Know the file I will be opening has less than 15 fields,
  FILE *cli_ptr = NULL;
  header cli_head[1];
  descriptor cli_descr[20]; // MAKE SURE YOU DECLARE MORE DESCRIPTORS THAN WHAT'S ON THE DBF FILE, IF YOU DON'T, IT WILL BE 

THE END OF THE WORLD
  
  //variables and structs related to CTASCTES.dbf
  
  InputField cabecera[4];
  int cabeceras = 4;
  InputField operacion[6];
  int noperaciones = 6;
  char lastOp[4 + 1] = {0};
  char buffer[118 + 50] = {0};
  char memo [10 + 1] = "          ";
  int opint[6] = {0};
  
  //Variables related to CLIPRO.dbf
  
  int cliente = 0;
  char nombreCli[35] = {0};
  char cliAcr[35] = {0};
  char cliDeu[35] = {0};
  char saldo[35] = {0};

  //memo variables
  
  char memoBuff[513] = {0};

  //other

  int repeat = 0;
  int exit = 0;

  //statements 

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
  snprintf(lastOp, 5,"%d", iLastOp); //I wasn't aware snprintf truncated the input to ensure null termination so size has to 

be 5
  //getch();

  do
  {
    repeat = 0;
    
    init_input_field(&cabecera[0], "Operacion:[", ctasctes_descr[0].length , false, 5, 2, INTEGER); //automatically write 

last op. + 1
    init_input_field(&cabecera[1], "Fecha: ", ctasctes_descr[1].length, false, 28, 2, DATE); //should make a date parser
    init_input_field(&cabecera[2], "factura no.:", ctasctes_descr[2].length, false, 46, 2, STRING); 
    init_input_field(&cabecera[3], "cliente: ", ctasctes_descr[3].length, false, 8, 4, INTEGER);


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(cabecera[1].input_buffer, 9, "%02d%02d%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    cabecera[1].count = ctasctes_descr[1].length;
    cabecera[1].cursor_pos = cabecera[1].count;

    memcpy(cabecera[0].input_buffer, lastOp, ctasctes_descr[0].length);
    cabecera[0].count = ctasctes_descr[0].length;
    cabecera[0].cursor_pos = cabecera[0].count;

    rightAlign(cabecera[0].input_buffer, ctasctes_descr[0].length);

    
    operaciones();
    input_fields_loop(cabecera, cabeceras, operaciones);

    rightAlign(cabecera[3].input_buffer, ctasctes_descr[3].length);
    exit = strncmp(cabecera[3].input_buffer, "   0", ctasctes_descr[3].length);
    mvprintw(2, 2, "1%d1", exit);

    if (exit == 0)
    {
      free(cabecera[0].prompt);
      free(cabecera[1].prompt);
      free(cabecera[2].prompt);
      free(cabecera[3].prompt);
      return;
    }

    if(strncmp(cabecera[3].input_buffer, "    ", ctasctes_descr[3].length) == 0)
    {
      free(cabecera[0].prompt);
      free(cabecera[1].prompt);
      free(cabecera[2].prompt);
      free(cabecera[3].prompt);
      repeat = 1;
    }
  } while(repeat);

  //gets client data and print it on the screen

  cli_ptr = fopen(cliPro, "r+b");

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
  if(cliente ==  -1)
  {
    mvprintw(0, 0, "No hay cliente registrado con ese nombre");
    getch();
    return;
  }

  
  get_data(nombreCli, cliente, cli_descr[1].fieldname, cli_ptr, cli_head, cli_descr);
  get_data(cliDeu, cliente, cli_descr[16].fieldname, cli_ptr, cli_head, cli_descr);
  get_data(cliAcr, cliente, cli_descr[17].fieldname, cli_ptr, cli_head, cli_descr);

//cliDeu and cliAcr Should look like something between X.XX or XXXXXXXXX.XX X being a digit

  
  subFields(saldo, cliDeu, cliAcr);
  rightAlign(saldo, cli_descr[16].length);
  
  mvprintw(13, 51, "%s", cliAcr);
  mvprintw(14, 51, "%s", cliDeu);
  mvprintw(4, 48, "%.23s", nombreCli);

  if(atonum(cliAcr) > atonum(cliDeu))
  {
    mvprintw(13, 64, "%s", saldo);//es una cadena
  }
  else
  { 
    mvprintw(14, 64, "%s", saldo);//es una cadena 
  }

  init_input_field(&operacion[0], "Credito ", ctasctes_descr[5].length , false, 39, 9, FLOAT);
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
    if(cabecera[i].type == STRING)
    {
      spaceFill(cabecera[i].input_buffer, ctasctes_descr[i].length);
    }
    else if(cabecera[i].type == DATE)
    {
      dateFormatter(cabecera[i].input_buffer);
    }
    else
    {
      rightAlign(cabecera[i].input_buffer, ctasctes_descr[i].length);
    }
  }
  
    for(int i = 0; i < 6; i++)
  {
    if(operacion[i].type == STRING)
    {
      spaceFill(operacion[i].input_buffer, ctasctes_descr[i + 5].length);
    }
    else
    {
      if(operacion[i].type == FLOAT)
      {
        addDecimals(operacion[i].input_buffer, ctasctes_descr[i + 5].length, ctasctes_descr[i + 5].decimal);
      }
      rightAlign(operacion[i].input_buffer, ctasctes_descr[i + 5].length);
      opint[i] = atonum(operacion[i].input_buffer);
    }
  }

  int off = 0;
  for(int i = 0; i < 4; i++)
  {
    memcpy(&buffer[off], cabecera[i].input_buffer, ctasctes_descr[i].length);
    off += ctasctes_descr[i].length;
  }

  memcpy(&buffer[off], nombreCli, ctasctes_descr[4].length); //Name of the client
  off += ctasctes_descr[4].length;

  for(int i = 0; i < 6; i++)
  {
    memcpy(&buffer[off], operacion[i].input_buffer, ctasctes_descr[i+5].length);
    off += ctasctes_descr[i+5].length;
  }

  
  //Now update the values of "acreedor" and "deudor" (Edit now testing this functions)
  //This is slow i should make a function That sums everything in  
  //REVISAR
  //also should make a function for this to make it more legible

  sumFields(cliAcr, operacion[0].input_buffer, cliAcr); //acreedor anterior + credito
  subFields(cliAcr, operacion[2].input_buffer, cliAcr); //-entrego cheque
  subFields(cliAcr, operacion[3].input_buffer, cliAcr); //-entrego efectivo
  sumFields(cliDeu, operacion[1].input_buffer, cliDeu);
  subFields(cliDeu, operacion[4].input_buffer, cliDeu);
  subFields(cliDeu, operacion[5].input_buffer, cliDeu);

  if(atonum(cliAcr) < 0)
  {
    subFields(cliDeu, cliAcr, cliDeu);
    subFields(cliAcr, cliAcr, cliAcr);
  }
  if(atonum(cliDeu) < 0)
  {
    subFields(cliAcr, cliDeu, cliAcr);
    subFields(cliDeu, cliDeu, cliDeu);
  }

  if(atonum(cliAcr) > atonum(cliDeu))
  {
    subFields(saldo, cliDeu, cliAcr);  

    rightAlign(cliDeu, cli_descr[16].length);
    rightAlign(cliAcr, cli_descr[17].length);
    rightAlign(saldo, cli_descr[16].length);

    mvprintw(13, 51, "           ");
    mvprintw(14, 51, "           ");
    mvprintw(14, 64, "           ");
  
    mvprintw(13, 51, "%s", cliAcr);
    mvprintw(14, 51, "%s", cliDeu);
    mvprintw(13, 64, "%s", saldo);//It is a string now
  }
  else if(atonum(cliAcr) < atonum(cliDeu))
  {
    subFields(saldo, cliAcr, cliDeu);
  
    rightAlign(cliDeu, cli_descr[16].length);
    rightAlign(cliAcr, cli_descr[17].length);
    rightAlign(saldo, cli_descr[16].length);

    mvprintw(13, 51, "           ");
    mvprintw(14, 51, "           ");
    mvprintw(14, 64, "           ");
  
    mvprintw(13, 51, "%s", cliAcr);
    mvprintw(14, 51, "%s", cliDeu);
    mvprintw(14, 64, "%s", saldo);//It is a string now
  }


  //iCliAcr = iCliAcr + opint[0] - opint[2] - opint[3];
  //iCliDeu = iCliDeu + opint[1] - opint[4] - opint[5];

  //Added Memo functionality

  int check = yesNoMenu("CONFIRMA", 19, 30, "REPITE", 19, 45);
  if(check == 1) 
  { 
    int block = 0;
    memoTextbox(memoBuff);
    if(isMemoEmpty(memoBuff) == 0)
    {
      block = addMemo("CTASCTES.DBT",memoBuff);

      snprintf(memo, 5, "%d", block);
      rightAlign(memo, ctasctes_descr[11].length);
    }
    memcpy(&buffer[off], memo, ctasctes_descr[11].length);
    off += ctasctes_descr[11].length;

    buffer[off] = 0x1A; //the last byte of buffer should be 0x1A since it is the .dbf EOF marker
    addRecord(buffer, cuentasCtes, 118);

    rightAlign(cliDeu, cli_descr[16].length);
    rightAlign(cliAcr, cli_descr[17].length);

    replaceField(cliAcr, cliente, "ACREEDOR", cli_ptr, cli_head, cli_descr);
    replaceField(cliDeu, cliente, "DEUDOR", cli_ptr, cli_head, cli_descr);
  }
  
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

void agregarOrdCom()
{ 
  int err = 0;
  //file names

  char compras[15] = "COMPRA.DBF";
  char prove[15] = "PROVE.DBF";

  //file pointers

  FILE *compra_ptr = NULL;
  header compra_head[1];
  descriptor compra_descr[20]; // I chose 15 because i Know the file I will be opening has less than 15 fields,
  FILE *pro_ptr = NULL;
  header pro_head[1];
  descriptor pro_descr[20]; // MAKE SURE YOU DECLARE MORE DESCRIPTORS THAN WHAT'S ON THE DBF FILE, IF YOU DON'T, IT WILL BE 

THE END OF THE WORLD
  
  //variables and structs related to COMPRA.dbf
  
  InputField cabecera[4];
  int cabeceras = 4;
  InputField operacion[6];
  int noperaciones = 6;
  char lastOp[6 + 1] = {0};
  char buffer[190 + 50] = {0};
  char memo [10 + 1] = "          ";
  int opint[6] = {0};
  
  //Variables related to PROVE.dbf
  
  int proveedor = 0;
  char nombreProv[35] = {0};
  char direccion[35] = {0};
  char localidad[35] = {0};
  char cPostal[35] = {0};
  char tel1[35] = {0};
  char tel2[35] = {0};
  char cliAcr[35] = {0};
  char cliDeu[35] = {0};
  char saldo[35] = {0};

  //memo variables
  
  char memoBuff[513] = {0};

  //other

  int repeat = 0;
  int exit = 0;

  //statements 

  compra_ptr = fopen(compras, "rb");
  if (compra_ptr == NULL) 
  {
    mvprintw(0, 0, "Error abriendo COMPRA.dbf: %s", strerror(errno));
    mvprintw(1, 0, "Presione cualquier tecla para salir...");
    refresh();
    getch();
    return;
  }
  
  store_header_data(compra_head, compra_ptr, 0);
  store_descriptor_data(compra_descr, compra_ptr);

  fseek(compra_ptr, -compra_head[0].record_bytes, SEEK_END);
  int pos = ftell(compra_ptr);
  fread(lastOp, compra_descr[0].length, 1, compra_ptr);

mvprintw(5, 0, "");
for(int i = 0; i < 10; i++)
{
  printw("%x ", lastOp[i]);
} 

  if (fclose(compra_ptr) != 0) 
  {
    mvprintw(0, 0, "Error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }

  int iLastOp = atoi(lastOp);
  iLastOp++;
  memset(lastOp, 0, compra_descr[0].length); //in agregarctacte the lastOp field is 4 bytes here it is 6 because garbage data 
  snprintf(lastOp, 7,"%d", iLastOp); //I wasn't aware snprintf truncated the input to ensure null termination so size has to 

be 7
  //getch();

  do
  {
    repeat = 0;
    
    init_input_field(&cabecera[0], "OrdCom:[", compra_descr[0].length , false, 5, 2, INTEGER); //automatically write last op. 

+ 1
    init_input_field(&cabecera[1], "Fecha: ", compra_descr[1].length, false, 28, 2, DATE); //should make a date parser
    init_input_field(&cabecera[2], "factura no.:", compra_descr[2].length, false, 46, 2, STRING); 
    init_input_field(&cabecera[3], "proveedor: ", compra_descr[3].length, false, 8, 4, INTEGER);

    operaciones();

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(cabecera[1].input_buffer, 9, "%02d%02d%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    cabecera[1].count = compra_descr[1].length;
    cabecera[1].cursor_pos = cabecera[1].count;

    memcpy(cabecera[0].input_buffer, lastOp, compra_descr[0].length);
    cabecera[0].count = compra_descr[0].length;
    cabecera[0].cursor_pos = cabecera[0].count;

    rightAlign(cabecera[0].input_buffer, compra_descr[0].length);

    

    input_fields_loop(cabecera, cabeceras, operaciones);

    rightAlign(cabecera[3].input_buffer, compra_descr[3].length);
    exit = strncmp(cabecera[3].input_buffer, "  0", compra_descr[3].length);
    mvprintw(2, 2, "1%d1", exit);

    if (exit == 0)
    {
      free(cabecera[0].prompt);
      free(cabecera[1].prompt);
      free(cabecera[2].prompt);
      free(cabecera[3].prompt);
      return;
    }

    if(strncmp(cabecera[3].input_buffer, "      ", compra_descr[3].length) == 0)
    {
      free(cabecera[0].prompt);
      free(cabecera[1].prompt);
      free(cabecera[2].prompt);
      free(cabecera[3].prompt);
      repeat = 1;
    }
  } while(repeat);

  //gets client data and print it on the screen

  pro_ptr = fopen(prove, "r+b");

  if (pro_ptr == NULL) 
  {
    mvprintw(0, 0, "Error abriendo CLIPRO.dbf: %s", strerror(errno));
    mvprintw(1, 0, "Presione cualquier tecla para salir...");
    refresh();
    getch();
    return;
  }

  store_header_data(pro_head, pro_ptr, 0);
  store_descriptor_data(pro_descr, pro_ptr);

  char provid[6] = {0};
  memcpy(provid, cabecera[3].input_buffer, compra_descr[0].length); 

  rightAlign(provid, pro_descr[0].length);

  proveedor = get_index(pro_descr[0].fieldname, provid, pro_ptr, pro_head, pro_descr);
  if(proveedor ==  -1)
  {
    mvprintw(0, 0, "No hay proveedor registrado con ese nombre");
    getch();
    return;
  }

  
  get_data(nombreProv, proveedor, pro_descr[1].fieldname, pro_ptr, pro_head, pro_descr);
  spaceFill(nombreProv, pro_descr[1].length);
//mvprintw(0, 0, "%s", nombreProv);
  get_data(direccion, proveedor, pro_descr[4].fieldname, pro_ptr, pro_head, pro_descr);
  spaceFill(direccion, pro_descr[5].length);
//printw("\n%s", direccion);
  get_data(localidad, proveedor, pro_descr[5].fieldname, pro_ptr, pro_head, pro_descr);
  spaceFill(localidad, pro_descr[6].length);
//printw("\n%s", localidad);
  get_data(cPostal, proveedor, pro_descr[7].fieldname, pro_ptr, pro_head, pro_descr);
  spaceFill(cPostal, pro_descr[7].length);
//printw("\n%s", cPostal);
  get_data(tel1, proveedor, pro_descr[10].fieldname, pro_ptr, pro_head, pro_descr);
  spaceFill(tel1, pro_descr[10].length);
//printw("\n%s", tel1);
  get_data(tel2, proveedor, pro_descr[11].fieldname, pro_ptr, pro_head, pro_descr);
  spaceFill(tel2, pro_descr[11].length);
//printw("\n%s", tel2);
  get_data(cliDeu, proveedor, pro_descr[13].fieldname, pro_ptr, pro_head, pro_descr);
  get_data(cliAcr, proveedor, pro_descr[12].fieldname, pro_ptr, pro_head, pro_descr);

//cliDeu and cliAcr Should look like something between X.XX or XXXXXXXXX.XX X being a digit

  
  subFields(saldo, cliDeu, cliAcr);
  rightAlign(saldo, pro_descr[12].length);
  
  mvprintw(13, 51, "%s", cliAcr);
  mvprintw(14, 51, "%s", cliDeu);
  mvprintw(4, 48, "%.23s", nombreProv);

  if(atonum(cliAcr) > atonum(cliDeu))
  {
    mvprintw(13, 64, "%s", saldo);//es una cadena
  }
  else
  { 
    mvprintw(14, 64, "%s", saldo);//es una cadena 
  }

  init_input_field(&operacion[0], "Credito ", compra_descr[11].length , false, 39, 9, FLOAT);
  init_input_field(&operacion[1], "Factura$", compra_descr[12].length, false, 39, 10, FLOAT);
  init_input_field(&operacion[2], "Cheque", compra_descr[13].length, false, 4, 17, FLOAT); 
  init_input_field(&operacion[3], "Efec.", compra_descr[14].length, false, 22, 17, FLOAT);
  init_input_field(&operacion[4], "Cheque", compra_descr[15].length, false, 39, 17, FLOAT); 
  init_input_field(&operacion[5], "Efectivo", compra_descr[16].length, false, 57, 17, FLOAT);

refresh();

  input_fields_loop(operacion, noperaciones, operaciones);

  //this sets up the fields (right aligns integers and floats, fills null characters in strings with empty spaces)

  for(int i = 0; i < 4; i++)
  {
    if(cabecera[i].type == STRING)
    {
      spaceFill(cabecera[i].input_buffer, compra_descr[i].length);
    }
    else if(cabecera[i].type == DATE)
    {
      dateFormatter(cabecera[i].input_buffer);
    }
    else
    {
      rightAlign(cabecera[i].input_buffer, compra_descr[i].length);
    }
  }
  
    for(int i = 0; i < 6; i++)
  {
    if(operacion[i].type == STRING)
    {
      spaceFill(operacion[i].input_buffer, compra_descr[i + 11].length);
    }
    else
    {
      if(operacion[i].type == FLOAT)
      {
        addDecimals(operacion[i].input_buffer, compra_descr[i + 11].length, compra_descr[i + 11].decimal);
      }
      rightAlign(operacion[i].input_buffer, compra_descr[i + 11].length);
      opint[i] = atonum(operacion[i].input_buffer);
    }
  }

  int off = 0;
  for(int i = 0; i < 4; i++)
  {
    memcpy(&buffer[off], cabecera[i].input_buffer, compra_descr[i].length);
    off += compra_descr[i].length;
  }

  memcpy(&buffer[off], nombreProv, compra_descr[4].length); //Name of the client
  off += compra_descr[4].length;

  memcpy(&buffer[off], direccion, compra_descr[5].length); //Name of the client
  off += compra_descr[5].length;

  memcpy(&buffer[off], localidad, compra_descr[6].length); //Name of the client
  off += compra_descr[6].length;

  memcpy(&buffer[off], cPostal, compra_descr[7].length); //Name of the client
  off += compra_descr[7].length;

  memcpy(&buffer[off], tel1, compra_descr[8].length); //Name of the client
  off += compra_descr[8].length;

  memcpy(&buffer[off], tel2, compra_descr[9].length); //Name of the client
  off += compra_descr[9].length;

  int memoOff = off;

  //memcpy(&buffer[off], nombreProv, compra_descr[10].length); //Name of the client
  off += compra_descr[10].length;

  for(int i = 0; i < 6; i++)
  {
    memcpy(&buffer[off], operacion[i].input_buffer, compra_descr[i+11].length);
    off += compra_descr[i+11].length;
  }

  
  //Now update the values of "acreedor" and "deudor" (Edit now testing this functions)
  //This is slow i should make a function That sums everything in  
  //REVISAR
  //also should make a function for this to make it more legible

  sumFields(cliAcr, operacion[0].input_buffer, cliAcr); //acreedor anterior + credito
  subFields(cliAcr, operacion[2].input_buffer, cliAcr); //-entrego cheque
  subFields(cliAcr, operacion[3].input_buffer, cliAcr); //-entrego efectivo
  sumFields(cliDeu, operacion[1].input_buffer, cliDeu);
  subFields(cliDeu, operacion[4].input_buffer, cliDeu);
  subFields(cliDeu, operacion[5].input_buffer, cliDeu);
mvprintw(0, 0, "%s\n%s", cliAcr, cliDeu);

  if(atonum(cliAcr) < 0)
  {
    subFields(cliDeu, cliAcr, cliDeu);
    subFields(cliAcr, cliAcr, cliAcr);
  }
  if(atonum(cliDeu) < 0)
  {
    subFields(cliAcr, cliDeu, cliAcr);
    subFields(cliDeu, cliDeu, cliDeu);
  }
mvprintw(2, 0, "%s\n%s", cliAcr, cliDeu);
  if(atonum(cliAcr) > atonum(cliDeu))
  {
    subFields(saldo, cliDeu, cliAcr);  

    rightAlign(cliDeu, pro_descr[12].length);
    rightAlign(cliAcr, pro_descr[13].length);
    rightAlign(saldo, pro_descr[12].length);

    mvprintw(13, 51, "           ");
    mvprintw(14, 51, "           ");
    mvprintw(14, 64, "           ");
  
    mvprintw(13, 51, "%s", cliAcr);
    mvprintw(14, 51, "%s", cliDeu);
    mvprintw(13, 64, "%s", saldo);//It is a string now
  }
  else if(atonum(cliAcr) < atonum(cliDeu))
  {
    subFields(saldo, cliAcr, cliDeu);
  
    rightAlign(cliDeu, pro_descr[12].length);
    rightAlign(cliAcr, pro_descr[13].length);
    rightAlign(saldo, pro_descr[12].length);

    mvprintw(13, 51, "           ");
    mvprintw(14, 51, "           ");
    mvprintw(14, 64, "           ");
  
    mvprintw(13, 51, "%s", cliAcr);
    mvprintw(14, 51, "%s", cliDeu);
    mvprintw(14, 64, "%s", saldo);//It is a string now
  }


  //iCliAcr = iCliAcr + opint[0] - opint[2] - opint[3];
  //iCliDeu = iCliDeu + opint[1] - opint[4] - opint[5];

  //Added Memo functionality

  int check = yesNoMenu("CONFIRMA", 19, 30, "REPITE", 19, 45);
  if(check == 1) 
  { 
    int block = 0;
    memoTextbox(memoBuff);

    if(isMemoEmpty(memoBuff) == 0)
    {
      block = addMemo("COMPRA.DBT",memoBuff);

      snprintf(memo, 5, "%d", block);
      rightAlign(memo, compra_descr[11].length);
    }

    memcpy(&buffer[memoOff], memo, compra_descr[11].length);

    buffer[off] = 0x1A; //the last byte of buffer should be 0x1A since it is the .dbf EOF marker

    addRecord(buffer, compras, 190);

    rightAlign(cliDeu, pro_descr[13].length);
    rightAlign(cliAcr, pro_descr[12].length);

    replaceField(cliAcr, proveedor, "SGACRE", pro_ptr, pro_head, pro_descr);
    replaceField(cliDeu, proveedor, "SGDEUD", pro_ptr, pro_head, pro_descr);

  }

mvprintw(0, 0, "%s\n%s %x %s", buffer, cabecera[3].input_buffer, pos, lastOp);
mvprintw(20, 0, "%s", cliAcr);
printw("\n%s", cliDeu);
printw("\n%s", saldo);
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

  if (fclose(pro_ptr) != 0) 
  {
    mvprintw(0, 0, "Error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }
}

void modCtacte()
{
  int err = 0;

  //file names

  char cuentasCtes[15] = "CTASCTES.DBF";
  char cliPro[15] = "CLIPRO.DBF";

  //file pointers
  FILE *ctasctes_ptr = NULL;
  header ctasctes_head[1];
  descriptor ctasctes_descr[MAX_DBF_FIELDS];
  FILE *cli_ptr = NULL;
  header cli_head[1];
  descriptor cli_descr[20];

  //variables


  InputField entrada[1];
  int entradas = 1;
  int indice = 0;
  char buffer[MAX_FIELD_LENGTH + 1] = {0};
  char memoBuff[MEMOBLOCKSIZE + 1] = {0};
  int yesNo = 0;
  
  InputField operacion[6];

  //Variables related to CLIPRO.dbf
 
  char clinum[10] = {0}; 
  int cliente = 0;
  char cliAcr[35] = {0};
  char cliDeu[35] = {0};
  char saldo[35] = {0};

  //this is the same as consulta_operacion, i'm selecting the op number to modify

  init_input_field(&entrada[0], "Operacion: ", 4, false, 5, 2, STRING);

  input_fields_loop(entrada, entradas, NULL);

  operaciones();

  refresh();

  ctasctes_ptr = fopen("CTASCTES.DBF", "rb");
  
  if (ctasctes_ptr == NULL) 
  {
    mvprintw(0, 0, "error abriendo CTASCTES.dbf: %s", strerror(errno));
    getch();
    return;
  }

  store_header_data(ctasctes_head, ctasctes_ptr, 0);
  store_descriptor_data(ctasctes_descr, ctasctes_ptr);

  rightAlign(entrada[0].input_buffer, ctasctes_descr[0].length);

  indice = get_index("OPERAC", entrada[0].input_buffer, ctasctes_ptr, ctasctes_head, ctasctes_descr);
mvprintw(0, 0, "indice = %d", indice);
  if(indice ==  -1)
  {
    mvprintw(0, 0, "not found");
    return;
  }

  //I init the input fields and copy the values of the fields into input_buffer 

  init_input_field(&operacion[0], "Credito ", ctasctes_descr[5].length , false, 39, 9, FLOAT);
  init_input_field(&operacion[1], "Factura$", ctasctes_descr[6].length, false, 39, 10, FLOAT);
  init_input_field(&operacion[2], "Cheque", ctasctes_descr[7].length, false, 4, 17, FLOAT); 
  init_input_field(&operacion[3], "Efec.", ctasctes_descr[8].length, false, 22, 17, FLOAT);
  init_input_field(&operacion[4], "Cheque", ctasctes_descr[9].length, false, 39, 17, FLOAT); 
  init_input_field(&operacion[5], "Efectivo", ctasctes_descr[10].length, false, 57, 17, FLOAT);

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

//copy paste of code to get client data

  cli_ptr = fopen(cliPro, "r+b");

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

  rightAlign(buffer, ctasctes_descr[3].length);

  cliente = get_index(cli_descr[0].fieldname, buffer, cli_ptr, cli_head, cli_descr);
  if(cliente ==  -1)
  {
    mvprintw(0, 0, "No hay cliente registrado con ese nombre");
    getch();
    return;
  }
  
  get_data(cliDeu, cliente, cli_descr[16].fieldname, cli_ptr, cli_head, cli_descr);
  get_data(cliAcr, cliente, cli_descr[17].fieldname, cli_ptr, cli_head, cli_descr);

//cliDeu and cliAcr Should look like something between X.XX or XXXXXXXXX.XX X being a digit

  
  subFields(saldo, cliDeu, cliAcr);
  rightAlign(saldo, cli_descr[16].length);
  
  mvprintw(13, 51, "%s", cliAcr);
  mvprintw(14, 51, "%s", cliDeu);

  if(atonum(cliAcr) > atonum(cliDeu))
  {
    mvprintw(13, 64, "%s", saldo);//es una cadena
  }
  else
  { 
    mvprintw(14, 64, "%s", saldo);//es una cadena 
  }

//end copy paste

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[4].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  mvprintw(4, 48, "%s", buffer);
  
  for(int i = 0; i < 6; i++)
  {
    memset(buffer, 0, MAX_FIELD_LENGTH + 1);
    get_data(buffer, indice, ctasctes_descr[5 + i].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
    memcpy(operacion[i].input_buffer, buffer, ctasctes_descr[5 + i].length);
    operacion[i].count = ctasctes_descr[5 + i].length;
    operacion[i].cursor_pos = operacion[i].count;
  }

  if (fclose(ctasctes_ptr) != 0) 
  {
    mvprintw(0,0,"error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }

  subFields(cliAcr, operacion[0].input_buffer, cliAcr); //acreedor anterior + credito
  sumFields(cliAcr, operacion[2].input_buffer, cliAcr); //-entrego cheque
  sumFields(cliAcr, operacion[3].input_buffer, cliAcr); //-entrego efectivo
  subFields(cliDeu, operacion[1].input_buffer, cliDeu);
  sumFields(cliDeu, operacion[4].input_buffer, cliDeu);
  sumFields(cliDeu, operacion[5].input_buffer, cliDeu);

  input_fields_loop(operacion, 6, NULL);

  for(int i = 0; i < 6; i++)
  {
    if(operacion[i].type == STRING)
    {
      spaceFill(operacion[i].input_buffer, ctasctes_descr[i + 5].length);
    }
    else
    {
      if(operacion[i].type == FLOAT)
      {
        addDecimals(operacion[i].input_buffer, ctasctes_descr[i + 5].length, ctasctes_descr[i + 5].decimal);
      }
      rightAlign(operacion[i].input_buffer, ctasctes_descr[i + 5].length);
    }
  }

  sumFields(cliAcr, operacion[0].input_buffer, cliAcr); //acreedor anterior + credito
  subFields(cliAcr, operacion[2].input_buffer, cliAcr); //-entrego cheque
  subFields(cliAcr, operacion[3].input_buffer, cliAcr); //-entrego efectivo
  sumFields(cliDeu, operacion[1].input_buffer, cliDeu);
  subFields(cliDeu, operacion[4].input_buffer, cliDeu);
  subFields(cliDeu, operacion[5].input_buffer, cliDeu);

  if(atonum(cliAcr) < 0)
  {
    subFields(cliDeu, cliAcr, cliDeu);
    subFields(cliAcr, cliAcr, cliAcr);
  }
  if(atonum(cliDeu) < 0)
  {
    subFields(cliAcr, cliDeu, cliAcr);
    subFields(cliDeu, cliDeu, cliDeu);
  }

  if(atonum(cliAcr) > atonum(cliDeu))
  {
    subFields(saldo, cliDeu, cliAcr);  

    rightAlign(cliDeu, cli_descr[16].length);
    rightAlign(cliAcr, cli_descr[17].length);
    rightAlign(saldo, cli_descr[16].length);

    mvprintw(13, 51, "           ");
    mvprintw(14, 51, "           ");
    mvprintw(14, 64, "           ");
  
    mvprintw(13, 51, "%s", cliAcr);
    mvprintw(14, 51, "%s", cliDeu);
    mvprintw(13, 64, "%s", saldo);//It is a string now
  }
  else if(atonum(cliAcr) < atonum(cliDeu))
  {
    subFields(saldo, cliAcr, cliDeu);
  
    rightAlign(cliDeu, cli_descr[16].length);
    rightAlign(cliAcr, cli_descr[17].length);
    rightAlign(saldo, cli_descr[16].length);

    mvprintw(13, 51, "           ");
    mvprintw(14, 51, "           ");
    mvprintw(14, 64, "           ");
  
    mvprintw(13, 51, "%s", cliAcr);
    mvprintw(14, 51, "%s", cliDeu);
    mvprintw(14, 64, "%s", saldo);//It is a string now
  }

    ctasctes_ptr = fopen("CTASCTES.DBF", "r+b");
    if (ctasctes_ptr == NULL) 
    {
      mvprintw(0, 0, "error abriendo CTASCTES.dbf: %s", strerror(errno));
      getch();
      return;
    }

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, ctasctes_descr[11].fieldname, ctasctes_ptr, ctasctes_head, ctasctes_descr);
  int blockNum = atoi(buffer);

  readMemo(memoBuff,"CTASCTES.DBT", blockNum);

  yesNo = yesNoMenu("CONFIRMA", 19, 30, "REPITE", 19, 45);
  if(yesNo == 1) 
  { 
    edMemoTextbox(memoBuff); // To be changed for something better, but an easy solution for the time being

    replaceMemo("CTASCTES.DBT",memoBuff, blockNum);

    for(int i = 0; i < 6; i++)
    {
      replaceField(operacion[i].input_buffer, indice, ctasctes_descr[5 + i].fieldname, ctasctes_ptr, ctasctes_head, 

ctasctes_descr);
    }

    rightAlign(cliDeu, cli_descr[16].length);
    rightAlign(cliAcr, cli_descr[17].length);

    err = replaceField(cliAcr, cliente, "ACREEDOR", cli_ptr, cli_head, cli_descr);
    err = replaceField(cliDeu, cliente, "DEUDOR", cli_ptr, cli_head, cli_descr);

  }
  else
  {
    return;
  }
  
  free(entrada[0].prompt);
  if (fclose(cli_ptr) != 0) 
  {
    mvprintw(0,0,"error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }
  if (fclose(ctasctes_ptr) != 0) 
  {
    mvprintw(0,0,"error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }

  getch();
  return;
}

void modCom()
{

  int err = 0;
  //file names

  char compras[15] = "COMPRA.DBF";
  char prove[15] = "PROVE.DBF";

  //file pointers

  FILE *compra_ptr = NULL;
  header compra_head[1];
  descriptor compra_descr[20]; // I chose 15 because i Know the file I will be opening has less than 15 fields,
  FILE *pro_ptr = NULL;
  header pro_head[1];
  descriptor pro_descr[20];
  
  //variables and structs related to COMPRA.dbf
  
  InputField entrada[1];
  int entradas = 1;
  int indice = 0;
  InputField operacion[6];
  int noperaciones = 6;
  char buffer[MAX_FIELD_LENGTH + 1] = {0};
  char memoBuff[MEMOBLOCKSIZE + 1] = {0};
  int yesNo = 0;

  //Variables related to PROVE.dbf
 
  char proNum[10] = {0}; 
  int Proveedor = 0;
  char cliAcr[35] = {0};
  char cliDeu[35] = {0};
  char saldo[35] = {0};

  //this is the same as consulta_operacion, i'm selecting the op number to modify

  init_input_field(&entrada[0], "Operacion: ", 6, false, 5, 2, STRING);

  input_fields_loop(entrada, entradas, NULL);

  operaciones();

  refresh();

  compra_ptr = fopen(compras, "rb");
  
  if (compra_ptr == NULL) 
  {
    mvprintw(0, 0, "error abriendo COMPRA.dbf: %s", strerror(errno));
    getch();
    return;
  }

  store_header_data(compra_head, compra_ptr, 0);
  store_descriptor_data(compra_descr, compra_ptr);

  rightAlign(entrada[0].input_buffer, compra_descr[0].length);

  indice = get_index("ORDCOM", entrada[0].input_buffer, compra_ptr, compra_head, compra_descr);
  if(indice ==  -1)
  {
    mvprintw(0, 0, "Registro no encontrado");
    getch();
    return;
  }

  //I init the input fields and copy the values of the fields into input_buffer 

  init_input_field(&operacion[0], "Credito ", compra_descr[11].length , false, 39, 9, FLOAT);
  init_input_field(&operacion[1], "Factura$", compra_descr[12].length, false, 39, 10, FLOAT);
  init_input_field(&operacion[2], "Cheque", compra_descr[13].length, false, 4, 17, FLOAT); 
  init_input_field(&operacion[3], "Efec.", compra_descr[14].length, false, 22, 17, FLOAT);
  init_input_field(&operacion[4], "Cheque", compra_descr[15].length, false, 39, 17, FLOAT); 
  init_input_field(&operacion[5], "Efectivo", compra_descr[16].length, false, 57, 17, FLOAT);

  get_data(buffer, indice, compra_descr[0].fieldname, compra_ptr, compra_head, compra_descr);
  mvprintw(2, 17, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, compra_descr[1].fieldname, compra_ptr, compra_head, compra_descr);
  mvprintw(2, 35, "%c%c-%c%c-%c%c", buffer[6], buffer[7], buffer[4], buffer[5], buffer[2], buffer[3]);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, compra_descr[2].fieldname, compra_ptr, compra_head, compra_descr);
  mvprintw(2, 60, "%s", buffer);

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, compra_descr[3].fieldname, compra_ptr, compra_head, compra_descr);
  mvprintw(4, 19, "%s", buffer);

//copy paste of code to get client data

  pro_ptr = fopen(prove, "r+b");

  if (pro_ptr == NULL) 
  {
    mvprintw(0, 0, "Error abriendo PROVE.dbf: %s", strerror(errno));
    mvprintw(1, 0, "Presione cualquier tecla para salir...");
    refresh();
    getch();
    return;
  }

  store_header_data(pro_head, pro_ptr, 0);
  store_descriptor_data(pro_descr, pro_ptr);

  rightAlign(buffer, pro_descr[0].length);

  Proveedor = get_index(pro_descr[0].fieldname, buffer, pro_ptr, pro_head, pro_descr);
  if(Proveedor ==  -1)
  {
    mvprintw(0, 0, "No hay Proveedor registrado con ese nombre");
    getch();
    return;
  }

  get_data(cliDeu, Proveedor, pro_descr[13].fieldname, pro_ptr, pro_head, pro_descr);
mvprintw(1, 0, "%s %d", cliDeu, Proveedor);

  get_data(cliAcr, Proveedor, pro_descr[12].fieldname, pro_ptr, pro_head, pro_descr);
mvprintw(2, 0, "%s", cliAcr);

//cliDeu and cliAcr Should look like something between X.XX or XXXXXXXXX.XX X being a digit
  rightAlign(cliAcr, pro_descr[12].length);
  rightAlign(cliDeu, pro_descr[12].length);
  
  subFields(saldo, cliDeu, cliAcr);
  rightAlign(saldo, pro_descr[12].length);
  
  mvprintw(13, 51, "%s", cliAcr);
  mvprintw(14, 51, "%s", cliDeu);

  if(atonum(cliAcr) > atonum(cliDeu))
  {
    mvprintw(13, 64, "%s", saldo);//es una cadena
  }
  else
  { 
    mvprintw(14, 64, "%s", saldo);//es una cadena 
  }

//end copy paste

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, compra_descr[4].fieldname, compra_ptr, compra_head, compra_descr);
  mvprintw(4, 48, "%s", buffer);
  
  for(int i = 0; i < 6; i++)
  {
    memset(buffer, 0, MAX_FIELD_LENGTH + 1);
    get_data(buffer, indice, compra_descr[11 + i].fieldname, compra_ptr, compra_head, compra_descr);
    memcpy(operacion[i].input_buffer, buffer, compra_descr[11 + i].length);
    operacion[i].count = compra_descr[11 + i].length;
    operacion[i].cursor_pos = operacion[i].count;
  }

  if (fclose(compra_ptr) != 0) 
  {
    mvprintw(0,0,"error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }

  subFields(cliAcr, operacion[0].input_buffer, cliAcr); //acreedor anterior + credito
  sumFields(cliAcr, operacion[2].input_buffer, cliAcr); //-entrego cheque
  sumFields(cliAcr, operacion[3].input_buffer, cliAcr); //-entrego efectivo
  subFields(cliDeu, operacion[1].input_buffer, cliDeu);
  sumFields(cliDeu, operacion[4].input_buffer, cliDeu);
  sumFields(cliDeu, operacion[5].input_buffer, cliDeu);

  input_fields_loop(operacion, 6, NULL);

  for(int i = 0; i < 6; i++)
  {
    if(operacion[i].type == STRING)
    {
      spaceFill(operacion[i].input_buffer, compra_descr[i + 11].length);
    }
    else
    {
      if(operacion[i].type == FLOAT)
      {
        addDecimals(operacion[i].input_buffer, compra_descr[i + 11].length, compra_descr[i + 11].decimal);
      }
      rightAlign(operacion[i].input_buffer, compra_descr[i + 11].length);
    }
  }

  sumFields(cliAcr, operacion[0].input_buffer, cliAcr); //acreedor anterior + credito
  subFields(cliAcr, operacion[2].input_buffer, cliAcr); //-entrego cheque
  subFields(cliAcr, operacion[3].input_buffer, cliAcr); //-entrego efectivo
  sumFields(cliDeu, operacion[1].input_buffer, cliDeu);
  subFields(cliDeu, operacion[4].input_buffer, cliDeu);
  subFields(cliDeu, operacion[5].input_buffer, cliDeu);

  if(atonum(cliAcr) < 0)
  {
    subFields(cliDeu, cliAcr, cliDeu);
    subFields(cliAcr, cliAcr, cliAcr);
  }
  if(atonum(cliDeu) < 0)
  {
    subFields(cliAcr, cliDeu, cliAcr);
    subFields(cliDeu, cliDeu, cliDeu);
  }

  if(atonum(cliAcr) > atonum(cliDeu))
  {
    subFields(saldo, cliDeu, cliAcr);  

    rightAlign(cliDeu, pro_descr[13].length);
    rightAlign(cliAcr, pro_descr[12].length);
    rightAlign(saldo, pro_descr[13].length);

    mvprintw(13, 51, "           ");
    mvprintw(14, 51, "           ");
    mvprintw(14, 64, "           ");
  
    mvprintw(13, 51, "%s", cliAcr);
    mvprintw(14, 51, "%s", cliDeu);
    mvprintw(13, 64, "%s", saldo);//It is a string now
  }
  else if(atonum(cliAcr) < atonum(cliDeu))
  {
    subFields(saldo, cliAcr, cliDeu);
  
    rightAlign(cliDeu, pro_descr[13].length);
    rightAlign(cliAcr, pro_descr[12].length);
    rightAlign(saldo, pro_descr[13].length);

    mvprintw(13, 51, "           ");
    mvprintw(14, 51, "           ");
    mvprintw(14, 64, "           ");

    mvprintw(13, 51, "%s", cliAcr);
    mvprintw(14, 51, "%s", cliDeu);
    mvprintw(14, 64, "%s", saldo);//It is a string now
  }

    compra_ptr = fopen(compras, "r+b");
    if (compra_ptr == NULL) 
    {
      mvprintw(0, 0, "error abriendo COMPRA.dbf: %s", strerror(errno));
      getch();
      return;
    }

  memset(buffer, 0, MAX_FIELD_LENGTH + 1);
  get_data(buffer, indice, compra_descr[10].fieldname, compra_ptr, compra_head, compra_descr);
  int blockNum = atoi(buffer);

  readMemo(memoBuff,"COMPRA.DBT", blockNum);

  yesNo = yesNoMenu("CONFIRMA", 19, 30, "REPITE", 19, 45);
  if(yesNo == 1) 
  { 
    edMemoTextbox(memoBuff); // To be changed for something better, but an easy solution for the time being

    replaceMemo("COMPRA.DBT",memoBuff, blockNum);

    for(int i = 0; i < 6; i++)
    {
      replaceField(operacion[i].input_buffer, indice, compra_descr[11 + i].fieldname, compra_ptr, compra_head, compra_descr);
    }

    rightAlign(cliDeu, pro_descr[13].length);
    rightAlign(cliAcr, pro_descr[12].length);

    replaceField(cliAcr, Proveedor, pro_descr[12].fieldname, pro_ptr, pro_head, pro_descr);
    replaceField(cliDeu, Proveedor, pro_descr[13].fieldname, pro_ptr, pro_head, pro_descr);
  }
  else
  {
    return;
  }
  
  free(entrada[0].prompt);
  if (fclose(pro_ptr) != 0) 
  {
    mvprintw(0, 0, "Error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }
  if (fclose(compra_ptr) != 0) 
  {
    mvprintw(0,0,"error al cerrar el archivo: %s", strerror(errno));
    getch();
    return;
  }

  getch();
  return;
}
