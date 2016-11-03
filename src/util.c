#include "util.h"

void shift_nibbles(uint8_t *sPtr, uint8_t *dPtr, const size_t len){
    *dPtr = 0;
    for(size_t i = 0; i < len; i++){
        *dPtr |= (*sPtr >> 4) & 0x0F;
        dPtr++; 
        *dPtr = (*sPtr << 4) & 0xF0;
        sPtr++;
    }
}