#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_entity.h"

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
    Entity *agumon;
    Vector4D mouseColor = {255,100,255,200};
    Vector2D fire_flip = {0,0};
    Vector2D fire_pos;
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*setup*/
    gf2d_entity_manager_init(100);
    sprite = gf2d_sprite_load_image("images/backgrounds/back.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    agumon = gf2d_entity_new();
    gf2d_entity_load(agumon,"images/aguman.png",48,48,11,vector2d(550,480),vector2d(3,3));
    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        //===================GROUNDED START========================
        if (keys[SDL_SCANCODE_Z]&& !attacking){
			grounded = 0;
		}
		if (keys[SDL_SCANCODE_X]&& !attacking){
			attacking = 1;
			if(agumon->flip.x == 0){
			fire_flip.x = 0;}
			else{
				fire_flip.x = 1;}
		}
		if ((keys[SDL_SCANCODE_LEFT]||keys[SDL_SCANCODE_RIGHT]) && grounded && !attacking){
		if (keys[SDL_SCANCODE_RIGHT]){
			agumon->flip.x = 0;
			agumon->frame+=0.1;
			if(agumon->frame>=17)agumon->frame=1;
			agumon->position.x+=2;
			}
		else{
        if (keys[SDL_SCANCODE_LEFT]){
			agumon->flip.x = 1;
			agumon->frame+=0.1;
			if(agumon->frame>=17)agumon->frame=1;
			agumon->position.x-=2;
			}
		}
		}
		else{
		if(grounded && !attacking)agumon->frame = 0;}
		//===================GROUNDED END========================
		
		
		//===================JUMPING START========================
		if(!grounded && !landing){
		air+=.1;
		if(agumon->frame<17)agumon->frame=17.1;
		if(agumon->frame<=23)agumon->frame+=.1;
			if(air<6){
			agumon->position.y-=4;
			}
			else{
			if(air<14.1){
			agumon->position.y+=3;
		}
		else{
			air = 0;
			landing = 1;
			recov = 1;
		}
	}
		slog("%f",agumon->position.y);
		if ((keys[SDL_SCANCODE_LEFT]||keys[SDL_SCANCODE_RIGHT]) && !attacking){
		if (keys[SDL_SCANCODE_RIGHT]){
			agumon->flip.x = 0;
			agumon->position.x+=2;
			}
		else{
        if (keys[SDL_SCANCODE_LEFT]){
			agumon->flip.x = 1;
			agumon->position.x-=2;
			}
		}
		}
	}
	
	if(landing){
			agumon->frame = 24;
			if(recov>0){
				recov-=.1;}
			else{
				grounded = 1;
				landing = 0;
				agumon->frame = 0;}
			}
			//===================JUMPING END========================
			
			
			//===================ATTACKING START=====================
			
			if(attacking&&grounded){
			if(agumon->frame<25)agumon->frame=25;
			if(agumon->frame<35)agumon->frame+=.15;
			if(agumon->frame>=35){
				agumon->frame = 0;
				attacking = 0;
				fire_pos.x = agumon->position.x;
				fire_pos.y = agumon->position.y;
				if(fire_flip.x == 0)gf2d_entity_spawn("images/fireball.png",24,24,3,vector2d(fire_pos.x+57.5,fire_pos.y+62.5),vector2d(2.5,2.5),vector2d(2.5,0),fire_flip);
				if(fire_flip.x == 1)gf2d_entity_spawn("images/fireball.png",24,24,3,vector2d(fire_pos.x+20,fire_pos.y+62.5),vector2d(2.5,2.5),vector2d(-2.5,0),fire_flip);
				}
			}
			
			if(attacking&&!grounded){
			if(agumon->frame<37)agumon->frame=37;
			if(agumon->frame<46)agumon->frame+=.2;
			if(agumon->frame>=46){
				agumon->frame = 0;
				attacking = 0;
				fire_pos.x = agumon->position.x;
				fire_pos.y = agumon->position.y;
				if(fire_flip.x == 0)gf2d_entity_spawn("images/fireball.png",24,24,3,vector2d(fire_pos.x+57.5,fire_pos.y+62.5),vector2d(2.5,2.5),vector2d(2.5,0),fire_flip);
				if(fire_flip.x == 1)gf2d_entity_spawn("images/fireball.png",24,24,3,vector2d(fire_pos.x+20,fire_pos.y+62.5),vector2d(2.5,2.5),vector2d(-2.5,0),fire_flip);
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
                &mouseColor,
                (int)mf);
                
                
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        //slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
