#include "handyplus.h"

HGP_WINDOW_INFO *hgp_window_init(double x, double y, double window_location_x, double window_location_y)
{
    hgp_layer_flag_1.layer_reverse_flag = 0;
    hgp_layer_flag_1.nextnode = &hgp_layer_flag_2;
    hgp_layer_flag_2.layer_reverse_flag = 1;
    hgp_layer_flag_2.nextnode = &hgp_layer_flag_3;
    hgp_layer_flag_3.layer_reverse_flag = 2;
    hgp_layer_flag_3.nextnode = &hgp_layer_flag_1;
    HGP_LAYER_FLAG_CURRENT_PTR = &hgp_layer_flag_1;

    return hgp_create_window(x, y, window_location_x, window_location_y);
}

HGP_WINDOW_INFO *hgp_create_window(double x, double y, double window_location_x, double window_location_y)
{
    HGP_WINDOW_INFO *new_window_ptr = malloc(sizeof(HGP_WINDOW_INFO));
    if (HGP_WINDOW_ENTER_NODE != NULL)
    {
        HGP_WINDOW_INFO *window_tmp_ptr = HGP_WINDOW_ENTER_NODE;
        for (int i = 0; i < HG_MAX_WINDOWS; i++)
        {
            if (window_tmp_ptr->next_window_node == NULL)
            {
                break;
            }
            else
            {
                window_tmp_ptr = window_tmp_ptr->next_window_node;
            }
        }
        new_window_ptr->previos_window_node = window_tmp_ptr;
        window_tmp_ptr->next_window_node = new_window_ptr;
        new_window_ptr->next_window_node = NULL;
    }
    else
    {
        new_window_ptr->previos_window_node = NULL;
        new_window_ptr->next_window_node = NULL;
        HGP_WINDOW_ENTER_NODE = new_window_ptr;
    }
    if (window_location_x == -1 || window_location_y == -1)
    {
        new_window_ptr->wid = HgOpen(x, y);
    }
    else
    {
        new_window_ptr->wid = HgWOpen(x, y, window_location_x, window_location_y);
    }
    new_window_ptr->window_x = x;
    new_window_ptr->window_y = y;
    new_window_ptr->start_layer_node=NULL;
    return new_window_ptr;
}

HGP_LAYER_INFO *hgp_add_layer(HGP_WINDOW_INFO *window)
{
    HGP_LAYER_INFO *new_layer_ptr = malloc(sizeof(HGP_LAYER_INFO));
    if (window->start_layer_node == NULL)
    {
        window->start_layer_node = new_layer_ptr;
        new_layer_ptr->previous_layer_node = NULL;
        new_layer_ptr->next_layer_node = NULL;
        new_layer_ptr->father_window_info = window;
        new_layer_ptr->obj_start_node = NULL;
    }
    else
    {
        HGP_LAYER_INFO *tmp_layer_ptr = window->start_layer_node;
        for (int i = 0; i < HG_MAX_LAYERS; i++)
        {
            if (tmp_layer_ptr->next_layer_node == NULL)
            {
                break;
            }
        }
        tmp_layer_ptr->next_layer_node = new_layer_ptr;
        new_layer_ptr->previous_layer_node = tmp_layer_ptr;
        new_layer_ptr->next_layer_node = NULL;
        new_layer_ptr->father_window_info = window;
        new_layer_ptr->obj_start_node = NULL;
    }
    for (int i = 0; i < 3; i++)
    {
        new_layer_ptr->lid[i] = HgWAddLayer(window->wid);
    }
    new_layer_ptr->BackGroundColor = HG_WHITE;
    return new_layer_ptr;
}

