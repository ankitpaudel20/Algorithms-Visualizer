#pragma once

#include <random>
#include <thread>
#include <vector>
#include <array>
#include <mutex>
#include <cstring>

#include "window.h"
#include "states.h"
#include "glad/glad.h"

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

struct sortingClass
{
	std::vector<int> swapPos;
	std::vector<int> compPos;
	std::vector<int> copyPos;

	std::mutex swapMutex;
	std::mutex compMutex;
	std::mutex copyMutex;

	float sleeptime = 0.05;
	int width;
	float spacing;
	int N = 10;

	std::vector<uint32_t> m_data;
	bool sorted = false;

	SDL_Rect *viewport;

	std::thread *thread = nullptr;

	sortingClass()
	{
		swapPos.reserve(2);
		compPos.reserve(2);
		copyPos.reserve(2);
	}

	void randomize(const SDL_Rect &viewport)
	{
		m_data.clear();
		m_data.reserve(N);
		for (uint32_t i = 0; i < N; i++)
			m_data.push_back(viewport.h / (N - 1) * (i + 1));

		std::random_device rd;
		std::mt19937 mt(rd());
		std::uniform_int_distribution<uint32_t> dist(0, N - 1);
		for (size_t i = N - 1; i > 0; i--)
			swap2(dist(mt) % i, i);

		// m_data = random_generator<uint32_t>(0, viewport.h, N);

		spacing = (float)viewport.w / ((N)*2);

		if (width > viewport.w / N * 0.95)
			width = viewport.w / N * 0.95;
	}

	void Draw(appState &state, SDL_Renderer *renderer)
	{
		SDL_RenderSetViewport(renderer, viewport);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

		for (int i = 0; i < m_data.size(); i++)
		{
			SDL_FRect a{(2 * i + 1) * spacing - width / 2, viewport->h - m_data[i], width, m_data[i]};
			SDL_RenderFillRectF(renderer, &a);
		}

		if (thread != nullptr)
		{
			if (!sorted)
			{
				SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0xFF, 0xFF);
				copyMutex.lock();
				glLineWidth(5.0f);
				if (!copyPos.empty())
				{
					for (int i = copyPos[0], j = 0; j <= copyPos[1]; i++, j++)
					{
						SDL_FRect a{(2 * i + 1) * spacing - width / 2, viewport->h - m_data[i], width, m_data[i]};
						SDL_RenderDrawRectF(renderer, &a);
					}
				}
				copyMutex.unlock();

				SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
				swapMutex.lock();
				for (auto i : swapPos)
				{
					SDL_FRect a{(2 * i + 1) * spacing - width / 2, viewport->h - m_data[i], width, m_data[i]};
					SDL_RenderFillRectF(renderer, &a);
				}
				swapMutex.unlock();

				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
				compMutex.lock();
				// glLineWidth(5.0f);
				for (auto i : compPos)
				{
					SDL_FRect a{(2 * i + 1) * spacing - width / 2, viewport->h - m_data[i], width, m_data[i]};
					SDL_RenderFillRectF(renderer, &a);
				}
				compMutex.unlock();
			}
			else
			{
				if (thread->joinable())
					thread->join();
				delete thread;
				thread = nullptr;
				state = appState::Idle;
				sorted = false;
				swapPos.clear();
				copyPos.clear();
				compPos.clear();
			}
		}
		//SDL_Rect rect{ width, 0, viewport.w - 2 * width, viewport.h };
	}

	inline bool cmp(const size_t &left, const size_t &right)
	{
		compPos.emplace_back(left);
		compPos.emplace_back(right);
		int a = sleeptime * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));
		//SDL_Delay(a);
		compMutex.lock();
		compPos.clear();
		compMutex.unlock();
		return m_data[left] > m_data[right];
	}

	inline bool cmp(const size_t &left, const uint32_t &ld, const size_t &right, const uint32_t &rd)
	{
		compPos.emplace_back(left);
		compPos.emplace_back(right);
		int a = sleeptime * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));
		//SDL_Delay(a);
		compMutex.lock();
		compPos.clear();
		compMutex.unlock();
		return ld > rd;
	}

	void swap(const size_t &left, const size_t &right)
	{
		swapPos.emplace_back(left);
		swapPos.emplace_back(right);
		int a = sleeptime * 1000 * 1.25;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));
		//SDL_Delay(a);
		uint32_t temp = m_data[left];
		m_data[left] = m_data[right];
		m_data[right] = temp;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));
		//SDL_Delay(a);

		swapMutex.lock();
		swapPos.clear();
		swapMutex.unlock();
	}

	void swap2(const size_t &left, const size_t &right)
	{
		uint32_t temp = m_data[left];
		m_data[left] = m_data[right];
		m_data[right] = temp;
	}

	inline void memcopy(const uint32_t &to, const uint32_t &from, const uint32_t &number)
	{
		auto *arr = &m_data[0];
		copyPos.emplace_back(from);
		copyPos.emplace_back(number);
		int a = sleeptime * 1000 * 3;
		//std::this_thread::sleep_for(std::chrono::milliseconds(a));
		SDL_Delay(a);
		memcpy(arr + to, arr + from, number * sizeof(uint32_t));
		copyPos[0] = to;
		//std::this_thread::sleep_for(std::chrono::milliseconds(a));
		SDL_Delay(a);

		copyMutex.lock();
		copyPos.clear();
		copyMutex.unlock();
	}

	void run(int selection)
	{

		switch (selection)
		{
		case 0:
			thread = new std::thread(&sortingClass::selectionsort, this);
			break;
		case 1:
			thread = new std::thread(&sortingClass::quicksort, this);
			break;
		case 2:
			thread = new std::thread(&sortingClass::mergesort, this);
			break;
		case 3:
			thread = new std::thread(&sortingClass::insertionsort, this);
			break;
		case 4:
			thread = new std::thread(&sortingClass::bubblesort, this);
			break;
		case 5:
			thread = new std::thread(&sortingClass::shellsort, this);
			// shellsort();
			break;
		case 6:
			thread = new std::thread(&sortingClass::shellsort2, this);
			// shellsort();
			break;
		default:
			break;
		}
	}

	void merge(uint32_t *arr, const size_t &a, const size_t &m, const size_t &b);
	void mergesortSubroutine(const size_t &a, const size_t &b);
	void mergesort();
	void qsort(const size_t &start_index, const size_t &end_index);
	void quicksort();
	void bubblesort();
	void radixsort();
	void insertionsort();
	void selectionsort();
	void shellsort();
	void shellsort2();
};
