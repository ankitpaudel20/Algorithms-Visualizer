#include "sort.h"

template<class T>
inline void sortingClass<T>::merge(T* arr, const size_t& a, const size_t& m, const size_t& b) {
	auto size = sizeof(T);
	T* tmp = new T[b - a + 1];
	size_t i = a, j = m + 1, k = 0;
	while (i < m + 1 && j < b + 1) {
		if (arr[i] < arr[j])
			tmp[k] = arr[i++];
		else
			tmp[k] = arr[j++];
		k++;
	}
	if (i < m + 1)
		memcpy(tmp + (b - a - m + i), arr + i, (m + 1 - i) * size);
	else if (j < b + 1)
		memcpy(tmp + (j - a), arr + j, (b - j + 1) * size);

	memcpy(arr + a, tmp, (b - a + 1) * size);
	delete[] tmp;
}

template<class T>
inline void sortingClass<T>::sortSubroutine(T* arr, const size_t& a, const size_t& b) {
	if (b <= a)
		return;
	unsigned m = (a + b) / 2;
	sortSubroutine<T>(arr, a, m);
	sortSubroutine<T>(arr, m + 1, b);
	merge<T>(arr, a, m, b);
}

template<class T>
inline void sortingClass<T>::mergesort() { sortSubroutine<T>(&m_data[0], 0, m_data.size() - 1); }

