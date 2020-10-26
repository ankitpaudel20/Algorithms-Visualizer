#include "sort.h"

template<class T>
void sortingClass<T>::bubblesort()
{
	auto size = m_data.size();
	for (uint32_t i = 0; i < size; i++)
		for (uint32_t j = 0; j < size - i; j++)
			if (cmp(&m_data[0], j, j + 1))
				swap<T>(&m_data[0], j, j + 1);
	sorted = true;
}
