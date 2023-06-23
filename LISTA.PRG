
************
*
* Programa.....: precios.prg
* Sitema.......: vivero
* Base de datos: stock1.dbf
* indice.......: stocod.ntx , stonom.ntx
* Autor........: jose e. gasperi
* Fecha........: 28/8/95

clear screen
@ 5,10 SAY"LISTA DE PRECIOS CORRESPONDIENTE AL MES DE AGOSTO"
@ 10,10 SAY"esta lista es orientativa y puede tener cambios diarios"
@ 11,10 say"de todos modos CULTIVO GASPERI espera le sea de gran utilidad"
@ 12,11 say"los precios son sin IVA,y los pagos tendra un plazo maximo"
@ 13,11 say"de 30 dias a contar desde que se encarga la mercaderia"
@ 24,11 say"toque alguna tecla para continuar"

inkey(0)


* Abre archivo agenda indexado por numero
Use STOCK1 index STOCOD, STONOM
do while .t.
   clear screen
   set intensity off
   set order to 1

   CODIG = space(4)
   NOMBR = SPACE(30)
   CANTIDAD = lastrec()
   @ 1,23,20,75 BOX "ÚÄ¿³ÙÄÀ³°"
   @ 1,26 say "VIVERO ***** consultasÄÄÄCant. :"
   @ 1,58 say +CANTIDAD+1000-1 picture '9999'
   @ 3,25 say "CODIGO...........:" get CODIG picture replicate('!',4)
   @ 4,25 SAY "NOMBRE..........:" get NOMBR picture replicate('!',30)
   read

   if CODIG = space(4) .and. NOMBR = space(30)
      * Significa que no quiere mas consultas
      * Cierra base de datos e indices
      Close database
      Close index
      clear screen
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

@ 24,0 say chr(24)+' anterior  '+chr(25)+'  siguiente '+' Esc  salir                 '
TECLA = 0
set cursor off
do while TECLA != 27

   CODIG = CODIGO
   NOMBR = NOMBRE
   CIENT = CIENTIFICO
   ENVAS = ENVASE
   PROVE = PROVEEDOR
   EXIST = EXISTENCIA
   PPEDI = PPEDIDO
   PRECB = PRECIOB
   TIPO  = TIPOP
   USO   = USOTR
   ALTUR = ALTURA
   FLORA = FLORACION
   CUALI = CUALIDADES
   LUZ   = LUZRE
   RIEG  = RIEGO
   TEMPE = TEMPERATUR

   @ 2,25 say "CODIGO..........:" get CODIG
   @ 3,25 SAY "NOMBRE..........:" get NOMBR
   @ 4,25 SAY "NOMBRE CIENTIFI.:" get CIENT
   @ 5,25 SAY "ENVASE..........:" get ENVAS
  * @ 6,25 SAY "PROVEEDOR.......:" get PROVE
  * @ 7,25 SAY "EXISTENCIA......:" get EXIST
  * @ 8,25 SAY "PUNTO DE PEDIDO.:" get PPEDI
   @ 9,25 SAY "PRECIO BASE....:" get PRECB
   @ 10,25 SAY "TIPO DE PLANTA.:" get TIPO
   @ 11,25 SAY "USO MAS COMUN..:" get USO
   @ 12,25 SAY "ALTURA.........:" get ALTUR
   @ 13,25 SAY "FLORACION......:" get FLORA
   @ 14,25 SAY "CUALIDADES.....:" get CUALI
   @ 15,25 SAY "LUZ REQUERIDA..:" get LUZ
   @ 16,25 SAY "RIEGO REQUERIDO:" get RIEG
   @ 17,25 SAY "TEMPERATURA....:" get TEMPE

   clear gets

    TECLA = inkey(0)
    if .not. bof() .or. .not. eof()
       @ 2,48 say replicate('°',26)
    endif
       do case
          case TECLA = 24
               skip 1
               tone(800,1/18)
               if eof()
                  @ 2,48 say 'fin de archivo'
                  skip -1
                  loop
               endif


          case TECLA = 5
               skip -1
               tone(800,1/18)
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
return

