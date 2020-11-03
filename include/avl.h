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


struct shared_data {
	std::mutex nmutex;
	std::vector<Circle> nodes;
	float* deltatime;
	float* sleeptime;
};

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

	Circle circle;
	//SDL_Rect circle.texPos;

	T* m_data = nullptr;
	unsigned int leftHeight = 0;
	unsigned int rightHeight = 0;
	unsigned depth;

	node<T>* leftChild = nullptr;
	node<T>* rightChild = nullptr;
	node<T>* parent = nullptr;



	static void refresh(node<T>* Node, uint32_t& maxdepth, const vec2<float>& dist)
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
		node<T>* rootpos;
		if (!Node->parent)
		{
			rootpos = Node;
		}

		auto newCurrentNode = Node->balance();

		if (newCurrentNode != Node) {
			maxdepth = newCurrentNode->refreshDepth();
			if (!newCurrentNode->parent) {
				newCurrentNode->circle.pos = rootpos->circle.pos;
				newCurrentNode->circle.texPos = rootpos->circle.texPos;
				newCurrentNode->circle.calculateTexPos(newCurrentNode->circle.letter);
			}
			newCurrentNode->refreshPos(maxdepth, dist);
		}
		if (newCurrentNode == nullptr)
			return;

		refresh(newCurrentNode->parent, maxdepth, dist);
	}

	void refreshPos(const uint32_t& maxdepth, const vec2<float>& dist) {
		if (parent)
		{
			if (parent->rightChild == this)
			{
				circle.pos.x = parent->circle.pos.x + (uint16_t)(pow(2, maxdepth - depth) * (dist.x / 2));
				circle.pos.y = parent->circle.pos.y + dist.y;
				circle.texPos.x = circle.pos.x - circle.texPos.w / 2;
				circle.texPos.y = circle.pos.y - circle.texPos.h / 2;
			}
			if (parent->leftChild == this)
			{
				circle.pos.x = parent->circle.pos.x - (uint16_t)(pow(2, maxdepth - depth) * (dist.x / 2));
				circle.pos.y = parent->circle.pos.y + dist.y;
				circle.texPos.x = circle.pos.x - circle.texPos.w / 2;
				circle.texPos.y = circle.pos.y - circle.texPos.h / 2;
			}
		}
		if (leftChild)
			leftChild->refreshPos(maxdepth, dist);
		if (rightChild)
			rightChild->refreshPos(maxdepth, dist);
	}


	uint32_t refreshDepth()
	{
		uint32_t max, temp = 0;
		if (!parent)
			depth = 0;
		else {
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

	void addData(const T& data, const uint32_t& _depth, uint32_t& maxdepth, SDL_Texture* letters, const vec2<float>& dist, shared_data& shared)
	{
		shared.nmutex.lock();
		Circle temp;
		temp.pos = circle.pos;
		temp.calculateTexPos(circle.letter);
		shared.nodes.emplace_back(temp);
		shared.nmutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds((uint32_t)(*shared.sleeptime * 1000)));


		shared.nmutex.lock();
		shared.nodes.clear();
		shared.nmutex.unlock();

		if (data > * m_data)
		{
			if (rightChild != nullptr)
				rightChild->addData(data, _depth + 1, maxdepth, letters, dist, shared);
			else
			{
				rightChild = new node<T>(data, this, _depth + 1, maxdepth);
				rightChild->circle.pos = vec2<float>(circle.pos.x + (float)(pow(2, maxdepth - depth - 1) * dist.x / 2), circle.pos.y + dist.y);
				rightChild->circle.letter = letters;
				SDL_QueryTexture(rightChild->circle.letter, nullptr, nullptr, &rightChild->circle.texPos.w, &rightChild->circle.texPos.h);

				rightChild->circle.texPos.x = rightChild->circle.pos.x - rightChild->circle.texPos.w / 2;
				rightChild->circle.texPos.y = rightChild->circle.pos.y - rightChild->circle.texPos.h / 2;
				refresh(this->rightChild, maxdepth, dist);
			}
		}
		else
		{
			if (leftChild != nullptr)
				leftChild->addData(data, _depth + 1, maxdepth, letters, dist, shared);
			else
			{

				leftChild = new node<T>(data, this, _depth + 1, maxdepth);
				leftChild->circle.pos = vec2<float>(circle.pos.x - (float)(pow(2, maxdepth - depth - 1) * dist.x / 2), circle.pos.y + dist.y);
				leftChild->circle.letter = letters;
				SDL_QueryTexture(leftChild->circle.letter, nullptr, nullptr, &leftChild->circle.texPos.w, &leftChild->circle.texPos.h);

				leftChild->circle.texPos.x = leftChild->circle.pos.x - leftChild->circle.texPos.w / 2;
				leftChild->circle.texPos.y = leftChild->circle.pos.y - leftChild->circle.texPos.h / 2;

				shared.nmutex.lock();
				shared.nodes.push_back(leftChild->circle);
				shared.nmutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds((uint32_t)(*shared.sleeptime * 500)));

				refresh(this->leftChild, maxdepth, dist);

			}
		}

		shared.nmutex.lock();
		temp.pos = circle.pos;
		temp.calculateTexPos(circle.letter);
		shared.nodes.emplace_back(temp);
		shared.nmutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds((uint32_t)(*shared.sleeptime * 1000)));


		shared.nmutex.lock();
		shared.nodes.clear();
		shared.nmutex.unlock();
	}


	void addData2(const T& data, const uint32_t& _depth, uint32_t& maxdepth, SDL_Texture* letters, const vec2<float>& dist)
	{
		if (data > * m_data)
		{
			if (rightChild != nullptr)
				rightChild->addData2(data, _depth + 1, maxdepth, letters, dist);
			else
			{
				rightChild = new node<T>(data, this, _depth + 1, maxdepth);
				rightChild->circle.pos = vec2<float>(circle.pos.x + (float)(pow(2, maxdepth - depth - 1) * dist.x / 2), circle.pos.y + dist.y);
				rightChild->circle.letter = letters;
				SDL_QueryTexture(rightChild->circle.letter, nullptr, nullptr, &rightChild->circle.texPos.w, &rightChild->circle.texPos.h);

				rightChild->circle.texPos.x = rightChild->circle.pos.x - rightChild->circle.texPos.w / 2;
				rightChild->circle.texPos.y = rightChild->circle.pos.y - rightChild->circle.texPos.h / 2;
				refresh(this->rightChild, maxdepth, dist);
			}
		}
		else
		{
			if (leftChild != nullptr)
				leftChild->addData2(data, _depth + 1, maxdepth, letters, dist);
			else
			{

				leftChild = new node<T>(data, this, _depth + 1, maxdepth);
				leftChild->circle.pos = vec2<float>(circle.pos.x - (float)(pow(2, maxdepth - depth - 1) * dist.x / 2), circle.pos.y + dist.y);
				leftChild->circle.letter = letters;
				SDL_QueryTexture(leftChild->circle.letter, nullptr, nullptr, &leftChild->circle.texPos.w, &leftChild->circle.texPos.h);

				leftChild->circle.texPos.x = leftChild->circle.pos.x - leftChild->circle.texPos.w / 2;
				leftChild->circle.texPos.y = leftChild->circle.pos.y - leftChild->circle.texPos.h / 2;
				refresh(this->leftChild, maxdepth, dist);

			}
		}
	}

	node<T>* search(const T& data, shared_data& shared)
	{
		shared.nmutex.lock();
		Circle temp;
		temp.pos = circle.pos;
		temp.calculateTexPos(circle.letter);
		shared.nodes.emplace_back(temp);
		shared.nmutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds((uint32_t)(*shared.sleeptime * 1000)));


		shared.nmutex.lock();
		shared.nodes.clear();
		shared.nmutex.unlock();

		if (*m_data == data)
			return this;
		else if (data > * m_data)
		{
			if (!rightChild)
				return nullptr;
			return rightChild->search(data, shared);
		}
		else
		{
			if (!leftChild)
				return nullptr;
			return leftChild->search(data, shared);
		}

		shared.nmutex.lock();
		temp.pos = circle.pos;
		temp.calculateTexPos(circle.letter);
		shared.nodes.emplace_back(temp);
		shared.nmutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds((uint32_t)(*shared.sleeptime * 1000)));

		shared.nmutex.lock();
		shared.nodes.clear();
		shared.nmutex.unlock();

	}

	node<T>* greatest(shared_data& shared)
	{
		shared.nmutex.lock();
		Circle temp;
		temp.pos = circle.pos;
		temp.calculateTexPos(circle.letter);
		shared.nodes.emplace_back(temp);
		shared.nmutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds((uint32_t)(*shared.sleeptime * 1000)));

		shared.nmutex.lock();
		shared.nodes.pop_back();
		shared.nmutex.unlock();

		if (rightChild == nullptr)
			return this;
		else
			return rightChild->greatest(shared);
	};


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

		//parpos.x = parpos.x - pow(2, maxdepth - 1) * (pow(2, depth) - 1) * (dist.x / 2));

		if (parent->leftChild == this)
			parpos.x = parpos.x - (float)(pow(2, maxdepth - depth) * (dist.x / 2));
		else
			parpos.x = parpos.x + (float)(pow(2, maxdepth - depth) * (dist.x / 2));

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
			SDL_RenderDrawLine(renderer, parpos.x, parpos.y, parpos.x - (float)pow(2, maxdepth - depth) * dist.x / 2, parpos.y + dist.y);
			parpos.x = parpos.x - (float)(pow(2, maxdepth - depth) * (dist.x / 2));
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			SDL_RenderDrawLine(renderer, parpos.x, parpos.y, parpos.x + (float)pow(2, maxdepth - depth) * dist.x / 2, parpos.y + dist.y);
			parpos.x = parpos.x + (float)(pow(2, maxdepth - depth) * (dist.x / 2));
		}
		parpos.y = parpos.y + dist.y;

		if (leftChild)
			leftChild->drawLines(renderer, parpos, radius, dist, maxdepth, getText);
		if (rightChild)
			rightChild->drawLines(renderer, parpos, radius, dist, maxdepth, getText);
	}

	void draw(SDL_Renderer* renderer, const float& radius, SDL_Texture* (*getText)(std::string, const std::string&, const int&, const uint32_t&, SDL_Renderer*)) {
		filledCircleColor(renderer, circle.pos.x, circle.pos.y, radius, 0xFFFFFFFF);
		SDL_RenderCopy(renderer, circle.letter, nullptr, &circle.texPos);


		SDL_Texture* text = getText(std::to_string(leftHeight), "segoeui", 10, 0, renderer);
		SDL_Rect rect;
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = circle.pos.x - rect.w / 4 - radius;
		rect.y = circle.pos.y - rect.h / 4 - radius;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		text = getText(std::to_string(rightHeight), "segoeui", 10, 0, renderer);
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = rect.x + 2 * radius - rect.w / 4;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		if (leftChild) {
			//SDL_RenderDrawLine(renderer, circle.pos.x, circle.pos.y, leftChild->circle.pos.x, leftChild->circle.pos.y);
			leftChild->draw(renderer, radius, getText);
		}
		if (rightChild) {
			//SDL_RenderDrawLine(renderer, circle.pos.x, circle.pos.y, rightChild->circle.pos.x, rightChild->circle.pos.y);
			rightChild->draw(renderer, radius, getText);
		}
	}

	void drawLines2(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

		if (leftChild)
		{
			SDL_RenderDrawLine(renderer, circle.pos.x, circle.pos.y, leftChild->circle.pos.x, leftChild->circle.pos.y);
			leftChild->drawLines2(renderer);
		}
		if (rightChild)
		{
			SDL_RenderDrawLine(renderer, circle.pos.x, circle.pos.y, rightChild->circle.pos.x, rightChild->circle.pos.y);
			rightChild->drawLines2(renderer);
		}

	}
};