HGP_OBJECT *hgp_add_object(HGP_LAYER_INFO *layer, int type) //TODO:add obj
{
    HGP_OBJECT *new_object_ptr = malloc(sizeof(HGP_OBJECT));
    if (layer->obj_start_node == NULL)
    {
        layer->obj_start_node = new_object_ptr;
        new_object_ptr->previous_object_node = NULL;
        new_object_ptr->next_object_node = NULL;
    }
    else
    {
        HGP_OBJECT *tmp_object_ptr = layer->obj_start_node;
        while (tmp_object_ptr->next_object_node != NULL)
        {
            tmp_object_ptr = tmp_object_ptr->next_object_node;
        }
        tmp_object_ptr->next_object_node = new_object_ptr;
        new_object_ptr->previous_object_node = tmp_object_ptr;
        new_object_ptr->next_object_node = NULL;
    }
    new_object_ptr->father_layer_info = layer;
    new_object_ptr->type = type;
    switch (type)
    {
    case HGP_OBJECT_RECT_FLAG:
        new_object_ptr->pointer = malloc(sizeof(HGP_RECT));
        break;
    case HGP_OBJECT_CIRCLE_FLAG:
        new_object_ptr->pointer = malloc(sizeof(HGP_CIRCLE));
        break;
    case HGP_OBJECT_ARC_FLAG:
        new_object_ptr->pointer = malloc(sizeof(HGP_ARC));
        break;
    case HGP_OBJECT_FAN_FLAG:
        new_object_ptr->pointer = malloc(sizeof(HGP_FAN));
        break;
    case HGP_OBJECT_LINE_FLAG:
        new_object_ptr->pointer = malloc(sizeof(HGP_LINE));
        break;
    case HGP_OBJECT_POLYGON_FLAG:
        new_object_ptr->pointer = malloc(sizeof(HGP_POLYGON));
        break;
    }
    return new_object_ptr;
}
int hgp_delete_object(HGP_OBJECT *obj_ptr) //TODO:obj
{
    if (obj_ptr->previous_object_node == NULL)
    {
        obj_ptr->father_layer_info->obj_start_node = obj_ptr->next_object_node;
    }
    else
    {
        obj_ptr->previous_object_node->next_object_node = obj_ptr->next_object_node;
    }
    free(obj_ptr->pointer);
    free(obj_ptr);
    obj_ptr = NULL;
    return 1;
}

int hgp_delete_layer(HGP_LAYER_INFO *layer_ptr)
{
    if (layer_ptr->previous_layer_node == NULL)
    {
        layer_ptr->father_window_info->start_layer_node = layer_ptr->next_layer_node;
    }
    else
    {
        layer_ptr->previous_layer_node->next_layer_node = layer_ptr->next_layer_node;
    }
    HGP_OBJECT *tmp_obj_ptr = layer_ptr->obj_start_node;
    if (tmp_obj_ptr != NULL)
    {
        while (tmp_obj_ptr->next_object_node != NULL)
        {
            tmp_obj_ptr = tmp_obj_ptr->next_object_node;
            hgp_delete_object(tmp_obj_ptr->previous_object_node);
        }
        hgp_delete_object(tmp_obj_ptr);
    }
    for (int i = 0; i < 3; i++)
    {
        HgLRemove(layer_ptr->lid[i]);
    }
    free(layer_ptr);
    return 1;
}

int hgp_destroy_window(HGP_WINDOW_INFO *window_ptr)
{
    if (window_ptr->previos_window_node == NULL)
    {
        HGP_WINDOW_ENTER_NODE = window_ptr->next_window_node;
    }
    else
    {
        window_ptr->previos_window_node->next_window_node = window_ptr->next_window_node;
    }
    HGP_LAYER_INFO *tmp_layer_ptr = window_ptr->start_layer_node;
    if (tmp_layer_ptr != NULL)
    {
        while (tmp_layer_ptr->next_layer_node != NULL)
        {
            tmp_layer_ptr = tmp_layer_ptr->next_layer_node;
            hgp_delete_layer(tmp_layer_ptr->previous_layer_node);
        }
        hgp_delete_layer(tmp_layer_ptr);
    }
    HgSleep(0.1);  
    HgWClose(window_ptr->wid);
    free(window_ptr);
    return 1;
}

