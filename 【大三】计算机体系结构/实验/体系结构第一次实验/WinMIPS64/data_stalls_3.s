.data 
N_COEFFS:  .word 3
coeff: .double 1.0,25.0,3.0
N_SAMPLES: .word 3
sample: .double 1,2,3,4,5,6,7,8,9,10
result: .double 0,0,0,0,0,0,0,0,0,0

        .text


start:	ld r1,N_COEFFS(r0)  # r1 = N_COEFFS
	ld r2,N_SAMPLES(r0)  # r2 = N_SAMPLES
	slt $t2,r1,r2    # N_COEFFS < N_SAMPLES?
        bnez $t2,smooth       # yes - go to smooth
	beq r1,r2,smooth # branch N_COEFFS = N_SAMPLES        
	halt

smooth:    
	daddu r2,r0,r0   ; r2 = 0
        ld r1,N_COEFFS(r2)  # r1 = N_COEFFS
        dsll r1,r1,3     # r1 = N_COEFFS*8
for:    slt $t2,r2,r1    # i < N_COEFFS?
        beqz $t2,out       # yes - exit
        ld r5,coeff(r2)  # r5=a[i]
        daddi r2,r2,8    # i++
        j for
out:    halt