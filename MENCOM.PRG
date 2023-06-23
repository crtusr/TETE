
************
*
* Programa.....: mencom.prg
* Sitema.......: vivero
* Base de datos: prove.dbf, compra.dbf
* indice.......: pronro.ntx,proraz.ntx   comord.ntx,comnro.ntx,comnom.ntx
* Autor........: jose e. gasperi
* Fecha........: 15/4/95


opcion = 1
do while .t.
   clear screen

   @ 01,21,11,53 BOX "ÚÄ¿³ÙÄÀ³°"

   @ 03,23 prompt '   Lista de proveedores      ' message 'informacion sobre proveedores y saldos   '
   @ 05,23 prompt '    Registro de compras      ' message '   registro e informacion de compras     '
   @ 07,23 prompt '    Balance de compras       ' message '         suma de saldos a pagar          '
   @ 09,23 prompt '      Menu anterior          ' message '        vuelve al menu anterior          '
   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA2
           Do PROVEDOR
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 2
           * Eligio la segunda opcion
           SAVE SCREEN TO PANTALLA2
           Do COMPRAS
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 3
           save screen to pantalla2
           Do BAlance
           restore screen from pantalla2

      case opcion = 4 .OR. OPCION = 0
           * Eligio la quinta opcion
           Clear
           return
   endcase

enddo
************************************************************
procedure PROVEDOR

opcion = 4
do while .t.


   @ 04,38,11,65 BOX "ÚÄ¿³ÙÄÀ³ "

   @ 05,40 prompt '       Altas       ' message 'Altas de fichas de clientes-proveedores  '
   @ 06,40 prompt '       Bajas       ' message 'Bajas de fichas de clientes-proveedores  '
   @ 07,40 prompt  '  Modificaciones  ' message 'Modificaciones de clientes-proveedores   '
   @ 08,40 prompt '     Consultas     ' message 'Consultas de clientes-proveedores        '
   @ 09,40 prompt 'Listado de acreedores' message 'Listado por pantalla o impresora segun criterio MINIMO Y MAXIMO'
   @ 10,40 prompt '     Retroceder    ' message 'vuelve al menu anterior                  '
   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA2
           Do ALTAPRO
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 2
           * Eligio la segunda opcion
           SAVE SCREEN TO PANTALLA2
           Do BAJAPRO
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 3
           * Eligio la tercer opcion
           SAVE SCREEN TO PANTALLA2
           Do MODIPRO
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 4
           * Eligio la cuarta opcion
           SAVE SCREEN TO PANTALLA2
           Do CONSUPRO
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 5
           * Eligio la quinta opcion
           SAVE SCREEN TO PANTALLA2
           Do LIST
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 6 .OR. OPCION = 0
           * Eligio la quinta opcion
           Clear
           return
   endcase

enddo
**********************************************************
procedure ALTAPRO


* set intensity off



do while .t.

         * apertura de archivos
         select 1
         Use PROVE index PRONRO,PRORAZ


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
 * set intensity on
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
   @  15,20  get CONT picture Replicate('!',3) valid (CONT="RI" .or. CONT="RNI")
   @ 16,20  get TEL1
   @ 17,20  get TEL2
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

enddo
return
****************************************************************************
procedure BAJAPRO

         * apertura de archivos
         select 1
         Use PROVE index PRONRO,PRORAZ

Do while .t.
   * Se declaran variables de trabajo
  NUME = 0         && Numero de identificador en la agenda


   @ 2,23,21,74 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 SAY "PROVEEDORES ***** bajas"
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

procedure MODIPRO

         * apertura de archivos
         select 1
         Use PROVE index PRONRO,PRORAZ

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




   @  4,43  get RAZO picture Replicate('!',30)
   @  5,43  get APEL picture Replicate('!',20)
   @  6,43  get NOMB picture Replicate('!',20)
   @  7,43  get DOMI picture replicate('!',30)
   @  8,43  get LOCA picture Replicate('!',20)
   @  9,43  get PROV picture Replicate('!',20)
   @ 10,43  get CPOS
   @ 11,43  get CUIT
   @ 12,43  get CONT picture Replicate('!',3)
   @ 13,43  get TEL1
   @ 14,43  get TEL2
   read
   * Validacion de datos

   if APEL = space(25)
      @ 24,0 say 'Debe ingresar nombre. [ENTER]'
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

enddo
return
***********************************************
procedure CONSUPRO

         * apertura de archivos
         select 1
         Use PROVE index PRONRO,PRORAZ

