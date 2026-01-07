; * * * * * * * * * * * * * * * *
; Lab2.1 - s255320 Jacopo Nasi  *
; * * * * * * * * * * * * * * * *

				.data
v1:			.double 4, 6, 5, 7, 8, 16, 2, 1, 3, 24
v2:			.double 5, 6, 5, 7, 8, 16, 2, 1, 3, 24
v3:			.space 80
v4:			.space 80
v5:			.space 80

				.text
main:		dadd R6, R0, R0						;		index
				daddui R7, R0, 10					;		counter

loop:		l.d F1, v1(R6)
				l.d F2, v2(R6)

				mul.d F3, F1, F2					; 	v3[i] = v1[i]*v2[i];
				s.d F3, v3(R6)						;		save R3 in V3(index)

				div.d F4, F3, F2					; 	v4[i] = v3[i]/v2[i];
				s.d F4, v4(R6)						;		save R4 in V4(index)

				add.d F5, F4, F2					; 	v5[i] = v4[i]+v2[i];
				s.d F5, v5(R6)						;		save R4 in V4(index)

				daddui R6, R6, 8					; 	increasing index
				daddi R7, R7, -1					;		decreasing counter
				bnez R7, loop							;		check loop
				nop

				halt