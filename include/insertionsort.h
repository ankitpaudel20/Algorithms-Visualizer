#include "sort.h"

template<class T>
void sortingClass<T>::insertionsort() {
	size_t i, j, k;
	T temp;
	for (i = 1; i < m_data.size(); i++) {
		for (j = i - 1; j >= 0 && j < m_data.size(); j--) {
			if (m_data[j] < m_data[i]) {
				temp = m_data[i];
				memcpy(&m_data[0] + j + 1, &m_data[0] + j, (i - j) * sizeof(T));
				m_data[j + 1] = temp;
				break;
			}
		}
		if (j > m_data.size()) {
			temp = m_data[i];
			memcpy(&m_data[0] + 1, &m_data[0], i * sizeof(T));
			m_data[0] = temp;
		}
	}
}
