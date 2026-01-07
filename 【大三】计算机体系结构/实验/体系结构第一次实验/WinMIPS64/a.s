; * * * * * * * * * * * * * * * *
; Lab1.1 - s255320 Jacopo Nasi  *
; * * * * * * * * * * * * * * * *

			.data
vect:	.word 5, 6, 5, 7, 8, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 8, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 8, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 8, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 8, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 8, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 999, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 8, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 8, 16, 2, 1, 3, 24
			.word 5, 6, 4, 7, 8, 16, 2, 1, 3, 24
max:	.word 0

; R1 = MAX, R2 = vect_val, R3 = index, R4 = value_count, R5 = compare

				.text
main:		dadd R3, R0, R0						; Clean R3
				daddui R4, R0, 100				; Set counter to number of elements
loop:		ld R1, max(R0)						; R1 = MAX(vect) at this time
				ld R2, vect(R3)						; Load new values
				slt R5, R1, R2						; Compare: If(R1<R2) R5=0 else new max found
				beqz R5, cont
				nop												; added for delay slot problems
update: sd R2, max(R0)

cont:		daddui R3, R3, 8					; Update index for iterations
				daddi R4, R4, -1					; Decrease counter
				bnez R4, loop							; Check if loop
				nop
				ld R10, max(R0)						; Load in R10 the final max value (debug pourpose 0x3e7)
				halt