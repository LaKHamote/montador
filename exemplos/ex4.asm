BB: EQU 0
AA:
  EQU 1  ;meuoasdasdk
;comentario
      INPUT OLD_DATA
NOVE: CONST -0x19
LOAD OLD_DATA
L1: DIV DOIS
STORE  NEW_DATA
IF AA
MUL DOIS
IF BB ; <--- nao faz sentido isso, mas sai no preprocessamento
MUL TRES ;  <--- nao faz sentido isso, mas sai no preprocessamento
STORE TMP_DATA
LOAD OLD_DATA
SUB TMP_DATA
STORE TMP_DATA
OUTPUT TMP_DATA
COPY NEW_DATA,   OLD_DATA
LOAD OLD_DATA
JMPP L1
STOP
DOIS:
    CONST 2
OLD_DATA: SPACE
NEW_DATA: SPACE
TMP_DATA: SPACE