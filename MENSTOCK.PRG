
************
*
* Programa.....: menstock.prg
* Sitema.......: vivero
* Base de datos: stock1.dbf
* indice.......: stocod.ntx , stonom.ntx
* Autor........: jose e. gasperi
* Fecha........: 15/9/91

* Abre archivo agenda indexado por numero
Use STOCK1 index STOCOD, STONOM

opcion = 4
do while .t.

   @ 04,12,15,33 BOX "ÚÄ¿³ÙÄÀ³ "

   @ 05,13 prompt '    Altas      ' message 'Altas al archivo de STOCK    pulse ENTER'
   @ 06,13 prompt '    Bajas      ' message 'Bajas al archivo de STOCK    pulse ENTER'
   @ 07,13 prompt 'Modificaciones ' message 'Modificaciones al archivo de STOCK pulse ENTER'
   @ 08,13 prompt '  Consultas    ' message 'Consultas al archivo de STOCK  pulse ENTER'
   @ 09,13 prompt ' Listado Gral. ' message 'Listado por impresora  pulse ENTER'
   @ 10,13 prompt 'Grupo de precios' message 'Grupo de plantas de un precio(pantalla)  pulse ENTER'
   @ 11,13 prompt 'Grupo de plantas' message 'Grupo de plantas por nombre (pantalla) pulse ENTER'
   @ 12,13 prompt 'grupo por prove.' message 'Grupo de plantas por proveedor  pulse ENTER'
   @ 13,13 prompt 'ingreso mercaderia' message 'suma mercaderia que ingreso al stock actual'
   @ 14,13 prompt '  Retroceder   ' message 'Vueve al menu anterior  pulse ENTER    '
   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA2
           Do ALTASTO
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 2
           * Eligio la segunda opcion
           SAVE SCREEN TO PANTALLA2
           Do BAJASTO
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 3
           * Eligio la tercer opcion
           SAVE SCREEN TO PANTALLA2
           Do MODISTO
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 4
           * Eligio la cuarta opcion
           SAVE SCREEN TO PANTALLA2
           Do CONSUSTO
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 5
           * Elijio la cuarta opcion
             SAVE SCREEN TO PANTALLA2
             Do IMPRESORA1
             RESTORE SCREEN FROM PANTALLA2

      case opcion = 6
           * Elijio la cuarta opcion
             SAVE SCREEN TO PANTALLA2
             Do IMPRESORA2
             RESTORE SCREEN FROM PANTALLA2

      case opcion = 7
           * Elijio la cuarta opcion
             SAVE SCREEN TO PANTALLA2
             Do IMPRESORA3
             RESTORE SCREEN FROM PANTALLA2

      case opcion = 8
           * Elijio la cuarta opcion
             SAVE SCREEN TO PANTALLA2
             Do IMPRESORA4
             RESTORE SCREEN FROM PANTALLA2
      case opcion = 9
           * Eligio la novena opcion
             SAVE SCREEN TO PANTALLA2
             Do ENTRADA_MERCA
             RESTORE SCREEN FROM PANTALLA2
           return

      case opcion = 10 .OR. OPCION = 0
           * Eligio la quinta opcion
           Clear
           return
   endcase

enddo
********************************************
procedure ALTASTO

* Programa.....: ALTASTO.prg
* Sistema......: VIVERO
* Base de datos: stock1.dbf
* Autor........: jose e. gasperi
* Fecha........: 15/09/1991   19:04:40


set intensity off

* Abre archivo stock1 indexado por codigo y numero
Use STOCK1 index STOCOD, STONOM

