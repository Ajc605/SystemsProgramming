	AREA storage,DATA
array
	; Total is 232 for the example list
	DCD 3, 7, 31, 193, 2, -5, 1
array_end

	AREA main,CODE
	EXPORT asm_main
asm_main
	; Your code goes here
	LDR r0, =array
	LDR r1, =array_end
	MOV r2, #0
	MOV r3, #0
	
loop
	LDR r2, [r0], #4
	ADD r3, r3, r2
	CMP r0, r1
	BLO loop
	B .			; Loop forever

	ALIGN
	END
