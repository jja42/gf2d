#ifndef __GF2D_DRAW_H__
#define __GF2D_DRAW_H__

#include <SDL.h>
#include "gfc_vector.h"
/**
 * @brief draw a pixel to the current rendering context
 * @param pixel the screen position to draw at
 * @param color the color to draw with
 */
void gf2d_draw_pixel(GFC_Vector2D pixel,GFC_Vector4D color);

void gf2d_draw_circle(GFC_Vector2D center, int radius, GFC_Vector4D color);

void gf2d_draw_rect(SDL_Rect rect,GFC_Vector4D color);

/**
 * @brief draw a line to the current rendering context
 * @param p1 starting point
 * @param p2 ending point
 * @param color the color to draw with
 */
void gf2d_draw_line(GFC_Vector2D p1,GFC_Vector2D p2, GFC_Vector4D color);

#endif