*set intensity off
do while .t.
        set order to 1
        set key -2 to VEntanaproveedor
   NUME=0
   RAZO=space(30)
   CANTIDAD = lastrec()
   @ 2,23,21,74 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 2,26 say "VIVERO ***** consultasÄÄÄCant. :"
   @ 2,58 say +CANTIDAD picture '9999'
   @ 3,25 say "NUMERO......:" get NUME picture '9999'
   @ 4,26 say "RAZON SOCIAL....:" get RAZO picture Replicate('!',30)

   read

   if (NUME = 0) .and. (RAZO = space(30))
      * Significa que no quiere mas consultas
      * Cierra base de datos e indices
      Close database
      Close index
      *set intensity on
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
  SGACR = SGACRE
  SGDEU = SGDEUD

   @ 03,25 say "NUMERO......:" get NUME
   @ 04,25 SAY "RAZON SOCIAL...:" get RAZO
   @ 05,25 SAY "APELLIDOS......:" get APEL
   @ 06,25 SAY "NOMBRES........:" get NOMB
   @ 07,25 SAY "DOMICILIO......:" get DOMI
   @ 08,25 SAY "LOCALIDAD......:" get LOCA
   @ 09,25 SAY "PROVINCIA......:" get PROV
   @ 10,25 SAY "CODIGO POSTAL..:" get CPOS
   @ 11,25 SAY "CUIT...........:" get CUIT
   @ 12,25 SAY "CONTRIBUYENTE..:" get CONT
   @ 13,25 SAY "TELEFONO1......:" get TEL1
   @ 14,25 SAY "TELEFONO2......:" get TEL2
   @ 15,25 say "saldo acreedor.:" get SGACR
   @ 16,25 say "saldo deudor...:" get SGDEU
 clear gets

      if SGACR > SGDEU
         @ 15,52 say (SGACR-SGDEU)
      elseif SGACR < SGDEU
         @ 16,52 say (SGDEU-SGACR)
      endif


    TECLA = inkey(0)
    @ 15,52 say replicate('°',13)
    @ 16,52 say replicate('°',13)

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
*set intensity on
return

**************************************************
Procedure LIST
do DEcide_salida
         * apertura de archivos
         select 1
         Use PROVE index PRONRO,PRORAZ

 set order to 2
 clear
 montomi=0
 @ 12,2 say"ingrese monto MINIMO para definir busqueda: " get montomi picture "999999.99"
 read
 montoma=0
 @ 14,2 say"ingrese monto MAXIMO para definir busqueda: " get montoma picture "999999.99"
 read
 clear
* list numero,razon,tipo,ncuit,domicilio,localidad,provincia,telefono1,sgacre,sgdeud to print off
?"    N               PROVEEDOR           ACREEDOR    DEUDOR      saldo"
?"    __________________________________________"
 list numero,razon,sgacre,sgdeud,(sgacre-sgdeud) for ((sgacre-sgdeud) >=montomi) .and. ((sgacre-sgdeud) <= montoma) .and. ((sgacre-sgdeud) <> 0) off
 d=0

 sum (sgacre-sgdeud) to d for ((sgacre-sgdeud) >=montomi) .and. ((sgacre-sgdeud) <= montoma)
?"Suma total de acuerdo al criterio elegido: ", d

 inkey(0)
set print off
set console on
return

********************************************
procedure COMPRAS
do DAtpro
set message to 24 center


do while .t.
   @ 21,00,23,80 BOX "ÚÄ¿³ÙÄÀ³ "
   @ 21,20 SAY "Registro de compras ******* menu de opciones****"
   opcion = 1
   @ 22,01 prompt ' Altas ' message '  Altas de compras                                           '
   @ 22,10 prompt ' Consultas ' message '         Consultas                                       '
   @ 22,24 prompt ' Retroceder ' message '  Vuelve al menu anterior                               '
   @ 22,38 prompt ' Modificaciones 'message' modifica el contedido de la orden de compra          '
   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA2
           Do ALTCOM
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 2
           * Eligio la cuarta opcion
           SAVE SCREEN TO PANTALLA2
           Do CONCOM
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 3 .OR. OPCION = 0
           Clear
           return
      case opcion = 4
           *eligio la cuarta opcion
           save screen to pantalla2
           do MODCOM
           RESTORE SCREEN FROM PANTALLA2
   endcase

enddo
****************************************************************
procedure ALTCOM
set intensity on