do while .t.

   * Se declaran variables de trabajo

   CODIG = space(4)
   NOMBR = space(30)
   ENVAS = space(7)
   PROVE = space(20)
   EXIST = 0
   PPEDI = 0
   PRECB = 0
   TIPO  = SPACE(30)



   @ 4,23,23,75 BOX "ÚÄ¿³ÙÄÀ³°"
  * @ 4,55 say lastrec()+1000-1
   @ 4,55 say lastrec()
   @ 4,26 say "TETEVI * altas * Ult.codigo: "
   @ 6,25 say "CODIGO......:" get CODIG  picture replicate('!',4)
   read

   if CODIG = space(4)
      * Significa que no quiere dar mas altas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      clear
      return  && Retorna al programa anterior o estado anterior.
   endif
   CODIG=alltrim(CODIG)
   if len(CODIG) < 4
      tone(200,1)
      loop
   endif

   seek CODIG   && Busca en el archivo si existe el numero que ingreso

   if .not. eof()  && Si no es fin de archivo significa que lo encontro
      @ 24,0 say 'Este numero ya fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   @ 06,25 say "CODIGO...........:"
   @ 07,25 SAY "NOMBRE...........:"
   @ 09,25 SAY "ENVASE...........:"
   @ 10,25 SAY "PROVEEDOR........:"
   @ 11,25 SAY "EXISTENCIA.......:"
   @ 12,25 SAY "PUNTO DE PEDIDO..:"
   @ 13,25 SAY "PRECIO BASE......:"
   @ 14,25 SAY "TIPO DE PLANTA...:"

*  Si no encontro en el numero ingresado
*  Pide demas datos para darlos de alta
   @  06,43  get CODIG
   @  07,43  get NOMBR picture Replicate('!',30)
   @  09,43  get ENVAS
   @  10,43  get PROVE picture replicate('!',20)
   @  11,43  get EXIST picture '99999'
   @  12,43  get PPEDI picture '99999'
   @  13,43  get PRECB picture '99999.99'
   @  14,43  get TIPO
   read
  do TItulo
  do POrpre


   * Validacion de datos

   if NOMBR = space(30)

      @ 24,0 say 'Debe ingresar nombre. [ENTER]                             '
      inkey(0)
      loop
   endif


* Da registro de alta

   Append Blank

   replace CODIGO     with CODIG
   replace NOMBRE     with NOMBR
   replace ENVASE     with ENVAS
   replace PROVEEDOR  with PROVE
   replace EXISTENCIA with EXIST
   replace PPEDIDO    with PPEDI
   replace PRECIOB    with PRECB
   replace TIPOP      with TIPO

enddo
****************************************************
procedure BAJASTO

* Programa.....: Bajasto.prg  (programa de Bajas)
* Sistema......: VIVERO
* base de datos: STOCK1.DBF , STOCOD.NTX, STONOM.NTX
* Autor........: jose e. gasperi
* Fecha........: 17/09/1991   22:35:40


* Abre archivo stock1 indexado por codigo y numero
Use STOCK1 index STOCOD, STONOM
set intensity off

Do while .t.
   * Se declaran variables de trabajo
  CODIG = space(4)


   @ 2,23,20,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "VIVERO ***** bajas"
   @ 3,25 SAY "CODIGO...........:" get CODIG picture replicate('!',4)
   read


if CODIG = space(4)
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


   seek CODIG   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]                   '
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   * Si  encontro en el arhcivo el numero ingresado
   * carga datos en variables

   * Se cargan los campos del registro seleccionado en variables

   CODIG = CODIGO
   NOMBR = NOMBRE
   ENVAS = ENVASE
   PROVE = PROVEEDOR
   EXIST = EXISTENCIA
   PPEDI = PPEDIDO
   PRECB = PRECIOB
   TIPO  = TIPOP

* Presenta los datos en pantalla


   @ 4,25 SAY "NOMBRE..........:" get NOMBR
   @ 6,25 SAY "ENVASE..........:" get ENVAS
   @ 7,25 SAY "PROVEEDOR.......:" get PROVE
   @ 8,25 SAY "EXISTENCIA......:" get EXIST
   @ 9,25 SAY "PUNTO DE PEDIDO.:" get PPEDI
   @ 10,25 SAY "PRECIO BASE....:" get PRECB
   @ 11,25 SAY "TIPO DE PLANTA.:" get TIPO

   clear gets
   do TItulo
   do POrpre


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

procedure MODISTO

* Programa.....: modisto.prg  (programa de modificaciones)
* Sistema......: VIVERO
* base de datos: stock1.dbf, stocod.ntx, stonum.ntx
* Autor........: jose e. gasperi
* Fecha........: 17/9/1991   22:58:40

