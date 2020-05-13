#include "simple_logger.h"
#include "menu.h"
#include "gf2d_draw.h"
#include "simple_json.h"
#include "simple_json_value.h"
#include "gf2d_entity.h"
#include "level.h"
#include "player.h"
#include "camera.h"
#include "gfc_audio.h"
#include "audio.h"
#include "SDL.h"
#include "enemy.h"
typedef struct 
{
    Uint32      maxMenus;         /**<Maximum number of entities*/
    Menu        *menuList;     /**<List of entities*/
    TTF_Font    *text;   
    MenuState	menu_state;
    float 		last_save;
    Player		*player;
	int 		menu_timer;
	char*		password[10];
	int			entity_selected;
	int			flip;
	Entity 		*enemy_list;
	Entity 		*platform_list;
}MenuManager;

static MenuManager menu_manager = {0};

Menu *menu_new(){
    int i;
    for (i=0; i < menu_manager.maxMenus; i++){
        if(menu_manager.menuList[i]._inuse)continue;
        menu_manager.menuList[i]._inuse = 1;
        menu_manager.menuList[i].text = 0;
        return &menu_manager.menuList[i];
    }
    slog("out of open menu slots in memory");
    return NULL;
}

void menu_manager_init(Uint32 maxMenus){
    if (menu_manager.menuList != NULL){
        //TODO: cleanup
    }
    if (!maxMenus){
        slog("cannot init 0 zise menu list");
        return;
    }
    menu_manager.menuList = (Menu * )malloc(sizeof(Menu) * maxMenus);
    if (menu_manager.menuList == NULL){
        slog("failed to allocate %i menus for the menu manager",maxMenus);
    }
    menu_manager.maxMenus = maxMenus;
    menu_manager.menu_state = MS_TitleScreen;
    menu_manager.last_save = 0;
    memset(menu_manager.menuList,0,sizeof(Menu)*maxMenus);
    menu_manager.player = (Player*)gf2d_entity_get(0)->data;
    slog("Menu manager initalized");
    menu_manager.menu_timer = 50;
    menu_manager.entity_selected = 0;
    menu_manager.flip = 0;
    menu_manager.enemy_list = (Entity*)gfc_allocate_array(sizeof(Entity),30);
    menu_manager.platform_list = (Entity*)gfc_allocate_array(sizeof(Entity),30);
    atexit(menu_manager_close);
}

void menu_manager_close(){
    int i;
    for (i=0; i < menu_manager.maxMenus; i++){
        if(menu_manager.menuList[i]._inuse){
            menu_free(&menu_manager.menuList[i]);
        }
    }
    menu_manager.maxMenus = 0;
    free(menu_manager.menuList);
    menu_manager.menuList =NULL;
    slog("menu manager closed");
}

void menu_free(Menu *self){
    if (!self)return;
    memset(self,0,sizeof(Menu));
}

void menu_update(Menu *self){
    if (!self)return;
    if (self->think){
        self->think(self);
    }
}

void menu_update_all(){
    int i;
    for (i = 0;i < menu_manager.maxMenus;i++)
    {
        if (!menu_manager.menuList[i]._inuse)continue;
        menu_update(&menu_manager.menuList[i]);
    }
}

void menu_draw(Menu *self){
    if(SDL_RenderCopy(gf2d_graphics_get_renderer(), self->Message, NULL, &self->box)){
    //slog("rendering %s", self->Message);
    return;
    }
}

void menu_draw_all()
{
    int i;
    for (i = 0;i < menu_manager.maxMenus;i++)
    {
        if (!menu_manager.menuList[i].text)continue;
        menu_draw(&menu_manager.menuList[i]);
    }
}

Menu *button_generate(
    void        (*think)(struct Menu_S *self),
	Box			*hitbox
){
    Menu *menu;
    menu = menu_new();
    menu->hitbox = hitbox;
    menu->think = think;
    return menu;
}

