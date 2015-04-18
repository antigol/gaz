#include "system.hh"
#include "random.hh"
#include <QTime>
#include <QDebug>
#include <QMultiMap>
#include <QQueue>
#include <map>
#include <unordered_map>
#include <iostream>
#include <algorithm>

System::System()
  : algorithm(0)
  , limited(false)
{
  _dt = 10e-3;
  _g = Vec3(0,0,0);
  _cor = 1;
}

System::~System()
{
  stop();
  wait();
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
  for (Particle& z : ps) {
    z.q += z.p / z.m * dt;

    // F = m*a <=> F = d(p)/dt <=> F*dt = d(p)
    z.p += (z.f + _g * z.m) * dt;
    z.f.setNull();
  }

  // collision avec les murs (rajouter la pensanteur)
  for (Particle& z : ps) {
    for (int k = 0; k < 3; ++k) {
      if (z.q[k] < -_dim[k] + z.r && z.p[k] < 0.0) {
        z.p[k] = -_cor*z.p[k];
      }
      if (z.q[k] > _dim[k] - z.r && z.p[k] > 0.0) {
        z.p[k] = -_cor*z.p[k];
      }
    }
  }

  // collision avec les particules
  switch (algorithm) {
  case 0:
    multimap(std::bind(Particle::collision, std::placeholders::_1, std::placeholders::_2, _cor), 1.001);
    break;
  case 1:
    x_sort();
    break;
  case 2:
    naive();
    break;
  }

  emit steped();
}

QVector<double> System::pairCorelation(int nBins, double nDiameter)
{
  QVector<double> distances;
  auto f = [&](Particle* p1, Particle* p2) {
    distances.push_back(Vec3::length(p1->q - p2->q));
  };

  multimap(f, nDiameter);
  qSort(distances);

  if (distances.isEmpty()) return QVector<double>();

  double step = distances.last() / nBins;
  QVector<double> result(nBins);
  for (int i = 0; i < nBins; ++i) {
    // ]a,b]
    result[i] = std::distance(std::upper_bound(distances.begin(), distances.end(), i * step),
                              std::upper_bound(distances.begin(), distances.end(), i * step + step));
    result[i] /= std::pow(i * step + step, 3.0) - std::pow(i * step, 3.0);
  }

  return result;
}

void System::stop()
{
  _stop = true;
}

void System::pause(bool on)
{
  if (on && !_pause)
    _mutex.lock();
  if (!on && _pause)
    _mutex.unlock();

  _pause = on;
}

void System::initialize()
{
  ptr_ps.clear();

  _maxd = 0.0;
  for (Particle& z : ps) {
    _maxd = qMax(_maxd, qMax(z.r, z.rg));
    ptr_ps.push_back(&z);
  }
  _maxd *= 2.0;
}

void System::reverse()
{
  for(auto& part : ps){
    part.p = -part.p;
  }
}

void System::naive()
{
  for (std::size_t i = 0; i < ptr_ps.size(); ++i) {
    for (std::size_t j = i+1; j < ptr_ps.size(); ++j) {
      Particle::collision(ptr_ps[i], ptr_ps[j], _cor);
    }
  }
}

void System::x_sort()
{
  std::sort(ptr_ps.begin(), ptr_ps.end(), [] (Particle* a, Particle* b) {return a->q[0] < b->q[0];});

  for (std::size_t i = 0; i < ptr_ps.size(); ++i) {
    for (std::size_t j = i+1; j < ptr_ps.size()
         && ptr_ps[j]->q[0] - ptr_ps[i]->q[0] <= _maxd; ++j)
      Particle::collision(ptr_ps[i], ptr_ps[j], _cor);
  }
}

void System::multimap(std::function<void(Particle*,Particle*)> f, double nDiameter)
{
  std::multimap<u_int64_t, Particle*> map;

  double divisor = nDiameter * _maxd;

  // O(n)
  for (std::size_t i = 0; i < ptr_ps.size(); ++i) {
    Particle *ptr = ptr_ps[i];
    u_int64_t x = std::max(std::round((ptr->q[0]+_dim[0]) / divisor), 0.0);
    u_int64_t y = std::max(std::round((ptr->q[1]+_dim[1]) / divisor), 0.0);
    u_int64_t z = std::max(std::round((ptr->q[2]+_dim[2]) / divisor), 0.0);
    map.emplace((z<<42) + (y<<21) + x, ptr); // O(1)
  }

  for (auto ic = map.cbegin(); ic != map.cend();) {
    u_int64_t key = ic->first;
    for (int z = 0; z <= 1; ++z) {
      for (int y = (z==0 ? 0 : -1); y <= 1; ++y) {
        u_int64_t keyBegin = key + z*(1ul<<42) + y*(1ul<<21) + (z==0 && y==0 ? 0 : -1);
        u_int64_t keyEnd   = key + z*(1ul<<42) + y*(1ul<<21) + 1;
        auto i = map.lower_bound(keyBegin);
        while (i != map.cend() && i->first <= keyEnd) {
          f(ic->second, i->second);
          ++i;
        }
      }
    }
    ic = map.erase(ic);
  }
}

void System::run()
{
  QTime time, stats_time;
  time.start();
  stats_time.start();
  _stop = false;
  _pause = false;

  QQueue<double> stats;
  double stats_sum = 0.0;
  int stats_n = 50;

  while (!_stop) {
    while (!_mutex.tryLock())
      msleep(1);
    double dt = double(time.restart())/1000.0;
    stats_sum += dt;
    stats.enqueue(dt);
    limited = dt > _dt && (dt = _dt);
    evolve(dt);
    _mutex.unlock();
    usleep(50);

    while (stats.size() > stats_n)
      stats_sum -= stats.dequeue();

    if (stats_time.elapsed() > 1000) {
      stats_time.restart();
      using namespace std;
      cout << "Time for collision check & interaction : "
           << stats_sum / stats_n * 1000.0
           << " ms (mean on " << stats_n << " values)"
           << endl;
    }
  }
}