* Abre archivo stock1 indexado por codigo y numero
Use STOCK1 index STOCOD, STONOM
set intensity off
do while .t.

   * Se declaran variables de trabajo

   CODIG = space(4)
   NOMBR = space(30)
   ENVAS = space(7)
   PROVE = space(20)
   EXIST = 0
   PPEDI = 0
   PRECB = 0
   TIPO  = SPACE(30)

   @ 2,23,20,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "VIVERO ***** modificaciones"
   @ 3,25 SAY "CODIGO......:" get CODIG picture replicate('!',4)
   read

   if CODIG = space(4)
      * Significa que no quiere modificar mas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      return  && Retorna al programa anterior o estado anterior.
   endif

   seek CODIG   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   * Si  encontro en el arhcivo el numero ingresado
   * carga datos en variables


   CODIG = CODIGO
   NOMBR = NOMBRE
   ENVAS = ENVASE
   PROVE = PROVEEDOR
   EXIST = EXISTENCIA
   PPEDI = PPEDIDO
   PRECB = PRECIOB
   TIPO  = TIPOP

   @ 04,25 SAY "NOMBRE...........:"
   @ 06,25 SAY "ENVASE...........:"
   @ 07,25 SAY "PROVEEDOR........:"
   @ 08,25 SAY "EXISTENCIA.......:"
   @ 09,25 SAY "PUNTO DE PEDIDO..:"
   @ 10,25 SAY "PRECIO BASE......:"
   @ 11,25 SAY "TIPO DE PLANTA...:"


   @ 4,43  get NOMBR picture "@!"
   @ 6,43  get ENVAS
   @ 7,43  get PROVE
   @ 8,43  get EXIST
   @ 9,43  get PPEDI
   @ 10,43 get PRECB
   @ 11,43 get TIPO

   read
   do TItulo
   do POrpre

   * Validacion de datos

   if NOMBR = space(30)
      @ 24,0 say 'Debe ingresar nombre. [ENTER]'
      inkey(0)
      loop
   endif

   * graba registro


   replace CODIGO     with CODIG
   replace NOMBRE     with NOMBR
   replace ENVASE     with ENVAS
   replace PROVEEDOR  with PROVE
   replace EXISTENCIA with EXIST
   replace PPEDIDO    with PPEDI
   replace PRECIOB    with PRECB
   replace TIPOP      with TIPO

enddo
return
***********************************************
procedure CONSUSTO


* Programa.....: consulvi.prg  (PROGRAMA DE CONSULTAS)
* Sistema......: vivero
* base de datos: stock1.dbf, stocod.ntx, stonom.ntx
* Autor........: jose e. gasperi
* Fecha........: 17/09/1991   22:36



set intensity off


* Abre archivo stock1 indexado por codigo y numero
Use STOCK1 index STOCOD, STONOM


do while .t.
   set order to 1

   set key -2 to VEntanastock       && cuando se pulse F3

@ 23,0 say '    Enter'+'               [ F3 ]  '
@ 24,0 say'   ficha indi.'+'          panorama'



   CODIG = space(4)
   NOMBR = SPACE(30)
   CANTIDAD = lastrec()
   @ 1,23,20,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 1,26 say "VIVERO ***** consultasÄÄÄCant. :"
 *  @ 1,58 say +CANTIDAD+1000-1 picture '9999'
   @ 1,58 say +CANTIDAD picture '9999'
   @ 2,25 say "CODIGO..........:" get CODIG picture replicate('!',4)
   @ 3,25 SAY "NOMBRE..........:" get NOMBR picture replicate('!',30)
   read

@ 23,0 say '    '+'['+chr(24)+']'+'             '+'['+chr(25)+']'+'          '+'[ Esc ]'
@ 24,0 say'anterior'+'            '+' siguiente'+'     '+'salir'




   if CODIG = space(4) .and. NOMBR = space(30)
      * Significa que no quiere mas consultas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      return  && Retorna al programa anterior o estado anterior.
   endif

 if CODIG != space(4)
   seek CODIG   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif
 endif

