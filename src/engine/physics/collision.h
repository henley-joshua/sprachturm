#ifndef COLLISION_H
#define COLLISION_H

typedef struct {
	bool left, right, top, bottom;
} BorderCollision;
BorderCollision checkBorderCollision(Rectangle rec);

#endif
