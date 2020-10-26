#include "sort.h"

template<class T>
void sortingClass<T>::qsort(T* array, const size_t& start_index, const size_t& end_index) {
	if (end_index == start_index + 1) {
		if (array[end_index] < array[start_index])
			swap(array, end_index, start_index);
		return;
	}
	if (end_index <= start_index || end_index >= std::numeric_limits<unsigned long>::max() - 2)
		return;

	size_t pivot = start_index + (end_index - start_index) / 2;
	size_t left_iterator = pivot - 1;
	size_t right_iterator = pivot + 1;
	size_t left_unfit_count = 0;
	size_t right_unfit_count = 0;

	for (left_iterator; left_iterator >= start_index && left_iterator < end_index; left_iterator--) {
		if (array[left_iterator] > array[pivot]) {
			if (right_iterator <= end_index) {
				for (right_iterator; right_iterator <= end_index; right_iterator++) {
					if (array[right_iterator] < array[pivot]) {
						swap(array, left_iterator, right_iterator);
						right_iterator += (right_iterator != end_index);
						break;
					}
				}
				if (right_iterator > end_index) {
					swap(array, left_iterator, pivot - left_unfit_count - 1);
					left_unfit_count++;
				}
			}
			else {
				swap(array, left_iterator, pivot - left_unfit_count - 1);
				left_unfit_count++;
			}
		}
	}

	if (left_unfit_count) {
		swap(array, pivot - left_unfit_count, pivot);
		pivot -= left_unfit_count;
	}

	if (right_iterator <= end_index) {
		for (right_iterator; right_iterator <= end_index; right_iterator++) {
			if (array[right_iterator] < array[pivot])
				swap(array, right_iterator, pivot + ++right_unfit_count);
		}

		if (right_unfit_count) {
			swap(array, pivot + right_unfit_count, pivot);
			pivot += right_unfit_count;
		}
	}

	qsort(array, start_index, pivot - 1);
	qsort(array, pivot + 1, end_index);
}

template<class T>
void sortingClass<T>::quicksort() { qsort<T>(&m_data[0], 0, m_data.size() - 1); }
