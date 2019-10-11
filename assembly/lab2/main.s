	AREA mainarea,CODE
	EXPORT __main
__main

	; Your instructions go here...
	MOV r0, #1
	MOV r1, #0
loop
	ADD r1, r0, r0, LSL #4 ; r1 = r0 + 16*r0 = 17r0
	SUB r1, r1, r0, LSL #2 ; r1 = 17r0 - 4r0 = 13ro
	blt loop
	
	B .			; Loop forever

	END