*set intensity off



   * Abre archivos PROVE.DBF y COMPRA.DBF
   select 1
   Use PROVE index PRONRO,PRORAZ
   select 2
   Use COMPRA index COMORD
do while .t.

   * Se declaran variables de trabajo
   ORDC = 0
   FECH = space(10)
   NROF = space(13)

   NROP = 0
   NOMB = space(20)
   DIRE = space(30)
   LOCA = space(20)
   CPOS = space(4)
   TEL1 = space(20)
   TEL2 = space(20)

   PACRE = 0  && parcial diario acreedor
   PDEUD = 0  && parcial diario deudor
   VECHE = 0  && valores entregados en cheques
   VEEFE = 0  && valores entregados en efectivo
   VRCHE = 0  && valores recibidos en cheque
   VREFE = 0  && valores recibidos en efectivo
select 2

      N = lastrec()
      ORDC = N+1

      AUX = ORDC       && seguridad


   @ 2,17 get ORDC
   read


      if ORDC>AUX     && seguridad
         clear        && seguridad
         return       && seguridad
      endif           && seguridad



   if ORDC = 0
      * Significa que no quiere dar mas altas
      * Cierra base de datos e indices
      Close database
      Close index
      *set intensity on
      clear
      return  && Retorna al programa anterior o estado anterior.
   endif

   seek ORDC   && Busca en el archivo si existe el numero que ingreso

   if .not. eof()  && Si no es fin de archivo significa que lo encontro
      @ 24,0 say 'Este numero ya fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif



   FECH = dtoc(DATE())
   @ 2,35 get FECH picture "99-99-9999"
   read

   @ 2,60 get NROF
   read

   select 1
   @ 04,19  get NROP
   read
       if NROP = 0
          loop
       endif
   seek NROP

   if eof()
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif
   NOMB = RAZON
   NOMB = ALLTRIM(RAZON)
   @ 04,48 say + NOMB
   DIRE = DOMICILIO
   @ 05,15 SAY + DIRE
   LOCA = LOCALIDAD
   @ 05,53 SAY + LOCA
   CPOS = CPOSTAL
   @ 06,13 SAY + CPOS
   TEL1 = TELEFONO1
   TEL2 = TELEFONO2
   @ 06,40 SAY + TEL1
   @ 06,63 SAY + TEL2

   SGACR = SGACRE
   SGDEU = SGDEUD

             @ 13,50 say  SGACR
             @ 14,50 say  SGDEU


      if SGACR > SGDEU
         @ 13,63 say (SGACR-SGDEU)
         @ 14,63 say space(13)
      elseif SGACR <= SGDEU
         @ 14,63 say (SGDEU-SGACR)
         @ 13,63 say space(13)
      endif



   select 2



   PACRE = PACREE
   PDEUD = PDEUDO
   VECHE = VECHEQ
   VEEFE = VEEFEC
   VRCHE = VRCHEQ
   VREFE = VREFEC

   @ 09,49   get PACRE    && boleta diaria de mercaderia que trae
   @ 10,49   get PDEUD    && boleta diaria de mercaderia que lleva
   @ 17,12   get VECHE
   @ 17,29   get VEEFE
   @ 17,47   get VRCHE
   @ 17,67   get VREFE
   read


             SGACR = SGACR + PACRE - VEEFE - VECHE
             SGDEU = SGDEU + PDEUD - VREFE - VRCHE

               if SGACR < 0
                  SGDEU = SGDEU-SGACR
                  SGACR = 0
               endif
               if SGDEU < 0
                  SGACR = SGACR-SGDEU
                  SGDEU = 0
               endif

             @ 13,50 say  SGACR
             @ 14,50 say  SGDEU

      if SGACR > SGDEU
         @ 13,63 say (SGACR-SGDEU)
         @ 14,63 say space(13)
      elseif SGACR < SGDEU
         @ 14,63 say (SGDEU-SGACR)
         @ 13,63 say space(13)
      endif

             set message to 20 center
         tone(500,1)
         opcion = 1
         @ 21,0 say space(80)
         @ 22,0 say space(80)
         @ 23,0 say space(80)
         @ 24,0 say space(80)
         @ 25,0 say space(80)
         @ 19,30 prompt 'CONFIRMA' message 'confirma la operacion      '
         @ 19,45 prompt 'REPITE  ' message 'repite el tipeo de datos   '
         menu to opcion

         do case
            case opcion = 2
             @ 13,50 say space(13)
             @ 14,50 say space(13)
             @ 13,63 say space(13)
             @ 14,63 say space(13)
            loop
            case opcion = 1
         endcase



         @ 19,01 say space(79)
             set message to 25 center
   append blank
   Replace ORDCOM         With  ORDC
   Replace FECHA          With  ctod(FECH)
   Replace NROFAC         With  NROF
   replace NROPRO         with  NROP
   replace NOMBRE         with  NOMB
   replace DIRECC         with  DIRE
   replace LOCALI         with  LOCA
   replace CPOSTA         with  CPOS
   replace TELEF1         with  TEL1
   replace TELEF2         with  TEL2
   replace PACREE         with  PACRE
   replace PDEUDO         with  PDEUD
   replace VECHEQ         with  VECHE
   replace VEEFEC         with  VEEFE
   replace VRCHEQ         with  VRCHE
   replace VREFEC         with  VREFE
   tone(1000,.5)
   @ 24,10 say"Ctrl-W para salir grabando el texto de la ventana"
   replace DETALL with memoedit(DETALL,9,4,13,35,.T.)
   select 1
   replace SGACRE         with  SGACR
   replace SGDEUD         with  SGDEU


   @ 13,50 say space(13)
   @ 14,50 say space(13)



   keyboard chr(48)+chr(13)


