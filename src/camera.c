#include "camera.h"
#include "simple_logger.h"
static Camera camera = {0};
void camera_init(){
camera.player = (Player*)gf2d_entity_get(0)->data;
camera.offset = vector2d(0,0);
}

void camera_update(){
camera.velocity.x = camera.player->ent->velocity.x;
camera.velocity.y = camera.player->ent->velocity.y;
camera.offset.x += camera.velocity.x ;
camera.offset.y += camera.velocity.y;
//slog("offset x: %f, offset y: %f",camera.offset.x,camera.offset.y);
}

Vector2D get_camera_velocity(){
return camera.velocity;
}

Vector2D get_camera_offset(){
return camera.offset;
}

void set_camera_offset(Vector2D offset){
	camera.offset = offset;
}