Menu *text_generate(
    SDL_Rect    box,
    void        (*think)(struct Menu_S *self),
    TTF_Font* Sans
){
    Menu *menu;
    menu = menu_new();
    menu->box = box;
    menu->think = think;
    menu->text = 1;
    menu->Sans = Sans;
    return menu;
}

void button_exit_think (Menu *self){
    if(get_menu_state() == MS_TitleScreen||get_menu_state() == MS_GameOver){
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_Exit;
        }
    }
}
}

void button_pause_exit_think (Menu *self){
    if(get_menu_state() == MS_Pause){
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_SelectScreen;
        }
    }
}
}

int collide_menu(Menu* self,Vector2D Mouse){
if ((Mouse.x <= self->hitbox->pos.x+self->hitbox->width) && (Mouse.x >= self->hitbox->pos.x-self->hitbox->width) && 
(Mouse.y <= self->hitbox->pos.y+self->hitbox->height) && (Mouse.y >= self->hitbox->pos.y-self->hitbox->height)) return 1;
else{
	return 0;}
}

void button_save_think (Menu *self){
    if(get_menu_state() == MS_Pause){
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) && menu_manager.last_save + 750 < SDL_GetTicks()) {
            menu_manager.last_save = SDL_GetTicks();
			Player* player = menu_manager.player;
			
            SJson *player_data;
            SJson *camera_offset;
            SJson *player_health;
            SJson *player_experience;
            SJson *player_agumon_lives;
            SJson *player_gabumon_lives;
            SJson *player_guilmon_lives;
            SJson *player_level;
            SJson *player_digitimer;
            SJson *player_digimon;
            SJson *player_digivolved;
            SJson *player_zubat_weapon;
            SJson *player_pikachu_weapon;
            SJson *player_articuno_weapon;
            SJson *player_lives;
            SJson *player_zubat_complete;
            SJson *player_articuno_complete;
            SJson *player_pikachu_complete;
            SJson *player_hp_pickups;

            player_data = sj_object_new();
            camera_offset = sj_array_new();
            player_health = sj_new_int(player->ent->health);
            player_experience = sj_new_int(player->ent->experience);
            player_agumon_lives = sj_new_int(player->agumon_lives);
            player_gabumon_lives = sj_new_int(player->gabumon_lives);
            player_guilmon_lives = sj_new_int(player->guilmon_lives);
            player_level = sj_new_int(player->level);
            player_digitimer = sj_new_float(player->digi_timer);
            player_digimon = sj_new_int(player->digimon);
            player_digivolved = sj_new_int(player->digivolved);
            player_zubat_weapon = sj_new_int(player->zubat_weapon);
            player_pikachu_weapon = sj_new_int(player->pikachu_weapon);
            player_articuno_weapon = sj_new_int(player->articuno_weapon);
            player_lives = sj_new_int(player->lives);
            player_zubat_complete = sj_new_int(player->zubat_completed);
            player_articuno_complete = sj_new_int(player->articuno_completed);
            player_pikachu_complete = sj_new_int(player->pikachu_completed);
            player_hp_pickups = sj_new_int(player->hp_pickups);
            
            sj_array_append(camera_offset,sj_new_float(get_camera_offset().x));
            sj_array_append(camera_offset,sj_new_float(get_camera_offset().y));
            sj_object_insert(player_data, "Offset", camera_offset);
            sj_object_insert(player_data, "Health", player_health);
            sj_object_insert(player_data, "Experience", player_experience);
            sj_object_insert(player_data, "AgumonLives", player_agumon_lives);
            sj_object_insert(player_data, "GabumonLives", player_gabumon_lives);
            sj_object_insert(player_data, "GuilmonLives", player_guilmon_lives);
            sj_object_insert(player_data, "Level", player_level);
            sj_object_insert(player_data, "DigiTimer", player_digitimer);
            sj_object_insert(player_data, "Digimon", player_digimon);
            sj_object_insert(player_data, "Digivolved", player_digivolved);
            sj_object_insert(player_data, "ZubatWeapon", player_zubat_weapon);
            sj_object_insert(player_data, "PikachuWeapon", player_pikachu_weapon);
            sj_object_insert(player_data, "ArticunoWeapon", player_articuno_weapon);
            sj_object_insert(player_data, "ArticunoComplete", player_articuno_complete);
            sj_object_insert(player_data, "ZubatComplete", player_zubat_complete);
            sj_object_insert(player_data, "PikachuComplete", player_pikachu_complete);
            sj_object_insert(player_data, "Lives", player_lives);
            sj_object_insert(player_data,"HpPickups",player_hp_pickups);
            sj_echo(player_data);
            sj_save(player_data, "sav/player.save");
            sj_free(player_data);
        }
    }
}
}

