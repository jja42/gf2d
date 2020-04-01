#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "gf2d_entity.h"

typedef struct Player_S{
Entity *ent;
void (*attack) (struct Player_S* self);   
void (*air_attack) (struct Player_S* self);
void (*special_attack) (struct Player_S* self);
void (*digivolve) (struct Player_S* self);
int move_end_frame;
int jump_start_frame;
int jump_end_frame;
int landing_frame;
int ground_attack_start_frame;
int ground_attack_end_frame;
int air_attack_start_frame;
int air_attack_end_frame;
int damaged_frame;
int digivolved;
float digi_timer;
void (*dedigivolve) (struct Player_S* self);
int agumon_lives;
int gabumon_lives;
int guilmon_lives;
int level;
int digimon;
int ground_attack_damage_frame;
int air_attack_damage_frame;
int zubat_weapon;
int articuno_weapon;
int pikachu_weapon;
int menu_timer;
}Player;

/**
 * @brief intialize the entity pointer for the player
 * @param the player to setup for
 */
void setup_player_ent(Player* self);

/**
 * @brief modify the player pointer to have info for agumon
 * @param the player pointer to be changed
 */
void load_agumon(Player* self);

/**
 * @brief modify the player pointer to have info for guilmon
 * @param the player pointer to be changed
 */
void load_guilmon(Player* self);

/**
 * @brief modify the player pointer to have info for gabumon
 * @param the player pointer to be changed
 */
void load_gabumon(Player* self);

/**
 * @brief modify the player pointer to have info for gabumon
 * @param the player pointer to be changed
 */
void load_wargreymon(Player* self);

/**
 * @brief modify the player pointer to have info for gabumon
 * @param the player pointer to be changed
 */
void load_gallantmon(Player* self);

/**
 * @brief modify the player pointer to have info for gabumon
 * @param the player pointer to be changed
 */
void load_etemon(Player* self);

void agumon_attack(Player* self);
void guilmon_attack(Player* self);
void gabumon_attack(Player* self);
void guilmon_air_attack(Player* self);
void gabumon_air_attack(Player* self);
void wargreymon_attack(Player* self);
void wargreymon_air_attack(Player* self);
void gallantmon_attack(Player* self);
void gallantmon_air_attack(Player* self);
void etemon_attack(Player* self);
void etemon_air_attack(Player* self);
void player_touch(Entity* self, Entity* other);
void player_pickup(int tag, Player* self);
void load_player_data(Player* self);
void articuno_weapon_attack(Player* self);
void zubat_weapon_attack(Player* self);
void pikachu_weapon_attack(Player* self);
#endif
