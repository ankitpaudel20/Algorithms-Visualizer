#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <cassert>
#include <algorithm>


// insert
// find
// height



template <class T>
struct bstNode
{
	struct rotation
	{
		static bstNode<T>* rr(bstNode<T>* node1)
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

		static bstNode<T>* lr(bstNode<T>* node1)
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

		static bstNode<T>* rl(bstNode<T>* node1)
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

		static bstNode<T>* ll(bstNode<T>* node1)
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

	T* m_data = nullptr;
	unsigned int leftHeight = 0;
	unsigned int rightHeight = 0;
	unsigned depth;

	bstNode<T>* leftChild = nullptr;
	bstNode<T>* rightChild = nullptr;
	bstNode<T>* parent = nullptr;

	static void refresh(bstNode<T>* Node, uint32_t& maxdepth)
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
		bstNode<T>* rootpos;
		if (!Node->parent)
		{
			rootpos = Node;
		}

		auto newCurrentNode = Node->balance();

		if (newCurrentNode != Node)
		{
			maxdepth = newCurrentNode->refreshDepth();

		}
		if (newCurrentNode == nullptr)
			return;

		refresh(newCurrentNode->parent, maxdepth);
	}


	uint32_t refreshDepth()
	{
		uint32_t max, temp = 0;
		if (!parent)
			depth = 0;
		else
		{
			depth = parent->depth + 1;
		}

		max = depth;

		if (leftChild)
			temp = leftChild->refreshDepth();
		max = max < temp ? temp : max;
		if (rightChild)
			temp = rightChild->refreshDepth();
		return max < temp ? temp : max;
	}

	bstNode<T>* balance()
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

	unsigned getHeight()
	{
		return std::max(rightHeight, leftHeight);
	}

	bstNode(const T& data, bstNode<T>* parentptr, const uint32_t& _depth, uint32_t& maxdepth)
	{
		parent = parentptr;
		depth = _depth;
		maxdepth = depth;
		m_data = new T;
		memcpy(m_data, &data, sizeof(T));
	}

	void addData(const T& data, const uint32_t& _depth, uint32_t& maxdepth)
	{
		if (data > * m_data)
		{
			if (rightChild != nullptr)
				rightChild->addData(data, _depth + 1, maxdepth);
			else
			{
				rightChild = new bstNode<T>(data, this, _depth + 1, maxdepth);

				refresh(this->rightChild, maxdepth);
			}
		}
		else
		{
			if (leftChild != nullptr)
				leftChild->addData(data, _depth + 1, maxdepth);
			else
			{

				leftChild = new bstNode<T>(data, this, _depth + 1, maxdepth);

				refresh(this->leftChild, maxdepth);
			}
		}


	}


	bstNode<T>* search(const T& data)
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

	bstNode<T>* greatest()
	{
		if (rightChild == nullptr)
			return this;
		else
			return rightChild->greatest();
	};

	~bstNode()
	{
		if (rightChild != nullptr)
			delete rightChild;
		if (leftChild != nullptr)
			delete leftChild;
		if (m_data != nullptr)
			delete m_data;
	}

	bool operator>(const bstNode& rhs) const { return m_data > rhs.m_data; }

	bool operator>(const T& rhs) const { return m_data > m_data; }

	bool operator<(const bstNode& rhs) const { return m_data < rhs.m_data; }

	bool operator>=(const bstNode& rhs) const { return m_data >= rhs.m_data; }

	bool operator<=(const bstNode& rhs) const { return m_data <= rhs.m_data; }

	bool operator==(const bstNode& rhs) const { return m_data == rhs.m_data; }

	friend std::ostream& operator<<(std::ostream& out, const bstNode<T>& n)
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
};

template <class T>
class bst
{

private:
	bstNode<T>* root = nullptr;
	unsigned no_of_data = 0;
	uint32_t maxDepth;


	void refreshroot()
	{
		while (root->parent != nullptr)
			root = root->parent;
	}

public:
	bst() {}

	uint32_t size() { return no_of_data; }

	friend std::ostream& operator<<(std::ostream& out, const bst<T>& t)
	{
		treeAdd::indent_counter = 0;
		out << *t.root << std::endl;
		return out;
	}

	~bst()
	{
		if (root != nullptr)
			delete root;
	}

	void insert(const T& data)
	{

		if (root != nullptr)
		{
			auto previousheight = root->getHeight();
			root->addData(data, 0, maxDepth);
			refreshroot();
		}
		else
		{
			root = new bstNode<T>(data, nullptr, 0, maxDepth);
		}
		no_of_data++;
	}

	bool del(const T& data)
	{
		if (root == nullptr)
			return false;

		bstNode<T>* toremove = root->search(data);
		if (!toremove)
		{
			//std::cout << "data not in Tree" << std::endl;
			return false;
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
				bstNode<T>::refresh(toremove->parent, maxDepth);
				refreshroot();
			}
			else
			{
				root = nullptr;
			}

			toremove->parent = nullptr;
			delete toremove;

			return true;
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

			bstNode<T>::refresh(toremove->rightChild, maxDepth);
			toremove->rightChild = nullptr;
			toremove->parent = nullptr;
			refreshroot();


			delete toremove;

			return true;
		}

		bstNode<T>* smaller = toremove->leftChild->greatest();

		*toremove->m_data = *smaller->m_data;

		if (smaller->parent->leftChild == smaller)
		{
			smaller->parent->leftChild = smaller->leftChild; //smaller doesnot have a right child
			smaller->parent->leftHeight--;
		}
		else
		{
			smaller->parent->rightChild = smaller->leftChild; //smaller doesnot have a right child
			smaller->parent->rightHeight--;
		}

		if (smaller->leftChild)
		{
			smaller->leftChild->parent = smaller->parent;
			smaller->leftChild->refreshDepth();
			smaller->leftChild = nullptr;
		}
		bstNode<T>::refresh(smaller->parent, maxDepth);
		refreshroot();

		smaller->parent = nullptr;
		delete smaller;
		return true;
	}

	bool find(const T& data) {
		return root->search(data);
	}

	std::vector<T> getSortedArray()
	{
		std::vector<bstNode<T>*> currentHead;
		currentHead.reserve(root->getHeight() + 1);
		for (unsigned i = 0; i < currentHead.capacity(); i++)
			currentHead.emplace_back(nullptr);

		std::vector<T> sortedData;
		sortedData.reserve(no_of_data);
		root->smallest()->traverse(sortedData, nullptr, root->greatest());
		return sortedData;
	}
};
