*************
*funcion para decidir el precio mayorista o publico
*************
function DEcidepr
         parameter PRE,CLI

         if CLI = space(20)
            INDICE = 1+PORC2/100
            return(PRE*INDICE*TCAM)
         endif
         if CLI = "CONS.COPROP.MIRAFLORES C.CLUB"
            INDICE = 1.1
            return(PRE*INDICE*TCAM)
         endif
         if CLI <> space(20)
            INDICE = 1+PORC1/100
            return(PRE*INDICE*TCAM)
         endif


**************************************************************
