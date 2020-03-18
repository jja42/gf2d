#ifndef __GF2D_SHAPE_H__
#define __GF2D_SHAPE_H__

#include "gfc_vector.h"
#include "gf2d_sprite.h"
#include <SDL.h>

typedef struct
{
    Vector2D pos;
    float width,height;
    Sprite* sprite;
    Vector2D offset;
    Vector2D scale;
}Box;

/**
 * @brief make a GF2D Box pointer
 * @param pos the position of the box
 * @param w the width of the box
 * @param h the height of the box
 * @param offset vector offset to align with model
 * @param Ent the entity to associate with this box
 * @return a GF2D Box pointer
 */
Box *gf2d_box(Vector2D pos, float w, float h, Vector2D offset, Vector2D scale);

/**
 * @brief check if two boxes are overlapping
 * @param a pointer to box A
 * @param b pointer to box B
 * @return true if there is any overlap, false otherwise
 */
Uint8 gf2d_box_overlap(Box *a,Box *b);

/**
 * @brief update a box
 * @param r the Box 
 * @param pos position to update to
 * @return updated Box
 */
void gf2d_box_update(Box *r,Vector2D pos);

#endif