if NOMBR != space(30)
    set order to 2
    set softseek on
    seek NOMBR
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

   CODIG = CODIGO
   NOMBR = NOMBRE
   ENVAS = ENVASE
   PROVE = PROVEEDOR
   EXIST = EXISTENCIA
   PPEDI = PPEDIDO
   PRECB = PRECIOB
   TIPO  = TIPOP

   @ 2,25 say "CODIGO..........:" get CODIG
   @ 3,25 SAY "NOMBRE..........:" get NOMBR
   @ 5,25 SAY "ENVASE..........:" get ENVAS
   @ 6,25 SAY "PROVEEDOR.......:" get PROVE
   @ 7,25 SAY "EXISTENCIA......:" get EXIST
   @ 8,25 SAY "PUNTO DE PEDIDO.:" get PPEDI
   @ 9,25 SAY "PRECIO BASE.....:" get PRECB
   @ 10,25 SAY "TIPO DE PLANTA..:" get TIPO

clear gets

    do TItulo
    do POrpre


    TECLA = inkey(0)
    if .not. bof() .or. .not. eof()
       @ 2,48 say replicate('°',26)
    endif
       do case
          case TECLA = 24
               skip 1
               do TItulo
               do POrpre
               if eof()
                  @ 2,48 say 'fin de archivo'
                  skip -1
                  do TItulo
                  do POrpre
                  loop
               endif


          case TECLA = 5
               skip -1
               do TItulo
               do POrpre
               if bof()
                  @ 2,48 say 'com de archivo'
                  loop
                endif
           case TECLA = 1
                go top
                do TItulo
                do POrpre
           case TECLA = 6
                go bottom
                do TItulo
                do POrpre
       endcase

    enddo
    set cursor on
enddo
set intensity on
return
**************************************
*procedimiento para escribir los ultimos titulos
**************************************
procedure TItulo
   @ 18,25 SAY "PRECIO MAY.......:"
   @ 19,25 SAY "PRECIO PUB.......:"
return


************************************
*Procedimiento para poder escribir los datos faltantes en la ficha de stock
*************************************
procedure POrpre

   PREMA = PRECB * (1+PORC1/100)
   @  18,43  say +PREMA picture '9999.99'

   PREPU = PRECB * (1+PORC2/100)
   @  19,43  say +PREPU picture '9999.99'
return

**************************************
*Procedimiento para escribir el listado por impresora
**************************************
procedure IMPRESORA1
  set console off
  set print on
    use STOCK1 index stonom

 list CODIGO,NOMBRE,ENVASE,PRECIOB,ALLTRIM(TIPOP) for PRECIOB !=0 off
*  list CODIGO,NOMBRE,ENVASE,PRECIOB,ALLTRIM(TIPOP) for (PROVEEDOR="226") .or. (PROVEEDOR="norte") off
  eject
  set console on
  set print off
procedure IMPRESORA2
   clear
   pesos=0
   @ 15,20 say "precio.........:" get pesos picture "999.99"
   read
   clear
   list CODIGO,NOMBRE,ENVASE,PRECIOB,ALLTRIM(TIPOP) for PRECIOB = pesos off
   inkey(0)
return
procedure IMPRESORA3
*  set console off
*  set print on

    use STOCK1 index stonom
   clear
   N=space(20)
   @ 15,20 say "Nombre..........:" get N
   read
   clear
  list CODIGO,NOMBRE,ENVASE,PROVEEDOR,PRECIOB,ALLTRIM(TIPOP) for PRECIOB !=0 .and. NOMBRE=N off
  inkey(0)
*  eject
*  set console on
*  set print off
return

procedure IMPRESORA4
do DEcide_salida
    use STOCK1 index stonom
   clear
   NUMERO=space(20)
   @ 15,20 say "Numero de proveedor..:" get NUMERO
   read
   clear
  list CODIGO,NOMBRE,ENVASE,PRECIOB,ALLTRIM(TIPOP) for PROVEEDOR=NUMERO off

  inkey(0)

  set console on
  set print off
return
*************************************************
*Procedure para abrir un BROWSE de precios
**************************************************

