#ifndef __TOUCH_H__
#define __TOUCH_H__

#include "gf2d_entity.h"

void projectile_touch(Entity* self, Entity* other);

void pickup_touch(Entity* self, Entity* other);

void player_touch(Entity* self, Entity* other);
#endif
