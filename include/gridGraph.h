#pragma once
#include "common.h"
#include "heap.h"
#include "stack.h"

#include<vector>
#include<stack>
#include "SDL2/SDL.h"
#include "SDL2/SDL2_gfxPrimitives.h"

typedef enum {
	NORMAL = 0x0001, WALL = 0x0002, START = 0x0004, END = 0x0008
} nodeProps;

class gridGraph {
	std::vector<gridGraphNode> grid;
	vec2<int> gridSize, newSize;
	float rectsize = 20;
	bool fill = true;
	bool diagonal = false;
	heap<gridGraphNode*> check_queue;


	gridGraphNode* start = nullptr;
	gridGraphNode* end = nullptr;
	Uint32 startx, starty, endx, endy;

	std::thread* thread = nullptr;
	bool done = false;



	const SDL_Rect* viewport;
	inline gridGraphNode& get(const Uint32& x, const Uint32& y) { return grid[x + y * gridSize.x]; }
	inline gridGraphNode& get(const vec2<float>& pos) {
		return grid[((Uint32)pos.x - startx) / rectsize + ((Uint32)pos.y - starty) / rectsize * gridSize.x];
	}

public:
	float sleeptime = 0.05;

	gridGraph(const SDL_Rect* _viewport) :viewport(_viewport), check_queue([](gridGraphNode* const& lhs, gridGraphNode* const& rhs) {return (lhs->H + lhs->G) < (rhs->H + rhs->G); }) {
		newSize.x = viewport->w / rectsize - 1;
		newSize.y = viewport->h / rectsize - 1;
		init();
		startx = get(0, 0).pos.x - get(0, 0).origin.x;
		starty = get(0, 0).pos.y - get(0, 0).origin.y;
		endx = get(gridSize.x - 1, 0).pos.x + get(gridSize.x - 1, 0).origin.x;
		endy = get(0, gridSize.y - 1).pos.y + get(0, gridSize.y - 1).origin.y;
	}

	void init() {
		gridSize = newSize;
		newSize = newSize;
		grid.clear();
		grid.reserve(gridSize.x * gridSize.y);
		gridGraphNode rect;
		start = nullptr;
		end = nullptr;
		for (size_t i = 0; i < gridSize.y; i++) {
			for (size_t j = 0; j < gridSize.x; j++) {
				rect.color = 0x706AFFFF;
				rect.size = vec2<float>(rectsize);
				rect.origin = vec2<float>(rectsize / 2);
				rect.pos = vec2<float>((j + 1) * rectsize + viewport->x, (i + 1) * rectsize + viewport->y);
				rect.isWall = false;
				rect.visited = false;

				grid.push_back(rect);
			}
		}

		startx = get(0, 0).pos.x - get(0, 0).origin.x;
		starty = get(0, 0).pos.y - get(0, 0).origin.y;
		endx = get(gridSize.x - 1, 0).pos.x + get(gridSize.x - 1, 0).origin.x;
		endy = get(0, gridSize.y - 1).pos.y + get(0, gridSize.y - 1).origin.y;
	}

	std::vector<gridGraphNode*> getneighbours(const gridGraphNode* const curr) {
		std::vector<gridGraphNode*> ret;
		int x, y;

		if (curr->pos.x - 2 * rectsize > startx) {
			x = (curr->pos.x - 2 * rectsize - startx) / rectsize;
			y = (curr->pos.y - starty) / rectsize;
			if (!get(x, y).visited)
			{
				ret.push_back(&get(x, y));
			}
		}

		if (curr->pos.x + 2 * rectsize < endx) {

			x = (curr->pos.x + 2 * rectsize - startx) / rectsize;
			y = (curr->pos.y - starty) / rectsize;
			if (!get(x, y).visited)
			{
				ret.push_back(&get(x, y));
			}
		}

		if (curr->pos.y - 2 * rectsize > starty) {
			x = (curr->pos.x - startx) / rectsize;
			y = (curr->pos.y - 2 * rectsize - starty) / rectsize;
			if (!get(x, y).visited)
			{
				ret.push_back(&get(x, y));
			}
		}

		if (curr->pos.y + 2 * rectsize < endy) {
			x = (curr->pos.x - startx) / rectsize;
			y = (curr->pos.y + 2 * rectsize - starty) / rectsize;
			if (!get(x, y).visited)
			{
				ret.push_back(&get(x, y));
			}
		}
		return ret;
	}

