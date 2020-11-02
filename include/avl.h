#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>
#include "common.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

// insert
// find
// height

class app;
namespace treeAdd
{
	unsigned indent_counter;
}

template <class T>
struct node
{
	struct rotation
	{
		static node<T>* rr(node<T>* node1)
		{

			auto node2 = node1->rightChild;
			auto strayLeftNode2 = node2->leftChild;
			auto strayParentNode1 = node1->parent;
			node2->leftChild = node1;
			node1->parent = node2;
			node1->rightChild = strayLeftNode2;
			if (strayLeftNode2 != nullptr)
			{
				strayLeftNode2->parent = node1;
			}
			if (strayParentNode1)
			{
				if (strayParentNode1->rightChild == node1)
					strayParentNode1->rightChild = node2;
				else
					strayParentNode1->leftChild = node2;
			}
			node2->parent = strayParentNode1;

			node1->refreshHeight();
			node2->refreshHeight();
			return node2;
		};

		static node<T>* lr(node<T>* node1)
		{
			auto node2 = node1->leftChild;
			auto node3 = node1->leftChild->rightChild;
			auto strayLeftNode3 = node3->leftChild;
			auto strayRightNode3 = node3->rightChild;
			auto strayParentNode1 = node1->parent;
			node3->leftChild = node2;
			node2->parent = node3;
			node3->rightChild = node1;
			node1->parent = node3;
			node2->rightChild = strayLeftNode3;
			if (strayLeftNode3 != nullptr)
			{
				strayLeftNode3->parent = node2;
			}
			node1->leftChild = strayRightNode3;
			if (strayRightNode3 != nullptr)
			{
				strayRightNode3->parent = node1;
			}
			if (strayParentNode1)
			{
				if (strayParentNode1->rightChild == node1)
					strayParentNode1->rightChild = node3;
				else
					strayParentNode1->leftChild = node3;
			}
			node3->parent = strayParentNode1;

			node1->refreshHeight();
			node2->refreshHeight();
			node3->refreshHeight();
			return node3;
		};

		static node<T>* rl(node<T>* node1)
		{
			auto node2 = node1->rightChild;
			auto node3 = node1->rightChild->leftChild;
			auto strayLeftNode3 = node3->leftChild;
			auto strayRightNode3 = node3->rightChild;
			auto strayParentNode1 = node1->parent;
			node3->leftChild = node1;
			node1->parent = node3;
			node3->rightChild = node2;
			node2->parent = node3;

			node1->rightChild = strayLeftNode3;
			if (strayLeftNode3 != nullptr)
			{
				strayLeftNode3->parent = node1;
			}

			node2->leftChild = strayRightNode3;
			if (strayRightNode3 != nullptr)
			{
				strayRightNode3->parent = node2;
			}

			node3->parent = strayParentNode1;
			if (strayParentNode1)
			{
				if (strayParentNode1->rightChild == node1)
					strayParentNode1->rightChild = node3;
				else
					strayParentNode1->leftChild = node3;
			}
			node1->refreshHeight();
			node2->refreshHeight();
			node3->refreshHeight();
			return node3;
		};

		static node<T>* ll(node<T>* node1)
		{
			auto node2 = node1->leftChild;
			auto strayRightNode2 = node2->rightChild;
			auto strayParentNode1 = node1->parent;

			node2->rightChild = node1;
			node1->parent = node2;
			node1->leftChild = strayRightNode2;
			if (strayRightNode2 != nullptr)
			{
				strayRightNode2->parent = node1;
			}
			if (strayParentNode1)
			{
				if (strayParentNode1->rightChild == node1)
					strayParentNode1->rightChild = node2;
				else
					strayParentNode1->leftChild = node2;
			}
			node2->parent = strayParentNode1;

			node1->refreshHeight();
			node2->refreshHeight();
			return node2;
		};
	};

	body circle;
	SDL_Texture* letters;

	T* m_data = nullptr;
	unsigned int leftHeight = 0;
	unsigned int rightHeight = 0;
	unsigned depth;

	node<T>* leftChild = nullptr;
	node<T>* rightChild = nullptr;
	node<T>* parent = nullptr;

	/*  void addDataUoChild(node<T> *childNode, const T &data) {
	if (childNode != nullptr)
			  childNode->addData(data, this);
	else {
			  childNode = new node<T>(data, this);
		refresh(childNode);
	}
	  }*/

