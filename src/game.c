#include <SDL.h>
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
    Uint8 paused = 0;
    
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
    menu_manager_init(10);
    Entity *platform_test = malloc(sizeof(Entity)); 
    platform_test = gf2d_entity_new();
    Entity *platform_test2 = malloc(sizeof(Entity)); 
    platform_test2 = gf2d_entity_new();
    Entity *enemy_test = malloc(sizeof(Entity)); 
    enemy_test = gf2d_entity_new();
    background = gf2d_sprite_load_image("images/backgrounds/sky_back.png");
    select_screen = gf2d_sprite_load_image("images/backgrounds/selectmenu.png");
    title_screen = gf2d_sprite_load_image("images/backgrounds/titlescreen.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    setup_player_ent(player);
    load_agumon(player);
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
    while(!done|| get_menu_state() == MS_Exit)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        //===================GROUNDED START========================
        if(!paused && get_menu_state() == MS_None){
		current_time = SDL_GetTicks();
		if(player->digivolved == 1){
			if (current_time > player->digi_timer){
				player->dedigivolve(player);}}
        if(keys[SDL_SCANCODE_1] && grounded && !attacking){
        if(current_time > change_timer){load_agumon(player);
        change_timer = SDL_GetTicks() + 500;}
		}
		if(keys[SDL_SCANCODE_2] && grounded && !attacking){
        if(current_time > change_timer){load_guilmon(player);
        change_timer = SDL_GetTicks() + 500;}
		}
		if(keys[SDL_SCANCODE_3] && grounded && !attacking){
        if(current_time > change_timer){load_gabumon(player);
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
        if (keys[SDL_SCANCODE_Z]&& !attacking && grounded){
			grounded = 0;
			jump_timer = current_time + 200;
		}
		if (keys[SDL_SCANCODE_D]&& !attacking && grounded && player->ent->experience >= 100){
			player->digivolve(player);
			player->ent->experience -= 100;
		}
		if(keys[SDL_SCANCODE_Z] && double_jump != 1 && current_time > jump_timer){
			slog("stuff");
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
		if (keys[SDL_SCANCODE_P]){
			current_time = SDL_GetTicks();
			if(paused == 0 && current_time > pause_timer){ 
				paused = 1;
				pause_timer = SDL_GetTicks() + 500;}
			if(paused == 1 && current_time > pause_timer){
				 paused = 0;
				 pause_timer = SDL_GetTicks() + 500;}
		}
	 if(get_menu_state() == MS_SelectScreen){gf2d_sprite_draw_image(select_screen,vector2d(0,0),vector2d(2.5,2.6));
		 SDL_GetMouseState(&mx,&my);
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
