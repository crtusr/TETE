************
*
* Programa.....: menclien.prg
* Sitema.......: vivero
* Base de datos: clipro.dbf
* indice.......:
* Autor........: jose e. gasperi
* Fecha........: 26/8/91


* Abre archivo agenda indexado por numero
Use CLIPRO index CLINUM,CLIRAZ,CLIAPE

opcion = 4
do while .t.

   @ 04,01,12,21 BOX "ÚÄ¿³ÙÄÀ³ "

   @ 05,02 prompt '      Altas      ' message 'Altas de fichas de clientes-proveedores  '
   @ 06,02 prompt '      Bajas      ' message 'Bajas de fichas de clientes-proveedores  '
   @ 07,02 prompt '  Modificaciones ' message 'Modificaciones de clientes-proveedores   '
   @ 08,02 prompt '    Consultas    ' message 'Consultas de clientes-proveedores        '
   @ 09,02 prompt ' List.de deudores ' message 'Listado de deudores segun criterio       '
   @ 10,02 prompt '   Listado Gral.  ' message 'listado general de todos los clientes    '
   @ 11,02 prompt '   Retroceder   ' message 'vuelve al menu anterior                  '

   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA2
           Do ALTASVI
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 2
           * Eligio la segunda opcion
           SAVE SCREEN TO PANTALLA2
           Do BAJASVI
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 3
           * Eligio la tercer opcion
           SAVE SCREEN TO PANTALLA2
           Do MODIFVI
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 4
           * Eligio la cuarta opcion
           SAVE SCREEN TO PANTALLA2
           Do CONSULVI
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 5
           * Eligio la quinta opcion
           SAVE SCREEN TO PANTALLA2
           Do LISTADO1
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 6
           * Eligio la sexta opcion
           SAVE SCREEN TO PANTALLA2
           Do LISTADO2
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 7 .OR. OPCION = 0
           * Eligio la quinta opcion
           Clear
           return
   endcase

enddo
**********************************************************
procedure ALTASVI


set intensity off

* Abre archivo agenda indexado por numero
Use CLIPRO index CLINUM,CLIRAZ,CLIAPE

do while .t.

   * Se declaran variables de trabajo

   NUME = 0
   RAZO = space(30)
   APEL = space(20)
   NOMB = space(20)
   DOMI = space(30)
   LOCA = space(20)
   PROV = space(10)
   CPOS = space(4)
   CUIT = space(13)
   CONT = space(3)
   TEL1 = space(20)
   TEL2 = space(20)
   TAAN = 0
   FUCO = space(10)
   ACME = 0
   TMAN = 0
   DEUD = 0
   ACRE = 0

   @ 5,01,26,54 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 5,32 SAY + LASTREC()
   @ 5,04 say "TETE **altas** ULTIMO REG.:"
   @ 6,03 say "NUMERO......:" get NUME picture '9999'
   read

   if nume = 0
      * Significa que no quiere dar mas altas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      clear
      return  && Retorna al programa anterior o estado anterior.
   endif

   seek nume   && Busca en el archivo si existe el numero que ingreso

   if .not. eof()  && Si no es fin de archivo significa que lo encontro
      @ 25,0 say 'Este numero ya fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif


   @ 7,03 SAY "RAZON SOCIAL.....:"
   @ 8,03 SAY "APELLIDOS........:"
   @ 9,03 SAY "NOMBRES..........:"
   @ 10,03 SAY "DOMICILIO........:"
   @ 11,03 SAY "LOCALIDAD........:"
   @ 12,03 SAY "PROVINCIA........:"
   @ 13,03 SAY "CODIGO POSTAL...:"
   @ 14,03 SAY "CUIT............:"
   @ 15,03 SAY "CONTRIBUYENTE...:"
   @ 16,03 SAY "TELEFONO1.......:"
   @ 17,03 SAY "TELEFONO2.......:"
   @ 18,03 SAY "TOTAL ACUMULATI.:"
   @ 19,03 SAY "FECHA ULT.COM...:"
   @ 20,03 SAY "ACUMULADO MES...:"
   @ 21,03 SAY "TOTAL MES.ANT...:"
   @ 22,03 SAY "DEUDOR..........:"
   @ 23,03 SAY "ACREEDOR........:"

