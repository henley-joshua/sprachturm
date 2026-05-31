#include "raylib.h"
#include "game/input/game_actions.h"
#include "engine/input/input.h"

void gameInputInit(void) {
	// TODO: add gamepad binding options and loading from save file
	inputBind(ACTION_MOVE_FORWARD, KEY_W);
	inputBind(ACTION_MOVE_BACKWARD, KEY_S);
	inputBind(ACTION_MOVE_LEFT, KEY_A);
	inputBind(ACTION_MOVE_RIGHT, KEY_D);
	inputBind(ACTION_JUMP, KEY_SPACE);
	inputBind(ACTION_SPRINT, KEY_LEFT_SHIFT);
}
