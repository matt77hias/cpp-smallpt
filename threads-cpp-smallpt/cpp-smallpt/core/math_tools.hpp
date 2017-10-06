#pragma once

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <stdint.h>
#include <math.h>

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace smallpt {

	//-------------------------------------------------------------------------
	// Declarations and Definitions: Constants
	//-------------------------------------------------------------------------

	const double g_pi = 3.14159265358979323846;

	//-------------------------------------------------------------------------
	// Declarations and Definitions: Utilities
	//-------------------------------------------------------------------------

	inline constexpr double Clamp(
		double x, double low = 0.0, double high = 1.0) noexcept {
		
		return (x < high) ? ((x > low) ? x : low) : high;
	}

	inline uint8_t ToByte(double x, double gamma = 2.2) noexcept {
		return static_cast< uint8_t >(Clamp(255.0 * pow(x, 1 / gamma), 
			                                0.0, 255.0));
	}
}