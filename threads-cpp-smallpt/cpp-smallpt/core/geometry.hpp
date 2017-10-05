#pragma once

#include "vector.hpp"

namespace smallpt {

	struct Ray final {

	public:

		constexpr explicit Ray(const Vector3 &o, const Vector3 &d, 
			double tmin = 0.0, double tmax = INFINITY, uint32_t depth = 0)
			: m_o(o), m_d(d),
			m_tmin(tmin), m_tmax(tmax), m_depth(depth) {};
		constexpr explicit Ray(Vector3 &&o, Vector3 &&d,
			double tmin = 0.0, double tmax = INFINITY, uint32_t depth = 0)
			: m_o(std::move(o)), m_d(std::move(d)),
			m_tmin(tmin), m_tmax(tmax), m_depth(depth) {};
		constexpr Ray(const Ray &ray) = default;
		constexpr Ray(Ray &&ray) = default;
		~Ray() = default;

		constexpr Ray &operator=(const Ray &ray) = default;
		constexpr Ray &operator=(Ray &&ray) = default;

		constexpr const Vector3 operator()(double t) const { 
			return m_o + m_d * t; 
		}

		friend inline std::ostream &operator<<(std::ostream& os, const Ray &r) {
			os << "o: " << r.m_o << std::endl;
			os << "d: " << r.m_d << std::endl;
			return os;
		}

		Vector3 m_o, m_d;
		mutable double m_tmin, m_tmax;
		uint32_t m_depth;
	};
}
