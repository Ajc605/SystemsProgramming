	AREA storage,DATA
array
	SPACE 4*20	; Declares a 20-word storage area
array_end

	AREA mainarea,CODE
	EXPORT __main
__main
	; Your code goes here
	LDR r0, =array
	LDR r1, =array_end
	MOV r2, #0
	MOV r3, #1
loop
	ADD r4, r3, r2 ; 
	STR r4, [r0], #4 ; Store the value at r3 to tha address in r0 and then increament r0 by 4
	MOV	r2, r3
	MOV r3, r4
	CMP r0, r1
	BLO loop

	B .			; Loop forever

	ALIGN
	END
