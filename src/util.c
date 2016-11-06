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
    uint8_t spare = 0, subject;
    for(size_t i = 0; i < len; i++){
        subject = *sPtr++;
        *dPtr++ = spare | ((subject & 0x0F) << 4);
        spare = subject >> 4;
    }
    *dPtr = spare;
}