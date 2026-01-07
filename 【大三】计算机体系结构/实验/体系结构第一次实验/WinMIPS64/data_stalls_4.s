.data
    CONTROL: .word 0x10000  ; address of control port 
    DATA: .word 0x10008     ; address of data port

.text
main:
    ld r20,CONTROL(r0)  ; load CONTROL
    ld r21,DATA(r0)     ; load DATA
    daddi r1,r0,1       ; int s = 1;
    daddi r2,r0,1       ; int i = 1;
    daddi r3,r0,5       ; int N = 5;
    daddi r4,r0,0       ; int Count = 0


loop:                   ;
    beq r4,r3,done      ; while ( N < 5 )

    dadd r5,r0,r1    	; load s into register
	sd r5,0(r21) 	 	; first set DATA
				        ; '0' is the address of memory
	daddi r5,r0,2 		; '2' is the integer output command
	sd r5,0(r20) 		; then write CONTROL 
				        ; outputs on WinMips64 terminal

    dadd r6,r0,r2    	; load i into register
	sd r6,0(r21) 	 	; first set DATA
				        ; '0' is the address of memory
	daddi r6,r0,2 		; '2' is the integer output command
	sd r6,0(r20) 		; then write CONTROL 
				        ; outputs on WinMips64 terminal

    dadd r1,r1,r2       ; s += i
    dadd r2,r2,r1       ; i += s
    daddi r4,r4,1       ; Count = Count + 1
    j loop ; }

done:
    halt