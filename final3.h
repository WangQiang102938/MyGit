#include "handyplus.h"

#define boarder_x 400
#define boarder_y 400

#define BULLET_DIRECT_UP 0
#define BULLET_DIRECT_DOWN 1
#define BULLET_DIRECT_LEFT 2
#define BULLET_DIRECT_RIGHT 3

#define FIGHTER_IDENTITY_ME 1
#define FIGHTER_IDENTITY_ENEMY 2

#define FIGHTER_SIZE_RATE_MAX 5

typedef struct timeval timeval;

typedef struct LIST
{
    void *ptr;
    struct LIST *next_node;
    struct LIST *prev_node;
    int type;
} LIST;

#define LIST_TYPE_FIGHTER 0
#define LIST_TYPE_BULLET 1
#define LIST_TYPE_PATHFILE 2
#define LIST_TYPE_PATH 3
#define LIST_TYPE_LOG 4

#define LIST_INSERT_HEAD 0
#define LIST_INSERT_FRONT 1
#define LIST_INSERT_BACK 2
#define LIST_INSERT_TAIL 3

LIST *List_Add(int type, int location, size_t mem_size, LIST **head_ptr, LIST *target_node);
void * List_Delete(LIST **target_node_ptr);

typedef struct FIGHTER
{
    double center_x;
    double center_y;
    double size_rate;
    unsigned long color;
    int fighter_identity;
    HGP_POLYGON *shell;
    HGP_RECT *left_gun;
    HGP_RECT *right_gun;
    LIST *list_info;
    HGP_LAYER_INFO *layer_info;
    //
    struct PATHFILE *path_file;
    struct LIST *bullet;
    struct PATH *path_current;
    struct PATH *path_head;
} FIGHTER;

typedef struct BULLET
{
    HGP_CIRCLE *bullet_obj_ptr;
    FIGHTER *fighter;
    LIST *list_info;
} BULLET;

typedef struct PATHFILE
{
    FILE *path_file;
    LIST *path_list_head;
    LIST *list_info;
} PATHFILE;

typedef struct PATH
{
    double x;
    double y;
    LIST *list_info;
} PATH;

size_t Get_Size_By_ID(int ID);

typedef struct PATH_LOG_INFO
{
    char *pathname;
    FILE *pathfile_ptr;
    LIST *path_head;
    LIST *path_current;
    int node_counter;
} PATH_LOG_INFO;

FIGHTER *fighter_init(HGP_LAYER_INFO *layer, double size_rate, int fighter_identity, LIST *bullet_info, unsigned long color);
int fighter_destroy(FIGHTER *fighter);
FIGHTER *fighter_add(LIST *fighter_list_head, FIGHTER *fighter);
int fighter_mirrow(FIGHTER *fighter);
int fighter_move(FIGHTER *fighter, double x, double y, int absolute_flag);
int create_bullet(FIGHTER *fighter);
unsigned long Get_BulletColor_By_FighterID(FIGHTER *fighter);
int bullet_move(LIST *bullet_list, double move_distance);
int check_timer(struct timeval *checked_time, int timer_ms);
int check_collapse(FIGHTER *fighter, LIST *bullet_list);
int fighter_move_by_pathfile(LIST *fighter_list);
PATH_LOG_INFO *path_log_init(char *pathname);
int path_log(PATH_LOG_INFO *loginfo, double x, double y);
int path_log_write(PATH_LOG_INFO *loginfo);
int path_load();
int path_clean();

int path_log(PATH_LOG_INFO *loginfo, double x, double y);
int path_log_write(PATH_LOG_INFO *loginfo_ptr);
//public list
LIST *fighter_list;
LIST *bullet_list;
LIST *pathfile_list;
LIST *log_list;
//public
int load_path_counter;
FIGHTER *control_fighter;
