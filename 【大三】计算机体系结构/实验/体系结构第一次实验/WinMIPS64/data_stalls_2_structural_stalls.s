;fahrenheit to celsius

.data

c:	.double 0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0

f:	.double -459.67,-50.0,-20.0,-10.0,0.0,32.0,70.0,98.6,100.0,212.0

s:	.double 32.0

n:	.double 5.0

d:	.double 9.0


.text
	
	daddi r1,r0,0 		;instruccion que sube
	daddi r10,r0,80

				;aqui estaba: daddi r1,r0,0
loop:	
	l.d f3,f(r1)

	l.d f4,s(r0)

				;aqui estaba: sub.d f7,f3,f4

	l.d f5,n(r0)

	l.d f6,d(r0)

	sub.d f7,f3,f4
		;instruccion que baja
	div.d f8,f5,f6

	mul.d f9,f7,f8

	s.d f9,c(r1)

	daddi r1,r1,8

	bne r1,r10,loop

	halt