#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "gf2d_entity.h"
typedef struct Enemy_S{
Entity *ent;
int move_end_frame;
int jump_start_frame;
int jump_end_frame;
int landing_frame;
int attack_start_frame;
int attack_end_frame;
int damaged_frame;
int patrol_bound_left;
int patrol_bound_right;
int attack_timer;
int enemy_type;
}Enemy;

void load_enemy_data(Enemy* self);

void load_betamon(Enemy* self,Vector2D position, int patrol_bound_left, int patrol_bound_right);

void load_penguinmon(Enemy* self,Vector2D position, int patrol_bound_left, int patrol_bound_right);

void load_wormmon(Enemy* self,Vector2D position, Vector2D flip);

void load_zubat(Enemy* self);

void load_pikachu(Enemy* self);

void load_articuno(Enemy* self);

void betamon_think(Entity* self);

void wormmon_think(Entity* self);

void penguinmon_think(Entity* self);

void zubat_think(Entity* self);

void articuno_think(Entity* self);

void pikachu_think(Entity* self);
#endif
