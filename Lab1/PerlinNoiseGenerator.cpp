#include "PerlinNoiseGenerator.h"

/// <summary>
/// This function takes the Perm table and converts coordinates into perlin noise
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
const float& PerlinNoiseGenerator::Noise2D(const float& x, const float& y) const
{
	// Wrapping the X and Y to 256
	const int X = static_cast<int>(std::floor(x)) & 255;
	const int Y = static_cast<int>(std::floor(y)) & 255;

	const float xf = x - std::floor(x);
	const float yf = y - std::floor(y);

	// Declare the corners of the grid square assigned to this coordinate
	const glm::vec2 topRight = glm::vec2(xf - 1.0f, yf - 1.0f);
	const glm::vec2 topLeft = glm::vec2(xf, yf - 1.0f);
	const glm::vec2 bottomRight = glm::vec2(xf - 1.0f, yf);
	const glm::vec2 bottomLeft = glm::vec2(xf, yf);

	std::array<int, 256> permTable = CreatePermutationTable();

	// Selecting the value from the permutation array for each corner
	const int valueTopRight = permTable[permTable[X + 1] + Y + 1];
	const int valueTopLeft = permTable[permTable[X] + Y + 1];
	const int valueBottomRight = permTable[permTable[X + 1] + Y];
	const int valueBottomLeft = permTable[permTable[X]+ Y];

	const float dotTopRight = glm::dot(topRight, GetConstantVector(valueTopRight));
	const float dotTopLeft = glm::dot(topLeft, GetConstantVector(valueTopLeft));
	const float dotBottomRight = glm::dot(bottomRight, GetConstantVector(valueBottomRight));
	const float dotBottomLeft = glm::dot(bottomLeft, GetConstantVector(valueBottomLeft));

	const float u = Fade(xf);
	const float v = Fade(yf);

	const float leftLerp = Lerp(dotBottomLeft, dotTopLeft, v);
	const float rightLerp = Lerp(dotBottomRight, dotTopRight, v);
	return Lerp(leftLerp, rightLerp, u);
}

const float& PerlinNoiseGenerator::FractalBrownianMotion(const float& x, const float& y, const int& octavesNum) const
{
	float amplitude = 1.0f;
	float frequency = 0.005;
	float result = 0;

	for (int o = 0; o < octavesNum; o++) {
		const float n = amplitude * Noise2D(x * frequency, y * frequency);
		result += n;

		amplitude *= .5f;
		frequency *= 2.f;
	}
	return result;
}

void PerlinNoiseGenerator::CreatePerlinNoiseTexture(const int& width, const int& height) 
{
	// Create a 1D vector to hold all the texture data (RGBA format)
	std::vector<float> textureData(width * height);  // Correct size for a 2D texture (width * height)

	// Populate the texture data with noise values
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			int index = h * width + w;  // Convert 2D coordinates to 1D index
			textureData[index] = FractalBrownianMotion(w, h, 3);  // Fill with noise value
		}
	}

	// Initialize the texture with the generated data
	perlinNoiseTexture.initData(width, height, &textureData);

	std::cout << "Sample texture data:" << std::endl;
	for (int h = 0; h < height && h < 5; h++) {  // Print first 5 rows for brevity
		for (int w = 0; w < width && w < 5; w++) {  // Print first 5 columns for brevity
			int index = h * width + w;
			std::cout << textureData[index] << " ";
		}
		std::cout << std::endl;
	}

}


void PerlinNoiseGenerator::DebuggingOutputToConsole(const int& width, const int& height, const unsigned int& seedValue) const
{
	system("cls");
	std::cout << "Seed Value is: " << seedValue << std::endl;

	std::cout.precision(2);
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			float noiseVal = FractalBrownianMotion(w, h, 2);

			std::cout << std::fixed << noiseVal;

			if (w < width - 1) { std::cout << ", "; }
		}
		std::cout << std::endl; // new line at end of each row
	}
}

const void PerlinNoiseGenerator::SetSeedValue(const unsigned int& seedValue) const
{
	seed = seedValue;
}

/// <summary>
/// This function will create and shuffle the required permutation table for use in GLSL. This permutation table will be passed over to GPU side for use in the shader.
/// 
/// Unsure if this is best implementation to do the bulk of the noise function in GPU.
/// </summary>
/// <param name="seedValue"></param>
std::array<int,256> PerlinNoiseGenerator::CreatePermutationTable() const
{
	// Randomise the permutation table based on the seed
	std::array<int, 256> permTable = BASE_PERMUTATION_TABLE;

	std::shuffle(permTable.begin(), permTable.end(), std::default_random_engine(seed));

	return permTable;
}

/// <summary>
/// Wasn't too keen on their implementation for the constant vector assigning via hard coded vectors. This way is more computation but more flexible
/// </summary>
/// <param name="permTableValue"></param>
/// <returns></returns>
glm::vec2 PerlinNoiseGenerator::GetConstantVector(const unsigned int& permTableValue) const
{
	const float angle = (permTableValue / 255.f) * 2.f * std::_Pi_val;
	return glm::vec2(std::cos(angle), std::sin(angle));
}

const float& PerlinNoiseGenerator::Fade(const float& value) const
{
	return ((6* value  - 15) * value + 10) * value * value * value;
}

const float& PerlinNoiseGenerator::Lerp(const float& a, const float& b, const float& v) const
{
	return a * (1.0 - v) + (b * v);
}
