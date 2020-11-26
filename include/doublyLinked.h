
#ifndef LINKED_H
#define LINKED_H

#include <cassert>
#include <iostream>
#include <exception>
#include <vector>

template <class T>
struct node
{
	using ValueType = T;

	T m_data;
	node<T>* next = nullptr;
	node<T>* previous = nullptr;

	explicit node(const T& data) : m_data(data) {}

	~node()
	{
		delete next;
	}
};

template <typename linkedList>
class listIterator
{
public:
	using ValueType = typename linkedList::ValueType;
	using PointerType = ValueType*;
	using RefrenceType = ValueType&;

public:
	listIterator(PointerType ptr) : m_Ptr(ptr) {}

	listIterator& operator++()
	{
		m_Ptr = m_Ptr->next;
		return *this;
	}

	listIterator& operator++(int)
	{
		listIterator it = *this;
		++(*this);
		return it;
	}

	listIterator& operator--()
	{
		m_Ptr = m_Ptr->previous;
		return *this;
	}

	listIterator& operator--(int)
	{
		listIterator it = *this;
		++(*this);
		return it;
	}

	listIterator& operator +(int value)
	{
		while (value-- > 0)
		{
			m_Ptr = m_Ptr->next;
			if (m_Ptr == nullptr)
				return *this;
		}
		return *this;
	}

	listIterator& operator -(int value)
	{
		while (value-- > 0)
		{
			m_Ptr = m_Ptr->previous;
			if (m_Ptr == nullptr)
				return *this;
		}
		return *this;
	}

	auto& operator* () {
		return m_Ptr->m_data;
	}

	bool operator==(const listIterator& other) const {
		return other.m_Ptr == m_Ptr;
	}

	bool operator!=(const listIterator& other) const {
		return other.m_Ptr != m_Ptr;
	}


private:
	PointerType m_Ptr;
};

template <class T>
class linkedList
{
private:
	node<T>* first = nullptr;
	node<T>* last = nullptr;
	unsigned m_size = 0;

	inline T deleteNode(node<T>* node)
	{
		T tempData = node->m_data;
		if (node->previous)
			node->previous->next = node->next;
		if (node->next)
			node->next->previous = node->previous;

		node->next = nullptr;
		delete node;
		m_size--;

		return tempData;
	}

public:
	using ValueType = node<T>;
	using Iterator = listIterator<linkedList<T>>;

public:
	linkedList() : m_size(0), first(nullptr), last(nullptr) {}

	explicit linkedList(const T& input) : m_size(1)
	{
		first = new node<T>(input);
		last = first;
	}

	linkedList(linkedList&& in) {
		first = in.first;
		last = in.last;
		m_size = in.m_size;
		in.first = nullptr;
		in.last = nullptr;
	}

	explicit linkedList(std::initializer_list<T> l) {
		for (auto& e : l)
		{
			insertLast(e);
		}
	}

	~linkedList() { delete first; }

	unsigned& size()
	{
		return m_size;
	}

	void mergeList(linkedList& other) {

		m_size += other.m_size;

		last->next = other.first;
		other.first->previous = last;

		last = other.last;

		other.first = nullptr;
		other.last = nullptr;
	}

	T& operator[](const unsigned& index)
	{

		if (index >= m_size)
			throw std::runtime_error("out of range");
		node<T>* ntemp;
		unsigned temp;

		if (index > m_size / 2)
		{
			ntemp = last;
			temp = m_size - index - 1;

			if (temp != 0)
			{
				while (temp-- > 0)
					ntemp = ntemp->previous;
				return ntemp->m_data;
			}
			else
				return last->m_data;
		}
		else
		{
			ntemp = first;
			temp = index;

			if (temp != 0)
			{
				while (temp-- > 0)
					ntemp = ntemp->next;
				return ntemp->m_data;
			}
			else
				return first->m_data;
		}
	}

	void insertAt(const T& data, const unsigned& index)
	{
		if (index > m_size)
			throw std::runtime_error("out of range");

		if (index == m_size)
			return insertLast(data);
		else if (index == 0)
			return insertBegin(data);

		unsigned temp;
		node<T>* ntemp;
		node<T>* newnode = new node<T>(data);
		m_size++;

		if (index > m_size / 2)
		{
			ntemp = last;
			temp = m_size - index - 1;

			while (temp-- > 0)
				ntemp = ntemp->previous;

			ntemp->previous->next = newnode;
			newnode->previous = ntemp->previous;

			newnode->next = ntemp;
			ntemp->previous = newnode;
		}
		else
		{
			ntemp = first;
			temp = index;

			while (temp-- > 0)
				ntemp = ntemp->next;

			ntemp->previous->next = newnode;
			newnode->previous = ntemp->previous;

			newnode->next = ntemp;
			ntemp->previous = newnode;
		}
	}

