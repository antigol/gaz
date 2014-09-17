#include "scriptreader.hh"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QScriptValueIterator>
#include <iostream>
#include "random.hh"

ScriptReader::ScriptReader(QObject *parent) :
  QObject(parent)
{
  QScriptValue v = _eng.newQObject(this, QScriptEngine::QtOwnership,
                                   QScriptEngine::ExcludeSuperClassContents | QScriptEngine::ExcludeChildObjects);

  _eng.globalObject().setProperty("App", v);
  _eng.setProcessEventsInterval(100);
}

bool ScriptReader::runFile(System *s, const QString fileName)
{
  QTextStream err(stderr);

  QFile scriptFile(fileName);
  if (!scriptFile.open(QIODevice::ReadOnly)) {
    err << "Error : open file" << endl;
    return false;
  }

  QTextStream stream(&scriptFile);
  QString contents = stream.readAll();
  scriptFile.close();

  _eng.abortEvaluation();

  _system = s;
  _p = Particle();
  dimension(20, 20, 20);
  g(0,0,0);
  cor(1);

  _eng.evaluate(contents, fileName);

  if (_eng.hasUncaughtException()) {
    err << _eng.uncaughtExceptionLineNumber() << ": "
        << _eng.uncaughtException().toString() << endl;
    return false;
  }

  _system->initialize();
  return true;
}

int ScriptReader::run(System *s, const QString code)
{
  _eng.abortEvaluation();

  _system = s;
  _p = Particle();
  dimension(20, 20, 20);
  g(0,0,0);
  cor(1);

  _eng.evaluate(code);

  if (_eng.hasUncaughtException()) {
    error = _eng.uncaughtException().toString();
    return _eng.uncaughtExceptionLineNumber();
  }

  _system->initialize();
  return 0;
}

void ScriptReader::print(const QString &msg)
{
  QTextStream(stdout) << msg << endl;
}

void ScriptReader::position(double x, double y, double z)
{
  _p.q = Vec3(x, y, z);
}

void ScriptReader::momentum(double x, double y, double z)
{
  _p.p = Vec3(x, y, z);
}

void ScriptReader::mass(double m)
{
  _p.m = m;
}

void ScriptReader::radius(double r)
{
  _p.r = r;
}

void ScriptReader::gradius(double rg)
{
  _p.rg = rg;
}

void ScriptReader::color(double r, double g, double b)
{
  _p.color = Vec3(r, g, b);
}

void ScriptReader::addParticle()
{
  _system->ps.push_back(_p);
}

void ScriptReader::dimension(double x, double y, double z)
{
  _system->setSizes(x, y, z);
}

double ScriptReader::rand(double a, double b) const
{
  return rdm::uniformd(a, b);
}

double ScriptReader::randn(double mu, double sigma) const
{
  return rdm::normal(mu, sigma);
}

void ScriptReader::g(double gx, double gy, double gz)
{
  _system->_g = Vec3(gx, gy, gz);
}

void ScriptReader::cor(double _cor)
{
  _system->_cor = _cor;
}
