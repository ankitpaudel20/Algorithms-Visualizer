#ifndef APP_H
#define APP_H
#include <iostream>
#include <vector>
#include "states.h"
#include "window.h"
#include "sort.h"

#include "glad/glad.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

class app
{
public:
	app()
	{
		m_window = new Window(1180, 600);
		sort.viewport = &m_window->viewport;
		sort.width = m_window->viewport.w / sort.N * 0.95;
		sort.randomize(m_window->viewport);

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplSDL2_InitForOpenGL(m_window->gWindow, m_window->gl_context);
		ImGui_ImplOpenGL3_Init("#version 130");

		m_state = appState::Idle;
	}

	~app()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL2_Shutdown();
		ImGui::DestroyContext();
	}

	void run()
	{
		SDL_ShowWindow(m_window->gWindow);

		bool listGrayed, stopButtonGrayed;
		static float flow = 0.0f;
		static float fhigh = 0.3f;
		while (!closeWindow)
		{
			while (SDL_PollEvent(&evnt) != 0)
			{
				ImGui_ImplSDL2_ProcessEvent(&evnt);
				if (evnt.type == SDL_QUIT)
					closeWindow = true;

				else if (evnt.type == SDL_KEYDOWN)
				{
					switch (evnt.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						closeWindow = true;
						break;
					}
				}
				else if (evnt.type == SDL_WINDOWEVENT)
				{
					switch (evnt.window.event)
					{

					case SDL_WINDOWEVENT_RESIZED:
						m_window->wwidth = evnt.window.data1;
						m_window->wheight = evnt.window.data2;
						m_window->viewport.w = evnt.window.data1;
						m_window->viewport.h = evnt.window.data2 - 50;
						sort.width = evnt.window.data1 / sort.N * 0.95;
						sort.spacing = (float)m_window->viewport.w / ((sort.N - 1) * 2);
						break;
					case SDL_WINDOWEVENT_MINIMIZED:
						SDL_Log("Window %d minimized", evnt.window.windowID);
						break;
					case SDL_WINDOWEVENT_MAXIMIZED:
						SDL_Log("Window %d maximized", evnt.window.windowID);
						break;
					case SDL_WINDOWEVENT_RESTORED:
						SDL_Log("Window %d restored", evnt.window.windowID);
						break;
					}
				}
			}

			sort.Draw(m_state, m_window->gRenderer);

			//IMGUI rendering
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplSDL2_NewFrame(m_window->gWindow);
				ImGui::NewFrame();

				{
					ImGui::Begin("test", NULL, ImGuiWindowFlags_NoMove);
					static float f = 0.0f;

					float space_available = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

					{
						if (m_state == appState::Animating)
						{
							ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
							ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							listGrayed = true;
						}
						else
							listGrayed = false;

						ImGui::PushItemWidth((float)m_window->wwidth / 5);
						ImGui::SameLine();
						ImGui::Text("Select Mode :");
						ImGui::SameLine();
						if (ImGui::BeginCombo(" | ##modelist", options[combo_selected].c_str()))
						{
							for (int n = 0; n < options.size(); n++)
							{
								const bool is_selected = (combo_selected == n);
								if (ImGui::Selectable(options[n].c_str(), is_selected))
									combo_selected = n;
							}
							ImGui::EndCombo();
						}
						ImGui::PopItemWidth();

						ImGui::PushItemWidth((float)m_window->wwidth / 13);
						ImGui::SameLine();
						ImGui::Text("Number of datas");
						ImGui::SameLine();
						ImGui::InputInt(" | ##no", &sort.N);
						if (sort.N < 3)
							sort.N = 3;
						ImGui::PopItemWidth();

						ImGui::PushItemWidth((float)m_window->wwidth / 5);
						ImGui::SameLine();
						ImGui::Text("bar Width");
						ImGui::SameLine();
						ImGui::SliderInt(" | ##bw", &sort.width, 1, m_window->viewport.w / (sort.N - 1));
						ImGui::SameLine();
						ImGui::PopItemWidth();

						if (ImGui::Button("Reload"))
						{
							sort.randomize(m_window->viewport);
							//sort.width = m_window->wwidth / sort.N * 0.95;
						}
						ImGui::SameLine();

						if (ImGui::Button("GO"))
						{
							sort.run(combo_selected);
							m_state = appState::Animating;
						}

						if (listGrayed)
						{
							ImGui::PopItemFlag();
							ImGui::PopStyleVar();
						}
					}
					//stop button
					/*ImGui::SameLine();
					{
						if (m_state == appState::Idle)
						{
							ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
							ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
							stopButtonGrayed = true;
						}
						else
							stopButtonGrayed = false;

						if (ImGui::Button("STOP!!!"))
						{
							m_state = appState::Idle;
						}
						if (stopButtonGrayed)
						{
							ImGui::PopItemFlag();
							ImGui::PopStyleVar();
						}
					}*/

					ImGui::PushItemWidth(200);
					ImGui::SliderScalar("sleep time", ImGuiDataType_Float, &sort.sleeptime, &flow, &fhigh);
					ImGui::PopItemWidth();

					ImGui::Text("sleep time :%f", sort.sleeptime);
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
					ImGui::Text("swap Pos: %d ,%d", sort.swapPos[0], sort.swapPos[1]);
					ImGui::Text("copy Pos: %d ,%d", sort.copyPos[0], sort.copyPos[1]);
					ImGui::Text("comp Pos: %d ,%d", sort.compPos[0], sort.compPos[1]);

					ImGui::SetWindowSize(ImVec2(m_window->wwidth, ImGui::GetWindowSize().y));
					ImGui::SetWindowPos(ImVec2(0, 0), false);
					ImGui::End();
				}

				// update screen

				ImGui::Render();
				// SDL_SetRenderDrawColor(gRenderer, clear_color.x, clear_color.y, clear_color.z, clear_color.w);
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			}

			SDL_RenderPresent(m_window->gRenderer);
			glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
			glClear(GL_COLOR_BUFFER_BIT);
		}
	}

private:
	void drawRectoutline(const float &pos, const uint32_t &height)
	{
		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(m_window->gRenderer, &r, &g, &b, &a);
		SDL_RenderSetViewport(m_window->gRenderer, &m_window->viewport);
		SDL_SetRenderDrawColor(m_window->gRenderer, 0xFF, 0x00, 0x00, 0xFF);
		SDL_FRect temp{pos - sort.width / 2, m_window->viewport.h - height, sort.width, height};
		SDL_RenderDrawRectF(m_window->gRenderer, &temp);
		SDL_SetRenderDrawColor(m_window->gRenderer, r, g, b, a);
	}

	appState m_state;
	Window *m_window;

	SDL_Event evnt;
	bool closeWindow = false;

	sortingClass sort;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::vector<std::string> options{
		"selection sort",
		"quick sort",
		"Merge sort",
		"insertion sort",
		"bubble sort",
	};
	int combo_selected = 0;
};

#endif // APP_H