	void genmaze() {


		for (auto& i : grid) {
			i.isWall = false;
			i.color = 0x706AFFFF;
		}

		gridGraphNode* curr = &grid[0];
		std::vector<gridGraphNode*> visited;
		std::stack<gridGraphNode*> st;

		time_t seconds;
		time(&seconds);
		srand((unsigned int)seconds);


		while (visited.size() < grid.size())
		{
			curr->isWall = true;
			visited.push_back(curr);
			st.push(curr);
			auto n = getneighbours(curr);
			curr++;
			/*if (!n.empty())
			{
				curr = n[0];
			}
			else {
				while (getneighbours(curr).empty())
				{
					curr = st.top();
					st.pop();
				}
			}*/
			if (n.size() > 1)
			{
				auto random = rand() % n.size();
				curr = n[random];
			}
			else if (n.size() == 1)
			{
				curr = n[0];
			}
			else
			{
				while (getneighbours(curr).empty())
				{
					//curr = st.pop();
					curr = st.top();
					st.pop();
				}
			}
		}

		for (auto& i : grid) {
			if (i.isWall)
			{
				i.color = 0xF852FFFF;
			}
		}

		return;
	}

	void expandNode(gridGraphNode* node, const vec2<int>& delpos, const float& weight) {
		int index = delpos.x + gridSize.x * delpos.y;
		if (!(node + index)->visited)
		{
			(node + index)->parent = node;
			(node + index)->color = 0x5DEAD0FF;
			(node + index)->calculateHL(end, weight);
			check_queue.push(node + index);
			(node + index)->visited = true;
		}
		else if ((node + index) != start)
		{
			float G = (node + index)->G;
			(node + index)->updateHL(end, node, weight);
			if (G != (node + index)->G)
				check_queue.remakeHeap();
		}
	}

	void solveAstar() {
		check_queue.changeComp([](gridGraphNode* const& lhs, gridGraphNode* const& rhs) {
			return ((lhs->H + lhs->G) == (rhs->H + rhs->G)) ? lhs->H < rhs->H : ((lhs->H + lhs->G) < (rhs->H + rhs->G));
			});

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
			i.G = std::numeric_limits<double>::infinity();
			i.H = std::numeric_limits<double>::infinity();
		}


		gridGraphNode* current = start;
		current->G = 0;
		current->H = vec2<float>::dist(current->pos, end->pos);
		current->visited = true;
		check_queue.push(current);
		int a = sleeptime * 1000;

		bool inLeft, inRight, inTop, inBottom;
		auto root = sqrt(2);

		do {
			a = sleeptime * 1000;
			current = check_queue.pop();

			if (current == end) {
				found = true;
				if (current->parent) {
					current->color = 0x00FF00FF;
					current = current->parent;
				}
				do {
					a = sleeptime * 1000;
					current->color = 0xFFFFFFFF;			//white color
					current = current->parent;
					std::this_thread::sleep_for(std::chrono::milliseconds(a));
				} while (current != start);

				current->color = 0x0000FFFF;
				break;
			}

			current->color = (current != start) ? 0x000000FF : current->color;

			inLeft = current->pos.x - rectsize > startx;
			inRight = current->pos.x + rectsize < endx;
			inTop = current->pos.y - rectsize > starty;
			inBottom = current->pos.y + rectsize < endy;

			std::this_thread::sleep_for(std::chrono::milliseconds(a));

			if (inRight && !(current + 1)->isWall)
				expandNode(current, vec2<int>(1, 0), rectsize);

			if (inLeft && !(current - 1)->isWall)
				expandNode(current, vec2<int>(-1, 0), rectsize);

			if (inBottom && !(current + gridSize.x)->isWall)
				expandNode(current, vec2<int>(0, 1), rectsize);

			if (inTop && !(current - gridSize.x)->isWall)
				expandNode(current, vec2<int>(0, -1), rectsize);

			if (diagonal)
			{
				if (inRight && inBottom && !(current + 1 + gridSize.x)->isWall)
					expandNode(current, vec2<int>(1, 1), root * rectsize);

				if (inRight && inTop && !(current + 1 - gridSize.x)->isWall)
					expandNode(current, vec2<int>(1, -1), root * rectsize);

				if (inLeft && inBottom && !(current - 1 + gridSize.x)->isWall)
					expandNode(current, vec2<int>(-1, 1), root * rectsize);

				if (inLeft && inTop && !(current - 1 - gridSize.x)->isWall)
					expandNode(current, vec2<int>(-1, -1), root * rectsize);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(a));
			current->color = (current != start) ? 0xA6A938FF : current->color;

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

		while (!check_queue.empty())
			check_queue.pop();


		done = true;
	}

	void solveBestFirst() {
		check_queue.changeComp([](gridGraphNode* const& lhs, gridGraphNode* const& rhs) {return (lhs->H) < (rhs->H); });

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
			i.G = std::numeric_limits<double>::infinity();
			i.H = std::numeric_limits<double>::infinity();
		}


