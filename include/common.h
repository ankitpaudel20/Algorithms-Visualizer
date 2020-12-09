#pragma once

#include <iostream>
#include <cmath>
#include <random>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL2_gfxPrimitives.h"


inline float dist(const float& x1, const float& y1, const float& x2, const float& y2) {
	return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}



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

	inline static float dist(const vec2<T>& a, const vec2<T>& b) {
		return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
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

	bool operator==(const vec2& in) {
		return x == in.x && y == in.y;
	}
	bool operator!=(const vec2& in) {
		return x != in.x || y != in.y;
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


struct Circle
{
	vec2<float> pos, vel = 0, targetVel = 0;
	SDL_Rect texPos;
	uint32_t color;
	SDL_Texture* letter = nullptr;

	void update(const float& deltatime)
	{
		/*	vel.x = vel.x + (targetVel.x - vel.x) * deltatime;
			vel.y = vel.y + (targetVel.y - vel.y) * deltatime;*/
			// calculate final position

		pos.x += vel.x * deltatime;
		pos.y += vel.y * deltatime;
		texPos.x += vel.x;
		texPos.y += vel.y;
	}

	void texInit(SDL_Texture* tex) {
		letter = tex;
		SDL_QueryTexture(tex, nullptr, nullptr, &texPos.w, &texPos.h);
		texPos.x = pos.x - texPos.w / 2;
		texPos.y = pos.y - texPos.h / 2;
	}

};


struct gridGraphNode {
	vec2<float> pos;
	double F = std::numeric_limits<double>::infinity(), H = std::numeric_limits<double>::infinity(), G = std::numeric_limits<double>::infinity();
	vec2<float> size, origin;
	Uint32 color;
	//Uint8 property = 0x00;
	bool visited = false, isWall = false;


	gridGraphNode* parent = nullptr;

	void draw(SDL_Renderer* renderer, const bool& fill) {
		if (fill)
			boxColor(renderer, pos.x - origin.x, pos.y - origin.y, pos.x - origin.x + size.x, pos.y - origin.y + size.y, color);
		else
			rectangleColor(renderer, pos.x - origin.x, pos.y - origin.y, pos.x - origin.x + size.x, pos.y - origin.y + size.y, color);
	}

	void calculateHL(const gridGraphNode* end, const double& weight) {
		G = parent->G + weight;
		H = vec2<float>::dist(pos, end->pos);
		F = G + H;
	}

	void updateHL(const gridGraphNode* end, gridGraphNode* _parent, const double& weight) {
		double _G = _parent->G + weight;
		H = vec2<float>::dist(pos, end->pos);
		if (G == std::numeric_limits<float>::infinity())
		{
			G = _G;
		}
		else if (_G < G)
		{
			parent = _parent;
			G = _G;
		}
		F = G + H;
	}

	bool operator <(const gridGraphNode& rhs) {
		return  (H + G) < (rhs.H + rhs.G);
	}
	bool operator >(const gridGraphNode& rhs) {
		return  (H + G) > (rhs.H + rhs.G);
	}
};

enum class appState {
	Idle, Animating
};