#ifndef INPUT_H
#define INPUT_H

#include "raylib.h"

typedef int ActionID;

typedef struct {
	bool pressed;
	bool held;
	bool released;
} ActionState;

// TODO: add gamepad integration
typedef struct {
	KeyboardKey key;
	MouseButton mouseButton;
} InputBinding;

void inputInit(int count);
void inputUpdate(void);
void inputBind(ActionID action, KeyboardKey key);
void inputUnbind(ActionID action);
void inputBindMouse(ActionID action, MouseButton button);
void inputUnbindMouse(ActionID action);
KeyboardKey inputGetBoundKey(ActionID action);
bool inputIsBound(ActionID action);
bool inputPressed(ActionID action);
bool inputHeld(ActionID action);
bool inputReleased(ActionID action);
Vector2 inputGetMouseDelta(void);
Vector2 inputGetMousePosition(void);
float inputGetMouseWheelDelta(void);
void inputShutdown(void);

#endif