*  Si no encontro en el numero ingresado
*  Pide demas datos para darlos de alta

   @  7,20  get RAZO picture Replicate('!',30) valid (RAZO !=space(30))
   @  8,20  get APEL picture Replicate('!',20)
   @  9,20  get NOMB picture Replicate('!',20)
   @  10,20  get DOMI picture replicate('!',30)
   @  11,20  get LOCA picture Replicate('!',20)
   @  12,20  get PROV picture Replicate('!',20)
   @  13,20  get CPOS
   @  14,20  get CUIT
   @  15,20  get CONT picture Replicate('!',3) valid (CONT="RI" .or. CONT="RNI" .or. CONT="RM" .or. CONT="CF")
   @ 16,20  get TEL1
   @ 17,20  get TEL2
   @ 18,20  get TAAN
   @ 19,20  get FUCO picture '99/99/99'
   @ 20,20  get ACME picture '9999999.99'
   @ 21,20  get TMAN picture '9999999.99'
   @ 22,20  get DEUD picture '9999999.99'
   @ 23,20  get ACRE picture '9999999.99'
   read



* Da registro de alta

   Append Blank
   Replace NUMERO        With  NUME
   Replace RAZON         With  RAZO
   Replace APELLIDOS     With  APEL
   Replace NOMBRES       With  NOMB
   Replace DOMICILIO     With  DOMI
   Replace LOCALIDAD     With  LOCA
   Replace PROVINCIA     With  PROV
   Replace CPOSTAL       With  CPOS
   Replace NCUIT         With  CUIT
   Replace TIPO          With  CONT
   Replace TELEFONO1     With  TEL1
   Replace TELEFONO2     With  TEL2
   Replace TELEFONO2     With  TEL2
   Replace TOTANOANT     With  TAAN
   Replace FEULTCOM      With  ctod(FUCO)
   Replace ACUMUMES      With  ACME
   Replace TOTMESANT     With  TMAN
   Replace DEUDOR        With  DEUD
   Replace ACREEDOR      With  ACRE


enddo
return
****************************************************************************
procedure BAJASVI


* Abre archivo agenda indexado por numero
Use CLIPRO index CLINUM,CLIRAZ,CLIAPE

Do while .t.
   * Se declaran variables de trabajo
  NUME = 0         && Numero de identificador en la agenda


   @ 2,23,21,74 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "TETEVI ***** bajas"
   @ 3,25 SAY "NUMERO...........:" get NUME picture '9999'
   read


if NUME = 0
   opcion=2
   @ 16,26,20,61 box "ÚÄ¿³ÙÄÀ³ "
   @ 17,27 say "Borra, esta seguro"
   @ 19,27 prompt "SI "
   @ 19,31 prompt "NO "
   menu to opcion

   do case
      case opcion = 1 .or. opcion = 0
        do borrar
        return
      case opcion = 2 .or. opcion = 0
        clear
        return
     endcase
endif


   seek NUME   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]                   '
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   * Si  encontro en el arhcivo el numero ingresado
   * carga datos en variables

   * Se cargan los campos del registro seleccionado en variables


  NUME= NUMERO
  RAZO= RAZON
  APEL= APELLIDOS
  NOMB= NOMBRES
  DOMI= DOMICILIO
  LOCA= LOCALIDAD
  PROV= PROVINCIA
  CPOS= CPOSTAL
  CUIT= NCUIT
  CONT= TIPO
  TEL1= TELEFONO1
  TEL2= TELEFONO2
  TAAN= TOTANOANT
  FUCO= FEULTCOM
  ACME= ACUMUMES
  TMAN= TOTMESANT
  DEUD= DEUDOR
  ACRE= ACREEDOR

