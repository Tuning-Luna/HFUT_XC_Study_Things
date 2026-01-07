.data
    CONTROL: .word 0x10000  ; address of control port 
    DATA: .word 0x10008     ; address of data port
    E: .double 2.7       ; set float e
    A: .double 1.0

.text
main:
    ld r20,CONTROL(r0)      ; load CONTROL
    ld r21,DATA(r0)         ; load DATA
    l.d f1,E(r0)            ; load E
    l.d f2,A(r0)          ; load float 1.0
    daddi r3,r0,4       ; int N = 5;
    daddi r4,r0,0       ; int Count = 0
    div.d f2,f2,f1         ; 1/e


loop:                   ;
    beq r4,r3,done      ; while ( N < 5 )

    mul.d f2,f2,f2  
    daddi r4,r4,1
    j loop ; }

done:
	add.d f3,f0,f2
	s.d f3,0(r21)		; write double to DATA register
    daddi r5,r0,3	; set for double output
	sd r5,0(r20)		; write to CONTROL register and make it happen


    halt