enddo
return
************************************************
Procedure CONCOM
set message to 24 center
*apertura de archivos


    select 1
    use PROVE index PRONRO,PRORAZ
    select 2
    use COMPRA index COMORD,COMFECH

*    index on dtos(fecha) to comfech

do while .T.
   clear screen
     select 1


   @ 00,10,18,70 BOX "ÚÄ¿³ÙÄÀ³ "

  ARRANQUE=3

  @ 02,20 prompt "         ORDEN INDIVIDUAL (por orden de compra)  "
  @ 04,20 prompt "         LISTADO POR IMPRESORA (por proveedor)   "
  @ 06,20 prompt "                  MENU ANTERIOR                  "
  @ 08,20 prompt "                ORDENES por FECHA                "
  @ 10,20 prompt "             Ordenes entre dos fechas            "
  @ 12,20 prompt "  LISTADO POR PANTALLA O IMPRESORA SEGUN CRITERIO"
  @ 14,20 prompt "             RESUMEN MENSUAL DE COMPRAS          "
  @ 16,20 prompt "        ULTIMAS 23 ORDENES (por proveedor)       "

  menu to ARRANQUE
  do case
     case ARRANQUE = 1
          DO INdividual
          return
     case ARRANQUE = 2
             NROP = 0
             clear
             tone (900,1)
             @ 15,25 say"ingrese numero de proveedor :" get NROP picture "9999"
              read

               if NROP = 0
                  close database
                  close index
                  clear
                  return
               endif

               seek NROP

                if  eof()    && significa que no lo encontro
                    loop
                endif
                 numero = NROP
                 proveedor = RAZON

                Do IMpres
                return
     case ARRANQUE = 3
          return
     case ARRANQUE = 4
          do ORDFECHA
          return
     case ARRANQUE = 5
          do ENtredosfechas
          return

     case ARRANQUE = 6
          do SAldos
          return
     case ARRANQUE = 7
     do SUcompras
        return
     case ARRANQUE = 8
   endcase


     NROP = 0
     clear
     tone (900,1)
     @ 15,25 say"ingrese numero de proveedor :" get NROP picture "9999"
     read

     if NROP = 0
        close database
        close index
        clear
        return
     endif

     seek NROP

          if  eof()    && significa que no lo encontro
             loop
          endif
     numero = NROP
     proveedor = RAZON

select 2


    @ 24,0 say space(80)
    @ 0,0 say"transacciones con el proveedor :" +proveedor+space(40)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"ordcom "+" "+"fecha  "+" "+"trajo_hoy"+" "+"llevo_hoy"+" "+"entre_ch"+" "+"entre_ef"+" "+"dio_che"+" "+"  dio_efe"
    @ 3,0 say"_____________________________________________________________________________"

     list ordcom fecha pacree pdeudo  vecheq veefec vrcheq vrefec for nropro=nrop off
  *  list ordcom fecha pacree pdeudo vecheq veefec vrcheq vrefec for (nropro=nrop) .and. (month(fecha) >= month(date())-3) .and. (year(fecha)=year(date())) off
    pac=0
    pde=0
    vec=0
    vee=0
    vrc=0
    vre=0
    sum PACREE,PDEUDO,VECHEQ,VEEFEC,VRCHEQ,VREFEC to pac,pde,vec,vee,vrc,vre for NROPRO=nrop

    ?"acreedor :",pac
    ?"deudor   :",pde
    ?"che_ent  :",vec
    ?"efe_ent  :",vee
    ?"che_rec  :",vrc
    ?"efe_rec  :",vre
    ?"SALDO___________________________: ",(pac+vrc+vre)-(pde+vec+vee)

    @ 0,0 say"transacciones con el proveedor :" +proveedor+space(40)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"ordcom "+" "+"fecha  "+" "+"trajo_hoy"+" "+"llevo_hoy"+" "+"entre_ch"+" "+"entre_ef"+" "+"dio_che"+" "+"  dio_efe"
    @ 3,0 say"_____________________________________________________________________________"
    inkey(0)