* Presenta los datos en pantalla


   @ 4,25 SAY "RAZON SOCIAL.....:" get RAZO
   @ 5,25 SAY "APELLIDOS........:" get APEL
   @ 6,25 SAY "NOMBRES..........:" get NOMb
   @ 7,25 SAY "DOMICILIO........:" get DOMI
   @ 8,25 SAY "LOCALIDAD........:" get LOCA
   @ 9,25 SAY "PROVINCIA........:" get PROV
   @ 10,25 SAY "CODIGO POSTAL....:" get CPOS
   @ 11,25 SAY "CUIT.............:" get CUIT
   @ 12,25 SAY "CONTRIBUYENTE....:" get CONT
   @ 13,25 SAY "TELEFONO1........:" get TEL1
   @ 14,25 SAY "TELEFONO2........:" get TEL2
   @ 15,25 SAY "TOTAL ACUMULATI..:" get TAAN
   @ 16,25 SAY "FECHA ULT.COM....:" get FUCO
   @ 17,25 SAY "ACUMULADO MES....:" get ACME
   @ 18,25 SAY "TOTAL MES.ANT....:" get TMAN
   @ 19,25 SAY "DEUDOR...........:" get DEUD
   @ 20,25 SAY "ACREEDOR.........:" get ACRE
      clear gets

*Presenta los datos en pantalla



   if deleted()
      * Si ya fue marcado para borrar
      @ 24,00 say 'Esta marcado para borrar. <R> / <C>                      '
      opcion = 'RC'
   Else
      @ 24,00 say '<B>orra la ficha / <C>ontinua sin cambios                '
      opcion = 'BC'
   endif


   t = ''
   do while .not. t $ opcion
      t = upper(chr(inkey()))
   enddo

   do case
      case t = 'R'
           * Pidio reincorporacion de un registro
           Recall
           @ 24,00 say 'Se reincorporo el registro. [ENTER]                  '
      case t = 'B'
           * Pidio borrar un registro
           Delete
           @ 24,00 say 'Se borro el registro. [ENTER]                         '
      case t = 'C'
           * Continua sin cambios
           @ 24,00 say 'Continua sin cambios. [ENTER]                         '
   endcase

   inkey(0)

enddo
return
*******************************************

procedure MODIFVI


