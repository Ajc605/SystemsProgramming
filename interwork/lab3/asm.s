	AREA asm,CODE
	
	; Your code goes here...
	IMPORT report
	EXPORT process
	
process
	STMFD sp!, {r0, r4- r11, lr}
	
	LDMIA r0, {r4-r7}
	ADD r5, r5, r4
	
	STMIA r0, {r4-r7}
	MOV r0, r6
	
	BLX r7
	
	LDMFD sp!, {r0, r4- r11, lr}
		
	
	END
	