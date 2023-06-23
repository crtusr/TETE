* Programa.....: menchequ.prg
* Sitema.......: vivero
* Base de datos: che_terc.dbf.dbf
* indice.......:
* Autor........: jose e. gasperi
* Fecha........: 08/01/99

* Abre archivo agenda indexado por numero
Use CHE_TERC index CHE_NUM,CHE_FEE,CHE_FEC,CHE_BAN,CHE_PLA,CHE_TIT,CHE_END,CHE_IMP,CHE_ENT,CHE_FEN


opcion = 4
do while .t.

   @ 06,30,15,50 BOX "ÚÄ¿³ÙÄÀ³"

   @ 07,31 prompt '    Altas      ' message 'Altas al archivo de CHEQUES            '
   @ 08,31 prompt '    Bajas      ' message 'Bajas al archivo de CHEQUES            '
   @ 09,31 prompt 'Modificaciones ' message 'Modificaciones al archivo de CHEQUES   '
   @ 10,31 prompt '  Consultas    ' message 'Consultas al archivo de CHEQUES        '
   @ 11,31 prompt '   Listado     ' message 'Listado por impresora de cheques para pagos'
   @ 12,31 prompt '   Pantalla    ' message 'Listado por pantalla de cheques para pagos'
   @ 13,31 prompt '  Retroceder   ' message 'Vuelve al menu anterior '
   @ 14,31 prompt 'cheques en fecha' message 'cheques en fecha para cobrar           '

   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA2
           Do ALTACHE
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 2
           * Eligio la segunda opcion
           SAVE SCREEN TO PANTALLA2
           Do BAJACHE
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 3
           * Eligio la tercer opcion
           SAVE SCREEN TO PANTALLA2
           Do MODICHE
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 4
           * Eligio la cuarta opcion
           SAVE SCREEN TO PANTALLA2
           Do CONSUCHE
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 5
           * Elijio la quinta opcion
             SAVE SCREEN TO PANTALLA2
             Do PAPEL
             RESTORE SCREEN FROM PANTALLA2
      case opcion = 6
           *eligio la sexta opcion
             SAVE SCREEN TO PANTALLA2
             Do PANTALLA
             RESTORE SCREEN FROM PANTALLA2
      case opcion = 7 .OR. OPCION = 0
           * Eligio la septima opcion
           Clear
           return

      case opcion = 8
           *eligio la octava opcion
             SAVE SCREEN TO PANTALLA2
             Do DEPOSITAR
             RESTORE SCREEN FROM PANTALLA2
   endcase

enddo
********************************************
procedure ALTACHE


set intensity off

* Abre archivo agenda indexado por
Use CHE_TERC index CHE_NUM,CHE_FEE,CHE_FEC,CHE_BAN,CHE_PLA,CHE_TIT,CHE_END,CHE_IMP,CHE_ENT,CHE_FEN

do while .t.

   * Se declaran variables de trabajo

   NUMER = space(4)
   FECHA_EM = space(10)
   FECHA_CO = space(10)
   BANC = space(10)
   PLAZ = space(10)
   TITULA = space(20)
   ENDOSANT = space(20)
   IMPORT= 0
   ENTREGAD=space(20)
   FECHA_EN = space(10)



   @ 4,23,23,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 4,55 say lastrec()
   @ 4,26 say "CHEQUES DE TERCEROS * altas * Ult.codigo: "
   @ 6,25 say "NUMERO......:" get NUMER  picture replicate('!',4)
   read

   if NUMER = space(4)
      * Significa que no quiere dar mas altas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      clear
      return  && Retorna al programa anterior o estado anterior.
   endif

   if len(NUMER) < 4
      tone(200,1)
      loop
   endif

   seek NUMER   && Busca en el archivo si existe el numero que ingreso

   if .not. eof()  && Si no es fin de archivo significa que lo encontro
      @ 24,0 say 'este numero ya fue dado de alta [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   @ 06,25 say "NUMERO...........:"
   @ 07,25 SAY "FECHA EMISION....:"
   @ 08,25 SAY "FECHA DE COBRO...:"
   @ 09,25 SAY "BANCO............:"
   @ 10,25 SAY "PLAZA............:"
   @ 11,25 SAY "TITULAR..........:"
   @ 12,25 SAY "ENDOSANTE........:"
   @ 13,25 SAY "IMPORTE..........:"
   @ 14,25 SAY "ENTREGADO........:"
   @ 15,25 SAY "FECHA_ENT........:"

