	AREA mainarea,CODE
	EXPORT __main
__main

	; Your instructions go here...
	MOV	r0, #5 ;Moves 0 into r0
	MOV r1, #0 ;Moves 0 into r1

	ADD r1, r0, r0, LSL #3; r1 = r0 + 8r0 = 9r0
	ADD r1, r1, r0, LSL #2 ; r1 = r1 + 4r0 = 9r0 + 4r0 = 13 r0

	B .			; Loop forever

	END
