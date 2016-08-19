#pragma once

inline Vector3 UniformSampleOnHemisphere(double u1, double u2) {
	const double r = sqrt(std::max(0.0, 1.0 - u1 * u1));
	const double phi = 2.0 * M_PI * u2;
	return Vector3(r * cos(phi), r * sin(phi), u1);
}