*  Si no encontro en el numero ingresado
*  Pide demas datos para darlos de alta
   @  06,43  say NUMER
   @  07,43  get FECHA_EM picture '99/99/9999'
   @  08,43  get FECHA_CO picture '99/99/9999'
   @  09,43  get BANC picture replicate('!',10)
   @  10,43  get PLAZ picture replicate('!',10)
   @  11,43  get TITULA picture replicate('!',20)
   @  12,43  get ENDOSANT picture replicate('!',20)
   @  13,43  get IMPORT PICTURE '9999999.99'
   @  14,43  get ENTREGAD picture replicate('!',20)
   @  15,43  get FECHA_EN picture '99/99/9999'
   read


   * Validacion de datos

   if NUMER = space(4)

      @ 24,0 say 'Debe ingresar los cuatro ultimos numeros de cheque. [ENTER]                             '
      inkey(0)
      loop
   endif


* Da registro de alta

   Append Blank

replace NUMERO      with NUMER
replace FECHA_EMI   with ctod(FECHA_EM)
replace FECHA_COB   with ctod(FECHA_CO)
replace BANCO       with BANC
replace PLAZA       with PLAZ
replace TITULAR     with TITULA
replace ENDOSANTE   with ENDOSANT
replace IMPORTE     with IMPORT
replace ENTREGADO   with ENTREGAD
replace FECHA_ENT   with ctod(FECHA_EN)

enddo
****************************************************
procedure BAJACHE

* Abre archivo agenda indexado por numero
Use CHE_TERC index CHE_NUM,CHE_FEE,CHE_FEC,CHE_BAN,CHE_PLA,CHE_TIT,CHE_END,CHE_IMP,CHE_ENT,CHE_FEN

set intensity off

Do while .t.
   * Se declaran variables de trabajo
  NUMER = space(4)


   @ 2,23,20,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "CHEQUES TERCEROS ***** bajas"
   @ 3,25 SAY "NUMERO...........:" get NUMER picture replicate('!',4)
   read


if NUMER = space(4)
   opcion=2
   @ 16,26,20,61 box "ÚÄ¿³ÙÄÀ³ "
   @ 17,27 say "Borra, esta seguro"
   @ 19,27 prompt "SI "
   @ 19,31 prompt "NO "
   menu to opcion

   do case
      case opcion = 1 .or. opcion = 0
        do borrar
        set intensity on
        return
      case opcion = 2 .or. opcion = 0
        clear
        set intensity on
        return
     endcase
endif


   seek NUMER   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]                   '
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   * Si  encontro en el arhcivo el numero ingresado
   * carga datos en variables

   * Se cargan los campos del registro seleccionado en variables


   NUMER = space(4)
   FECHA_EM = space(10)
   FECHA_CO = space(10)
   BANC = space(10)
   PLAZ = space(10)
   TITULA = space(20)
   ENDOSANT = space(20)
   IMPORT= 0
   ENTREGAD=space(20)
   FECHA_EN = space(10)
* Presenta los datos en pantalla

   @  4,25 say "NUMERO.............:" get NUMER
   @  6,25 say "FECHA EMISION......:" get FECHA_EM
   @  7,25 say "FECHA DE COBRO.....:" get FECHA_CO
   @  8,25 say "BANCO..............:" get BANC
   @  9,25 say "PLAZA..............:" get PLAZ
   @  10,25 say "TITULAR...........:" get TITULA
   @  11,25 say "ENDOSANTE.........:" get ENDOSANT
   @  12,25 say "IMPORTE...........:" get IMPORT
   @  13,25 say "ENTREGADO.........:" get ENTREGAD
   @  14,25 say "FECHA DE ENTREGA..:" get FECHA_EN


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

procedure MODICHE

