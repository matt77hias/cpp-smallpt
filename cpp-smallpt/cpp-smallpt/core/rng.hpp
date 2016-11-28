#pragma once

#include <random>

class RNG {
public:
	RNG(uint32_t seed = 606418532) {
		distribution = std::uniform_real_distribution<double>(0.0, 1.0);
		Seed(seed);
	}

	void Seed(uint32_t seed) {
		generator.seed(seed);
	}

	inline double UniformFloat() {
		return distribution(generator);
	}

private:
	std::default_random_engine generator;
	std::uniform_real_distribution<double> distribution;
};