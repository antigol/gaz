#ifndef __RANDOM_HH__
#define __RANDOM_HH__

#include <random>

namespace rdm {

// retourne un générateur de nombres aléatoires global
inline std::default_random_engine& global_random_engine()
{
	// crée un generateur de graines en statique
	static std::random_device rdev;
	// crée un générateur de nombres aléatoires en statique
	static std::default_random_engine eng(rdev()); 
	return eng;
}

// retourne un nombre entre 0 et 1 (non compris)
inline double canonical()
{
	return std::generate_canonical<double, 10>(global_random_engine()); // 10 bits of randomness
}

// retourne un nombre entier dans l'intervale fermé [a,b]
inline int uniformi(int a, int b)
{
	static std::uniform_int_distribution<> d{};
	using parm_t = decltype(d)::param_type;
	return d(global_random_engine(), parm_t{a, b});
}

// retourne un nombre entre a et b (non compris)
inline double uniformd(double a, double b)
{
	static std::uniform_real_distribution<> d{};
	using parm_t = decltype(d)::param_type;
	return d(global_random_engine(), parm_t{a, b});
}

// retourne un nombre distribué selon la loi normale centrée en u d'écart type s
inline double normal(double u, double s)
{
	static std::normal_distribution<> d{};
	using parm_t = decltype(d)::param_type;
	return d(global_random_engine(), parm_t{u, s});
}

}

#endif