* Abre archivo agenda indexado por numero
Use CHE_TERC index CHE_NUM,CHE_FEE,CHE_FEC,CHE_BAN,CHE_PLA,CHE_TIT,CHE_END,CHE_IMP,CHE_ENT,CHE_FEN

set intensity off
do while .t.

   * Se declaran variables de trabajo

   NUMER = space(4)
   FECHA_EM = space(10)
   FECHA_CO = space(10)
   BANC = space(10)
   PLAZ = space(10)
   TITULA = space(20)
   ENDOSANT = space(20)
   IMPORT= 0
   ENTREGAD=space(20)
   FECHA_EN = space(10)

   @ 2,23,20,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 3,26 SAY "Cheques de terceros ***** modificaciones"
   @ 4,25 SAY "NUMERO......:" get NUMER picture replicate('!',4)
   read

   if NUMER = space(4)
      * Significa que no quiere modificar mas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      return  && Retorna al programa anterior o estado anterior.
   endif

   seek NUMER   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   * Si  encontro en el arhcivo el numero ingresado
   * carga datos en variables


   NUMER = NUMERO
   FECHA_EM = dtoc(FECHA_EMI)
   FECHA_CO =dtoc(FECHA_COB)
   BANC = BANCO
   PLAZ = PLAZA
   TITULA = TITULAR
   ENDOSANT = ENDOSANTE
   IMPORT = IMPORTE
   ENTREGAD = ENTREGADO
   FECHA_EN =dtoc(FECHA_ENT)

   @ 04,25 SAY "NUMERO...........:"
   @ 05,25 SAY "FECHA EMISION....:"
   @ 06,25 SAY "FECHA COBRO......:"
   @ 07,25 SAY "BANCO............:"
   @ 08,25 SAY "PLAZA............:"
   @ 09,25 SAY "TITULAR..........:"
   @ 10,25 SAY "ENDOSANTE........:"
   @ 11,25 SAY "IMPORTE..........:"
   @ 12,25 SAY "ENTREGADO........:"
   @ 13,25 SAY "FECHA ENTREGADO..:"

   @ 4,43  get NUMER picture "@!"
   @ 5,43  get FECHA_EM picture "99/99/9999"
   @ 6,43  get FECHA_CO picture "99/99/9999"
   @ 7,43  get BANC picture replicate("!",10)
   @ 8,43  get PLAZ picture replicate("!",10)
   @ 09,43 get TITULA picture replicate("!",20)
   @ 10,43 get ENDOSANT picture replicate("!",20)
   @ 11,43 GET IMPORT picture "9999999.99"
   @ 12,43 GET ENTREGAD picture replicate("!",20)
   @ 13,44 GET FECHA_EN picture "99/99/9999"

   read

   * Validacion de datos

   if NUMER = space(4)
      @ 24,0 say 'Debe ingresar nombre. [ENTER]'
      inkey(0)
      loop
   endif

   * graba registro

replace NUMERO      with NUMER
replace FECHA_EMI    with ctod(FECHA_EM)
replace FECHA_COB    with ctod(FECHA_CO)
replace BANCO       with BANC
replace PLAZA       with PLAZ
replace TITULAR     with TITULA
replace ENDOSANTE   with ENDOSANT
replace IMPORTE     with IMPORT
replace ENTREGADO   with ENTREGAD
replace FECHA_ENT   with ctod(FECHA_EN)


enddo
return
***********************************************
procedure CONSUCHE

set intensity off



* Abre archivo agenda indexado por numero
Use CHE_TERC index CHE_NUM,CHE_FEE,CHE_FEC,CHE_BAN,CHE_PLA,CHE_TIT,CHE_END,CHE_IMP,CHE_ENT,CHE_FEN

do while .t.
   set order to 1

   set key -2 to VEntanacheques       && cuando se pulse F3

