#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "gf2d_entity.h"
#include "gfc_audio.h"

void projectile_touch(Entity* self, Entity* other);

void pickup_touch(Entity* self, Entity* other);

void player_touch(Entity* self, Entity* other);

void platform_touch(Entity* self, Entity* other);

void solid_collision(Entity* self, int dir);

void enemy_drop(Entity* self);

void enemy_touch(Entity* self, Entity* other);

void door_touch(Entity* self, Entity* other);

void hp_drop(Entity* self);
#endif
