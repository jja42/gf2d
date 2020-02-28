#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gf2d_sprite.h"

typedef struct Player_S{
Entity *ent;
void (*attack) (struct Player_S* self);   
void (*move) (struct Player_S* self);
void (*jump) (struct Player_S* self);
}Player;
