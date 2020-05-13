#ifndef __LEVEL_H__
#define __LEVEL_H__
#include "touch.h"
#include "gf2d_entity.h"
#include "enemy.h"
void load_level_zubat();
void load_level_pikachu();
void load_level_articuno();
void load_level_zubat_boss();
void load_level_pikachu_boss();
void load_level_articuno_boss();
void load_level(char* level_name);
void load_num_level(int num);
void reload_num_level(int num);
void load_boss_level(int num);
void load_custom_level();
#endif
