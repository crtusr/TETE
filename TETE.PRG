
*********************************
*
* Programa.....: tete.prg
* Sitema.......: vivero
* base de datos: clipro.dbf
* indice.......:
* Autor........: jose e. gasperi
* Fecha........: 26/8/91

***********************************
clear

set intensity on

set color to 15
?"               같같같같같   같같같같같   같같같같같   같같같같같"
set color to 3
?"                   같       같     2023      같       같        "
set color to 14
?"                   같       같    2 0 2 3    같       같        "
set color to 14
?"                   같       같같같  2 0 2 3  같       같같같    "
set color to 12
?"                   같       같  2  0  2 3    같       같        "
set color to 12
?"                   같       같  2  0  2  3   같       같        "
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
   @ 12,15 say "                Cap. Giachino 415                       "
   @ 13,15 say "                1613 Ing. Pablo Nogues            "
   @ 14,15 say "                Prov. Bs. As. Argentina           "
   @ 15,15 say "                TEL 11 15 5037 9211 / 4463-1051     "

   @ 17,15 say "   HOME PAGE:  www.cultivogasperi.com.ar   "
   @ 18,15 say "   E-MAIL   : cultivosgasperi@yahoo.com.ar                "

   @ 24,15 say "     Pulse alguna tecla para continuar            "
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
public COMPROBANTE
opcion = 2
do while .t.

   @ 15,15 say time()
*    set color to w/c
     set color to w/b
*    set color to g/n
*    set color to w/b
   set message to 18 center
   clear

   @ 0,0,16,80 BOX "旼엿耗윰 "
   @ 17,10,19,70 BOX "旼엿耗윰 "
   set color to 14
   @ 0,3 SAY "VIVERO *** menu principal  2  0  2  3 "
*   set color to  w/c
   @ 02,01 prompt 'Clientes' message '                Ficha de clientes              '
   @ 02,13 prompt 'Stock' message '                  Stock de mercaderia             '
   @ 02,21 prompt 'FACTURACION' message '          Confeccion de facturas            '
   @ 02,36 prompt 'compras' message'       Informacion referida a los proveedores    '
   @ 02,51 prompt 'Ctas.ctes.' message '   Informacion referida a cuentas corrientes '
   @ 02,65 prompt 'Parametros' message 'Parametros (tipo de cambio, % iva, desc., %)'
   @ 04,01 prompt 'NOTA DE CREDITO' message '    confeccion de notas de credito    '
   @ 04,20 prompt 'REMITO' message '            Confeccion de REMITOS               '
   @ 04,28 prompt 'Registro de cheques' message'  Registro de cheques de terceros     '
   @ 04,50 prompt 'DOS' message ' Vuelve al sistema operativo           '
*   @ 10,02 say "E-mail :   marta.r.gasperi@gmail.com"
*   @ 11,02 say "E-mail :   tetegasperi@yahoo.com.ar"
*   @ 12,02 say "E-mail :   cultivosgasperi@yahoo.com.ar "
   set color to 14
   @ 13,05 say "*****************  ARGENTINA CAMPEON 2022-2026 !!!!   @@@@@@@@@@      "

   set color to w/b
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
           COMPROBANTE="NOTA DE CREDITO"
           Do FACTURA
           COMPROBANTE=" "
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