#ifndef LCG_MINI_H
#define LCG_MINI_H

#include <stdint.h>

//#define LM_SAFETY


#ifndef LM_GENERATORS_ONLY
uint64_t lm_generate(uint64_t seed);
#endif

//******************************** generators *********************************

#ifndef LM_GENERATE_ONLY
int lm_add_generator_unsafely(uint64_t seed, int* generator);
int lm_add_generator_safely(uint64_t seed, int* generator);
#ifdef LM_SAFETY
#define lm_add_generator lm_add_generator_safely
#else
#define lm_add_generator lm_add_generator_unsafely
#endif
int lm_remove_generator_unsafely(int generator);
int lm_remove_generator_safely(int generator);
#ifdef LM_SAFETY
#define lm_remove_generator lm_remove_generator_safely
#else
#define lm_remove_generator lm_remove_generator_unsafely
#endif

// NOTE: if generator == -1.. use builtin generator
int lm_generate_next_unsafely(int generator, uint64_t* a);
int lm_generate_next_safely(int generator, uint64_t* a);
#ifdef LM_SAFETY
#define lm_generate_next lm_generate_next_safely
#else
#define lm_generate_next lm_generate_next_unsafely
#endif

int lm_reset_generator_unsafely(int generator);
int lm_reset_generator_safely(int generator);
#ifdef LM_SAFETY
#define lm_reset_generator lm_reset_generator_safely
#else
#define lm_reset_generator lm_reset_generator_unsafely
#endif
#endif

#endif
