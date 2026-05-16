#ifndef RENDERER_H
#define RENDERER_H

int createWindow(void);
typedef struct {
	bool left, right, top, bottom;
} BorderCollision;
BorderCollision checkBorderCollision(Rectangle rec);

#endif
