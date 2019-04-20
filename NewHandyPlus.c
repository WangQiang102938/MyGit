#include "NewHandyPlus.h"

//---------------------------------------
HGP_WINDOW_PROP *hgp_window_init() //if x or y equal -1,use default window location
{
    hgp_status.init_flag = 1;
    hgp_status.Layer_Count = 0;
    hgp_status.Model_Count = 0;
    hgp_status.Window_Count = 0;
    hgp_status.pointer_checker = 0;
    hgp_status.window_list_entry = NULL;
    hgp_status.double_cache_flag = 0;
}
HGP_WINDOW_PROP *hgp_window_addWindow(double x, double y, double width, double height)
{
    if (hgp_status.init_flag != 1) //initialize check
    {
        printf("need initialize\n");
        return NULL;
    }
    if (hgp_status.window_list_entry == NULL) //initialize
    {
        hgp_status.window_list_entry = malloc(sizeof(HGP_WINDOW_LIST));
        hgp_status.window_list_entry->Next = NULL;
        hgp_status.window_list_entry->This = NULL;
        hgp_status.window_list_entry->Prev = NULL;
    }
    HGP_WINDOW_LIST *tmp_window_list = hgp_status.window_list_entry;
    while (tmp_window_list->Next != NULL) //find tail//FIXIT:use next instead this;
    {
        tmp_window_list = tmp_window_list->Next;
    }
    //create new node;
    tmp_window_list->Next = malloc(sizeof(HGP_WINDOW_LIST));
    tmp_window_list->Next->Prev = tmp_window_list;
    tmp_window_list->Next->This = NULL;
    tmp_window_list->Next->Next = NULL;
    //initialize window
    tmp_window_list->This = malloc(sizeof(HGP_WINDOW_PROP));
    HGP_WINDOW_PROP *tmp_window_prop = tmp_window_list->This;
    tmp_window_prop->x = x;
    tmp_window_prop->y = y;
    tmp_window_prop->width = width;
    tmp_window_prop->height = height;
    tmp_window_prop->my_node = tmp_window_list;
    tmp_window_prop->layer_mode = 0;
    if (x == -1 || y == -1)
    {
        tmp_window_prop->window_id = HgOpen(width, height);
    }
    else
    {
        tmp_window_prop->window_id = HgWOpen(x, y, width, height);
    }
    //initialize layer list
    tmp_window_prop->layer_head_node = NULL;
}
int hgp_window_deleteWindow(HGP_WINDOW_PROP *target_window)
{
    if (target_window == NULL)
    {
        printf("HGP_ERROR:hgp_window_deleteWindow(NULL)\n");
        return 0;
    }
    if (hgp_status.pointer_checker)
    {
        printf("HGP_DEBUG:hgp_window_deleteWindow(ptr):check\n");
        target_window->x;
    }
    //todo:delete layer;
    HgWClose(target_window->window_id);
    HGP_WINDOW_LIST *tmp_window_list = target_window->my_node;
    free(tmp_window_list->This);
    //delete node;
    if (tmp_window_list->Prev == NULL)
    {
        if (tmp_window_list->Next == NULL)
        {
            hgp_status.window_list_entry = NULL;
            free(tmp_window_list);
            return 1;
        }
        else
        {
            hgp_status.window_list_entry = tmp_window_list->Next;
            tmp_window_list->Next->Prev = NULL;
            free(tmp_window_list);
            return 1;
        }
    }
    else
    {
        if (tmp_window_list->Next == NULL)
        {
            tmp_window_list->Prev->Next = NULL;
            free(tmp_window_list);
            return 1;
        }
        else
        {
            tmp_window_list->Prev->Next = tmp_window_list->Next;
            tmp_window_list->Next->Prev = tmp_window_list->Prev;
            free(tmp_window_list);
            return 1;
        }
    }
    return -1; //unknown falure...
}
int hgp_window_sync(HGP_WINDOW_PROP *target_window) //TODO:
{
}
//---------------------------------------
HGP_LAYER_PROP *hgp_layer_addLayer(HGP_WINDOW_PROP *target_window)
{
    if (target_window == NULL)
    {
        printf("HGP_ERROR:hgp_layer_addLayer(NULL)\n");
        return NULL;
    }
    //TODO:add point checker;
    //add layer_mode=0 function;
    HGP_LAYER_LIST *tmp_layer_list = target_window->layer_head_node;
    if (tmp_layer_list == NULL)
    {
        tmp_layer_list = malloc(sizeof(HGP_LAYER_LIST));
        tmp_layer_list->Prev = NULL;
        tmp_layer_list->This = NULL;
        tmp_layer_list->Next = NULL;
    }
    else
    {
        while (tmp_layer_list->Next != NULL)
        {
            tmp_layer_list = tmp_layer_list->Next;
        }
        tmp_layer_list->Next = malloc(sizeof(HGP_LAYER_LIST));
        tmp_layer_list->Next->Next = NULL;
        tmp_layer_list->Next->Prev = tmp_layer_list;
    }
    tmp_layer_list->Next->This = malloc(sizeof(HGP_LAYER_PROP));
    HGP_LAYER_PROP *tmp_layer_prop = tmp_layer_list->Next->This;
    tmp_layer_prop->layer_id[0] = HgWAddLayer(target_window->window_id);
    //todo:add double layer func
    tmp_layer_prop->my_node = tmp_layer_list;
    tmp_layer_prop->window_prop = target_window;
    tmp_layer_prop->visible_flag = 1;
    tmp_layer_prop->model_head_node = NULL;
    tmp_layer_prop->model_tail_node = NULL;
}
int hgp_layer_deleteLayer(HGP_LAYER_PROP *target_layer)
{
    if (target_layer == NULL)
    {
        printf("HGP_ERROR:hgp_layer_deleteLayer(NULL)\n");
        return 0;
    }
    //TODO:add point checker;
    HGP_LAYER_LIST *tmp_layer_list = target_layer->my_node;
    HgLRemove(target_layer->layer_id[0]);
    //TODO:add double layer remove func
    //TODO:Add model remove func
    if (tmp_layer_list->Prev == NULL)
    {
        if (tmp_layer_list->Next == NULL)
        {
            target_layer->window_prop->layer_head_node = NULL;
            free(target_layer);
            free(tmp_layer_list);
            return 1;
        }
        else
        {
            target_layer->window_prop->layer_head_node = tmp_layer_list->Next;
            free(target_layer);
            free(tmp_layer_list);
            return 1;
        }
    }
    else if (tmp_layer_list->Next == NULL)
    {
        tmp_layer_list->Prev->Next = NULL;
        free(target_layer);
        free(tmp_layer_list);
        return 1;
    }
    else
    {
        tmp_layer_list->Next->Prev = tmp_layer_list->Prev;
        tmp_layer_list->Prev->Next = tmp_layer_list->Next;
        free(target_layer);
        free(tmp_layer_list);
    }
    return -1;
}
int hgp_layer_sync(HGP_LAYER_PROP *target_layer)
{
}
//---------------------------------------
HGP_COMMON_MODEL_PROP *hgp_model_addModel(HGP_LAYER_PROP *target_layer, int type, double x, double y, int isInsertTail)
{
    if (target_layer == NULL)
    {
        printf("HGP_ERROR:hgp_model_addModel(NULL)\n");
        return NULL;
    }
    HGP_COMMON_MODEL_PROP *tmp_model_prop = malloc(sizeof(HGP_COMMON_MODEL_PROP));
    HGP_COMMON_MODEL_LIST *tmp_model_list = malloc(sizeof(HGP_COMMON_MODEL_LIST));
    tmp_model_prop->change_flag = -1;
    tmp_model_prop->layer_prop = target_layer;
    tmp_model_prop->model_type = type;
    tmp_model_prop->my_node = tmp_model_list;
    tmp_model_prop->visible_flag = 1;
    tmp_model_prop->x = x;
    tmp_model_prop->y = y;
    tmp_model_prop->rotate_arc = 0;
    tmp_model_prop->zoom_rate = 1;
    void *tmp_model_prop_ptr;
    switch (type)
    {
    case HGP_MODELTYPE_RECT:
        tmp_model_prop_ptr = malloc(sizeof(HGP_RECT));
        break;
    case HGP_MODELTYPE_CIRCLE:
        tmp_model_prop_ptr = malloc(sizeof(HGP_CIRCLE));
        break;
    case HGP_MODELTYPE_ARC:
        tmp_model_prop_ptr = malloc(sizeof(HGP_ARC));
        break;
    case HGP_MODELTYPE_FAN:
        tmp_model_prop_ptr = malloc(sizeof(HGP_FAN));
        break;
    case HGP_MODELTYPE_LINE:
        tmp_model_prop_ptr = malloc(sizeof(HGP_LINE));
        break;
    case HGP_MODELTYPE_POLYGON:
        tmp_model_prop_ptr = malloc(sizeof(HGP_POLYGON));
        break;
    default:
    {
        printf("HGP_ERROR:addModel type error\n");
        free(tmp_model_prop);
        free(tmp_model_list);
        return NULL;
    }
    }
    //list
    if (target_layer->model_head_node == NULL)
    {
        target_layer->model_head_node = tmp_model_list;
        target_layer->model_tail_node = tmp_model_list;
    }
    else
    {
        if (isInsertTail)
        {
            tmp_model_list->Next = NULL;
            tmp_model_list->This = tmp_model_prop;
            tmp_model_list->Prev = target_layer->model_tail_node;
            target_layer->model_tail_node->Next = tmp_model_list;
            target_layer->model_tail_node = tmp_model_list;
        }
        else
        {
            tmp_model_list->Next = target_layer->model_head_node;
            tmp_model_list->This = tmp_model_prop;
            tmp_model_list->Prev = NULL;
            target_layer->model_head_node->Prev = tmp_model_list;
            target_layer->model_head_node = tmp_model_list;
        }
    }
}
int hgp_model_deleteModel(HGP_COMMON_MODEL_PROP *target_model)
{
    if (target_model == NULL)
    {
        printf("HGP_ERROR:deleteModel(NULL)\n");
        return NULL;
    }
    HGP_COMMON_MODEL_LIST *tmp_model_list = target_model->my_node;
    //TODO:ADD POLYGON FREE FUNC;
    tmp_model_list->This = NULL;
    //list operation
    if (tmp_model_list->Prev == NULL)
    {
        if (tmp_model_list->Next == NULL)
        {
            target_model->layer_prop->model_head_node = NULL;
            target_model->layer_prop->model_tail_node = NULL;
        }
        else
        {
            target_model->layer_prop->model_head_node = tmp_model_list->Next;
            target_model->layer_prop->model_head_node->Prev = NULL;
        }
    }
    else
    {
        if (tmp_model_list->Next == NULL)
        {
            target_model->layer_prop->model_tail_node = tmp_model_list->Prev;
            target_model->layer_prop->model_tail_node->Next = NULL;
        }
        else
        {
            tmp_model_list->Prev->Next = tmp_model_list->Next;
            tmp_model_list->Next->Prev = tmp_model_list->Prev;
        }
    }
    free(target_model);
}
int hgp_model_move_to(HGP_COMMON_MODEL_PROP *target_model, double target_x, double target_y)
{
    target_model->x = target_x;
    target_model->y = target_y;
}
int hgp_model_move_byAngle(HGP_COMMON_MODEL_PROP *target_model, double direct_arc, double distance)
{
    target_model->x += cos(direct_arc) * distance;
    target_model->y += sin(direct_arc) * distance;
}
int hgp_model_sync(HGP_COMMON_MODEL_PROP *target_model)
{
    int window_id;
    switch (target_model->layer_prop->window_prop->layer_mode)
    {
    case -1:
        window_id = target_model->layer_prop->window_prop->window_id;
        break;
    case 0:
        window_id = target_model->layer_prop->layer_id[0];
        break;
    case 1:
        window_id = target_model->layer_prop->layer_id[hgp_status.double_cache_flag];
        break;
    default:
        return 0;
    }
    switch (target_model->model_type)
    {
    case HGP_MODELTYPE_RECT:
    {
        HGP_RECT *tmp_model_prop = target_model->model_ptr;
        HgWSetWidth(window_id, tmp_model_prop->line_width);
        HGCSetColor(window_id, tmp_model_prop->fill_color, HG_ColorFill);
        HGCSetColor(window_id, tmp_model_prop->shell_color, HG_ColorDraw);
        HGCRectangle(window_id, target_model->x, target_model->y,
                     target_model->zoom_rate * tmp_model_prop->width / 2,
                     target_model->zoom_rate * tmp_model_prop->height / 2,
                     target_model->rotate_arc, tmp_model_prop->fill_flag, tmp_model_prop->stroke_flag);
    }
    break;
    case HGP_MODELTYPE_CIRCLE:
    {
        HGP_CIRCLE *tmp_model_prop = target_model->model_ptr;
        HgWSetWidth(window_id, tmp_model_prop->line_width);
        HGCSetColor(window_id, tmp_model_prop->fill_color, HG_ColorFill);
        HGCSetColor(window_id, tmp_model_prop->shell_color, HG_ColorDraw);
        HGCCircle(window_id, target_model->x, target_model->y, target_model->zoom_rate * tmp_model_prop->r,
                  tmp_model_prop->fill_color, tmp_model_prop->stroke_flag);
    }
    break;
    case HGP_MODELTYPE_ARC:
    {
        
    }
    }
}
//---------------------------------------
HGP_POLYGON_NODE *hgp_model_polygon_addPoint(HGP_POLYGON *target_polygon, int insert_location, HGP_POLYGON_NODE *target_node, double x, double y)
{
}
int hgp_model_polygon_delPoint(int mode, HGP_POLYGON_NODE *target_node, int target_count)
{
}
//---------------------------------------
HGP_RECT *hgp_model_rectInit(HGP_LAYER_PROP *target_layer, double x, double y, double width, double height, unsigned long shell_color, unsigned long fill_color, double rotate_arc, double line_width, int fill_flag, int stroke_flag)
{
}
HGP_CIRCLE *hgp_model_circleInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, unsigned long fill_color, double line_width, int fill_flag, int stroke_flag)
{
}
HGP_ARC *hgp_model_arcInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, double arc_start, double arc_value, double line_width)
{
}
HGP_FAN *hgp_model_fanInit(HGP_LAYER_PROP *target_layer, double x, double y, double r, unsigned long shell_color, unsigned long fill_color, double are_start, double arc_value, double line_width, int fill_flag, int stroke_flag)
{
}
HGP_LINE *hgp_model_lineInit(HGP_LAYER_PROP *target_layer, double x, double y, double end_x, double end_y, unsigned long line_color, double line_width); //end_x,end_y is relative coordinate of x,y
HGP_POLYGON *hgp_model_polygonInit(HGP_LAYER_PROP *target_layer, double x, double y, unsigned long line_color, unsigned long fill_color, double line_width, int fill_flag, int stroke_flag)
{
}
