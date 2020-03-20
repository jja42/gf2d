#include "touch.h"
#include "simple_logger.h"
#include "player.h"

void projectile_touch(Entity* self, Entity* other){
if (other->owner != self->owner){
	other->health -= 10;
	slog("%f", other->health);
	gf2d_entity_free(self);
}
}

void pickup_touch(Entity* self, Entity* other){
if(other->tag=1){
Player* p = (Player*)other->data;
player_pickup(self->tag, p);
gf2d_entity_free(self);
}
}

void player_touch(Entity* self, Entity* other){
}
