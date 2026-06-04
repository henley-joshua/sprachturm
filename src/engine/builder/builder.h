#ifndef BUILDER_H
#define BUILDER_H

#include "raylib.h"

typedef struct Asset { // Temp struct until I know what struct is going to be needed for entity
  Vector3 position;
  char fileLocation[256];
  Model model;
  float scale;
  Color tint;
} Asset;

Asset builderInit(const char *modelLocation);
bool builderMove(Asset *asset);
void testBuild(const char *string);
void runBuilder(Asset *asset);
void drawAsset(Asset *asset);
#endif
