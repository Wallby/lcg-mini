#include "lcg_mini.h"

#include <malloc.h>
#include <string.h>


#define max(a, b) (a > b ? a : b)

//*****************************************************************************

#ifndef LM_GENERATORS_ONLY
uint64_t lm_generate(uint64_t seed)
#else
static uint64_t lm_generate(uint64_t seed) //< used in lm_generate_next
#endif
{
	// NOTE: a from table 6, not sure if supposed to use with m == 2^63
	//       ^
	//       https://onlinelibrary.wiley.com/doi/10.1002/spe.3030#spe3030-sec-0009-title
	uint64_t a = 0xd1342543de82ef95ull;
	uint64_t c = 1ull;
	uint64_t m = 0xffffffffffffffffull; //< 2^63
	return (a * seed + c) % m;
}

//******************************** generators *********************************

#ifndef LM_GENERATE_ONLY
struct info_about_generator_t
{
	int bIsGenerator;
	uint64_t seed;
	uint64_t x;
};
static struct info_about_generator_t info_about_generator_default = { .bIsGenerator = 1 };

int numGeneratorsTheresRoomFor = 0;
int numGenerators = 0;
struct info_about_generator_t* infoPerGenerator;

static int is_generator(int generator)
{
	if((generator < 0) | (generator >= numGeneratorsTheresRoomFor))
	{
		return 0;
	}
	if(infoPerGenerator[generator].bIsGenerator == 0)
	{
		return 0;
	}
	
	return 1;
}

int lm_add_generator_unsafely(uint64_t seed, int* generator)
{
	int indexToGenerator;
	if(numGenerators == numGeneratorsTheresRoomFor)
	{
		struct info_about_generator_t* a = infoPerGenerator;
		//infoPerGenerator = new struct info_about_generator[numGeneratorsTheresRoomFor + 1];
		infoPerGenerator = (struct info_about_generator_t*)malloc(sizeof(struct info_about_generator_t*) * (numGeneratorsTheresRoomFor + 1));
		if(numGeneratorsTheresRoomFor > 0)
		{
			//delete a;
			free(a);
		}
		++numGeneratorsTheresRoomFor;
		
		indexToGenerator = numGeneratorsTheresRoomFor - 1;
	}
	else
	{
		for(int i = 0; i < numGeneratorsTheresRoomFor; ++i)
		{
			if(infoPerGenerator[i].bIsGenerator == 0)
			{
				indexToGenerator = i;
				break;
			}
		}
	}
	
	struct info_about_generator_t* infoAboutGenerator = &infoPerGenerator[indexToGenerator];
	*infoAboutGenerator = info_about_generator_default;
	// NOTE: seed == 0 results in seed == 1 after calling lm_generate once
	//       remove seed == 0 as an option and for convenience use the..
	//       .. same sequence for seed == 2^63 and seed == 0
	infoAboutGenerator->seed = seed == 0ull ? 0xffffffffffffffffull : seed;
	infoAboutGenerator->x = infoAboutGenerator->seed;	
	++numGenerators;
	
	*generator = indexToGenerator;
	
	return 1;
}
int lm_add_generator_safely(uint64_t seed, int* generator)
{
	if(generator == NULL)
	{
		return -1;
	}
	
	return lm_add_generator_unsafely(seed, generator);
}

int lm_remove_generator_unsafely(int generator)
{
	--numGenerators;
	
	if(generator == numGeneratorsTheresRoomFor - 1)
	{
		int indexToPreviousGenerator = -1;
		for(int i = generator - 1; i >= 0; --i)
		{
			if(infoPerGenerator[i].bIsGenerator == 1)
			{
				indexToPreviousGenerator = i;
				break;
			}
		}
		
		struct info_about_generator_t* a = infoPerGenerator;
		if(indexToPreviousGenerator > -1)
		{
			int lastNumElementsToRemove = generator - indexToPreviousGenerator;
			
			int b = numGeneratorsTheresRoomFor - lastNumElementsToRemove;
			//infoPerGenerator = new struct info_about_generator_t[b];
			infoPerGenerator = (struct info_about_generator_t*)malloc(sizeof(struct info_about_generator_t) * b);
			memcpy(infoPerGenerator, a, sizeof(struct info_about_generator_t) * b);
		}
		//delete a;
		free(a);
	}
	else
	{
		infoPerGenerator[generator].bIsGenerator = 0;
	}
	
	return 1;
}
int lm_remove_generator_safely(int generator)
{
	if(is_generator(generator) == 0)
	{
		return -1;
	}
	
	return lm_remove_generator_unsafely(generator);
}

int lm_generate_next_unsafely(int generator, uint64_t* a)
{
	struct info_about_generator_t* infoAboutGenerator = &infoPerGenerator[generator];
	
	infoAboutGenerator->x = lm_generate(infoAboutGenerator->x);
	*a = infoAboutGenerator->x;
	
	return 1;
}
int lm_generate_next_safely(int generator, uint64_t* a)
{
	if(is_generator(generator) == 0)
	{
		return -1;
	}
	if(a == NULL)
	{
		return -1;
	}

	return lm_generate_next_unsafely(generator, a);
}

int lm_reset_generator_unsafely(int generator)
{
	struct info_about_generator_t* infoAboutGenerator = &infoPerGenerator[generator];
	
	infoAboutGenerator->x = infoAboutGenerator->seed;	
	
	return 1;
}
int lm_reset_generator_safely(int generator)
{
	if(is_generator(generator) == 0)
	{
		return -1;
	}
	
	return lm_reset_generator_unsafely(generator);
}
#endif
