#include <snark.h>

uint32_t KEY_TURN_LEFT  = SDLK_LEFT;
uint32_t KEY_TURN_RIGHT = SDLK_RIGHT;
uint32_t KEY_FORWARD    = SDLK_UP;
uint32_t KEY_BACKWARD   = SDLK_DOWN;

bool key_turn_left;
bool key_turn_right;
bool key_forward;
bool key_backward;

void HandleKeyDown(uint32_t key)
{
	if (key == KEY_TURN_LEFT) {
		key_turn_left = true;
	} else if (key == KEY_TURN_RIGHT) {
		key_turn_right = true;
	} else if (key == KEY_FORWARD) {
		key_forward = true;
	} else if (key == KEY_BACKWARD) {
		key_backward = true;
	}
}

void HandleKeyUp(uint32_t key)
{
	if (key == KEY_TURN_LEFT) {
		key_turn_left = false;
	} else if (key == KEY_TURN_RIGHT) {
		key_turn_right = false;
	} else if (key == KEY_FORWARD) {
		key_forward = false;
	} else if (key == KEY_BACKWARD) {
		key_backward = false;
	}
}
