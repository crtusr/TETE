* Programa.....: factura.prg
* Sistema.......: vivero
* Base de datos: clipro.dbf,stock1.dbf
* indice.......:
* Autor........: jose e. gasperi
* Fecha........: 24/9/91

set procedure to PROTETE
set confirm on
set escape off
declare CODIGO[20]
declare CANTIDAD[20]
declare NOM_MANUAL[20]
public NUME,RAZO,APEL,NOMB,DOMI,LOCA,PROV,CPOS,CUIT,CONT,TEL1,TEL2,TAAN
public FUCO,ACME,TMAN,DEUD,ACRE
public CODIG,NOMBR,CIENT,ENVAS,PROVE,EXIST,PPEDI,PRECB
public CODIGO[20],CANTIDAD[20],TOTALD
public LLAVE,VECPRECIO[20],NOM_MANUAL[20]
use
select 1
use clipro index  clinum,cliraz

select 2
use stock1 index  stocod,stonom
*index on codigo to stocod

*set color to w/b,G

do while .T.
do DAtcli       &&trae la ficha del cliente
   @ 2,16 say +NFAC
   @ 2,58 say +COMPROBANTE
   fechaf=dtoc(DATE())
  * @ 2,32 say +date()
   @ 2,15 get fechaf picture"99-99-9999"

   select 1
   NUME = 0
  set key -2 to BUscacliente
   @ 4,16 get NUME picture'9999'
   read

if NUME =0
   close database
   close index
   set escape on
   return
 endif

if NUME!=9999
   seek NUME
   LLAVE=0
 do VErfin         && verifica fin de archivo para

    if LLAVE=1
       loop
    endif
endif

 do CArvar         && carga campos de clipro.dbf en memoria
 do CArdatcl       && pone las variables del cliente seleccionado en la factura
    if NUME=9999
       do CArgaman       && carga los datos del cliente manualmente
    endif
@ 24,0 say space(80)
  optar=3
  @ 12,05 prompt"  salir   "
  @ 12,18 prompt"  repite  "
  @ 12,32 prompt"  continuar "
    menu to optar
     do case
        case optar=1
             close database
             close index
             return
        case optar=2
             loop
        case optar=3
    endcase
*******************************************************
*Opcion de forma de pago
********************************************************
if COMPROBANTE="FACTURA"
  opcion=1
  @ 15,05 prompt"CONTADO"
  @ 15,25 prompt"CHEQUE "
  menu to opcion
   do case
      case opcion=1
      PAGO="CONTADO"
      case opcion=2
      PAGO="CHEQUE"
   endcase
endif
*********************************
*Comienza la factura
*********************************
 select 2
 do BOleta
@ 0,0 SAY "DATOS: "+alltrim(RAZO)+" "+alltrim(DOMI)+" "+CONT+" "+CUIT
if COMPROBANTE = "FACTURA"
   tone(900)
endif
if COMPROBANTE = "REMITO"
   tone(450)
endif
if COMPROBANTE = "NOTA DE CREDITO"
   tone(90)
endif
@ 1,31 say +COMPROBANTE
    for k=1 to 20
       NOM_MANUAL[k]=space(25)
       codigo[k]=space(4)
       cantidad[k]=0
       vecprecio[k]=0
    next k
     paso=0
     r=4
     ult=4
     TOTAL=0
do while .T.

       set key 1 to MEnu
       set key 6 to FInfact
       set key 5 to FLechaup
       set key 24 to FLechadown
   for i=1 to 17
       @ 22,2 say "(9999) para escribir en el renglon"
       @ 24,2 say chr(25)+"  M O D I F I C A  "+chr(24)+"  ( home ) MENU  "+"  ( end ) FIN FACTURA   ( F3 ) BUSCA"

       set key -2 to VEntana         && cuando se pulsa F3

       @ r,2 get codigo[i]
             read

       if paso=1 .or. paso=6
         set key 1 to
         set key 6 to
         set key 5 to
         set key 24 to
         return
       endif

     if paso=3
        set key 1 to
        set key 6 to
        set key 5 to
        set key 24 to
        return
     endif

do while paso=5 .or. paso=24
   set key 13 to REtorno
   @ r,2 get codigo[i]
             read

     if paso=6
        set key 1 to
        set key 6 to
        set key 5 to
        set key 24 to
        set key 13 to
        return
     endif

      if paso=0
         exit
      endif

   loop
