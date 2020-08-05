	AREA utils, CODE, READONLY
	
	EXPORT getPSR
	EXPORT getCONTROL
		
getPSR
	STMFD sp!, {r4, lr}
	
	MRS r0, psr
	
	LDMFD sp!, {r4, pc}
	
getCONTROL
	STMFD sp!, {r4, lr}
	
	MRS r0, control
	
	LDMFD sp!, {r4, pc}

	END