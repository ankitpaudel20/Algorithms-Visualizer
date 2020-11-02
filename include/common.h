#pragma once

#include <iostream>
#include <cmath>
#include "SDL2/SDL_ttf.h"

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



struct fontinfo {
	std::string name;
	int size;

	bool operator>(const fontinfo& rhs)const {
		return std::string(name + (char)size) > std::string(rhs.name + (char)rhs.size);
	}
	bool operator<(const fontinfo& rhs)const {
		return std::string(name + (char)size) < std::string(rhs.name + (char)rhs.size);
	}
};


struct body
{
	vec2<int> size;
	vec2<float> pos, vel, targetVel;

	SDL_FRect getrect()
	{
		return SDL_FRect{ pos.x - (float)size.x, pos.y - (float)size.y, (float)size.x, (float)size.y };
	}

	void update(float deltatime)
	{
		vel.x = vel.x + (targetVel.x - vel.x) * deltatime;
		vel.y = vel.y + (targetVel.y - vel.y) * deltatime;
		// calculate final position

		pos.x += vel.x;
		pos.y += vel.y;
	}
};