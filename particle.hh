#ifndef PARTICLE_HH
#define PARTICLE_HH

#include "vec3.hh"

struct Particle
{
	Particle();

	double m; // masse
	Vec3 p; // quantité de mouvement
	Vec3 q; // position
	double r; // rayon
	double rg; // rayon graviation
	Vec3 color;
	Vec3 f; // force

    static void collision(Particle *a, Particle *b, double cor);
};

#endif // PARTICLE_HH
