#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "gf2d_entity.h"
#include "player.h"
#include "gfc_vector.h"

typedef struct Camera_S{
int level_bounds_top;
int level_bounds_bottom;
int level_bounds_left;
int level_bounds_right;
GFC_Vector2D offset;
GFC_Vector2D velocity;
Player		*player;
}Camera;

void camera_init();

void camera_update();

GFC_Vector2D get_camera_velocity();

GFC_Vector2D get_camera_offset();

void set_camera_offset(GFC_Vector2D offset);
#endif
