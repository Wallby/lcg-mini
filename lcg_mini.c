#include "lcg_mini.h"


#define max(a, b) (a > b ? a : b)

static uint64_t seed = 0xffffffffffffffffull;
static uint64_t x = 0xffffffffffffffffull;
// ^
// not 0 on purpose (see note in lm_set_seed)
// ^
// 0xffffffffffffffffull == 2^63

//void lm_set_seed(uint64_t seed)
void lm_set_seed(uint64_t a)
{
	// NOTE: seed == 0 results in seed == 1 after calling lm_get once
	//       remove seed == 0 as an option and for convenience use the..
	//       .. same sequence for seed == 2^63 and seed == 0
	seed = a == 0ull ? 0xffffffffffffffffull : a;
	x = seed;
}

uint64_t lm_get()
{
	// NOTE: a from table 6, not sure if supposed to use with m == 2^63
	//       ^
	//       https://onlinelibrary.wiley.com/doi/10.1002/spe.3030#spe3030-sec-0009-title
	uint64_t a = 0xd1342543de82ef95ull;
	uint64_t c = 1ull;
	uint64_t m = 0xffffffffffffffffull; //< 2^63
	x = (a * x + c) % m;
	return x;
}
