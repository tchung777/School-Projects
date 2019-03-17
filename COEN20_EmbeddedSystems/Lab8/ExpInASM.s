		.syntax unified
		.arch	armv7-m
		.text
		.thumb_func
		.align	2

// ------------------------------------------------------------------------------------------
// int32_t ExpInASM(int32_t x, uint32_t n)
// ------------------------------------------------------------------------------------------

		.globl  ExpInASM
ExpInASM:
    //R0 is x
    //R1 is n
	// Insert your code here ....

        LDR R2, =1 //y =1

        loop:
            AND R3,R1,#1 //n&1
            CMP R3,#0 //if(n&1==0)
            BEQ L1
            MUL R2,R2,R0 //y = p*y

        L1:
            LSR R1,R1,#1 //n >> 1

            CMP R1,#0 //if(n==0)
            IT EQ
            BEQ break //goto break
            MUL R0,R0,R0 //p = p*p

            B loop //while(1)

        break:
            MOV R0,R2 //return y


		BX      LR

     		.end
