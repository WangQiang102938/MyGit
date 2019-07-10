#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <handy.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread/pthread.h>
#include "handy.h"
typedef struct HGP_STATUS
{
    int init_flag;
    int Window_Count;
    int Layer_Count;
    int Model_Count;
    int pointer_checker;
    int double_cache_flag;
    struct HGP_WINDOW_LIST *window_list_entry;
} HGP_STATUS;
HGP_STATUS hgp_status;
//------------------------------------------------------
//Window:
struct HGP_WINDOW_PROP *hgplus_entry;

typedef struct HGP_WINDOW_PROP
{
    double x;
    double y;
    double width;
    double height;
    int window_id;
    HGP_WINDOW_LIST *my_node;
    HGP_LAYER_LIST *layer_head_node;
    char *window_title;
} HGP_WINDOW_PROP;

typedef struct HGP_WINDOW_LIST
{
    struct HGP_WINDOW_LIST *Next;
    struct HGP_WINDOW_PROP *This;
    struct HGP_WINDOW_LIST *Prev;
} HGP_WINDOW_LIST;
//func
HGP_WINDOW_PROP *hgp_window_init();
HGP_WINDOW_PROP *hgp_window_addWindow(double x, double y, double width, double height);
int hgp_window_deleteWindow(HGP_WINDOW_PROP *target_window);
int hgp_window_sync(HGP_WINDOW_PROP *target_window);
//todo:add windowinfo sync function;
//todo:add window adjust function:move zoom sizechange...
//------------------------------------------------------
//LAYER:
typedef struct HGP_LAYER_PROP
{
    int layer_id[2];
    int double_cache_mode; //-1=disable layer;0=single layer;1=double layer;
    struct HGP_LAYER_PROP *my_node;
    int visible_flag;
    struct HGP_COMMON_MODEL_LIST *model_head_node;
    struct HGP_COMMON_MODEL_LIST *model_tail_node;
    //todo:object prop;
    struct HGP_WINDOW_PROP *window_prop;
} HGP_LAYER_PROP;

typedef struct HGP_LAYER_LIST
{
    struct HGP_LAYER_LIST *Next;
    struct HGP_LAYER_PROP *This;
    struct HGP_LAYER_LIST *Prev;
} HGP_LAYER_LIST;
//func
HGP_LAYER_PROP *hgp_layer_addLayer(HGP_WINDOW_PROP *target_window, int doubleCacheMode);
int hgp_layer_deleteLayer(HGP_LAYER_PROP *target_layer);
int hgp_layer_sync(HGP_LAYER_PROP *target_layer);
//------------------------------------------------------
//COMMON MODEL

#define HGP_MODELTYPE_RECT 1
#define HGP_MODELTYPE_CIRCLE 2
#define HGP_MODELTYPE_ARC 3
#define HGP_MODELTYPE_FAN 4
#define HGP_MODELTYPE_LINE 5
#define HGP_MODELTYPE_POLYGON 6
#define HGP_MODELTYPE_OVAL 7

typedef struct HGP_COMMON_MODEL_PROP
{
    int model_type;
    void *model_ptr;
    struct HGP_COMMON_MODEL_PROP *my_node;
    struct HGP_LAYER_PROP *layer_prop;
    int visible_flag; //0=invisible,1=visible;
    int change_flag;  //-1=disable,0=normal,1=changed(fast draw)
    double x;
    double y;
    double rotate_arc;
    double zoom_rate;
} HGP_COMMON_MODEL_PROP;

typedef struct HGP_COMMON_MODEL_LIST
{
    struct HGP_COMMON_MODEL_LIST *Next;
    struct HGP_COMMON_MODEL_PROP *This;
    struct HGP_COMMON_MODEL_LIST *Prev;
} HGP_COMMON_MODEL_LIST;
//func
HGP_COMMON_MODEL_PROP *hgp_model_addModel(HGP_LAYER_PROP *target_layer, int type, double x, double y, int isInsertTail);
int hgp_model_deleteModel(HGP_COMMON_MODEL_PROP *target_model);
//TODO:ADD MODEL QUEUE ORDER CHANGE FUNC
int hgp_model_move_to(HGP_COMMON_MODEL_PROP *target_model, double target_x, double target_y);
int hgp_model_move_byAngle(HGP_COMMON_MODEL_PROP *target_model, double direct_arc, double distance);
int hgp_model_sync(HGP_COMMON_MODEL_PROP *target_model);
int hgp_model_syncAll();
//------------------------------------------------------
//MODEL:

