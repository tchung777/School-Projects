		.syntax	unified
		.arch	armv7-m
		.text
		.thumb_func
		.align	2

// ------------------------------------------------------------------------------------------
// uint32_t AbsValinASM(int32_t x)
// ------------------------------------------------------------------------------------------

		.globl  AbsValinASM
AbsValinASM:

	// The first parameter (x) is passed to this function in R0.
	// The function should return the result in R0.

	// Registers R0 through R3 may be modified without
	// preserving their original content. However, the
	// value of all other registers must be preserved!

	// Insert your code here ...
        ASR R1,R0, #31
        //MVN R1,R1
        ADD R0,R0,R1
        EOR R0,R0,R1
		BX	LR		            // return

		.end
