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
    double size_rate;
    unsigned long color;
    int fighter_type;
    struct enemy_fighter_path_list *path_head_node;
    struct enemy_fighter_path_list *path_current_node;
    HGP_POLYGON *shell;
    HGP_RECT *left_gun;
    HGP_RECT *right_gun;
} FIGHTER_1;

typedef struct BULLET_NODE
{
    struct BULLET_NODE *next;
    HGP_CIRCLE *obj_ptr;
} BULLET_NODE;
typedef struct BULLET
{
    struct BULLET_NODE *head;
    int direct;
    unsigned long color;
} BULLET;

typedef struct enemy_fighter_path_list
{
    struct enemy_fighter_path_list *next;
    double x;
    double y;
} enemy_fighter_path_list;

typedef struct read_path_list
{
    struct read_path_list *next;
    FILE *path;
} read_path_list;

int fighter_init(FIGHTER_1 *fighter1, HGP_LAYER_INFO *layer, double size_rate);
int check_timer(struct timeval *checked_time, int timer_ms);
int bullet_move(BULLET *bullet_list, double move_distance);
int create_bullet(FIGHTER_1 *fighter, int bullet_direct, BULLET *bullet_list, HGP_LAYER_INFO *layer);
int fighter_move(FIGHTER_1 *fighter, double x, double y, int absolute_flag);
int fighter_mirrow(FIGHTER_1 *fighter);
int check_collapse(FIGHTER_1 *fighter, BULLET *bullet_list);
int fighter_destroy(FIGHTER_1 *fighter);
int fighter_move_by_pathfile(FIGHTER_1 *fighter, FILE *pathfile);
