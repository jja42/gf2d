#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"
#include "gf2d_entity.h"
#include "touch.h"
#include "camera.h"
#include "enemy.h"
#include "menu.h"
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
	Vector2D veloc;
	if(self->tag == 7){
	 veloc = self->velocity;}
		
	if(self->colliding == 1 && self->tag!=6){self->velocity = vector2d(-.1,0);}
	if(self->colliding == 2 && self->tag!=6){self->velocity = vector2d(.1,0);}
	
	if(self->tag == 8)self->think(self);
	
	if(self->gravity == 1)self->velocity.y += 3;
	if(self->frozen > 0)self->velocity = vector2d(0,0);
	if(self->tag == 1){camera_update();}
	self->velocity.x -= get_camera_velocity().x;
	self->velocity.y -= get_camera_velocity().y;
	
	
	if(self->tag!=1)vector2d_set(self->position,self->position.x+ self->velocity.x,self->position.y + self->velocity.y);
	
	gf2d_box_update(self->box,self->position);
	if(self->invincibility > 0)self->invincibility-=1;
	if(self->tag != 7)self->velocity = vector2d(0,0);
	if(self->tag == 7)self->velocity = veloc;
	if(self->duration<99){self->duration-=.1;
		self->frame+=.1;
		if(self->frame>=3)self->frame =0;}
	if(self->duration<0)gf2d_entity_free(self);
	if(self->tag == 1 || self->tag == 8){
		self->gravity = 1;
		self->colliding = 0;
	if(self->frozen > 0){//slog("%i",self->frozen);
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
        self->data = NULL;
        //slog("warning: data not freed at entity free!");
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

void gf2d_platform_spawn(Vector2D position, Vector2D scale){
	Entity* self = gf2d_entity_new();
	gf2d_entity_load(self,"images/platform.png",48,16,1,position,scale);
	self->box = gf2d_box(self->position, 24*scale.x, 8*scale.y, vector2d(24*scale.x,8*scale.y));
    self->touch = platform_touch;
    self->tag = 6;
    if(get_menu_state() == MS_Editor)editor_add_platform(self);
}

void gf2d_enemy_spawn(Vector2D position, int enemy_type, int patrol_bound_left, int patrol_bound_right, int flip){
	patrol_bound_left -= get_camera_offset().x;
	patrol_bound_right -= get_camera_offset().x;
	load_enemy(position,flip,enemy_type,patrol_bound_left,patrol_bound_right);
}

void gf2d_door_spawn(int level, Vector2D position){
	Entity* self = gf2d_entity_new();
	gf2d_entity_load(self,"images/door.png",40,70,1,position,vector2d(2,2));
	self->box = gf2d_box(self->position,40,70,vector2d(40,70));
	self->touch = door_touch;
	self->special = level;
	self->tag = 6;
}

void gf2d_apply_offset(){
int i;
    for (i = 1; i < gf2d_entity_manager.entity_max;i++)
    {
        if (gf2d_entity_manager.entity_list[i]._inuse == 0)continue;
        vector2d_set(gf2d_entity_manager.entity_list[i].position,gf2d_entity_manager.entity_list[i].position.x - get_camera_offset().x,gf2d_entity_manager.entity_list[i].position.y - get_camera_offset().y);
    }
}

void gf2d_level_clear(){
int i;
    for (i = 1; i < gf2d_entity_manager.entity_max;i++)
    {
        if (gf2d_entity_manager.entity_list[i]._inuse == 0)continue;
        gf2d_entity_free(&gf2d_entity_manager.entity_list[i]);
    }
}

/*eol@eof*/
