#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_entity.h"
#include "player.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Uint8 grounded = 1;
    Uint8 landing = 0;
    Uint8 attacking = 0;
    Sprite *sprite;
    
    int mx,my;
    float recov = 0;
    float air = 0;
    float mf = 0;
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
    sprite = gf2d_sprite_load_image("images/backgrounds/back.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    setup_player_ent(player);
    load_agumon(player);
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        //===================GROUNDED START========================
        if(keys[SDL_SCANCODE_1] && grounded && !attacking){
        load_agumon(player);
		}
		if(keys[SDL_SCANCODE_2] && grounded && !attacking){
        load_guilmon(player);
		}
		if(keys[SDL_SCANCODE_3] && grounded && !attacking){
        load_gabumon(player);
		}
		if(keys[SDL_SCANCODE_4] && grounded && !attacking){
        load_wargreymon(player);
		}
		if(keys[SDL_SCANCODE_5] && grounded && !attacking){
        load_gallantmon(player);
		}
		if(keys[SDL_SCANCODE_6] && grounded && !attacking){
        load_etemon(player);
		}
        if (keys[SDL_SCANCODE_Z]&& !attacking){
			grounded = 0;
		}
		if (keys[SDL_SCANCODE_X]&& !attacking){
			attacking = 1;
		}
		if ((keys[SDL_SCANCODE_LEFT]||keys[SDL_SCANCODE_RIGHT]) && grounded && !attacking){
		if (keys[SDL_SCANCODE_RIGHT]){
			player->ent->flip.x = 0;
			player->ent->frame+=0.1;
			if(player->ent->frame>=player->move_end_frame)player->ent->frame=1;
			player->ent->position.x+=3;
			}
		else{
        if (keys[SDL_SCANCODE_LEFT]){
			player->ent->flip.x = 1;
			player->ent->frame+=0.1;
			if(player->ent->frame>=player->move_end_frame)player->ent->frame=1;
			player->ent->position.x-=3;
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
			player->ent->position.y-=4;
			}
			else{
			if(air<14.1){
			player->ent->position.y+=3;
		}
		else{
			air = 0;
			landing = 1;
			recov = 1;
		}
	}
	
		if ((keys[SDL_SCANCODE_LEFT]||keys[SDL_SCANCODE_RIGHT])){
		if (keys[SDL_SCANCODE_RIGHT]){
			player->ent->flip.x = 0;
			player->ent->position.x+=2;
			}
		else{
        if (keys[SDL_SCANCODE_LEFT]){
			player->ent->flip.x = 1;
			player->ent->position.x-=2;
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
			
			
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            
            gf2d_entity_draw_all();
            gf2d_entity_update_all();
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                NULL,
                (int)mf);
                
                
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
