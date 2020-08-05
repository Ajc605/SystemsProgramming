	AREA asm,CODE
	
	EXPORT process
	
	; Your code goes here...
process
	STMFD sp!, {r4, r5, r6, lr}; //Pushing registers
	
	LDR r1, [r0]
	LDR r2, [r0, #4]
	LDR r3, [r0, #8]
	LDR r4, [r0, #12]
	
	ADD r2, r2, r1; Add X and Y
	STR r2, [r0, #4]; Store x and y in Y
	
	MOV r0, r3; Pass the prt files to r0 for callback function argument
	BLX r4; Branch to callback function 
	
	LDMFD sp!, {r4, r5, r6, pc}; Poping registers
	
	END
	