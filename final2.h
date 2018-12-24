#include "handyplus.h"

#define boarder_x 400
#define boarder_y 400

#define BULLET_DIRECT_UP 0
#define BULLET_DIRECT_DOWN 1
#define BULLET_DIRECT_LEFT 2
#define BULLET_DIRECT_RIGHT 3

typedef struct timeval timeval;
typedef struct FIGHTER //fighter info
{
    double center_x;
    double center_y;
    double size_rate;
    unsigned long color;
    int fighter_type;
    HGP_POLYGON *shell;
    HGP_RECT *left_gun;
    HGP_RECT *right_gun;
    //other info
    struct fighter_path_list *path_head_node;
    struct fighter_path_list *path_current_node;
    BULLET * bullet_info;
    FILE * pathfile;
    HGP_LAYER_INFO * layer_info;
} FIGHTER;

typedef struct BULLET_NODE //bullet list node
{
    struct BULLET_NODE *next;
    HGP_CIRCLE *obj_ptr;
} BULLET_NODE;

typedef struct BULLET //bullet list head and info
{
    struct BULLET_NODE *head;
    int direct;
    unsigned long color;
} BULLET;

typedef struct fighter_path_list //path list
{
    struct fighter_path_list *next;
    double x;
    double y;
} fighter_path_list;

typedef struct read_path_list //path file list
{
    struct read_path_list *next;
    FILE *path;
} read_path_list;

int fighter_init(FIGHTER *fighter1, HGP_LAYER_INFO *layer, double size_rate);
int check_timer(struct timeval *checked_time, int timer_ms);
int bullet_move(BULLET *bullet_list, double move_distance);
int create_bullet(FIGHTER *fighter, int bullet_direct, BULLET *bullet_list, HGP_LAYER_INFO *layer);
int fighter_move(FIGHTER *fighter, double x, double y, int absolute_flag);
int fighter_mirrow(FIGHTER *fighter);
int check_collapse(FIGHTER *fighter, BULLET *bullet_list);
int fighter_destroy(FIGHTER *fighter);
int fighter_move_by_pathfile(FIGHTER *fighter, FILE *pathfile);

typedef struct log_prop
{
    char *pathname;
    FILE *log_file_path_ptr;
    fighter_path_list *head;
    fighter_path_list *current_node;
    int node_counter;
} log_prop;

log_prop *path_log_init(char *pathname);
int path_log(log_prop *loginfo, double x, double y);
int path_log_write(log_prop **loginfo_ptr);

int logflag;
int logpath_counter;
char logpathname[128];
log_prop *loginfo;
