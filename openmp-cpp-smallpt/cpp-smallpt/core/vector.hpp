#pragma once

struct Vector3 {

	Vector3(double a = 0) : x(a), y(a), z(a) {}
	Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
	Vector3(const Vector3 &v) : x(v.x), y(v.y), z(v.z) {}
	
	inline bool HasNaNs() const {
		return isnan(x) || isnan(y) || isnan(z);
	}

	inline Vector3 &operator=(const Vector3 &v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}

	inline Vector3 operator-() const { 
		return Vector3(-x, -y, -z); 
	}
	inline Vector3 operator+(const Vector3 &v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	inline Vector3 operator-(const Vector3 &v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	inline Vector3 operator*(const Vector3 &v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}
	inline Vector3 operator/(const Vector3 &v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}
	inline Vector3 operator+(double a) const {
		return Vector3(x + a, y + a, z + a);
	}
	inline Vector3 operator-(double a) const {
		return Vector3(x - a, y - a, z - a);
	}
	inline Vector3 operator*(double a) const {
		return Vector3(x * a, y * a, z * a);
	}
	inline Vector3 operator/(double a) const {
		const double inv_a = 1.0 / a;
		return Vector3(x * inv_a, y * inv_a, z * inv_a);
	}
	friend inline Vector3 operator+(double a, const Vector3 &v) {
		return Vector3(v.x + a, v.y + a, v.z + a);
	}
	friend inline Vector3 operator-(double a, const Vector3 &v) {
		return Vector3(v.x - a, v.y - a, v.z - a);
	}
	friend inline Vector3 operator*(double a, const Vector3 &v) {
		return Vector3(v.x * a, v.y * a, v.z * a);
	}
	friend inline Vector3 operator/(double a, const Vector3 &v) {
		const double inv_a = 1.0 / a;
		return Vector3(v.x * inv_a, v.y * inv_a, v.z * inv_a);
	}

	inline Vector3 &operator+=(const Vector3 &v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	inline Vector3 &operator-=(const Vector3 &v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	inline Vector3 &operator*=(const Vector3 &v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}
	inline Vector3 &operator/=(const Vector3 &v) {
		x /= v.x;
		y /= v.y;
		z /= v.z;
		return *this;
	}
	inline Vector3 &operator+=(double a) {
		x += a;
		y += a;
		z += a;
		return *this;
	}
	inline Vector3 &operator-=(double a) {
		x -= a;
		y -= a;
		z -= a;
		return *this;
	}
	inline Vector3 &operator*=(double a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}
	inline Vector3 &operator/=(double a) {
		const double inv_a = 1.0 / a;
		x *= inv_a;
		y *= inv_a;
		z *= inv_a;
		return *this;
	}

	inline double Dot(const Vector3 &v) const { 
		return x * v.x + y * v.y + z * v.z; 
	}
	inline Vector3 Cross(const Vector3 &v) const { 
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); 
	}

	inline bool operator==(const Vector3 &v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	inline bool operator!=(const Vector3 &v) const {
		return x != v.x || y != v.y || z != v.z;
	}
	inline bool operator<(const Vector3 &v) const {
		return x < v.x && y < v.y && z < v.z;
	}
	inline bool operator<=(const Vector3 &v) const {
		return x <= v.x && y <= v.y && z <= v.z;
	}
	inline bool operator>(const Vector3 &v) const {
		return x > v.x && y > v.y && z > v.z;
	}
	inline bool operator>=(const Vector3 &v) const {
		return x >= v.x && y >= v.y && z >= v.z;
	}

	friend inline Vector3 Sqrt(const Vector3 &v) {
		return Vector3(sqrt(v.x), sqrt(v.y), sqrt(v.z));
	}
	friend inline Vector3 Pow(const Vector3 &v, double a) {
		return Vector3(pow(v.x, a), pow(v.y, a), pow(v.z, a));
	}
	friend inline Vector3 Abs(const Vector3 &v) {
		return Vector3(abs(v.x), abs(v.y), abs(v.z));
	}
	friend inline Vector3 Min(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}
	friend inline Vector3 Max(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}
	friend inline Vector3 Round(const Vector3 &v) {
		return Vector3(std::round(v.x), std::round(v.y), std::round(v.z));
	}
	friend inline Vector3 Floor(const Vector3 &v) {
		return Vector3(std::floor(v.x), std::floor(v.y), std::floor(v.z));
	}
	friend inline Vector3 Ceil(const Vector3 &v) {
		return Vector3(std::ceil(v.x), std::ceil(v.y), std::ceil(v.z));
	}
	friend inline Vector3 Trunc(const Vector3 &v) {
		return Vector3(std::trunc(v.x), std::trunc(v.y), std::trunc(v.z));
	}
	friend inline Vector3 Clamp(const Vector3 &v, double low = 0, double high = 1) {
		return Vector3(Clamp(v.x, low, high), Clamp(v.y, low, high), Clamp(v.z, low, high));
	}
	friend inline Vector3 Lerp(double a, const Vector3 &v1, const Vector3 &v2) {
		return v1 + a * (v2 - v1);
	}
	friend inline Vector3 Permute(const Vector3 &v, int x, int y, int z) {
		return Vector3(v[x], v[y], v[z]);
	}

	inline double operator[](size_t i) const {
		return raw[i];
	}
	inline double &operator[](size_t i) {
		return raw[i];
	}

	inline int MinDimension() const {
		return (x < y && x < z) ? 0 : ((y < z) ? 1 : 2);
	}
	inline int MaxDimension() const {
		return (x > y && x > z) ? 0 : ((y > z) ? 1 : 2);
	}
	inline double Min() const {
		return (x < y && x < z) ? x : ((y < z) ? y : z);
	}
	inline double Max() const {
		return (x > y && x > z) ? x : ((y > z) ? y : z);
	}

	inline double Norm2_squared() const { 
		return x * x + y * y + z * z; 
	}
	inline double Norm2() const { 
		return sqrt(Norm2_squared());
	}
	inline Vector3 &Normalize() { 
		const double a = 1 / Norm2();
		x *= a;
		y *= a;
		z *= a;
		return *this; 
	}

	friend inline std::ostream &operator<<(std::ostream& os, const Vector3 &v) {
		os << '[' << v.x << ' ' << v.y << ' ' << v.z << ']';
		return os;
	}

	union {
		struct {
			double x, y, z;
		};
		double raw[3];
	};
};