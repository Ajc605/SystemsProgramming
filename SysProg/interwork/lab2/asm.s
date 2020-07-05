	AREA asm,CODE
	
		EXPORT fib
		IMPORT report

fib
	STMFD sp!, {r4, r5, r6, lr}; //Pushing registers
	MOV r4, r0; Store n
	MOV r5, #0
	MOV r6, #1
	
loop
	ADD r0, r5, r6
	MOV r5, r6
	MOV r6, r0
	BL report ; Print the value in r0
	SUBS r4, #1
	BNE loop
	
	LDMFD sp!, {r4, r5, r6, pc}; Poping registers
	
	END
	