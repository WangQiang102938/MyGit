#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <handy.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <pthread/pthread.h>

#define INIT_DEFAULT 0
#define INIT_COSTOMIZE 1
#define INIT_FAULT 0x0000FFFF

typedef struct HGP_LAYER_FLAG_LINKLIST
{
    int layer_reverse_flag;
    struct HGP_LAYER_FLAG_LINKLIST *nextnode;
} HGP_LAYER_FLAG_LINKLIST;

HGP_LAYER_FLAG_LINKLIST hgp_layer_flag_1;
HGP_LAYER_FLAG_LINKLIST hgp_layer_flag_2;
HGP_LAYER_FLAG_LINKLIST hgp_layer_flag_3;

HGP_LAYER_FLAG_LINKLIST *HGP_LAYER_FLAG_CURRENT_PTR;

//-----Object Struct-----//
typedef struct HGP_WINDOW_INFO
{
    int wid;
    double window_x;
    double window_y;
    struct HGP_WINDOW_INFO *next_window_node;
    struct HGP_WINDOW_INFO *previos_window_node;
    struct HGP_LAYER_INFO *start_layer_node;
} HGP_WINDOW_INFO;

typedef struct HGP_LAYER_INFO
{
    int lid[3];
    unsigned long BackGroundColor;
    struct HGP_OBJECT *obj_start_node;
    struct HGP_LAYER_INFO *next_layer_node;
    struct HGP_LAYER_INFO *previous_layer_node;
    struct HGP_WINDOW_INFO *father_window_info;
} HGP_LAYER_INFO;

typedef struct HGP_OBJECT
{
    int type;
    void *pointer;
    struct HGP_OBJECT *next_object_node;
    struct HGP_OBJECT *previous_object_node;
    struct HGP_LAYER_INFO *father_layer_info;
    int change_flag;//-1:disable;0:normal;1:changed
} HGP_OBJECT;


typedef struct HGP_RECT
{
    HGP_OBJECT *obj_ptr;
    double x;
    double y;
    double width;
    double height;
    unsigned long shell_color;
    unsigned long fill_color;
    int fill_flag;
    double rotate_arc;
    int stroke_lenth;
} HGP_RECT;

typedef struct HGP_CIRCLE
{
    HGP_OBJECT *obj_ptr;
    double x;
    double y;
    double r;
    unsigned long shell_color;
    unsigned long fill_color;
    int fill_flag;
    int stroke_lenth;
} HGP_CIRCLE;

typedef struct HGP_ARC
{
    HGP_OBJECT *obj_ptr;
    double x;
    double y;
    double r;
    unsigned long shell_color;
    double arc_start;
    double arc_value;
} HGP_ARC;

typedef struct HGP_FAN
{
    HGP_OBJECT *obj_ptr;
    double x;
    double y;
    double r;
    unsigned long shell_color;
    unsigned long fill_color;
    int fill_flag;
    int stroke_lenth;
    double arc_start;
    double arc_value;
} HGP_FAN;

typedef struct HGP_LINE
{
    HGP_OBJECT *obj_ptr;
    double start_x;
    double start_y;
    double end_x;
    double end_y;
    unsigned long color;
    double line_width;
} HGP_LINE;

typedef struct HGP_POLYGON_NODE
{
    double x;
    double y;
    struct HGP_POLYGON_NODE *next_node;
    struct HGP_POLYGON_NODE *prev_node;
    HGP_POLYGON* polygon_ptr;
}HGP_POLYGON_NODE;

typedef struct HGP_POLYGON
{
    HGP_OBJECT *obj_ptr;
    int counter;
    HGP_POLYGON_NODE *head;
    double line_width;
    unsigned long color;
    int fill_flag;
    double stroke_lenth;
    unsigned int fill_color;
} HGP_POLYGON;
HGP_POLYGON_NODE * hgp_polygon_add_point(int insert_flag,HGP_POLYGON_NODE * target_node,HGP_POLYGON *polygon_ptr,double x,double y);
HGP_POLYGON_NODE * hgp_polygon_del_point(HGP_POLYGON_NODE * target_node);
#define HGP_POLYGON_INSERT_HEAD 0
#define HGP_POLYGON_INSERT_TAIL 1
#define HGP_POLYGON_INSERT_FRONT_TAR 2
#define HGP_POLYGON_INSERT_AFTER_TAR 3
//-----Globle-----//
void breakpoint();
int breakpointcount;
#define HGP_DIRECT_RIGHT 0
#define HGP_DIRECT_LEFT M_PI
#define HGP_DIRECT_UP (M_PI * 0.5)
#define HGP_DIRECT_DOWN (M_PI * 1.5)

