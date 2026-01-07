.data
a: .double 1.0, 2.0, 3.0, 4.0, 5.0, 6.0
b: .double 2.0, 3.0, 4.0, 5.0, 6.0, 7.0
c: .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
d: .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0

.text
main:
	daddi r1, r0, 6		;r1 now contains n = 6
	daddi r2, r0, 10	;r2 now contains 10
	mtc1 r2, f1			;f1 now contains alpha = 10.0
	dadd r2, r0, r0		;r2 now contains i = 0	
	daddi r4, r1, -1

loop:
	dsll r3, r2, 3		;r3 now contains 8*i or 8*r2
	l.d f2, a(r3)		;f2 contains a[i]
	l.d f3, b(r3)		;f3 contains b[i]
	l.d f5, d(r3)		;f5 contains d[i]
	mul.d f6, f2, f3	;f6 has a[i]*b[i]
	;loop meta
	daddi r2, r2, 1 	;i++
	dsll r3, r2, 3		;r3 now contains 8*i or 8*r2
	;end of loop meta
	mul.d f7, f6, f1	;f7 has c[i]*alpha
	l.d f2, a(r3)		;f2 contains a[i]
	l.d f3, b(r3)		;f3 contains b[i]
	add.d f8, f5, f7	;f8 now has d[i] + c[i]*alpha	;This is the part between muli and addi that needs a stall and cannot be optimised by scheduling
	s.d f6, c(r3)		;stored f6 to c[i]
	mul.d f6, f2, f3	;f6 has a[i]*b[i]
	s.d f8, d(r3)		;successfully stored f8 to d[i]
	l.d f5, d(r3)		;f5 contains d[i]
	mul.d f7, f6, f1	;f7 has c[i]*alpha
	;loop meta
	daddi r2, r2, 1 	;i++
	slt r5, r4, r2		;r5 = r4<r2 (n-1<i)
	;end of loop meta
	add.d f8, f5, f7	;f8 now has d[i] + c[i]*alpha	;This is the part between muli and addi that needs a stall and cannot be optimised by scheduling
	s.d f6, c(r3)		;stored f6 to c[i]
	s.d f8, d(r3)		;successfully stored f8 to d[i]
	beq r5, r0, loop	;if r5 = 0, loop

exit:
halt