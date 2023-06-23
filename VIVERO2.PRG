
*********************************
*
* Programa.....: vivero.prg
* Sitema.......: vivero
* base de datos: clipro.dbf
* indice.......:
* Autor........: jose e. gasperi
* Fecha........: 26/8/91

***********************************
clear
*set color to 2,14,0
*setcolor("w/b+,g/n+")
set color to 15
?"               같같같같같   같같같같같   같같같같같   같같같같같"
set color to 3
?"                   같       같               같       같        "
set color to 14
?"                   같       같               같       같        "
set color to 14
?"                   같       같같같           같       같같같    "
set color to 12
?"                   같       같               같       같        "
set color to 12
?"                   같       같               같       같        "
set color to 15
?"                   같       같같같같같       같       같같같같같"

set color to 2,14,0

@ 8,9 to 18,70 double

   @ 9,15 say "     Sistema creado por Jose Ernesto Gasperi      "
   @ 10,15 say "esta prohibido su copiado para la comercializacion"
   @ 12,15 say "                Galvani 415                       "
   @ 13,15 say "                1613 Ing. Pablo Nogues            "
   @ 14,15 say "                Prov. Bs. As. Argentina           "
   @ 15,15 say "                TE 660-2321 / 444-5215            "

   @ 24,15 say "     Pulse alguna tecla para continuar              "
   inkey(0)
   @ 24,0 say space(80)



set bell off
set echo off
set date french
set scoreboard off
set wrap on
set message to 17 center
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

opcion = 1
do while .t.
   set message to 17 center
   clear
   @ 0,0,15,80 BOX "旼엿耗윰�"
   @ 16,10,18,70 BOX "旼엿耗윰 "
   @ 0,3 SAY "VIVERO *** menu principal"

   @ 02,01 prompt 'Clientes' message '                Ficha de clientes              '
   @ 02,13 prompt 'Stock' message '                  Stock de mercaderia             '
   @ 02,21 prompt 'Facturacion' message '          Confeccion de facturas            '
   @ 02,36 prompt 'compras' message'       Informacion referida a los proveedores    '
   @ 02,51 prompt 'Ctas.ctes.' message '   Informacion referida a cuentas corrientes '
   @ 02,62 prompt 'Parametros' message 'Parametros (tippo de cambio, % iva, desc., %)'
   @ 02,76 prompt 'DOS' message '                Vuelve al sistema operativo         '
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
           Do FACTURA
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 4
           SAVE SCREEN TO PANTALLA1
           Do MENCOM
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 5
           SAVE SCREEN TO PANTALLA1
           Do
           RESTORE SCREEN FROM PANTALLA1
      case opcion = 6
           SAVE SCREEN TO PANTALLA1
           Do PARAMET
           RESTORE SCREEN FROM PANTALLA1

      case opcion = 7 .or. opcion = 0
           clear
           return
      endcase
 enddo
