#define TRUE 1
#define FALSE 0

void TwosComp(int input[8],int output[8]);

void SubBinary(int a[8], int b[8], int c[8]){

	int output[8];
	TwosComp(b,output);

	int i;
    int carry = 0;
	for(i = 0; i<8; i++) {
		c[i] = a[i]+output[i]+carry;
		carry = 0;
		if(c[i] == 2) {
			c[i] = 0;
			carry = 1;
		}
		if(c[i] >= 3) {
            c[i] = 1;
            carry = 1;
		}

	}
}

void TwosComp(int input[8], int output[8]) {
	int flip = FALSE;
	int i;
	for(i = 0; i < 8 ; i++) {
		int out = input[i];
		if(flip)
			out=!input[i];
		if(input[i] && !flip) {
			flip = TRUE;
		}
		output[i] = out;

	}
}