@ 23,0 say '    Enter'+'               [ F3 ]  '
@ 24,0 say'   ficha indi.'+'          panorama'



   NUMER = space(4)
   BANC= SPACE(10)
   TITULA = space(20)
   ENDOSANT = space(20)
   IMPORT = 0
   ENTREGAD = space(20)

   CANTIDAD = lastrec()
   @ 1,23,14,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 1,26 say "VIVERO ***** consultasÄÄÄCant. :"
 *  @ 1,58 say +CANTIDAD+1000-1 picture '9999'
   @ 01,58 say +CANTIDAD picture '9999'
   @ 03,25 say "NUMERO.............:" get NUMER picture replicate('!',4)
   @ 06,25 SAY "BANCO..............:" get BANC PICTURE REPLICATE('!',10)
   @ 08,25 SAY "TITULAR............:" get TITULA PICTURE REPLICATE('!',20)
   @ 09,25 SAY "ENDOSANTE..........:" get ENDOSANT PICTURE REPLICATE('!',20)
   @ 10,25 SAY "IMPORTE............:" get IMPORT PICTURE '9999999.99'
   @ 11,25 SAY "ENTREGADO A........:" get ENTREGAD PICTURE REPLICATE('!',20)

   read

@ 23,0 say '    '+'['+chr(24)+']'+'             '+'['+chr(25)+']'+'          '+'[ Esc ]'
@ 24,0 say'anterior'+'            '+' siguiente'+'     '+'salir'




   if NUMER = space(4) .and. BANC = space(10) .AND. TITULA = space(20) .and. ENDOSANT=SPACE(20) .and. IMPORT=0 .and. ENTREGAD=space(20)
      * Significa que no quiere mas consultas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      return  && Retorna al programa anterior o estado anterior.
   endif

 if NUMER != space(4)
   seek NUMER   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif
 endif

if BANC != space(10)
    set order to 4
    set softseek on
    seek BANC
    if eof()
       @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
       inkey(0)
       loop
    endif
endif

if TITULA != space(20)
    set order to 6
    set softseek on
    seek TITULA
    if eof()
       @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
       inkey(0)
       loop
    endif
endif

if ENDOSANT != space(20)
    set order to 7
    set softseek on
    seek ENDOSANT
    if eof()
       @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
       inkey(0)
       loop
    endif
endif

if IMPORT != 0
    set order to 8
    set softseek on
    seek IMPORT
    if eof()
       @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
       inkey(0)
       loop
    endif
endif

if ENTREGAD != space(20)
    set order to 9
    set softseek on
    seek ENTREGAD
    if eof()
       @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
       inkey(0)
       loop
    endif
endif
    set softseek off

TECLA = 0
set cursor off
do while TECLA != 27

   NUMER = NUMERO
   FECHA_EM =dtoc(FECHA_EMI)
   FECHA_CO =dtoc(FECHA_COB)
   BANC = BANCO
   PLAZ = PLAZA
   TITULA = TITULAR
   ENDOSANT = ENDOSANTE
   IMPORT = IMPORTE
   ENTREGAD = ENTREGADO
   FECHA_EN =dtoc(FECHA_ENT)

   @ 03,25 say "NUMERO.............:" get NUMER
   @ 04,25 say "FECHA EMISION......:" get FECHA_EM
   @ 05,25 say "FECHA DE COBRO.....:" get FECHA_CO
   @ 06,25 say "BANCO..............:" get BANC
   @ 07,25 say "PLAZA..............:" get PLAZ
   @ 08,25 say "TITULAR............:" get TITULA
   @ 09,25 say "ENDOSANTE..........:" get ENDOSANT
   @ 10,25 say "IMPORTE............:" get IMPORT
   @ 11,25 say "ENTREGADO..........:" get ENTREGAD
   @ 12,25 say "FECHA DE ENTREGA...:" get FECHA_EN


clear gets



    TECLA = inkey(0)
    if .not. bof() .or. .not. eof()
       @ 2,48 say replicate('°',26)
    endif
       do case
          case TECLA = 24
               skip 1
               if eof()
                  @ 2,48 say 'fin de archivo'
                  skip -1
                  loop
               endif


          case TECLA = 5
               skip -1
               if bof()
                  @ 2,48 say 'com de archivo'
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
**************************************
*Procedimiento para escribir el listado por impresora
**************************************
procedure PAPEL
  set console off
  set print on
