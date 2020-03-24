#ifndef _MENU_H_
#define _MENU_H_

#include <stdlib.h>
#include "gf2d_graphics.h"
#include <SDL_ttf.h>
#include "gf2d_shape.h"
#include "player.h"

typedef enum
{
    MS_None = 0,
    MS_TitleScreen = 1,
    MS_SelectScreen = 2,
    MS_Exit = 3,
    MS_Pause = 4
}MenuState;

typedef struct Menu_S
{
    Uint8       _inuse;             /**<Check if entity in memory is active or not*/
    Uint8		text;
    Vector2D    position;           /**<2D position of entity*/
    SDL_Rect	box;
    Box			*hitbox;
    TextLine	textline;
    TTF_Font* 	Sans;
    SDL_Texture *Message;
    void        (*think)(struct Menu_S *self);

}Menu;

/**
 * @brief get a pointer to a new entity
 * @return NULL on out of memory or error, a pointer to a blank entity otherwise
 */
Menu *menu_new();

/**
 * @brief initialize entity resource manager
 * @param maxEnts maximum entities
 * */
void menu_manager_init(Uint32 maxEnts);

/**
 * @brief Close entity system
 */
void menu_manager_close();

/**
 * @brief free previously allocated entity
 * @param self a pointer to the entity to free
 */
void menu_free(Menu *menu);

/**
 * @brief Update frame info of menu
 * @param self a pointer to the entity to update
 */
void menu_update(Menu *self);

/**
 * @brief update every active menu
 */
void menu_update_all();

/**
 * @brief draw every active menu
 */
void menu_draw_all();


Menu *button_generate(
    void        (*think)(struct Menu_S *self),
    Box			*hitbox
);

void button_exit_think (Menu *self);

void button_pause_exit_think (Menu *self);

void button_save_think (Menu *self);

void button_level_think (Menu *self);

int collide_menu(Menu *self,Vector2D mouse);

void button_start_think(Menu *self);

MenuState get_menu_state();

void set_menu_state(MenuState state);

void xp_text_think(Menu *self);

void agumon_lives_text_think(Menu *self);

void gabumon_lives_text_think(Menu *self);

void guilmon_lives_text_think(Menu *self);

void life_text_think(Menu *self);

#endif
