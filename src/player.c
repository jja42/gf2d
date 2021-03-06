#include "player.h"
#include "touch.h"
#include "simple_logger.h"
#include "audio.h"
#include "camera.h"

void setup_player_ent(Player* self){
self->ent = gf2d_entity_new();
self->ent->touch = player_touch;
self->ent->healthmax = 100;
self->ent->owner = 1;
self->ent->tag = 1;
self->ent->data = self;
self->ent->gravity = 1;
self->ent->health = 100;
self->ent->experience = 0;
self->agumon_lives = 3;
self->gabumon_lives = 3;
self->guilmon_lives = 3;
self->lives = 3;
self->level = 0;
self->zubat_completed = 0;
self->articuno_completed = 0;
self->pikachu_completed = 0;
self->zubat_weapon = 0;
self->articuno_weapon = 0;
self->pikachu_weapon = 0;
self->hp_pickups = 0;
self->ent->frame = 0;
load_agumon(self);
}

void reset_player(Player* self){
self->ent->healthmax = 100;
self->ent->gravity = 1;
self->ent->health = 100;
self->ent->experience = 0;
self->agumon_lives = 3;
self->gabumon_lives = 3;
self->guilmon_lives = 3;
self->lives = 3;
self->zubat_completed = 0;
self->articuno_completed = 0;
self->pikachu_completed = 0;
self->zubat_weapon = 0;
self->articuno_weapon = 0;
self->pikachu_weapon = 0;
self->hp_pickups = 0;
self->ent->frame = 0;
load_agumon(self);
}

void load_player_data(Player *self){
SJson *player_data = sj_load("sav/player.save");

if (player_data){
			SJson *camera_offset;
            SJson *player_health;
            SJson *player_experience;
            SJson *player_agumon_lives;
            SJson *player_gabumon_lives;
            SJson *player_guilmon_lives;
            SJson *player_lives;
            SJson *player_level;
            SJson *player_digitimer;
            SJson *player_digimon;
            SJson *player_digivolved;
            SJson *player_articuno_weapon;
            SJson *player_pikachu_weapon;
            SJson *player_zubat_weapon;
            SJson *player_zubat_complete;
            SJson *player_articuno_complete;
            SJson *player_pikachu_complete;
            SJson *player_hp_pickups;
            
            camera_offset = sj_object_get_value(player_data, "Offset");
            player_health = sj_object_get_value(player_data, "Health");
            player_experience = sj_object_get_value(player_data, "Experience");
            player_agumon_lives = sj_object_get_value(player_data, "AgumonLives");
            player_gabumon_lives = sj_object_get_value(player_data, "GabumonLives");
            player_guilmon_lives = sj_object_get_value(player_data, "GuilmonLives");
            player_level = sj_object_get_value(player_data, "Level");
            player_digitimer = sj_object_get_value(player_data, "Digitimer");
            player_digimon = sj_object_get_value(player_data, "Digimon");
            player_digivolved = sj_object_get_value(player_data,"Digivolved");
            player_zubat_weapon = sj_object_get_value(player_data,"ZubatWeapon");
            player_pikachu_weapon = sj_object_get_value(player_data,"PikachuWeapon");
            player_articuno_weapon = sj_object_get_value(player_data,"ArticunoWeapon");
            player_zubat_complete = sj_object_get_value(player_data,"ZubatComplete");
            player_articuno_complete = sj_object_get_value(player_data,"ArticunoComplete");
            player_pikachu_complete = sj_object_get_value(player_data,"PikachuComplete");
            player_lives = sj_object_get_value(player_data,"Lives");
            player_hp_pickups = sj_object_get_value(player_data,"HpPickups");
            
            SJson *offsetX;
            SJson *offsetY;
            offsetX = sj_array_get_nth(camera_offset, 0);
            offsetY = sj_array_get_nth(camera_offset, 1);
            float cam_offset_x;
            float cam_offset_y;
            sj_get_float_value(offsetX, &cam_offset_x);
            sj_get_float_value(offsetY, &cam_offset_y);
            set_camera_offset(vector2d(cam_offset_x,cam_offset_y));
            sj_get_integer_value(player_health,&self->ent->health);
            sj_get_integer_value(player_experience,&self->ent->experience);
            sj_get_integer_value(player_agumon_lives,&self->agumon_lives);
            sj_get_integer_value(player_gabumon_lives,&self->gabumon_lives);
            sj_get_integer_value(player_guilmon_lives,&self->guilmon_lives);
            sj_get_integer_value(player_level,&self->level);
            sj_get_float_value(player_digitimer,&self->digi_timer);
            sj_get_integer_value(player_digimon,&self->digimon);
            sj_get_integer_value(player_digivolved,&self->digivolved);
            sj_get_integer_value(player_zubat_weapon,&self->zubat_weapon);
            sj_get_integer_value(player_pikachu_weapon,&self->pikachu_weapon);
            sj_get_integer_value(player_articuno_weapon,&self->articuno_weapon);
            sj_get_integer_value(player_lives,&self->lives);
            sj_get_integer_value(player_zubat_complete,&self->zubat_completed);
            sj_get_integer_value(player_articuno_complete,&self->articuno_completed);
            sj_get_integer_value(player_pikachu_complete,&self->pikachu_completed);
            sj_get_integer_value(player_hp_pickups,&self->hp_pickups);
            
			switch (self->digimon){
			case 1: load_agumon(self);
			break;
			case 2: load_guilmon(self);
			break;
			case 3: load_gabumon(self);
			break;
			case 4: load_wargreymon(self);
			break;
			case 5: load_gallantmon(self);
			break;
			case 6: load_etemon(self);
			break;
}
}
}

