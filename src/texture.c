#include <snark.h>

SnarkTexture *LoadTexture(char *filename)
{
	SnarkTexture *texture = calloc(1, sizeof(SnarkTexture));
	if (!texture) {
		error(ENOMEM, strerror(ENOMEM));
		return NULL;
	}

	SDL_Surface *src = IMG_Load(filename);
	if (!src) {
		error(SDL_ERROR, IMG_GetError());
		free(texture);
		return NULL;
	}

	texture->img = SDL_CreateTextureFromSurface(render, src);
	if (!texture->img) {
		error(SDL_ERROR, IMG_GetError());
		free(texture);
		SDL_FreeSurface(src);
		return NULL;
	}

	texture->ref = 1;
	texture->w   = src->w;
	texture->h   = src->h;

	SDL_FreeSurface(src);

	return texture;
}

SnarkTexture *CopyTexture(SnarkTexture *texture)
{
	if (texture) {
		texture->ref += 1;
	}
	return texture;
}

void DestroyTexture(SnarkTexture **texture_p)
{
	SnarkTexture *texture = *texture_p;
	*texture_p            = NULL;

	if (texture) {
		texture->ref -= 1;
		if (texture->ref) {
			return;
		}

		SDL_DestroyTexture(texture->img);
		free(texture);
	}
}
