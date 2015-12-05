#include "particle.hh"
#include <cmath>
#include <QDebug>

Particle::Particle()
{
  m = 1.0;
  r = 1.0;
  rg = 0.0;
  color = Vec3(1.0, 1.0, 1.0);
  f.setNull();
}

void Particle::simple_interaction(Particle *a, Particle *b, double cor)
{
  if (a == b)
    return;

  Vec3 n = a->q - b->q;
  double n_norm = Vec3::norm(n); // longueur au carré
  // détecte si les particules sont proches et aussi si elle se font face
  if (n_norm < (a->r + b->r)*(a->r + b->r) && Vec3::dot(n, a->p * b->m - b->p * a->m) < 0.0) {
    // on ce place dans le référentiel de centre de masse
    Vec3 vg = (a->p + b->p) / (a->m + b->m);
    // p  = q. de mvt de la particule a;
    // -p = q. de mvt de la particule b;
    Vec3 p = a->p - a->m * vg;
    Vec3 p_prim = p - (1.0+cor) * Vec3::dot(p, n) / n_norm * n;
    // on retourne dans les coordonnées globales
    a->p = a->m * vg + p_prim;
    b->p = b->m * vg - p_prim;
  }

  // interaction gravitationelle
  else if (n_norm < (a->rg + b->rg)*(a->rg + b->rg)) {
    // F = ma*mb / r^2
    Vec3 F = a->m * b->m * n * pow(n_norm, -3.0/2.0);
    a->f -= F;
    b->f += F;
  }
}

void Particle::backtrack_interaction(Particle* a, Particle* b, double cor)
{
  if (a == b)
    return;

  double r = a->r + b->r;
  double r2 = r*r;
  Vec3 q = a->q - b->q;
  double q2 = Vec3::norm(q); // longueur au carré

  Vec3 v = a->p/a->m + a->p/b->m;
  double qv = Vec3::dot(q, v);

  // détecte si les particules sont proches et aussi si elle se font face
  if (q2 < r2 && qv < 0.0) {
    // différance de vitesse
    double v2 = Vec3::norm(v);

    // on ce place dans le référentiel de centre de masse
    Vec3 vg = (a->p + b->p) / (a->m + b->m);
    // p  = q. de mvt de la particule a;
    // -p = q. de mvt de la particule b;
    Vec3 p = a->p - a->m * vg;
    Vec3 p_prim = p - (1.0+cor) * Vec3::dot(p, q) / q2 * q;
    // on retourne dans les coordonnées globales
    Vec3 apf = a->m * vg + p_prim;
    Vec3 bpf = b->m * vg - p_prim;

    double t = (qv + std::sqrt(qv*qv + v2*(r2-q2))) / v2; // positive time in the past

//    a->q += t * (a->p - apf) / a->m;
//    b->q += t * (b->p - bpf) / b->m;
    a->q = a->q - t * a->p/a->m;
    b->q = b->q - t * b->p/b->m;

    a->p = apf;
    b->p = bpf;
  }

  // interaction gravitationelle
  else if (q2 < (a->rg + b->rg)*(a->rg + b->rg)) {
    // F = ma*mb / r^2
    Vec3 F = a->m * b->m * q * pow(q2, -3.0/2.0);
    a->f -= F;
    b->f += F;
  }
}

void Particle::simple_interaction_wall(Particle* a, int k, double w, double cor)
{
  if (w > 0.0) {
    if (a->q[k] > w - a->r && a->p[k] > 0.0) {
      a->p[k] = -cor*a->p[k];
    }
  } else {
    if (a->q[k] < w + a->r && a->p[k] < 0.0) {
      a->p[k] = -cor*a->p[k];
    }
  }
}

void Particle::backtrack_interaction_wall(Particle* a, int k, double w, double cor)
{
  if (w > 0.0) {
    w -= a->r;
    if (a->q[k] > w && a->p[k] > 0.0) {
      //a->q[k] = w - cor * (a->q[k] - w);
      a->p[k] = -cor*a->p[k];
    }
  } else {
    w += a->r;
    if (a->q[k] < w && a->p[k] < 0.0) {
//      a->q[k] = w + cor * (w - a->q[k]);
      a->p[k] = -cor*a->p[k];
    }
  }
}
