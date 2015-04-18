#ifndef VEC3_HH
#define VEC3_HH

class Vec3
{
public:
  Vec3();
  Vec3(double x, double y, double z);

  double &operator[](int i);
  const double &operator[](int i) const;

  Vec3 &operator+=(const Vec3& a);
  Vec3 &operator-=(const Vec3& a);
  Vec3 &operator*=(double a);
  Vec3 &operator/=(double a);
  const Vec3 operator-() const;

  void setNull();
  bool isNull() const;

  static double dot(const Vec3& a, const Vec3& b);
  static double norm(const Vec3& a);
  static double length(const Vec3& a);
  static Vec3 random(double L = 1.0);
private:
  double _d[3];
};

inline const Vec3 operator+(Vec3 a, const Vec3& b) { return a += b; }
inline const Vec3 operator-(Vec3 a, const Vec3& b) { return a -= b; }
inline const Vec3 operator*(Vec3 a, double b) { return a *= b; }
inline const Vec3 operator*(double a, Vec3 b) { return b *= a; }
inline const Vec3 operator/(Vec3 a, double b) { return a /= b; }

#endif // VEC3_HH
