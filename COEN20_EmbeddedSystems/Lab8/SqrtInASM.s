		.syntax unified
		.arch	armv7-m
		.text
		.thumb_func
		.align	2

// ------------------------------------------------------------------------------------------
// uint32_t SqrtInASM(uint32_t x)
// ------------------------------------------------------------------------------------------

		.globl  SqrtInASM
SqrtInASM:

	// Insert your code here ....
        //x = R0
        LDR R1, =0 // y = 0
        LDR R2, =0x40000000 // m = 0x40000000

        loop:
            CMP R2, #0
            IT EQ
            BEQ done
            ORR R3, R1,R2 // b=y | m
            LSR R1, R1, #1 // y = y >> 1
            CMP R3, R0
            ITT LE //if(b <= x)
            SUBLE R0, R0, R3 //if b <= x, x = x-b
            ORRLE R1, R1, R2 // if b <= x, y | m

            LSR R2, R2, #2 // m = m >> 2

            B loop

        done:
            MOV R0, R1




		BX      LR

	        .end
