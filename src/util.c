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

void shift_swap_nibbles(uint8_t *sPtr, uint8_t *dPtr, const size_t len){
    uint8_t spare = 0;
    *dPtr = 0;
    for(size_t i = 0; i < len; i++){
        *dPtr++ = spare | (*sPtr << 4);
        spare = *sPtr++ >> 4;
    }
    *dPtr = spare;
}