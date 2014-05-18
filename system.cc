#include "system.hh"
#include "random.hh"
#include <QTime>
#include <QDebug>
#include <QMultiMap>

System::System()
{
}

void System::setSizes(double x, double y, double z)
{
	_dim[0] = x;
	_dim[1] = y;
	_dim[2] = z;
}

void System::initRandomParticles(int n, double speed)
{
	_ps.clear();
	_ps.reserve(n);
	for (int i = 0; i < n; ++i) {
		Particle p;
		p.q[0] = rdm::uniformd(-_dim[0], _dim[0]);
		p.q[1] = rdm::uniformd(-_dim[1], _dim[1]);
		p.q[2] = rdm::uniformd(-_dim[2], _dim[2]);

		p.r = rdm::uniformd(1.2, 3.0);
		p.p = Vec3::random(rdm::uniformd(0.0, speed)) * p.m;

//		p.color[0] = p.color[1] = double(i) / double(n);
		p.color[0] = p.color[1] = (p.q[0] + _dim[0]) / (2.0 * _dim[0]);
//		p.q[1] = i * 3.0;
//		p.p[1] = 10.0 * p.m;

		_ps.append(p);
	}
	computeMaxR();
}

int System::evolve(double dt)
{
	int count = 0;

	for (Particle& z : _ps)
		z.q += z.p / z.m * dt;

	// collision avec les murs
	for (Particle& z : _ps) {
		for (int k = 0; k < 3; ++k) {
			if (z.q[k] < -_dim[k] + z.r && z.p[k] < 0.0)
				z.p[k] = -z.p[k];
			if (z.q[k] > _dim[k] - z.r && z.p[k] > 0.0)
				z.p[k] = -z.p[k];
		}
	}

	// collisions entre les particules
#define BOX

#ifdef NAIVE
	for (int i = 0; i < _ps.size(); ++i) {
		for (int j = 0; j < _ps.size(); ++j) {
			if (i == j) continue;
			count += Particle::collision(_ps[i], _ps[j]);
		}
	}
#endif

#ifdef XSORT
	qSort(_ps.begin(), _ps.end(), [] (const Particle& a, const Particle& b) {return a.q[0] < b.q[0];});

	for (int i = 0; i < _ps.size(); ++i) {
		Particle &pi = _ps[i];
		for (int j = i-1; j >= 0
			 && pi.q[0] - _ps[j].q[0] <= 2.0*_maxr; --j)
			count += Particle::collision(pi, _ps[j]);
		for (int j = i+1; j < _ps.size()
			 && _ps[j].q[0] - pi.q[0] <= 2.0*_maxr; ++j)
			count += Particle::collision(pi, _ps[j]);
	}
#endif

#ifdef BOX
	QMultiMap<quint64, Particle*> cases;
	for (int i = 0; i < _ps.size(); ++i) {
		quint64 x = (_ps[i].q[0]+_dim[0]) / (2.0*_maxr);
		quint64 y = (_ps[i].q[1]+_dim[1]) / (2.0*_maxr);
		quint64 z = (_ps[i].q[2]+_dim[2]) / (2.0*_maxr);
		cases.insert(z * (1ul<<40) + y * (1ul<<20) + x, &_ps[i]);
	}

	for (QMultiMap<quint64, Particle*>::const_iterator ic = cases.begin(); ic != cases.end(); ++ic) {
		for (int z = 0; z <= 1; ++z) {
			for (int y = (z==0 ? 0 : -1); y <= 1; ++y) {
				int x = (z==0 && y==0 ? 0 : -1);
				quint64 key = ic.key() + z * (1ul<<40) + y * (1ul<<20) + x;

				for (auto it = cases.find(key); it != cases.end() && it.key() <= key+1; ++it) {
					count += Particle::collision(*it.value(), *ic.value());
				}
			}
		}
	}
#endif

	return count;
}

void System::computeMaxR()
{
	_maxr = 0.0;
	for (Particle& z : _ps)
		_maxr = qMax(_maxr, z.r);
}

// pour 5000
// XSORT 100ms
// NAIVE 1885ms
// BOX 12ms
