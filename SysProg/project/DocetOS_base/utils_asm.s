  
	AREA utils, CODE, READONLY

; Export function locations
	EXPORT getPSR
	EXPORT getCONTROL

getPSR
	MRS r0, psr
	
getCONTROL
	MRS r0, control
	
END