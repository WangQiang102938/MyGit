#include "handyplus.h"

#define boarder_x 400
#define boarder_y 400

#define BULLET_DIRECT_UP 0
#define BULLET_DIRECT_DOWN 1
#define BULLET_DIRECT_LEFT 2
#define BULLET_DIRECT_RIGHT 3

typedef struct timeval timeval;
typedef struct FIGHTER_1
{
    double center_x;
    double center_y;
    HGP_POLYGON *shell;
    HGP_RECT *left_gun;
    HGP_RECT *right_gun;
} FIGHTER_1;

typedef struct BULLET_NODE
{
    struct BULLET_NODE * next;
    HGP_CIRCLE * obj_ptr;
} BULLET_NODE;
typedef struct BULLET
{
    struct BULLET_NODE * head;
    int direct;
    unsigned long color;
}BULLET;

int fighter_init(FIGHTER_1 *fighter1, HGP_LAYER_INFO *layer, double size_rate);
int check_timer(struct timeval *checked_time, int timer_ms);
int bullet_move(BULLET *bullet_list, double move_distance);
int create_bullet(FIGHTER_1 *fighter, int bullet_direct, BULLET *bullet_list, HGP_LAYER_INFO *layer);
int fighter_move(FIGHTER_1 *fighter, double x, double y, int absolute_flag);
int fighter_mirrow(FIGHTER_1 *fighter);
