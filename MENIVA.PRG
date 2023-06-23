************
*
* Programa.....: meniva.prg
* Sitema.......: TETE
* Base de datos: libroiva.dbf
* indice.......: libfac.ntx    libcli.ntx
* Autor........: jose e. gasperi
* Fecha........: 26/7/97


* Abre libro de iva
Use LIBROIVA index LIBFAC,LIBCLI

opcion = 4
do while .t.

   @ 04,01,11,18 BOX "ÚÄ¿³ÙÄÀ³ "

   @ 05,02 prompt '    Resumen por cliente      ' message 'Altas de fichas de clientes-proveedores  '
   @ 06,02 prompt '    Resumen por fecha      ' message 'Bajas de fichas de clientes-proveedores  '
   @ 07,02 prompt '      Modificaciones      ' message 'Modificaciones de clientes-proveedores   '
   @ 08,02 prompt '        Consultas       ' message 'Consultas de clientes-proveedores        '
   @ 09,02 prompt '         Listado      ' message 'Listado por impresora                    '
   @ 10,02 prompt '       Retroceder    ' message 'vuelve al menu anterior                  '
   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA2
         *  Do RESCLIE
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 2
           * Eligio la segunda opcion
           SAVE SCREEN TO PANTALLA2
         *  Do RESFECH
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 3
           * Eligio la tercer opcion
           SAVE SCREEN TO PANTALLA2
           Do MODIFIVA
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 4
           * Eligio la cuarta opcion
           SAVE SCREEN TO PANTALLA2
           Do CONSUIVA
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 5
           * Eligio la quinta opcion
           SAVE SCREEN TO PANTALLA2
          * Do LISTIVA
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 6 .OR. OPCION = 0
           * Eligio la quinta opcion
           Clear
           return
   endcase

enddo
**********************************************************
procedure RESCLIE


set intensity off

* Abre archivo de libroiba indexado por numero de factura
Use LIBROIVA index LIBFAC,LIBCLI

do while .t.

   * Se declaran variables de trabajo

   FECH = space(10)      && FECHA
   FACT = space(13)     && FACTURA
   CLIE = space(30)     && CLIENTE
   NCUI = space(13)     && NCUIT
   RESF = space(3)      && RESFISC
   IMPO = 0             && IMPORTE
   IVA1 = 0             && IVA1
   IVA2 = 0             && IVA2
   TOTA = 0             && TOTALF

   @ 5,01,26,54 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 5,32 SAY + LASTREC()
   @ 5,04 say "TETE **altas** ULTIMO REG.:"
   @ 6,03 say "NUMERO......:" get NUME picture '9999'
   read



enddo
return
****************************************************************************
procedure RESFECH


* Abre archivo de libroiba indexado por numero de factura
Use LIBROIVA index LIBFAC,LIBCLI

Do while .t.
   * Se declaran variables de trabajo
  NUME = 0         && Numero de identificador en la agenda


   @ 2,23,21,74 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "TETEVI ***** bajas"
   @ 3,25 SAY "NUMERO...........:" get NUME picture '9999'
   read



enddo
return
*******************************************

procedure MODIFIVA

* Abre archivo de libroiba indexado por numero de factura
Use LIBROIVA index LIBFAC,LIBCLI

do while .t.



   @ 2,23,21,74 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "VIVERO ***** modificaciones"
   @ 3,25 SAY "NUMERO......:" get NUME picture '9999'
   read



enddo
return
***********************************************
procedure CONSUIVA

set intensity off


* Abre archivo de libroiba indexado por numero de factura
Use LIBROIVA index LIBFAC,LIBCLI

do while .t.
   NUME=0
   @ 2,23,21,74 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 say "VIVERO ***** consultasÄÄÄCant. :"
   read

return

**************************************************
Procedure LISTADO
 set console off
 set print on
 ? chr(15)

Use CLIPRO index CLINUM,CLIRAZ

 set order to 2

 list numero,razon,tipo,ncuit,domicilio,localidad,provincia,telefono1,telefono2  off
 eject
 set console on
 set print off
return
********************************************************

*************************************************
*Procedure para abrir un BROWSE de clientes
**************************************************

procedure VEntanacliente


   save screen to ACTUAL

     IF RAZO != SPACE(30)
        SET ORDER TO 2
        SET SOFTSEEK ON
        SEEK RAZO

   restore screen from ACTUAL

        IF EOF()
           tone(50,5)
           INKEY(0)
           RETURN
        ENDIF
      ENDIF

   save screen to ACTUAL

          @ 2,2 to 17,77 double
          declare vector[4]

          vector[1]="numero"
          vector[2]="razon"
          vector[3]="apellidos"
          vector[4]="telefono1"


          dbedit(3,3,16,76,vector)

      SET ORDER TO 1

   restore screen from ACTUAL
   set key -2 to     && desactiva F3 como funcion dbedit
   set softseek off  && desactiva la busqueda "blanda"
return
*******************************************
