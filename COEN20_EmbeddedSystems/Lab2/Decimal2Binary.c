void Dec2Bin(int decimal, int binary[8]) {
    int neg = 0;
    int i;
    if(decimal < 0) {
        decimal*=-1;
        neg = 1;
    }
    int index = 0;
    int quotient = decimal;
    for(i=0;i<8;i++) {
        int bit = quotient % 2;
        binary[index] = bit;
        quotient /= 2;
        index++;
    }
    if(neg) {
        int flip = 0;
        for(i = 0; i < 8; i++) {
            if(flip)
                binary[i]^=1;
            if(binary[i])
                flip = 1;
        }
    }
}

