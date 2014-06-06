#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "particle.hh"
#include <vector>
#include <map>
#include <QThread>
#include <QMutex>
#include <QTime>

class System : public QThread
{
public:
	System();
	~System();

	void setSizes(double x, double y, double z);

	void evolve(double dt);

	std::vector<Particle> ps;
	double _dim[3];
	int algorithm;
	double _dt;
	bool limited;

	void stop();
	void pause(bool on);

	void initialize();
private:
	void naive();
	void x_sort();
	void multimap();

	double _maxd;
	std::vector<Particle*> ptr_ps;
	bool _stop;
	bool _pause;
	QMutex _mutex;
	QTime time;

protected:
	void run() override;
};

#endif // SYSTEM_HH
