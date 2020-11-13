#pragma once

#include "SDL2/SDL.h"

struct Window
{

	bool ready = false;
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	SDL_GLContext gl_context;
	SDL_Rect viewport;

	Uint32 wwidth = 1180;
	Uint32 wheight = 600;

	Window(Uint32 width, Uint32 height) : wwidth(width), wheight(height)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			return;
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, wwidth, wheight, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
		SDL_SetWindowResizable(gWindow, SDL_TRUE);

		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return;
		}

		viewport = SDL_Rect{ 0, 50, (int)wwidth, (int)wheight - 50 };

		SDL_GL_SetSwapInterval(1); // Enable vsync
		gl_context = SDL_GL_CreateContext(gWindow);
		SDL_GL_MakeCurrent(gWindow, gl_context);

		SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
		gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
		if (gRenderer == NULL)
		{
			printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return;
		}
		// Initialize renderer color
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

		ready = true;
	}

	~Window()
	{
		SDL_GL_DeleteContext(gl_context);

		// Destroy window
		SDL_DestroyRenderer(gRenderer);
		SDL_DestroyWindow(gWindow);
		gWindow = nullptr;
		gRenderer = nullptr;

		// Quit SDL subsystems
		SDL_Quit();
	}
};