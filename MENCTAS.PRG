
* Programa.....: menctas.prg
* Sitema.......: tete
* Base de datos: ctasctes.dbf ; clipro.dbf
* indice.......: clinro.ntx ; cliraz.ntx  ; ctasope.ntx ; ctasfech.ntx ; ctascli.ntx
* Autor........: jose e. gasperi
* Fecha........: 14/7/96
**************************************************************************************



do DAtclien
set message to 24 center


do while .t.
   @ 21,00,23,80 BOX "ÚÄ¿³ÙÄÀ³ "
   @ 21,20 SAY "Registro de Cuentas Corrientes ******* menu de opciones****"
   opcion = 1
   @ 22,05 prompt ' Registrar ' message '  registro de deuda en cta cte                                 '
   @ 22,20 prompt ' Consultas ' message '      Consultas de ctas ctes                                   '
   @ 22,35 prompt ' Menu anterior ' message '     Vuelve al menu anterior                               '
   @ 22,50 prompt ' Modificaciones 'message  ' Modifica transaccion por numero                           '
   menu to opcion

   do case
      case opcion = 1
           * Eligio la primer opcion
           SAVE SCREEN TO PANTALLA2
           Do REGICTAS
           RESTORE SCREEN FROM PANTALLA2
      case opcion = 2
           * Eligio la cuarta opcion
           SAVE SCREEN TO PANTALLA2
           Do CONSUCTAS
           RESTORE SCREEN FROM PANTALLA2

      case opcion = 3 .OR. OPCION = 0
           Clear
           close database
           close index
           return

      case opcion = 4
           *eligio la cuarta opcion
           save screen to pantalla2
           do MODCTAS
           RESTORE SCREEN FROM PANTALLA2
   endcase

enddo
****************************************************************
procedure REGICTAS


   * Abre archivos CLIPRO.DBF y CTASCTES.DBF
   select 1
   Use CLIPRO index CLINUM,CLIRAZ
   select 2
   Use CTASCTES index CTASOPE


do while .t.

   * Se declaran variables de trabajo
   AUXILIAR=0     && seguridad
   OPER = 0
   FECH = space(10)
   NROF = space(13)

   NROC = 0
   NOMB = space(20)
   DIRE = space(25)
   LOCA = space(20)
   CPOS = space(4)
   TEL1 = space(10)
   TEL2 = space(10)

   PACRE = 0  && parcial diario acreedor
   PDEUD = 0  && parcial diario deudor
   VECHE = 0  && valores entregados en cheques
   VEEFE = 0  && valores entregados en efectivo
   VRCHE = 0  && valores recibidos en cheque
   VREFE = 0  && valores recibidos en efectivo
select 2

      N = lastrec()
      OPER = N+1
      AUXILIAR = OPER    && seguridad

    @ 2,17 get OPER
   read

  if OPER > AUXILIAR                &&  seguridad
     clear                         && seguridad
     return                        &&  seguridad
  endif                            &&   seguridad

   if OPER = 0
      * Significa que no quiere dar mas altas
      * Cierra base de datos e indices
      Close database
      Close index
      *set intensity on
      clear
      return  && Retorna al programa anterior o estado anterior.
   endif

   seek OPER   && Busca en el archivo si existe el numero que ingreso

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
   @ 04,19  get NROC
   read
       if NROC = 0
          loop
       endif
   seek NROC

   if eof()
      @ 24,0 say 'Este numero no fue dado de alta. [ENTER]'
      inkey(0)     && Espera a que se presione una tecla
      loop         &&  Va al comienzo del ciclo detrabajo
   endif
   NOMB = RAZON
   DENO = NOMB   && esto es para asignar una variable a ctasctes.dbf
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

   SGACR = ACREEDOR
   SGDEU = DEUDOR

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



   PACRE = ACREE
   PDEUD = DEUDO
   VECHE = ECHEQ
   VEEFE = EEFEC
   VRCHE = RCHEQ
   VREFE = REFEC

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
                  SGDEU = SGDEU-(SGACR)
                  SGACR = 0
               endif
               if SGDEU < 0
                  SGACR = SGACR-(SGDEU)
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
             @ 13,63 SAY SPACE(13)
             @ 14,63 SAY SPACE(13)
            loop
            case opcion = 1
         endcase



         @ 19,01 say space(79)
             set message to 25 center
   append blank
   Replace OPERAC         With  OPER
   Replace FECHA          With  ctod(FECH)
   Replace FACTUR         With  NROF
   replace NROCLI         with  NROC
   replace DENOMI        with  DENO
   replace ACREE         with  PACRE
   replace DEUDO         with  PDEUD
   replace ECHEQ         with  VECHE
   replace EEFEC         with  VEEFE
   replace RCHEQ         with  VRCHE
   replace REFEC         with  VREFE
   tone(1000,.5)
   @ 24,10 say"Ctrl-W para salir grabando el texto de la ventana"
   replace OBSERV with memoedit(OBSERV,9,4,13,35,.T.)
   select 1
   replace DEUDOR         with  SGDEU
   replace ACREEDOR       with  SGACR

   @ 13,50 say space(13)
   @ 14,50 say space(13)


   keyboard chr(48)+chr(13)


