.data
A1: .word 34,90,-100,200000,3,43,0,5,22,-4,17,17,3,44322,4230324,3253,-98,23,324,429  ; a[]
CR: .word 0x10000
DR: .word 0x10008

.text
main:

ld r1,CR(r0)
ld r2,DR(r0)
dadd r4,r0,r0
ld r10,A1(r4) ;t

dadd r3,r0,r0 ;i


loop:
ld r9,A1(r3) ;i
slt r6,r9,r10    ;if t<a[i]
daddi r3,r3,8 
daddi r5,r3,-160
beq r5,r0,over
beq r6,r0,max
j loop

max:
;ld r10,(r9)
daddi r10,r9,0  ;t=a[i]
j loop



over:
daddi r20,r0,1
sd r10,(r2)
sd r20,(r1)
halt