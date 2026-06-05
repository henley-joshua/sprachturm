#include <stdint.h>
#include <string.h>
#include "raylib.h"
#include "engine/entity/entity.h"


#define MAX_ENTITIES 1024

static Entity entityPool[MAX_ENTITIES];
static bool entityActive[MAX_ENTITIES];
static int entityCount = 0;
static uint32_t nextEntityId = 1;


void entitySystemInit(int maxEntites) {
	memset(entityPool, 0, sizeof(entityPool));
	memset(entityActive, 0, sizeof(entityActive));
	entityCount = 0;
	nextEntityId = 1;
}

void entitySystemShutdown(void) {
	for (int count = 0; count < MAX_ENTITIES; count++) {
		if (entityActive[count]) {
			entityDestroy(&entityPool[count]);
		}
	}

	memset(entityPool, 0, sizeof(entityPool));
	memset(entityActive, 0, sizeof(entityActive));
	entityCount = 0;
	nextEntityId = 1;
}

void entitySetActiveStatus(Entity *entity, bool active) {
	entity->active = active;
}

Entity *entityCreate(void) {
	for (int count = 0; count < MAX_ENTITIES; count++) {
		if (!entityActive[count]) {
			entityPool[count] = (Entity){ 0 };
			entityActive[count] = true;
			entityPool[count].id = nextEntityId++;
			entityPool[count].active = true;
			entityPool[count].model.visible = true;
			entityCount++;
			return &entityPool[count];
		}
	}
	TraceLog(LOG_WARNING, "<Entity> Pool full - cannot create entity");
	return NULL;
}

void entityDestroy(Entity *entity) {
	if (!entity) return;

	physicsUnregister(&entity->body);
	//rendererUnregister(entity);   will be added
	
	int index = (int)(entity - entityPool);
	if (index < 0 || index >= MAX_ENTITIES) return;

	entityActive[index] = false;
	entityCount--;
}
