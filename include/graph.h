#pragma once
#include "common.h"
#include "heap.h"

#include<vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

#include "doublyLinked.h"
#include "bst.h"

std::vector<float> random_generator_float(float start, float end, uint32_t count)
{
	std::vector<float> out(count);
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<float> dist(start, end);
	for (unsigned i = 0; i < count; i++)
		out[i] = dist(mt);

	return out;
}


struct line {
	float x1, y1, x2, y2;
	Uint32 color = 0xFF0000FF;

	line(const float& _x1, const float& _y1, const float& _x2, const float& _y2) :x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};


struct graphNode {
	vec2<float> pos, origin;
	float radius;
	Uint32 color;

	int forest = -1;
	//std::vector<std::pair<graphNode*, Uint32>> childs;
	//std::vector<std::pair<graphNode*, Uint32>> parents;
};


struct graphNodePair {
	graphNode* l;
	graphNode* r;
	float d;
	graphNodePair(graphNode* lhs, graphNode* rhs) :l(lhs), r(rhs) {
		d = sqrtf((l->pos.x - r->pos.x) * (l->pos.x - r->pos.x) + (l->pos.y - r->pos.y) * (l->pos.y - r->pos.y));
	}
};

class graph {
	std::vector<graphNode> nodes;
	std::vector<line> lines;
	graphNode* start = nullptr;
	int N = 80, newN;
	Uint32 radius;

	std::mutex lineMutex;

	std::thread* thread = nullptr;
	bool done = false;



public:
	const SDL_Rect* viewport;
	float sleeptime = 0.15;

	graph(const SDL_Rect* _viewport) :viewport(_viewport) {
		radius = 4;
		init();
	}

	void init() {

		nodes.clear();
		lines.clear();

		nodes.reserve(N);
		lines.reserve(N - 1);
		newN = N;
		graphNode node;

		node.radius = radius;
		node.origin = vec2<float>(radius, radius);
		node.color = 0xFFFFFFFF;
		auto x = random_generator_float(viewport->x, viewport->w, N);
		auto y = random_generator_float(viewport->y, viewport->h, N);
		for (size_t i = 0; i < N; i++)
		{
			node.pos = vec2<float>(x[i], y[i]);
			nodes.push_back(node);
		}
	}

	inline bool isin(const int& mx, const int& my, const vec2<float>& pos, float& radius) {
		return vec2<float>::dist(vec2<float>(mx, my), pos) <= radius;
	}

	void select(const int& mx, const int& my) {
		if (mx <= viewport->x || mx >= viewport->x + viewport->w || my <= viewport->y || my >= viewport->y + viewport->h)
			return;

		if (start)
		{
			start->color = 0xFFFFFFFF;
		}
		start = nullptr;

		for (auto& i : nodes)
		{
			i.color = 0xFFFFFFFF;
			if (isin(mx, my, i.pos, i.radius))
			{
				start = &i;
				i.color = 0xFF0000FF;
				break;
				lines.clear();
			}
		}
	}

	void Draw(appState& state, SDL_Renderer* renderer) {
		if (!lines.empty())
		{

			lineMutex.lock();
			//uint8_t r = 255, g = 0, b = 0, a = 255;
			//SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
			for (auto& l : lines)
				//SDL_RenderDrawLineF(renderer, l.x1, l.y1, l.x2, l.y2);
				lineColor(renderer, l.x1, l.y1, l.x2, l.y2, l.color);

			lineMutex.unlock();
		}
		for (graphNode& i : nodes)
		{
			filledCircleColor(renderer, i.pos.x, i.pos.y, i.radius, i.color);
		}

		if (thread != nullptr)
		{
			if (done)
			{
				if (thread->joinable())
					thread->join();
				delete thread;
				thread = nullptr;
				state = appState::Idle;
				done = false;
			}
		}
	}

	void reset() {
		lines.clear();
		for (auto& i : nodes)
		{
			i.color = 0xFFFFFFFF;
		}
		if (start)
		{
			start->color = 0xFF0000FF;
		}
	}

