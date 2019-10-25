	AREA asm,CODE
	
	; Your code goes here
	EXPORT calculate

calculate
	ADD r0, r0, r0, LSL #2;5X
	ADD r0, r1, r0; 5X+Y
	BX lr
	END
	