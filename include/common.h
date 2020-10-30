#pragma once

#include <iostream>
#include <cmath>

template <class T>
struct vec2
{
	T x, y;

	vec2() {}
	vec2(T a) : x(a), y(a) {}
	vec2(T a, T b) : x(a), y(b) {}

	float mag()
	{
		return sqrtf(pow(x, 2) + pow(y, 2));
	}

	friend vec2 operator+(const vec2& a, const vec2& b)
	{
		return vec2(a.x + b.x, a.y + b.y);
	}

	friend vec2 operator+(vec2 in, T a)
	{
		return vec2(in.x + a, in.y + a);
	}

	friend vec2 operator-(const vec2& a, const vec2& b)
	{
		return vec2(a.x - b.x, a.y - b.y);
	}

	friend vec2 operator-(vec2 in, T a)
	{
		return vec2(in.x - a, in.y - a);
	}

	friend vec2 operator*(const vec2& a, const vec2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	friend vec2 operator*(vec2 in, T a)
	{
		return vec2(in.x * a, in.y * a);
	}

	friend vec2 operator/(const vec2& a, const vec2& b)
	{
		return vec2(a.x / b.x, a.y / b.y);
	}

	friend vec2 operator/(vec2 in, T a)
	{
		return vec2(in.x / a, in.y / a);
	}

	vec2 operator-()
	{
		return vec2(-x, -y);
	}

	vec2 operator+=(vec2 in)
	{
		x += in.x;
		y += in.y;
		return *this;
	}

	vec2 operator-=(vec2 in)
	{
		x -= in.x;
		y -= in.y;
		return *this;
	}

	vec2 operator*=(vec2 in)
	{
		x *= in.x;
		y *= in.y;
		return *this;
	}

	vec2 operator/=(vec2 in)
	{
		x /= in.x;
		y /= in.y;
		return *this;
	}

	friend std::ostream& operator<<(std::ostream& out, const vec2& vec)
	{
		out << "( " << vec.x << ", " << vec.y << " )\n";
		return out;
	}
};

struct Circle
{

	float* radius;
	vec2<uint32_t> pos;
	uint32_t color = 0x00ffffff;
	size_t next = std::numeric_limits<size_t>::max();
	size_t previous = std::numeric_limits<size_t>::max();

	Circle(float& _radius, const vec2<uint32_t>& _pos) : pos(_pos)
	{
		radius = &_radius;
	}
	Circle() {};
};