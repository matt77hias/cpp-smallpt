#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "vector.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace smallpt {

	inline const Vector3 UniformSampleOnSphere(
		double u1, double u2) noexcept {
		
		const double cos_theta = 1.0 - 2.0 * u1;
		const double sin_theta = sqrt(std::max(0.0, 1.0 - cos_theta * cos_theta));
		const double phi = 2.0 * g_pi * u2;
		return Vector3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
	}

	inline const Vector3 UniformSampleOnHemisphere(
		double u1, double u2) noexcept {
		
		// u1 := cos_theta
		const double sin_theta = sqrt(std::max(0.0, 1.0 - u1 * u1));
		const double phi = 2.0 * g_pi * u2;
		return Vector3(cos(phi) * sin_theta, sin(phi) * sin_theta, u1);
	}

	inline const Vector3 CosineWeightedSampleOnHemisphere(
		double u1, double u2) noexcept {
		
		const double cos_theta = sqrt(1.0 - u1);
		const double sin_theta = sqrt(u1);
		const double phi = 2.0 * g_pi * u2;
		return Vector3(cos(phi) * sin_theta, sin(phi) * sin_theta, cos_theta);
	}
}