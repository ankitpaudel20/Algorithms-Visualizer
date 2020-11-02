#pragma once

#include<thread>
#include "imgui.h"
#include "avl.h"
#include"common.h"

class app;

struct treeClass {
	avlTree<int> avl;
	float sleeptime = 0.05;
	vec2<float> dist = 75;
	float radius = 25;

	int insertBuffer = 0;
	int deleteBuffer = 0;

	SDL_Rect* viewport;

	bool done = false;
	enum class state {
		insert, remove, idle
	};
	state treeState = state::idle;

	std::thread* thread = nullptr;

	treeClass() {
		std::vector<int> datas{ 10, 50, -10, 60, 125, 48, 54, -6, 0, 25 };
		avl.insertarray(&datas[0], datas.size());
	}

	void imguiDraw(appState& state, int& combo_selected, Window* window) {
		ImGui::SameLine();
		ImGui::PushItemWidth((float)window->wwidth / 13);

		ImGui::InputInt("##insert", &insertBuffer);
		ImGui::SameLine();
		if (ImGui::Button("Insert"))
		{
			avl.insert(insertBuffer);
		}
		ImGui::SameLine();
		ImGui::Text(" | ");

		ImGui::SameLine();
		ImGui::InputInt("##delete", &deleteBuffer);
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
			avl.del(deleteBuffer);
			/*avl.del(54);
			avl.del(125);*/
			/*avl.del(48);
			avl.del(54);
			avl.del(60);
			avl.del(-6);
			avl.del(0);
			avl.del(-10);*/
		}
		ImGui::PopItemWidth();
	}


	void draw(appState& appstate, SDL_Renderer* renderer, SDL_Texture* (*getText)(std::string, const std::string&, const int&, const uint32_t&, SDL_Renderer*)) {
		SDL_RenderSetViewport(renderer, viewport);

		//SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		//filledCircleColor(renderer, viewport->w / 2, viewport->h / 2, (uint16_t)radius, 0xffffffff);
		int a = 0;
		if (thread != nullptr)
		{
			SDL_Texture* text;
			SDL_Rect rect{ 20, viewport->h / 2, 200, 500 };

			if (treeState == state::insert)
			{
				text = getText(std::to_string(insertBuffer), "segoeui", 13, 0, renderer);
			}
			else
			{
				text = getText(std::to_string(deleteBuffer), "segoeui", 13, 0, renderer);
			}

			SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(renderer, text, nullptr, &rect);

		}

		avl.draw(renderer, *viewport, (uint16_t)radius, dist, getText);
	}

};