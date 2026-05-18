#include "raylib.h"
#include "utils/macros.h"

void drawEnvironment(void) {
	Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
	Vector3 cubePosition2 = { 10.0f, 0.0f, 10.0f };
	Vector3 cubePosition3 = { 20.0f, 0.0f, 20.0f };
	Vector3 cubePosition4 = { 20.0f, 0.0f, 0.0f };
	Vector3 cubePosition5 = { 10.0f, 0.0f, 0.0f };
	Vector3 cubePosition6 = { 0.0f, 0.0f, 10.0f };
	Vector3 cubePosition7 = { 0.0f, 0.0f, 20.0f };
	Vector3 cubeList[7] = {cubePosition, cubePosition2,
			       cubePosition3, cubePosition4,
			       cubePosition5, cubePosition6,
			       cubePosition7};
	DrawGrid(100, 10.0f);
	for (int i = 0; i < ARRAY_LEN(cubeList); i++) {
		DrawCube(cubeList[i], 2.0f, 2.0f, 2.0f, RED);
		DrawCubeWires(cubeList[i], 2.0f, 2.0f, 2.0f, LIGHTGRAY);
	}
}
