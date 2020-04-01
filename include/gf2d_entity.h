#ifndef __GF3D_ENTITY_H__
#define __GF3D_ENTITY_H__

#include "gf2d_sprite.h"
#include "simple_json.h"
#include "gf2d_shape.h"

typedef enum
{
    ES_Idle = 0,
    ES_Damaged = 1,
    ES_Dead = 2
}EntityState;

typedef struct Entity_S
{
    Uint8           _inuse; /**<flag to keep track if this isntance is in use and should not be reassigned*/
    Uint8           _set;
    Sprite          *sprite;          /**<the sprite for this entity*/
    Vector2D        position;       /**<position of the entity*/
    Vector2D        velocity;       /**<velocity of the entity*/
    Vector2D        flip;   		/**<whether the entity is flipped on any axis*/
    Vector3D        rotation;       /**<rotation of the entity*/
    Vector2D        scale;          /**<*please default to 1,1*/
    void (*touch) (struct Entity_S* self,struct Entity_S* other);
    void (*think) (struct Entity_S* self);
    int           	health;
    int           healthmax;
    float           owner;
    int           experience;
    float           duration;
    int             tag;			/**1 - player, 2 - experience, 3 - pickup agu 4 - pickup gabu 5 - pickup guil 6 - platform 7 - projectile 8 - enemy 9 - hp */
    char*			name;
    float			frame;
    Box*			box;
    int				gravity;
    int				colliding;
    int				timer;
    int				frozen;
    int				special;
    int				invincibility;
    void *data;                     /**<additional entity specific data*/
    
}Entity;

/**
 * @brief initializes the entity subsystem
 * @param entity_max maximum number of simultaneous entities you wish to support
 */
void gf2d_entity_manager_init(Uint32 entity_max);

/**
 * @brief get an empty entity from the system
 * @return NULL on out of space or a pointer to an entity otherwise
 */
Entity *gf2d_entity_new();

/**
 * @brief free an active entity
 * @param self the entity to free
 */
void    gf2d_entity_free(Entity *self);

/**
 * @brief draw a single entity
 * @param self the entity to draw
 */
void gf2d_entity_draw(Entity *self);

/**
 * @brief draw all active entities in the system
 */
void gf2d_entity_draw_all();

/**
 * @brief allows the max for the entity manager to be accessed
 * @return the max number of entities, 0 if entity manager is not initialized
 */
int     gf2d_entity_max();

/**
 * @brief access an initialized entity
 * @param index the index of the entity to access
 * @return pointer to an entity if valid, null otherwise
 */
Entity *gf2d_entity_get(int index);

/**
 * @brief give entity proper data based on params
 * @param the information for the entity
 */
void gf2d_entity_load(Entity* ent, char* filename, int width, int height, int frames_per_line, Vector2D pos,Vector2D scale);

/**
 * @brief spawn a non-specific pickup entity
 * @param the information for the entity
 */
void gf2d_pickup_spawn(char* filename, int width, int height, int frames_per_line, Vector2D pos,Vector2D scale,Vector2D velocity,Vector2D flip,Vector2D boxoffset, int boxw, int boxh, int tag, int duration);

/**
 * @brief spawn a non-specific projectile entity
 * @param the information for the entity
 */
void gf2d_projectile_spawn(char* filename, int width, int height, int frames_per_line, Vector2D pos,Vector2D scale,Vector2D velocity,Vector2D flip,Vector2D boxoffset, int boxw, int boxh, float owner, int duration);

/**
 * @brief spawn a specific weapon projectile entity
 * @param the information for the entity
 */
void gf2d_special_projectile_spawn(char* filename, int width, int height, int frames_per_line, Vector2D pos,Vector2D scale,Vector2D velocity,Vector2D flip,Vector2D boxoffset, int boxw, int boxh, float owner, int duration, int special);

/**
 * @brief takes information from json file and intitializes the entity with that information
 * @param filename name of the file to be loaded
 */
Entity *gf2d_entity_load_from_file(char *filename);

/**
 * @brief iterate through all entities and update them
 */
void gf2d_entity_update_all();

/**
 * @brief call the update function of the given entity
 * @param the entity to update
 */
void gf2d_entity_update(Entity *self);

void gf2d_platform_spawn(Vector2D position, Vector2D scale);

void gf2d_enemy_spawn(Vector2D position, int enemy_type, int patrol_bound_left, int patrol_bound_right, int flip);

void gf2d_basic_collision();

void projectile_touch(Entity* self, Entity* other);

void gf2d_apply_offset();

void gf2d_door_spawn(int level, Vector2D position);

void gf2d_level_clear();
#endif
