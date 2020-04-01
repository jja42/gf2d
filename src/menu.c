#include "simple_logger.h"
#include "menu.h"
#include "gf2d_draw.h"
#include "simple_json.h"
#include "simple_json_value.h"
#include "gf2d_entity.h"
#include "level.h"
#include "player.h"

typedef struct 
{
    Uint32      maxMenus;         /**<Maximum number of entities*/
    Menu        *menuList;     /**<List of entities*/
    TTF_Font    *text;   
    MenuState	menu_state;
    float 		last_save;
    Player		*player;
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
    slog("rendering %s", self->Message);}
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
    if(get_menu_state() == MS_TitleScreen){
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
			menu_manager.menu_state = MS_Exit;
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
            SJson *player_pos;
            SJson *player_health;
            SJson *player_experience;
            SJson *player_agumon_lives;
            SJson *player_gabumon_lives;
            SJson *player_guilmon_lives;
            SJson *player_level;
            SJson *player_digitimer;
            SJson *player_digimon;
            SJson *player_digivolved;

            player_data = sj_object_new();
            player_pos = sj_array_new();
            player_health = sj_new_int(player->ent->health);
            player_experience = sj_new_int(player->ent->experience);
            player_agumon_lives = sj_new_int(player->agumon_lives);
            player_gabumon_lives = sj_new_int(player->gabumon_lives);
            player_guilmon_lives = sj_new_int(player->guilmon_lives);
            player_level = sj_new_int(player->level);
            player_digitimer = sj_new_float(player->digi_timer);
            player_digimon = sj_new_int(player->digimon);
            player_digivolved = sj_new_int(player->digivolved);
            
            sj_array_append(player_pos,sj_new_float(player->ent->position.x));
            sj_array_append(player_pos,sj_new_float(player->ent->position.y));
            sj_object_insert(player_data, "Position", player_pos);
            sj_object_insert(player_data, "Health", player_health);
            sj_object_insert(player_data, "Experience", player_experience);
            sj_object_insert(player_data, "AgumonLives", player_agumon_lives);
            sj_object_insert(player_data, "GabumonLives", player_gabumon_lives);
            sj_object_insert(player_data, "GuilmonLives", player_guilmon_lives);
            sj_object_insert(player_data, "Level", player_level);
            sj_object_insert(player_data, "DigiTimer", player_digitimer);
            sj_object_insert(player_data, "Digimon", player_digimon);
            sj_object_insert(player_data, "Digivolved", player_digivolved);
            sj_echo(player_data);
            sj_save(player_data, "sav/player.save");
            sj_free(player_data);
        }
    }
}
}

/*void button_pikachu_level_think (Menu *self){
    
}*/

/*void button_articuno_level_think (Menu *self){
    
}*/

void button_start_think(Menu *self){
	if(get_menu_state() == MS_TitleScreen){
	int mx,my;
    SDL_GetMouseState(&mx,&my);
    if (collide_menu(self, vector2d(mx,my))){
        if (SDL_GetMouseState(NULL, NULL) && SDL_BUTTON(SDL_BUTTON_LEFT)) {
			menu_manager.menu_state = MS_SelectScreen;
        }
    }
}
}

void button_zubat_level_think(Menu *self){
	if(get_menu_state() == MS_SelectScreen){
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

void xp_text_think(Menu *self){
	if(get_menu_state() == MS_Pause){
	char xp[16];
	snprintf(xp,16, "%i", menu_manager.player->ent->experience);
	SDL_Color White = {255, 255, 255};  

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(self->Sans, &xp, White); 

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
}

MenuState get_menu_state(){
	return menu_manager.menu_state;}
	
void	set_menu_state(MenuState state){
	menu_manager.menu_state = state;
}