	static void refresh(node<T>* Node, uint32_t& maxdepth)
	{
		if (!Node)
			return;

		if (Node->rightChild)
		{
			Node->rightHeight = Node->rightChild->getHeight() + 1;
			Node->depth = Node->rightChild->depth - 1;
		}
		if (Node->leftChild)
		{
			Node->leftHeight = Node->leftChild->getHeight() + 1;
			Node->depth = Node->leftChild->depth - 1;
		}
		auto newCurrentNode = Node->balance();

		if (newCurrentNode != Node)
			maxdepth = newCurrentNode->refreshDepth();



		if (Node->rightChild)
		{
			Node->rightChild->circle.pos.x = Node->circle.pos.x + (uint16_t)((maxdepth - Node->rightChild->depth + 1) * (dist.x / 2));;
		}
		if (Node->leftChild)
		{
			Node->leftChild->circle.pos.x = Node->circle.pos.x - (uint16_t)((maxdepth - Node->leftChild->depth + 1) * (dist.x / 2));;
		}
		Node->rightChild->circle.pos.y = Node->circle.pos.y + dist.y;



		if (newCurrentNode == nullptr)
			return;

		refresh(newCurrentNode->parent, maxdepth);
	}

	static void swap(node<T>* node1, node<T>* node2)
	{
		auto node1left = node1->leftChild;
		auto node1right = node1->rightChild;
		auto node1parent = node1->parent;

		auto node2left = node2->leftChild;
		auto node2right = node2->rightChild;
		auto node2parent = node2->parent;

		/*	node1->leftChild = node2left;
			if (!node2left)
				node2left->parent = node1;
			node1->rightChild = node2right;
			if (!node2right)
				node2right->parent = node1;

			node2->leftChild = node1left;
			if (!node1left)
				node1left->parent = node2;
			node2->rightChild = node1right;
			if (!node1right)
				node1right->parent = node2;
				*/

		node1->leftChild = node2->leftChild;
		if (node2->leftChild)
			node2->leftChild->parent = node1;
		node1->rightChild = node2->rightChild;
		if (node2->rightChild)
			node2->rightChild->parent = node1;

		node1->parent = node2->parent;
		if (node2->parent)
		{
			if (node2->parent->rightChild == node2)
				node2->parent->rightChild = node1;
			else
				node2->parent->leftChild = node1;
		}

		node2->leftChild = node1left;
		if (node2->leftChild)
			node2->leftChild->parent = node2;
		node2->rightChild = node1right;
		if (node2->rightChild)
			node2->rightChild->parent = node2;

		node2->parent = node1parent;
		if (node1parent)
		{
			if (node1parent->rightChild == node1)
				node1parent->rightChild = node2;
			else
				node1parent->leftChild = node2;
		}
	}

	node<T>* balance()
	{
		int diff = leftHeight - rightHeight;
		if (diff >= 2)
		{
			int diff2 = leftChild->leftHeight - leftChild->rightHeight;
			if (diff2 < 0)
				return rotation::lr(this);
			else
				return rotation::ll(this);
		}
		else if (diff <= -2)
		{
			int diff2 = rightChild->leftHeight - rightChild->rightHeight;
			if (diff2 < 0)
				return rotation::rr(this);
			else
				return rotation::rl(this);
		}
		return this;
	}

	void refreshHeight()
	{
		rightHeight = (rightChild == nullptr) ? 0 : rightChild->getHeight() + 1;
		leftHeight = (leftChild == nullptr) ? 0 : leftChild->getHeight() + 1;
	}

	uint32_t refreshDepth()
	{
		uint32_t max, temp = 0;
		if (!parent)
			depth = 0;
		else
			depth = parent->depth + 1;

		max = depth;

		if (leftChild)
			temp = leftChild->refreshDepth();
		max = max < temp ? temp : max;
		if (rightChild)
			temp = rightChild->refreshDepth();
		return max < temp ? temp : max;
	}

	unsigned getHeight()
	{
		return std::max(rightHeight, leftHeight);
	}

	node(const T& data, node<T>* parentptr, const uint32_t& _depth, uint32_t& maxdepth)
	{
		parent = parentptr;
		depth = _depth;
		maxdepth = depth;
		m_data = new T;
		memcpy(m_data, &data, sizeof(T));
	}

