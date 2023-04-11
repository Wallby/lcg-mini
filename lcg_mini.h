#ifndef LCG_MINI_H
#define LCG_MINI_H

#define LCG_MINI_VERSION 1.0.0

#include <stdint.h>


void lm_set_seed(uint64_t seed);
uint64_t lm_get();

#endif
