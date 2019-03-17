void AddBinary(int a[8], int b[8], int c[8]) {
	int i;
	int carry = 0;
	for(i = 0; i<8; i++) {
		c[i] = a[i]+b[i]+carry;
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


