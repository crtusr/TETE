
************
*
* Programa.....: paramet.prg
* Sitema.......: vivero
* Base de datos: param.dbf
* indice.......:
* Autor........: jose e. gasperi
* Fecha........: 23/9/91

opcion = 1
do while .t.

   @ 05,62,9,78 BOX "ÚÄ¿³ÙÄÀ³ "

   @ 06,63 prompt 'Modificaciones ' message 'Modificaciones al archivo de PARAMETROS  '
   @ 07,63 prompt '  Consultas    ' message 'Consultas al archivo de PARAMETROS       '
   @ 08,63 prompt '  Retroceder   ' message 'Vueve al menu anterior                   '
   menu to opcion

   do case
      case opcion = 1
           * Eligio la primera opcion
           SAVE SCREEN TO PANTALLA2
           Do MODIFPA
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 2
           * Eligio la segunda opcion
           SAVE SCREEN TO PANTALLA2
           Do CONSULPA
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 3 .OR. OPCION = 0
           * Eligio la tercera opcion
           Clear
           return
   endcase

enddo

*******************************************

procedure MODIFPA

* Programa.....: modifpa.prg  (programa de modificaciones)
* Sistema......: vivero
* base de datos: PARAM.DBF
* Autor........: jose e. gasperi
* Fecha........: 23/9/91

* Abre archivo agenda indexado por numero
Use PARAM

do while .T.


   @ 2,23,12,64 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "VIVERO ***** parametros"


   @ 03,25 SAY "NUMERO DE FACT...:"
   @ 04,25 SAY "TIPO DE CAMBIO...:"
   @ 05,25 SAY "IVA RI...........:"
   @ 06,25 SAY "IVA RNI..........:"
   @ 07,25 SAY "DESCUENTO........:"
   @ 08,25 SAY " % MAYORISTA.....:"
   @ 09,25 SAY " % PUBLICO.......:"


   @ 03,43  get NFAC picture '999999'
   @ 04,43  get TCAM picture '9999.99'
   @ 05,43  get IVA1 picture '9999.99'
   @ 06,43  get IVA2 picture '9999.99'
   @ 07,43  get DESC picture '9999.99'
   @ 08,43  get PORC1 picture '9999.99'
   @ 09,43  get PORC2 picture '9999.99'
   read

@ 24,0 say "Estan todos los datos correctos ? [ENTER]                      "
TECLA=inkey(0)

      if TECLA !=13
         loop
      endif

   * graba registro

   Replace NFACTURA      With  NFAC
   Replace TCAMBIO       With  TCAM
   Replace IVAUNO        With  IVA1
   Replace IVADOS        With  IVA2
   Replace DESCUENTO     With  DESC
   Replace PORCEN1       With  PORC1
   Replace PORCEN2       With  PORC2
   use
   return
enddo
***********************************************
procedure CONSULPA


* Programa.....: consulpa.prg  (PROGRAMA DE CONSULTAS)
* Sistema......: vivero
* base de datos: PARAM.dbf
* Autor........: jose e. gasperi
* Fecha........: 123/9/91



set intensity off

* Abre archivo agenda indexado por numero
Use PARAM


do while .t.
   @ 2,23,12,64 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 say "VIVERO ***** consultasÄÄÄCant. :"

   @ 03,25 SAY "NUMERO DE FACT...:"
   @ 04,25 SAY "TIPO DE CAMBIO...:"
   @ 05,25 SAY "IVA RI...........:"
   @ 06,25 SAY "IVA RNI..........:"
   @ 07,25 SAY "DESCUENTO........:"
   @ 08,25 SAY " % MAYORISTA.....:"
   @ 09,25 SAY " % PUBLICO.......:"


   @ 03,43  get NFAC picture '999999'
   @ 04,43  get TCAM picture '9999.99'
   @ 05,43  get IVA1 picture '9999.99'
   @ 06,43  get IVA2 picture '9999.99'
   @ 07,43  get DESC picture '9999.99'
   @ 08,43  get PORC1 picture '9999.99'
   @ 09,43  get PORC2 picture '9999.99'
 clear gets

   @ 24,0 say "Presione cualquier tecla para continuar                          "
   inkey(0)

   set cursor on
   set intensity on
   use
   return

enddo

***