template <class T>
class avlTree
{

private:
	node<T>* root = nullptr;
	unsigned no_of_data = 0;
	uint32_t maxDepth;

	vec2<float> dist = 75;
	float radius = 25;


	int insertBuffer = 0;
	int deleteBuffer = 0;


	bool done = false;

	std::thread* thread = nullptr;

	SDL_Texture* (*getText)(std::string, const std::string&, const int&, const uint32_t&, SDL_Renderer*);


	/// <summary>
	/// for working with sleeptimes sliders
	/// </summary>
	const float flow = 0.2f;
	const float fhigh = 5.0f;



	void refreshroot()
	{
		while (root->parent != nullptr)
			root = root->parent;
	}

public:
	const SDL_Rect* viewport;

	shared_data shared;

	float sleeptime = 0.05;


	avlTree(SDL_Texture* (*_getText)(std::string, const std::string&, const int&, const uint32_t&, SDL_Renderer*)) :getText(_getText) { shared.sleeptime = &sleeptime; }

	void imguiDraw(appState& state, int& combo_selected, Window* window) {
		ImGui::SameLine();

		ImGui::Text("debug");
		ImGui::SameLine();

		ImGui::PushItemWidth((float)window->wwidth / 13);

		ImGui::InputInt("##insert", &insertBuffer);
		ImGui::SameLine();
		if (ImGui::Button("Insert"))
		{
			SDL_Texture* letters = getText(std::to_string(insertBuffer), "segoeui", 15, 0, window->gRenderer);
			thread = new std::thread(&avlTree::insert, this, insertBuffer, letters, std::ref(shared));
			state = appState::Animating;
			//insert(insertBuffer, letters);
		}
		ImGui::SameLine();
		ImGui::Text(" | ");

		ImGui::SameLine();
		ImGui::InputInt("##delete", &deleteBuffer);
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			SDL_Texture* letters = getText(std::to_string(insertBuffer), "segoeui", 15, 0, window->gRenderer);
			thread = new std::thread(&avlTree::del, this, deleteBuffer, std::ref(shared));
			state = appState::Animating;
			//del(deleteBuffer, shared);
		}
		ImGui::SameLine();
		if (ImGui::Button("break"))
		{
			printf("breaking");
		}
		ImGui::SameLine();
		ImGui::PopItemWidth();