		gridGraphNode* current = start;
		current->G = 0;
		current->H = vec2<float>::dist(current->pos, end->pos);
		current->visited = true;
		check_queue.push(current);
		int a = sleeptime * 1000;

		bool inLeft, inRight, inTop, inBottom;
		auto root = sqrt(2);

		do {
			a = sleeptime * 1000;



			current = check_queue.pop();

			if (current == end) {
				found = true;
				if (current->parent) {
					current->color = 0x00FF00FF;
					current = current->parent;
				}
				do {
					a = sleeptime * 1000;
					current->color = 0xFFFFFFFF;			//white color
					current = current->parent;
					std::this_thread::sleep_for(std::chrono::milliseconds(a));
				} while (current != start);

				current->color = 0x0000FFFF;
				break;
			}

			current->color = (current != start) ? 0x000000FF : current->color;

			inLeft = current->pos.x - rectsize > startx;
			inRight = current->pos.x + rectsize < endx;
			inTop = current->pos.y - rectsize > starty;
			inBottom = current->pos.y + rectsize < endy;

			std::this_thread::sleep_for(std::chrono::milliseconds(a));

			if (inRight && !(current + 1)->isWall)
				expandNode(current, vec2<int>(1, 0), rectsize);

			if (inLeft && !(current - 1)->isWall)
				expandNode(current, vec2<int>(-1, 0), rectsize);

			if (inBottom && !(current + gridSize.x)->isWall)
				expandNode(current, vec2<int>(0, 1), rectsize);

			if (inTop && !(current - gridSize.x)->isWall)
				expandNode(current, vec2<int>(0, -1), rectsize);

			if (diagonal)
			{
				if (inRight && inBottom && !(current + 1 + gridSize.x)->isWall)
					expandNode(current, vec2<int>(1, 1), root * rectsize);

				if (inRight && inTop && !(current + 1 - gridSize.x)->isWall)
					expandNode(current, vec2<int>(1, -1), root * rectsize);

				if (inLeft && inBottom && !(current - 1 + gridSize.x)->isWall)
					expandNode(current, vec2<int>(-1, 1), root * rectsize);

				if (inLeft && inTop && !(current - 1 - gridSize.x)->isWall)
					expandNode(current, vec2<int>(-1, -1), root * rectsize);
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(a));
			current->color = (current != start) ? 0xA6A938FF : current->color;
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

		while (!check_queue.empty())
			check_queue.pop();


		done = true;
	}

	void solveDijkstra() {

		//checkQueue
		check_queue.changeComp([](gridGraphNode* const& lhs, gridGraphNode* const& rhs) {return (lhs->G) < (rhs->G); });


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
			i.G = std::numeric_limits<double>::infinity();
			i.H = std::numeric_limits<double>::infinity();
		}


		gridGraphNode* current = start;
		current->G = 0;
		current->H = vec2<float>::dist(current->pos, end->pos);
		current->visited = true;
		check_queue.push(current);
		int a = sleeptime * 1000;

		bool inLeft, inRight, inTop, inBottom;
		auto root = sqrt(2);

		do {
			a = sleeptime * 1000;
			current = check_queue.pop();

			if (current == end) {
				found = true;
				if (current->parent) {
					current->color = 0x00FF00FF;
					current = current->parent;
				}
				do {
					a = sleeptime * 1000;
					current->color = 0xFFFFFFFF;			//white color
					current = current->parent;
					std::this_thread::sleep_for(std::chrono::milliseconds(a));
				} while (current != start);

				current->color = 0x0000FFFF;
				break;
			}

			current->color = (current != start) ? 0x000000FF : current->color;

			inLeft = current->pos.x - rectsize > startx;
			inRight = current->pos.x + rectsize < endx;
			inTop = current->pos.y - rectsize > starty;
			inBottom = current->pos.y + rectsize < endy;

			std::this_thread::sleep_for(std::chrono::milliseconds(a));


			if (inRight && !(current + 1)->isWall)
				expandNode(current, vec2<int>(1, 0), rectsize);

			if (inLeft && !(current - 1)->isWall)
				expandNode(current, vec2<int>(-1, 0), rectsize);

			if (inBottom && !(current + gridSize.x)->isWall)
				expandNode(current, vec2<int>(0, 1), rectsize);

			if (inTop && !(current - gridSize.x)->isWall)
				expandNode(current, vec2<int>(0, -1), rectsize);

			if (diagonal)
			{
				if (inRight && inBottom && !(current + 1 + gridSize.x)->isWall)
					expandNode(current, vec2<int>(1, 1), root * rectsize);

				if (inRight && inTop && !(current + 1 - gridSize.x)->isWall)
					expandNode(current, vec2<int>(1, -1), root * rectsize);

				if (inLeft && inBottom && !(current - 1 + gridSize.x)->isWall)
					expandNode(current, vec2<int>(-1, 1), root * rectsize);

				if (inLeft && inTop && !(current - 1 - gridSize.x)->isWall)
					expandNode(current, vec2<int>(-1, -1), root * rectsize);
			}


			std::this_thread::sleep_for(std::chrono::milliseconds(a));
			current->color = (current != start) ? 0xA6A938FF : current->color;
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

		while (!check_queue.empty())
			check_queue.pop();


		done = true;
	}

