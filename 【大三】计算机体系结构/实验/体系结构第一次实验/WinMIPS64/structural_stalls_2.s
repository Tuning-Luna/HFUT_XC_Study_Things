.data

resultado:	.double 0			;PI x R^2
titulo:		.asciiz "Calculo circulo.\n"
pregunta: 	.asciiz "Radio= "
str:		.asciiz "Resultado= "
PI: 		.double 3.14159265358979

; Hemos copiado esta cabecera para guiarnos.
;
; Memory Mapped I/O area
;
; Address of CONTROL and DATA registers
;
; Set CONTROL = 1, Set DATA to Unsigned Integer to be output
; Set CONTROL = 2, Set DATA to Signed Integer to be output
; Set CONTROL = 3, Set DATA to Floating Point to be output
; Set CONTROL = 4, Set DATA to address of string to be output
; Set CONTROL = 5, Set DATA+5 to x coordinate, DATA+4 to y coordinate, and DATA to RGB colour to be output
; Set CONTROL = 6, Clears the terminal screen
; Set CONTROL = 7, Clears the graphics screen
; Set CONTROL = 8, read the DATA (either an integer or a floating-point) from the keyboard
; Set CONTROL = 9, read one byte from DATA, no character echo.
;

CONTROL: .word32 0x10000
DATA:    .word32 0x10008

.text
		lwu r8,DATA(r0)		;r8 para CONTROL
		lwu r9,CONTROL(r0)	;r9 para DATA
		
		daddi r11,r0,4		;Activado para imprimir string

		daddi r1,r0,titulo	;Apuntamos a titulo
		sd r1,(r8)		;Preparamos los datos apuntados a imprimir
		sd r11,(r9)		;Imprimimos titulo
		
		daddi r1,r0,pregunta	;Apuntamos a pregunta
		sd r1,0(r8)		;Preparamos los datos apuntados a imprimir
		sd r11,0(r9)		;Imprimimos pregunta
		
		l.d f5, PI(r0)		;Carga de PI
		daddi r3,r0,8		;Preparamos para CONTROL
		sd r3,0(r9)		;CONTROL = 8 (input de tipo int o float por teclado)
		l.d f1,0(r8)		;f1 es el numero introducido
		cvt.d.l f1, f1		;Controlamos el numero entero introducido, lo convertimos a tipo float

		mul.d f2, f1, f1	;Calculamos
		mul.d f3, f2, f5	

		daddi r1, r0, str	;Carga para str
		daddi r2, $zero, 3	;Para CONTROL = 3
		sd r1, (r8)		
		sd r11, (r9)		;Control imprime str
		s.d f3, (r8)
		sd r2, (r9)		;Control imprime el resultado
		
		s.d f3, resultado(r0)	;Guardamos el resultado

		halt