clear
* Abre archivo agenda indexado por numero
Use CHE_TERC index CHE_IMP
 ?" num."+" fecha_cob"+" titular         "+"     endosante          "+"    IMPORTE"
 ?"---------------------------------------------------------------------------------"
 list NUMERO,FECHA_COB,TITULAR,ENDOSANTE,IMPORTE for ENTREGADO=space(20) off
* list NUMERO,FECHA_COB,TITULAR,ENDOSANTE,IMPORTE,ENTREGADO
*im=0

*sum IMPORTE to im
*sum IMPORTE to im for TITULAR="FRASER"
*clear
*ENTRE=space(20)
*@ 15,15 say"suma de cheques entregados a : " get ENTRE
*read
*sum IMPORTE to im for ENTREGADO=ENTRE
*@ 17,15 say"suma :"
*@ 17,21 say im
inkey(0)

eject
  set console on
  set print off
return

**************************************
*Procedimiento para escribir el listado por pantalla
**************************************
procedure PANTALLA
clear
* Abre archivo agenda indexado por numero
Use CHE_TERC index CHE_IMP
?" num."+" fecha_cob"+" titular         "+"     endosante          "+"    IMPORTE"
?"---------------------------------------------------------------------------------"
 list NUMERO,FECHA_COB,TITULAR,ENDOSANTE,IMPORTE for ENTREGADO=space(20) off
* list NUMERO,FECHA_COB,TITULAR,ENDOSANTE,IMPORTE,ENTREGADO
su=0
sum IMPORTE to su for ENTREGADO=space(20)
*sum IMPORTE to su for TITULAR="FRASER"
?su
inkey(0)
return

**************************************
*Procedimiento para escribir el listado de cheques para cobrar
**************************************
procedure DEPOSITAR
clear
* Abre archivo agenda indexado por numero
Use CHE_TERC index CHE_IMP
?" num."+" fecha_cob"+" titular         "+"     endosante          "+"    IMPORTE"
?"---------------------------------------------------------------------------------"
 list NUMERO,FECHA_COB,TITULAR,ENDOSANTE,IMPORTE for FECHA_COB <= date() .and. ENTREGADO=" " off
* list NUMERO,FECHA_COB,TITULAR,ENDOSANTE,IMPORTE,ENTREGADO

su=0
sum IMPORTE to su for (ENTREGADO=space(20) .and. FECHA_COB <= date())
?su
inkey(0)
return

*************************************************
*Procedure para abrir un BROWSE de CHEQUES
**************************************************

procedure VEntanacheque


   save screen to ACTUAL

     IF NUMER != SPACE(4)
        SET ORDER TO 1
        SEEK NUMER

          restore screen from ACTUAL

        IF EOF()
           tone(50,5)
           INKEY(0)
           RETURN
        ENDIF
     endif

       IF BANC != SPACE(10)
          SET ORDER TO 4
          SET SOFTSEEK ON
          SEEK BANC
       ENDIF
       if TITULA != space(20)
          set order to 6
          set softseek on
          seek TITULA
       endif

       if ENDOSANT != space(20)
          set order to 7
          set softseek on
          seek ENDOSANT
       endif
       if IMPORT !=0
          set order to 8
          set softseek on
          seek IMPORT
       endif
       if ENTREGAD != space(20)
          set order to 9
          set softseek on
          seek ENTREGAD
       endif

*       if FECHA_EN != space(10)
 *         set order to 10
  *        set softseek on
    *      seek FECHA_EN
   *    endif
   save screen to ACTUAL

          @ 2,2 to 17,77 double
          declare vector[10]
          vector[1]="NUMERO"
          vector[2]="FECHA_EMI"
          vector[3]="FECHA_COB"
          vector[4]="BANCO"
          vector[5]="plaza"
          vector[6]="TITULAR"
          vector[7]="ENDOSANTE"
          vector[8]="IMPORTE"
          vector[9]="ENTREGADO"
          vector[10]="FECHA_ENT"

          dbedit(3,3,16,76,vector)
                                     *  codigo[i]=CODIGO
      SET ORDER TO 1

   restore screen from ACTUAL
   set key -2 to     && desactiva F3 como funcion dbedit
   set softseek off  && desactiva la busqueda "blanda"
return
*******************************************
