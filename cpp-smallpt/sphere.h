#pragma once

#include "cpp-smallpt.h"

enum Reflection_t { DIFFUSE, SPECULAR, REFRACTIVE };

#define EPSILON_SPHERE 1e-4

struct Sphere {

	Sphere(double r, const Vector3 &p, const Vector3 &e, const Vector3 &f, Reflection_t reflection_t)
		: r(r), p(p), e(e), f(f), reflection_t(reflection_t) {}

	bool Intersect(const Ray &ray) const {
		// (o + t*d - p) . (o + t*d - p) - r*r = 0
		// <=> (d . d) * t^2 + 2 * d . (o - p) * t + (o - p) . (o - p) - r*r = 0
		// 
		// Discriminant check
		// (2 * d . (o - p))^2 - 4 * (d . d) * ((o - p) . (o - p) - r*r) <? 0
		// <=> (d . (o - p))^2 - (d . d) * ((o - p) . (o - p) - r*r) <? 0
		// <=> (d . op)^2 - 1 * (op . op - r*r) <? 0
		// <=> b^2 - (op . op) + r*r <? 0
		// <=> D <? 0
		//
		// Solutions
		// t = (- 2 * d . (o - p) +- 2 * sqrt(D)) / (2 * (d . d))
		// <=> t = dop +- sqrt(D)

		Vector3 op = p - ray.o;
		const double dop = ray.d.Dot(op);
		const double D = dop * dop - op.Dot(op) + r * r;

		if (D < 0)
			return false;

		const double sqrtD = sqrt(D);
		
		const double tmin = dop - sqrtD;
		if (ray.tmin < tmin && tmin < ray.tmax) {
			ray.tmax = tmin;
			return true;
		}

		const double tmax = dop + sqrtD;
		if (ray.tmin < tmax && tmax < ray.tmax) {
			ray.tmax = tmax;
			return true;
		}

		return false;
	}

	double r;
	Vector3 p, e, f; // position, emission, reflection
	Reflection_t reflection_t;
};