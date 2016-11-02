#include "crc.h"

uint8_t reflect_byte(uint8_t input){
    uint8_t output = 0, i;
    for(i = 0; i < 8; i++){
        output <<= 1;
        if( input & (1<<i) )
            output |= 0x01;
    }
    return output;
}

uint16_t reflect_word(uint16_t input){
    uint16_t output = 0, i;
    for(i = 0; i < 16; i++){
        output <<= 1;
        if( input & (1 << i))
            output |= 0x0001;
    }
    return output;
}

uint16_t generate_crc16(const crc_context *ctx, const uint8_t *bytes, const size_t length)
{
    const uint16_t *table = (const uint16_t*)ctx->table;
    uint16_t crc = (uint16_t)ctx->init;
    uint8_t curByte, pos;
    size_t i;
    for (i = 0; i < length; i++)
    {
        curByte = (ctx->input_reflected ? reflect_byte(*bytes) : *bytes);
        pos = (uint8_t)((crc >> 8) ^ curByte);
        
        crc = (crc << 8) ^ table[pos];
        bytes++;
    }
    crc = (ctx->result_reflected ? reflect_word(crc) : crc);
    return (crc ^ ctx->final_xor);
}