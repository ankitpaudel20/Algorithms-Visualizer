#pragma once

#include <future>
#include <random>
#include "window.h"


template <class T>
std::vector<T> random_generator(T start, T end, uint32_t count)
{
	std::vector<T> out(count);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<T> dist(start, end);
	for (unsigned i = 0; i < count; i++)
		out[i] = dist(mt);

	return out;
}

template<class T>
struct sortingClass {

	float sleeptime = 0.05;
	int width;
	float spacing;
	int N = 10;

	std::vector<uint32_t> m_data;
	bool running = false;
	bool ready = false;
	bool sorted = false;

	Window* window;

	std::future<void> m_future;




	void stop() {
		m_future.get();
	}

	void randomize(const SDL_Rect& viewport)
	{
		m_data = random_generator<uint32_t>(0, viewport.h, N);
		spacing = (float)viewport.w / ((N - 1) * 2);
		width = viewport.w / (N - 1) * 0.95;
		ready = true;
	}

	void Draw() {
		SDL_RenderSetViewport(window->gRenderer, &window->viewport);
		SDL_SetRenderDrawColor(window->gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		for (int i = 0; i < m_data.size(); i++)
			drawRect((2 * i + 1) * spacing, m_data[i]);
		SDL_Rect rect{ width, 0, window->viewport.w - 2 * width, window->viewport.h };
	}

	void drawRect(const float& pos, const uint32_t& height)
	{
		SDL_FRect a{ pos - width / 2, window->viewport.h - height, width, height };
		SDL_RenderFillRectF(window->gRenderer, &a);

	}

	template <class T>
	bool cmp(const uint32_t& left, const uint32_t& right) {
		uint32_t r, g, b, a;
		SDL_GetRenderDrawColor(window->gRenderer, &r, &g, &b, &a);
		SDL_RenderSetViewport(window->gRenderer, &window->viewport);
		SDL_SetRenderDrawColor(window->gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		drawRectoutline(2 * left + 1 * spacing, m_data[left]);
		drawRectoutline(2 * right + 1 * spacing, m_data[right]);
		SDL_SetRenderDrawColor(window->gRenderer, r, g, b, a);
		SDL_Delay(sleeptime * 1000);
		return m_data[left] > m_data[right];

	}

	void drawRectoutline(const float& pos, const uint32_t& height)
	{
		SDL_FRect a{ pos - sortWidth / 2, viewport.h - height, sortWidth, height };
		SDL_RenderDrawRectF(gRenderer, &a);
	}

	template <class T>
	inline void swap(const size_t& left, const size_t& right)
	{

		uint32_t r, g, b, a;
		SDL_GetRenderDrawColor(window->gRenderer, &r, &g, &b, &a);
		SDL_RenderSetViewport(window->gRenderer, &window->viewport);
		SDL_SetRenderDrawColor(window->gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		drawRectoutline(2 * left + 1 * spacing, m_data[left]);
		drawRectoutline(2 * right + 1 * spacing, m_data[right]);
		SDL_SetRenderDrawColor(window->gRenderer, r, g, b, a);
		SDL_Delay(sleeptime * 500);
		T temp = m_data[left];
		m_data[left] = m_data[right];
		m_data[right] = temp;
		SDL_Delay(sleeptime * 500);
	}


	void run(int selection, Window* gwindow) {
		running = true;
		window = gwindow;

		switch (selection)
		{
		case 0:
			m_future = std::async(std::launch::async, bubblesort);

		default:
			break;
		}
	}


	const T operator[](const size_t& index) {
		return m_data[index];
	}

	void merge(T* arr, const size_t& a, const  size_t& m, const size_t& b);
	void sortSubroutine(T* arr, const size_t& a, const size_t& b);
	void mergesort();
	void qsort(T* array, const size_t& start_index, const size_t& end_index);
	void quicksort();
	void bubblesort();
	void radixsort();
	void insertionsort();
	void selectionsort();

};