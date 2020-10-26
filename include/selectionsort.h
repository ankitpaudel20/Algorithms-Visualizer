
#include "sort.h"

template<class T>
void sortingClass<T>::selectionsort() {
	size_t i, j, k;
	T temp;
	T min = 0;

	for (i = 1; i < m_data.size(); i++) {
		for (j = i; j < m_data.size(); j++)
			min = array[j] < array[min] ? j : min;

		swap(array, i - 1, min);
		min = i;
	}
}