enddo
return
************************************************
Procedure CONSUCTAS
set message to 24 center

   * Abre archivos CLIPRO.DBF y CTASCTES.DBF
   select 1
   Use CLIPRO index CLINUM,CLIRAZ
   select 2
   Use CTASCTES index CTASOPE



            * index on dtos(fecha) to comfech


do while .T.
   clear screen


   select 1

   @ 00,10,18,70 BOX "ÚÄ¿³ÙÄÀ³°"

  ARRANQUE=3

  @ 02,20 prompt  "OPERACION INDIVIDUAL (por numero de operacion)"
  @ 04,20 prompt "LISTADO POR IMPRESORA (por cliente)"
  @ 06,20 prompt "MENU ANTERIOR"
  @ 08,20 prompt "BALANCE DE DEUDAS"
  @ 10,20 prompt "resumen de facturacion mes a mes"
  @ 12,20 prompt "OPERACIONES POR FECHA"
  @ 14,20 prompt "Operaciones entre dos fechas"
  @ 16,20 prompt "ULTIMAS 23 OPERACIONES (por cliente)"


  menu to ARRANQUE
  do case
     case ARRANQUE = 1
          DO INdivi
          return
     case ARRANQUE = 2
             NROC = 0
             clear
             tone (900,1)
             @ 15,25 say"ingrese numero de CLIENTE :" get NROC picture "9999"
              read

               if NROC = 0
                  close database
                  close index
                  clear
                  return
               endif

               seek NROC

                if  eof()    && significa que no lo encontro
                    loop
                endif
                 numero = NROC
                 cliente = RAZON

                Do IMpre

                return
     case ARRANQUE = 3
          return
     case ARRANQUE = 4
          DO BAlan
          return

     case ARRANQUE = 5
          DO REsumen_mensual
          return

     case ARRANQUE = 6
          DO ORDDIA
          return
     case ARRANQUE = 7
          DO ENntre_fechas
          return
     case ARRANQUE = 8
   endcase


     NROC = 0
     clear
     tone (900,1)
     @ 15,25 say"ingrese numero de CLIENTE :" get NROC picture "9999"
     read

     if NROC = 0
        close database
        close index
        clear
        return
     endif

     seek NROC

          if  eof()    && significa que no lo encontro
             loop
          endif
     numero = NROC
     cliente = RAZON

   select 2


    @ 24,0 say space(80)
    @ 0,0 say"transacciones con el cliente :" +cliente+space(40)

    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"operacion "+" "+"fecha  "+" "+"cred_hoy"+" "+"deuda_hoy"+" "+"entre_ch"+" "+"entre_ef"+" "+"dio_che"+" "+"  dio_efe"
    @ 3,0 say"_____________________________________________________________________________"

     list operac fecha acree deudo  echeq eefec rcheq refec for nrocli=nroc off
   * list operac fecha acree deudo  echeq eefec rcheq refec for (nrocli=nroc) .and. (month(fecha) >= month(date())-3) .and. (year(fecha)=year(date())) off
    ac=0
    de=0
    ec=0
    ee=0
    rc=0
    re=0
    sum ACREE,DEUDO,ECHEQ,EEFEC,RCHEQ,REFEC to ac,de,ec,ee,rc,re for NROCLI=nroc
    ?"credito :",ac
    ?"deudor  :",de
    ?"che_ent :",ec
    ?"efe_ent :",ee
    ?"che_rec :",rc
    ?"efe_rec :",re
    ?"SALDO________________________ :",-((de+ec+ee)-(ac+rc+re))


    @ 0,0 say"transacciones con el cliente :" +cliente+space(40)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"operacion "+" "+"fecha  "+" "+"credito"+" "+"$ factura"+" "+"entre_ch"+" "+"entre_ef"+" "+"dio_che"+" "+"  dio_efe"
    @ 3,0 say"_____________________________________________________________________________"

    inkey(0)

 RETURN

