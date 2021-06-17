#ifndef H_RAZORBEARD_MATH
#define H_RAZORBEARD_MATH

#include <stdint.h>

int32_t zeros(uint32_t x);
int32_t fixed_log2 (int32_t x);
int32_t fixed_exp2 (int32_t x);
uint32_t gamma_22(uint32_t x);
uint32_t r_gamma_22(uint32_t x);
uint32_t isqrt(uint32_t x);

#endif
