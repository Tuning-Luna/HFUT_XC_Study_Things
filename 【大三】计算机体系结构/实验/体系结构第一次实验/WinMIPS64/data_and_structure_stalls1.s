 .data 
N_COEFFS:  .word 3
coeff: .double 5.0,2.0,-3.0
N_SAMPLES: .word 3
sample: .double 1,2,3,4,5,6,7,8,9,10
result: .double 0,0,0,0,0,0,0,0,0,0
C_ZERO: .double 0.0
        .text


start:	ld r1,N_COEFFS(r0)  ; r1 = N_COEFFS
	ld r2,N_SAMPLES(r0)  ; r2 = N_SAMPLES
	slt r3,r1,r2    ; N_COEFFS < N_SAMPLES?
        bnez r3,smooth       ; yes - go to smooth
	beq r1,r2,smooth ; branch N_COEFFS = N_SAMPLES        
	halt

smooth:    
	L.D F3,C_ZERO(r0)  ; F3=norm=0.0;
	daddu r2,r0,r0   ; r2 = 0
    ld r1,N_COEFFS(r2)  ; r1 = N_COEFFS
    L.D F1,coeff(r2)  ; F1=coeff[0]
    L.D F0,C_ZERO(r0)  ; F0=0

	c.lt.d F1,F0	;coeff[0]<0	c.lt.d freg,freg - set FP flag if less than
	bc1t neg_coeff_zero 	;- branch to address if FP flag is true
	ADD.D F3,F3,F1	;norm+=coeff[0];
	jal coeff_one         ; call coeff_one

neg_coeff_zero: 
	SUB.D F3,F3,F1	;norm-=coeff[0];
	jal coeff_one         ; call coeff_one

coeff_one:
	daddi r2,r0,8   ; r2 = 8
	L.D F1,coeff(r2)  ; F1=coeff[1]
	c.lt.d F1,F0	;coeff[1]<0	c.lt.d freg,freg - set FP flag if less than
	bc1t neg_coeff_one 	;- branch to address if FP flag is true
	ADD.D F3,F3,F1	;norm+=coeff[1];
	jal coeff_two         ; call coeff_two

neg_coeff_one: 
	SUB.D F3,F3,F1	;norm-=coeff[1];
	jal coeff_two         ; call coeff_two

coeff_two:
	daddi r2,r0,16   ; r2 = 16
	L.D F1,coeff(r2)  ; F1=coeff[2]
	c.lt.d F1,F0	;coeff[2]<0	c.lt.d freg,freg - set FP flag if less than
	bc1t neg_coeff_two 	;- branch to address if FP flag is true
	ADD.D F3,F3,F1	;norm+=coeff[2];
	jal go_result         ; call go_result

neg_coeff_two: 
	SUB.D F3,F3,F1	;norm-=coeff[2];
	jal go_result         ; call go_result

go_result:	halt

out:    halt