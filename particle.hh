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

  static void simple_interaction(Particle *a, Particle *b, double cor);
  static void backtrack_interaction(Particle* a, Particle* b, double cor);

  static void backtrack_interaction_wall(Particle* a, int dim, double pos);
};

#endif // PARTICLE_HH
