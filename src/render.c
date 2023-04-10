#include <snark.h>

int RES_WIDTH  = WIN_INIT_WIDTH;
int RES_HEIGHT = WIN_INIT_HEIGHT;

float FOV = 1.0;

float zoom;

void RenderMapPoint(float x, float y, uint8_t r, uint8_t g, uint8_t b)
{
	float size     = zoom / 8.0;
	SDL_Rect pixel = {(int)floor(x * zoom - (size / 2)),
	                  (int)floor(y * zoom - (size / 2)),
	                  (int)ceil(size),
	                  (int)ceil(size)};
	SDL_SetRenderDrawColor(render, r, g, b, 0xff);
	SDL_RenderFillRect(render, &pixel);
}

void RenderMapPlayer(void)
{
	RenderMapPoint(px, py, 0x00, 0xff, 0xff);
	RenderMapPoint(
	    px + 0.5 * cos(pa), py + 0.5 * sin(pa), 0xff, 0x00, 0x00);
}

void CastRay(float x,
             float y,
             float a,
             float *dist,
             int *index,
             int *side,
             float *dist_along)
{
	float dx         = cos(a);
	float dy         = sin(a);
	float dx_over_dy = dx / dy;
	float dy_over_dx = dy / dx;

	float horiz_x, horiz_y;

	if (dx >= 0) {
		horiz_x = floor(x + 1.0);
		horiz_y = y + dy_over_dx * (horiz_x - x);

		while (horiz_x >= 0.0 && horiz_y >= 0.0 &&
		       horiz_x < (float)MAP->cols &&
		       horiz_y < (float)MAP->rows &&
		       !MAP->tile[(int)floor(horiz_x) +
		                  (int)floor(horiz_y) * MAP->cols]
		            .wall) {
			horiz_x += 1.0;
			horiz_y += dy_over_dx;
		}
	} else {
		horiz_x = floor(x);
		horiz_y = y + dy_over_dx * (horiz_x - x);

		while (horiz_x >= 1.0 && horiz_y >= 0.0 &&
		       horiz_x < ((float)MAP->cols + 1.0) &&
		       horiz_y < (float)MAP->rows &&
		       !MAP->tile[(int)floor(horiz_x) - 1 +
		                  (int)floor(horiz_y) * MAP->cols]
		            .wall) {
			horiz_x -= 1.0;
			horiz_y -= dy_over_dx;
		}
	}

	float vert_x, vert_y;

	if (dy >= 0) {
		vert_y = floor(y + 1.0);
		vert_x = x + dx_over_dy * (vert_y - y);

		while (vert_x > 0.0 && vert_y > 0.0 &&
		       vert_x < (float)MAP->cols && vert_y < (float)MAP->rows &&
		       !MAP->tile[(int)floor(vert_x) +
		                  (int)floor(vert_y) * MAP->cols]
		            .wall) {
			vert_y += 1.0;
			vert_x += dx_over_dy;
		}
	} else {
		vert_y = floor(y);
		vert_x = x + dx_over_dy * (vert_y - y);

		while (vert_x > 0.0 && vert_y > 1.0 &&
		       vert_x < (float)MAP->cols &&
		       vert_y < ((float)MAP->rows + 1.0) &&
		       !MAP->tile[(int)floor(vert_x) +
		                  ((int)floor(vert_y) - 1) * MAP->cols]
		            .wall) {
			vert_y -= 1.0;
			vert_x -= dx_over_dy;
		}
	}

	float horiz_dist =
	    sqrt((horiz_x - x) * (horiz_x - x) + (horiz_y - y) * (horiz_y - y));

	float vert_dist =
	    sqrt((vert_x - x) * (vert_x - x) + (vert_y - y) * (vert_y - y));

	if (horiz_dist < vert_dist) {
		*dist = horiz_dist;

		if (dx >= 0) {
			*index = (int)floor(horiz_x) +
			         (int)floor(horiz_y) * MAP->cols;
			*side       = 3;
			*dist_along = horiz_y - floor(horiz_y);
		} else {
			*index = (int)floor(horiz_x) - 1 +
			         (int)floor(horiz_y) * MAP->cols;
			*side       = 1;
			*dist_along = floor(horiz_y) + 1.0 - horiz_y;
		}
	} else {
		*dist = vert_dist;

		if (dy >= 0) {
			*index =
			    (int)floor(vert_x) + (int)floor(vert_y) * MAP->cols;
			*side       = 0;
			*dist_along = vert_x - floor(vert_x);
		} else {
			*index = (int)floor(vert_x) +
			         ((int)floor(vert_y) - 1) * MAP->cols;
			*side       = 2;
			*dist_along = floor(vert_x) + 1.0 - vert_x;
		}
	}
}

