	AREA asm,CODE
	
	EXPORT calculate

calculate
	STMFD sp!, {r0, r1, r2, lr}; //Pushing registers
	
	ADD r0, r0, r0, LSL #2; r0 = r0 + 4r0 = 5r0 => 5x
	ADD r0, r0, r1; r0 = 5r0 + r1 => 5x + y
	BX lr
	
	LDMFD sp!, {r2, pc}; //Poping registers
	
	END
	