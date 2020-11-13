#pragma once
#include "common.h"
#include "heap.h"

#include<vector>
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"


typedef enum {
	NORMAL = 0x0001, WALL = 0x0002, START = 0x0004, END = 0x0008
} nodeProps;



class aStar {
	std::vector<graphNode> grid;
	vec2<int> gridSize, newSize;
	float rectsize = 20;
	bool fill = true;

	heap<graphNode*> check_queue;


	graphNode* start = nullptr;
	graphNode* end = nullptr;
	std::thread* thread = nullptr;
	bool done = false;


	const SDL_Rect* viewport;
	inline graphNode& get(const Uint32& x, const Uint32& y) { return grid[x + y * gridSize.x]; }
	inline graphNode& get(const vec2<float>& pos) { return grid[(Uint32)pos.x / rectsize + (Uint32)pos.y / rectsize * gridSize.x]; }

	//bool cmp(const graphNode* lhs, const graphNode* rhs) { return lhs->H + lhs->G < rhs->H + rhs->G; }
	bool cmp(const vec2<float>& lhs, const vec2<float>& rhs) {
		auto L = get(lhs);
		auto R = get(rhs);
		return (L.H + L.G) < (R.H + R.G);
	}

public:
	float sleeptime = 0.05;

	aStar(const SDL_Rect* _viewport) :viewport(_viewport), check_queue([](graphNode* const& lhs, graphNode* const& rhs) {return (lhs->H + lhs->G) < (rhs->H + rhs->G); }) {
		init(vec2<int>(viewport->w / rectsize - 1, viewport->h / rectsize - 1));
	}