void button_pikachu_level_think (Menu *self){
    if(get_menu_state() == MS_SelectScreen && menu_manager.player->pikachu_completed!=1){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_None;
			load_level_pikachu();
        }
    }
	}
}

void button_articuno_level_think (Menu *self){
    if(get_menu_state() == MS_SelectScreen && menu_manager.player->articuno_completed!=1){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_None;
			load_level_articuno();
        }
    }
	}
}

void button_start_think(Menu *self){
	if(get_menu_state() == MS_TitleScreen||get_menu_state() == MS_PasswordScreen){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_SelectScreen;
        }
    }
}
}
void button_retry_think(Menu *self){
if(get_menu_state() == MS_GameOver){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			reset_player(menu_manager.player);
			reload_num_level(menu_manager.player->level);
			set_menu_state(MS_None);
        }
    }
}
}
void button_editor_think(Menu *self){
	if(get_menu_state() == MS_SelectScreen){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			set_camera_offset(vector2d(0,0));
			gf2d_level_clear();
			menu_manager.menu_state = MS_Editor;
			menu_manager.player->ent->position = vector2d(200,600);
			gf2d_platform_spawn(vector2d(200,742),vector2d(3,3));
			gf2d_door_spawn(5,vector2d(1275,600));
			gf2d_platform_spawn(vector2d(1250,742),vector2d(3,3));
        }
    }
}
}

void button_continue_think(Menu *self){
	if(get_menu_state() == MS_TitleScreen){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			load_player_data(menu_manager.player);
			if(menu_manager.player->level != 0){
			load_num_level(menu_manager.player->level);}
			menu_manager.menu_state = MS_None;
        }
    }
}
}

void button_editor_exit_think(Menu *self){
	if(get_menu_state() == MS_Editor){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_SelectScreen;
        }
    }
}
}