#define HGP_MODEL_Direct_Right 0
#define HGP_MODEL_Direct_Left M_PI
#define HGP_MODEL_Direct_Up (M_PI * 0.5)
#define HGP_MODEL_Direct_Down (M_PI * 1.5)

typedef struct HGP_RECT
{
    HGP_COMMON_MODEL_PROP *model_prop;
    double width;
    double height;
    unsigned long shell_color;
    unsigned long fill_color;
    int fill_flag;
    int stroke_flag;
    double line_width;
} HGP_RECT;

typedef struct HGP_CIRCLE
{
    HGP_COMMON_MODEL_PROP *model_prop;
    double r;
    unsigned long shell_color;
    unsigned long fill_color;
    double line_width;
    int fill_flag;
    int stroke_flag;
} HGP_CIRCLE;

typedef struct HGP_OVAL//todo:
{
    HGP_COMMON_MODEL_PROP *model_prop;
    double r1;
    double r2;
    unsigned long shell_color;
    unsigned long fill_color;
    double line_width;
    int fill_flag;
    int stroke_flag;
} HGP_OVAL;

typedef struct HGP_ARC
{
    HGP_COMMON_MODEL_PROP *model_prop;
    double r;
    unsigned long shell_color;
    double arc_value;
    double line_width;
    double stroke;
} HGP_ARC;

typedef struct HGP_FAN
{
    HGP_COMMON_MODEL_PROP *model_prop;
    double r;
    unsigned long shell_color;
    unsigned long fill_color;
    int fill_flag;
    int stroke_flag;
    double line_width;
    double arc_value;
} HGP_FAN;

typedef struct HGP_LINE
{
    HGP_COMMON_MODEL_PROP *model_prop;
    double end_x; //relative x;
    double end_y; //relative y;
    unsigned long color;
    double line_width;
} HGP_LINE;

typedef struct HGP_POLYGON
{
    HGP_COMMON_MODEL_PROP *model_prop;
    HGP_POLYGON_NODE *start_node;
    HGP_POLYGON_NODE *tail_node;
    unsigned long line_color;
    unsigned long fill_color;
    int fill_flag;
    double line_width;
    int stroke_flag;
} HGP_POLYGON;

typedef struct HGP_POLYGON_NODE
{
    struct HGP_POLYGON_NODE *next;
    struct HGP_POLYGON_NODE *prev;
    struct HGP_POLYGON *polygon_prop;
    double x;
    double y;
} HGP_POLYGON_NODE;
#define HGP_MODEL_POLYGON_INSERT_head 0
#define HGP_MODEL_POLYGON_INSERT_front 1
#define HGP_MODEL_POLYGON_INSERT_back 2
#define HGP_MODEL_POLYGON_INSERT_tail 3

HGP_POLYGON_NODE *hgp_model_polygon_addPoint(HGP_POLYGON *target_polygon, int insert_location, HGP_POLYGON_NODE *target_node, double x, double y);
int hgp_model_polygon_delPoint(int mode, HGP_POLYGON_NODE *target_node, int target_count);
//model init;
HGP_RECT *hgp_model_rectInit(HGP_LAYER_PROP *target_layer, double x, double y, double width, double height, unsigned long shell_color, unsigned long fill_color, double rotate_arc, double line_width, int fill_flag, int stroke_flag, int isInsertTail);
HGP_CIRCLE *hgp_model_circleInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, unsigned long fill_color, double line_width, int fill_flag, int stroke_flag, int isInsertTail);
HGP_ARC *hgp_model_arcInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, double arc_start, double arc_value, double line_width, int isInsertTail);
HGP_FAN *hgp_model_fanInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, unsigned long fill_color, double arc_start, double arc_value, double line_width, int fill_flag, int stroke_flag, int isInsertTail);
HGP_LINE *hgp_model_lineInit(HGP_LAYER_PROP *target_layer, double x, double y, double end_x, double end_y, unsigned long line_color, double line_width, int isInsertTail); //end_x,end_y is relative coordinate of x,y
HGP_POLYGON *hgp_model_polygonInit(HGP_LAYER_PROP *target_layer, double x, double y, unsigned long line_color, unsigned long fill_color, double line_width, int fill_flag, int stroke_flag, int isInsertTail);