void breakpoint()
{
    printf("breakpoint:%d\n", breakpointcount);
    breakpointcount++;
}

int hgp_update(int flag) //TODO:add obj
{
    HGP_WINDOW_INFO *window_ptr = HGP_WINDOW_ENTER_NODE;
    HGP_LAYER_INFO *layer_ptr = NULL;
    HGP_OBJECT *object_ptr = NULL;
    while (1)//window
    {
        layer_ptr = window_ptr->start_layer_node;
        while (1)//layer
        {
            object_ptr = layer_ptr->obj_start_node;
            while (1)//object
            {
                hgp_single_draw(object_ptr);
                if (object_ptr->next_object_node == NULL)
                {
                    break;
                }
                else
                {
                    object_ptr = object_ptr->next_object_node;
                }
            }
            HgLShow(layer_ptr->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],1);
            HgLShow(layer_ptr->lid[HGP_LAYER_FLAG_CURRENT_PTR->nextnode->nextnode->layer_reverse_flag],0);
            HgLClear(layer_ptr->lid[HGP_LAYER_FLAG_CURRENT_PTR->nextnode->layer_reverse_flag]);
            if (layer_ptr->next_layer_node == NULL)
            {
                break;
            }
            else
            {
                layer_ptr = layer_ptr->next_layer_node;
            }
        }
        if (window_ptr->next_window_node == NULL)
        {
            break;
        }
        else
        {
            window_ptr = window_ptr->next_window_node;
        }
    }
    HGP_LAYER_FLAG_CURRENT_PTR=HGP_LAYER_FLAG_CURRENT_PTR->nextnode;
    return 1;
}

int hgp_single_draw(HGP_OBJECT *object_ptr)
{
    switch (object_ptr->type)
    {
    case HGP_OBJECT_RECT_FLAG:
    {
        HGP_RECT *obj_pointer = object_ptr->pointer;
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->fill_color, HG_ColorFill);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->shell_color, HG_ColorDraw);
        HGCRectangle(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
                     obj_pointer->x,
                     obj_pointer->y,
                     obj_pointer->width / 2,
                     obj_pointer->height / 2,
                     obj_pointer->rotate_arc,
                     obj_pointer->fill_flag,
                     obj_pointer->stroke_lenth);
        break;
    }
    case HGP_OBJECT_CIRCLE_FLAG:
    {
        HGP_CIRCLE *obj_pointer = ((HGP_CIRCLE *)object_ptr->pointer);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->fill_color, HG_ColorFill);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->shell_color, HG_ColorDraw);
        HGCCircle(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
                  obj_pointer->x,
                  obj_pointer->y,
                  obj_pointer->r,
                  obj_pointer->fill_flag,
                  obj_pointer->stroke_lenth);
        break;
    }
    case HGP_OBJECT_ARC_FLAG:
    {
        HGP_ARC *obj_pointer = ((HGP_ARC *)object_ptr->pointer);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->shell_color, HG_ColorDraw);
        HGCFan(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
               obj_pointer->x,
               obj_pointer->y,
               obj_pointer->r,
               obj_pointer->arc_start,
               obj_pointer->arc_start + obj_pointer->arc_value,
               0, 0);
        break;
    }
    case HGP_OBJECT_FAN_FLAG:
    {
        HGP_FAN *obj_pointer = ((HGP_FAN *)object_ptr->pointer);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->shell_color, HG_ColorDraw);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->fill_color, HG_ColorFill);
        HGCFan(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
               obj_pointer->x,
               obj_pointer->y,
               obj_pointer->r,
               obj_pointer->arc_start,
               obj_pointer->arc_start + obj_pointer->arc_value,
               obj_pointer->fill_flag,
               obj_pointer->stroke_lenth);
        break;
    }
    case HGP_OBJECT_LINE_FLAG: //todo
    {
        HGP_LINE *obj_pointer = ((HGP_LINE *)object_ptr->pointer);
        HgWSetWidth(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->line_width);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->color, HG_ColorDraw);
        HgWLine(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->start_x,
                obj_pointer->start_y, obj_pointer->end_x, obj_pointer->end_y);
        break;
    }
    case HGP_OBJECT_POLYGON_FLAG: //todo
    {
        HGP_POLYGON *obj_pointer = ((HGP_POLYGON *)object_ptr->pointer);
        HgWSetWidth(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->line_width);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->color, HG_ColorDraw);
        HGCSetColor(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->fill_color, HG_ColorFill);
        HGCPolygon(object_ptr->father_layer_info->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->counter,
                   obj_pointer->x, obj_pointer->y, obj_pointer->fill_flag, obj_pointer->stroke_lenth);
        break;
    }
    default:
        break;
    }
    return 1;
}

