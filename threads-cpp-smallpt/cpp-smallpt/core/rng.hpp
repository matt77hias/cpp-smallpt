#pragma once

#include <stdint.h>
#include <random>

namespace smallpt {

	class RNG final {

	public:

		explicit RNG(uint32_t seed = 606418532)
			: m_generator(),
			m_distribution(std::uniform_real_distribution< double >(0.0, 1.0)) {

			Seed(seed);
		}
		RNG(const RNG &rng) = default;
		RNG(RNG &&rng) = default;
		~RNG() = default;

		RNG &operator=(const RNG &v) = delete;
		RNG &operator=(RNG &&v) = delete;

		void Seed(uint32_t seed) noexcept {
			m_generator.seed(seed);
		}

		double UniformFloat() noexcept {
			return m_distribution(m_generator);
		}

	private:

		std::default_random_engine m_generator;
		std::uniform_real_distribution< double > m_distribution;
	};
}