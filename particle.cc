#include "particle.hh"
#include <cmath>

Particle::Particle()
{
	m = 1.0;
	r = 1.0;
	color = Vec3(1.0, 1.0, 1.0);
}

void Particle::collision(Particle *a, Particle *b)
{
	if (a == b)
		return;

	Vec3 n = a->q - b->q;
	double n_norm = Vec3::norm(n);
	// détecte si les particules sont proches et aussi si elle se font face
	if (n_norm < pow(a->r + b->r, 2.0) && Vec3::dot(n, a->p * b->m - b->p * a->m) < 0.0) {
		// on ce place dans le référentiel de centre de masse
		Vec3 vg = (a->p + b->p) / (a->m + b->m);
		// p : q. de mvt de la particule a, la particule b à une q. de mvt de -p
		Vec3 p = a->p - a->m * vg;
		Vec3 p_prim = p - 2.0 * Vec3::dot(p, n) / n_norm * n;
		// on retourne dans les coordonnées globales
		a->p = a->m * vg + p_prim;
		b->p = b->m * vg - p_prim;
	}
}
