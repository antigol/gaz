#ifndef SYSTEM_HH
#define SYSTEM_HH

#include "particle.hh"
#include <vector>
#include <map>
#include <functional>

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
    QVector<double> pairCorelation(int nBins);

	std::vector<Particle> ps;
	double _dim[3];
	int algorithm;
	double _dt;
    Vec3 _g;
    double _cor;
	bool limited;

	void stop();
	void pause(bool on);

	void initialize();
private:
	void naive();
	void x_sort();
    void multimap(std::function<void(Particle*, Particle*)> f, double nDiameter);

	double _maxd;
	std::vector<Particle*> ptr_ps;
	bool _stop;
	bool _pause;
	QMutex _mutex;

protected:
	void run() override;
};

#endif // SYSTEM_HH
