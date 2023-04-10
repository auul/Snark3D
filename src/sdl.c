#include <snark.h>

#define SDL_INIT_FLAGS \
	(SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS)
#define SDL_WIN_FLAGS (SDL_WINDOW_INPUT_GRABBED)
#define SDL_RENDER_FLAGS (SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE)

SDL_Window *win;
SDL_Renderer *render;
SDL_Texture *render_buffer;

SDL_Texture *wall_light;
SDL_Texture *wall_dark;

int WIN_WIDTH;
int WIN_HEIGHT;
bool win_run;

uint32_t PIXEL_FORMAT;
uint8_t BITS_PER_PIXEL;
uint8_t BYTES_PER_PIXEL;
uint32_t RMASK;
uint32_t GMASK;
uint32_t BMASK;
uint32_t AMASK;

bool WinExit(void)
{
	if (wall_light) {
		SDL_DestroyTexture(wall_light);
		wall_light = NULL;
	}

	if (wall_dark) {
		SDL_DestroyTexture(wall_dark);
		wall_dark = NULL;
	}

	if (render_buffer) {
		SDL_DestroyTexture(render_buffer);
		render_buffer = NULL;
	}

	if (render) {
		SDL_DestroyRenderer(render);
		render = NULL;
	}

	if (win) {
		SDL_DestroyWindow(win);
		win = NULL;
	}

	return false;
}

bool WinInit(void)
{
	if (SDL_Init(SDL_INIT_FLAGS) != 0) {
		error(SDL_ERROR, SDL_GetError());
		return WinExit();
	}

	win = SDL_CreateWindow(WIN_INIT_TITLE,
	                       SDL_WINDOWPOS_UNDEFINED,
	                       SDL_WINDOWPOS_UNDEFINED,
	                       WIN_INIT_WIDTH,
	                       WIN_INIT_HEIGHT,
	                       SDL_WIN_FLAGS);
	if (!win) {
		error(SDL_ERROR, SDL_GetError());
		return WinExit();
	}

	WIN_WIDTH  = WIN_INIT_WIDTH;
	WIN_HEIGHT = WIN_INIT_HEIGHT;

	render = SDL_CreateRenderer(win, -1, SDL_RENDER_FLAGS);
	if (!render) {
		error(SDL_ERROR, SDL_GetError());
		return WinExit();
	}

	SDL_Surface *win_surface = SDL_GetWindowSurface(win);
	if (!win_surface) {
		error(SDL_ERROR, SDL_GetError());
		return WinExit();
	}

	PIXEL_FORMAT    = win_surface->format->format;
	BITS_PER_PIXEL  = win_surface->format->BitsPerPixel;
	BYTES_PER_PIXEL = win_surface->format->BytesPerPixel;
	RMASK           = win_surface->format->Rmask;
	GMASK           = win_surface->format->Gmask;
	BMASK           = win_surface->format->Bmask;
	AMASK           = win_surface->format->Amask;

	SDL_Texture *render_buffer = SDL_CreateTexture(render,
	                                               PIXEL_FORMAT,
	                                               SDL_TEXTUREACCESS_TARGET,
	                                               RES_WIDTH,
	                                               RES_HEIGHT);
	if (!render_buffer) {
		error(SDL_ERROR, SDL_GetError());
		return WinExit();
	}

	SDL_Surface *src;

	src = IMG_Load("textures/light-wall.png");
	if (!src) {
		error(SDL_ERROR, SDL_GetError());
		return WinExit();
	}
	wall_light = SDL_CreateTextureFromSurface(render, src);
	if (!wall_light) {
		error(SDL_ERROR, SDL_GetError());
		SDL_FreeSurface(src);
		return WinExit();
	}
	SDL_FreeSurface(src);

	src = IMG_Load("textures/dark-wall.png");
	if (!src) {
		error(SDL_ERROR, SDL_GetError());
		return WinExit();
	}
	wall_dark = SDL_CreateTextureFromSurface(render, src);
	if (!wall_dark) {
		error(SDL_ERROR, SDL_GetError());
		SDL_FreeSurface(src);
		return WinExit();
	}
	SDL_FreeSurface(src);

	return true;
}

void HandleEvent(SDL_Event *e)
{
	switch (e->type) {
	case SDL_QUIT:
		win_run = false;
		break;
	case SDL_KEYDOWN:
		HandleKeyDown(e->key.keysym.sym);
		break;
	case SDL_KEYUP:
		HandleKeyUp(e->key.keysym.sym);
		break;
	}
}

void WinLoop(void)
{
	SDL_Event e;

	for (win_run = true; win_run;) {
		UpdatePlayer();
		RenderView();
		SDL_RenderCopy(render, render_buffer, NULL, NULL);
		SDL_RenderPresent(render);

		while (SDL_PollEvent(&e)) {
			HandleEvent(&e);
		}
	}
}
