
*********************************
*
* Programa.....: tete.prg
* Sitema.......: vivero
* base de datos: clipro.dbf
* indice.......:
* Autor........: jose e. gasperi
* Fecha........: 26/8/99

***********************************
clear

set intensity on

set color to 15
?"               같같같같같   같같같같같   같같같같같   같같같같같"
set color to 3
?"                   같       같               같       같        "
set color to 14                      2017
?"                   같       같               같       같        "
set color to 14
?"                   같       같같같 2017      같       같같같    "
set color to 12
?"                   같       같               같       같        "
set color to 12          2017            2017      2017
?"                   같       같               같       같        "
set color to 15
?"                   같       같같같같같       같       같같같같같"


if iscolor()
  set color to w/n
else
   set color to W/n
endif


@ 8,9 to 22,70 double

   @ 9,15 say "     Sistema creado por Jose Ernesto Gasperi      "
   @ 10,15 say "esta prohibido su copiado para la comercializacion"
   @ 12,15 say "                Galvani 415                       "
   @ 13,15 say "                1613 Ing. Pablo Nogues            "
   @ 14,15 say "                Prov. Bs. As. Argentina           "
   @ 15,15 say "                TE 463-0151 / 463-1051            "

   @ 17,15 say "   Programa para ser usado por Cultivos Norma     "
   @ 18,15 say "   en cualquiera de sus puestos de trabajo        "
   @ 19,15 say "   "
   @ 20,15 say "   "
   @ 21,15 say "   "

   @ 24,15 say "     Pulse alguna tecla para continuar              "
   inkey(0)

   llave_remito = 0  && se inicializa la variable para imprimir remitos

   @ 24,0 say space(80)


set century on
set bell off
set echo off
set date french
set scoreboard off
set wrap on
set procedure to funtete
set procedure to protete

use PARAM
    NFAC = NFACTURA
    TCAM = TCAMBIO
    IVA1 = IVAUNO
    IVA2 = IVADOS
    DESC = DESCUENTO
    PORC1= PORCEN1
    PORC2= PORCEN2
use

public NFAC,TCAM,IVA1,IVA2,DESC,PORC1,PORC2

opcion = 2
do while .t.
  set color to w/b
   set message to 18 center
   clear
   @ 0,0,16,80 BOX "旼엿耗윰 "
   @ 17,10,19,70 BOX "旼엿耗윰 "
   @ 0,3 SAY "VIVERO *** menu principal      2017  "

   @ 02,01 prompt 'Clientes' message '                Ficha de clientes              '
   @ 02,13 prompt 'Stock' message '                  Stock de mercaderia             '
   @ 02,21 prompt 'FACTURACION' message '          Confeccion de facturas            '
   @ 02,36 prompt 'compras' message'       Informacion referida a los proveedores    '
   @ 02,51 prompt 'Ctas.ctes.' message '   Informacion referida a cuentas corrientes '
   @ 02,65 prompt 'Parametros' message 'Parametros (tippo de cambio, % iva, desc., %)'
   @ 04,01 prompt 'Libro de IVA' message '    Da informacion sobre el libro de IVA    '
   @ 04,20 prompt 'REMITO' message '            Confeccion de REMITOS               '
   @ 04,28 prompt 'Registro de cheques' message'  Registro de cheques de terceros     '
   @ 04,50 prompt 'DOS' message '              Vuelve al sistema operativo           '
   @ 15,02 say date()

   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA1
           Do MENCLIEN
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 2
           * Eligio la segunda opcion
           SAVE SCREEN TO PANTALLA1
           Do MENSTOCK
           RESTORE SCREEN FROM PANTALLA1

      case opcion = 3
           SAVE SCREEN TO PANTALLA1
           COMPROBANTE="FACTURA"
           Do FACTURA
           COMPROBANTE=" "
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 4
           SAVE SCREEN TO PANTALLA1
           Do MENCOM
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 5
           SAVE SCREEN TO PANTALLA1
           Do MENCTAS
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 6
           SAVE SCREEN TO PANTALLA1
           Do PARAMET
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 7
           SAVE SCREEN TO PANTALLA1
           Do
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 8
           llave_remito = 1  && se pone en 1 para cuando imprima sepa de donde viene
           save screen to pantalla1
           COMPROBANTE="REMITO"
           do FACTURA
           COMPROBANTE=" "
           llave_remito = 0  && para darle prioridad a la factura
           restore screen from pantalla1
      case opcion = 9
           save screen to pantalla1
           do MENCHEQU
           restore screen from pantalla1
      case opcion = 10 .or. opcion = 0
           clear
           return
      endcase
 enddo