return
ENDDO

**********************************************
*procedimiento de consulta individual
*********************************************
procedure INdividual
select 2
do while .t.
  clear

*  FE=space(10)
   FE=0
   @ 2,17 say"ingrese orden de compra :" get FE picture '9999'
   @ 24,15 say"Con orden de compra 0 y Enter vuelve al menu anterior"
    READ

   if FE = 0
      return  && Retorna al programa anterior o estado anterior.
   endif

    if FE != 0
      seek FE   && Busca en el archivo si existe el numero que ingreso
          if eof()  && Si  es fin de archivo significa que no lo encontro
           @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
            inkey(0)     && Espera a que se presione una tecla
           loop         &&  Va al comienzo del ciclo detrabajo
          endif
     endif

  do DAtpro       && carga el texto de la ficha

   * Si  encontro en el archivo el numero ingresado
   * carga datos en variables
@ 20,0 say" Puede modificar la ventana de edicion AHORA"
@ 21,0 say" Ctrl-W            ---------> graba ventana          "
@ 22,0 say" Esc + Esc         ---------> sale                  "
@ 23,0 say" Esc  +"+chr(24)+"        -----> anterior           "
@ 24,0 say" Esc  +"+chr(25)+"        --------->  siguiente      "
TECLA = 0
set cursor off
do while TECLA != 27
   ORDC=ORDCOM
   FECH=FECHA
   NROF=NROFAC
   NROP=NROPRO
   NOMB=NOMBRE
   DIRE=DIRECC
   LOCA=LOCALI
   CPOS=CPOSTA
   TEL1=TELEF1
   TEL2=TELEF2
   PACR=PACREE
   PDEU=PDEUDO
   VECH=VECHEQ
   VEEF=VEEFEC
   VRCH=VRCHEQ
   VREF=VREFEC

   @ 2,17  get ORDC
   @ 2,35  get FECH
   @ 2,60  get NROF
   @ 4,19  get NROP
   @ 4,48  get NOMB
   @ 5,15  get DIRE
   @ 5,53  get LOCA
   @ 6,13  get CPOS
   @ 6,40  get TEL1
   @ 6,63  get TEL2
   @ 9,49  get PACR
   @ 10,49 get PDEU
   @ 17,12 get VECH
   @ 17,29 get VEEF
   @ 17,47 get VRCH
   @ 17,67 get VREF
 clear gets

set cursor on
   tone(1000,1)

  replace DETALL with memoedit(DETALL,9,4,13,35,.T.)


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
    return
enddo
************************************************
*procedimiento para imprimir las transacciones historicas
***************************************************
procedure IMpres

select 2
set device to print
set printer to C:\users\oem\desktop\texto.txt
set console off

*@ 0,0 say chr(27)+chr(15)   && impresion condensada

    @ 0,0 say"transacciones con el proveedor :" +proveedor+space(40)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"ordcom "+" "+"fecha  "+" "+"trajo_hoy"+" "+"llevo_hoy"+" "+"entre_ch"+" "+"entre_ef"+" "+"dio_che"+" "+"  dio_efe"
    @ 3,0 say"_____________________________________________________________________________"

    list ordcom fecha pacree pdeudo  vecheq veefec vrcheq vrefec for nropro=nrop to print off
   eject
set console on
set device to screen

return

************************************************
*procedimiento para mostrar por pantalla los saldos distintos de cero
***************************************************
procedure SAldos

