#include <stdlib.h>
#include <string.h>

#include "simple_logger.h"
#include "simple_json.h"
#include "gf2d_entity.h"

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
if(self->box != NULL){
   gf2d_sprite_draw(
		self->box->sprite,
		self->box->pos,
		&self->box->scale,
		NULL,
		NULL,
		NULL,
		NULL,
		0);
	}
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

Entity *gf2d_entity_load_from_file(char *filename)
{
    TextLine assetname;
    snprintf(assetname,GFCLINELEN,"entities/%s.json",filename);
    Entity *ent = NULL;
    ent = gf2d_entity_new();
    if (!ent)
    {
        return NULL;
    }
    SJson *json,*ent_info;
    if (!filename)return NULL;
    json = sj_load(assetname);
    if (!json)
    {
        slog("failed to entity file %s",filename);
        return NULL;
    }
    ent_info = sj_object_get_value(json,"ent_info");
    if (!ent_info)
    {
        slog("missing entity info in entity file %s",filename);
        sj_free(json);
        gf2d_entity_free(ent);
        return NULL;
    }
    strncpy(ent->name,sj_get_string_value(sj_object_get_value(ent_info,"name")),GFCLINELEN);
    
    sj_get_float_value((ent_info,"position.x"),&ent->position.x);
    sj_get_float_value((ent_info,"position.y"),&ent->position.y);
    sj_free(json);
    slog("loaded ent info for %s",filename);
    return ent;
}


void gf2d_entity_load(Entity *ent, char* filename, int width, int height, int frames_per_line, Vector2D pos, Vector2D scale)
{   
    ent->sprite = gf2d_sprite_load_all(filename,width,height,frames_per_line);
    ent->position = pos;
    ent->scale = scale;
    ent->duration = 99;
}

void gf2d_entity_spawn(char* filename, int width, int height, int frames_per_line, Vector2D pos, Vector2D scale, Vector2D velocity,Vector2D flip,Vector2D boxscale,Vector2D boxoffset){
	Entity *ent = gf2d_entity_new();
	ent->sprite = gf2d_sprite_load_all(filename,width,height,frames_per_line);
    ent->position = pos;
    ent->scale = scale;
    ent->duration = 30;
    ent->flip = flip;
    ent->velocity = velocity;
    ent->box = gf2d_box(pos,5,5,boxoffset,boxscale);
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
	vector2d_set(self->position,self->position.x+ self->velocity.x,self->position.y + self->velocity.y);
	gf2d_box_update(self->box,self->position);
	if(self->duration<99){self->duration-=.1;
		self->frame+=.1;
		if(self->frame>=3)self->frame =0;}
	if(self->duration<0)gf2d_entity_free(self);
}
int gf2d_entity_max(){
    if (gf2d_entity_manager.entity_max){
        return gf2d_entity_manager.entity_max;
    }
    else{
        return 0;}
}

Entity *gf2d_entity_get(int i){
    if (gf2d_entity_manager.entity_list[i]._set == 1){
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
		if(gf2d_entity_manager.entity_list[j]._inuse == 0 || gf2d_entity_manager.entity_list[k]._inuse == 0)continue;
		if(gf2d_box_overlap(gf2d_entity_manager.entity_list[j].box,gf2d_entity_manager.entity_list[k].box)){
		slog("touching tips");}
	}
}
}


/*eol@eof*/