	void addData(const T& data, node<T>* parentptr, const uint32_t& _depth, uint32_t& maxdepth)
	{
		if (m_data == nullptr)
		{
			parent = parentptr;
			m_data = new T;
			memcpy(m_data, &data, sizeof(T));
			depth = _depth;
			maxdepth = depth;
			refresh(this, maxdepth);
			return;
		}

		if (data > * m_data)
		{
			if (rightChild != nullptr)
				rightChild->addData(data, this, _depth + 1, maxdepth);
			else
			{
				rightChild = new node<T>(data, this, _depth + 1, maxdepth);
				refresh(this->rightChild, maxdepth);
			}
		}
		else
		{
			if (leftChild != nullptr)
				leftChild->addData(data, this, _depth + 1, maxdepth);
			else
			{
				leftChild = new node<T>(data, this, _depth + 1, maxdepth);
				//maxdepth = _depth + 1;
				refresh(this->leftChild, maxdepth);
			}
		}
	}

	node<T>* search(const T& data)
	{
		if (*m_data == data)
			return this;
		else if (data > * m_data)
		{
			if (!rightChild)
				return nullptr;
			return rightChild->search(data);
		}
		else
		{
			if (!leftChild)
				return nullptr;
			return leftChild->search(data);
		}
	}

	node<T>* greatest()
	{
		if (rightChild == nullptr)
			return this;
		else
			return rightChild->greatest();
	};

	node<T>* smallest()
	{
		if (leftChild == nullptr)
			return this;
		else
			return leftChild->smallest();
	};

	void traverse(std::vector<T>& sorted, const node<T>* from, const node<T>* greatest)
	{
		if (getHeight() == 0)
		{
			sorted.emplace_back(*m_data);
			if (*m_data == *greatest->m_data)
				return;
			parent->traverse(sorted, this, greatest);
		}
		else
		{
			if (from == leftChild)
			{
				sorted.emplace_back(*m_data);
				if (rightChild != nullptr)
					rightChild->traverse(sorted, this, greatest);
				else
					parent->traverse(sorted, this, greatest);
			}
			else if (from == rightChild)
			{
				parent->traverse(sorted, this, greatest);
			}
			else if (from == parent)
			{
				if (leftChild != nullptr)
					leftChild->traverse(sorted, this, greatest);
				else
				{
					sorted.emplace_back(*m_data);
					rightChild->traverse(sorted, this, greatest);
				}
			}
			else
			{
				std::cout << "error in traversng from pointer not identified" << std::endl;
				assert(false);
			}
		}
	}

	~node()
	{
		if (rightChild != nullptr)
			delete rightChild;
		if (leftChild != nullptr)
			delete leftChild;
		if (m_data != nullptr)
			delete m_data;
		std::cout << "node destroyed" << std::endl;
	}

	bool operator>(const node& rhs) const { return m_data > rhs.m_data; }

	bool operator>(const T& rhs) const { return m_data > m_data; }

	bool operator<(const node& rhs) const { return m_data < rhs.m_data; }

	bool operator>=(const node& rhs) const { return m_data >= rhs.m_data; }

	bool operator<=(const node& rhs) const { return m_data <= rhs.m_data; }

	bool operator==(const node& rhs) const { return m_data == rhs.m_data; }

	friend std::ostream& operator<<(std::ostream& out, const node<T>& n)
	{
		if (n.leftChild == nullptr)
			out << *n.m_data << "("
			<< "NaN";
		else
			out << *n.m_data << "(" << *n.leftChild;

		out << " , ";

		if (n.rightChild == nullptr)
			out << "NaN"
			<< ")";
		else
			out << *n.rightChild << ")";

		/* out << n.m_data << "(" <<
			 ((n.leftChild == nullptr) ? "NaN" : *n.leftChild) <<
			 " , " <<
			 ((n.rightChild == nullptr) ? "NaN" : *n.rightChild) <<
			 ")";*/
		return out;
	}

	void drawCircle(SDL_Renderer* renderer, vec2<uint32_t> parpos, const float& radius, const vec2<float>& dist, const uint32_t& maxdepth, SDL_Texture* (*getText)(std::string, const std::string&, const int&, const uint32_t&, SDL_Renderer*))
	{
		parpos.y = parpos.y + dist.y;
		SDL_Texture* text = getText(std::to_string(*m_data), "segoeui", 15, 0, renderer);
		SDL_Rect rect;
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);

		if (parent->leftChild == this)
			parpos.x = parpos.x - (uint16_t)((maxdepth - depth + 1) * (dist.x / 2));
		else
			parpos.x = parpos.x + (uint16_t)((maxdepth - depth + 1) * (dist.x / 2));

