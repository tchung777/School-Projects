/*
    Name:Thomas Chung
    Objective: This function gives you the negative 2's complement of whatever you set as input.
*/
#define TRUE 1
#define FALSE 0

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


