MOD_B: BEGIN
A: EXTERN
B: EXTERN
L1: EXTERN
PUBLIC R
PUBLIC MOD_B
LOAD A
MUL B
STORE R
DIV DOIS
STORE R
JMP L1
R: SPACE
DOIS: CONST 2
END