#include <stdlib.h>
#include "raylib.h"
#include "engine/input/input.h"

static ActionState *actionStates = NULL;
static InputBinding *bindings = NULL;
static int maxActions = 0;
static bool initialized = false;

void inputInit(int count) {
	maxActions = count;
	// Use calloc to initialize all to 0
	actionStates = calloc(count, sizeof(ActionState));
	bindings = calloc(count, sizeof(InputBinding));

	for (int i = 0; i < count; i++) {
		// TODO: add gamepad integration
		bindings[i].key = KEY_NULL;
		bindings[i].mouseButton = -1;
	}

	initialized = true;
}

// Updates the state of all bound keys
void inputUpdate(void) {
	if (!initialized) return;

	// TODO: add gamepad integration
	for (int count = 0; count < maxActions; count++) {
		bool keyHeld = bindings[count].key != KEY_NULL && IsKeyDown(bindings[count].key);
		bool mouseHeld = bindings[count].mouseButton != -1 && IsMouseButtonDown(bindings[count].mouseButton);

		bool wasHeld = actionStates[count].held;
		bool isHeld = keyHeld || mouseHeld;

		actionStates[count].pressed = isHeld && !wasHeld;
		actionStates[count].held = isHeld;
		actionStates[count].released = !isHeld && wasHeld;
	}
}

void inputBind(ActionID action, KeyboardKey key) {
	if (action < 0 || action >= maxActions) return;
	bindings[action].key = key;
}

void inputUnbind(ActionID action) {
	if (action < 0 || action >= maxActions) return;
	bindings[action].key = KEY_NULL;
}

void inputBindMouse(ActionID action, MouseButton button) {
	if (action < 0 || action >= maxActions) return;
	bindings[action].mouseButton = button;
}

void inputUnbindMouse(ActionID action) {
	if (action < 0 || action >= maxActions) return;
	bindings[action].mouseButton = -1;
}

KeyboardKey inputGetBoundKey(ActionID action) {
	if (action < 0 || action >= maxActions) return KEY_NULL;
	return bindings[action].key;
}

bool inputIsBound(ActionID action) {
	if (action < 0 || action >= maxActions) return false;
	if (bindings[action].key != KEY_NULL || bindings[action].mouseButton != -1) return true;
	return false;
}

bool inputPressed(ActionID action) {
	if (action < 0 || action >= maxActions) return false;
	return actionStates[action].pressed;
}

bool inputHeld(ActionID action) {
	if (action < 0 || action >= maxActions) return false;
	return actionStates[action].held;
}

bool inputReleased(ActionID action) {
	if (action < 0 || action >= maxActions) return false;
	return actionStates[action].released;
}

Vector2 inputGetMouseDelta(void) {
	return GetMouseDelta();
}

Vector2 inputGetMousePosition(void) {
	return GetMousePosition();
}

float inputGetMouseWheelDelta(void) {
	return GetMouseWheelMove();
}

void inputShutdown(void) {
	if (actionStates) { free(actionStates); actionStates = NULL; }
	if (bindings) { free(bindings); bindings = NULL; }
	maxActions = 0;
	initialized = false;
}