void button_editor_save_think(Menu *self){
	if(get_menu_state() == MS_Editor&& menu_manager.last_save + 250 < SDL_GetTicks()) {
    menu_manager.last_save = SDL_GetTicks();
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			SJson *level_data;
			SJson *player_position;
            SJson *player_level;
            SJson *platforms;
            SJson *enemies;

            level_data = sj_object_new();
            platforms = sj_array_new();
            enemies = sj_array_new();
            player_position = sj_array_new();
			sj_array_append(player_position,sj_new_float(150.0));
			sj_array_append(player_position,sj_new_float(400.0));
            player_level = sj_new_int(7);
        int i;
		for (i = 0; i < 30; i++)
			{
			if (menu_manager.platform_list[i]._inuse){
				SJson *platform_data = sj_object_new();
				SJson *platform_pos = sj_array_new();
				SJson *platform_scale = sj_array_new();
				sj_array_append(platform_pos,sj_new_float(menu_manager.platform_list[i].position.x));
				sj_array_append(platform_pos,sj_new_float(menu_manager.platform_list[i].position.y));
				sj_array_append(platform_scale,sj_new_float(menu_manager.platform_list[i].scale.x));
				sj_array_append(platform_scale,sj_new_float(menu_manager.platform_list[i].scale.y));
				sj_object_insert(platform_data, "Position", platform_pos);
				sj_object_insert(platform_data, "Scale", platform_scale);
				sj_array_append(platforms,platform_data);
			}
			}
		
		for (i = 0; i < 30; i++)
			{
			if (menu_manager.enemy_list[i]._inuse){
				SJson *enemy_data = sj_object_new();
				SJson *enemy_pos = sj_array_new();
				SJson *enemy_type = sj_new_int(menu_manager.enemy_list[i].type);
				SJson *left_bound = sj_new_int(menu_manager.enemy_list[i].position.x-30);
				SJson *right_bound = sj_new_int(menu_manager.enemy_list[i].position.x+30);
				SJson *flip = sj_new_int(menu_manager.enemy_list[i].flip.x);
				sj_array_append(enemy_pos,sj_new_float(menu_manager.enemy_list[i].position.x));
				sj_array_append(enemy_pos,sj_new_float(menu_manager.enemy_list[i].position.y));
				sj_object_insert(enemy_data, "Position", enemy_pos);
				sj_object_insert(enemy_data, "EnemyType", enemy_type);
				sj_object_insert(enemy_data, "LeftBound", left_bound);
				sj_object_insert(enemy_data, "RightBound", right_bound);
				sj_object_insert(enemy_data, "Flip", flip);
				sj_array_append(enemies,enemy_data);
			}
			}
            sj_object_insert(level_data, "PlayerPos", player_position);
            sj_object_insert(level_data, "PlayerLevel", player_level);
            sj_object_insert(level_data, "Platforms", platforms);
            sj_object_insert(level_data, "Enemies", enemies);
            sj_echo(level_data);
            sj_save(level_data, "levels/custom.level");
            sj_free(level_data);
        }
    }
}
}

void button_editor_betamon_think(Menu *self){
	if(get_menu_state() == MS_Editor){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.entity_selected = 1;
        }
    }
}
}

void button_editor_wormmon_think(Menu *self){
	if(get_menu_state() == MS_Editor){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.entity_selected = 2;
        }
    }
}
}

void button_editor_penguinmon_think(Menu *self){
	if(get_menu_state() == MS_Editor){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.entity_selected = 3;
        }
    }
}
}

void button_editor_platform_think(Menu *self){
	if(get_menu_state() == MS_Editor){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.entity_selected = 4;
        }
    }
}
}

void button_editor_place_think(Menu *self){
	if(get_menu_state() == MS_Editor){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
		if(menu_manager.menu_timer<=0){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_timer = 100;
			if(menu_manager.entity_selected == 0) gfc_sound_play(No,0,.25,5,1);
			if(menu_manager.entity_selected == 1) load_enemy(vector2d(mx,my),menu_manager.flip,1,mx-10,mx+10);
			if(menu_manager.entity_selected == 2) load_enemy(vector2d(mx,my),menu_manager.flip,3,0,0);
			if(menu_manager.entity_selected == 3) load_enemy(vector2d(mx,my),menu_manager.flip,2,mx-10,mx+10);
			if(menu_manager.entity_selected == 4) gf2d_platform_spawn(vector2d(mx,my),vector2d(3,3));
        }
	}
	else{
		menu_manager.menu_timer--;}
    }
}
}

void button_zubat_level_think(Menu *self){
	if(get_menu_state() == MS_SelectScreen && menu_manager.player->zubat_completed!=1){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_None;
			load_level_zubat();
        }
    }
	}
}

void button_hp_think(Menu *self){
	if(get_menu_state() == MS_Pause && menu_manager.player->hp_pickups > 0){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if(menu_manager.player->ent->health < menu_manager.player->ent->healthmax){
			menu_manager.player->ent->health+=10;
			menu_manager.player->hp_pickups -= 1;
			gfc_sound_play(PlayerRestore,0,.5,5,1);}
			else{
				return;}
        }
    }
	}
}