int hgp_direct_draw(HGP_OBJECT *object_ptr)
{
    HGP_LAYER_FLAG_CURRENT_PTR = HGP_LAYER_FLAG_CURRENT_PTR->nextnode->nextnode;
    hgp_single_draw(object_ptr);
    HGP_LAYER_FLAG_CURRENT_PTR = HGP_LAYER_FLAG_CURRENT_PTR->nextnode;
    return 0;
}

int hgp_quit() //disabled hgp_killHG
{
    HGP_WINDOW_INFO *window_ptr = HGP_WINDOW_ENTER_NODE;
    while (window_ptr->next_window_node == NULL)
    {
        window_ptr = window_ptr->next_window_node;
        hgp_destroy_window(window_ptr->previos_window_node);
    }
    hgp_destroy_window(window_ptr);
    return 0;
}

int hgp_killHG()
{
    FILE *hg_process_info = popen("ps -e | grep 'HgDisplayer' | awk '{print $1}'", "r");
    char pidbuf[10] = {0};
    fgets(pidbuf, 10, hg_process_info);
    int hgpid = 0;
    sscanf(pidbuf, "%d", &hgpid);
    if (hgpid != 0)
    {
        char hgkillcombuf[32] = {0};
        sprintf(hgkillcombuf, "kill -9 %d", hgpid);
        popen(hgkillcombuf, "w");
    }
    return hgpid;
}

int hgp_object_move(HGP_OBJECT *object_ptr, double direct_arc, double distance) //TODO:add obj
{
    int type = object_ptr->type;
    void *ptr = object_ptr->pointer;
    double move_x = cos(direct_arc) * distance;
    double move_y = sin(direct_arc) * distance;
    switch (type)
    {
    case HGP_OBJECT_RECT_FLAG:
    {
        HGP_RECT *rect = (HGP_RECT *)ptr;
        rect->x = rect->x + move_x;
        rect->y = rect->y + move_y;
        break;
    }
    case HGP_OBJECT_CIRCLE_FLAG:
    {
        HGP_CIRCLE *circle = (HGP_CIRCLE *)ptr;
        circle->x = circle->x + move_x;
        circle->y = circle->y + move_y;
        break;
    }
    case HGP_OBJECT_ARC_FLAG:
    {
        HGP_ARC *arc = (HGP_ARC *)ptr;
        arc->x = arc->x + move_x;
        arc->y = arc->y + move_y;
        break;
    }
    case HGP_OBJECT_FAN_FLAG:
    {
        HGP_FAN *fan = (HGP_FAN *)ptr;
        fan->x = fan->x + move_x;
        fan->y = fan->y + move_y;
        break;
    }
    case HGP_OBJECT_LINE_FLAG:
    {
        HGP_LINE *obj = (HGP_LINE *)ptr;
        obj->start_x += move_x;
        obj->start_y += move_y;
        obj->end_x += move_x;
        obj->end_x += move_y;
    }
    }
    //hgp_update();
    return 0;
}

