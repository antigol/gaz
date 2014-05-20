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
	Vec3 color;

	static int collision(Particle *a, Particle *b);
};

#endif // PARTICLE_HH
