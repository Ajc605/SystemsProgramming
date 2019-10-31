	AREA asm,CODE
	
	; Your code goes here...
	IMPORT report
	EXPORT process
	
process
	STMFD sp!, {r4, r5, r6, r7, r8, lr}
	
	LDR r4, [r0]
	LDR r5, [r0 , #4]
	LDR r6, [r0 , #8]
	LDR r7, [r0 , #16]
	ADD r5, r4
	
	STR r5, [r0 , #4]
	;MOV r0, r5
	;BL report
	
	BLX r7
	
	LDMFD sp!, {r4, r5, r6, r7, r8, lr}
		
	
	END
	