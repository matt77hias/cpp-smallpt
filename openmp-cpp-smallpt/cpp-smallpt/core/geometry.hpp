#pragma once

struct Ray {

	Ray(const Vector3 &o, const Vector3 &d, double tmin = 0.0, double tmax = INFINITY, int depth = 0) 
			: o(o), d(d), tmin(tmin), tmax(tmax), depth(depth) {};
	
	inline Vector3 operator()(double t) const { return o + d * t; }

	friend inline std::ostream &operator<<(std::ostream& os, const Ray &r) {
		os << "o: " << r.o << std::endl;
		os << "d: " << r.d << std::endl;
		return os;
	}

	Vector3 o, d;
	mutable double tmin, tmax;
	int depth;
};
