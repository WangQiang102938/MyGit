#include "handyplus.h"

#define boarder_x 400
#define boarder_y 400

typedef struct FIGHTER_1
{
    double center_x;
    double center_y;
    HGP_POLYGON *shell;
    HGP_RECT *left_gun;
    HGP_RECT *right_gun;
} FIGHTER_1;

typedef struct BULLET
{
    double x;
    double y;
    struct BULLET * next;
    struct BULLET * prev;
    HGP_CIRCLE * obj_ptr;
} BULLET;

int fighter_init(FIGHTER_1 *fighter1, HGP_LAYER_INFO *layer, double size_rate);