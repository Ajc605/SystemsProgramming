	AREA mainarea,CODE
	EXPORT __main

__main
	MOV	r0, #0 ; Load 0 to r0
	MOV r1, #0 ; Load 0 to r1
loop
	ADD	r0, #1 ; Add 1 to ro
	ADD	r1, r0 ; Add ro to r1
	CMP	r0, #10 ; Check if r0 is 10 
	BLT loop

	B .			; Loop forever

	END
