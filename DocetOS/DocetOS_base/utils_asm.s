	PRESERVE8
	AREA utils, CODE, READONLY

; Export function locations
	EXPORT getPSR

getPSR
	MRS r0, psr
	