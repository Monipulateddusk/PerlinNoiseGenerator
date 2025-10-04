#include "PerlinNoiseGenerator.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


/// <summary>
/// This function takes the Perm table and converts coordinates into perlin noise
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
const float PerlinNoiseGenerator::Noise2D(const float& x, const float& y, const int& curOcative)
{
	// Wrapping the X and Y to 256
	const int X = static_cast<int>(std::floor(x)) & 255;
	const int Y = static_cast<int>(std::floor(y)) & 255;

	const float xf = x - std::floor(x);
	const float yf = y - std::floor(y);

	// Declare the corners of the grid square assigned to this coordinate
	const glm::vec2 topLeft = glm::vec2(xf, yf - 1.0f);
	const glm::vec2 topRight = glm::vec2(xf - 1.0f, yf - 1.0f);
	const glm::vec2 bottomLeft = glm::vec2(xf, yf);
	const glm::vec2 bottomRight = glm::vec2(xf - 1.0f, yf);

	// Selecting the value from the permutation array for each corner
	const int aa = p_table[	(p_table[X % 256] + Y		) % 256]; // Bottom left
	const int ab = p_table[	(p_table[X % 256] + Y + 1	) % 256]; // Top left
	const int ba = p_table[	(p_table[(X + 1) % 256] + Y	) % 256]; // Bottom Right
	const int bb = p_table[	(p_table[(X + 1) % 256] + Y + 1) % 256]; // Top Right

	const glm::vec2 vecTopRight = GetConstantVector(bb);
	const glm::vec2 vecTopLeft= GetConstantVector(ab);
	const glm::vec2 vecBottomRight = GetConstantVector(ba);
	const glm::vec2 vecBottomLeft = GetConstantVector(aa);

	const float dotTopRight = glm::dot(topRight, vecTopRight);
	const float dotTopLeft = glm::dot(topLeft, vecTopLeft);
	const float dotBottomRight = glm::dot(bottomRight, vecBottomRight);
	const float dotBottomLeft = glm::dot(bottomLeft, vecBottomLeft);

	const float u = SmoothStep(xf);
	const float v = SmoothStep(yf);

	const float leftLerp = Lerp(dotBottomLeft, dotTopLeft, u);
	const float rightLerp = Lerp(dotBottomRight, dotTopRight, u);
	const float res = Lerp(leftLerp, rightLerp, v);
	return res;
}

const float PerlinNoiseGenerator::FractalBrownianMotion(const float& x, const float& y, const int& octavesNum)
{
	float amplitude = userSelectedAmp;
	float frequency = userSelectedFreq;
	float maxAmp = 0.0f;
	float result = 0;

	for (int o = 0; o < octavesNum; o++) {
		const float n = amplitude * Noise2D((x * frequency) / 256, (y * frequency) / 256, o);
		result += n;

		maxAmp += amplitude;
		amplitude *= 0.5f;
		frequency *= 2.f;
	}
	result /= maxAmp;
	return result;
}

void PerlinNoiseGenerator::CreatePerlinNoiseTexture()
{
	int width = 256, height = 256;
	char* data = new char[256 * 256]{};
	// Create a 1D vector to hold all the texture data (RGBA format)
	std::vector<float> textureData(width * height);  // Correct size for a 2D texture (width * height)

	float floatData;

	p_table = CreatePermutationTable();

	// Populate the texture data with noise values
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			floatData = FractalBrownianMotion((float)w, (float)h, userSelectedOcativeCount);

			floatData = (floatData - 0) / (1 - 0);

			int index = h * width + w;  // Convert 2D coordinates to 1D index
			textureData[index] = floatData;  // Fill with noise value
		}
	}
	// Convert float values in textureData to unsigned char for image format (grayscale)
	for (int i = 0; i < width * height; ++i) {
		data[i] = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, textureData[i] * 255.0f)));
	}

	// Testing path
	//std::filesystem::path currentPath = std::filesystem::current_path();
	//std::cout << "Current working directory: " << currentPath << std::endl;

	
	int res = stbi_write_png("..\\res\\PerlinNoise\\GeneratedPerlinNoise.png", width, height, 1, data, width * 1);

	if (res == 0) {
		std::cout << "UNABLE TO WRITE TO FILE" << std::endl;
	}
	else {
		std::cout << "Result of image is: " << res << std::endl;
	}

	delete[] data;
}


void PerlinNoiseGenerator::DebuggingOutputToConsole(const int& width, const int& height, const unsigned int& seedValue) 
{
	system("cls");
	std::cout << "Seed Value is: " << seedValue << std::endl;

	std::cout.precision(2);
	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			float noiseVal = FractalBrownianMotion((float)w, (float)h, 2);

			std::cout << std::fixed << noiseVal;

			if (w < width - 1) { std::cout << ", "; }
		}
		std::cout << std::endl; // new line at end of each row
	}
}

const unsigned int& PerlinNoiseGenerator::getSeedValue() const
{
	return seed;
}

const int& PerlinNoiseGenerator::getOcativeCount() const
{
	return userSelectedOcativeCount;
}

const float& PerlinNoiseGenerator::getAmpCount() const
{
	return userSelectedAmp;
}

const float& PerlinNoiseGenerator::getFreqCount() const
{
	return userSelectedFreq;
}

const void PerlinNoiseGenerator::setSeedValue(unsigned int& seedValue)
{
	seed = seedValue;
}

const void PerlinNoiseGenerator::setOcativeCount(int& octValue)
{
	userSelectedOcativeCount = octValue;
}

const void PerlinNoiseGenerator::setAmpCount(float& ampValue)
{
	userSelectedAmp = ampValue;
}

const void PerlinNoiseGenerator::setFreqCount(float& freqValue)
{
	userSelectedFreq = freqValue;
}

/// <summary>
/// This function will create and shuffle the required permutation table for use in GLSL. This permutation table will be passed over to GPU side for use in the shader.
/// 
/// Unsure if this is best implementation to do the bulk of the noise function in GPU.
/// </summary>
/// <param name="seedValue"></param>
std::array<int,512> PerlinNoiseGenerator::CreatePermutationTable() 
{
	std::array<int, 512> permTable = {};

	for (int i = 0; i < 256; i++) {
		permTable[i] = BASE_PERMUTATION_TABLE[i];
		permTable[i + 256] = BASE_PERMUTATION_TABLE[i];
	}


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
	const float angle = (const float)((permTableValue / 255.f) * 2.f * (float)(std::_Pi_val));
	return glm::vec2(std::cos(angle), std::sin(angle));
}

const float PerlinNoiseGenerator::Fade(const float& value) 
{
	return value * value * value * (value * (value * 6.0f - 15.0f) + 10.0f);
}

const float PerlinNoiseGenerator::SmoothStep(const float& value)
{
	return value *value * (3.0f - 2.0f *value);
}

const float PerlinNoiseGenerator::Lerp(const float& a, const float& b, const float& v) 
{
	return a * (1.0f - v) + (b * v);
}

/// <summary>
/// Cubic interpolation
/// </summary>
/// <param name="p0"></param>
/// <param name="p1"></param>
/// <param name="p2"></param>
/// <param name="p3"></param>
/// <param name="t"></param>
/// <returns></returns>
const float PerlinNoiseGenerator::Interpolate(const float& p0, const float& p1, const float& p2, const float& p3, const float& t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	// Cubic interpolation formula
	return p1 + 0.5f * t * (p2 - p0 + t * (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3 + t * (3.0f * (p1 - p2) + p3 - p0)));
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