* Abre archivo agenda indexado por numero
Use CLIPRO index CLINUM,CLIRAZ,CLIAPE
do while .t.

   * Se declaran variables de trabajo
   NUME = 0
   RAZO = space(30)
   APEL = space(20)
   NOMB = space(20)
   DOMI = space(30)
   LOCA = space(20)
   PROV = space(10)
   CPOS = space(4)
   CUIT = space(13)
   CONT = space(3)
   TEL1 = space(20)
   TEL2 = space(20)
   TAAN = 0
   FUCO = space(10)
   ACME = 0
   TMAN = 0
   DEUD = 0
   ACRE = 0


   @ 2,23,21,74 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "VIVERO ***** modificaciones"
   @ 3,25 SAY "NUMERO......:" get NUME picture '9999'
   read

   if NUME = 0
      * Significa que no quiere modificar mas
      * Cierra base de datos e indices
      Close database
      Close index
      return  && Retorna al programa anterior o estado anterior.
   endif

   seek NUME   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   * Si  encontro en el arhcivo el numero ingresado
   * carga datos en variables

  NUME= NUMERO
  RAZO= RAZON
  APEL= APELLIDOS
  NOMB= NOMBRES
  DOMI= DOMICILIO
  LOCA= LOCALIDAD
  PROV= PROVINCIA
  CPOS= CPOSTAL
  CUIT= NCUIT
  CONT= TIPO
  TEL1= TELEFONO1
  TEL2= TELEFONO2
  TAAN= TOTANOANT
  FUCO= dtoc(FEULTCOM)
  ACME= ACUMUMES
  TMAN= TOTMESANT
  DEUD= DEUDOR
  ACRE= ACREEDOR



   @ 4,25 SAY "RAZON SOCIAL.....:"
   @ 5,25 SAY "APELLIDOS........:"
   @ 6,25 SAY "NOMBRES..........:"
   @ 7,25 SAY "DOMICILIO........:"
   @ 8,25 SAY "LOCALIDAD........:"
   @ 9,25 SAY "PROVINCIA........:"
   @ 10,25 SAY "CODIGO POSTAL...:"
   @ 11,25 SAY "CUIT............:"
   @ 12,25 SAY "CONTRIBUYENTE...:"
   @ 13,25 SAY "TELEFONO1.......:"
   @ 14,25 SAY "TELEFONO2.......:"
   @ 15,25 SAY "TOTAL ACUMULATI.:"
   @ 16,25 SAY "FECHA ULT.COM...:"
   @ 17,25 SAY "ACUMULADO MES...:"
   @ 18,25 SAY "TOTAL MES.ANT...:"
   @ 19,25 SAY "DEUDOR..........:"
   @ 20,25 SAY "ACREEDOR........:"




   @  4,43  get RAZO picture Replicate('!',30)
   @  5,43  get APEL picture Replicate('!',20)
   @  6,43  get NOMB picture Replicate('!',20)
   @  7,43  get DOMI picture replicate('!',30)
   @  8,43  get LOCA picture Replicate('!',20)
   @  9,43  get PROV picture Replicate('!',20)
   @ 10,43  get CPOS
   @ 11,43  get CUIT
   @ 12,43  get CONT picture Replicate('!',3) valid (CONT="RI" .or. CONT="RNI" .or. CONT="RM" .or. CONT="CF")
   @ 13,43  get TEL1
   @ 14,43  get TEL2
   @ 15,43  get TAAN
   @ 16,43  get FUCO picture '99/99/9999'
   @ 17,43  get ACME
   @ 18,43  get TMAN
   @ 19,43  get DEUD
   @ 20,43  get ACRE
   read
   * Validacion de datos

   if APEL = space(25)
      @ 24,0 say 'Debe ingresar nombre. [ENTER]'
      inkey(0)
      loop
   endif

   if dtoc(ctod(FUCO)) != FUCO
      @ 24,0 say 'La fecha es incorrecta. [ENTER]                           '
      inkey(0)
      loop
   endif

   * graba registro


   Replace NUMERO        With  NUME
   Replace RAZON         With  RAZO
   Replace APELLIDOS     With  APEL
   Replace NOMBRES       With  NOMB
   Replace DOMICILIO     With  DOMI
   Replace LOCALIDAD     With  LOCA
   Replace PROVINCIA     With  PROV
   Replace CPOSTAL       With  CPOS
   Replace NCUIT         With  CUIT
   Replace TIPO          With  CONT
   Replace TELEFONO1     With  TEL1
   Replace TELEFONO2     With  TEL2
   Replace TOTANOANT     With  TAAN
   Replace FEULTCOM      With  ctod(FUCO)
   Replace ACUMUMES      With  ACME
   Replace TOTMESANT     With  TMAN
   Replace DEUDOR        With  DEUD
   Replace ACREEDOR      With  ACRE

enddo
return
***********************************************
procedure CONSULVI

set intensity off

* Abre archivo agenda indexado por numero
Use CLIPRO index CLINUM,CLIRAZ,CLIAPE,CLILOC


