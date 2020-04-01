#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"
#include "gf2d_entity.h"
#include "touch.h"
#include "camera.h"
#include "enemy.h"
typedef struct
{
    Entity *entity_list;
    int  entity_max;
}EntityManager;

static EntityManager gf2d_entity_manager = {0};

void gf2d_entity_manager_close()
{
    if(gf2d_entity_manager.entity_list != NULL)
    {
        free(gf2d_entity_manager.entity_list);
    }
    memset(&gf2d_entity_manager,0,sizeof(EntityManager));
}

void gf2d_entity_manager_init(Uint32 entity_max)
{
    gf2d_entity_manager.entity_max = entity_max;
    gf2d_entity_manager.entity_list = (Entity*)gfc_allocate_array(sizeof(Entity),entity_max);
    if (!gf2d_entity_manager.entity_list)
    {
        slog("failed to allocate entity list");
        return;
    }
    atexit(gf2d_entity_manager_close);
}

Entity *gf2d_entity_new()
{
    int i;
    for (i = 0; i < gf2d_entity_manager.entity_max; i++)
    {
        if (gf2d_entity_manager.entity_list[i]._inuse)continue;
        //. found a free entity
        memset(&gf2d_entity_manager.entity_list[i],0,sizeof(Entity));
        gf2d_entity_manager.entity_list[i]._inuse = 1;
        return &gf2d_entity_manager.entity_list[i];
    }
    slog("request for entity failed: all full up");
    return NULL;
}

void gf2d_entity_draw(Entity *self)
{
    if (!self)return;
    if (!self->_inuse)return;

    gf2d_sprite_draw(
        self->sprite,
        self->position,
        &self->scale,
		NULL,
        &self->rotation,
        &self->flip,
        NULL,
        self->frame);
}

void gf2d_entity_draw_all()
{
    int i;
    for (i = 0; i < gf2d_entity_manager.entity_max;i++)
    {
        if (gf2d_entity_manager.entity_list[i]._inuse == 0)continue;
        gf2d_entity_draw(&gf2d_entity_manager.entity_list[i]);
    }
}


void gf2d_entity_load(Entity *ent, char* filename, int width, int height, int frames_per_line, Vector2D pos, Vector2D scale)
{   
    ent->sprite = gf2d_sprite_load_all(filename,width,height,frames_per_line);
    ent->position = pos;
    ent->scale = scale;
    ent->duration = 99;
}

void gf2d_pickup_spawn(char* filename, int width, int height, int frames_per_line, Vector2D pos, Vector2D scale, Vector2D velocity,Vector2D flip,Vector2D boxoffset, int boxw, int boxh, int tag, int duration){
Entity *ent = gf2d_entity_new();
	ent->sprite = gf2d_sprite_load_all(filename,width,height,frames_per_line);
    ent->position = pos;
    ent->scale = scale;
    ent->duration = duration;
    ent->flip = flip;
    ent->velocity = velocity;
    ent->box = gf2d_box(pos,boxw,boxh,boxoffset);
    ent->touch = pickup_touch;
    ent->tag = tag;
}

void gf2d_projectile_spawn(char* filename, int width, int height, int frames_per_line, Vector2D pos, Vector2D scale, Vector2D velocity,Vector2D flip,Vector2D boxoffset, int boxw, int boxh, float owner, int duration){
	Entity *ent = gf2d_entity_new();
	ent->sprite = gf2d_sprite_load_all(filename,width,height,frames_per_line);
    ent->position = pos;
    ent->scale = scale;
    ent->duration = duration;
    ent->flip = flip;
    ent->velocity = velocity;
    ent->box = gf2d_box(pos,boxw,boxh,boxoffset);
    ent->owner = owner;
    ent->touch = projectile_touch;
    ent->tag = 7;
}
void gf2d_special_projectile_spawn(char* filename, int width, int height, int frames_per_line, Vector2D pos, Vector2D scale, Vector2D velocity,Vector2D flip,Vector2D boxoffset, int boxw, int boxh, float owner, int duration, int special){
	Entity *ent = gf2d_entity_new();
	ent->sprite = gf2d_sprite_load_all(filename,width,height,frames_per_line);
    ent->position = pos;
    ent->scale = scale;
    ent->duration = duration;
    ent->flip = flip;
    ent->velocity = velocity;
    ent->box = gf2d_box(pos,boxw,boxh,boxoffset);
    ent->owner = owner;
    ent->touch = projectile_touch;
    ent->tag = 7;
    ent->special = special;
}

void gf2d_entity_update_all(){
	int i;
    for (i = 0; i < gf2d_entity_manager.entity_max;i++)
    {
        if (gf2d_entity_manager.entity_list[i]._inuse == 0)continue;
        gf2d_entity_update(&gf2d_entity_manager.entity_list[i]);
    }
    gf2d_basic_collision();
}

void gf2d_entity_update(Entity *self){
	if(self->tag != 1 && self->tag !=7) {
		self->velocity.x -= get_camera_velocity().x;
		self->velocity.y -= get_camera_velocity().y;}
		
	if(self->tag == 7 && self->owner != 5 && self->owner != 4 && self->special != 3){
		self->velocity.y = -get_camera_velocity().y;}
		
	if(self->colliding == 1){self->velocity = vector2d(-.1,0);}
	if(self->colliding == 2){self->velocity = vector2d(.1,0);}
	
	if(self->tag == 8)self->think(self);
	
	if(self->gravity == 1)self->velocity.y += 3;
	
	if(self->frozen == 0)vector2d_set(self->position,self->position.x+ self->velocity.x,self->position.y + self->velocity.y);
	else{self->velocity = vector2d(0,0);}
	
	gf2d_box_update(self->box,self->position);
	if(self->tag == 1){camera_update();}
	if(self->invincibility > 0)self->invincibility-=1;
	if(self->tag != 7)self->velocity = vector2d(0,0);
	if(self->duration<99){self->duration-=.1;
		self->frame+=.1;
		if(self->frame>=3)self->frame =0;}
	if(self->duration<0)gf2d_entity_free(self);
	if(self->tag == 1 || self->tag == 8){
		self->gravity = 1;
		self->colliding = 0;
	if(self->frozen > 0){slog("%i",self->frozen);
		self->frozen-=1;}
	}
}
int gf2d_entity_max(){
    if (gf2d_entity_manager.entity_max){
        return gf2d_entity_manager.entity_max;
    }
    else{
        return 0;}
}

Entity *gf2d_entity_get(int i){
    if (gf2d_entity_manager.entity_list[i]._inuse == 1){
        return &gf2d_entity_manager.entity_list[i];
    }
    return NULL;
}
void gf2d_entity_free(Entity *self)
{
    if (!self)
    {
        slog("self pointer is not valid");
        return;
    }
    self->_inuse = 0;
    gf2d_sprite_free(self->sprite);
    if (self->data != NULL)
    {
        slog("warning: data not freed at entity free!");
    }
}

void gf2d_basic_collision(){
for(int j = 0; j < gf2d_entity_manager.entity_max; j++){
	for(int k = j+1; k < gf2d_entity_manager.entity_max; k++){
		Entity* jent = &gf2d_entity_manager.entity_list[j];
		Entity* kent = &gf2d_entity_manager.entity_list[k];
		if(jent->_inuse == 0 || kent->_inuse == 0)continue;
		if(gf2d_box_overlap(gf2d_entity_manager.entity_list[j].box,gf2d_entity_manager.entity_list[k].box)){
		jent->touch(jent,kent);
		kent->touch(kent,jent);
	}
	}
}
}


/*eol@eof*/
