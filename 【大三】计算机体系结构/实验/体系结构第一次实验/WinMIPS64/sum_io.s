;output the sum to terminal IO
.data
A: .word 10
B: .word 8
C: .word 0
CR: .word32 0x10000
DR: .word32 0x10008

.text
main:
ld r4,A(r0)
ld r5,B(r0)
dadd r3,r4,r5
sd r3,C(r0)

lwu r1, CR(r0) ;control register
lwu r2, DR(r0) ;data register
daddi r10, r0, 1
sd r3, (r2) ;output r3
sd r10, (r1) ; output to screen

halt