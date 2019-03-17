		.syntax	unified
		.arch	armv7-m
		.text
		.thumb_func
		.align	2

// ------------------------------------------------------------------------------------------
// uint32_t BitFieldClear(uint32_t word, int lsb, int width)
// ------------------------------------------------------------------------------------------

		.globl  BitFieldClear
BitFieldClear:

		// On entry: R0 = word, R1 = lsb, R2 = width

		// Insert your code here ...
        //uint32_t mask = (1 << (lsb+width)) - (1 << lsb) ;
        ADD R2, R2, R1
        LDR R3,=1
        LSL R2, R3, R2
        LSL R1, R3, R1
        SUB R2, R2, R1
        //return word & ~mask ;

        MVN R2, R2
        AND R0, R0, R2

		BX	    LR		    // return result in R0

		.end