	void Draw(appState& state, SDL_Renderer* renderer, clock_type& stop) {
		for (auto i : grid)
			i.draw(renderer, fill);
		glad_glLineWidth(1);
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

		for (auto& i : grid)
		{
			if (i.color == 0xFFFFFFFF || i.color == 0x00ff00ff)
			{
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0xFF);
				if (i.parent != nullptr) {
					SDL_RenderDrawLine(renderer, i.pos.x, i.pos.y, i.parent->pos.x, i.parent->pos.y);
				}
			}
		}



		if (thread != nullptr)
		{
			if (done)
			{
				stop = std::chrono::high_resolution_clock::now();
				if (thread->joinable())
					thread->join();
				delete thread;
				thread = nullptr;
				state = appState::Idle;
				done = false;
			}
		}

	}

	void refreshSize() {
		newSize.x = viewport->w / rectsize - 1;
		newSize.y = viewport->h / rectsize - 1;
	}

	void color(const int& mx, const int& my, const Uint8& flags) {
		if (!flags)
			return;

		if (mx <= startx || mx >= endx || my <= starty || my >= endy)
			return;


		int x2 = (mx - startx) / rectsize;
		int y2 = (my - starty) / rectsize;


		if (flags & (1 << 0))
		{
			get(x2, y2).color = 0xF852FFFF;

			if (&get(x2, y2) == start)
				start = nullptr;
			else if (&get(x2, y2) == end)
				end = nullptr;

			get(x2, y2).isWall = true;

		}
		else if (flags & (1 << 1))
		{
			get(x2, y2).color = 0x706AFFFF;

			if (&get(x2, y2) == start)
				start = nullptr;
			else if (&get(x2, y2) == end)
				end = nullptr;

			get(x2, y2).isWall = false;
		}
		else if (flags & (1 << 2))
		{
			if (!start)
			{
				get(x2, y2).color = 0x0000FFFF;
				start = &get(x2, y2);
				get(x2, y2).isWall = false;
			}
		}
		else if (flags & (1 << 3))
		{
			if (!end)
			{
				get(x2, y2).color = 0x00FF00FF;
				end = &get(x2, y2);
				get(x2, y2).isWall = false;
			}
		}
	}

	void reset() {
		//start = end = nullptr;
		for (auto& i : grid)
		{
			if (!i.isWall)
			{
				i.color = 0x706AFFFF;
				i.parent = nullptr;
			}
		}
		if (start)
			start->color = 0x0000FFFF;
		if (end)
			end->color = 0x00FF00FF;
	}

	void randomfill() {

		time_t seconds;
		time(&seconds);
		srand((unsigned int)seconds);
		bool random = false;

		for (auto& i : grid)
		{
			random = 0 + (rand() % (1 - 0 + 1)) == 1;
			i.isWall = random;
			if (i.isWall)
				i.color = 0xF852FFFF;
			else
				i.color = 0x706AFFFF;
			i.visited = false;
		}
		start = end = nullptr;
	}

	void imguiDraw(appState& state, int& combo_selected, Window* window, clock_type& start) {

		ImGui::PushItemWidth((float)window->wwidth / 8);
		ImGui::SameLine();
		ImGui::Text("cell Size");
		ImGui::SameLine();
		ImGui::InputFloat("##rectsize", &rectsize);
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
			init();
		}
		ImGui::SameLine();
		if (ImGui::Button("reset"))
		{
			reset();
		}
		ImGui::SameLine();
		if (ImGui::Button("Random"))
		{
			randomfill();
		}
		//ImGui::SameLine();
		if (ImGui::Button("Solve"))
		{
			reset();
			switch (combo_selected)
			{
			case 8:
				thread = new std::thread(&gridGraph::solveAstar, this);
				break;
			case 9:
				thread = new std::thread(&gridGraph::solveBestFirst, this);
				break;
			case 10:
				thread = new std::thread(&gridGraph::solveDijkstra, this);
				break;
			default:
				break;
			}

			//thread = new std::thread(&gridGraph::solveDijkstra, this);
			//solve();
			state = appState::Animating;
			start = std::chrono::high_resolution_clock::now();
		}

		ImGui::SameLine();
		ImGui::Checkbox("Fill? ", &fill);
		ImGui::SameLine();
		ImGui::Checkbox("use diagonal? ", &diagonal);

	}

};