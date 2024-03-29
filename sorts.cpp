
#include "sort.h"

#include "heap.h"

void sortingClass::selectionsort()
{
	size_t i, j, k;
	uint32_t temp;
	uint32_t min = 0;

	for (i = 1; i < m_data.size(); i++)
	{
		for (j = i; j < m_data.size(); j++)
			// min = m_data[j] < m_data[min] ? j : min;
			min = cmp(j, min) ? min : j;

		swap(i - 1, min);
		min = i;
	}
	sorted = true;
}

void sortingClass::qsort(const size_t& start_index, const size_t& end_index)
{
	if (end_index == start_index + 1)
	{
		if (m_data[end_index] < m_data[start_index])
			// if (cmp(end_index, start_index))
			swap(end_index, start_index);
		return;
	}
	if (end_index <= start_index || end_index >= std::numeric_limits<unsigned long>::max() - 2)
		return;

	size_t pivot = start_index + (end_index - start_index) / 2;
	size_t left_iterator = pivot - 1;
	size_t right_iterator = pivot + 1;
	size_t left_unfit_count = 0;
	size_t right_unfit_count = 0;

	for (left_iterator; left_iterator >= start_index && left_iterator < end_index; left_iterator--)
	{
		// if (m_data[left_iterator] > m_data[pivot])
		if (cmp(left_iterator, pivot))
		{
			if (right_iterator <= end_index)
			{
				for (right_iterator; right_iterator <= end_index; right_iterator++)
				{
					if (cmp(pivot, right_iterator))
						// if (m_data[right_iterator] < m_data[pivot])
					{
						swap(left_iterator, right_iterator);
						right_iterator += (right_iterator != end_index);
						break;
					}
				}
				if (right_iterator > end_index)
				{
					swap(left_iterator, pivot - left_unfit_count - 1);
					left_unfit_count++;
				}
			}
			else
			{
				swap(left_iterator, pivot - left_unfit_count - 1);
				left_unfit_count++;
			}
		}
	}

	if (left_unfit_count)
	{
		swap(pivot - left_unfit_count, pivot);
		pivot -= left_unfit_count;
	}

	if (right_iterator <= end_index)
	{
		for (right_iterator; right_iterator <= end_index; right_iterator++)
		{
			if (cmp(pivot, right_iterator))
				// if (m_data[right_iterator] < m_data[pivot])
				swap(right_iterator, pivot + ++right_unfit_count);
		}

		if (right_unfit_count)
		{
			swap(pivot + right_unfit_count, pivot);
			pivot += right_unfit_count;
		}
	}

	qsort(start_index, pivot - 1);
	qsort(pivot + 1, end_index);
}

void sortingClass::quicksort()
{
	qsort(0, m_data.size() - 1);
	sorted = true;
}

void sortingClass::merge(uint32_t* arr, const size_t& a, const size_t& m, const size_t& b)
{
	auto size = sizeof(uint32_t);
	uint32_t* tmp = new uint32_t[b - a + 1];
	size_t i = a, j = m + 1, k = 0;
	while (i < m + 1 && j < b + 1)
	{
		// if (arr[i] < arr[j])
		if (cmp(j, i))
			tmp[k] = arr[i++];
		else
			tmp[k] = arr[j++];
		k++;
	}
	if (i < m + 1)
	{
		memcpy(tmp + (b - a - m + i), arr + i, (m + 1 - i) * size);
	}
	else if (j < b + 1)
	{
		memcpy(tmp + (j - a), arr + j, (b - j + 1) * size);
	}

	// memcpy(arr + a, tmp, (b - a + 1) * size);
	int time = sleeptime * 1000;
	for (i = a, j = 0; j <= b - a; i++, j++)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
		arr[i] = tmp[j];
	}

	delete[] tmp;
}

