#include "PerlinNoiseGenerator.h"
/// <summary>
/// This function will ultimatly generate the result perlin noise but give the user flexibility on how they want to use it. 
/// 
/// For example: 
/// We could be using C++'s native SRNG and giving it a seed value for the random number generator. 
/// However, the user may want their perlin noise to repeat and, thus, would need a permutation table to accomplish this
/// 
/// 
/// </summary>
/// <param name="seedValue"></param>
void PerlinNoiseGenerator::CreatePerlinNoise(const unsigned int& seedValue)
{
	std::system("CLS");

	std::cout << "PerlinNoiseSeedValue is: " << seedValue << std::endl;

	// Randomise the permutation table based on the seed
	std::array<int, 256> permTable = BASE_PERMUTATION_TABLE;

	std::shuffle(permTable.begin(), permTable.end(), std::default_random_engine(seedValue));

	// Test to see if it worked. 
	for (int i = 0; i < 256; i++) {
		std::cout << permTable[i];
		if (i % 16 == 15) { // Break the output into 16 values per line for better readability
			std::cout << "\n";
		}
		else {
			std::cout << ", ";
		}
	}
}