#define HGP_OBJECT_RECT_FLAG 0
#define HGP_OBJECT_CIRCLE_FLAG 1
#define HGP_OBJECT_ARC_FLAG 2
#define HGP_OBJECT_FAN_FLAG 3
#define HGP_OBJECT_LINE_FLAG 4
#define HGP_OBJECT_POLYGON_FLAG 5

#define HGP_OBJECT_TOTAL_NUM 1048576
//-----Window-----//
#define HGP_LAYER_NODE_INFO_START 1
#define HGP_LAYER_NODE_INFO_END -1
#define HGP_LAYER_NODE_INFO_NORMAL 0

#define HGP_OBJECT_NODE_INFO_START 1
#define HGP_OBJECT_NODE_INFO_END -1
#define HGP_OBJECT_NODE_INFO_NORMAL 0

HGP_WINDOW_INFO *HGP_WINDOW_ENTER_NODE;
HGP_WINDOW_INFO *hgp_window_init(double x, double y, double window_location_x, double window_location_y);
HGP_WINDOW_INFO *hgp_create_window(double x, double y, double window_location_x, double window_location_y);
HGP_LAYER_INFO *hgp_add_layer(HGP_WINDOW_INFO *window);
HGP_OBJECT *hgp_add_object(HGP_LAYER_INFO *layer, int type);
HGP_OBJECT * hgp_delete_object(HGP_OBJECT *obj_ptr);
HGP_LAYER_INFO * hgp_delete_layer(HGP_LAYER_INFO *layer_ptr);
HGP_WINDOW_INFO * hgp_destroy_window(HGP_WINDOW_INFO *window_ptr);
void breakpoint();
int hgp_update(int flag);
int hgp_single_draw(HGP_OBJECT *object_ptr);
int hgp_direct_draw(HGP_OBJECT *object_ptr);
int hgp_quit();
int hgp_killHG();
int hgp_object_move(HGP_OBJECT *object_ptr, double direct_arc, double distance);
int hgp_object_zoom(HGP_OBJECT *object_ptr, double zoom_rate, double zoom_center_x, double zoom_center_y);
int hgp_object_rotate(HGP_OBJECT *object_ptr, double rotate_arc, int need_angle_input_1);
//-----Object Init-----//
HGP_RECT *hgp_rect_init(HGP_LAYER_INFO *layer_ptr, double x, double y,
                        double width, double height,
                        unsigned long shell_color, unsigned long fill_color,
                        int fill_flag, double rotate_arc, int stroke_lenth);

HGP_CIRCLE *hgp_circle_init(HGP_LAYER_INFO *layer_ptr,
                            double x, double y, double r,
                            unsigned long shell_color,
                            unsigned long fill_color,
                            int fill_flag, int stroke_lenth);

HGP_ARC *hgp_arc_init(HGP_LAYER_INFO *layer_ptr,
                      double x, double y, double r,
                      unsigned long shell_color,
                      double arc_start, double arc_value);

HGP_FAN *hgp_fan_init(HGP_LAYER_INFO *layer_ptr,
                      double x, double y, double r,
                      unsigned long shell_color,
                      unsigned long fill_color,
                      int fill_flag, int stroke_lenth,
                      double arc_start, double arc_value);

HGP_LINE *hgp_line_init(HGP_LAYER_INFO *layer_ptr, double start_x, double start_y, double end_x, double end_y,
                        unsigned long color, double line_width);
//-----Object Operation-----//
int hgp_object_move(HGP_OBJECT *obj_ptr, double direct_angle, double distance);
int hgp_object_rotate(HGP_OBJECT *obj_ptr, double rotate_arc, int need_angle_input_1);
int hgp_object_zoom(HGP_OBJECT *obj_ptr, double zoom_rate, double zoom_center_x, double zoom_center_y);

//thread test
pthread_t thread1_ptr;
void thread1();