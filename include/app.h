#ifndef APP_H
#define APP_H
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include "states.h"
#include "window.h"
#include "sort.h"
#include "treeClass.h"

#include "glad/glad.h"
#include "SDL2/SDL_ttf.h"

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
		tree.viewport = &m_window->viewport;
		sort.width = m_window->viewport.w / sort.N * 0.95;
		sort.randomize(m_window->viewport);

		if (TTF_Init() == -1)
		{
			printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		}

		if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
		std::string path(SDL_GetBasePath());
		//printf("%s\n", path.append(+"../../../assets/segoeuib.ttf").c_str());
		loadfont({ "segoeui", 5 }, path.append(+"../../../assets/segoeuib.ttf").c_str());

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

		for (auto text : text_cache)
		{
			if (text.second != nullptr)
			{
				SDL_DestroyTexture(text.second);
			}
		}

		for (auto font : font_cache)
		{
			if (font.second != nullptr)
			{
				TTF_CloseFont(font.second);
			}
		}

		text_cache.clear();
		TTF_Quit();
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

			if (combo_selected < 7)
			{
				sort.Draw(m_state, m_window->gRenderer);
			}
			else
			{
				tree.draw(m_state, m_window->gRenderer, getText);
			}
			/*SDL_Texture* text = getText("hello world", "8bit", 80, 300, m_window->gRenderer);
			SDL_Texture* text1 = getText("hello world small", "8bit", 15, 150, m_window->gRenderer);
			SDL_Rect rect{ 200, 100, 200, 500 };

			SDL_QueryTexture(text, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(m_window->gRenderer, text, nullptr, &rect);

			rect.x = 500;
			SDL_QueryTexture(text1, nullptr, nullptr, &rect.w, &rect.h);
			SDL_RenderCopy(m_window->gRenderer, text1, nullptr, &rect);*/

			//IMGUI rendering
			{
				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplSDL2_NewFrame(m_window->gWindow);
				ImGui::NewFrame();

				{
					ImGui::Begin("test", NULL, ImGuiWindowFlags_NoMove);
					static float f = 0.0f;

					//float space_available = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

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

						if (combo_selected < 7)
						{
							sort.imguiDraw(m_state, combo_selected, m_window);
						}
						else
						{
							tree.imguiDraw(m_state, combo_selected, m_window);
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
					ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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

public:
	static SDL_Texture* createTextTexture(const std::string& fontname, std::string text, const int& size, const uint32_t& wrap, SDL_Renderer* renderer)
	{
		//SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), { 0, 0, 0 });
		SDL_Surface* surface;
		if (wrap)
			surface = TTF_RenderText_Blended_Wrapped(getfont(fontname, size), text.c_str(), { 0, 0, 0 }, wrap);
		else
			surface = TTF_RenderText_Blended(getfont(fontname, size), text.c_str(), { 0, 0, 0 });

		if (surface == nullptr)
		{
			printf("text render error : %s\n", TTF_GetError());
			return nullptr;
		}
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surface);

		if (tex == nullptr)
		{
			printf("text texture creation error : %s\n", SDL_GetError());
			return nullptr;
		}
		SDL_FreeSurface(surface);
		return tex;
	}

	static void loadfont(const fontinfo& info, const std::string& path)
	{
		if (font_cache[info] == nullptr)
		{
			font_cache[info] = TTF_OpenFont(path.c_str(), info.size);

			if (font_cache[info] == nullptr)
			{
				printf("error loading font %s : %s", path.c_str(), TTF_GetError());
				return;
			}
		}

		if (font_path.find(info.name) == font_path.end())
			font_path[info.name] = path;
	}

	static TTF_Font* getfont(const std::string& name, const int& size)
	{
		fontinfo key{ name, size };

		if (font_cache[key] != nullptr)
		{
			return font_cache[key];
		}
		if (font_path.find(name) == font_path.end())
		{
			printf("font by name %s cannot be found\n", name.c_str());
			return nullptr;
		}
		loadfont(key, font_path[name]);
		return font_cache[key];
	}

	static SDL_Texture* getText(std::string text, const std::string& fontname, const int& size, const uint32_t& wrap, SDL_Renderer* renderer)
	{
		std::string key = text + fontname + (char)size + (char)wrap;
		if (text_cache[key] == nullptr)
			text_cache[key] = createTextTexture(fontname, text, size, wrap, renderer);

		return text_cache[key];
	}

	static std::map<std::string, SDL_Texture*> text_cache;
	static std::map<fontinfo, TTF_Font*> font_cache;
	static std::map<std::string, std::string> font_path;

	appState m_state;
	Window* m_window;

	SDL_Event evnt;
	bool closeWindow = false;

	sortingClass sort;
	treeClass tree;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	std::vector<std::string> options{
		"selection sort",
		"quick sort",
		"Merge sort",
		"insertion sort",
		"bubble sort",
		"shell sort",
		"shell sort2",
		"avl Tree" };
	int combo_selected = 7;
};

std::map<std::string, SDL_Texture*> app::text_cache;
std::map<fontinfo, TTF_Font*> app::font_cache;
std::map<std::string, std::string> app::font_path;
#endif // APP_H
