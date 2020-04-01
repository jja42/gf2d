#include "touch.h"
#include "simple_logger.h"
#include "player.h"
#include <stdlib.h>

void projectile_touch(Entity* self, Entity* other){
if (other->owner != self->owner && other->invincibility == 0){
	if(other->tag != 6)other->health -= 10;
	if((self->owner == 5 || self->special == 2) && other->tag != 6) {
		other->frozen = 100;}
	if((self->owner == 4 || self->special == 3) && other->tag == 6){
		return;}
	if(self->special == 1)self->velocity.x = -1 * self->velocity.x;
	if(other->tag != 6)other->invincibility = 100;
	slog("%i", other->health);
	if(other->health <= 0 && (other->tag == 8 || other->tag == 1)){
		if(self->special == 1)hp_drop(other);
		else{enemy_drop(other);}
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

void door_touch(Entity* self, Entity* other){
if(other->tag == 1){
Player* p = (Player*)other->data;
p->level = 0;
load_boss_level(self->special);
}
}

void player_touch(Entity* self, Entity* other){
}

void enemy_touch(Entity* self, Entity* other){
if(other->tag == 1 && other->invincibility == 0){
other->health -= 10;
if(self->owner == 3 && self->health < self->healthmax){
	self->health+=10;
	slog("stealing health. current health: %i",self->health);}
other->invincibility = 100;
slog("taking damage");
if(other->health <= 0){
	gf2d_entity_free(other);}
}
}

void platform_touch(Entity* self, Entity* other){
	if((other->owner == 4 && other->tag == 7) || other->special == 3)return;
	//check if low enough                                                             //check if high enough
if((self->box->pos.y - self->box->height <= other->box->pos.y+other->box->height) && (other->box->pos.y+other->box->height <= self->box->pos.y - self->box->height + 10.0))other->gravity = 0;
else {
	if((self->box->pos.x - self->box->width <= other->box->pos.x + other->box->width) && (self->box->pos.x + self->box->width > other->box->pos.x + other->box->width)) solid_collision(other,0);
	else{solid_collision(other,1);}
}
}

void solid_collision(Entity* self, int dir){
	if(dir == 0){self->colliding = 1;
		//slog("colliding left");
	return;}
	if(dir == 1){self->colliding = 2;
		//slog("colliding right");
	return;}
}

void enemy_drop(Entity* self){
if(self->tag == 8 && self->owner == 2){
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
Player* p = (Player*)gf2d_entity_get(0)->data;
if (self->owner == 3){
	p->zubat_weapon = 1;
	p->level = 0;
	p->menu_timer = 200;
}
if (self->owner == 5){
	p->articuno_weapon = 1;
	p->level = 0;
	p->menu_timer = 200;
}
if (self->owner == 4){
	p->pikachu_weapon = 1;
	p->level = 0;
	p->menu_timer = 200;
}
}

void hp_drop(Entity *self){
	gf2d_pickup_spawn("images/hp.png", 16, 16, 1, vector2d(self->position.x,self->position.y+50), vector2d(2,2),vector2d(0,0),vector2d(0,0),vector2d(16,16),16,16,9,30); 
}
