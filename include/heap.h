#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
enum heapError
{
	HEAP_EMPTY
};

class heapException
{
	heapError type;

public:
	heapException(const heapError& error) : type(error) {}

	const heapError what() const noexcept
	{
		return type;
	}
};

inline unsigned rc(unsigned i) { return 2 * i + 2; }

inline unsigned lc(unsigned i) { return 2 * i + 1; }

inline unsigned par(unsigned i) { return i / 2; }

template <class T>
class heap
{
	std::vector<T> m_data;

	//std::function<bool(const T const&, const T const&)> cmp;
	bool (*cmp)(T const&, T const&);

	void balanceBelow(const unsigned& index)
	{
		if (!hasleft(index))
			return;

		if (hasright(index))
		{
			const unsigned& greater = cmp(m_data[lc(index)], m_data[rc(index)]) ? lc(index) : rc(index);
			//const unsigned& greater = m_data[lc(index)] > m_data[rc(index)] ? lc(index) : rc(index);
			if (cmp(m_data[greater], m_data[index]))
				//if (m_data[index] < m_data[greater])
			{
				swap(m_data[greater], m_data[index]);
				balanceBelow(greater);
			}
		}
		else
		{
			//if (m_data[index] < m_data[lc(index)])
			if (cmp(m_data[lc(index)], m_data[index]))
			{
				swap(m_data[lc(index)], m_data[index]);
				balanceBelow(lc(index));
			}
		}
	}

	void balanceAbove(const unsigned& index)
	{
		if (index == 0)
			return;

		if (cmp(m_data[index], m_data[par(index)]))
			//if (m_data[par(index)] < m_data[index])
		{
			swap(m_data[par(index)], m_data[index]);
			balanceAbove(par(index));
		}
	}

	void heapify()
	{
		unsigned h = std::pow(2, getheight() - 1) + 1;
		int start = pow(2, getheight() - 1) - 1;
		auto end = pow(2, getheight()) - 2;
		for (unsigned i = start; i <= end; i++)
		{
			balanceBelow(i);
		}
		if (start > 2)
			for (--start; start >= 0; --start)
				balanceBelow(start);

	}

	inline bool hasright(unsigned index) { return 2 * (int)index + 1 < (int)m_data.size() - 1; }

	inline bool hasleft(unsigned index) { return  2 * (int)index < (int)m_data.size() - 1; }

	inline void swap(T& first, T& second)
	{
		T temp = first;
		first = second;
		second = temp;
	}

public:



	heap(bool (*_cmp)(T const&, T const&)) : cmp(_cmp) {}

	void changeComp(bool (*_cmp)(T const&, T const&)) { cmp = _cmp; }

	void addArray(const T* data, const size_t& size) {
		for (int i = 0; i < size; i++)
			push(data[i]);
	}

	void remakeHeap() {
		heapify();
	}

	unsigned getheight() { return std::floor(std::log2(m_data.size())); }

	void reserve(const unsigned& count) { m_data.reserve(count); }

	std::vector<T> getSorted()
	{
		std::vector<T> ret(m_data.size());
		ret.emplace_back(pop());
	}

	void push(const T& data)
	{
		m_data.emplace_back(data);
		balanceAbove(m_data.size() - 1);
	}

	void add(const T& data)
	{
		m_data.emplace_back(data);
	}

	bool empty() { return m_data.empty(); }

	T pop()
	{
		if (m_data.size() == 0)
			throw heapException(heapError::HEAP_EMPTY);
		swap(m_data[0], m_data[m_data.size() - 1]);
		auto ret = m_data.back();
		m_data.pop_back();
		balanceBelow(0);
		return ret;
	}

};