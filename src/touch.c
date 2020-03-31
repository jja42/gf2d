#include "touch.h"
#include "simple_logger.h"
#include "player.h"
#include <stdlib.h>

void projectile_touch(Entity* self, Entity* other){
if (other->owner != self->owner && other->invincibility == 0){
	other->health -= 10;
	if(other->tag == 1)other->invincibility = 100;
	slog("%i", other->health);
	if(other->health == 0 && (other->tag == 8 || other->tag == 1)){
		enemy_drop(other);
		gf2d_entity_free(other);}
	gf2d_entity_free(self);
}
}

void pickup_touch(Entity* self, Entity* other){
if(other->tag == 1){
Player* p = (Player*)other->data;
player_pickup(self->tag, p);
gf2d_entity_free(self);
}
}

void player_touch(Entity* self, Entity* other){
}

void enemy_touch(Entity* self, Entity* other){
if(other->tag == 1 && other->invincibility == 0){
other->health -= 10;
other->invincibility = 100;
slog("taking damage");
}
}

void platform_touch(Entity* self, Entity* other){
	//check if low enough                                                             //check if high enough
if((self->box->pos.y - self->box->height <= other->box->pos.y+other->box->height) && (other->box->pos.y+other->box->height <= self->box->pos.y - self->box->height + 10.0))other->gravity = 0;
else {
	if((self->box->pos.x - self->box->width <= other->box->pos.x + other->box->width) && (self->box->pos.x + self->box->width > other->box->pos.x + other->box->width)) solid_collision(other,0);
	else{solid_collision(other,1);}
}
}

void solid_collision(Entity* self, int dir){
	if(dir == 0){self->colliding = 1;
		slog("colliding left");
	return;}
	if(dir == 1){self->colliding = 2;
		slog("colliding right");
	return;}
}

void enemy_drop(Entity* self){
if(self->tag == 8){
int drop = rand() % 10;
switch(drop){

case 0: gf2d_pickup_spawn("images/exp_orb.png", 15, 15, 1, vector2d(self->position.x,self->position.y+50), vector2d(2,2),vector2d(0,0),vector2d(0,0),vector2d(14,14),14,14,2,30); 
break;

case 1: gf2d_pickup_spawn("images/exp_orb.png", 15, 15, 1, vector2d(self->position.x,self->position.y+50), vector2d(2,2),vector2d(0,0),vector2d(0,0),vector2d(14,14),14,14,2,30);
break;

case 2: gf2d_pickup_spawn("images/exp_orb.png", 15, 15, 1, vector2d(self->position.x,self->position.y+50), vector2d(2,2),vector2d(0,0),vector2d(0,0),vector2d(14,14),14,14,2,30);
break;

case 3: gf2d_pickup_spawn("images/exp_orb.png", 15, 15, 1, vector2d(self->position.x,self->position.y+50), vector2d(2,2),vector2d(0,0),vector2d(0,0),vector2d(14,14),14,14,2,30);
break;

case 4: gf2d_pickup_spawn("images/exp_orb.png", 15, 15, 1, vector2d(self->position.x,self->position.y+50), vector2d(2,2),vector2d(0,0),vector2d(0,0),vector2d(14,14),14,14,2,30);
break;

case 5: gf2d_pickup_spawn("images/exp_orb.png", 15, 15, 1, vector2d(self->position.x,self->position.y+50), vector2d(2,2),vector2d(0,0),vector2d(0,0),vector2d(14,14),14,14,2,30);
break;

case 6: gf2d_pickup_spawn("images/exp_orb.png", 15, 15, 1, vector2d(self->position.x,self->position.y+50), vector2d(2,2),vector2d(0,0),vector2d(0,0),vector2d(14,14),14,14,2,30);
break;

case 7: gf2d_pickup_spawn("images/aguman_extra_life.png", 19, 13, 1, vector2d(self->position.x,self->position.y+50), vector2d(3,3),vector2d(0,0),vector2d(0,0),vector2d(27,18),27,18,3,30);
break;

case 8: gf2d_pickup_spawn("images/gabumon_extra_life.png", 20, 16, 1, vector2d(self->position.x,self->position.y+50), vector2d(3,3),vector2d(0,0),vector2d(0,0),vector2d(30,24),30,24,4,30);
break;

case 9: gf2d_pickup_spawn("images/guilmon_extra_life.png", 20, 16, 1, vector2d(self->position.x,self->position.y+50), vector2d(3,3),vector2d(0,0),vector2d(0,0),vector2d(30,24),30,24,5,30);
break;
}
}
}
