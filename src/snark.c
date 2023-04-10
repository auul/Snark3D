#include <snark.h>

int main(int argc, char **args)
{
	if (!WinInit()) {
		return 1;
	}

	SnarkTexture *light      = LoadTexture("textures/light-wall.png");
	SnarkTexture *dark       = LoadTexture("textures/dark-wall.png");
	SnarkTexture *texture[4] = {light, dark, light, dark};
	SnarkTexture *empty[4]   = {NULL, NULL, NULL, NULL};

	MAP = NewMap(10, 10);
	if (!MAP) {
		return 1;
	}
	SurroundMap(MAP, texture);
	SetMapWall(MAP, 3, 3, true, empty);
	SetMapWall(MAP, 3, 5, true, empty);
	SetMapPlayer(MAP, (float)MAP->cols / 2.0, (float)MAP->rows / 2.0, 0.0);
	SwitchMap(MAP);

	WinLoop();

	free(MAP);
	DestroyTexture(&light);
	DestroyTexture(&dark);

	WinExit();

	return 0;
}