	void insertBegin(const T& data)
	{
		node<T>* temp = first;
		first = new node<T>(data);
		first->next = temp;

		if (temp)
			temp->previous = first;
		else
			last = first;

		m_size++;
	}

	void insertLast(const T& data)
	{

		node<T>* newend = new node<T>(data);
		if (last)
			last->next = newend;
		newend->previous = last;
		last = newend;
		if (!first)
			first = last;

		m_size++;
	}

	void insertAfter(const T& data, const T& refdata)
	{
		node<T>* temp = first;
		T tempData;
		while (temp->next != nullptr)
		{
			if (temp->m_data == refdata)
			{
				node<T>* tempnext = temp->next;

				temp->next = new node<T>(data);
				temp->next->previous = temp;

				temp->next->next = tempnext;
				tempnext->previous = temp->next;

				m_size++;
				return;
			}
			temp = temp->next;
		}
		throw std::runtime_error("data not found");
	}

	void insertBefore(const T& data, const T& refdata)
	{
		if (refdata == first->m_data)
			return insertBegin(data);

		node<T>* temp = first;
		while (temp != nullptr)
		{
			if (temp->next->m_data == refdata)
			{
				node<T>* tempnext = temp->next;

				temp->next = new node<T>(data);
				temp->next->previous = temp;

				temp->next->next = tempnext;
				tempnext->previous = temp->next;

				m_size++;
				return;
			}
			temp = temp->next;
		}
	}

	T removeAfter(const T& data)
	{

		if (data == last->m_data)
			throw std::runtime_error("its the last data");

		node<T>* temp = first;
		while (temp->next != nullptr)
		{
			if (temp->m_data == data)
			{
				if (temp->next->next == nullptr)
					last = temp;
				return deleteNode(temp->next);
			}
			temp = temp->next;
		}
		throw std::runtime_error("the data is not found");
	}

	T removeStart()
	{
		if (first == nullptr)
			throw std::runtime_error("empty list");

		node<T>* temp = first;

		first = first->next;
		if (first)
			first->previous = nullptr;

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
			node<T>* temp = last;

			last = last->previous;
			if (last)
				last->next = nullptr;
			T ret = temp->m_data;
			delete temp;
			m_size--;

			return ret;
		}

		throw std::runtime_error("no data to remove");
	}

	T removeAt(const unsigned& index)
	{
		if (index >= m_size)
			throw std::runtime_error("index not in range");

		else if (index == 0)
			removeStart();
		else if (index == m_size - 1)
			removeLast();

		unsigned temp;
		node<T>* ntemp;
		T tempData;

		if (index > m_size / 2)
		{
			ntemp = last;
			temp = m_size - index - 1;

			while (temp-- > 0)
				ntemp = ntemp->previous;
		}
		else
		{
			ntemp = first;
			temp = index;

			while (temp-- > 0)
				ntemp = ntemp->next;
		}

		return deleteNode(ntemp);
	}

	T removeData(const T& data, const bool& beginning = true)
	{

		if (first->m_data == data)
			return removeStart();
		else if (last->m_data == data)
			return removeLast();

		node<T>* temp;
		if (beginning)
		{
			temp = first;
			while (temp->next != nullptr)
			{
				if (temp->m_data == data)
					return deleteNode(temp);
				temp = temp->next;
			}
		}
		else
		{
			temp = last;
			while (temp->previous != nullptr)
			{
				if (temp->m_data == data)
					return deleteNode(temp);
				temp = temp->previous;
			}
		}
		throw std::runtime_error("data not found");
	}

	Iterator begin() { return Iterator(first); }

	Iterator end() { return Iterator(nullptr); }

	Iterator itLast() { return Iterator(last); }

	template <class U>
	friend std::ostream& operator<<(std::ostream& out, linkedList<U>& in)
	{
		for (auto i : in)
		{
			out << i << " , ";
		}
		out << std::endl;

		return out;
	}

	void check()
	{
		unsigned a = 0;
		node<T>* temp = first;
		node<T>* parent = nullptr;
		while (temp != nullptr)
		{
			if (temp->previous != parent)
			{
				std::cerr << "list not good; error in previous\n";
			}

			if (parent)
			{
				if (parent->next != temp)
				{
					std::cerr << "list not good; error in next\n";
				}
			}

			parent = temp;
			temp = temp->next;
			a++;
		}
		if (a != m_size)
		{
			std::cerr << "list not good; error in size\n";
		}
	}
};

#endif //LINKED_H