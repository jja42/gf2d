#include "level.h"
#include "simple_logger.h"

void load_level_zubat(){
	Entity *platform_test = malloc(sizeof(Entity)); 
    platform_test = gf2d_entity_new();
    Entity *platform_test2 = malloc(sizeof(Entity)); 
    platform_test2 = gf2d_entity_new();
    Enemy *enemy_test = malloc(sizeof(Enemy)); 
    load_pikachu(enemy_test);
    
    gf2d_entity_load(platform_test,"images/box.png",40,40,1,vector2d(550,560),vector2d(3,3));
    gf2d_entity_load(platform_test2,"images/box.png",40,40,1,vector2d(750,400),vector2d(3,3));
    platform_test->box = gf2d_box(platform_test->position, 60, 60, vector2d(60,60));
    platform_test->touch = platform_touch;
    platform_test->tag = 6;
    platform_test2->box = gf2d_box(platform_test2->position, 60, 60, vector2d(60,60));
    platform_test2->touch = platform_touch;
    platform_test2->tag = 6;
    platform_test2->health = 20;
}
void load_level_pikachu(){
}
void load_level_articuno(){
}
