 do while .t.
    clear screen

    use PROVE index PRONRO
    ACR = 0
    DEU = 0
    SUM SGACRE,SGDEUD to ACR,DEU
        @ 4,20 say "B A L A N C E    D E    C O M P R A S "
        @ 5,20 SAY"---------------------------------------"
        @ 10,20 SAY"suma de compras a pagar"
        @ 12,20 say ACR
        @ 16,20 say "suma de saldos a favor de GASPERI"
        @ 18,20 say DEU
        DIF=ACR-DEU
        @ 20,20 say "diferencia................:"
        @ 20,48 say DIF
        inkey(0)
     return
 enddo
