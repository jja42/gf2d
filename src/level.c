#include "level.h"
#include "simple_logger.h"

void load_level_zubat(){
	Entity *platform_test = malloc(sizeof(Entity)); 
    platform_test = gf2d_entity_new();
    Entity *platform_test2 = malloc(sizeof(Entity)); 
    platform_test2 = gf2d_entity_new();
    Enemy *enemy_test = malloc(sizeof(Enemy)); 
    
    load_wormmon(enemy_test,vector2d(750,200),vector2d(1,0));
    
    gf2d_entity_load(platform_test,"images/platform.png",48,16,1,vector2d(550,560),vector2d(3,3));
    gf2d_entity_load(platform_test2,"images/platform.png",48,16,1,vector2d(750,400),vector2d(3,3));
    platform_test->box = gf2d_box(platform_test->position, 72, 24, vector2d(72,24));
    platform_test->touch = platform_touch;
    platform_test->tag = 6;
    platform_test2->box = gf2d_box(platform_test2->position, 72, 24, vector2d(72,24));
    platform_test2->touch = platform_touch;
    platform_test2->tag = 6;
}
void load_level_pikachu(){
}
void load_level_articuno(){
}
