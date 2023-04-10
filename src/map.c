#include <snark.h>

SnarkMap *MAP;

void ClearTile(SnarkTile *tile)
{
	tile->wall = false;
}

SnarkMap *NewMap(int cols, int rows)
{
	SnarkMap *map =
	    malloc(sizeof(SnarkMap) + cols * rows * sizeof(SnarkTile));
	if (!map) {
		error(ENOMEM, strerror(ENOMEM));
		return NULL;
	}

	map->cols = cols;
	map->rows = rows;

	int size = cols * rows;
	for (int i = 0; i < size; i++) {
		ClearTile(map->tile + i);
	}

	return map;
}

void SetMapWall(SnarkMap *map, int x, int y, bool wall, SnarkTexture **texture)
{
	if (x < 0 || y < 0 || x >= map->cols || y >= map->rows) {
		return;
	}

	map->tile[x + y * map->cols].wall = wall;
	if (wall && texture) {
		map->tile[x + y * map->cols].texture[0] = texture[0];
		map->tile[x + y * map->cols].texture[1] = texture[1];
		map->tile[x + y * map->cols].texture[2] = texture[2];
		map->tile[x + y * map->cols].texture[3] = texture[3];
	}
}

void SurroundMap(SnarkMap *map, SnarkTexture **texture)
{
	for (int x = 0; x < map->cols; x++) {
		SetMapWall(map, x, 0, true, texture);
		SetMapWall(map, x, map->rows - 1, true, texture);
	}

	for (int y = 0; y < map->rows; y++) {
		SetMapWall(map, 0, y, true, texture);
		SetMapWall(map, map->cols - 1, y, true, texture);
	}
}

void SetMapPlayer(SnarkMap *map, float px, float py, float pa)
{
	map->px = px;
	map->py = py;
	map->pa = pa;
}

void SwitchMap(SnarkMap *map)
{
	MAP = map;
	px  = map->px;
	py  = map->py;
	pa  = map->pa;
}
