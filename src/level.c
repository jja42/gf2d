#include "level.h"
#include "simple_logger.h"
#include "simple_json.h"
#include "player.h"

void load_level_zubat(){
	load_level("levels/zubat.level");
}
void load_level_pikachu(){
	load_level("levels/pikachu.level");
}
void load_level_articuno(){
	load_level("levels/articuno.level");
}
void load_level(char* level_name){
	SJson *levelFile = sj_load(level_name);
	if(levelFile){
	Player *p = (Player*)gf2d_entity_get(0)->data;
	SJson *player_pos = sj_object_get_value(levelFile, "PlayerPos");
	SJson *player_level = sj_object_get_value(levelFile, "PlayerLevel");
	 SJson *playerposX;
     SJson *playerposY;
     playerposX = sj_array_get_nth(player_pos, 0);
     playerposY = sj_array_get_nth(player_pos, 1);
	 sj_get_float_value(playerposX, &p->ent->position.x);
     sj_get_float_value(playerposY, &p->ent->position.y);
     sj_get_integer_value(player_level,&p->level);
     if(p->level == 0){
     set_camera_offset(vector2d(0,0));
	}
	SJson *Platform_array = sj_object_get_value(levelFile, "Platforms");
	SJson *Enemy_array = sj_object_get_value(levelFile, "Enemies");
	
	for (int i = 0;i < sj_array_get_count(Platform_array);i++){
		SJson *platform_data = sj_array_get_nth(Platform_array,i);
		SJson *platform_pos = sj_object_get_value(platform_data, "Position");
		SJson *platform_scale = sj_object_get_value(platform_data, "Scale");
		SJson *platformposX;
		SJson *platformposY;
		SJson *platformscaleX;
		SJson *platformscaleY;
		platformposX = sj_array_get_nth(platform_pos, 0);
		platformposY = sj_array_get_nth(platform_pos, 1);
		platformscaleX = sj_array_get_nth(platform_scale, 0);
		platformscaleY = sj_array_get_nth(platform_scale, 1);
		float platform_x;
		float platform_y;
		float platformscalex;
		float platformscaley;
		sj_get_float_value(platformposX, &platform_x);
		sj_get_float_value(platformposY, &platform_y);
		sj_get_float_value(platformscaleX,&platformscalex);
		sj_get_float_value(platformscaleY,&platformscaley);
		gf2d_platform_spawn(vector2d(platform_x,platform_y),vector2d(platformscalex,platformscaley));
	}
	for (int j = 0;j < sj_array_get_count(Enemy_array);j++){
		SJson *enemy_data = sj_array_get_nth(Enemy_array,j);
		SJson *enemy_pos = sj_object_get_value(enemy_data, "Position");
		SJson *enemy_type = sj_object_get_value(enemy_data, "EnemyType");
		SJson *enemy_left_bound = sj_object_get_value(enemy_data, "LeftBound");
		SJson *enemy_right_bound = sj_object_get_value(enemy_data, "RightBound");
		SJson *enemyposX;
		SJson *enemyposY;
		SJson *enemyflip = sj_object_get_value(enemy_data,"Flip");
		enemyposX = sj_array_get_nth(enemy_pos, 0);
		enemyposY = sj_array_get_nth(enemy_pos, 1);
		float enemy_x;
		float enemy_y;
		int enemytype;
		int enemy_leftbound;
		int enemy_rightbound;
		int enemy_flip;
		sj_get_float_value(enemyposX, &enemy_x);
		sj_get_float_value(enemyposY, &enemy_y);
		sj_get_integer_value(enemy_type,&enemytype);
		sj_get_integer_value(enemy_left_bound,&enemy_leftbound);
		sj_get_integer_value(enemy_right_bound,&enemy_rightbound);
		sj_get_integer_value(enemyflip,&enemy_flip);
		gf2d_enemy_spawn(vector2d(enemy_x,enemy_y),enemytype,enemy_leftbound,enemy_rightbound,enemy_flip);
	}
}
gf2d_apply_offset();
}

void load_num_level(int num){
Player *p = (Player*)gf2d_entity_get(0)->data;
load_player_data(p);
switch(num){
case 1: load_level_zubat();
break;
case 2: load_level_zubat_boss();
break;
case 3: load_level_articuno();
break;
case 4: load_level_articuno_boss();
break;
case 5: load_level_pikachu();
break;
case 6: load_level_pikachu_boss();
break;
}
}

void load_level_articuno_boss(){
}

void load_level_zubat_boss(){
}

void load_level_pikachu_boss(){
}
