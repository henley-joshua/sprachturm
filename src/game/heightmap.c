#define HM_WIDTH 128
#define HM_HEIGHT 128

float heightmap[HM_WIDTH * HM_HEIGHT];

void generateFlat(void) {
	for (int z = 0; z < HM_HEIGHT; z++) {
		for (int x = 0; x < HM_WIDTH; x++) {
			heightmap[z * HM_WIDTH + x] = 0.0f;
		}
	}
}
