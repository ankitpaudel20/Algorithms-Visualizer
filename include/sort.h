#pragma once

#include <random>
#include <thread>
#include <vector>
#include <array>
#include <mutex>

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
		m_data = random_generator<uint32_t>(0, viewport.h, N);
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
				for (int i = copyPos[0]; i < copyPos[1]; i++)
				{
					SDL_FRect a{(2 * i + 1) * spacing - width / 2, viewport->h - m_data[i], width, m_data[i]};
					SDL_RenderDrawRectF(renderer, &a);
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
				glLineWidth(5.0f);
				for (auto i : compPos)
				{
					SDL_FRect a{(2 * i + 1) * spacing - width / 2, viewport->h - m_data[i], width, m_data[i]};
					SDL_RenderDrawRectF(renderer, &a);
				}
				compMutex.unlock();
			}
			else
			{
				/* printf("values after sort : ");
				for (auto i : m_data)
				{
					printf("%d, ", i);
				}
				printf("\n"); */

				if (thread->joinable())
					thread->join();
				delete thread;
				thread = nullptr;
				state = appState::Idle;
				sorted = false;
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
		compMutex.lock();
		compPos.clear();
		compMutex.unlock();
		return m_data[left] > m_data[right];
	}

	inline bool cmpless(const size_t &left, const size_t &right)
	{
		compPos.emplace_back(left);
		compPos.emplace_back(right);
		int a = sleeptime * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));
		compMutex.lock();
		compPos.clear();
		compMutex.unlock();
		return m_data[left] < m_data[right];
	}

	inline void swap(const size_t &left, const size_t &right)
	{
		swapPos.emplace_back(left);
		swapPos.emplace_back(right);
		int a = sleeptime * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));
		uint32_t temp = m_data[left];
		m_data[left] = m_data[right];
		m_data[right] = temp;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));

		swapMutex.lock();
		swapPos.clear();
		swapMutex.unlock();
	}

	inline void memcopy(const uint32_t &to, const uint32_t &from, const uint32_t &number)
	{
		auto *arr = &m_data[0];
		copyPos.emplace_back(from);
		copyPos.emplace_back(number);
		int a = sleeptime * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));
		memcpy(arr + to, arr + from, number * sizeof(uint32_t));
		copyPos[0] = to;
		std::this_thread::sleep_for(std::chrono::milliseconds(a));

		copyMutex.lock();
		copyPos.clear();
		copyMutex.unlock();
	}

	void run(int selection)
	{
		if (selection == 0)
		{
			/* printf("values before sort : ");
			for (auto i : m_data)
			{
				printf("%d, ", i);
			}
			printf("\n"); */
			// insertionsort();
			thread = new std::thread(&sortingClass::insertionsort, this);
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
};
