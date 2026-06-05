#include "raylib.h"


#define MAX_ENTITIES 1024

typedef struct {
	Entity slots[MAX_ENTITIES];
	bool active[MAX_ENTITIES];
	int count;
	uint32_t nextId;
} EntityPool;


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
			return &entityPool[i];
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
