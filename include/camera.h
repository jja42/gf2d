#ifndef __CAMERA_H__
#define __CAMERA_H__
#include "gf2d_entity.h"
#include "player.h"

typedef struct Camera_S{
int level_bounds_top;
int level_bounds_bottom;
int level_bounds_left;
int level_bounds_right;
Vector2D position;
Vector2D velocity;
Player		*player;
}Camera;

void camera_init();

void camera_update();

Vector2D get_camera_velocity();
#endif
