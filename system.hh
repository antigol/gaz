#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "particle.hh"
#include <QVector>
#include <map>

class System
{
public:
	System();

	void setSizes(double x, double y, double z);
	void initRandomParticles(int n, double speed);

	void evolve(double dt);

	QVector<Particle> _ps;
	double _dim[3];

	void initialize();
private:

	double _maxd;

	std::multimap<u_int64_t, Particle*> _map;
};

#endif // SYSTEM_HH
