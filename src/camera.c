#include "camera.h"
#include "simple_logger.h"
static Camera camera = {0};
void camera_init(){
camera.player = (Player*)gf2d_entity_get(0)->data;
camera.position = camera.player->ent->position;
}

void camera_update(){
camera.position = camera.player->ent->position;
camera.velocity.x = camera.player->ent->velocity.x*.5;
camera.velocity.y = camera.player->ent->velocity.y*.5;
}

Vector2D get_camera_velocity(){
return camera.velocity;
}
