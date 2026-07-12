#include "raylib.h"
#include "builder.h"
#include <stdio.h> 
#include <string.h>

Asset builderInit(const char *modelLocation) {
  Asset asset;
  // Define Values will need to be updated with full list later.
  asset.position = (Vector3){ 0.0f, 2.0f, 0.0f };
  asset.scale = 2.0f;
  asset.tint = WHITE;

  strncpy(asset.fileLocation, modelLocation, sizeof(asset.fileLocation) - 1);
  asset.fileLocation[sizeof(asset.fileLocation) - 1] = '\n';
  asset.model = LoadModel(modelLocation);
  if(asset.model.meshCount == 0) {
    printf("ERROR: NO MESH\n");
  }
  printf("TEST FILE LOCATION: %s \n", asset.fileLocation);
  return asset;
}

bool builderMove(Asset *asset){
  // Needs to be integrated with key system
  if (IsKeyPressed(KEY_UP)) {
    asset->position.x += 1.0f;
  } else if (IsKeyPressed(KEY_DOWN)) {
    asset->position.x -= 1.0f;
  } else if (IsKeyPressed(KEY_LEFT)) {
    asset->position.z += 1.0f;
  } else if (IsKeyPressed(KEY_RIGHT)) {
    asset->position.z -= 1.0f;
  } else if (IsKeyPressed(KEY_ENTER)){
      return 1;
  } else {
    return 0;
  }
}
void testBuilder(const char * string) {
  printf(string);
}
void runBuilder(Asset *asset) {
  builderMove(asset);
  drawAsset(asset);
}

void drawAsset(Asset *asset) {
  DrawModel(asset->model, asset->position, asset->scale, asset->tint);
}