int hgp_object_zoom(HGP_OBJECT *object_ptr, double zoom_rate, double zoom_center_x, double zoom_center_y) //TODO:add obj
{
    int type = object_ptr->type;
    void *ptr = object_ptr->pointer;
    switch (type)
    {
    case HGP_OBJECT_RECT_FLAG:
    {
        HGP_RECT *rect = (HGP_RECT *)ptr;
        double center_x_cache = rect->x - (rect->width / 2) + (rect->width * zoom_center_x / 100);
        double center_y_cache = rect->y - (rect->height / 2) + (rect->height * zoom_center_y / 100);
        //turn
        rect->x = (rect->x - center_x_cache) * zoom_rate + center_x_cache;
        rect->y = (rect->y - center_y_cache) * zoom_rate + center_y_cache;
        rect->width = rect->width * zoom_rate;
        rect->height = rect->height * zoom_rate;
    }
    case HGP_OBJECT_CIRCLE_FLAG:
    {
        HGP_CIRCLE *circle = (HGP_CIRCLE *)ptr;
        double center_x_cache = circle->x - (circle->r / 2) + (circle->r * zoom_center_x / 100);
        double center_y_cache = circle->y - (circle->r / 2) + (circle->r * zoom_center_y / 100);
        //turn
        circle->x = (circle->x - center_x_cache) * zoom_rate + center_x_cache;
        circle->y = (circle->y - center_y_cache) * zoom_rate + center_y_cache;
        circle->r = circle->r * zoom_rate;
    }
    case HGP_OBJECT_ARC_FLAG:
    {
        HGP_ARC *arc = (HGP_ARC *)ptr;
        double center_x_cache = arc->x - (arc->r / 2) + (arc->r * zoom_center_x / 100);
        double center_y_cache = arc->y - (arc->r / 2) + (arc->r * zoom_center_y / 100);
        //turn
        arc->x = (arc->x - center_x_cache) * zoom_rate + center_x_cache;
        arc->y = (arc->y - center_y_cache) * zoom_rate + center_y_cache;
        arc->r = arc->r * zoom_rate;
    }
    case HGP_OBJECT_FAN_FLAG:
    {
        HGP_FAN *fan = (HGP_FAN *)ptr;
        double center_x_cache = fan->x - (fan->r / 2) + (fan->r * zoom_center_x / 100);
        double center_y_cache = fan->y - (fan->r / 2) + (fan->r * zoom_center_y / 100);
        //turn
        fan->x = (fan->x - center_x_cache) * zoom_rate + center_x_cache;
        fan->y = (fan->y - center_y_cache) * zoom_rate + center_y_cache;
        fan->r = fan->r * zoom_rate;
    }
    case HGP_OBJECT_LINE_FLAG: //line only accept zoom_rate,TODO: add zoom_center function
    {
        HGP_LINE *obj_pointer = (HGP_LINE *)ptr;
        double center_x_cache = (obj_pointer->start_x + obj_pointer->end_x) / 2;
        double center_y_cache = (obj_pointer->start_y + obj_pointer->end_y) / 2;
        //turn
        obj_pointer->start_x = (obj_pointer->start_x - center_x_cache) * zoom_rate + center_x_cache;
        obj_pointer->start_y = (obj_pointer->start_y - center_y_cache) * zoom_rate + center_y_cache;
        obj_pointer->end_x = (obj_pointer->end_x - center_x_cache) * zoom_rate + center_x_cache;
        obj_pointer->end_y = (obj_pointer->end_y - center_y_cache) * zoom_rate + center_y_cache;
    }
    case HGP_OBJECT_POLYGON_FLAG: //polygon only accept coordinate not persentage
    {
        HGP_POLYGON *obj_pointer = (HGP_POLYGON *)ptr;
        double center_x_cache = zoom_center_x;
        double center_y_cache = zoom_center_y;
        //turn
        for (int i = 0; i < obj_pointer->counter; i++)
        {
            obj_pointer->x[i] = (obj_pointer->x[i] - center_x_cache) * zoom_rate + center_x_cache;
            obj_pointer->y[i] = (obj_pointer->y[i] - center_y_cache) * zoom_rate + center_y_cache;
        }
    }
    }
    return 0;
}

