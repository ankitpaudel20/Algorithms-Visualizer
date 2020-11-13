#pragma once

#include <iostream>
#include <cmath>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL2_gfxPrimitives.h"

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


struct graphNode {
	vec2<float> pos;
	double H, G;
	vec2<float> size, origin;
	Uint32 color;
	//Uint8 property = 0x00;
	bool visited = false, isWall = false;


	graphNode* parent;

	void draw(SDL_Renderer* renderer, const bool& fill) {
		if (fill)
			boxColor(renderer, pos.x - origin.x, pos.y - origin.y, pos.x - origin.x + size.x, pos.y - origin.y + size.y, color);
		else
			rectangleColor(renderer, pos.x - origin.x, pos.y - origin.y, pos.x - origin.x + size.x, pos.y - origin.y + size.y, color);
	}

	void calculateHL(const graphNode* end) {
		G = parent->G + 1;
		H = vec2<float>::dist(pos, end->pos);
	}

	bool operator <(const graphNode& rhs) {
		return  (H + G) < (rhs.H + rhs.G);
	}
	bool operator >(const graphNode& rhs) {
		return  (H + G) > (rhs.H + rhs.G);
	}
};

enum class appState {
	Idle, Animating
};