	void imguiDraw(appState& state, int& combo_selected, Window* window) {
		ImGui::SameLine();
		ImGui::PushItemWidth((float)window->wwidth / 8);
		ImGui::InputInt(" | ##num", &newN);
		if (newN > 2)
		{
			N = newN;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		if (ImGui::Button("Reload"))
		{
			init();
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			reset();
		}
		ImGui::SameLine();
		if (ImGui::Button("Solve"))
		{
			switch (combo_selected)
			{
			case 11:
				thread = new std::thread(&graph::prims, this);
				//prims();
				break;
			case 12:
				thread = new std::thread(&graph::kruskal, this);
				break;
			default:
				break;
			}
			state = appState::Animating;
		}
	}

	void prims() {
		if (!start)
		{
			start = &nodes[0];
		}

		lines.clear();
		int time = sleeptime * 1000;


		std::vector<graphNode*> notVisited, visited;
		notVisited.reserve(nodes.size());
		visited.reserve(nodes.size());

		visited.push_back(start);
		start->radius *= 1.75;
		for (size_t i = 0; i < nodes.size(); i++)
		{
			if (&nodes[i] != start)
			{
				notVisited.push_back(&nodes[i]);
			}
		}
		size_t i, j;

		while (!notVisited.empty())
		{
			time = sleeptime * 1000;;
			float min = std::numeric_limits<float>::max(), d = 0;
			int minIndex1, minIndex2;
			float a;

			for (i = 0; i < visited.size(); i++)
			{
				for (j = 0; j < notVisited.size(); j++)
				{
					d = dist(visited[i]->pos.x, visited[i]->pos.y, notVisited[j]->pos.x, notVisited[j]->pos.y);
					if (d < min)
					{
						min = d;
						minIndex2 = j;
						minIndex1 = i;
					}
				}
				a = min + 1;
			}

			notVisited[minIndex2]->color = 0x95ff00FF;
			visited[minIndex1]->color = 0x0000FFFF;
			auto temp = visited[minIndex1]->radius;
			visited[minIndex1]->radius = visited[minIndex1] != start ? temp * 1.5 : temp;
			std::this_thread::sleep_for(std::chrono::milliseconds(time / 2));

			lineMutex.lock();
			lines.emplace_back(visited[minIndex1]->pos.x, visited[minIndex1]->pos.y, notVisited[minIndex2]->pos.x, notVisited[minIndex2]->pos.y);
			lineMutex.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(time / 2));

			visited.push_back(notVisited[minIndex2]);
			notVisited.erase(notVisited.begin() + minIndex2);


			visited[minIndex1]->color = visited[minIndex1] != start ? 0xFFFF00FF : 0xFF0000FF;
			visited[minIndex1]->radius = visited[minIndex1] != start ? temp : start->radius;
		}

		for (auto i : visited)
		{
			i->color = 0xFFFF00FF;
		}
		start->color = 0xFF0000FF;

		done = true;
	}

	void kruskal() {
		size_t i, j;
		float min = std::numeric_limits<float>::max(), d = 0;
		heap<graphNodePair> queue([](graphNodePair const& lhs, graphNodePair const& rhs) {return lhs.d < rhs.d; });
		int time = sleeptime * 1000;
		lines.clear();

		for (i = 0; i < nodes.size() - 1; i++)
		{
			for (j = i + 1; j < nodes.size(); j++)
			{
				queue.add(graphNodePair(&nodes[i], &nodes[j]));
			}
		}

		queue.remakeHeap();


		for (size_t i = 0; i < nodes.size(); i++)
		{
			nodes[i].forest = -1;
		}
		int id = 0;

		std::vector<linkedList<graphNode*>> trees;

		while (lines.size() < nodes.size() - 1) {
			auto min = queue.pop();

			if ((min.l->forest != min.r->forest) || (min.l->forest == -1 && min.r->forest == -1))
			{
				time = sleeptime * 1000;
				std::this_thread::sleep_for(std::chrono::milliseconds(time / 2));
				lineMutex.lock();
				lines.emplace_back(min.l->pos.x, min.l->pos.y, min.r->pos.x, min.r->pos.y);
				lineMutex.unlock();
				std::this_thread::sleep_for(std::chrono::milliseconds(time / 2));

				if (min.l->forest != -1)
				{
					if (min.r->forest != -1)
					{
						auto it = trees[min.l->forest].itLast();
						trees[min.l->forest].mergeList(trees[min.r->forest]);
						for (it; it != trees[min.l->forest].end(); it++)
							it.operator*()->forest = min.l->forest;
					}
					else
					{
						min.r->forest = min.l->forest;
						trees[min.l->forest].insertLast(min.r);
					}
				}
				else if (min.r->forest != -1) {

					if (min.l->forest != -1)
					{
						auto it = trees[min.r->forest].itLast();
						trees[min.r->forest].mergeList(trees[min.l->forest]);
						for (it; it != trees[min.r->forest].end(); it++)
							it.operator*()->forest = min.r->forest;
					}
					else
					{
						min.l->forest = min.r->forest;
						trees[min.r->forest].insertLast(min.l);
					}
				}
				else
				{
					min.r->forest = min.l->forest = id++;
					trees.emplace_back(linkedList<graphNode*>{ min.r, min.l });
				}
			}
		}

		done = true;
	}

};