	void solve() {

		if (!(start && end))
		{
			done = true;
			return;
		}

		bool found = false;

		for (auto& i : grid)
		{
			i.parent = nullptr;
			i.visited = false;
			i.G = 0;
			i.H = 0;
		}

		/*	std::vector<graphNode*> nVisited;
			nVisited.resize(grid.size());*/

		graphNode* current = start;
		current->G = 0;
		current->H = vec2<float>::dist(current->pos, end->pos);
		current->visited = true;
		check_queue.push(current);

		Uint32 startx = get(0, 0).pos.x - get(0, 0).origin.x;
		Uint32 starty = get(0, 0).pos.y - get(0, 0).origin.y;
		Uint32 endx = get(gridSize.x - 1, 0).pos.x + get(gridSize.x - 1, 0).origin.x;
		Uint32 endy = get(0, gridSize.y - 1).pos.y + get(0, gridSize.y - 1).origin.y;

		do {
			int a = sleeptime * 1000;
			current = check_queue.pop();

			if (current == end) {
				found = true;
				if (current->parent) {
					current->color = 0x00FF00FF;
					current = current->parent;
				}
				do {
					current->color = 0xFFFFFFFF;			//white color
					current = current->parent;
				} while (current != start);

				current->color = 0x0000FFFF;
				break;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(a));

			bool added = false;

			if (current->pos.x + rectsize < endx)
			{
				if (!(current + 1)->isWall && !(current + 1)->visited)
				{
					(current + 1)->parent = current;
					(current + 1)->calculateHL(end);
					(current + 1)->color = 0x5DEAD0FF;
					check_queue.push(current + 1);
					(current + 1)->visited = true;
					added = true;
				}
			}

			if (current->pos.x - rectsize > startx)
			{
				if (!(current - 1)->isWall && !(current - 1)->visited)
				{
					(current - 1)->parent = current;
					(current - 1)->calculateHL(end);
					(current - 1)->color = 0x5DEAD0FF;
					check_queue.push(current - 1);
					(current - 1)->visited = true; added = true;
				}
			}

			if (current->pos.y + rectsize < endy)
			{
				if (!(current + gridSize.x)->isWall && !(current + gridSize.x)->visited)
				{
					(current + gridSize.x)->parent = current;
					(current + gridSize.x)->calculateHL(end);
					(current + gridSize.x)->color = 0x5DEAD0FF;
					check_queue.push(current + gridSize.x);
					(current + gridSize.x)->visited = true; added = true;
				}
			}

			if (current->pos.y - rectsize > starty)
			{
				if (!(current - gridSize.x)->isWall && !(current - gridSize.x)->visited)
				{
					(current - gridSize.x)->parent = current;
					(current - gridSize.x)->calculateHL(end);
					(current - gridSize.x)->color = 0x5DEAD0FF;
					check_queue.push(current - gridSize.x);
					(current - gridSize.x)->visited = true; added = true;
				}
			}
			if (!added)
			{
				//__debugbreak();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(a));
			current->color = 0xA6A938FF;
			std::this_thread::sleep_for(std::chrono::milliseconds(a));
		} while (!check_queue.empty());

		if (!found)
		{
			printf("it cannot be found");
			while (current != start) {
				current->color = 0xFFFFFFFF;			//white color
				current = current->parent;
			}
		}



		done = true;
	}

	void Draw(appState& state, SDL_Renderer* renderer) {
		for (auto i : grid)
			i.draw(renderer, fill);

		if (fill)
		{
			vlineRGBA(renderer, get(0, 0).pos.x - get(0, 0).origin.x, get(0, 0).pos.y - get(0, 0).origin.y, get(0, gridSize.y - 1).pos.y + get(0, gridSize.y - 1).origin.y, 0, 0, 0, 255);
			for (size_t i = 0; i < gridSize.x; i++)
			{
				vlineRGBA(renderer, get(i, 0).pos.x + get(i, 0).origin.x, get(i, 0).pos.y - get(i, 0).origin.y, get(i, gridSize.y - 1).pos.y + get(i, gridSize.y - 1).origin.y, 0, 0, 0, 255);
			}
			hlineRGBA(renderer, get(0, 0).pos.x - get(0, 0).origin.x, get(gridSize.x - 1, 0).pos.x + get(gridSize.x - 1, 0).origin.x, get(0, 0).pos.y - get(0, 0).origin.y, 0, 0, 0, 255);
			for (size_t i = 0; i < gridSize.y; i++)
			{
				hlineRGBA(renderer, get(0, i).pos.x - get(0, i).origin.x, get(gridSize.x - 1, i).pos.x + get(gridSize.x - 1, i).origin.x, get(0, i).pos.y + get(0, i).origin.y, 0, 0, 0, 255);
			}
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

	void init(const vec2<int>& newsize) {
		gridSize = newsize;
		newSize = newsize;
		grid.clear();
		grid.reserve(gridSize.x * gridSize.y);
		graphNode rect;
		start = nullptr;
		end = nullptr;
		for (size_t i = 0; i < gridSize.y; i++) {
			for (size_t j = 0; j < gridSize.x; j++) {
				rect.color = 0xFFFF00FF;
				rect.size = vec2<float>(rectsize);
				rect.origin = vec2<float>(rectsize / 2);
				rect.pos = vec2<float>((j + 1) * rectsize + viewport->x, (i + 1) * rectsize + viewport->y);
				rect.isWall = false;
				rect.visited = false;

				grid.push_back(rect);
			}
		}
	}

	void color(const int& mx, const int& my, const Uint8& flags) {
		if (!flags)
			return;


		Uint32 startx = get(0, 0).pos.x - get(0, 0).origin.x;
		Uint32 starty = get(0, 0).pos.y - get(0, 0).origin.y;
		Uint32 endx = get(gridSize.x - 1, 0).pos.x + get(gridSize.x - 1, 0).origin.x;
		Uint32 endy = get(0, gridSize.y - 1).pos.y + get(0, gridSize.y - 1).origin.y;

		if (mx <= startx || mx >= endx || my <= starty || my >= endy)
			return;


		int x2 = (mx - startx) / rectsize;
		int y2 = (my - starty) / rectsize;


		if (flags & (1 << 0))
		{
			get(x2, y2).color = 0xFF0000FF;
			//get(x2, y2).property = 0x00;

			if (&get(x2, y2) == start)
				start = nullptr;
			else if (&get(x2, y2) == end)
				end = nullptr;

			//get(x2, y2).property |= nodeProps::WALL;
			get(x2, y2).isWall = true;
		}
		else if (flags & (1 << 1))
		{
			get(x2, y2).color = 0xFFFF00FF;
			//get(x2, y2).property = 0x00;

			if (&get(x2, y2) == start)
				start = nullptr;
			else if (&get(x2, y2) == end)
				end = nullptr;

			get(x2, y2).isWall = false;

			//get(x2, y2).property |= nodeProps::NORMAL;
		}
		else if (flags & (1 << 2))
		{
			if (!start)
			{
				/*get(x2, y2).property = 0x00;
				get(x2, y2).property |= nodeProps::START;
				get(x2, y2).property |= nodeProps::NORMAL;*/
				get(x2, y2).color = 0x0000FFFF;
				start = &get(x2, y2);
				get(x2, y2).isWall = false;
			}
		}
		else if (flags & (1 << 3))
		{
			if (!end)
			{
				/*get(x2, y2).property = 0x00;
				get(x2, y2).property |= nodeProps::END;
				get(x2, y2).property |= nodeProps::NORMAL;*/
				get(x2, y2).color = 0x00FF00FF;
				end = &get(x2, y2);
				get(x2, y2).isWall = false;
			}
		}
	}

	void reset() {
		start = end = nullptr;
		for (auto& i : grid)
		{
			if (!i.isWall)
			{
				i.color = 0xFFFF00FF;
				i.parent = nullptr;
			}
		}
	}

	void imguiDraw(appState& state, int& combo_selected, Window* window) {

		ImGui::PushItemWidth((float)window->wwidth / 8);
		ImGui::SameLine();
		ImGui::Text("grid Size");
		ImGui::SameLine();
		ImGui::SliderInt(" | ##gsx", &newSize.x, 5, viewport->w / rectsize - 1);
		ImGui::SameLine();
		ImGui::SliderInt(" | ##gsy", &newSize.y, 5, viewport->h / rectsize - 1);
		ImGui::SameLine();
		ImGui::PopItemWidth();

		if (ImGui::Button("Reload"))
		{
			init(newSize);
		}

		ImGui::SameLine();
		if (ImGui::Button("reset"))
		{
			reset();
		}
		ImGui::SameLine();
		if (ImGui::Button("Solve"))
		{
			thread = new std::thread(&aStar::solve, this);
			//solve();
			state = appState::Animating;
		}
		ImGui::SameLine();
		ImGui::Checkbox("Fill? ", &fill);
	}
};