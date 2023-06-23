*****************
*prodedimiento para pedir ficha de parametros
************************
procedure PIfipa
   @ 03,25 SAY "NUMERO DE FACT...:"
   @ 04,25 SAY "TIPO DE CAMBIO...:"
   @ 05,25 SAY "IVA RI...........:"
   @ 06,25 SAY "IVA RNI..........:"
   @ 07,25 SAY "DESCUENTO........:"
   @ 08,25 SAY " % MAYORISTA.....:"
   @ 09,25 SAY " % PUBLICO.......:"
return

************************
*procedimiento para pedir los datos por pantalla
************************
procedure TOdapa

   @ 03,43  get NFAC picture '999999'
   @ 04,43  get TCAM picture '99999'
   @ 05,43  get IVA1 picture '9999999'
   @ 06,43  get IVA2 picture '9999999'
   @ 07,43  get DESC picture '999'
   @ 08,43  get PORC1 picture '999'
   @ 09,43  get PORC2 picture '999'
return
****************************
*procedimiento para grabar los datos de parametros
*****************************
procedure GRdapa

   Replace NFACTURA      With  NFAC
   Replace TCAMBIO       With  TCAM
   Replace IVAUNO        With  IVA1
   Replace IVADOS        With  IVA2
   Replace DESCUENTO     With  DESC
   Replace PORCEN1       With  PORC1
   Replace PORCEN2       With  PORC2
return
*****************************
*procedimiento para traer el formato de ficha de cliente para facturacion
*****************************
procedure DAtcli
clear
text
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³fecha:                                  ³ Comprobante:                       ³
ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
³Nro. Cliente :         Razon Social :                                        ³
³Apellido y Nombres :                                                         ³
³Domicilio :                                                                  ³
³CP :                Localidad :                                              ³
³Provincia :                CUIT :               RESP.FISCAL :                ³
³                                                                             ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
endtext
return
*****************************
*procedimiento para verificar que se encontro el fin de archivo
*****************************
procedure VErfin

if eof()
    if codigo[i]="9999"
       return
    endif
  @ 24,0 say 'Codigo incorrecto pulse alguna tecla para continuar                '
  LLAVE = 1
  inkey(0)
  @ 24,0 say space(80)
endif
return
*****************************
*procedimiento para posicionar los datos en pantalla
*****************************
procedure CArdatcl
   if NUME=9999
      return
   endif
*@ 2,17 say +NFAC picture'999999'
@ 2,15 say +date() picture'99/99/9999'
@ 4,16 say +NUME picture '9999'
@ 4,38 say +RAZO
@ 5,22 say +APEL
@ 6,13 say +DOMI
@ 7,06 say +CPOS
@ 7,33 say +LOCA
@ 8,13 say +PROV
@ 8,34 say +CUIT
@ 8,63 say +CONT

return
*****************************
*procedimiento para cargar variables en memoria de clipro.dbf
*****************************
procedure CArvar
  if NUME=9999
     return
  endif
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
return
****************************
*procedimiento para descontar del stock y actualizar la ficha de cliente
****************************
procedure ACtualiza
if NUME!=9999
     select 1  && selecciona el DBF de cliente-proveedor
MES = month(date())

      if MES != month(FUCO)
        if COMPROBANTE = "NOTA DE CREDITO"
           TAAN=TAAN - TOTALD
        endif
        if COMPROBANTE != "NOTA DE CREDITO"
           TAAN = TAAN + TOTALD  && ojo TAAN es un total acumulativo
           FUCO = DATE()
           TMAN = ACME
           ACME = TOTALD
        endif

               replace TOTANOANT with TAAN  &&  es un total acumulativo ojo
               replace FEULTCOM   with DATE()
               replace ACUMUMES   with ACME
               replace TOTMESANT  with TMAN

        select 2 && selecciona el DBF de stock1
         for i=1 to 20
             seek CODIGO[i]
             EXIST = EXISTENCIA
             EXIST = EXIST - CANTIDAD[i]
             replace EXISTENCIA with EXIST
         next i
       return
      endif
endif
if NUME!=9999


      select 1 && selecciona el DBF de cliente-proveedor
      if COMPROBANTE = "NOTA DE CREDITO"
             TAAN = TAAN - TOTALD
             ACME = ACME - TOTALD
      endif
      if COMPROBANTE != "NOTA DE CREDITO"
             TAAN = TAAN + TOTALD
             ACME = ACME + TOTALD
      endif
             replace TOTANOANT with TAAN
             replace FEULTCOM  with date()
             replace ACUMUMES  with ACME

endif
       select 2 && selecciona el DBF de stock1

         for i=1 to 20
             seek CODIGO[i]
             EXIST = EXISTENCIA
             if COMPROBANTE = "NOTA DE CREDITO"
                EXIST = EXIST + CANTIDAD[i]
             endif
             if COMPROBANTE != "NOTA DE CREDITO"
                EXIST = EXIST - CANTIDAD[i]
             endif
             replace EXISTENCIA with EXIST
         next i
      return
