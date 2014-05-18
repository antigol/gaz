#include "particle.hh"
#include <cmath>

Particle::Particle()
{
	m = 1.67e-27;
	r = 1.2;
	color = Vec3(1.0, 1.0, 1.0);
}

int Particle::collision(Particle &a, Particle &b)
{
	Vec3 n = a.q - b.q;
	double n_norm = Vec3::norm(n);
	// détecte si les particules sont proches et aussi si elle se font face
	if (n_norm < pow(a.r + b.r, 2.0) && Vec3::dot(n, a.p - b.p) < 0.0) {
		// on ce place dans le référentiel de centre de masse
		Vec3 vg = (a.p + b.p) / (a.m + b.m);
		// p : q. de mvt de la particule a, la particule b à une q. de mvt de -p
		Vec3 p = a.p - a.m * vg;
		Vec3 p_prim = p - 2.0 * Vec3::dot(p, n) / n_norm * n;
		// on retourne dans les coordonnées globales
		a.p = a.m * vg + p_prim;
		b.p = b.m * vg - p_prim;

		return 1;
	}
	return 0;
}
