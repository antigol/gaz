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

	bool runFile(System* s, const QString file);
	int run(System* s, const QString code);

	QString error;

public slots:
	void print(const QString& msg);
	void position(double x, double y, double z);
	void momentum(double x, double y, double z);
	void mass(double m);
	void radius(double r);
	void gradius(double rg);
	void color(double r, double g, double b);
	void colorHSV(double h, double v, double s);
	void addParticle();
	void dimension(double wx, double wy, double wz);
	double rand(double a, double b) const;
	double randn(double mu, double sigma) const;
	void g(double gx, double gy, double gz);
	void cor(double _cor);

private:
	QScriptEngine _eng;

	System* _system;

	Particle _p;
};

#endif // SCRIPTREADER_HH
