#include "stdafx.h"

#include "sphere.hpp"
#include "specular.hpp"

// Scene
#define REFRACTIVE_INDEX_OUT 1.0
#define REFRACTIVE_INDEX_IN 1.5

Sphere spheres[] = {
	Sphere(1e5,  Vector3(1e5 + 1, 40.8, 81.6),   Vector3(),   Vector3(0.75,0.25,0.25), DIFFUSE),	//Left
	Sphere(1e5,  Vector3(-1e5 + 99, 40.8, 81.6), Vector3(),   Vector3(0.25,0.25,0.75), DIFFUSE),	//Right
	Sphere(1e5,  Vector3(50, 40.8, 1e5),         Vector3(),   Vector3(0.75),           DIFFUSE),	//Back
	Sphere(1e5,  Vector3(50, 40.8, -1e5 + 170),  Vector3(),   Vector3(),               DIFFUSE),	//Front
	Sphere(1e5,  Vector3(50, 1e5, 81.6),         Vector3(),   Vector3(0.75),           DIFFUSE),	//Bottom
	Sphere(1e5,  Vector3(50, -1e5 + 81.6, 81.6), Vector3(),   Vector3(0.75),           DIFFUSE),	//Top
	Sphere(16.5, Vector3(27, 16.5, 47),          Vector3(),   Vector3(0.999),          SPECULAR),	//Mirror
	Sphere(16.5, Vector3(73, 16.5, 78),          Vector3(),   Vector3(0.999),          REFRACTIVE),	//Glass
	Sphere(600,	 Vector3(50, 681.6 - .27, 81.6), Vector3(12), Vector3(),               DIFFUSE)		//Light
};


bool Intersect(const Ray &ray, size_t &id) {
	bool hit = false;
	const size_t n = sizeof(spheres) / sizeof(Sphere);
	for (size_t i = 0; i < n; ++i) {
		if (spheres[i].Intersect(ray)) {
			hit = true;
			id = i;
		}
	}
	return hit;
}


bool Intersect(const Ray &ray) {
	const size_t n = sizeof(spheres) / sizeof(Sphere);
	for (size_t i = 0; i < n; ++i)
		if (spheres[i].Intersect(ray))
			return true;
	return false;
}


Vector3 Radiance(const Ray &ray, RNG &rng) {
	Ray r = ray;
	Vector3 L;
	Vector3 F(1.0);

	while (true) {
		size_t id;
		if (!Intersect(r, id))
			return L;

		const Sphere &shape = spheres[id];
		const Vector3 p = r(r.tmax);
		const Vector3 n = (p - shape.p).Normalize();

		L += F * shape.e;
		F *= shape.f;
		
		// Russian roulette
		if (r.depth > 4) {
			const double continue_probability = shape.f.Max();
			if (rng.UniformFloat() >= continue_probability)
				return L;
			F /= continue_probability;
		}

		// Next path segment
		switch (shape.reflection_t) {	
		case SPECULAR: {
			const Vector3 d = IdealSpecularReflect(r.d, n);
			r = Ray(p, d, EPSILON_SPHERE, INFINITY, r.depth + 1);
			break;
		}	
		case REFRACTIVE: {
			double pr;
			const Vector3 d = IdealSpecularTransmit(r.d, n, REFRACTIVE_INDEX_OUT, REFRACTIVE_INDEX_IN, pr, rng);
			F *= pr;
			r = Ray(p, d, EPSILON_SPHERE, INFINITY, r.depth + 1);
			break;
		}
		default: {
			const Vector3 w = n.Dot(r.d) < 0 ? n : -n;
			const Vector3 u = ((std::abs(w.x) > 0.1 ? Vector3(0.0, 1.0, 0.0) : Vector3(1.0, 0.0, 0.0)).Cross(w)).Normalize();
			const Vector3 v = w.Cross(u);

			const Vector3 sample_d = CosineWeightedSampleOnHemisphere(rng.UniformFloat(), rng.UniformFloat());
			const Vector3 d = (sample_d.x * u + sample_d.y * v + sample_d.z * w).Normalize();
			r = Ray(p, d, EPSILON_SPHERE, INFINITY, r.depth + 1);
		}
		}
	}
}


class RenderTask : public Task {

public:

	RenderTask(int y, int w, int h, int nb_samples, const Vector3 &eye, const Vector3 &gaze, const Vector3 &cx, const Vector3 &cy, RNG &rng, Vector3 *Ls)
		: y(y), w(w), h(h), nb_samples(nb_samples), eye(eye), gaze(gaze), cx(cx), cy(cy), rng(rng), Ls(Ls) {}

	virtual void Run() {
		for (int x = 0; x < w; ++x) // pixel column
			for (int sy = 0, i = (h - 1 - y) * w + x; sy < 2; ++sy) // 2 subpixel row
				for (int sx = 0; sx < 2; ++sx) { // 2 subpixel column
					Vector3 L;
					for (int s = 0; s < nb_samples; s++) { // samples per subpixel
						const double u1 = 2.0 * rng.UniformFloat();
						const double u2 = 2.0 * rng.UniformFloat();
						const double dx = u1 < 1 ? sqrt(u1) - 1.0 : 1.0 - sqrt(2.0 - u1);
						const double dy = u2 < 1 ? sqrt(u2) - 1.0 : 1.0 - sqrt(2.0 - u2);
						Vector3 d = cx * (((sx + 0.5 + dx) / 2 + x) / w - 0.5) +
									cy * (((sy + 0.5 + dy) / 2 + y) / h - 0.5) + gaze;
						L += Radiance(Ray(eye + d * 140, d.Normalize(), EPSILON_SPHERE), rng) * (1.0 / nb_samples);
					}
					Ls[i] += 0.25 * Clamp(L);
				}
	}

private:

	const int y;
	const int w;
	const int h;
	const int nb_samples;

	const Vector3 &eye;
	const Vector3 &gaze;
	const Vector3 &cx;
	const Vector3 &cy;
	
	RNG &rng;

	Vector3 *Ls;
};


int main(int argc, char *argv[]) {
	RNG rng;
	const int nb_samples = (argc == 2) ? atoi(argv[1]) / 4 : 1;

	const int w = 1024;
	const int h = 768;
	
	const Vector3 eye = Vector3(50, 52, 295.6);
	const Vector3 gaze = Vector3(0, -0.042612, -1).Normalize();
	const double fov = 0.5135;
	const Vector3 cx = Vector3(w * fov / h, 0.0, 0.0);
	const Vector3 cy = (cx.Cross(gaze)).Normalize() * fov;

	Vector3 *Ls = new Vector3[w * h];
	
	std::vector< Task * > render_tasks;
	for (int y = 0; y < h; ++y) // pixel row
		render_tasks.push_back(new RenderTask(y, w, h, nb_samples, eye, gaze, cx, cy, rng, Ls));
	EnqueueTasks(render_tasks);
	WaitForAllTasks();
	for (int y = 0; y < render_tasks.size(); ++y)
		delete render_tasks[y];
	TasksCleanup();
	
	WritePPM(w, h, Ls);

	delete[] Ls;
}