		shared.nmutex.lock();
		for (auto i : shared.nodes)
		{
			ImGui::Text("its pos is ( %f, %f )  its vel is ( %f, %f )  target vel is ( %f, %f )", i.pos.x, i.pos.y, i.vel.x, i.vel.y, i.targetVel.x, i.targetVel.y);
		}
		ImGui::Text("delta time is %f", *shared.deltatime);

		shared.nmutex.unlock();
	}

	void draw(appState& state, SDL_Renderer* renderer)
	{
		if (!root)
			return;

		root->drawLines2(renderer);
		root->draw(renderer, radius, getText);

		if (thread != nullptr)
		{
			if (done)
			{
				thread->join();
				delete thread;
				thread = nullptr;
				done = false;
				state = appState::Idle;
			}
			else
			{
				shared.nmutex.lock();
				for (auto i : shared.nodes)
				{
					filledCircleRGBA(renderer, (uint16_t)i.pos.x, (uint16_t)i.pos.y, radius, 0, 255, 0, 255);
					SDL_RenderCopy(renderer, i.letter, nullptr, &i.texPos);
				}
				shared.nmutex.unlock();
			}
		}

		/*if (root->leftChild)
			root->leftChild->drawLines(renderer, vec2<uint32_t>(viewport.w / 4, 100), radius, dist, maxDepth, getText);
		if (root->rightChild)
			root->rightChild->drawLines(renderer, vec2<uint32_t>(viewport.w / 4, 100), radius, dist, maxDepth, getText);

		filledCircleColor(renderer, viewport.w / 4, 100, radius, 0xFFFFFFFF);

		SDL_Texture* text = getText(std::to_string(*root->m_data), "segoeui", 15, 0, renderer);
		SDL_Rect rect;
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = viewport.w / 4 - rect.w / 2;
		rect.y = 100 - rect.h / 2;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		text = getText(std::to_string(root->leftHeight), "segoeui", 10, 0, renderer);
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = viewport.w / 4 - rect.w / 4 - radius;
		rect.y = 100 - rect.h / 4 - radius;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		text = getText(std::to_string(root->rightHeight), "segoeui", 10, 0, renderer);
		SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
		rect.x = rect.x + 2 * radius - rect.w / 4;
		SDL_RenderCopy(renderer, text, nullptr, &rect);

		vec2<uint32_t> pos(viewport.w, 0);
		if (root->leftChild)
			root->leftChild->drawCircle(renderer, vec2<uint32_t>(viewport.w / 4, 100), radius, dist, maxDepth, getText);
		if (root->rightChild)
			root->rightChild->drawCircle(renderer, vec2<uint32_t>(viewport.w / 4, 100), radius, dist, maxDepth, getText);*/
	}

	uint32_t size() { return no_of_data; }
	friend std::ostream& operator<<(std::ostream& out, const avlTree<T>& t)
	{
		treeAdd::indent_counter = 0;
		out << *t.root << std::endl;
		return out;
	}

	~avlTree()
	{
		if (root != nullptr)
			delete root;
	}


	void insert(const T& data, SDL_Texture* letters, shared_data& shared)
	{
		if (root != nullptr)
		{
			auto previousheight = root->getHeight();
			root->addData(data, 0, maxDepth, letters, dist, shared);
			refreshroot();
			if (root->getHeight() > previousheight)
			{
				root->refreshPos(maxDepth, dist);
			}
		}
		else
		{
			root = new node<T>(data, nullptr, 0, maxDepth);

			root->circle.pos = vec2<float>(viewport->w / 2, 100);
			root->circle.letter = letters;
			SDL_QueryTexture(root->circle.letter, nullptr, nullptr, &root->circle.texPos.w, &root->circle.texPos.h);

			root->circle.texPos.x = root->circle.pos.x - root->circle.texPos.w / 2;
			root->circle.texPos.y = root->circle.pos.y - root->circle.texPos.h / 2;
		}
		no_of_data++;

		shared.nmutex.lock();
		shared.nodes.clear();
		shared.nmutex.unlock();
		done = true;
	}

	void insertarray(const T* datas, size_t size, SDL_Renderer* renderer)
	{
		if (root != nullptr)
		{
			for (size_t i = 0; i < size; i++)
			{
				SDL_Texture* letters = getText(std::to_string(datas[i]), "segoeui", 15, 0, renderer);
				root->addData2(datas[i], 0, maxDepth, letters, dist);
				refreshroot();
				no_of_data++;
			}
		}
		else
		{
			root = new node<T>(datas[0], nullptr, 0, maxDepth);
			no_of_data++;

			root->circle.pos = vec2<float>(viewport->w / 2, 100);
			root->circle.letter = getText(std::to_string(*root->m_data), "segoeui", 15, 0, renderer);
			SDL_QueryTexture(root->circle.letter, nullptr, nullptr, &root->circle.texPos.w, &root->circle.texPos.h);

			root->circle.texPos.x = root->circle.pos.x - root->circle.texPos.w / 2;
			root->circle.texPos.y = root->circle.pos.y - root->circle.texPos.h / 2;

			for (size_t i = 1; i < size; i++)
			{
				SDL_Texture* letters = getText(std::to_string(datas[i]), "segoeui", 15, 0, renderer);
				root->addData2(datas[i], 0, maxDepth, letters, dist);
				refreshroot();
				no_of_data++;
				root->refreshPos(maxDepth, dist);
			}
		}
		refreshroot();
	}

	void refreshpos() {

		root->circle.pos = vec2<float>(viewport->w / 2, 100);
		SDL_QueryTexture(root->circle.letter, nullptr, nullptr, &root->circle.texPos.w, &root->circle.texPos.h);

		root->circle.texPos.x = root->circle.pos.x - root->circle.texPos.w / 2;
		root->circle.texPos.y = root->circle.pos.y - root->circle.texPos.h / 2;

		root->refreshPos(maxDepth, dist);
	}

	void del(const T& data, shared_data& shared)
	{
		if (root == nullptr)
			return;

		node<T>* toremove = root->search(data, shared);
		if (!toremove)
		{
			std::cout << "data not in Tree" << std::endl;
			return;
		}
		no_of_data--;

		shared.nmutex.lock();
		Circle temp;
		temp.pos = toremove->circle.pos;
		temp.calculateTexPos(toremove->circle.letter);
		shared.nodes.emplace_back(temp);
		shared.nmutex.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds((uint32_t)(*shared.sleeptime * 1000)));




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
				node<T>::refresh(toremove->parent, maxDepth, dist);
				refreshroot();
			}
			else
			{
				root = nullptr;
			}

			toremove->parent = nullptr;
			root->refreshPos(maxDepth, dist);
			delete toremove;
			done = true;

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

			node<T>::refresh(toremove->rightChild, maxDepth, dist);
			toremove->rightChild = nullptr;
			toremove->parent = nullptr;
			refreshroot();

			root->refreshPos(maxDepth, dist);

			delete toremove;
			done = true;

			return;
		}

		node<T>* smaller = toremove->leftChild->greatest(shared);

		*toremove->m_data = *smaller->m_data;
		toremove->circle.letter = smaller->circle.letter;
		toremove->circle.texPos.w = smaller->circle.texPos.w;
		toremove->circle.texPos.h = smaller->circle.texPos.h;

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
		node<T>::refresh(smaller->parent, maxDepth, dist);
		refreshroot();

		root->refreshPos(maxDepth, dist);
		smaller->parent = nullptr;
		shared.nmutex.lock();
		shared.nodes.clear();
		shared.nmutex.unlock();

		delete smaller;
		done = true;

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

};
