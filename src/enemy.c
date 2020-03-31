#include "enemy.h"
#include "simple_logger.h"
#include "touch.h"
#include "camera.h"


void load_enemy_data(Enemy* self){
}
void load_betamon(Enemy* self,Vector2D position, int patrol_bound_left, int patrol_bound_right){
	self->ent = gf2d_entity_new();
	self->ent->touch = enemy_touch;
	self->ent->health = 50;
	self->ent->healthmax = 100;
	self->ent->owner = 2;
	self->ent->tag = 8;
	self->ent->data = self;
	self->ent->gravity = 1;
	gf2d_entity_load(self->ent,"images/betamon.png",25,24,5,position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 2;
	self->jump_start_frame = 3;
	self->jump_end_frame = 3;
	self->landing_frame = 4;
	self->damaged_frame = 5;
	self->ent->timer = 0;
	self->patrol_bound_left = patrol_bound_left;
	self->patrol_bound_right = patrol_bound_right;
	self->ent->box = gf2d_box(self->ent->position, 33, 30, vector2d(42,39));
	self->ent->think = betamon_think;
	self->enemy_type = 1;
}
void load_penguinmon(Enemy* self,Vector2D position, int patrol_bound_left, int patrol_bound_right){
	self->ent = gf2d_entity_new();
	self->ent->touch = enemy_touch;
	self->ent->health = 50;
	self->ent->healthmax = 100;
	self->ent->owner = 2;
	self->ent->tag = 8;
	self->ent->data = self;
	self->ent->gravity = 1;
	gf2d_entity_load(self->ent,"images/penguinmon.png",25,24,5,position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 5;
	self->attack_start_frame = 6;
	self->attack_end_frame = 7;
	self->damaged_frame = 8;
	self->ent->timer = 0;
	self->patrol_bound_left = patrol_bound_left;
	self->patrol_bound_right = patrol_bound_right;
	self->ent->box = gf2d_box(self->ent->position, 30, 30, vector2d(36,36));
	self->ent->think = penguinmon_think;
	self->enemy_type = 2;
}
void load_wormmon(Enemy* self,Vector2D position, Vector2D flip){
	self->ent = gf2d_entity_new();
	self->ent->touch = enemy_touch;
	self->ent->health = 50;
	self->ent->healthmax = 100;
	self->ent->owner = 2;
	self->ent->tag = 8;
	self->ent->data = self;
	self->ent->gravity = 1;
	gf2d_entity_load(self->ent,"images/wormmon.png",16,24,5,position,vector2d(3,3));
	self->ent->frame = 0;
	self->attack_start_frame = 0;
	self->attack_end_frame = 0;
	self->damaged_frame = 1;
	self->ent->flip = flip;
	self->ent->timer = 0;
	self->ent->box = gf2d_box(self->ent->position, 18, 30, vector2d(24,39));
	self->ent->think = wormmon_think;
	self->enemy_type = 3;
}
void load_zubat(Enemy* self){
	self->ent = gf2d_entity_new();
	self->ent->touch = enemy_touch;
	self->ent->health = 50;
	self->ent->healthmax = 100;
	self->ent->owner = 2;
	self->ent->tag = 8;
	self->ent->data = self;
	self->ent->gravity = 1;
    self->ent->position = vector2d(750,100);
	gf2d_entity_load(self->ent,"images/zubat.png",32,34,3,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 2;
	self->attack_start_frame = 0;
	self->attack_end_frame = 0;
	self->ent->box = gf2d_box(self->ent->position, 30, 48, vector2d(48,51));
	self->enemy_type = 4;
}
void load_pikachu(Enemy* self){
	self->ent = gf2d_entity_new();
	self->ent->touch = enemy_touch;
	self->ent->health = 50;
	self->ent->healthmax = 100;
	self->ent->owner = 2;
	self->ent->tag = 8;
	self->ent->data = self;
	self->ent->gravity = 1;
    self->ent->position = vector2d(750,100);
	gf2d_entity_load(self->ent,"images/pikachu.png",37,50,4,self->ent->position,vector2d(2,2));
	self->ent->frame = 0;
	self->move_end_frame = 4;
	self->attack_start_frame = 5;
	self->attack_end_frame = 7;
	self->ent->box = gf2d_box(self->ent->position, 22, 36, vector2d(38,64));
	self->enemy_type = 5;
}
void load_articuno(Enemy* self){
	self->ent = gf2d_entity_new();
	self->ent->touch = player_touch;
	self->ent->health = 50;
	self->ent->healthmax = 100;
	self->ent->owner = 2;
	self->ent->tag = 8;
	self->ent->data = self;
	self->ent->gravity = 1;
    self->ent->position = vector2d(750,100);
	gf2d_entity_load(self->ent,"images/articuno.png",71,55,3,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 2;
	self->attack_start_frame = 2;
	self->attack_end_frame = 2;
	self->ent->box = gf2d_box(self->ent->position, 48, 63, vector2d(108,81));
	self->enemy_type = 6;
}

void penguinmon_think(Entity* self){
	Enemy* e = (Enemy*)self->data;
	e->patrol_bound_left -= get_camera_velocity().x;
	e->patrol_bound_right -= get_camera_velocity().x;
	if(self->flip.x == 1){
		if(self->position.x > e->patrol_bound_left){
		self->velocity.x = -1;
		}
		else{self->flip.x = 0;}
	}
	else{
		if(self->position.x < e->patrol_bound_right){
			self->velocity.x = 1;
		}
		else{self->flip.x = 1;}
	}
	if(self->timer == 0){
	if(self->flip.x == 0)gf2d_projectile_spawn("images/enemy_attack.png",8,6,3,vector2d(self->position.x+50,self->position.y+20),vector2d(3,3),vector2d(2.5,0),self->flip,vector2d(12,6),20,20,2,30);
	if(self->flip.x == 1)gf2d_projectile_spawn("images/enemy_attack.png",8,6,3,vector2d(self->position.x,self->position.y+20),vector2d(3,3),vector2d(-2.5,0),self->flip,vector2d(12,6),12,6,2,30);
	self->timer = 200;}
	self->timer--;
}

void wormmon_think(Entity* self){
	self->flip.x = 1;
	self->gravity = 0;
	if(self->timer == 0){
	if(self->flip.x == 0)gf2d_projectile_spawn("images/enemy_attack.png",8,6,3,vector2d(self->position.x+25,self->position.y+30),vector2d(3,3),vector2d(2.5,0),self->flip,vector2d(12,6),20,20,2,30);
	if(self->flip.x == 1)gf2d_projectile_spawn("images/enemy_attack.png",8,6,3,vector2d(self->position.x,self->position.y+30),vector2d(3,3),vector2d(-2.5,0),self->flip,vector2d(12,6),12,6,2,30);
	self->timer = 150;}
	self->timer--;
}

void betamon_think(Entity* self){
	Enemy* e = (Enemy*)self->data;
	e->patrol_bound_left -= get_camera_velocity().x;
	e->patrol_bound_right -= get_camera_velocity().x;
	if(self->flip.x == 0){
		if(self->position.x > e->patrol_bound_left){
		self->velocity.x = -1;
		}
		else{self->flip.x = 1;}
	}
	else{
		if(self->position.x < e->patrol_bound_right){
			self->velocity.x = 1;
		}
		else{self->flip.x = 0;}
	}
	if(self->timer < 50){
		self->velocity.y = -5;}
	if(self->timer > 200)self->timer = 0;
		self->timer++;
}