do while .t.
        set order to 1
        set key -2 to VEntanacliente
   NUME=0
   RAZO=space(30)
   CANTIDAD = lastrec()
   LOCA=space(20)
   @ 2,23,21,74 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 say "VIVERO ***** consultasÄÄÄCant. :"
   @ 2,58 say +CANTIDAD picture '9999'
   @ 3,25 say "NUMERO......:" get NUME picture '9999'
   @ 4,26 say "RAZON SOCIAL....:" get RAZO picture Replicate('!',30)
   @ 5,26 say "LOCALIDAD.......:" get LOCA picture Replicate('!',20)
   read

   if (NUME = 0) .and. (RAZO = space(30)) .and. (LOCA=space(20))
      * Significa que no quiere mas consultas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      return  && Retorna al programa anterior o estado anterior.
   endif

    if NUME != 0
      seek NUME   && Busca en el archivo si existe el numero que ingreso
          if eof()  && Si  es fin de archivo significa que no lo encontro
           @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
            inkey(0)     && Espera a que se presione una tecla
           loop         &&  Va al comienzo del ciclo detrabajo
          endif
     endif

     if RAZO != space(30)
        set order to 2
        set softseek on
        seek RAZO
         if eof()
            @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
            inkey(0)
            loop
          endif
      endif
  set softseek off

     if LOCA != space(20)
        set order to 4
        set softseek on
        seek LOCA
         if eof()
            @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
            inkey(0)
            loop
          endif
      endif
  set softseek off


   * Si  encontro en el archivo el numero ingresado
   * carga datos en variables

@ 24,0 say chr(24)+' anterior  '+chr(25)+'  siguiente '+' Esc  salir                 '
TECLA = 0
set cursor off
do while TECLA != 27

  NUME= NUMERO
  RAZO= RAZON
  APEL= APELLIDOS
  NOMB= NOMBRES
  DOMI= DOMICILIO
  LOCA= LOCALIDAD
  PROV= PROVINCIA
  CPOS= CPOSTAL
  CUIT= NCUIT
  CONT= TIPO
  TEL1= TELEFONO1
  TEL2= TELEFONO2
  TAAN= TOTANOANT
  FUCO=dtoc(FEULTCOM)
  ACME= ACUMUMES
  TMAN= TOTMESANT
  DEUD= DEUDOR
  ACRE= ACREEDOR

   @ 03,25 say "NUMERO......:" get NUME
   @ 04,25 SAY "RAZON SOCIAL....:" get RAZO
   @ 05,25 SAY "APELLIDOS.......:" get APEL
   @ 06,25 SAY "NOMBRES.........:" get NOMB
   @ 07,25 SAY "DOMICILIO.......:" get DOMI
   @ 08,25 SAY "LOCALIDAD.......:" get LOCA
   @ 09,25 SAY "PROVINCIA.......:" get PROV
   @ 10,25 SAY "CODIGO POSTAL...:" get CPOS
   @ 11,25 SAY "CUIT............:" get CUIT
   @ 12,25 SAY "CONTRIBUYENTE...:" get CONT
   @ 13,25 SAY "TELEFONO1.......:" get TEL1
   @ 14,25 SAY "TELEFONO2.......:" get TEL2
   @ 15,25 SAY "TOTAL ACUMULATI.:" get TAAN
   @ 16,25 SAY "FECHA ULT.COM...:" get FUCO
   @ 17,25 SAY "ACUMULADO MES...:" get ACME
   @ 18,25 SAY "TOTAL MES.ANT...:" get TMAN
   @ 19,25 SAY "DEUDOR..........:" get DEUD
   @ 20,25 SAY "ACREEDOR........:" get ACRE

      if ACRE > DEUD
         @ 20,58 say (ACRE-DEUD)
         @ 19,58 say space(13)
      elseif ACRE < DEUD
         @ 19,58 say (DEUD-ACRE)
         @ 20,58 say space(13)
      endif
      if ACRE=DEUD
         @ 19,58 say space(13)
         @ 20,58 say space(13)
      endif
 clear gets

    TECLA = inkey(0)
    if .not. bof() .or. .not. eof()
       @ 3,48 say replicate('°',14)
    endif
       do case
          case TECLA = 24
               skip 1
               if eof()
                  @ 3,48 say 'fin de archivo'
                  skip -1
                  loop
               endif


          case TECLA = 5
               skip -1
               if bof()
                  @ 3,48 say 'com de archivo'
                  loop
                endif
           case TECLA = 1
                go top
           case TECLA = 6
                go bottom

       endcase

    enddo
    set cursor on