int hgp_object_rotate(HGP_OBJECT *object_ptr, double rotate_arc, int need_angle_input_1) //TODO:add obj:line polygon
{
    if (need_angle_input_1)
    {
        rotate_arc = rotate_arc * M_PI / 180;
        //printf("\nangle 2 arc:%lf\n",rotate_arc);
    }
    int type = object_ptr->type;
    void *ptr = object_ptr->pointer;
    switch (type)
    {
    case HGP_OBJECT_RECT_FLAG:
    {
        HGP_RECT *rect = (HGP_RECT *)ptr;
        rect->rotate_arc = rect->rotate_arc + rotate_arc;
        if (rect->rotate_arc > 2 * M_PI)
            rect->rotate_arc = rect->rotate_arc - 2 * M_PI;
        break;
    }
    case HGP_OBJECT_ARC_FLAG:
    {
        HGP_ARC *arc = (HGP_ARC *)ptr;
        arc->arc_start = arc->arc_start + rotate_arc;
        if (arc->arc_start > 2 * M_PI)
            arc->arc_start = arc->arc_start - 2 * M_PI;
        break;
    }
    case HGP_OBJECT_FAN_FLAG:
    {
        HGP_FAN *fan = (HGP_FAN *)ptr;
        fan->arc_start = fan->arc_start + rotate_arc;
        if (fan->arc_start > 2 * M_PI)
            fan->arc_start = fan->arc_start - 2 * M_PI;
        break;
    }
    case HGP_OBJECT_LINE_FLAG: //TODO
    {
        HGP_LINE *obj_pointer = (HGP_LINE *)ptr;
        double rotate_center_x = (obj_pointer->start_x + obj_pointer->end_x) / 2;
        double rotate_center_y = (obj_pointer->start_y + obj_pointer->end_y) / 2;
        double lenth = sqrt(pow(obj_pointer->start_x - obj_pointer->end_x, 2) +
                            pow(obj_pointer->start_y + obj_pointer->end_y, 2));
        double old_cos = (obj_pointer->start_x - obj_pointer->end_x) / lenth;
        double old_sin = (obj_pointer->start_y - obj_pointer->end_y) / lenth;
        double new_cos = old_cos * cos(rotate_arc) + old_sin * sin(rotate_arc);
        double new_sin = old_sin * cos(rotate_arc) - old_cos * sin(rotate_arc);
        obj_pointer->start_x = new_cos * lenth / 2;
    }
    }
    return 0;
}

//-----Obj init-----//
HGP_RECT *hgp_rect_init(HGP_LAYER_INFO *layer_ptr, double x, double y,
                        double width, double height,
                        unsigned long shell_color, unsigned long fill_color,
                        int fill_flag, double rotate_arc, int stroke_lenth)
{
    HGP_OBJECT *obj_ptr = (hgp_add_object(layer_ptr, HGP_OBJECT_RECT_FLAG));
    HGP_RECT *Operate_Rect_Pointer = obj_ptr->pointer;
    Operate_Rect_Pointer->obj_ptr = obj_ptr;
    Operate_Rect_Pointer->x = x;
    Operate_Rect_Pointer->y = y;
    Operate_Rect_Pointer->width = width;
    Operate_Rect_Pointer->height = height;
    Operate_Rect_Pointer->shell_color = shell_color;
    Operate_Rect_Pointer->fill_color = fill_color;
    Operate_Rect_Pointer->fill_flag = fill_flag;
    Operate_Rect_Pointer->rotate_arc = rotate_arc;
    Operate_Rect_Pointer->stroke_lenth = stroke_lenth;
    return Operate_Rect_Pointer;
}

