#include "PerlinNoiseGenerator.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


/// <summary>
/// This function takes the Perm table and converts coordinates into perlin noise
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
const float& PerlinNoiseGenerator::Noise2D(const float& x, const float& y)
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

	const glm::vec2 vecTopRight = GetConstantVector(valueTopRight);
	const glm::vec2 vecTopLeft= GetConstantVector(valueTopLeft);
	const glm::vec2 vecBottomRight = GetConstantVector(valueBottomRight);
	const glm::vec2 vecBottomLeft = GetConstantVector(valueBottomLeft);

	const float dotTopRight = glm::dot(topRight, vecTopRight);
	const float dotTopLeft = glm::dot(topLeft, vecTopLeft);
	const float dotBottomRight = glm::dot(bottomRight, vecBottomRight);
	const float dotBottomLeft = glm::dot(bottomLeft, vecBottomLeft);

	const float u = Fade(xf);
	const float v = Fade(yf);

	const float leftLerp = Lerp(dotBottomLeft, dotTopLeft, v);
	const float rightLerp = Lerp(dotBottomRight, dotTopRight, v);
	return Lerp(leftLerp, rightLerp, u);
}

const float& PerlinNoiseGenerator::FractalBrownianMotion(const float& x, const float& y, const int& octavesNum)
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

void PerlinNoiseGenerator::CreatePerlinNoiseTexture()
{
	int width = 256, height = 256;
	unsigned char data[256 * 256]{};
	// Create a 1D vector to hold all the texture data (RGBA format)
	std::vector<float> textureData(width * height);  // Correct size for a 2D texture (width * height)

	float floatData;
	// Populate the texture data with noise values
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			floatData = FractalBrownianMotion(w, h, 1);

			int index = h * width + w;  // Convert 2D coordinates to 1D index
			textureData[index] = floatData;  // Fill with noise value
		}
	}
	// Convert float values in textureData to unsigned char for image format (grayscale)
	for (int i = 0; i < width * height; ++i) {
		data[i] = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, 255.f - textureData[i] * 255.0f)));
	}

	// Testing path
	//std::filesystem::path currentPath = std::filesystem::current_path();
	//std::cout << "Current working directory: " << currentPath << std::endl;


	int res = stbi_write_png("GeneratedPerlinNoise.png", width, height, 1, data, width * 1);

	if (res == 0) {
		std::cout << "UNABLE TO WRITE TO FILE" << std::endl;
	}
	else {
		std::cout << "Result of image is: " << res << std::endl;
	}
}


void PerlinNoiseGenerator::DebuggingOutputToConsole(const int& width, const int& height, const unsigned int& seedValue) 
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
std::array<int,256> PerlinNoiseGenerator::CreatePermutationTable() 
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
glm::vec2 PerlinNoiseGenerator::GetConstantVector(const unsigned int& permTableValue) 
{
	const float angle = (permTableValue / 255.f) * 2.f * std::_Pi_val;
	return glm::vec2(std::cos(angle), std::sin(angle));
	
}

const float& PerlinNoiseGenerator::Fade(const float& value) 
{
	return ((6* value  - 15) * value + 10) * value * value * value;
}

const float& PerlinNoiseGenerator::Lerp(const float& a, const float& b, const float& v) 
{
	return a * (1.0 - v) + (b * v);
}

unsigned char* PerlinNoiseGenerator::ConvertFloatVecToUnsignedChar(const std::vector<float> floatVec)
{
	// Create a buffer large enough to hold all the bytes of the float vector
	size_t byteSize = floatVec.size() * sizeof(float);
	unsigned char* byteArray = new unsigned char[byteSize];

	// Copy the raw data from the vector into the byte array
	std::memcpy(byteArray, floatVec.data(), byteSize);

	return byteArray;
}
