set print on
?chr(15)
set console off
use clipro index cliraz
* las siguientes instrucciones es un listado completo normal
*?"NRO  RAZON SOCIAL         NOMBRES              DOMICILIO            LOCALIDAD            PROVINCIA  CUIT         CON  TELEFONO "
*?"-------------------------------------------------------------------------------------------------------------------------------"
*do while .t.
*   list NUMERO,RAZON,NOMBRES,DOMICILIO,LOCALIDAD,PROVINCIA,NCUIT,alltrim(TIPO),TELEFONO1 FOR RAZON <> SPACE(20) OFF
*   EJEC
*   SET CONSOLE ON
*   RETURN

?"NRO  RAZON SOCIAL         NOMBRES              DOMICILIO            LOCALIDAD            PROVINCIA  CUIT         CON  TELEFONO "
?"-------------------------------------------------------------------------------------------------------------------------------"
do while .t.
   list NUMERO,RAZON,NOMBRES,DOMICILIO,LOCALIDAD,PROVINCIA,NCUIT,alltrim(TIPO),TELEFONO1 FOR TOTANOANT <> 0 OFF
   EJEC
   SET CONSOLE ON
   RETURN
ENDDO
