#include "handyplus.h"

HGP_LAYER_INFO *layer;
//List Definision
typedef struct LIST
{
    struct LIST *next_node;
    struct LIST *prev_node;
    void *p;
    int type;
    int circle_flag; //-1=disable,0=start_node,1=normal_node;
} LIST;

#define LIST_INSERT_HEAD 0
#define LIST_INSERT_TAIL 1
#define LIST_INSERT_FRONT 2
#define LIST_INSERT_BACK 3

LIST *List_Init(int type, void *pointer);
int List_Insert(LIST *insert_node, LIST *target_node, int insert_location);
int List_Delete(LIST *target_list);
LIST *List_Search(LIST *head, void *pointer);
//

//Fighter Definision
typedef struct FIGHTER
{
    HGP_POLYGON *body;
    HGP_RECT *left_gun;
    HGP_RECT *right_gun;
    double x;
    double y;
    double size_rate;
    int fighter_identity;
    LIST *node;
    unsigned long color;
    int path_flag;
    PATHFILE *pathfile;
    PATH *current_path;
} FIGHTER;

FIGHTER *fighter_head;

FIGHTER *Fighter_Init(double x, double y, double size_rate,
                      int fighter_identity, unsigned long color);
int Fighter_Create(FIGHTER *fighter);
int Fighter_Destroy(FIGHTER *fighter);
int Fighter_Move(FIGHTER *fighter, double x, double y, int coordinate_flag);
int Fighter_Shoot(FIGHTER *fighter);
FIGHTER *Fighter_Next(FIGHTER *Fighter);
int Fighter_LoadPath(struct PATHFILE *pathfile);
//

//Bullet Definision
typedef struct BULLET
{
    double x;
    double y;
    LIST *node;
    HGP_CIRCLE *shell;
    int identity;
} BULLET;

BULLET *bullet_head;

BULLET *Bullet_Init(double x, double y, int identity);
int Bullet_Create(BULLET *bullet);
int Bullet_Delete(BULLET *bullet);
int Bullet_CheckCollapse(FIGHTER *fighter, BULLET *bullet);
BULLET *Bullet_Next(BULLET *bullet);
//

//PathFile path
typedef struct PATHFILE
{
    FILE *file;
    LIST *node;
    PATH *path;
} PATHFILE;

PATHFILE * pathfile_head;

PATHFILE *PathFile_Init(FILE *file);
int PathFile_Delete(PATHFILE *pathfile);
PATHFILE *PathFile_Next(PATHFILE * pathfile);

typedef struct PATH
{
    double x;
    double y;
    LIST *node;
} PATH;

PATH * Path_Init(double x,double y);
int Path_Add(PATH * entry,PATH * path);
int Path_Delete(PATH **entry,PATH * path);
PATH* Path_Next(PATH * path);
