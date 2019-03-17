#include <stdint.h>

int16_t SBinaryMultiply(int8_t mpier, int8_t mcand)
	{
    uint16_t product = 0;
    int i;
    for(i = 0;i < 8;i++) {
        if((mcand&(1<<i))) {
            product += ((uint8_t)mpier << i);
        }
    }

    if(mpier < 0)
        product -= (mcand<<8);
    if(mcand < 0)
        product -= (mpier<<8);
	// Insert your code here ...

	return product ;
	}

