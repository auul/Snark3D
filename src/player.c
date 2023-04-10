#include <snark.h>

float px;
float py;
float pa;

void UpdatePlayer(void)
{
	if (key_turn_left) {
		pa -= KEY_TURN_STEP;
		while (pa < 0.0) {
			pa += 2.0 * M_PI;
		}
	} else if (key_turn_right) {
		pa += KEY_TURN_STEP;
		while (pa > 2.0 * M_PI) {
			pa -= 2.0 * M_PI;
		}
	}

	if (key_forward) {
		px += WALK_STEP * cos(pa);
		py += WALK_STEP * sin(pa);
	}
}
