#include "handyplus.h"

#define snake_up 0
#define snake_down 1
#define snake_left 2
#define snake_right 3

#define snake_body_no_body 0
#define snake_body_body 2

typedef struct world_rect{
    HGP_RECT* this_rect;
    struct world_rect * left;
    struct world_rect * right;
    struct world_rect * up;
    struct world_rect * down;
    struct world_rect * snake_node;
    int food;
}world_rect;

world_rect *world_array[50][50]={NULL};
world_rect *snake_head_ptr;

int snake_direct;
int snake_head_x;
int snake_head_y;

int food_x;
int food_y;

void food_generate(void);
world_rect *snake_next_node(world_rect* current_node);
void *snake_move();
void snake_update();
int score;
int foodcount;
const int foodtotal=10;//change this to modify num of food_generate