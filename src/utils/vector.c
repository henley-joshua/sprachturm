#include <math.h>
#include "raylib.h"

float getVector2Magnitude(Vector2 vector) {
		return sqrtf(vector.x * vector.x +
			     vector.y * vector.y);
}

float getVector3Magnitude(Vector3 vector) {
		return sqrtf(vector.x * vector.x +
			     vector.y * vector.y +
			     vector.z * vector.z);
}
