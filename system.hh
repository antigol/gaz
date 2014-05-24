#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "particle.hh"
#include <vector>
#include <map>

class System
{
public:
	System();

	void setSizes(double x, double y, double z);

	void evolve(double dt);

	std::vector<Particle> ps;
	double _dim[3];
	int algorithm;

	void initialize();
private:
	void naive();
	void x_sort();
	void multimap();

	double _maxd;
	std::vector<Particle*> ptr_ps;
};

#endif // SYSTEM_HH
