///
#pragma once


#include "emb_def.h"
#include "emb_algorithm.h"

extern "C" { 
#include "arm_math.h"
}
#include "limits.h"
#include "float.h"


namespace emb {


inline constexpr float pi = PI;
inline constexpr float pi_over_2 = pi / 2;
inline constexpr float pi_over_4 = pi / 4;
inline constexpr float pi_over_3 = pi / 3;
inline constexpr float pi_over_6 = pi / 6;
inline constexpr float two_pi = 2 * pi;

inline constexpr float sqrt_2 = sqrt(2.f);
inline constexpr float sqrt_3 = sqrt(3.f);


/**
 * @brief
 */
template <typename T>
constexpr int sgn(T value) { return (value > T(0)) - (value < T(0)); }


/**
 * @brief
 */
constexpr float to_rad(float deg) { return pi * deg / 180; }


/**
 * @brief
 */
constexpr float to_deg(float rad) { return 180 * rad / pi; }


/**
 * @brief
 */
constexpr float normalize_2pi(float value)
{
	value = fmod(value, two_pi);
	if (value < 0)
	{
		value += two_pi;
	}
	return value;
}


/**
 * @brief
 */
constexpr float normalize_pi(float value)
{
	value = fmod(value + pi, two_pi);
	if (value < 0)
	{
		value += two_pi;
	}
	return value - pi;
}


/**
 * @brief
 */
template <typename T>
class Range
{
private:
	T lo_;
	T hi_;
public:
	Range(const T& val1, const T& val2)
	{
		if (val1 < val2)
		{
			lo_ = val1;
			hi_ = val2;
		}
		else
		{
			lo_ = val2;
			hi_ = val1;
		}
	}

	constexpr bool contains(const T& val) const { return (lo_ <= val) && (val <= hi_); }

	constexpr const T& lo() const { return lo_; }
	void setLo(const T& value)
	{
		if (value <= hi_)
		{
			lo_ = value;
		}
	}

	constexpr const T& hi() const { return hi_; }
	void setHi(const T& value)
	{
		if (value >= lo_)
		{
			hi_ = value;
		}
	}
};


/**
 * @brief
 */
template <typename T, typename Time>
class Integrator
{
private:
	T m_sum;
	Time m_dt;
	T m_init;
public:
	Range<T> range;

	Integrator(const Range<T>& _range, const Time& _dt, const T& _init)
		: range(_range)
		, m_dt(_dt)
		, m_init(_init)
	{
		reset();
	}

	void integrate(const T& value)
	{
		m_sum = clamp(m_sum + value * m_dt, range.lo(), range.hi());
	}

	void add(const T& value)
	{
		m_sum = clamp(m_sum + value, range.lo(), range.hi());
	}

	const T& value() const { return m_sum; }
	void reset()
	{
		m_sum = clamp(m_init, range.lo(), range.hi());
	}
};


} // namespace emb


