#include "system.hh"
#include "random.hh"
#include <QTime>
#include <QDebug>
#include <QMultiMap>
#include <map>
#include <unordered_map>

System::System()
	: algorithm(0)
{
}

void System::setSizes(double x, double y, double z)
{
	_dim[0] = x;
	_dim[1] = y;
	_dim[2] = z;
}

void System::evolve(double dt)
{
	// évolution
	for (Particle& z : ps)
		z.q += z.p / z.m * dt;

	// collision avec les murs
	for (Particle& z : ps) {
		for (int k = 0; k < 3; ++k) {
			if (z.q[k] < -_dim[k] + z.r && z.p[k] < 0.0) {
				z.p[k] = -z.p[k];
				break;
			}
			if (z.q[k] > _dim[k] - z.r && z.p[k] > 0.0) {
				z.p[k] = -z.p[k];
				break;
			}
		}
	}

	switch (algorithm) {
	case 0:
		multimap();
		break;
	case 1:
		x_sort();
		break;
	case 2:
		naive();
		break;
	}
}

void System::initialize()
{
	ptr_ps.clear();

	_maxd = 0.0;
	for (Particle& z : ps) {
		_maxd = qMax(_maxd, z.r);
		ptr_ps.push_back(&z);
	}
	_maxd *= 2.0001;
}

void System::naive()
{
	for (std::size_t i = 0; i < ptr_ps.size(); ++i) {
		for (std::size_t j = 0; j < ptr_ps.size(); ++j) {
			if (i == j) continue;
			Particle::collision(ptr_ps[i], ptr_ps[j]);
		}
	}
}

void System::x_sort()
{
	std::sort(ptr_ps.begin(), ptr_ps.end(), [] (Particle* a, Particle* b) {return a->q[0] < b->q[0];});

	for (std::size_t i = 0; i < ptr_ps.size(); ++i) {
		Particle *pi = ptr_ps[i];
		for (int j = i-1; j >= 0
			 && pi->q[0] - ptr_ps[j]->q[0] <= _maxd; --j)
			Particle::collision(pi, ptr_ps[j]);
		for (std::size_t j = i+1; j < ptr_ps.size()
			 && ptr_ps[j]->q[0] - pi->q[0] <= _maxd; ++j)
			Particle::collision(pi, ptr_ps[j]);
	}
}

void System::multimap()
{
	std::multimap<u_int64_t, Particle*> map;

	for (std::size_t i = 0; i < ptr_ps.size(); ++i) {
		Particle *ptr = ptr_ps[i];
		u_int64_t x = std::max(std::round((ptr->q[0]+_dim[0]) / _maxd), 0.0);
		u_int64_t y = std::max(std::round((ptr->q[1]+_dim[1]) / _maxd), 0.0);
		u_int64_t z = std::max(std::round((ptr->q[2]+_dim[2]) / _maxd), 0.0);
		map.emplace((z<<42) + (y<<21) + x, ptr);
	}
	for (auto ic = map.cbegin(); ic != map.cend(); ++ic) {
		u_int64_t key = ic->first;
		for (int z = 0; z <= 1; ++z) {
			for (int y = (z==0 ? 0 : -1); y <= 1; ++y) {
				u_int64_t keyBegin = key + z*(1ul<<42) + y*(1ul<<21) + (z==0 && y==0 ? 0 : -1);
				u_int64_t keyEnd   = key + z*(1ul<<42) + y*(1ul<<21) + 1;
				auto i = map.lower_bound(keyBegin);
				while (i != map.cend() && i->first <= keyEnd) {
					Particle::collision(ic->second, i->second);
					++i;
				}
			}
		}
	}
}
