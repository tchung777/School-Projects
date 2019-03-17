int pow(int x, int y) {
    int ans=1;
    int i;
    for(i = 0; i < y; i++)
        ans*=x;
    return ans;
}

signed int Bin2Signed(int binary[8]){
    int i;
    if(!binary[8]) {
        int flip = 0;
        for(i = 0; i < 8; i++) {
            if(flip)
                binary[i]^=1;
            if(binary[i])
                flip = 1;
        }
        int integer = 0;
        for(i = 0;i < 8;i++)
            integer+=(binary[i]*pow(2,i));
        return -1*integer;
    } else {
        int integer = 0;
        for(i = 0;i < 8;i++)
            integer+=(binary[i]*pow(2,i));
        return integer;
    }
}

unsigned int Bin2Unsigned(int binary[8]) {
    int integer = 0;
    int i;
    for(i = 0;i < 8;i++)
        integer+=(binary[i]*pow(2,i));
    return integer;
}



