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
	void addParticule(double x, double y, double z, double vx, double vy, double vz, double mass = 1.0, double radius = 1.0);
	void setDimension(double wx, double wy, double wz);

private:
	QScriptEngine _eng;
	System* _system;
};

#endif // SCRIPTREADER_HH
