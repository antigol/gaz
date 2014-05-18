#include "vec3.hh"
#include "random.hh"

Vec3::Vec3()
{
	_d[0] = 0.0;
	_d[1] = 0.0;
	_d[2] = 0.0;
}

Vec3::Vec3(double x, double y, double z)
{
	_d[0] = x;
	_d[1] = y;
	_d[2] = z;
}

double &Vec3::operator[](int i)
{
	return _d[i];
}

const double &Vec3::operator[](int i) const
{
	return _d[i];
}

Vec3 &Vec3::operator+=(const Vec3 &a)
{
	_d[0] += a[0];
	_d[1] += a[1];
	_d[2] += a[2];
	return *this;
}

Vec3 &Vec3::operator-=(const Vec3 &a)
{
	_d[0] -= a[0];
	_d[1] -= a[1];
	_d[2] -= a[2];
	return *this;
}

Vec3 &Vec3::operator*=(double a)
{
	_d[0] *= a;
	_d[1] *= a;
	_d[2] *= a;
	return *this;
}

Vec3 &Vec3::operator/=(double a)
{
	_d[0] /= a;
	_d[1] /= a;
	_d[2] /= a;
	return *this;
}

double Vec3::dot(const Vec3 &a, const Vec3 &b)
{
	return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

double Vec3::norm(const Vec3 &a)
{
	return a[0]*a[0] + a[1]*a[1] + a[2]*a[2];
}

Vec3 Vec3::random(double L)
{
	double z = rdm::uniformd(-L, L);
	double psi = rdm::uniformd(0.0, 2*M_PI);
	double r = sqrt(L*L - z*z);
	return Vec3(r * cos(psi), r * sin(psi), z);
}