select 1
do DEcide_salida
    clear
    IMPORT=0
    @ 10,20 say"importe a partir del cual salen las deudas : " get IMPORT picture "9999.99"
    read
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"  PROVEEDOR                       ACREEDOR  DEUDOR   SALDO"
    @ 3,0 say"_____________________________________________________________________________"
    list RAZON,SGACRE,SGDEUD,(SGACRE-SGDEUD) for (SGACRE-SGDEUD) >= IMPORT OFF
  *  list RAZON SGACRE SGDEUD  OFF
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"  PROVEEDOR                       ACREEDOR  DEUDOR"
    @ 3,0 say"_____________________________________________________________________________"
inkey(0)
set print off
set console on
return
***************************************************
*procedimiento para balancear los acreedores y deudores
***************************************************
procedure BAlance

 do while .t.
    clear screen
    SELECT 2
    use compra index comord

    ACR = 0
    DEU = 0
    ECH = 0
    EEF = 0
    RCH = 0
    REF = 0

    SUM PACREE,PDEUDO,VECHEQ,VEEFEC,VRCHEQ,VREFEC TO ACR,DEU,ECH,EEF,RCH,REF

        @ 4,20 say "B A L A N C E    D E    C O M P R A S "
        @ 5,20 SAY"---------------------------------------"
        @ 10,20 SAY"suma de compras a pagar:"
        @ 10,45 say ACR
        @ 12,20 say "suma de saldos a favor de GASPERI:"
        @ 12,56 say DEU
        @ 14,20 SAY"suma de cheques entregados:"
        @ 14,48 say ECH
        @ 16,20 say"suma de efectivo entregado:"
        @ 16,48 say EEF
        @ 18,20 say"suma de cheques recibidos:"
        @ 18,48 say RCH
        @ 20,20 say"suma de efectivo recibido:"
        @ 20,48 say REF

        @ 22,20 say"BALANCE GENERAL........:"
        @ 22,48 say -((acr-deu)-(ech+eef)+(rch+ref))

        inkey(0)
     return
 enddo

*************************************************************
* procedimiento para buscar ordenes de compra por fecha
*************************************************************
procedure ORDFECHA

DO WHILE .T.

select 2
set order to 2

index on dtos(fecha) to COMFECH

     FECH = ctod("  /  /  ")
     clear
     tone (900,1)
     @ 15,25 say"ingrese fecha :" get FECH
     read


     if EMPTY(FECH)
        close database
        close index
        clear
        set order to 1
        return
     endif

     * FECH=dtos(FE)
     seek FECH

          if  eof()    && significa que no lo encontro
             loop
          endif




    @ 24,0 say space(80)
    @ 0,0 say"transacciones el dia :" +dtoc(FECH)+space(40)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"ordcom"+" "+"prove           "+"trajo_hoy"+" "+"llevo_hoy"+" "+"ent_cheq."+" "+"ent_efe."+" "+"dio_cheq."+" "+"dio_efec."
    @ 3,0 say"_____________________________________________________________________________"

  *  list ordcom nombre pacree pdeudo  vecheq veefec vrcheq vrefec for FECHA=FECH off
    LIST alltrim(ORDCOM) NOMBRE PACREE PDEUDO VECHEQ VEEFEC VRCHEQ VREFEC for FECHA=FECH off

    @ 0,0 say"transacciones el dia :" +dtoc(FECH)+space(40)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"ordcom"+" "+"prove           "+"trajo_hoy"+" "+"llevo_hoy"+" "+"ent_cheq."+" "+"ent_efe."+" "+"dio_cheq."+" "+"dio_efec."
    @ 3,0 say"_____________________________________________________________________________"
    inkey(0)

set order to 1
return
ENDDO


*******************************************

procedure MODCOM

SELECT 1
    use PROVE index PRONRO,PRORAZ
select 2
   Use COMPRA index COMORD
do while .t.

   * Se declaran variables de trabajo
   ORDC = 0
   FECH = space(10)
   NROF = space(13)

   NROP = 0
   NOMB = space(20)
   DIRE = space(25)
   LOCA = space(20)
   CPOS = space(4)
   TEL1 = space(20)
   TEL2 = space(20)

   PACRE = 0  && parcial diario acreedor
   PDEUD = 0  && parcial diario deudor
   VECHE = 0  && valores entregados en cheques
   VEEFE = 0  && valores entregados en efectivo
   VRCHE = 0  && valores recibidos en cheque
   VREFE = 0  && valores recibidos en efectivo


   @ 2,17 get ORDC
   read


   if ORDC = 0
      * Significa que no quiere dar mas altas
      * Cierra base de datos e indices
      Close database
      Close index
      *set intensity on
      clear
      return  && Retorna al programa anterior o estado anterior.
   endif

   seek ORDC   && Busca en el archivo si existe el numero que ingreso


   if eof()
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif

  NROP = NROPRO
  @ 04,19 say + NROP
   NOMB = NOMBRE
   NOMB = ALLTRIM(NOMBRE)
   @ 04,48 say + NOMB
   DIRE = DIRECC
   @ 05,15 SAY + DIRE
   LOCA = LOCALI
   @ 05,53 SAY + LOCA
   CPOS = CPOSTA
   @ 06,13 SAY + CPOS
   TEL1 = TELEF1
   TEL2 = TELEF2
   @ 06,40 SAY + TEL1
   @ 06,63 SAY + TEL2
