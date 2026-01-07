.data
max:	.word 0
titulo:	.asciiz "Hailstone numbers\n"
pregunta: .asciiz "Number= "
str:	.asciiz "Maximum= "

CONTROL: .word32 0x10000
DATA:	 .word32 0x10008

.text
        lwu r8,DATA(r0)   	
        lwu r9,CONTROL(r0)	

        daddi r11,r0,4    	

        daddi r1,r0,titulo	
        sd r1,(r8)      	
        sd r11,(r9)			
 
        daddi r1,r0,pregunta
        sd r1,0(r8)       	
        sd r11,0(r9)     	

        daddi r1,$zero,8  	
        sd r1,0(r9)	  		
        ld r1,0(r8)			
        sd r1,max(r0)   	

bucle:  andi r3,r1,1  
 	daddi r12,r0,1 
        beqz r3,par

impar:  daddu r2,r1,r1   	
        dadd r1,r2,r1   	
        daddi r1,r1,1   	
        j maximo

par:	dsrl r1,r1,1		

maximo: sd r1,(r8)		
	ld r4,max(r0)
	sd r12,(r9)      
	slt r3,r4,r1     	
        beqz r3,fin
        sd r1,max(r0)    	

fin:    slti r3,r1,2     	
        beqz r3,bucle

        ld r2,max(r0)    	
        daddi r1,r0,str  	
        sd r1,(r8)       	
        sd r11,(r9)			
        sd r2,(r8)       	
        sd r12,(r9)		
        halt
        halt