void button_articuno_weapon_think(Menu *self){
	if(get_menu_state() == MS_Pause && menu_manager.player->articuno_weapon){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.player->attack = articuno_weapon_attack;
			menu_manager.player->air_attack = articuno_weapon_attack;
			gfc_sound_play(PlayerRestore,0,.5,5,1);}
        }
    }
	}

void button_pikachu_weapon_think(Menu *self){
	if(get_menu_state() == MS_Pause && menu_manager.player->pikachu_weapon){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.player->attack = pikachu_weapon_attack;
			menu_manager.player->air_attack = pikachu_weapon_attack;
			gfc_sound_play(PlayerRestore,0,.5,5,1);}
        }
    }
	}

void button_zubat_weapon_think(Menu *self){
	if(get_menu_state() == MS_Pause && menu_manager.player->zubat_weapon){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.player->attack = zubat_weapon_attack;
			menu_manager.player->air_attack = zubat_weapon_attack;
			gfc_sound_play(PlayerRestore,0,.5,5,1);}
        }
    }
	}

void button_agumon_switch_think(Menu *self){
	if(get_menu_state() == MS_Pause && menu_manager.player->agumon_lives > 0 && menu_manager.player->digimon != 1){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			load_agumon(menu_manager.player);
			gfc_sound_play(PlayerRestore,0,.5,5,1);
			menu_manager.player->agumon_lives -= 1;}
        }
    }
	}

void button_gabumon_switch_think(Menu *self){
	if(get_menu_state() == MS_Pause && menu_manager.player->gabumon_lives > 0 && menu_manager.player->digimon != 3){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			load_gabumon(menu_manager.player);
			gfc_sound_play(PlayerRestore,0,.5,5,1);
			menu_manager.player->gabumon_lives -=1;}
        }
    }
	}

void button_guilmon_switch_think(Menu *self){
	if(get_menu_state() == MS_Pause && menu_manager.player->guilmon_lives > 0 && menu_manager.player->digimon != 2){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			load_guilmon(menu_manager.player);
			gfc_sound_play(PlayerRestore,0,.5,5,1);
			menu_manager.player->guilmon_lives -=1;}
        }
    }
	}

void button_password_think(Menu *self){
	if(get_menu_state() == MS_SelectScreen){
	if(menu_manager.menu_timer <= 0){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_PasswordScreen;
        }
    }
	}
else{
	menu_manager.menu_timer -= 1;
}
}
}

void button_password_zero_think(Menu *self){
	if(get_menu_state() == MS_PasswordScreen){
	if(menu_manager.menu_timer <= 0){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			strcat(menu_manager.password,"0");
			menu_manager.menu_timer = 50;
        }
    }
	}
else{
	menu_manager.menu_timer -= 1;
}
}
	}
	
	void button_password_one_think(Menu *self){
	if(get_menu_state() == MS_PasswordScreen){
	if(menu_manager.menu_timer <= 0){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			strcat(menu_manager.password,"1");
			menu_manager.menu_timer = 50;
        }
    }
	}
else{
	menu_manager.menu_timer -= 1;
}
}
	}

void button_password_enter_think (Menu *self){
    if(get_menu_state() == MS_PasswordScreen){
	if(menu_manager.menu_timer <= 0){
    int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if(!strcmp(menu_manager.password,"101")||!strcmp(menu_manager.password,"000")||!strcmp(menu_manager.password,"111")||!strcmp(menu_manager.password,"010")){
			gfc_sound_play(PauseMenu,0,.5,5,1);
			menu_manager.menu_timer = 50;
			if(!strcmp(menu_manager.password,"101"))menu_manager.player->articuno_completed = 1;
			if(!strcmp(menu_manager.password,"000"))menu_manager.player->zubat_completed = 1;
			if(!strcmp(menu_manager.password,"111"))menu_manager.player->pikachu_completed = 1;
			if(!strcmp(menu_manager.password,"010")){
				load_custom_level();
				menu_manager.menu_state = MS_None;}
		}
		else{
			gfc_sound_play(No,0,.5,5,1);
			menu_manager.menu_timer = 50;
		}
        menu_manager.password[0] = 0;}
    }
}
    else{
	menu_manager.menu_timer -= 1;
}
}
}
	