select 1

  seek nrop  && mueve el puntero de busqueda a ese registro

   SGACR = SGACRE
   SGDEU = SGDEUD

  *    if SGACR > SGDEU
  *       @ 13,63 say (SGACR-SGDEU)
  *       @ 14,63 say space(13)
  *    elseif SGACR < SGDEU
  *       @ 14,63 say (SGDEU-SGACR)
  *       @ 13,63 say space(13)
  *    endif


select 2

   PACRE = PACREE
   PDEUD = PDEUDO
   VECHE = VECHEQ
   VEEFE = VEEFEC
   VRCHE = VRCHEQ
   VREFE = VREFEC

   * inversion de los saldos generales para poder sumar nuevamente
             SGACR = SGACR - PACRE + VEEFE + VECHE
             SGDEU = SGDEU - PDEUD + VREFE + VRCHE


   @ 09,49   get PACRE    && boleta diaria de mercaderia que trae
   @ 10,49   get PDEUD    && boleta diaria de mercaderia que lleva
   @ 17,12   get VECHE
   @ 17,29   get VEEFE
   @ 17,47   get VRCHE
   @ 17,67   get VREFE
   read


             SGACR = SGACR + PACRE - VEEFE - VECHE
             SGDEU = SGDEU + PDEUD - VREFE - VRCHE

               if SGACR < 0
                  SGDEU = SGDEU-SGACR
                  SGACR = 0
               endif
               if SGDEU < 0
                  SGACR = SGACR-SGDEU
                  SGDEU = 0
               endif

             @ 13,50 say  SGACR
             @ 14,50 say  SGDEU

      if SGACR > SGDEU
         @ 13,63 say (SGACR-SGDEU)
         @ 14,63 say space(13)
      elseif SGACR < SGDEU
         @ 14,63 say (SGDEU-SGACR)
         @ 13,63 say space(13)
      endif


             set message to 20 center
         tone(500,1)
         opcion = 1
         @ 21,0 say space(80)
         @ 22,0 say space(80)
         @ 23,0 say space(80)
         @ 24,0 say space(80)
         @ 25,0 say space(80)
         @ 19,30 prompt 'CONFIRMA' message 'confirma la operacion      '
         @ 19,45 prompt 'REPITE  ' message 'repite el tipeo de datos   '
         menu to opcion

         do case
            case opcion = 2
             @ 13,50 say space(13)
             @ 14,50 say space(13)
             @ 13,63 say space(13)
             @ 14,63 say space(13)
            loop
            case opcion = 1
         endcase



         @ 19,01 say space(79)
             set message to 25 center

   replace PACREE         with  PACRE
   replace PDEUDO         with  PDEUD
   replace VECHEQ         with  VECHE
   replace VEEFEC         with  VEEFE
   replace VRCHEQ         with  VRCHE
   replace VREFEC         with  VREFE
   tone(1000,.5)
   @ 24,10 say"Ctrl-W para salir grabando el texto de la ventana"
   replace DETALL with memoedit(DETALL,9,4,13,35,.T.)
  select 1
  replace SGACRE         with  SGACR
  replace SGDEUD         with  SGDEU


   @ 13,50 say space(13)
   @ 14,50 say space(13)



   keyboard chr(48)+chr(13)


enddo
return
*************************************************
*Procedure para abrir un BROWSE de clientes
**************************************************

procedure VEntanaproveedor


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
          declare vector[5]
          vector[1]="numero"
          vector[2]="razon"
          vector[3]="sgacre"
          vector[4]="sgdeud"
          vector[5]="telefono1"


          dbedit(3,3,16,76,vector)

      SET ORDER TO 1

   restore screen from ACTUAL
   set key -2 to     && desactiva F3 como funcion dbedit
   set softseek off  && desactiva la busqueda "blanda"
