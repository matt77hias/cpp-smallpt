#pragma once

//-----------------------------------------------------------------------------
// SYstem Includes
//-----------------------------------------------------------------------------
#pragma region

#include <stdint.h>
#include <random>

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace smallpt {

	class RNG final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		explicit RNG(uint32_t seed = 606418532) noexcept
			: m_generator(),
			m_distribution(std::uniform_real_distribution< double >(0.0, 1.0)) {

			Seed(seed);
		}
		RNG(const RNG &rng) noexcept = default;
		RNG(RNG &&rng) noexcept = default;
		~RNG() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		RNG &operator=(const RNG &v) = delete;
		RNG &operator=(RNG &&v) = delete;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		void Seed(uint32_t seed) noexcept {
			m_generator.seed(seed);
		}

		double UniformFloat() noexcept {
			return m_distribution(m_generator);
		}

	private:

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		std::default_random_engine m_generator;
		std::uniform_real_distribution< double > m_distribution;
	};
}