HGP_CIRCLE *hgp_circle_init(HGP_LAYER_INFO *layer_ptr,
                            double x, double y, double r,
                            unsigned long shell_color,
                            unsigned long fill_color,
                            int fill_flag, int stroke_lenth)
{
    HGP_OBJECT *obj_ptr = (hgp_add_object(layer_ptr, HGP_OBJECT_CIRCLE_FLAG));
    HGP_CIRCLE *Operate_Circle_Pointer = obj_ptr->pointer;
    Operate_Circle_Pointer->obj_ptr = obj_ptr;
    Operate_Circle_Pointer->x = x;
    Operate_Circle_Pointer->y = y;
    Operate_Circle_Pointer->r = r;
    Operate_Circle_Pointer->shell_color = shell_color;
    Operate_Circle_Pointer->fill_color = fill_color;
    Operate_Circle_Pointer->fill_flag = fill_flag;
    Operate_Circle_Pointer->stroke_lenth = stroke_lenth;
    return Operate_Circle_Pointer;
}

HGP_ARC *hgp_arc_init(HGP_LAYER_INFO *layer_ptr,
                      double x, double y, double r,
                      unsigned long shell_color,
                      double arc_start, double arc_value)
{
    HGP_OBJECT *obj_ptr = (hgp_add_object(layer_ptr, HGP_OBJECT_ARC_FLAG));
    HGP_ARC *Operate_Arc_Pointer = obj_ptr->pointer;
    Operate_Arc_Pointer->obj_ptr = obj_ptr;
    Operate_Arc_Pointer->x = x;
    Operate_Arc_Pointer->y = y;
    Operate_Arc_Pointer->r = r;
    Operate_Arc_Pointer->shell_color = shell_color;
    Operate_Arc_Pointer->arc_start = arc_start;
    Operate_Arc_Pointer->arc_value = arc_value;
    return Operate_Arc_Pointer;
}

HGP_FAN *hgp_fan_init(HGP_LAYER_INFO *layer_ptr,
                      double x, double y, double r,
                      unsigned long shell_color,
                      unsigned long fill_color,
                      int fill_flag, int stroke_lenth,
                      double arc_start, double arc_value)
{
    HGP_OBJECT *obj_ptr = (hgp_add_object(layer_ptr, HGP_OBJECT_FAN_FLAG));
    HGP_FAN *Operate_Fan_Pointer = obj_ptr->pointer;
    Operate_Fan_Pointer->obj_ptr = obj_ptr;
    Operate_Fan_Pointer->x = x;
    Operate_Fan_Pointer->y = y;
    Operate_Fan_Pointer->r = r;
    Operate_Fan_Pointer->shell_color = shell_color;
    Operate_Fan_Pointer->fill_color = fill_color;
    Operate_Fan_Pointer->fill_flag = fill_flag;
    Operate_Fan_Pointer->stroke_lenth = stroke_lenth;
    Operate_Fan_Pointer->arc_start = arc_start;
    Operate_Fan_Pointer->arc_value = arc_value;
    return Operate_Fan_Pointer;
}

HGP_LINE *hgp_line_init(HGP_LAYER_INFO *layer_ptr, double start_x, double start_y, double end_x, double end_y,
                        unsigned long color, double line_width)
{
    HGP_OBJECT *obj_ptr = (hgp_add_object(layer_ptr, HGP_OBJECT_LINE_FLAG));
    HGP_LINE *Operate_Obj_Pointer = obj_ptr->pointer;
    Operate_Obj_Pointer->obj_ptr = obj_ptr;
    Operate_Obj_Pointer->start_x = start_x;
    Operate_Obj_Pointer->start_y = start_y;
    Operate_Obj_Pointer->end_x = end_x;
    Operate_Obj_Pointer->end_y = end_y;
    Operate_Obj_Pointer->color = color;
    Operate_Obj_Pointer->line_width = line_width;
    return Operate_Obj_Pointer;
}

//thread test
void thread()
{
    hgp_update(0);
}