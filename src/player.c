#include "player.h"
#include "touch.h"
#include "simple_logger.h"

void setup_player_ent(Player* self){
self->ent = gf2d_entity_new();
self->ent->position = vector2d(550,200);
self->ent->touch = player_touch;
self->ent->healthmax = 100;
self->ent->health = 100;
self->ent->owner = 1;
self->ent->tag = 1;
self->ent->experience = 0;
self->ent->data = self;
self->ent->gravity = 1;
}
void load_agumon(Player* self){
	gf2d_entity_load(self->ent,"images/aguman.png",48,48,11,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 17;
	self->jump_start_frame = 18;
	self->jump_end_frame = 23;
	self->landing_frame = 24;
	self->ground_attack_start_frame = 25;
	self->ground_attack_end_frame = 35;
	self->air_attack_start_frame = 37;
	self->air_attack_end_frame = 46;
	self->damaged_frame = 46;
	self->attack = agumon_attack;
	self->air_attack = agumon_attack;
	self->ent->box = gf2d_box(self->ent->position, 30, 39, vector2d(72,105));
	self->digivolve = load_wargreymon;
}

void load_guilmon(Player* self){
	gf2d_entity_load(self->ent,"images/guilmon.png",40,50,9,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 7;
	self->jump_start_frame = 8;
	self->jump_end_frame = 15;
	self->landing_frame = 16;
	self->ground_attack_start_frame = 17;
	self->ground_attack_end_frame = 34;
	self->air_attack_start_frame = 35;
	self->air_attack_end_frame = 40;
	self->damaged_frame = 41;
	self->attack = guilmon_attack;
	self->air_attack = guilmon_air_attack;
	self->ent->box = gf2d_box(self->ent->position,35,40,vector2d(35,60));
	self->digivolve = load_gallantmon;
}

void load_gabumon(Player* self){
	gf2d_entity_load(self->ent,"images/gabumon.png",82,48,7,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 9;
	self->jump_start_frame = 10;
	self->jump_end_frame = 15;
	self->landing_frame = 16;
	self->ground_attack_start_frame = 17;
	self->ground_attack_end_frame = 30;
	self->air_attack_start_frame = 31;
	self->air_attack_end_frame = 39;
	self->damaged_frame = 40;
	self->attack = gabumon_attack;
	self->air_attack = gabumon_air_attack;
	self->digivolve = load_etemon;
}

void load_wargreymon(Player* self){
	gf2d_entity_load(self->ent,"images/greymon_beta.png",64,64,8,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 8;
	self->jump_start_frame = 9;
	self->jump_end_frame = 15;
	self->landing_frame = 16;
	self->ground_attack_start_frame = 34;
	self->ground_attack_end_frame = 48;
	self->air_attack_start_frame = 27;
	self->air_attack_end_frame = 33;
	self->attack = wargreymon_attack;
	self->air_attack = wargreymon_air_attack;
	self->ent->box = gf2d_box(self->ent->position,50,50,vector2d(15,30));
}

void load_gallantmon(Player* self){
	gf2d_entity_load(self->ent,"images/gallantmon_beta.png",100,80,8,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 8;
	self->jump_start_frame = 9;
	self->jump_end_frame = 17;
	self->landing_frame = 18;
	self->ground_attack_start_frame = 35;
	self->ground_attack_end_frame = 51;
	self->air_attack_start_frame = 27;
	self->air_attack_end_frame = 34;
	self->attack = gallantmon_attack;
	self->air_attack = gallantmon_air_attack;
}

void load_etemon(Player* self){
	gf2d_entity_load(self->ent,"images/etemon_beta.png",64,64,8,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 8;
	self->jump_start_frame = 10;
	self->jump_end_frame = 20;
	self->landing_frame = 21;
	self->ground_attack_start_frame = 40;
	self->ground_attack_end_frame = 51;
	self->air_attack_start_frame = 33;
	self->air_attack_end_frame = 39;
	self->attack = etemon_attack;
	self->air_attack = etemon_air_attack;
}

void agumon_attack(Player* self){
if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/fireball.png",24,24,3,vector2d(self->ent->position.x+57.5,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(2.5,0),self->ent->flip,vector2d(40,32.5),20,20,1,30);
if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/fireball.png",24,24,3,vector2d(self->ent->position.x+20,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(-2.5,0),self->ent->flip,vector2d(40,32.5),20,20,1,30);
}

void gabumon_attack(Player* self){
}
void gabumon_air_attack(Player* self){
}

void guilmon_attack(Player* self){
	//if(self->ent->flip.x == 0)gf2d_entity_spawn("images/guilmon_ranged_attack.png",16,16,3,vector2d(self->ent->position.x+57.5,self->ent->position.y+70),vector2d(3.5,3.5),vector2d(4,0),self->ent->flip,vector2d(2,5),1);
	//if(self->ent->flip.x == 1)gf2d_entity_spawn("images/guilmon_ranged_attack.png",16,16,3,vector2d(self->ent->position.x+20,self->ent->position.y+70),vector2d(3.5,3.5),vector2d(-4,0),self->ent->flip,vector2d(0,5),1);
}
void guilmon_air_attack(Player* self){
}

void wargreymon_attack(Player* self){
	//if(self->ent->flip.x == 0)gf2d_entity_spawn("images/greymon_ranged_attack.png",24,28,3,vector2d(self->ent->position.x+65,self->ent->position.y+30),vector2d(4,4),vector2d(3,0),self->ent->flip,vector2d(1,1),1);
	//if(self->ent->flip.x == 1)gf2d_entity_spawn("images/greymon_ranged_attack.png",24,28,3,vector2d(self->ent->position.x+20,self->ent->position.y+30),vector2d(4,4),vector2d(-3,0),self->ent->flip,vector2d(1,1),1);
}

void wargreymon_air_attack(Player* self){
}

void gallantmon_attack(Player* self){
	//if(self->ent->flip.x == 0)gf2d_entity_spawn("images/gallantmon_ranged_attack.png",24,24,3,vector2d(self->ent->position.x+220,self->ent->position.y+100),vector2d(3,3),vector2d(3,0),self->ent->flip,vector2d(1,1),1);
	//if(self->ent->flip.x == 1)gf2d_entity_spawn("images/gallantmon_ranged_attack.png",24,24,3,vector2d(self->ent->position.x,self->ent->position.y+100),vector2d(3,3),vector2d(-3,0),self->ent->flip,vector2d(1,1),1);
}

void gallantmon_air_attack(Player* self){
}

void etemon_attack(Player* self){
	//if(self->ent->flip.x == 0)gf2d_entity_spawn("images/etemon_ranged_attack.png",24,24,3,vector2d(self->ent->position.x+100,self->ent->position.y+30),vector2d(3,3),vector2d(3,0),self->ent->flip,vector2d(1,1),1);
	//if(self->ent->flip.x == 1)gf2d_entity_spawn("images/etemon_ranged_attack.png",24,24,3,vector2d(self->ent->position.x,self->ent->position.y+30),vector2d(3,3),vector2d(-3,0),self->ent->flip,vector2d(1,1),1);
}

void etemon_air_attack(Player* self){
}

void player_pickup(int tag, Player *self){
switch(tag){

case 2: self->ent->experience += 100;
slog("%f",self->ent->experience);
break;

case 3: load_agumon(self);
break;

case 4: load_gabumon(self);
break;

case 5: load_guilmon(self);
break;
}
}