enddo

   if paso=0
     do CAracter
     do DEscripcion
   endif

    r=r+1
    ult=r
    tone(150,1)

 next i

  do cuentas
  close database
  close index
  return

 enddo
enddo
procedure cuentas
       IMPUESTO=0
       save screen to pant

       @ 12,33,23,67 box "ÚÄ¿³ÙÄÀ³°"

       @ 14,36 say"TOTAL.........:"
       @ 15,36 say"DESCUENTO.....:"
       @ 16,36 say"TOTAL.........:"
       @ 17,36 say"IMPUESTO......:"
       @ 18,36 say"IVA 21 % .....:"
       @ 19,36 say"IVA 10.5 % ...:"
       @ 21,36 say"TOTAL A PAGAR.:"

       @ 14,55 say TOTAL picture"99999.99"
       DESCU = TOTAL * DESC / 100
       TOTALD = TOTAL - DESCU

    if CONT = "RI"

       IVA = TOTALD * IVA1 / 100
       @ 15,55 say DESCU picture"99999.99"
       @ 16,55 say TOTALD picture"99999.99"
       @ 17,55 say IMPUESTO picture"99999.99"

       @ 18,55 say IVA picture"99999.99"
       TOTALP = TOTALD + IVA
       @ 21,55 say TOTALP picture"99999.99"
       IVAAD = 0
    endif
    if CONT="RIE"

       IVA = 0.00
       @ 15,55 say DESCU picture"99999.99"
       @ 16,55 say TOTALD picture"99999.99"
       @ 17,55 say IMPUESTO picture"99999.99"

       @ 18,55 say IVA picture"99999.99"
       TOTALP = TOTALD
       @ 21,55 say TOTALP picture"99999.99"
       IVAAD = 0.00
    endif
    if CONT="RNI"
       IVA = TOTALD * IVA1 / 100
       IVAAD = TOTALD * IVA2 / 100
       @ 15,55 say DESCU picture"99999.99"
       @ 16,55 say TOTALD picture"99999.99"
       @ 17,55 say IMPUESTO picture"99999.99"
       @ 18,55 say IVA picture"99999.99"
       @ 19,55 say IVAAD picture"99999.99"
         TOTALP = TOTALD + IVA + IVAAD
       @ 21,55 say TOTALP picture"99999.99"
    endif
    if CONT="RM" .or. CONT="CF"
       @ 15,55 say DESCU picture"99999.99"
       @ 16,55 say TOTALD picture"99999.99"
       @ 17,55 say IMPUESTO picture"99999.99"
       IVA = 0.00
       IVAAD = 0.00
       TOTALP = TOTALD
       @ 21,55 say TOTALP picture"99999.99"
    endif


  @ 24,0 say space(80)
  ARRANQUE=1
  @ 24,0 prompt  "    IMPRIME     "
  @ 24,20 prompt " actualiza stock  "
  @ 24,40 prompt " MENU PRINCIPAL "
  @ 24,60 prompt "CONTINUA FACTURA"

  menu to ARRANQUE
  do case
     case ARRANQUE = 1
        if llave_remito = 0
           do ACtualiza
           pagina="original"
           do IMpresion
           pagina="duplicado"
           do IMpresion
           set escape on
           SET COLOR TO
          return
        endif
          pagina="original"
          do IMpresion
          pagina="duplicado"
          llave_remito = 1
          do IMpresion
          set escape on
          set color to
          return
     case ARRANQUE =2
           do ACtualiza
           SET COLOR TO
           return
     case ARRANQUE = 3
           paso=3
           set escape on
           SET COLOR TO
           return
     case ARRANQUE = 4
          restore screen from pant
          paso=4
          return
   endcase


procedure MEnu
          close database
          close index
          paso=1
          tone(500,1)
          keyboard chr(13)
return

procedure FInfact
          do cuentas
      if paso=4
         return
      endif

          close database
          close index
          paso=6
          tone(250,1)
          keyboard chr(13)
return

procedure FLechaup
       if r>4
            i=i-1
            r=r-1
            paso=5
        endif
        set key 13 to
            keyboard chr(13)
return

procedure FLechadown
       if r>=4 .and. r < ult
            i=i+1
            r=r+1
            paso=24
        endif
        set key 13 to
             keyboard chr(13)
return