void load_agumon(Player* self){
	self->ent->position.y -= 1;
	gf2d_entity_load(self->ent,"images/aguman.png",48,48,11,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 17;
	self->jump_start_frame = 18;
	self->jump_end_frame = 23;
	self->landing_frame = 24;
	self->ground_attack_start_frame = 25;
	self->ground_attack_end_frame = 35;
	self->ground_attack_damage_frame = 35;
	self->air_attack_start_frame = 37;
	self->air_attack_end_frame = 46;
	self->air_attack_damage_frame = 46;
	self->damaged_frame = 46;
	self->attack = agumon_attack;
	self->air_attack = agumon_attack;
	self->ent->box = gf2d_box(self->ent->position, 30, 39, vector2d(72,105));
	self->digivolve = load_wargreymon;
	self->digivolved = 0;
	self->digimon = 1;
}

void load_guilmon(Player* self){
	self->ent->position.y -= 1;
	gf2d_entity_load(self->ent,"images/guilmon_2.png",40,50,9,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 8;
	self->jump_start_frame = 9;
	self->jump_end_frame = 15;
	self->landing_frame = 16;
	self->ground_attack_start_frame = 17;
	self->ground_attack_end_frame = 34;
	self->air_attack_start_frame = 35;
	self->air_attack_end_frame = 43;
	self->damaged_frame = 44;
	self->attack = guilmon_attack;
	self->air_attack = guilmon_air_attack;
	self->ent->box = gf2d_box(self->ent->position,39,45,vector2d(63,105));
	self->digivolve = load_gallantmon;
	self->digivolved = 0;
	self->digimon = 2;
	self->air_attack_damage_frame = 37;
	self->ground_attack_damage_frame = 34;
}

void load_gabumon(Player* self){
	self->ent->position.y -= 1;
	gf2d_entity_load(self->ent,"images/gabumon.png",48,48,9,self->ent->position,vector2d(3,3));
	self->ent->frame = 0;
	self->move_end_frame = 9;
	self->jump_start_frame = 10;
	self->jump_end_frame = 15;
	self->landing_frame = 10;
	self->ground_attack_start_frame = 16;
	self->ground_attack_end_frame = 31;
	self->air_attack_start_frame = 32;
	self->air_attack_end_frame = 41;
	self->damaged_frame = 42;
	self->attack = gabumon_attack;
	self->air_attack = gabumon_air_attack;
	self->ent->box = gf2d_box(self->ent->position,36,42,vector2d(72,102));
	self->digivolve = load_etemon;
	self->digivolved = 0;
	self->digimon = 3;
	self->air_attack_damage_frame = 37;
	self->ground_attack_damage_frame = 23;
}

void load_wargreymon(Player* self){
	self->ent->position.y -= 20;
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
	self->ent->box = gf2d_box(self->ent->position,54,66,vector2d(87,102));
	self->digi_timer = 3000;
	self->digivolved = 1;
	self->dedigivolve = load_agumon;
	self->digimon = 4;
	self->air_attack_damage_frame = 31;
	self->ground_attack_damage_frame = 48;
}

void load_gallantmon(Player* self){
	self->ent->position.y -= 65;
	self->ent->position.x -= 50;
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
	self->ent->box = gf2d_box(self->ent->position,54,75,vector2d(144,141));
	self->digi_timer = 3000;
	self->digivolved = 1;
	self->dedigivolve = load_guilmon;
	self->digimon = 5;
	self->air_attack_damage_frame = 30;
	self->ground_attack_damage_frame = 51;
}

void load_etemon(Player* self){
	self->ent->position.y -= 50;
	self->ent->position.x -= 10;
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
	self->ent->box = gf2d_box(self->ent->position,48,72,vector2d(99,99));
	self->digi_timer = 3000;
	self->digivolved = 1;
	self->dedigivolve = load_gabumon;
	self->digimon = 6;
	self->air_attack_damage_frame = 36;
	self->ground_attack_damage_frame = 51;
}

void agumon_attack(Player* self){
if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/fireball.png",24,24,3,vector2d(self->ent->position.x+57.5,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(2.5,0),self->ent->flip,vector2d(40,32.5),20,20,1,30);
if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/fireball.png",24,24,3,vector2d(self->ent->position.x+20,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(-2.5,0),self->ent->flip,vector2d(40,32.5),20,20,1,30);
gfc_sound_play(FireballAttack,0,.5,2,1);
}

void gabumon_attack(Player* self){
if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x+80,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(0,0),self->ent->flip,vector2d(40,32.5),20,20,1,1);
if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(0,0),self->ent->flip,vector2d(40,32.5),20,20,1,1);
gfc_sound_play(GabumonPunch,0,.5,2,1);
}

