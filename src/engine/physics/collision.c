#include "raylib.h"

typedef struct {
	bool left, right, top, bottom;
} BorderCollision;

BorderCollision checkBorderCollision(Rectangle rec) {
	return (BorderCollision) {
		.left = rec.x <= 0,
		.right = rec.x + rec.width >= GetScreenWidth(),
		.top = rec.y <= 0,
		.bottom = rec.y + rec.height >= GetScreenHeight()
	};
}
