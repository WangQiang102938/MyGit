#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <handy.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread/pthread.h>
#include "handy.h"
//------------------------------------------------------
//Window:
HGP_WINDOW_PROP *hgplus_entry;

typedef struct HGP_WINDOW_PROP
{
    double x;
    double y;
    double width;
    double height;
    int window_id;
    int double_cache_mode;
    HGP_WINDOW_LIST *my_node;
    HGP_LAYER_PROP *start_layer_prop;
    HGP_WINDOW_LIST *entry_node;
} HGP_WINDOW_PROP;

typedef struct HGP_WINDOW_LIST
{
    struct HGP_WINDOW_LIST *Next;
    struct HGP_WINDOW_LIST *This;
    struct HGP_WINDOW_LIST *Prev;
} HGP_WINDOW_LIST;
//func
HGP_WINDOW_PROP *hgp_window_init(double x, double y, double width, double height);
HGP_WINDOW_PROP *hgp_window_addWindow(HGP_WINDOW_PROP *target_window, double x, double y, double width, double height);
int hgp_window_deleteWindow(HGP_WINDOW_PROP *target_window);
int hgp_window_sync(HGP_WINDOW_PROP *target_window);
//------------------------------------------------------
//LAYER:
typedef struct HGP_LAYER_PROP
{
    int layer_id[2];
    struct HGP_LAYER_PROP *my_node;
    int visible_flag;
    //todo:object prop;
    struct HGP_WINDOW_PROP *window_prop;
} HGP_LAYER_PROP;

typedef struct HGP_LAYER_LIST
{
    struct HGP_LAYER_LIST *Next;
    struct HGP_LAYER_LIST *This;
    struct HGP_LAYER_LIST *Prev;
} HGP_LAYER_LIST;
//func
HGP_LAYER_PROP *hgp_layer_addLayer(HGP_WINDOW_PROP *target_window);
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
} HGP_COMMON_MODEL_PROP;

typedef struct HGP_COMMON_MODEL_LIST
{
    HGP_COMMON_MODEL_PROP *Next;
    HGP_COMMON_MODEL_PROP *This;
    HGP_COMMON_MODEL_PROP *Prev;
} HGP_COMMON_MODEL_LIST;
//func
HGP_COMMON_MODEL_PROP *hgp_model_addModel(HGP_LAYER_PROP *target_layer, int type);
int hgp_model_deleteModel(HGP_COMMON_MODEL_PROP *target_model);
int hgp_model_move_to(HGP_COMMON_MODEL_PROP *target_model, double target_x, double target_y);
int hgp_model_move_byAngle(HGP_COMMON_MODEL_PROP *target_model, double direct_arc, double distance);
int hgp_model_zoom(HGP_COMMON_MODEL_PROP *target_model, double zoom_rate);
int hgp_model_rotate(HGP_COMMON_MODEL_PROP *target_model, double rotate_arc);
int hgp_model_sync(HGP_COMMON_MODEL_PROP *target_model);
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
    double rotate_arc;
    int fill_flag;
    int stroke_flag;
    double line_width;
} HGP_RECT;

typedef struct HGP_CIRCLE
{
    double r;
    unsigned long shell_color;
    unsigned long fill_color;
    double line_width;
    int fill_flag;
    int stroke_flag;
} HGP_CIRCLE;

typedef struct HGP_ARC
{
    HGP_COMMON_MODEL_PROP *model_prop;
    double r;
    unsigned long shell_color;
    double arc_start;
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
    double arc_start;
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
HGP_RECT *hgp_model_rectInit(HGP_LAYER_PROP *target_layer, double x, double y, double width, double height, unsigned long shell_color, unsigned long fill_color, double rotate_arc, double line_width, int fill_flag, int stroke_flag);
HGP_CIRCLE *hgp_model_circleInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, unsigned long fill_color, double line_width, int fill_flag, int stroke_flag);
HGP_ARC *hgp_model_arcInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, double arc_start, double arc_value, double line_width);
HGP_FAN *hgp_model_fanInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, unsigned long fill_color, double are_start, double arc_value, double line_width, int fill_flag, int stroke_flag);
HGP_LINE *hgp_model_lineInit(HGP_LAYER_PROP *target_layer, double x, double y, double end_x, double end_y, unsigned long line_color, double line_width); //end_x,end_y is relative coordinate of x,y
HGP_POLYGON *hgp_model_polygonInit(HGP_LAYER_PROP *target_layer, double x, double y, unsigned long line_color, unsigned long fill_color, double line_width, int fill_flag, int stroke_flag);