procedure CAracter
          CARACTERES = 0
          CODI = CODIGO[i]
          CODI = alltrim(CODI)
          CARACTERES = len (CODI)
  do while CARACTERES != 4
     tone(50,5)
     @ r,2 get CODIGO[i]
     read
     do CAracter
     loop
  enddo

          seek CODIGO[i]
               LLAVE = 0
          do VErfin

   do while LLAVE = 1
       tone(100,3)
        @ r,2 get CODIGO[i]
         read
          seek CODIGO[i]
          LLAVE = 0
        do VErfin
       loop
    enddo
return


procedure DEscripcion

   if CONT ="CF" .or. CONT ="RM"
      PRECIO = DEcidepr(PRECIOB,RAZO)
      PRECIO = PRECIO*(1+IVA1/100)
      PRECIO = val(str(PRECIO,8,2))
      VECPRECIO[i]=PRECIO
   elseif CONT="RI" .or. CONT="RIE"
      PRECIO = DEcidepr(PRECIOB,RAZO)
      PRECIO = val(str(PRECIO,8,2))
      VECPRECIO[i]=PRECIO
   elseif CONT="RNI"
      PRECIO = DEcidepr(PRECIOB,RAZO)
      PRECIO = val(str(PRECIO,8,2))
      VECPRECIO[i]=PRECIO
   endif


          if codigo[i] !="9999"
             NOM = rtrim(NOMBRE)
             ENV = rtrim(ENVASE)
             @ r,22 say space(21)
             @ r,22 say +NOM+"  "+ENV
             tone(200,1)
          endif


            if codigo[i] = "9999"
              * NOM_MANUAL[i]=space(25)
               @ r,22 get NOM_MANUAL[i]
               read
            endif

          @ r,11 get CANTIDAD[i] picture '9999'
          read
          tone(100,1)

          @ r,54 say PRECIO picture '99999.99'
          @ r,54 get PRECIO picture '99999.99'
          read
          VECPRECIO[i]=PRECIO
              subtotal=cantidad[i]*precio
              @ r,67 say +subtotal picture '99999.99'
              total=0

          for z=1 to 17
              total=total+VECPRECIO[z]*cantidad[z]
          next z

                @ 21,52,23,77 box"ÚÄ¿³ÙÄÀ³ "
                @ 22,53 say "Subtotal:"
                @ 22,62 say +total picture "99999.99"


         paso=0
return

procedure REtorno
          paso=0
          set key 13 to
          keyboard chr(13)
return

procedure VEntana
          set key -2 to    && desactiva la funcion F3 (VEntana)
          set key 5 to
          set key 24 to
          set key 1 to
          set key 6 to

   save screen to boleta
         keyboard chr(13)
         NOMBR = SPACE(30)
         @ 10,20,15,75 BOX"ÚÄ¿³ÙÄÀ³°"

         @ 12,22 SAY "NOMBRE...........:" GET NOMBR PICTURE REPLICATE('!',30)
         READ

     IF NOMBR != SPACE(30)
        SET ORDER TO 2
        SET SOFTSEEK ON
        SEEK NOMBR

     restore screen from boleta

        IF EOF()
           tone(50,5)
           INKEY(0)
           RETURN
        ENDIF
     ENDIF

   save screen to boleta

          @ 2,2 to 17,77 double
          declare vector[4]
          vector[1]="codigo"
          vector[2]="nombre"
          vector[3]="envase"
                                    *  vector[5]="proveedor"
          vector[4]="preciob"


          dbedit(3,3,16,76,vector)
          codigo[i]=CODIGO
      SET ORDER TO 1

   restore screen from boleta

          set key 5 to FLechaup
          set key 24 to FLechadown
          set key 1 to MEnu
          set key 6 to FInfact

      return

*******************************************

*************************************************
*Procedure para abrir un BROWSE de clientes
**************************************************

procedure BUscacliente


   save screen to ACTUAL

          @ 15,2 to 22,77 double
          declare vector[6]
          vector[1]="numero"
          vector[2]="razon"
          vector[3]="apellidos"
          vector[4]="localidad"
          vector[5]="telefono1"
          vector[6]="telefono2"

          dbedit(16,3,20,50,vector)

      SET ORDER TO 1

   restore screen from ACTUAL
   set key -2 to     && desactiva F3 como funcion dbedit
   set softseek off  && desactiva la busqueda "blanda"
return
*******************************************