void RenderMapRay(float x, float y, float a)
{
	float dist, dist_along;
	int index, side;

	CastRay(x, y, a, &dist, &index, &side, &dist_along);
	SDL_SetRenderDrawColor(render, 0x00, 0xff, 0x00, 0xff);
	SDL_RenderDrawLine(render,
	                   (int)floor(x * zoom),
	                   (int)floor(y * zoom),
	                   (int)floor((x + dist * cos(a)) * zoom),
	                   (int)floor((y + dist * sin(a)) * zoom));
}

void RenderMap(void)
{
	SDL_SetRenderTarget(render, render_buffer);
	SDL_SetRenderDrawColor(render, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(render);

	float zoom_x = (float)RES_WIDTH / (float)MAP->cols;
	float zoom_y = (float)RES_HEIGHT / (float)MAP->rows;

	if (zoom_x < zoom_y) {
		zoom = zoom_x;
	} else {
		zoom = zoom_y;
	}

	SDL_Rect tile = {0, 0, (int)ceil(zoom), (int)ceil(zoom)};
	SDL_SetRenderDrawColor(render, 0x55, 0x55, 0x55, 0xff);

	for (int row = 0; row < MAP->rows; row++) {
		tile.y = row * zoom;
		for (int col = 0; col < MAP->cols; col++) {
			tile.x = col * zoom;

			if (MAP->tile[col + row * MAP->cols].wall) {
				SDL_RenderFillRect(render, &tile);
			} else {
				SDL_RenderDrawRect(render, &tile);
			}
		}
	}

	RenderMapPlayer();

	for (int i = 0; i < RES_WIDTH; i++) {
		RenderMapRay(px,
		             py,
		             pa - (FOV / 2.0) +
		                 ((float)i * FOV / (float)RES_WIDTH));
	}

	SDL_SetRenderTarget(render, NULL);
}

void RenderSky(void)
{
	SDL_Rect sky_box = {0, 0, RES_WIDTH, RES_HEIGHT / 2};
	SDL_SetRenderDrawColor(render, 0x00, 0x33, 0x33, 0xff);
	SDL_RenderFillRect(render, &sky_box);
}

void RenderFloor(void)
{
	SDL_Rect floor_box = {0, RES_HEIGHT / 2, RES_WIDTH, RES_HEIGHT / 2};
	SDL_SetRenderDrawColor(render, 0x55, 0x55, 0x55, 0xff);
	SDL_RenderFillRect(render, &floor_box);
}

void RenderColumn(int i)
{
	float dist, dist_along;
	int index, side;

	CastRay(px,
	        py,
	        pa - (FOV / 2.0) + ((float)i * FOV / (float)RES_WIDTH),
	        &dist,
	        &index,
	        &side,
	        &dist_along);

	int height            = (int)ceil((float)RES_HEIGHT / dist);
	SDL_Rect dest_box     = {i, (RES_HEIGHT / 2) - (height / 2), 1, height};
	SnarkTexture *texture = MAP->tile[index].texture[side];

	if (texture) {
		SDL_Rect src_box = {(int)floor((float)texture->w * dist_along),
		                    0,
		                    1,
		                    texture->h};
		SDL_RenderCopy(render,
		               MAP->tile[index].texture[side]->img,
		               &src_box,
		               &dest_box);
	} else {
		if (side % 2) {
			SDL_SetRenderDrawColor(render, 0xff, 0x00, 0xff, 0xff);
		} else {
			SDL_SetRenderDrawColor(render, 0x33, 0x00, 0x33, 0xff);
		}
		SDL_RenderFillRect(render, &dest_box);
	}
}

void RenderView(void)
{
	SDL_SetRenderTarget(render, render_buffer);

	RenderSky();
	RenderFloor();

	for (int i = 0; i < RES_WIDTH; i++) {
		RenderColumn(i);
	}

	SDL_SetRenderTarget(render, NULL);
}
