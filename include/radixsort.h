
#include "sort.h"

template<class T>
void sortingClass<T>::radixsort()
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
