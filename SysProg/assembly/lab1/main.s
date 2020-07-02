	AREA mainarea,CODE
	EXPORT __main

__main
	MOV	r0, #0 //Moves 0 into r0
	MOV r1, #0 //Moves 0 into r1
loop
	ADD	r0, #1 //Increase the value of r0 by 1
	ADD	r1, r0 //Increase the value of r1 by r0
	CMP	r0, #10 //Sets Z if r0 == 10 or N is r0 < 10
	BLT loop

	B .			; Loop forever

	END