enddo

**********************************************
*procedimiento de consulta individual
*********************************************
procedure INdivi
select 2
do while .t.
  clear

*  FE=space(10)
   FE=0
   @ 2,17 say"ingrese numero de operacion :" get FE picture '9999'
   @ 24,15 say"Con orden de compra 0 y Enter vuelve al menu anterior"
    READ
 * FE=dtos(ctod(FE))

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

  do DAtclien       && carga el texto de la ficha

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
   OPER=OPERAC
   FECH=FECHA
   NROF=FACTUR
   NROC=NROCLI
   DENO=DENOMI
   PACR=ACREE
   PDEU=DEUDO
   VECH=ECHEQ
   VEEF=EEFEC
   VRCH=RCHEQ
   VREF=REFEC

   @ 2,17  get OPER
   @ 2,35  get FECH
   @ 2,60  get NROF
   @ 4,19  get NROC
   @ 4,48  get DENO
   @ 9,49  get PACR
   @ 10,49 get PDEU
   @ 17,12 get VECH
   @ 17,29 get VEEF
   @ 17,47 get VRCH
   @ 17,67 get VREF
 clear gets

set cursor on
   tone(1000,1)

  replace OBSERV with memoedit(OBSERV,9,4,13,35,.T.)


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
    RETURN
enddo
************************************************
*procedimiento para imprimir las transacciones historicas
***************************************************
procedure IMpre

select 2

    ac=0
    de=0
    ec=0
    ee=0
    rc=0
    re=0
sum ACREE,DEUDO,ECHEQ,EEFEC,RCHEQ,REFEC to ac,de,ec,ee,rc,re for NROCLI=nroc

set console off
set device to print
set printer to c:\users\oem\desktop\texto.txt

    @ 0,0 say"transacciones con el cliente :" +cliente+space(40)
    @ 1,0 say"-----------------------------------------------------------------------------"
    @ 2,0 say"operacion "+" "+"fecha  "+" "+"credito"+" "+"$ factura"+" "+"entre_ch"+" "+"entre_ef"+" "+"dio_che"+" "+"  dio_efe"
    @ 3,0 say"_____________________________________________________________________________"

    list operac fecha acree deudo  echeq eefec rcheq refec factur for nrocli=nroc to print off
@ 0,0 say"creditos asignados:"
@ 0,20 say ac
@ 1,0 say"suma de facturas:"
@ 1,20 say de
@ 2,0 say"cheques entregados:"
@ 2,20 say ec
@ 3,0 say"efectivo entregado:"
@ 3,20 say ee
@ 4,0 say"cheques recibidos:"
@ 4,20 say rc
@ 5,0 say"efectivo recibido:"
@ 5,20 say re
   eject


set console on
set device to screen


return


***************************************************
procedure REsumen_mensual
clear
select 2
?" MES         FACTURACION       cheques      efectivo         SUMA"
?"---------------------------------------------------------------------"

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=1
?"ENERO:....", fa , che , efe
acumulador=fa

fa=0
che=0
efe=0

sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=2
?"FEBRERO..:", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=3
?"MARZO:....", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=4
?"ABRIL:....", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=5
?"MAYO:.....", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=6
?"JUNIO....:", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=7
?"JULIO:....", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=8
?"AGOSTO:...", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=9
?"SETIEMBRE:", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=10
?"OCTUBRE:..", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=11
?"NOVIEMBRE:", fa , che , efe , fa+acumulador
acumulador=fa+acumulador

fa=0
che=0
efe=0
sum deudo to fa RCHEQ to che REFEC to efe for month(fecha)=12
?"DICIEMBRE:", fa , che , efe , fa+acumulador
acumulador=fa+acumulador
inkey(0)
return
***************************************************
*procedimiento para balancear las deudas de clientes
***************************************************
procedure BAlan

 do while .t.
    clear screen
    SELECT 2
    use ctasctes index ctasope

    AC = 0
    DE = 0
    EC = 0
    EE = 0
    RC = 0
    RE = 0

    SUM ACREE,DEUDO,ECHEQ,EEFEC,RCHEQ,REFEC TO AC,DE,EC,EE,RC,RE

        @ 4,20 say "B A L A N C E    D E    CTAS. C O R R I E N T E S "
        @ 5,20 SAY"---------------------------------------------------"
        @ 10,20 SAY"suma de creditos a favor de clientes:"
        @ 10,61 say AC
        @ 12,20 say "SUMA DE FACTURAS A FAVOR DE GASPERI:"
        @ 12,61 say DE
        @ 14,20 say"suma de cheques entregados por gasperi:"
        @ 14,61 say EC
        @ 16,20 say"suma de efectivo entregado por gasperi:"
        @ 16,61 say EE
        @ 18,20 say"suma de cheques recibidos de clientes:"
        @ 18,61 say RC
        @ 20,20 say"suma de efectivo recibido de clientes:"
        @ 20,61 say RE
        @ 22,20 say"BALANCE GENERAL......................:"
        @ 22,61 say -((DE-AC)-(RC+RE)+(EC+EE))

        inkey(0)
     return
enddo
**********************************************
*PROCEDIMIENTO PARA MODIFICAR UNA TRANSACCION PARTICULAR POR NUMERO DE TRANSACCION
*******************************************

procedure MODCTAS


   * Abre archivos CLIPRO.DBF y CTASCTES.DBF
   select 1
   Use CLIPRO index CLINUM,CLIRAZ
   select 2
   Use CTASCTES index CTASOPE

do while .t.

   SELECT 2

   * Se declaran variables de trabajo
   ORDC = 0
   FECH = space(10)
   NROF = space(13)

   NROC = 0
   NOMB = space(20)
   DIRE = space(25)
   LOCA = space(20)
   CPOS = space(4)
   TEL1 = space(10)
   TEL2 = space(10)

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

   nro = nrocli

select 1

  seek nro  && mueve el puntero de busqueda a ese registro



  NROC = NUMERO
  @ 04,19 say + NROC
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



   SGACR = ACREEDOR
   SGDEU = DEUDOR

  *    if SGACR > SGDEU
  *       @ 13,63 say (SGACR-SGDEU)
  *       @ 14,63 say space(13)
  *    elseif SGACR < SGDEU
  *       @ 14,63 say (SGDEU-SGACR)
  *       @ 13,63 say space(13)
  *    endif


select 2

   PACRE = ACREE
   PDEUD = DEUDO
   VECHE = ECHEQ
   VEEFE = EEFEC
   VRCHE = RCHEQ
   VREFE = REFEC

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

   replace ACREE         with  PACRE
   replace DEUDO         with  PDEUD
   replace ECHEQ         with  VECHE
   replace EEFEC         with  VEEFE
   replace RCHEQ         with  VRCHE
   replace REFEC         with  VREFE
   tone(1000,.5)
   @ 24,10 say"Ctrl-W para salir grabando el texto de la ventana"
   replace OBSERV with memoedit(OBSERV,9,4,13,35,.T.)
  select 1
  replace     ACREEDOR         with  SGACR
  replace     DEUDOR           with  SGDEU


   @ 13,50 say space(13)
   @ 14,50 say space(13)



   keyboard chr(48)+chr(13)


enddo
return

********************************************
*procedimiento para listar las operaciones por fecha
********************************************
procedure ORDDIA
do while .t.

