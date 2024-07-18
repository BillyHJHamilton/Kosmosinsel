
#include <random>

// Use the hardware random generator to seed the
// Mersenne Twister generator (faster and adequate for games). 
static std::mt19937 s_Generator(std::random_device{}());

float Random(float min, float max)
{
	std::uniform_real_distribution<float> Distribution(min, max);
	return Distribution(s_Generator);
}
