	AREA mainarea,CODE
	EXPORT __main

__main
	MOV	r0, #0 //Set's r0 = 0
	MOV r1, #0 //Set's r1 = 0
loop
	ADD	r0, #1 //Increase the value of r0 by 1
	ADD	r1, r0 //Increase the value of r1 by r0
	CMP	r0, #10 //Sets Z if r0 == 10 or N is r0 < 10
	BLT loop

	B .			; Loop forever

	END