void sortingClass::mergesortSubroutine(const size_t& a, const size_t& b)
{
	if (b <= a)
		return;
	unsigned m = (a + b) / 2;
	mergesortSubroutine(a, m);
	mergesortSubroutine(m + 1, b);
	merge(&m_data[0], a, m, b);
}

void sortingClass::mergesort()
{
	mergesortSubroutine(0, m_data.size() - 1);
	sorted = true;
}

void sortingClass::bubblesort()
{
	auto size = m_data.size();
	for (uint32_t i = 0; i < size; i++)
		for (uint32_t j = 1; j < size - i; j++)
			if (cmp(j - 1, j))
				swap(j - 1, j);
	sorted = true;
}

void sortingClass::radixsort()
{
	unsigned max = 0;

	size_t i, j, k, l;

	for (i = 0; i < m_data.size(); i++)
		max = (m_data[i] > max) ? m_data[i] : max;

	int no_digits = 1;
	while (max >= 10)
	{
		no_digits++;
		max = max / 10;
	}

	std::vector<std::vector<uint32_t>> buckets(10);

	for (i = 0; i < 10; i++)
		buckets[i].reserve(m_data.size() / 5);

	uint32_t mul = 1;

	for (i = 0; i < no_digits; i++)
	{
		mul = std::pow(10, i);

		for (j = 0; j < m_data.size(); j++)
			buckets[(m_data[j] / mul) % 10].push_back(m_data[j]);

		for (j = 0, k = 0; j < 10; j++)
		{
			for (l = 0; l < buckets[j].size(); l++)
				m_data[k++] = buckets[j][l];
			buckets[j].clear();
		}
	}
}

void sortingClass::insertionsort()
{
	size_t i, j;
	for (i = 1; i < m_data.size(); i++)
	{
		for (j = i - 1; j >= 0 && j < m_data.size() && cmp(j, j + 1); j--)
		{
			swap(j, j + 1);
		}
	}
	sorted = true;
}

void sortingClass::shellsort2()
{
	std::vector<uint32_t> k;
	k.push_back(1);
	while (k.back() < m_data.size())
	{
		k.push_back(3 * k.back() + 1);
	}
	k.pop_back();

	size_t i, j;

	for (auto gap = k.rbegin(); gap != k.rend(); gap++) {
		for (i = *gap; i < m_data.size(); i++) {
			for (j = i; j >= *gap && j < m_data.size() && cmp(j - *gap, j); j -= *gap)
			{
				swap(j, j - *gap);
			}
		}
	}
	sorted = true;
}

void sortingClass::shellsort()
{
	for (int gap = N / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < N; i += 1) {
			for (int j = i; j >= gap && cmp(j - gap, j); j -= gap)
				swap(j, j - gap);
		}
	}
	sorted = true;
}

void sortingClass::heapsort()
{
	heapify();
	for (size_t i = 0; i < m_data.size(); i++)
	{
		swap(0, m_data.size() - (i + 1));
		balanceBelow(m_data.size() - (i + 1), 0);
	}
	sorted = true;
}

void sortingClass::balanceBelow(const size_t& size, const unsigned& index)
{
	if (!hasleft(index, size))
		return;
	if (hasright(index, size))
	{
		const unsigned& greater = cmp(lc(index), rc(index)) ? lc(index) : rc(index);
		if (cmp(greater, index))
		{
			swap(greater, index);
			balanceBelow(size, greater);
		}
	}
	else
	{
		if (cmp(lc(index), index))
		{
			swap(lc(index), index);
			balanceBelow(size, lc(index));
		}
	}
}

void sortingClass::heapify()
{
	unsigned h = std::pow(2, getheight() - 1) + 1;
	int start = pow(2, getheight() - 1) - 1;
	auto end = pow(2, getheight()) - 2;
	for (unsigned i = start; i <= end; i++)
	{
		balanceBelow(m_data.size(), i);
	}
	for (--start; start >= 0; --start)
	{
		balanceBelow(m_data.size(), start);
	}
}
