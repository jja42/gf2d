#ifndef _MENU_H_
#define _MENU_H_

#include <stdlib.h>
#include "gf2d_sprite.h"
#include "gf2d_graphics.h"
#include <SDL_ttf.h>
#include "gf2d_shape.h"

typedef enum
{
    MS_None = 0,
    MS_TitleScreen = 1,
    MS_SelectScreen = 2,
    MS_Exit = 3
}MenuState;

typedef struct Menu_S
{
    Uint8       _inuse;             /**<Check if entity in memory is active or not*/
    Vector2D    position;           /**<2D position of entity*/
    SDL_Rect	box;
    Box			*hitbox;
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


Menu *menu_generic(
    SDL_Rect    box,
    void        (*think)(struct Menu_S *self),
    TTF_Font* Sans,
    TextLine     text,
    Box			*hitbox
);

void button_exit_think (Menu *self);

void button_save_think (Menu *self);

void button_level_think (Menu *self);

int collide_menu(Menu *self,Vector2D mouse);

void think();

MenuState get_menu_state();
#endif