void xp_text_think(Menu *self){
	if(get_menu_state() == MS_Pause){
	char xp[16];
	snprintf(xp,16, "%i", menu_manager.player->ent->experience);
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &xp, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void agumon_lives_text_think(Menu *self){
if(get_menu_state() == MS_Pause){
	char lives[16];
	snprintf(lives,16, "x %i", menu_manager.player->agumon_lives);
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &lives, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void gabumon_lives_text_think(Menu *self){
if(get_menu_state() == MS_Pause){
	char lives[16];
	snprintf(lives,16, "x %i", menu_manager.player->gabumon_lives);
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &lives, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void guilmon_lives_text_think(Menu *self){
if(get_menu_state() == MS_Pause){
	char lives[16];
	snprintf(lives,16, "x %i", menu_manager.player->guilmon_lives);
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &lives, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void hp_count_text_think(Menu *self){
if(get_menu_state() == MS_Pause){
	char lives[16];
	snprintf(lives,16, "x %i", menu_manager.player->hp_pickups);
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &lives, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void life_text_think(Menu *self){
if(get_menu_state() == MS_Pause){
	char life[16];
	snprintf(life,16, "%i", menu_manager.player->ent->health);
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void lives_count_text_think(Menu *self){
if(get_menu_state() == MS_Pause){
	char life[16];
	snprintf(life,16, "%i", menu_manager.player->lives);
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void pikachu_weapon_text_think(Menu *self){
if(get_menu_state() == MS_Pause && menu_manager.player->pikachu_weapon){
	char life[16];
	snprintf(life,16, "Thunder");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void zubat_weapon_text_think(Menu *self){
if(get_menu_state() == MS_Pause && menu_manager.player->zubat_weapon){
	char life[16];
	snprintf(life,16, "Leech Life");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void articuno_weapon_text_think(Menu *self){
if(get_menu_state() == MS_Pause && menu_manager.player->articuno_weapon){
	char life[16];
	snprintf(life,16, "Ice Beam");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

void password_text_think(Menu *self){
if(get_menu_state() == MS_PasswordScreen){
	SDL_Color White = {255, 255, 255};  
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &menu_manager.password, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
else{
	char life[16];
	snprintf(life,16, " ");
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &life, White); 

    SDL_Texture* Message = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surfaceMessage); 

    self->Message = Message;
}
}

MenuState get_menu_state(){
	return menu_manager.menu_state;}
	
void	set_menu_state(MenuState state){
	menu_manager.menu_state = state;
}

void	set_menu_flip(){
	if(menu_manager.flip == 0)menu_manager.flip = 1;
	else{
		menu_manager.flip = 0;
	}
}

void editor_add_enemy(Entity* self, int enemy_type){
	int i;
	slog("Adding enemy of type: %i",enemy_type);
    for (i = 0; i < 30; i++)
    {
        if (menu_manager.enemy_list[i]._inuse)continue;
        //. found a free entry
        menu_manager.enemy_list[i] = *self;
        return;
    }
    slog("request for entity failed: all full up");
    return NULL;
}

void editor_add_platform(Entity* self){
	int i;
	slog("Adding platform");
    for (i = 0; i < 30; i++)
    {
        if (menu_manager.platform_list[i]._inuse)continue;
        //. found a free entry
        menu_manager.platform_list[i] = *self;
        return;
    }
    slog("request for entity failed: all full up");
    return NULL;
}
