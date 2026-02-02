#ifndef PROTETE_H
#define PROTETE_H

#include <inttypes.h>

#define CLI_MAX_LENGTH 30
#define PRO_MAX_LENGTH 30
#define STOCK_MAX_LENGTH 30
#define CHEC_MAX_LENGTH 20
#define FILTER_BUFFER_SIZE 255

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

typedef enum 
{
  YEAR_ON,
  YEAR_OFF,
}ExtFieldSettings;

typedef struct
{
  char fieldName[11];
  uint8_t setting;
}SearchFields;

/*
static int extBetwFields( Searchconfig* config
                          char* buffer, 
                          const char* low, 
                          const char* high, 
                          )
                          
                        
*/
typedef struct 
{
  const char fName[16];  
  const size_t nOfFields; 
  SearchFields* fields;
  const char* fieldName;
  const int extraSpace;
}SearchConfig;

#define INIT_SEARCH_CONFIG(...) (SearchConfig){ \
    .fName = "Default", \
    .extraSpace = 0,    \
    .nOfFields = 1,     \
    .fields = NULL,     \
    .fieldName = NULL,  \
    __VA_ARGS__         \
}

void sto_consulta();
void consulta_operacion();
void consulta_compra();
void ultimas_op_cli();
void opsCliPorImpresora();
void opsComPorImpresora();
void ultimasOpCom();
void opCom();
void agregarCtacte();
void agregarOrdCom();
void modCtacte();
void modCom();
void consCli();
void consPro();
void modCli();
void modPro();
void modStock();
void modCheque();
void agregarCli();
void agregarPro();
void acreedores();
void agregarStock();
void grupoPorPrecio();
void grupoPorNombre();
void grupoPorProv();
void agregarCheque();
void consCheque();
void chequesNoEntregados();
void deudores();
void funcTest();

#endif