select 2
index on dtos(fecha) to ctasfech

FECH = ctod("  /  /    ")
clear
tone (500,1)
@ 15,25 say"ingrese fecha :" get FECH
read

if EMPTY(FECH)
   close database
   close index
   clear
   return
endif
 seek FECH

   if eof()
      loop
   endif

do DEcide_salida
clear

@ 24,0 say space(80)
@ 0,0 say"operaciones del dia :" +dtoc(FECH)+space(40)
@ 1,0 say"---------------------------------------------------------------------------  "
*@ 2,0 say"operacion"+" "+"CLIENTE "+"ACREE "+"DEUDOR "+"     ECHE "+"    EEFE "+"    RCHE "+"   REFE"
@ 2,0 say"CLIENTE "+"                 ACREE "+"   DEUDOR "+"   ECHE "+"   EEFE "+"     RCHE "+"    REFE"
@ 3,0 SAY"_____________________________________________________________________________"

 *  LIST alltrim(OPERAC) NROCLI ACREE DEUDO ECHEQ EEFEC RCHEQ REFEC for FECHA = FECH
   LIST DENOMI ACREE DEUDO ECHEQ EEFEC RCHEQ REFEC for FECHA = FECH off
 *  LIST DENOMI ACREE DEUDO for FECHA = FECH off
*a=0
*b=0
*c=0
*d=0
*e=0
*f=0
*sum ACREE,DEUDO,ECHEQ,EEFEC,RCHEQ,REFEC to a,b,c,d,e,f for FECHA =FECH
*?"acreedor..........:",a
*?"deudor............:",b
*?"entregue cheques...",c
*?"entregue efectivo.:",d
*?"recibi cheques....................:",e
*?"recibi efectivo...................:",f
inkey(0)
set print off
set console on

 return
 enddo
************************************************************
*PROCEDIMIENTO PARA LISTAR LAS OPERACIONES ENTRE DOS FECHAS(todavia no funciona)
************************************************************
PROCEDURE ENntre_fechas

do while .t.

select 2
index on dtos(fecha) to ctasfech

FECH = ctod("  /  /    ")

clear
tone (500,1)
@ 15,25 say"ingrese primer fecha :" get FECH
read
FECH1=FECH

if EMPTY(FECH1)
   close database
   close index
   clear
   return
endif

@ 17,25 say"ingrese segunda fecha :" get FECH
read
FECH2=FECH


if EMPTY(FECH2)
   close database
   close index
   clear
   return
endif

 seek FECH1

   if eof()
      loop
   endif

 seek FECH2

   if eof()
      loop
   endif

do DEcide_salida
clear

@ 24,0 say space(80)
@ 0,0 say"operaciones del dia :" +dtoc(FECH)+space(40)
@ 1,0 say"---------------------------------------------------------------------------  "
*@ 2,0 say"operacion"+" "+"CLIENTE "+"ACREE "+"DEUDOR "+"     ECHE "+"    EEFE "+"    RCHE "+"   REFE"
@ 2,0 say"CLIENTE "+"                 ACREE "+"   DEUDOR "+"   ECHE "+"   EEFE "+"     RCHE "+"    REFE"
@ 3,0 SAY"_____________________________________________________________________________"

 *  LIST alltrim(OPERAC) NROCLI ACREE DEUDO ECHEQ EEFEC RCHEQ REFEC for FECHA = FECH
   LIST DENOMI ACREE DEUDO ECHEQ EEFEC RCHEQ REFEC for ((FECHA >= FECH1) .and. (FECHA <= FECH2)) off
a=0
b=0
c=0
d=0
e=0
f=0
sum ACREE,DEUDO,ECHEQ,EEFEC,RCHEQ,REFEC to a,b,c,d,e,f for ((FECHA >= FECH1) .and. (FECHA <= FECH2))
?"acreedor..........:",a
?"deudor............:",b
?"entregue cheques...",c
?"entregue efectivo.:",d
?"recibi cheques....................:",e
?"recibi efectivo...................:",f
inkey(0)
set print off
set console on

 return
 enddo