*****************************************************
*Procedimiento para cargar manualmente los datos del cliente
******************************************************
procedure CArgaman
          @ 2,16 say +NFAC picture'999999'
          @ 2,32 say +DATE() picture'99/99/99'
          @ 4,15 say +NUME picture'9999'
          RAZO=space(20)
          APEL=space(20)
          DOMI=space(20)
          CPOS=space(4)
          LOCA=space(20)
          PROV=space(10)
          CUIT=space(13)
          CONT=space(15)
          @ 4,37 get RAZO
          @ 5,21 get APEL
          @ 6,12 get DOMI
          @ 7,05 get CPOS
          @ 7,32 get LOCA
          @ 8,12 get PROV
          @ 8,33 get CUIT
          @ 8,64 get CONT picture "@!" valid(CONT="RI" .or. CONT="RNI" .or. CONT="CF")
          READ
return
****************************************************************
*procedimiento para salida de factura por impresora
****************************************************************
procedure IMpresion

    if !isprinter()
       clear
       tone(100,2)
       @ 5,20 to 11,60 double
       @ 7,21 say "la impresora no esta preparada"
       @ 8,21 say "        o  no tiene papel"
       @ 9,21 say " resuelva el problema y pulse una tecla"
       inkey(0)
    endif
   clear
   tone(100,5)
    if pagina="original"

       @ 5,20 to 11,60 double
       @ 7,21 say "DEBE COLOCAR EN LA IMPRESORA"
       @ 8,21 say "LOS FORMULARIOS CORRESPONDIENTES"
       @ 9,21 say "LUEGO PULSE CUALQUIER TECLA"
       inkey(0)
    endif


   set device to print
   set printer to C:\users\oem\desktop\texto.txt
   set console off

   *corre todas las columnas
    M=2

   * corre todas las filas

    N=3

 * @ 0,0 say chr(27)+chr(40)+chr(115)+chr(49)+chr(83)   && impresion italica
 * @ 1,0 say chr(27)+chr(40)+chr(115)+chr(51)+chr(66)  && bold
   *@ 0,25 say "Cond.de venta :"
   *@ 0,65 say "Remito Nro.:"
   *@ 0,85 say "FECHA :"

   if COMPROBANTE="FACTURA"
      @ 12,20 say PAGO
   endif

   @ 12,70 say date()
   @ 14,13 say ltrim(RAZO)
   @ 14,48 say NUME
   @ 15,13 say DOMI
   @ 15,62  say CONT
   @ 16,13 say LOCA
   @ 16,62 say CUIT
   @ 17,13 say PROV
   *@ 6+N,0+M  say "codigo"
   *@ 6+N,7+M say "cantidad"
   *@ 6+N,4+M say "descripcion"
   *@ 6+N,52+M say"precio"
   *@ 6+N,62+M say"importe"
   *@ 7+N,0+M say"-------------------------------------------------------------------------------"

 if llave_remito = 0

   for i=1 to 20
       seek CODIGO[i]

           if CODIGO[i] != space(4)
              if codigo[i] != "9999"
                 NOM = rtrim(NOMBRE)
                 ENV = rtrim (ENVASE)
              endif

              SUBTOTAL = (CANTIDAD[i])*(VECPRECIO[i])
              @ i+18+N,M say CODIGO[i]
              @ i+18+N,9+M say CANTIDAD[i] picture "9999"

            if codigo[i] != "9999"
              @ i+18+N,15+M say NOM+"  "+ENV
            endif
            if codigo[i] = "9999"
               @ i+18+N,15+M say NOM_MANUAL[i]
            endif

              @ i+18+N,58+M say VECPRECIO[i] picture "99999.99"
              @ i+18+N,68+M say SUBTOTAL picture "99999.99"

           endif
   next i

*@ 25+N,76+M say"SUBTOTAL"

*@ 26+N,1 say "RENGLON"

              @ 41+N,72 say TOTAL picture"99999.99"
*@ 26+N,60+M say "0.00"                       &&15

*@ 42+N,50 say"descuento:... "
*@ 42+N,68 say DESCU

              @ 48+N,72 say TOTALD picture"99999.99"      &&30


if COMPROBANTE != "NOTA DE CREDITO"
           *   @ 44+N,11 say"Cheques a la orden de Jose E. Gasperi O/R "
            *  @ 45+N,11 say" "
             * @ 46+N,11 say"* INTERNET: www.cultivogasperi.com.ar"
              * @ 47+N,11 say"* MAIL    : marta@cultivogasperi.com.ar "
               * @ 48+N,11 say"* Utilice los codigos para futuros pedidos"
  ** NO ESCRIBIR RENGLONES MAS ABAJO QUE EL RENGLON 32
endif

  * @ 44+N,68+M say TOTALD picture"99999.99"      &&30

    if CONT="RI"
       @ 50+N,64+M say IVA1 picture"99.9"          &&41
       @ 50+N,71+M say IVA picture"99999.99"         &&46
       @ 52+N,64+M say IVA2 picture"99.9"           &&51
       @ 52+N,71+M say IVAAD picture"99999.99"       &&605
    elseif CONT="RNI"
       @ 50+N,64+M say IVA1 picture"99.9"          &&41
       @ 50+N,71+M say IVA picture"99999.99"         &&46
       @ 52+N,64+M say IVA2 picture"99.9"           &&51
       @ 52+N,71+M say IVAAD picture"99999.99"       &&60
    endif
