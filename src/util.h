#ifndef UTIL_H_
#define UTIL_H_

#include <stdint.h>
#include <stdlib.h>

void shift_nibbles(uint8_t *sPtr, uint8_t *dPtr, const size_t len);
void shift_swap_nibbles(uint8_t *sPtr, uint8_t *dPtr, const size_t len);

#endif /* UTIL_H_ */