		rect.x = parpos.x - rect.w / 2;
		rect.y = parpos.y - rect.h / 2;
		filledCircleColor(renderer, parpos.x, parpos.y, radius, 0xFFFFFFFF);
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		text = getText(std::to_string(depth), "segoeui", 10, 0, renderer);
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = parpos.x - rect.w / 2;
		rect.y = parpos.y + radius - rect.h / 4;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		text = getText(std::to_string(leftHeight), "segoeui", 10, 0, renderer);
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = parpos.x - rect.w / 4 - radius;
		rect.y = parpos.y - rect.h / 4 - radius;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		text = getText(std::to_string(rightHeight), "segoeui", 10, 0, renderer);
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = rect.x + 2 * radius - rect.w / 4;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		if (leftChild)
			leftChild->drawCircle(renderer, parpos, radius, dist, maxdepth, getText);
		if (rightChild)
			rightChild->drawCircle(renderer, parpos, radius, dist, maxdepth, getText);
	}

	void drawLines(SDL_Renderer* renderer, vec2<uint32_t> parpos, const float& radius, const vec2<float>& dist, const uint32_t& maxdepth, SDL_Texture* (*getText)(std::string, const std::string&, const int&, const uint32_t&, SDL_Renderer*))
	{

		if (parent->leftChild == this)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, parpos.x, parpos.y, parpos.x - (maxdepth - depth + 1) * dist.x / 2, parpos.y + dist.y);
			parpos.x = parpos.x - (uint16_t)((maxdepth - depth + 1) * (dist.x / 2));
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, parpos.x, parpos.y, parpos.x + (maxdepth - depth + 1) * dist.x / 2, parpos.y + dist.y);
			parpos.x = parpos.x + (uint16_t)((maxdepth - depth + 1) * (dist.x / 2));
		}
		parpos.y = parpos.y + dist.y;

		if (leftChild)
			leftChild->drawLines(renderer, parpos, radius, dist, maxdepth, getText);
		if (rightChild)
			rightChild->drawLines(renderer, parpos, radius, dist, maxdepth, getText);
	}
};

template <class T>
class avlTree
{

private:
	node<T>* root = nullptr;
	unsigned no_of_data = 0;
	uint32_t maxDepth;

	void refreshroot()
	{
		while (root->parent != nullptr)
			root = root->parent;
	}

public:
	void draw(SDL_Renderer* renderer, const SDL_Rect& viewport, const uint16_t& radius, const vec2<float>& dist, SDL_Texture* (*getText)(std::string, const std::string&, const int&, const uint32_t&, SDL_Renderer*))
	{

		if (!root)
			return;

		if (root->leftChild)
			root->leftChild->drawLines(renderer, vec2<uint32_t>(viewport.w / 2, 100), radius, dist, maxDepth, getText);
		if (root->rightChild)
			root->rightChild->drawLines(renderer, vec2<uint32_t>(viewport.w / 2, 100), radius, dist, maxDepth, getText);

		filledCircleColor(renderer, viewport.w / 2, 100, radius, 0xFFFFFFFF);

		SDL_Texture* text = getText(std::to_string(*root->m_data), "segoeui", 15, 0, renderer);
		SDL_Rect rect;
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = viewport.w / 2 - rect.w / 2;
		rect.y = 100 - rect.h / 2;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		text = getText(std::to_string(root->leftHeight), "segoeui", 10, 0, renderer);
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = viewport.w / 2 - rect.w / 4 - radius;
		rect.y = 100 - rect.h / 4 - radius;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		text = getText(std::to_string(root->rightHeight), "segoeui", 10, 0, renderer);
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = rect.x + 2 * radius - rect.w / 4;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		vec2<uint32_t> pos(viewport.w, 0);
		if (root->leftChild)
			root->leftChild->drawCircle(renderer, vec2<uint32_t>(viewport.w / 2, 100), radius, dist, maxDepth, getText);
		if (root->rightChild)
			root->rightChild->drawCircle(renderer, vec2<uint32_t>(viewport.w / 2, 100), radius, dist, maxDepth, getText);
	}

	uint32_t size() { return no_of_data; }
	friend std::ostream& operator<<(std::ostream& out, const avlTree<T>& t)
	{
		treeAdd::indent_counter = 0;
		out << *t.root << std::endl;
		return out;
	}

	avlTree() = default;

	explicit avlTree(const T& data) : no_of_data(1)
	{
		root = new node<T>(data, nullptr, 0, maxDepth);
	}

	~avlTree()
	{
		if (root != nullptr)
			delete root;
	}

