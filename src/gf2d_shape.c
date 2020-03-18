#include "gf2d_shape.h"
#include "simple_logger.h"
#include "gf2d_sprite.h"

Box *gf2d_box(Vector2D pos, float w, float h, Vector2D offset, Vector2D scale)
{
    Box *r = malloc(sizeof(Box));
    r->sprite = gf2d_sprite_load_image("images/box.png");
    r->pos.x = pos.x + offset.x;
    r->pos.y = pos.y + offset.y;
    r->offset.x = offset.x;
    r->offset.y = offset.y;
    r->width = w;
    r->height = h;
    r->scale = scale;
    return r;
}
void gf2d_box_update(Box *r,Vector2D pos){
    r->pos.x = pos.x + r->offset.x;
    r->pos.y = pos.y + r->offset.y;
}

Uint8 gf2d_box_overlap(Box *a,Box *b)
{
	slog("%f",a->pos.x);
    if ((a->pos.x +a->width >= b->pos.x-b->width)&&(a->pos.x -a->width <= b->pos.x+b->width)&&
        (a->pos.y +a->height >= b->pos.y-b->height)&&(a->pos.y -a->height <= b->pos.y+b->height))
    {
        return 1;
    }
    return 0;
}

/*eol@eof*/
