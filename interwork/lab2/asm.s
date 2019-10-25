	AREA asm,CODE
	
	IMPORT report
	EXPORT fib
		
fib
	;ro = past value, r1 is check value
	STMFD 	sp!, {r4, r5, r6, r7, r8, lr}
	MOV r4, r0
	MOV r5, #0
	MOV r6, #1
loop
	
	ADD r7, r5, r6; r7 = r5 + r6
	MOV r0, r6
	BL report
	
	MOV r5, r6;Move r6 into r5
	MOV r6, r7;Move r7 into r6
	
	SUBS r4, #1; r4 - 1	
	BNE loop
	
	LDMFD 	sp!, {r4, r5, r6, r7, r8, lr}
	; Your code goes here
	
	END
	