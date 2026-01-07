.data
after: .asciiz "After sort the array is:\n"
before: .asciiz "Before sort the array is:\n"
CONTROL: .word 0x10000
DATA: .word 0x10008
SP: .word 0x7ffffffc
flag: .word 0
array: .word 8,6,3,7,1,0,9,4,5,2

.text
main:
daddi r29,r0,SP         ;r29--$sp
ld r16,CONTROL(r0)
ld r17,DATA(r0)
daddi r8,r0,4           ;set string output
daddi r9,r0,before      ;prompt line
sd r9,(r17)
sd r8,(r16)             ;print

daddi r8,r0,2           ;set signed Integer output
daddi r1,r0,0           ;i<-0 
Loop:
ld r9,array(r1)         ;get a[i]
sd r9,(r17)
sd r8,(r16)             ;print a[i]
daddi r1,r1,8           ;i++
daddi r2,r1,-80
beq r2,r0,Exit          ;if i=10, exit
j Loop
Exit:

daddi r4,r0,array       ;parameter1 address of a[]
daddi r5,r0,10          ;parameter2 n
jal bubblesort

daddi r8,r0,4           ;set string output
daddi r9,r0,after       ;prompt line
sd r9,(r17)
sd r8,(r16)             ;print

daddi r8,r0,2           ;set signed Integer output
daddi r1,r0,0           ;i<-0 
Loop3:
ld r9,array(r1)         ;get a[i]
sd r9,(r17)
sd r8,(r16)             ;print a[i]
daddi r1,r1,8           ;i++
daddi r2,r1,-80
beq r2,r0,Exit3         ;if i=10, exit
j Loop3
Exit3:

halt



bubblesort:
daddi r29,r29,-24       ;make room on stack
sd $ra,16(r29)
sd r16,8(r29)
sd r17,0(r29)

dadd r22,r4,r0          ;get a[]
daddi r23,r5,0          ;get n

and r18,r18,r0          ;i=0
fori:
slt r10,r18,r23         ;if i<n, r10=1
beq r10,r0,exiti        ;if i>n, jump exiti

daddi r19,r18,-1        ;j<-i-1
forj:
slti r10,r19,0          ;if j<0, r10=1
bne r10,r0,exitj        ;if j<0, jump exitj
dsll r11,r19,3          ;j*8
dadd r12,r11,r22        ;a+j*8
ld r13,0(r12)           ;a[j]
ld r14,8(r12)           ;a[j+1]
slt r10,r14,r13
beq r10,r0,exitj        ;if a[j+1]>a[j] jump exitj

dadd r4,r0,r22          ;parameter1 address of a[]
daddi r5,r19,0          ;parameter2 j
jal swap

daddi r19,r19,-1        ;j--
j forj
exitj:
daddi r18,r18,1
j fori

exiti:
ld r17,0(r29)
ld r16,8(r29)
ld $ra,16(r29)          
daddi r29,r29,24        ;restore stack pointer
jr $ra


swap:

dsll r8,r5,3            ;r8=k*8
dadd r8,r4,r8           ;r8=a+(k*8)
ld r9,0(r8)             ;r9=a[k]
ld r10,8(r8)            ;r10=a[k+1]
sd r10,0(r8)            ;a[k]=r10
sd r9,8(r8)             ;a[k+1]=r9

jr $ra