procedure VEntanastock


   save screen to ACTUAL
        * keyboard chr(13)
        * NOMBR = SPACE(30)
        * @ 10,20,15,75 BOX"ÚÄ¿³ÙÄÀ³°"
        * @ 12,22 SAY "NOMBRE...........:" GET NOMBR PICTURE REPLICATE('!',30)
        * READ
     IF NOMBR != SPACE(30)
        SET ORDER TO 2
        SET SOFTSEEK ON
        SEEK NOMBR

   restore screen from ACTUAL

        IF EOF()
           tone(50,5)
           INKEY(0)
           RETURN
        ENDIF
      ENDIF

   save screen to ACTUAL

          @ 2,2 to 17,77 double
          declare vector[5]
          vector[1]="codigo"
          vector[2]="nombre"
          vector[3]="envase"
          vector[4]="preciob"
          vector[5]="proveedor"


          dbedit(3,3,16,76,vector)
                                     *  codigo[i]=CODIGO
      SET ORDER TO 1

   restore screen from ACTUAL
   set key -2 to     && desactiva F3 como funcion dbedit
   set softseek off  && desactiva la busqueda "blanda"
return
*******************************************

procedure ENTRADA_MERCA

* Programa.....: menstock.prg
* Sistema......: TETE
* base de datos: stock1.dbf, stocod.ntx, stonum.ntx
* Autor........: jose e. gasperi
* Fecha........: 20/08/2003 12:05

* Abre archivo stock1 indexado por codigo y numero
Use STOCK1 index STOCOD, STONOM
set intensity off
do while .t.

   * Se declaran variables de trabajo

   CODIG = space(4)
   NOMBR = space(30)
   ENVAS = space(7)
   PROVE = space(20)
   EXIST = 0
   PPEDI = 0
   PRECB = 0
   TIPO  = SPACE(30)
   ENTRA = 0 && es una variable interna para ingresar lo que entro

   @ 2,23,20,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "VIVERO ***** entrada de mercaderia"
   @ 3,25 SAY "CODIGO......:" get CODIG picture replicate('!',4)
   read

   if CODIG = space(4)
      * Significa que no quiere modificar mas
      * Cierra base de datos e indices
      Close database
      Close index
      set intensity on
      return  && Retorna al programa anterior o estado anterior.
   endif

   seek CODIG   && Busca en el archivo si existe el numero que ingreso

   if eof()  && Si  es fin de archivo significa que no lo encontro
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

   * Si  encontro en el arhcivo el numero ingresado
   * carga datos en variables


   CODIG = CODIGO
   NOMBR = NOMBRE
   ENVAS = ENVASE
   PROVE = PROVEEDOR
   EXIST = EXISTENCIA
   PPEDI = PPEDIDO
   PRECB = PRECIOB
   TIPO  = TIPOP
   ENTRA = 0

   @ 04,25 SAY "NOMBRE...........:"
   @ 06,25 SAY "ENVASE...........:"
   @ 07,25 SAY "PROVEEDOR........:"
   @ 08,25 SAY "ENTRADA..........:"
   @ 09,25 SAY "PUNTO DE PEDIDO..:"
   @ 10,25 SAY "PRECIO BASE......:"
   @ 11,25 SAY "TIPO DE PLANTA...:"


  * @ 4,43  get NOMBR picture "@!"
  * @ 6,43  get ENVAS
  * @ 7,43  get PROVE
    @ 8,43  get ENTRA
  * @ 9,43  get PPEDI
  * @ 10,43 get PRECB
  * @ 11,43 get TIPO
   read
   EXIST=EXIST+ENTRA

  * do TItulo
  * do POrpre

   * Validacion de datos

   * if NOMBR = space(30)
    *  @ 24,0 say 'Debe ingresar nombre. [ENTER]'
    *  inkey(0)
    *  loop
  * endif

   * graba registro


   replace CODIGO     with CODIG
   replace NOMBRE     with NOMBR
   replace ENVASE     with ENVAS
   replace PROVEEDOR  with PROVE
   replace EXISTENCIA with EXIST
   replace PPEDIDO    with PPEDI
   replace PRECIOB    with PRECB
   replace TIPOP      with TIPO

enddo
return


