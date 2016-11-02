#ifndef CRC_H_
#define CRC_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint32_t init;
    const void* table;
    uint32_t final_xor;
    uint8_t input_reflected;
    uint8_t result_reflected;
}crc_context;

uint8_t  generate_crc8 (const crc_context *ctx, const uint8_t *bytes, const size_t length);
uint16_t generate_crc16(const crc_context *ctx, const uint8_t *bytes, const size_t length);
uint32_t generate_crc32(const crc_context *ctx, const uint8_t *bytes, const size_t length);

#endif /* CRC_H_ */