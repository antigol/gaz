#include "scriptreader.hh"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <iostream>

ScriptReader::ScriptReader(QObject *parent) :
	QObject(parent)
{
	QScriptValue v = _eng.newQObject(this, QScriptEngine::QtOwnership,
									 QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeChildObjects);
	_eng.globalObject().setProperty("MAIN", v);
}

bool ScriptReader::run(System *s, const QString fileName)
{
	QFile scriptFile(fileName);
	if (!scriptFile.open(QIODevice::ReadOnly))
		return false;

	QTextStream stream(&scriptFile);
	QString contents = stream.readAll();
	scriptFile.close();

	_system = s;

	_eng.evaluate(contents, fileName);

	if (_eng.hasUncaughtException()) {
		QTextStream err(stderr);
		err << _eng.uncaughtExceptionLineNumber() << ": "
			<< _eng.uncaughtException().toString() << endl;
		return false;
	}

	_system->initialize();
	return true;
}

void ScriptReader::print(const QString &msg)
{
	qDebug() << msg;
}

void ScriptReader::addParticule(double x, double y, double z, double vx, double vy, double vz, double mass, double radius)
{
	Particle p;
	p.m = mass;
	p.r = radius;
	p.q = Vec3(x, y, z);
	p.p = Vec3(vx, vy, vz) * mass;

	_system->_ps.append(p);
}

void ScriptReader::setDimension(double wx, double wy, double wz)
{
	_system->setSizes(wx, wy, wz);
}
