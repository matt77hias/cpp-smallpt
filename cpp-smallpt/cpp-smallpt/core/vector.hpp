#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "math_tools.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <algorithm>
#include <iostream>

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace smallpt {

	//-------------------------------------------------------------------------
	// Declarations and Definitions: Vector3
	//-------------------------------------------------------------------------

	struct Vector3 final {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		constexpr explicit Vector3(double a = 0.0) noexcept
			: Vector3(a, a, a) {}
		constexpr Vector3(double x, double y, double z) noexcept
			: m_x(x), m_y(y), m_z(z) {}
		constexpr Vector3(const Vector3 &v) noexcept = default;
		constexpr Vector3(Vector3 &&v) noexcept = default;
		~Vector3() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------

		constexpr Vector3 &operator=(const Vector3 &v) = default;
		constexpr Vector3 &operator=(Vector3 &&v) = default;

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		bool HasNaNs() const noexcept {
			return isnan(m_x) || isnan(m_y) || isnan(m_z);
		}

		constexpr const Vector3 operator-() const noexcept {
			return Vector3(-m_x, -m_y, -m_z);
		}
		
		constexpr const Vector3 operator+(const Vector3 &v) const noexcept {
			return Vector3(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
		}
		constexpr const Vector3 operator-(const Vector3 &v) const noexcept {
			return Vector3(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
		}
		constexpr const Vector3 operator*(const Vector3 &v) const noexcept {
			return Vector3(m_x * v.m_x, m_y * v.m_y, m_z * v.m_z);
		}
		constexpr const Vector3 operator/(const Vector3 &v) const noexcept {
			return Vector3(m_x / v.m_x, m_y / v.m_y, m_z / v.m_z);
		}
		
		constexpr const Vector3 operator+(double a) const noexcept {
			return Vector3(m_x + a, m_y + a, m_z + a);
		}
		constexpr const Vector3 operator-(double a) const noexcept {
			return Vector3(m_x - a, m_y - a, m_z - a);
		}
		constexpr const Vector3 operator*(double a) const noexcept {
			return Vector3(m_x * a, m_y * a, m_z * a);
		}
		constexpr const Vector3 operator/(double a) const noexcept {
			const double inv_a = 1.0 / a;
			return Vector3(m_x * inv_a, m_y * inv_a, m_z * inv_a);
		}
		
		friend constexpr const Vector3 operator+(
			double a, const Vector3 &v) noexcept {
			
			return Vector3(a + v.m_x, a + v.m_y, a + v.m_z);
		}
		friend constexpr const Vector3 operator-(
			double a, const Vector3 &v) noexcept {
			
			return Vector3(a - v.m_x, a - v.m_y, a - v.m_z);
		}
		friend constexpr const Vector3 operator*(
			double a, const Vector3 &v) noexcept {
			
			return Vector3(a * v.m_x, a * v.m_y, a * v.m_z);
		}
		friend constexpr const Vector3 operator/(
			double a, const Vector3 &v) noexcept {
			
			return Vector3(a / v.m_x, a / v.m_y, a / v.m_z);
		}

		constexpr Vector3 &operator+=(const Vector3 &v) noexcept {
			m_x += v.m_x;
			m_y += v.m_y;
			m_z += v.m_z;
			return *this;
		}
		constexpr Vector3 &operator-=(const Vector3 &v) noexcept {
			m_x -= v.m_x;
			m_y -= v.m_y;
			m_z -= v.m_z;
			return *this;
		}
		constexpr Vector3 &operator*=(const Vector3 &v) noexcept {
			m_x *= v.m_x;
			m_y *= v.m_y;
			m_z *= v.m_z;
			return *this;
		}
		constexpr Vector3 &operator/=(const Vector3 &v) noexcept {
			m_x /= v.m_x;
			m_y /= v.m_y;
			m_z /= v.m_z;
			return *this;
		}
		
		constexpr Vector3 &operator+=(double a) noexcept {
			m_x += a;
			m_y += a;
			m_z += a;
			return *this;
		}
		constexpr Vector3 &operator-=(double a) noexcept {
			m_x -= a;
			m_y -= a;
			m_z -= a;
			return *this;
		}
		constexpr Vector3 &operator*=(double a) noexcept {
			m_x *= a;
			m_y *= a;
			m_z *= a;
			return *this;
		}
		constexpr Vector3 &operator/=(double a) noexcept {
			const double inv_a = 1.0 / a;
			m_x *= inv_a;
			m_y *= inv_a;
			m_z *= inv_a;
			return *this;
		}

		constexpr double Dot(const Vector3 &v) const noexcept {
			return m_x * v.m_x + m_y * v.m_y + m_z * v.m_z;
		}
		constexpr const Vector3 Cross(const Vector3 &v) const noexcept {
			return Vector3(m_y * v.m_z - m_z * v.m_y, m_z * v.m_x - m_x * v.m_z, m_x * v.m_y - m_y * v.m_x);
		}

		constexpr bool operator==(const Vector3 &v) const {
			return m_x == v.m_x && m_y == v.m_y && m_z == v.m_z;
		}
		constexpr bool operator!=(const Vector3 &v) const {
			return m_x != v.m_x || m_y != v.m_y || m_z != v.m_z;
		}
		constexpr bool operator<(const Vector3 &v) const {
			return m_x < v.m_x && m_y < v.m_y && m_z < v.m_z;
		}
		constexpr bool operator<=(const Vector3 &v) const {
			return m_x <= v.m_x && m_y <= v.m_y && m_z <= v.m_z;
		}
		constexpr bool operator>(const Vector3 &v) const {
			return m_x > v.m_x && m_y > v.m_y && m_z > v.m_z;
		}
		constexpr bool operator>=(const Vector3 &v) const {
			return m_x >= v.m_x && m_y >= v.m_y && m_z >= v.m_z;
		}

		constexpr double operator[](size_t i) const noexcept {
			return (&m_x)[i];
		}
		constexpr double &operator[](size_t i) noexcept {
			return (&m_x)[i];
		}

		constexpr size_t MinDimension() const noexcept {
			return (m_x < m_y && m_x < m_z) ? 0 : ((m_y < m_z) ? 1 : 2);
		}
		constexpr size_t MaxDimension() const noexcept {
			return (m_x > m_y && m_x > m_z) ? 0 : ((m_y > m_z) ? 1 : 2);
		}
		constexpr double Min() const noexcept {
			return (m_x < m_y && m_x < m_z) ? m_x : ((m_y < m_z) ? m_y : m_z);
		}
		constexpr double Max() const noexcept {
			return (m_x > m_y && m_x > m_z) ? m_x : ((m_y > m_z) ? m_y : m_z);
		}

		constexpr double Norm2_squared() const noexcept {
			return m_x * m_x + m_y * m_y + m_z * m_z;
		}
		double Norm2() const noexcept {
			return sqrt(Norm2_squared());
		}
		void Normalize() noexcept {
			const double a = 1.0 / Norm2();
			m_x *= a;
			m_y *= a;
			m_z *= a;
		}

		friend std::ostream &operator<<(std::ostream& os, const Vector3 &v) {
			os << '[' << v.m_x << ' ' << v.m_y << ' ' << v.m_z << ']';
			return os;
		}

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		double m_x, m_y, m_z;
	};
	
	//-------------------------------------------------------------------------
	// Declarations and Definitions: Vector3 Utilities
	//-------------------------------------------------------------------------

	inline const Vector3 Sqrt(const Vector3 &v) noexcept {
		return Vector3(sqrt(v.m_x), 
			           sqrt(v.m_y), 
			           sqrt(v.m_z));
	}
	
	inline const Vector3 Pow(const Vector3 &v, double a) noexcept {
		return Vector3(pow(v.m_x, a), 
			           pow(v.m_y, a), 
			           pow(v.m_z, a));
	}
	
	inline const Vector3 Abs(const Vector3 &v) noexcept {
		return Vector3(abs(v.m_x), 
			           abs(v.m_y), 
			           abs(v.m_z));
	}
	
	inline const Vector3 Min(const Vector3 &v1, const Vector3 &v2) noexcept {
		return Vector3(std::min(v1.m_x, v2.m_x), 
			           std::min(v1.m_y, v2.m_y), 
			           std::min(v1.m_z, v2.m_z));
	}
	
	inline const Vector3 Max(const Vector3 &v1, const Vector3 &v2) noexcept {
		return Vector3(std::max(v1.m_x, v2.m_x), 
			           std::max(v1.m_y, v2.m_y), 
			           std::max(v1.m_z, v2.m_z));
	}
	
	inline const Vector3 Round(const Vector3 &v) noexcept {
		return Vector3(std::round(v.m_x), 
			           std::round(v.m_y), 
			           std::round(v.m_z));
	}
	
	inline const Vector3 Floor(const Vector3 &v) noexcept {
		return Vector3(std::floor(v.m_x), 
			           std::floor(v.m_y), 
			           std::floor(v.m_z));
	}
	
	inline const Vector3 Ceil(const Vector3 &v) noexcept {
		return Vector3(std::ceil(v.m_x),
			           std::ceil(v.m_y), 
			           std::ceil(v.m_z));
	}
	
	inline const Vector3 Trunc(const Vector3 &v) noexcept {
		return Vector3(std::trunc(v.m_x), 
			           std::trunc(v.m_y), 
			           std::trunc(v.m_z));
	}
	
	constexpr const Vector3 Clamp(
		const Vector3 &v, double low = 0.0, double high = 1.0) noexcept {
		
		return Vector3(Clamp(v.m_x, low, high), 
			           Clamp(v.m_y, low, high), 
			           Clamp(v.m_z, low, high));
	}
	
	constexpr const Vector3 Lerp(
		double a, const Vector3 &v1, const Vector3 &v2) noexcept {
		
		return v1 + a * (v2 - v1);
	}
	
	constexpr const Vector3 Permute(
		const Vector3 &v, size_t x, size_t y, size_t z) noexcept {
		
		return Vector3(v[x], v[y], v[z]);
	}

	inline const Vector3 Normalize(const Vector3 &v) noexcept {
		const double a = 1.0 / v.Norm2();
		return a * v;
	}
}