return
*******************************************
*procedure para saber la suma total de lo comprado mas cheques entregados
*******************************************
procedure SUcompras
select 2

clear screen
?" MES         COMPRAS                                        SUMA"
?"                              cheques        efectivo            "
?"-----------------------------------------------------------------"
fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=1
?"ENERO:....", fa , che , efec
acumulador=fa

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=2
?"FEBRERO..:", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=3
?"MARZO:....", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=4
?"ABRIL:....", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=5
?"MAYO:.....", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=6
?"JUNIO....:", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=7
?"JULIO:....", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=8
?"AGOSTO:...", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=9
?"SETIEMBRE:", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=10
?"OCTUBRE:..", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for month(fecha)=11
?"NOVIEMBRE:", fa , che , efec , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efec=0
sum pacree to fa VECHEQ to che VEEFEC to efec for (month(fecha)=12).and.(year(fecha)=2003)
?"DICIEMBRE:", fa , che , efec , fa+acumulador
acumulador=fa+acumulador
inkey(0)
return
*****************************************************************************
*lo que sigue era una opcion anterior y la dejo por si la quiero usar algun dia
*sum PACREE to sumatoria
*@ 10,15 say "Sumatoria de Compras :"
*@ 10,38 say sumatoria
*use

* Abre archivo agenda indexado por numero
*Use CHE_TERC index CHE_IMP
*im=0
*tot=0
*sum IMPORTE to tot for year(FECHA_ENT)=2002
*@ 14,15 say"suma total de cheques:"
*@ 14,38 say tot
*ENTRE=space(20)
*@ 24,15 say"suma de cheques entregados a : " get ENTRE
*read
*sum IMPORTE to im for ENTREGADO=ENTRE .and. year(FECHA_ENT)=2001

*@ 17,15 say"suma de acuerdo a pregunta:"
*@ 17,43 say im

*inkey(0)
*use
*return

*************************************************************
* procedimiento para buscar ordenes de compra entre DOS FECHAS
*************************************************************
procedure ENtredosfechas

DO WHILE .T.

select 2
set order to 2

index on dtos(fecha) to COMFECH

     FECH = ctod("  /  /  ")
     clear
     tone (1200,1)
     @ 15,25 say"ingrese fecha :" get FECH
     read
     FECH1=FECH

     if EMPTY(FECH)
        close database
        close index
        clear
        set order to 1
        return
     endif

     @ 17,25 say "ingrese segunda fecha :" get FECH
     read
     FECH2=FECH

     if EMPTY(FECH)
        close database
        close index
        clear
        set order to 1
        return
     endif

     seek FECH1

          if  eof()    && significa que no lo encontro
             loop
          endif

     seek FECH2

          if  eof()    && significa que no lo encontro
             loop
          endif
    do DEcide_salida
    clear

    @ 24,0 say space(80)
    @ 0,0 say"transacciones del dia :" +dtoc(FECH1)+space(4)+dtoc(FECH2)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"ordcom"+"  "+"fecha      "+"proveedor      "+"         "+"trajo"+"    "+"llevo"
    @ 3,0 say"_____________________________________________________________________________"

  *  list ordcom nombre pacree pdeudo  vecheq veefec vrcheq vrefec for FECHA=FECH off
  *  LIST alltrim(ORDCOM) NOMBRE PACREE PDEUDO VECHEQ VEEFEC VRCHEQ VREFEC for ((FECHA >= FECH1) .and. (FECHA <= FECH2)) off
     LIST ORDCOM FECHA NOMBRE PACREE PDEUDO VECHEQ VEEFEC for ((FECHA >= FECH1) .and. (FECHA <= FECH2)) off


  a=0
  b=0
  c=0
  d=0
  e=0
  f=0

sum PACREE,PDEUDO,VECHEQ,VEEFEC,VRCHEQ,VREFEC to a,b,c,d,e,f for ((FECHA >= FECH1) .and. (FECHA <= FECH2))
?"acreedor..........:",a
?"deudor............:",b
?"entregue cheques...",c
?"entregue efectivo.:",d
?"recibi cheques....................:",e
?"recibi efectivo...................:",f

    @ 24,0 say space(80)
    @ 0,0 say"transacciones del dia :" +dtoc(FECH1)+space(4)+dtoc(FECH2)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"ordcom"+"  "+"fecha      "+"proveedor      "+"         "+"trajo"+"    "+"llevo"
    @ 3,0 say"_____________________________________________________________________________"

inkey(0)
set print off
set console on
set order to 1
return
ENDDO


*******************************************
