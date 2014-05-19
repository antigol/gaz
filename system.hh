#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "particle.hh"
#include <QVector>

class System
{
public:
	System();

	void setSizes(double x, double y, double z);
	void initRandomParticles(int n, double speed);

	int evolve(double dt);

	QVector<Particle> _ps;
	double _contract;

private:
	void computeMaxR();

	double _dim[3];
	double _maxd;

};

#endif // SYSTEM_HH
