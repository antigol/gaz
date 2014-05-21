#ifndef SCRIPTREADER_HH
#define SCRIPTREADER_HH

#include <QObject>
#include <QScriptEngine>
#include "system.hh"

class ScriptReader : public QObject
{
	Q_OBJECT
public:
	explicit ScriptReader(QObject *parent = 0);

	bool run(System* s, const QString file);

public slots:
	void print(const QString& msg);
	void position(double x, double y, double z);
	void momentum(double x, double y, double z);
	void mass(double m);
	void radius(double r);
	void color(double r, double g, double b);
	void addParticule();
	void dimension(double wx, double wy, double wz);
	double rand(double a, double b) const;
	double randn(double mu, double sigma) const;

private:
	QScriptEngine _eng;
	System* _system;

	Particle _p;
};

#endif // SCRIPTREADER_HH