void gabumon_air_attack(Player* self){
if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x+60,self->ent->position.y+70),vector2d(2.5,2.5),vector2d(0,0),self->ent->flip,vector2d(40,32.5),20,20,1,1);
if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x+20,self->ent->position.y+70),vector2d(2.5,2.5),vector2d(0,0),self->ent->flip,vector2d(40,32.5),20,20,1,1);
gfc_sound_play(GabumonSpin,0,.5,2,1);
}

void guilmon_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/guilmon_ranged_attack.png",16,16,3,vector2d(self->ent->position.x+57.5,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(2.5,0),self->ent->flip,vector2d(40,32.5),20,20,1,30);
	if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/guilmon_ranged_attack.png",16,16,3,vector2d(self->ent->position.x+20,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(-2.5,0),self->ent->flip,vector2d(40,32.5),20,20,1,30);
gfc_sound_play(FireballAttack,0,.5,2,1);
}
void guilmon_air_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x+57.5,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(0,0),self->ent->flip,vector2d(40,32.5),20,20,1,1);
	if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x+20,self->ent->position.y+62.5),vector2d(2.5,2.5),vector2d(0,0),self->ent->flip,vector2d(40,32.5),20,20,1,1);
gfc_sound_play(GuilmonClaw,0,.5,2,1);
}

void wargreymon_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/greymon_ranged_attack.png",24,28,3,vector2d(self->ent->position.x+65,self->ent->position.y+30),vector2d(4,4),vector2d(3,0),self->ent->flip,vector2d(44,56),36,36,1,30);
	if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/greymon_ranged_attack.png",24,28,3,vector2d(self->ent->position.x+20,self->ent->position.y+30),vector2d(4,4),vector2d(-3,0),self->ent->flip,vector2d(44,56),36,36,1,30);
gfc_sound_play(WarGreymonProjectile,0,.5,2,1);
}
void wargreymon_air_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/empty.png",24,28,3,vector2d(self->ent->position.x+80,self->ent->position.y+40),vector2d(4,4),vector2d(0,0),self->ent->flip,vector2d(44,56),36,36,1,1);
	if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/empty.png",24,28,3,vector2d(self->ent->position.x+20,self->ent->position.y+30),vector2d(4,4),vector2d(0,0),self->ent->flip,vector2d(44,56),36,36,1,1);