@ 54+N,72 say TOTALP picture"99999.99"
eject
   set device to screen
   set console on
return
endif

   for i=1 to 20
       seek CODIGO[i]

           if CODIGO[i] != space(4)
              if codigo[i] != "9999"
                 NOM = rtrim(NOMBRE)
                 ENV = rtrim (ENVASE)
              endif

              @ i+20+N,4+M say CODIGO[i]
              @ i+20+N,13+M say CANTIDAD[i] picture "9999"

            if codigo[i] != "9999"
              @ i+20+N,22+M say NOM+"  "+ENV
            endif
            if codigo[i] = "9999"
               @ i+20+N,22+M say NOM_MANUAL[i]
            endif

           endif
   next i
if COMPROBANTE != "NOTA DE CREDITO"

  * @ 42+N,11 say"Cheques a la orden de Jose E. Gasperi O/R "
   * @ 43+N,11 say" "
    * @ 44+N,11 say"* INTERNET: www.cultivogasperi.com.ar"
     * @ 44+N,11 say"* MAIL    : marta@cultivogasperi.com.ar  "
      * @ 44+N,11 say"* Utilice los codigos para futuros pedidos"
  ** NO ESCRIBIR RENGLONES MAS ABAJO QUE EL RENGLON 32

endif
eject
   set device to screen
   set console on
llave_remito = 0
return

*****************************************************************************

procedure BOleta
clear

text
ÚÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³Codigo   ³Cantidad ³          Descripcion         ³   Precio   ³  Subtotal   ³
ÃÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄ´
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
³         ³         ³                              ³            ³             ³
ÀÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

endtext
return
********************************************************************************
Procedure DAtpro
clear
text
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³ ord.compra:[      ]    fecha:[        ]  factura nro.:[             ]  ³
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³nro.proveedor:[   ]                 nombre:[                         ]  ³
   ³direccion:[                         ] localidad:[                    ]  ³
   ³cpostal:[    ]           telefono1:[         ]  telefono2:[          ]  ³
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³DETALLE:                          ³      S A L D O   P A R C I A L      ³
   ³                                  ³acreedor:[              ]            ³
   ³                                  ³deudor  :[              ]            ³
   ³                                  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³                                  ³      S A L D O    G E N E R A L     ³
   ³                                  ³acreedor:[         ]                 ³
   ³                                  ³deudor  :[         ]                 ³
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³           VALORES ENTREGADOS     ³           VALORES RECIBIDOS         ³
   ³cheque:[        ] efec.:[        ]³cheque:[        ] efectivo:[        ]³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
endtext
return
******************************************************************************

Procedure DAtclien
clear
text
   ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
   ³ operacion :[      ]    fecha:[        ]  factura nro.:[             ]  ³
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³nro.cliente  :[   ]                 nombre:[                         ]  ³
   ³direccion:[                         ] localidad:[                    ]  ³
   ³cpostal:[    ]           telefono1:[         ]  telefono2:[          ]  ³
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³DETALLE:                          ³      S A L D O   P A R C I A L      ³
   ³                                  ³credito :[              ]            ³
   ³                                  ³factura$:[              ]            ³
   ³                                  ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³                                  ³      S A L D O    G E N E R A L     ³
   ³                                  ³acreedor:[         ]                 ³
   ³                                  ³deudor  :[         ]                 ³
   ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´
   ³           VALORES ENTREGADOS     ³           VALORES RECIBIDOS         ³
   ³cheque:[        ] efec.:[        ]³cheque:[        ] efectivo:[        ]³
   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
endtext
return
************************************************************************
Procedure DEcide_salida
clear
   @ 03,12,11,32 BOX "ÚÄ¿³ÙÄÀ³°"
   opcion=1
   @ 05,13 prompt '   Pantalla      ' message 'salida por pantalla            '
   @ 07,13 prompt '   Impresora       ' message 'salida por impresora         '
   @ 09,13 prompt '  Retroceder   ' message 'Vueve al menu anterior               '
   menu to opcion

   do case
      case opcion = 1
           return
      case opcion = 2
           * Eligio la segunda opcion
          * SAVE SCREEN TO PANTALLA2
           Do LISTA4
           RESTORE SCREEN FROM PANTALLA2
           return
      case opcion = 3 .OR. OPCION = 0
           * Eligio la tercera opcion
           Clear
           llavelistado=1
           return
   endcase
*********************************************************
Procedure LIsta4

    if !isprinter()
       clear
       tone(100,2)
       @ 5,20 to 11,60 double
       @ 7,21 say "la impresora no esta preparada"
       @ 8,21 say "        o  no tiene papel"
       @ 9,21 say " resuelva el problema y pulse una tecla"
       inkey(0)
    endif
   clear

*  set console off
  set device to print
  set printer to c:\users\oem\desktop\texto.txt
  set console off
  return
