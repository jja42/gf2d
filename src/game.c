#include <SDL.h>
#include <SDL_ttf.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_entity.h"
#include "player.h"
#include "touch.h"
#include "menu.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Uint8 grounded = 1;
    Uint8 landing = 0;
    Uint8 attacking = 0;
    Uint8 double_jump = 0;
    Sprite *background;
    Sprite *select_screen;
    Sprite *title_screen;
    Sprite *pause_screen;
    
    int mx,my;
    float recov = 0;
    float air = 0;
    float mf = 0;
    float change_timer = 0;
    float current_time = 0;
    float pause_timer = 0;
    float jump_timer = 0;
    Sprite *mouse;
    Player *player = malloc(sizeof(Player));
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1600,
        900,
        1600,
        900,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*setup*/
    gf2d_entity_manager_init(100);
    setup_player_ent(player);
    menu_manager_init(10);
    
    button_generate(button_start_think,gf2d_box(vector2d(800,495),128,38,vector2d(0,0)));
    button_generate(button_exit_think,gf2d_box(vector2d(800,610),128,38,vector2d(0,0)));
    button_generate(button_level_think,gf2d_box(vector2d(262,546),110,93,vector2d(0,0)));
    button_generate(button_pause_exit_think,gf2d_box(vector2d(1090,574),56,23,vector2d(0,0)));
    button_generate(button_save_think,gf2d_box(vector2d(506,574),68,20,vector2d(0,0)));
    
    TTF_Init();
    TTF_Font* Sans = TTF_OpenFont("fonts/megaman_2.ttf", 60);
    SDL_Rect xp_counter = { 475, 730, 50, 45};
    SDL_Rect agumon_lives_counter = { 1000, 155, 100, 45};
    SDL_Rect gabumon_lives_counter = { 1000, 280, 100, 45};
    SDL_Rect guilmon_lives_counter = { 1000, 425, 100, 45};
    SDL_Rect life_counter = {1030,730,100,45};
    text_generate(agumon_lives_counter,agumon_lives_text_think,Sans);
    text_generate(gabumon_lives_counter,gabumon_lives_text_think,Sans);
    text_generate(guilmon_lives_counter,guilmon_lives_text_think,Sans);
    text_generate(xp_counter,xp_text_think,Sans);
    text_generate(life_counter,life_text_think,Sans);
    
    Entity *platform_test = malloc(sizeof(Entity)); 
    platform_test = gf2d_entity_new();
    Entity *platform_test2 = malloc(sizeof(Entity)); 
    platform_test2 = gf2d_entity_new();
    Entity *enemy_test = malloc(sizeof(Entity)); 
    enemy_test = gf2d_entity_new();
    
    background = gf2d_sprite_load_image("images/backgrounds/sky_back.png");
    select_screen = gf2d_sprite_load_image("images/backgrounds/selectscreen.png");
    title_screen = gf2d_sprite_load_image("images/backgrounds/titlescreen.png");
    pause_screen = gf2d_sprite_load_image("images/backgrounds/pausescreen.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    
    gf2d_entity_load(platform_test,"images/box.png",40,40,1,vector2d(550,560),vector2d(3,3));
    gf2d_entity_load(platform_test2,"images/box.png",40,40,1,vector2d(750,400),vector2d(3,3));
    gf2d_entity_load(enemy_test,"images/aguman.png",48,48,11,vector2d(750,100),vector2d(3,3));
    platform_test->box = gf2d_box(platform_test->position, 60, 60, vector2d(60,60));
    platform_test->touch = platform_touch;
    platform_test->tag = 6;
    platform_test2->box = gf2d_box(platform_test2->position, 60, 60, vector2d(60,60));
    platform_test2->touch = platform_touch;
    platform_test2->tag = 6;
    enemy_test->touch = player_touch;
    enemy_test->box = gf2d_box(enemy_test->position, 30, 39, vector2d(72,105));
    enemy_test->health = 50;
    enemy_test->tag = 8;
    
    /*main game loop*/
    while(!done && get_menu_state() != MS_Exit)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        //===================GROUNDED START========================
        if(get_menu_state() == MS_None){
			current_time = SDL_GetTicks();
		if(player->digivolved == 1){
			player->digi_timer -=1;
			if (player->digi_timer == 0){
				player->dedigivolve(player);}}
        if(keys[SDL_SCANCODE_1] && grounded && !attacking){
        if(current_time > change_timer && player->agumon_lives > 0){load_agumon(player);
        change_timer = SDL_GetTicks() + 500;}
		}
		if(keys[SDL_SCANCODE_2] && grounded && !attacking){
        if(current_time > change_timer && player->guilmon_lives > 0){load_guilmon(player);
        change_timer = SDL_GetTicks() + 500;}
		}
		if(keys[SDL_SCANCODE_3] && grounded && !attacking){
        if(current_time > change_timer && player->gabumon_lives > 0){load_gabumon(player);
        change_timer = SDL_GetTicks() + 500;}
		}
		if(keys[SDL_SCANCODE_4] && grounded && !attacking){
        if(current_time > change_timer){load_wargreymon(player);
        change_timer = SDL_GetTicks() + 500;}
		}
		if(keys[SDL_SCANCODE_5] && grounded && !attacking){
        if(current_time > change_timer){load_gallantmon(player);
        change_timer = SDL_GetTicks() + 500;}
		}
		if(keys[SDL_SCANCODE_6] && grounded && !attacking){
        if(current_time > change_timer){load_etemon(player);
        change_timer = SDL_GetTicks() + 500;}
		}
        if (keys[SDL_SCANCODE_Z]&& !attacking && player->ent->gravity == 0){
			grounded = 0;
			jump_timer = current_time + 200;
		}
		if (keys[SDL_SCANCODE_D]&& !attacking && grounded && player->ent->experience >= 100){
			player->digivolve(player);
			player->ent->experience -= 100;
		}
		if(keys[SDL_SCANCODE_Z] && double_jump != 1 && current_time > jump_timer){
			double_jump = 1;
			air = 0;
			}
		if (keys[SDL_SCANCODE_X]&& !attacking){
			attacking = 1;
		}
		if ((keys[SDL_SCANCODE_LEFT]||keys[SDL_SCANCODE_RIGHT]) && grounded && !attacking){
		if (keys[SDL_SCANCODE_RIGHT]){
			player->ent->flip.x = 0;
			player->ent->frame+=0.1;
			if(player->ent->frame>=player->move_end_frame)player->ent->frame=1;
			player->ent->velocity.x=3;
			}
		else{
        if (keys[SDL_SCANCODE_LEFT]){
			player->ent->flip.x = 1;
			player->ent->frame+=0.1;
			if(player->ent->frame>=player->move_end_frame)player->ent->frame=1;
			player->ent->velocity.x=-3;
			}
		}
		}
		else{
		if(grounded && !attacking)player->ent->frame = 0;}
		//===================GROUNDED END========================
		
		
		//===================JUMPING START========================
		if(!grounded && !landing){
		air+=.1;
		if(player->ent->frame<player->jump_start_frame)player->ent->frame=player->jump_start_frame;
		if(player->ent->frame<=player->jump_end_frame)player->ent->frame+=.1;
			if(air<6){
			player->ent->velocity.y=-7;
			}
		else{if(player->ent->gravity == 0){
			air = 0;
			landing = 1;
			recov = 1;
		}}
	
		if ((keys[SDL_SCANCODE_LEFT]||keys[SDL_SCANCODE_RIGHT])){
		if (keys[SDL_SCANCODE_RIGHT]){
			player->ent->flip.x = 0;
			player->ent->velocity.x=2;
			}
		else{
        if (keys[SDL_SCANCODE_LEFT]){
			player->ent->flip.x = 1;
			player->ent->velocity.x=-2;
			}
		}
		}
	}
	
	if(landing){
			player->ent->frame = player->landing_frame;
			if(recov>0){
				recov-=.1;}
			else{
				grounded = 1;
				landing = 0;
				double_jump = 0;
				player->ent->frame = 0;}
			}
			//===================JUMPING END========================
			
			
			//===================ATTACKING START=====================
			
			if(attacking&&grounded){
			if(player->ent->frame<player->ground_attack_start_frame)player->ent->frame=player->ground_attack_start_frame;
			if(player->ent->frame<player->ground_attack_end_frame)player->ent->frame+=.15;
			if(player->ent->frame>=player->ground_attack_end_frame){
				player->ent->frame = 0;
				attacking = 0;
				player->attack(player);
				}
			}
			
			if(attacking&&!grounded){
			if(player->ent->frame<player->air_attack_start_frame)player->ent->frame=player->air_attack_start_frame;
			if(player->ent->frame<player->air_attack_end_frame)player->ent->frame+=.2;
			if(player->ent->frame>=player->air_attack_end_frame){
				player->ent->frame = 0;
				attacking = 0;
				player->air_attack(player);
				}
			}
						
			//===================ATTACKING END========================
			
			
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(background,vector2d(0,0),vector2d(10,10));
            
            gf2d_entity_draw_all();
            gf2d_entity_update_all();
            
                
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
         // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}
		if (keys[SDL_SCANCODE_P] && (get_menu_state() == MS_None || get_menu_state() == MS_Pause)){
			current_time = SDL_GetTicks();
			if(get_menu_state() == MS_None && current_time > pause_timer){ 
				set_menu_state(MS_Pause);
				pause_timer = SDL_GetTicks() + 200;}
			if(get_menu_state() == MS_Pause && current_time > pause_timer){
				set_menu_state(MS_None);
				 pause_timer = SDL_GetTicks() + 200;}
		}
	 if(get_menu_state() == MS_Pause){gf2d_sprite_draw_image(pause_screen,vector2d(0,0),vector2d(1,1));
		  SDL_GetMouseState(&mx,&my);
       menu_update_all();
       menu_draw_all();
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
		 gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                (int)mf);
	 gf2d_grahics_next_frame();
	 }
	 if(get_menu_state() == MS_SelectScreen){gf2d_sprite_draw_image(select_screen,vector2d(0,0),vector2d(1,1));
		 SDL_GetMouseState(&mx,&my);
       menu_update_all();
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
		 gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                (int)mf);
	 gf2d_grahics_next_frame();}
	 if(get_menu_state() == MS_TitleScreen){
		gf2d_sprite_draw_image(title_screen,vector2d(0,0),vector2d(1,1));
		SDL_GetMouseState(&mx,&my);
        menu_update_all();
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
		 gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                (int)mf);
                gf2d_grahics_next_frame();}
	 if (keys[SDL_SCANCODE_ESCAPE])done = 1;
}
	
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
