
#ifndef LAB1_STACK_H
#define LAB1_STACK_H

#include <iostream>
#include <stdexcept>

class stackException : public std::exception
{
private:
	std::string m_error;

public:
	stackException(std::string error) : m_error{ error } {}

	const char* what() const noexcept { return m_error.c_str(); }
};

template <class T>
class stack
{
	T* m_data;
	T* ptr;

	unsigned m_size;
	unsigned capacity;

public:

	stack(const unsigned& initial_size = 5) : capacity(initial_size), m_size(0)
	{
		m_data = new T[initial_size];
		ptr = m_data;
	}

	~stack()
	{
		delete m_data;
	}

	void push(const T& data)
	{
		if (m_size != capacity)
		{
			*ptr = data;
			if (m_size + 1 < capacity)
			{
				ptr++;
			}
			m_size++;
		}
		else {
			T* temp = new T[capacity];
			memcpy(temp, m_data, capacity * sizeof(T));
			delete m_data;
			m_data = new T[capacity * 2];
			memcpy(m_data, temp, capacity * sizeof(T));
			delete temp;
			capacity *= 2;
		}
	}

	T pop()
	{
		T ret;
		if (m_size != 0)
		{
			if (m_size < capacity) {
				ret = *(ptr - 1);
				ptr--;
			}
			else
			{
				ret = *ptr;
			}
			m_size--;
		}
		else
			throw stackException("stack underflow");

		return ret;
	}

	const T* top()
	{
		if (m_size != 0)
			return ptr - 1;
		else
			return nullptr;
	}

	bool empty()
	{
		return m_size == 0;
	}

	size_t size() { return m_size; }

};

#endif //LAB1_STACK_H
