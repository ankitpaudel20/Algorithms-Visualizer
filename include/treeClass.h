#pragma once

#include<thread>
#include "imgui.h"
#include "avl.h"
#include"common.h"

struct treeClass {
	avlTree<int> avl;
	float sleeptime = 0.05;
	vec2<float> dist = 50;
	float radius = 15;

	int insertBuffer;
	int deleteBuffer;



	SDL_Rect* viewport;
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
		}
		ImGui::SameLine();
		ImGui::Text(" | ");

		ImGui::SameLine();
		ImGui::InputInt("##delete", &deleteBuffer);
		ImGui::SameLine();
		if (ImGui::Button("Delete"))
		{
		}
		ImGui::PopItemWidth();
	}


	void draw(appState& state, SDL_Renderer* renderer) {
		SDL_RenderSetViewport(renderer, viewport);

		//SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
		//filledCircleColor(renderer, viewport->w / 2, viewport->h / 2, (uint16_t)radius, 0xffffffff);
		int a = 0;
		avl.draw(renderer, *viewport, (uint16_t)radius, dist);

	}

};