enddo
set intensity on
return

**************************************************
Procedure LISTADO1
llavelistado=0
do DEcide_salida

   if llavelistado=1
      return
   endif

Use CLIPRO index CLINUM,CLIRAZ

 set order to 2

 clear
   montomi=0
 @ 12,2 say"ingrese monto MINIMO de deuda individual: " get montomi picture "999999.99"
 read
   montoma=0
 @ 14,2 say"ingrese monto MAXIMO de deuda individual: " get montoma picture "999999.99"
 read
 clear

clear screen

* list numero,razon,tipo,ncuit,domicilio,localidad,provincia,telefono1 off
* list numero,razon,telefono1,feultcom for TIPO="RI" off
* list numero,razon,feultcom,for TOTANOANT <> 0 off
* list numero,razon,feultcom,TOTANOANT,ACUMUMES,TOTMESANT for dtoc(FEULTCOM)="04/01/99" off
* list numero,razon,telefono1,feultcom for NOMBRES="JOSE" off
* list numero,razon,telefono1,feultcom for LOCALIDAD="VICENTE" off
* list numero,razon,telefono1,feultcom for LOCALIDAD="TIGRE" off
* list numero,razon,telefono1,feultcom for dtoc(FEULTCOM) <> " " off
*  list numero,razon,telefono1,feultcom for dtoc(FEULTCOM)="04/01/99" .or. dtoc(FEULTCOM)="05/01/99" .or. dtoc(FEULTCOM)="06/01/99" off
*  list numero,razon,telefono1,feultcom for FEULTCOM >= ctod("01/03/99") off
* list numero,razon,telefono1,feultcom,acumumes for FEULTCOM >= ctod("01/01/99") off
  list numero,razon,telefono1,(deudor-acreedor) for ((deudor-acreedor) >= montomi) .and.((deudor-acreedor) <= montoma) .and. ((deudor-acreedor)<> 0) off
*  list numero,razon,deudor,acreedor to print off

d=0

  sum (deudor-acreedor) to d for ((deudor-acreedor) >= montomi) .and.((deudor-acreedor) <= montoma)
?"Suma total de acuerdo al criterio elegido: ", d

inkey(0)
   * set console off
   * set device to print
   * set printer to c:\users\oem\desktop\texto.txt
   * set console on

set print off
set console on




return
********************************************************


**************************************************
Procedure LISTADO2
llavelistado=0
do DEcide_salida

   if llavelistado=1
      return
   endif

Use CLIPRO index CLINUM,CLIRAZ

 set order to 2

 clear


 list numero,razon,tipo,ncuit,domicilio,localidad,provincia,telefono1 off

inkey(0)
set print off
set console on

return

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

     IF LOCA != SPACE(20)
        SET ORDER TO 4
        SET SOFTSEEK ON
        SEEK LOCA

        restore screen from ACTUAL

        IF EOF()
           tone(50,5)
           INKEY(0)
           RETURN
        ENDIF
     ENDIF
   save screen to ACTUAL

          @ 2,2 to 17,77 double
          declare vector[6]
          vector[1]="numero"
          vector[2]="razon"
          vector[3]="apellidos"
          vector[4]="localidad"
          vector[5]="telefono1"
          vector[6]="telefono2"

          dbedit(3,3,16,76,vector)

      SET ORDER TO 1

   restore screen from ACTUAL
   set key -2 to     && desactiva F3 como funcion dbedit
   set softseek off  && desactiva la busqueda "blanda"
return
*******************************************
