//
// Created by ankit on 6/6/2020.
//

#ifndef LINKED_H
#define LINKED_H

#include <cassert>
#include <iostream>
#include <exception>
#include <vector>

template <class T>
struct listNode
{
	T m_data;
	listNode<T>* next;

	explicit listNode(const T& data) : m_data(data), next(nullptr) {}

	~listNode()
	{
		delete next;
	}
};

template <class T>
class linkedList
{
private:
	listNode<T>* start;
	unsigned m_size;

public:
	linkedList() : m_size(0), start(nullptr) {}

	explicit linkedList(const T& input) : m_size(1)
	{
		start = new listNode<T>(input);
	}

	unsigned& size()
	{
		return m_size;
	}

	T& operator[](const unsigned& index)
	{
		unsigned temp = index;

		if (index >= m_size)
			throw std::runtime_error("out of range");

		listNode<T>* ntemp = start;

		if (temp != 0)
		{
			while (temp-- > 0)
				ntemp = ntemp->next;
			return ntemp->m_data;
		}
		return start->m_data;
	}

	void insertAt(const T& data, const unsigned& index)
	{

		unsigned temp = index;

		listNode<T>* ntemp = start;

		if (temp != 0)
		{
			if (index > m_size)
				throw std::runtime_error("out of range");
			while (temp-- > 1)
				ntemp = ntemp->next;

			listNode<T>* detached = ntemp->next;

			ntemp->next = new listNode<T>(data);
			ntemp->next->next = detached;
			m_size++;
			return;
		}

		start = new listNode<T>(data);
		start->next = ntemp;
		m_size++;
		return;
	}

	void insertBegin(const T& data)
	{
		listNode<T>* temp = start;
		start = new listNode<T>(data);
		start->next = temp->next;
		m_size++;
	}

	void insertLast(const T& data)
	{
		if (start != nullptr)
		{
			listNode<T>* temp = start;
			while (temp->next != nullptr)
				temp = temp->next;

			temp->next = new listNode<T>(data);
			m_size++;
		}
		else
		{
			start = new listNode<T>(data);
			m_size++;
		}
	}

	void insertAfter(const T& data, const T& refdata)
	{
		listNode<T>* temp = start;
		T tempData;
		while (temp->next != nullptr)
		{
			if (temp->m_data == refdata)
			{
				listNode<T>* tempnext = temp->next;
				temp->next = new listNode<T>(data);
				temp->next->next = tempnext;
				m_size++;
				return;
			}
			temp = temp->next;
		}
		throw std::runtime_error("data not found");
	}

	void insertBefore(const T& data, const T& refdata)
	{
		listNode<T>* temp = start;
		while (temp != nullptr)
		{
			if (temp->next->m_data == refdata)
			{
				auto* tempnext = temp->next;
				temp->next = new listNode<T>(data);
				temp->next->next = tempnext;
				m_size++;
				return;
			}
		}
	}

	T removeAfter(const T& data)
	{
		listNode<T>* temp = start;
		while (temp->next != nullptr)
		{
			if (temp->m_data == data)
			{
				T ret;
				auto* detached_next = temp->next->next;
				ret = temp->next->m_data;
				temp->next->next = nullptr;
				delete temp->next;
				temp->next = detached_next;
				return ret;
			}
			temp = temp->next;
		}
		throw std::runtime_error("the data is not found");
	}

	T removeStart()
	{
		if (start == nullptr)
			throw std::runtime_error("empty list");
		listNode<T>* temp = start;
		start = start->next;
		temp->next = nullptr;
		T ret = temp->m_data;
		delete temp;
		m_size--;
		return ret;
	}

	T removeLast()
	{
		if (m_size != 0)
		{
			listNode<T>* temp = start;
			while (temp->next->next != nullptr)
				temp = temp->next;

			delete temp->next;
			temp->next = nullptr;
			m_size--;
		}
		throw std::runtime_error("no data to remove");
	}

	T removeAt(const unsigned& index)
	{
		if (index >= m_size)
			// throw std::runtime_error("index not in range");
			throw std::runtime_error("index not in range");

		listNode<T>* temp = start;
		listNode<T>* parent = nullptr;
		T tempData;

		unsigned tmp = index;
		while (tmp-- != 0)
		{
			parent = temp;
			temp = temp->next;
		}

		tempData = temp->m_data;

		if (parent != nullptr)
			parent->next = temp->next;
		else
			start = temp->next;

		temp->next = nullptr;
		delete temp;
		m_size--;
		return tempData;
	}

	T removeData(const T& data)
	{
		listNode<T>* temp = start;
		listNode<T>* parent = nullptr;
		T tempData;
		int i = 0;
		while (temp->next != nullptr)
		{
			if (temp->m_data == data)
			{
				tempData = temp->m_data;

				if (parent != nullptr)
					parent->next = temp->next;
				else
					start = temp->next;

				temp->next = nullptr;
				delete temp;
				m_size--;
				return tempData;
			}

			parent = temp;
			temp = temp->next;
			i++;
		}
		throw std::runtime_error("data not found");
	}

	const T& first() { return start->m_data; }

	const T& last()
	{
		listNode<T>* temp = start;
		while (temp->next != nullptr)
			temp = temp->next;

		return temp->m_data;
	}

	template <class U>
	friend std::ostream& operator<<(std::ostream& out, linkedList<U>& in)
	{
		auto temp = in.start;
		while (temp != nullptr)
		{
			out << temp->m_data << ((temp->next) ? " , " : "");
			temp = temp->next;
		}
		out << std::endl;
		return out;
	}
};

#endif //LINKED_H