	/*    avlTree(const T *datas, size_t size) {
			for (size_t i = 0; i < size; i++) {
				unsigned level = 0;
				root->addData(datas[i], nullptr, level);
				m_level[level] += 1;
			}
		}*/

	void insert(const T& data)
	{
		if (root != nullptr)
		{
			root->addData(data, nullptr, 0, maxDepth);
			refreshroot();
		}
		else
		{
			root = new node<T>(data, nullptr, 0, maxDepth);
		}
		no_of_data++;
	}

	void insertarray(const T* datas, size_t size)
	{

		if (root != nullptr)
		{
			for (size_t i = 0; i < size; i++)
			{
				unsigned level = 0;
				root->addData(datas[i], nullptr, 0, maxDepth);
				refreshroot();
				no_of_data++;
			}
		}
		else
		{
			root = new node<T>(datas[0], nullptr, 0, maxDepth);
			no_of_data++;
			for (size_t i = 1; i < size; i++)
			{
				root->addData(datas[i], nullptr, 0, maxDepth);
				refreshroot();
				no_of_data++;
			}
		}
		refreshroot();
	}

	/*const T* getData(const T& data) {
		node<T>* result = root->search(data);
		if (result)
			return result->m_data;
		return nullptr;
	}

	const T* getLeftChild(const T& data) {
		node<T>* result = root->search(data);
		if (result)
			return result->leftChild->m_data;

		return nullptr;
	}

	const T* getRightChild(const T& data) {
		node<T>* result = root->search(data);
		if (result)
			return result->leftChild->m_data;

		return nullptr;
	}

	const T* getParent(const T& data) {
		node<T>* result = root->search(data);
		if (result)
			return result->parent->m_data;

		return nullptr;
	}*/

	void del(const T& data)
	{
		if (root == nullptr)
			return;

		node<T>* toremove = root->search(data);
		if (!toremove)
		{
			std::cout << "data not in Tree" << std::endl;
			return;
		}
		no_of_data--;

		if (toremove->getHeight() == 0)
		{
			if (toremove->parent)
			{
				if (toremove->parent->leftChild == toremove)
				{
					toremove->parent->leftChild = nullptr;
					toremove->parent->leftHeight--;
				}
				else
				{
					toremove->parent->rightChild = nullptr;
					toremove->parent->rightHeight--;
				}
				node<T>::refresh(toremove->parent, maxDepth);
				refreshroot();
			}
			else
			{
				root = nullptr;
			}

			toremove->parent = nullptr;

			delete toremove->m_data;
			return;
		}

		if (!toremove->leftChild)
		{

			if (toremove->parent)
			{
				if (toremove->parent->rightChild == toremove)
					toremove->parent->rightChild = toremove->rightChild;
				else
					toremove->parent->leftChild = toremove->rightChild;

				toremove->rightChild->parent = toremove->parent;
				toremove->rightChild->refreshDepth();
			}
			else
			{
				toremove->rightChild->parent = nullptr;
				root = toremove->rightChild;
			}

			node<T>::refresh(toremove->rightChild, maxDepth);
			toremove->rightChild = nullptr;
			toremove->parent = nullptr;
			refreshroot();

			delete toremove->m_data;
			return;
		}

		node<T>* smaller = toremove->leftChild->greatest();

		*toremove->m_data = *smaller->m_data;

		if (smaller->parent->leftChild == smaller)
		{
			smaller->parent->leftChild = smaller->leftChild; //smaller doesnot have a right child
			smaller->parent->leftHeight--;
		}

		else
		{
			smaller->parent->rightChild = smaller->leftChild; //smaller doesnot have a left child
			smaller->parent->rightHeight--;
		}

		if (smaller->leftChild)
		{
			smaller->leftChild->parent = smaller->parent;
			smaller->leftChild->refreshDepth();
			smaller->leftChild = nullptr;
		}
		node<T>::refresh(smaller->parent, maxDepth);

		smaller->parent = nullptr;
		refreshroot();
		delete smaller->m_data;
	}

	std::vector<T> getSortedArray()
	{
		std::vector<node<T>*> currentHead;
		currentHead.reserve(root->getHeight() + 1);
		for (unsigned i = 0; i < currentHead.capacity(); i++)
			currentHead.emplace_back(nullptr);

		std::vector<T> sortedData;
		sortedData.reserve(no_of_data);
		root->smallest()->traverse(sortedData, nullptr, root->greatest());
		return sortedData;
	}

	/*void changetest(const int &t){
		if (root!= nullptr){
			root->test=t;
		}
	}

	const int & gettest(){
		return root->test;
	}*/
};