gfc_sound_play(GuilmonClaw,0,.5,2,1);
}

void gallantmon_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/gallantmon_ranged_attack.png",24,24,3,vector2d(self->ent->position.x+200,self->ent->position.y+100),vector2d(3,3),vector2d(3,0),self->ent->flip,vector2d(48,36),21,21,1,30);
	if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/gallantmon_ranged_attack.png",24,24,3,vector2d(self->ent->position.x,self->ent->position.y+100),vector2d(3,3),vector2d(-3,0),self->ent->flip,vector2d(48,36),21,21,1,30);
gfc_sound_play(GallantmonProjectile,0,.5,2,1);
}
void gallantmon_air_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x+200,self->ent->position.y+100),vector2d(3,3),vector2d(0,0),self->ent->flip,vector2d(48,36),21,21,1,1);
	if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x+20,self->ent->position.y+100),vector2d(3,3),vector2d(0,0),self->ent->flip,vector2d(48,36),21,21,1,1);
gfc_sound_play(GuilmonClaw,0,.5,2,1);
}

void etemon_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/etemon_ranged_attack.png",24,24,3,vector2d(self->ent->position.x+100,self->ent->position.y+50),vector2d(3,3),vector2d(3,0),self->ent->flip,vector2d(36,42),24,24,1,30);
	if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/etemon_ranged_attack.png",24,24,3,vector2d(self->ent->position.x,self->ent->position.y+50),vector2d(3,3),vector2d(-3,0),self->ent->flip,vector2d(36,42),24,24,1,30);
gfc_sound_play(EtemonProjectile,0,.5,2,1);
}

void etemon_air_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x+100,self->ent->position.y+30),vector2d(3,3),vector2d(0,0),self->ent->flip,vector2d(36,42),24,24,1,1);
	if(self->ent->flip.x == 1)gf2d_projectile_spawn("images/empty.png",24,24,3,vector2d(self->ent->position.x,self->ent->position.y+30),vector2d(3,3),vector2d(0,0),self->ent->flip,vector2d(36,42),24,24,1,1);
gfc_sound_play(GabumonSpin,0,.5,2,1);
}

void zubat_weapon_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_special_projectile_spawn("images/zubat_attack.png",25,24,3,vector2d(self->ent->position.x+80,self->ent->position.y+20),vector2d(2,2),vector2d(1.5,0),self->ent->flip,vector2d(26,24),24,20,1,30,1);
	if(self->ent->flip.x == 1)gf2d_special_projectile_spawn("images/zubat_attack.png",25,24,3,vector2d(self->ent->position.x-40,self->ent->position.y+20),vector2d(2,2),vector2d(-1.5,0),self->ent->flip,vector2d(26,24),24,20,1,30,1);
gfc_sound_play(ZubatAttack,0,.5,2,1);
}

void articuno_weapon_attack(Player* self){
	if(self->ent->flip.x == 0)gf2d_special_projectile_spawn("images/articuno_weapon_attack.png",20,10,3,vector2d(self->ent->position.x+130,self->ent->position.y+50),vector2d(2,2),vector2d(2,0),self->ent->flip,vector2d(24,10),16,8,1,30,2);
	if(self->ent->flip.x == 1)gf2d_special_projectile_spawn("images/articuno_weapon_attack.png",20,10,3,vector2d(self->ent->position.x,self->ent->position.y+50),vector2d(2,2),vector2d(-2,0),self->ent->flip,vector2d(24,10),16,8,1,30,2);
gfc_sound_play(ArticunoAttack,0,.5,2,1);
}

void pikachu_weapon_attack(Player* self){
	gf2d_special_projectile_spawn("images/pikachu_attack.png",22,203,3,vector2d(self->ent->position.x,self->ent->position.y-500),vector2d(2,2),vector2d(0,2),self->ent->flip,vector2d(24,204),16,202,1,30,3);
gfc_sound_play(PikachuAttack,0,.5,2,1);
}

void player_pickup(int tag, Player *self){
switch(tag){

case 2: self->ent->experience += 35;
break;

case 3: self->agumon_lives++;
break;

case 4: self->gabumon_lives++;
break;

case 5: self->guilmon_lives++;
break;

case 9: self->hp_pickups++;
break;
}
}
