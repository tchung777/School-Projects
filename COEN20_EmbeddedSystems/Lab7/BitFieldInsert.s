		.syntax	unified
		.arch	armv7-m
		.text
		.thumb_func
		.align	2

// ------------------------------------------------------------------------------------------
// uint32_t BitFieldInsert(uint32_t word, int lsb, int width, uint32_t value)
// ------------------------------------------------------------------------------------------

		.globl  BitFieldInsert
BitFieldInsert:

		// On entry: R0 = word, R1 = lsb, R2 = width, R3 = value

		// Insert your code here ...
        PUSH {R4, R5}

        //mask = (1 << (lsb+width)) - (1 << lsb) ;
        LDR R4,=1 // MASK
        ADD R5, R1, R2 //LSB+WIDTH
        LSL R5, R4, R5 //R5 = (1<<(LSB+WIDTH))
        LSL R4, R4, R1 //R4 = (1<<(lsb))
        SUB R5, R5, R4 //R5 = (1<<(LSB+WIDTH)) - (1<<LSB)

        MVN R5, R5 //~MASK
        AND R0, R5 //WORD&=~MASK

        //mask = (1 << width) - 1 ;
        LDR R4,=1 // MASK
        LSL R4,R4,R2 //(1<<width)
        SUB R4, R4, #1 //R2 = (1<<(WIDTH)) - 1
        //value &= mask ;
        AND R4, R3, R4 //value &=mask

        //return word | (value << lsb) ;
        LSL R4, R4, R1 //VALUE << LSB


        ORR R0, R0, R4 //WORD | (VALUE << LSB)

        POP {R4, R5}


		BX      LR		            // return result in R0

		.end
