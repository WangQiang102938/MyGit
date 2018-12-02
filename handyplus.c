#include "handyplus.h"
//#include "handy.h"
#define HGP_THIS_WINDOW HGP_WINDOW_CONTAINER[window]
#define HGP_THIS_LAYER HGP_WINDOW_CONTAINER[window]->Layer[layer]
#define HGP_THIS_OBJECT HGP_WINDOW_CONTAINER[window]->Layer[layer]->Object
#define breakp breakpoint();


int hgp_window_init(double x, double y)
{
    hgp_layer_flag_1.layer_reverse_flag = 0;
    hgp_layer_flag_1.nextnode = &hgp_layer_flag_2;
    hgp_layer_flag_2.layer_reverse_flag = 1;
    hgp_layer_flag_2.nextnode = &hgp_layer_flag_3;
    hgp_layer_flag_3.layer_reverse_flag = 2;
    hgp_layer_flag_3.nextnode = &hgp_layer_flag_1;
    HGP_LAYER_FLAG_CURRENT_PTR = &hgp_layer_flag_1;
    for (int i = 0; i < HG_MAX_WINDOWS; i++)
    {
        HGP_WINDOW_CONTAINER[i] = NULL;
    }
    int wid = hgp_create_window(x, y);
    if (wid == -1)
    {
        printf("window init failed\n");
    }
    // memset()
    return wid;
}

int hgp_create_window(double x, double y)
{
    int window = -1;
    for (int i = 0; i < HG_MAX_WINDOWS; i++)
    {
        if (HGP_WINDOW_CONTAINER[i] == NULL)
        {
            window = i;
            break;
        }
    }
    breakp;
    if (window != -1)
    {
        HGP_THIS_WINDOW = ((HGP_WINDOW_INFO *)malloc(sizeof(HGP_WINDOW_INFO)));
        HGP_THIS_WINDOW->wid = HgOpen(x, y);
        HGP_THIS_WINDOW->window_x = x;
        HGP_THIS_WINDOW->window_y = y;
        for (int i = 0; i < HG_MAX_LAYERS; i++)
        {
            HGP_THIS_WINDOW->Layer[i] = NULL;
        }
        hgp_add_layer(window);
    }
    breakp;
    return window;
}

int hgp_add_layer(int window)
{
    int layer = -1;
    for (int i = 0; i < HG_MAX_LAYERS; i++)
    {
        if (HGP_THIS_WINDOW->Layer[i] == NULL)
        {
            layer = i;
            break;
        }
    }
    if (layer != -1)
    {
        HGP_THIS_WINDOW->Layer[layer] = ((HGP_LAYER_INFO *)malloc(sizeof(HGP_LAYER_INFO)));
        HGP_THIS_WINDOW->Layer[layer]->lid[0] = HgWAddLayer(HGP_THIS_WINDOW->wid);
        HGP_THIS_WINDOW->Layer[layer]->lid[1] = HgWAddLayer(HGP_THIS_WINDOW->wid);
        HGP_THIS_WINDOW->Layer[layer]->lid[2] = HgWAddLayer(HGP_THIS_WINDOW->wid);
        HGP_THIS_WINDOW->Layer[layer]->BackGroundColor = HG_WHITE;

        for (int i = 0; i < HGP_OBJECT_TOTAL_NUM; i++)
        {
            HGP_THIS_OBJECT[i] = NULL;
        }
    }
    return layer;
}

void *hgp_add_object(int obj_type_flag, int window, int layer) //TODO:add obj
{
    int obj_id = -1;
    for (int i = 0; i < HGP_OBJECT_TOTAL_NUM; i++)
    {
        if (HGP_THIS_OBJECT[i] == NULL)
        {
            obj_id = i;
            break;
        }
    }
    if (obj_id != -1)
    {
        switch (obj_type_flag)
        {
        case HGP_OBJECT_RECT_FLAG:
        {
            HGP_THIS_OBJECT[obj_id] = malloc(sizeof(HGP_OBJECT));
            HGP_THIS_OBJECT[obj_id]->pointer = malloc(sizeof(HGP_RECT));
            HGP_THIS_OBJECT[obj_id]->type = HGP_OBJECT_RECT_FLAG;
            HGP_RECT *obj_pointer = HGP_THIS_OBJECT[obj_id]->pointer;
            obj_pointer->window_info.window_id = window;
            obj_pointer->window_info.layer_id = layer;
            obj_pointer->window_info.object_pointer_id = obj_id;
            HGP_THIS_OBJECT[obj_id]->window_info = obj_pointer->window_info;
            return obj_pointer;
        }
        case HGP_OBJECT_CIRCLE_FLAG:
        {
            HGP_THIS_OBJECT[obj_id] = malloc(sizeof(HGP_OBJECT));
            HGP_THIS_OBJECT[obj_id]->pointer = malloc(sizeof(HGP_CIRCLE));
            HGP_THIS_OBJECT[obj_id]->type = HGP_OBJECT_CIRCLE_FLAG;
            HGP_CIRCLE *obj_pointer = HGP_THIS_OBJECT[obj_id]->pointer;
            obj_pointer->window_info.window_id = window;
            obj_pointer->window_info.layer_id = layer;
            obj_pointer->window_info.object_pointer_id = obj_id;
            HGP_THIS_OBJECT[obj_id]->window_info = obj_pointer->window_info;
            return obj_pointer;
        }
        case HGP_OBJECT_ARC_FLAG:
        {
            HGP_THIS_OBJECT[obj_id] = malloc(sizeof(HGP_OBJECT));
            HGP_THIS_OBJECT[obj_id]->pointer = malloc(sizeof(HGP_ARC));
            HGP_THIS_OBJECT[obj_id]->type = HGP_OBJECT_ARC_FLAG;
            HGP_ARC *obj_pointer = HGP_THIS_OBJECT[obj_id]->pointer;
            obj_pointer->window_info.window_id = window;
            obj_pointer->window_info.layer_id = layer;
            obj_pointer->window_info.object_pointer_id = obj_id;
            HGP_THIS_OBJECT[obj_id]->window_info = obj_pointer->window_info;
            return obj_pointer;
        }
        case HGP_OBJECT_FAN_FLAG:
        {
            HGP_THIS_OBJECT[obj_id] = malloc(sizeof(HGP_OBJECT));
            HGP_THIS_OBJECT[obj_id]->pointer = malloc(sizeof(HGP_FAN));
            HGP_THIS_OBJECT[obj_id]->type = HGP_OBJECT_FAN_FLAG;
            HGP_FAN *obj_pointer = HGP_THIS_OBJECT[obj_id]->pointer;
            obj_pointer->window_info.window_id = window;
            obj_pointer->window_info.layer_id = layer;
            obj_pointer->window_info.object_pointer_id = obj_id;
            HGP_THIS_OBJECT[obj_id]->window_info = obj_pointer->window_info;
            return obj_pointer;
        }
        case HGP_OBJECT_LINE_FLAG:
        {
            HGP_THIS_OBJECT[obj_id] = malloc(sizeof(HGP_OBJECT));
            HGP_THIS_OBJECT[obj_id]->pointer = malloc(sizeof(HGP_LINE));
            HGP_THIS_OBJECT[obj_id]->type = HGP_OBJECT_LINE_FLAG;
            HGP_LINE *obj_pointer = HGP_THIS_OBJECT[obj_id]->pointer;
            obj_pointer->window_info.window_id = window;
            obj_pointer->window_info.layer_id = layer;
            obj_pointer->window_info.object_pointer_id = obj_id;
            HGP_THIS_OBJECT[obj_id]->window_info = obj_pointer->window_info;
            return obj_pointer;
        }
        case HGP_OBJECT_POLYGON_FLAG:
        {
            HGP_THIS_OBJECT[obj_id] = malloc(sizeof(HGP_OBJECT));
            HGP_THIS_OBJECT[obj_id]->pointer = malloc(sizeof(HGP_POLYGON));
            HGP_THIS_OBJECT[obj_id]->type = HGP_OBJECT_POLYGON_FLAG;
            HGP_POLYGON *obj_pointer = HGP_THIS_OBJECT[obj_id]->pointer;
            obj_pointer->window_info.window_id = window;
            obj_pointer->window_info.layer_id = layer;
            obj_pointer->window_info.object_pointer_id = obj_id;
            HGP_THIS_OBJECT[obj_id]->window_info = obj_pointer->window_info;
            return obj_pointer;
        }
        default:
            return NULL;
        }
    }
    return NULL;
}

int hgp_delete_object(HGP_OBJECT_WINDOW_INFO obj_info) //TODO:obj
{
    int window = obj_info.window_id;
    int layer = obj_info.layer_id;
    int obj_id = obj_info.object_pointer_id;
    if (HGP_THIS_OBJECT[obj_id] != NULL)
    {
        free(HGP_THIS_OBJECT[obj_id]->pointer);
        free(HGP_THIS_OBJECT[obj_id]);
        //HGP_THIS_OBJECT[obj_id]->pointer = NULL;
        HGP_THIS_OBJECT[obj_id] = NULL;
    }
    return 1;
}

int hgp_delete_layer(int window, int layer)
{
    for (int i = 0; i < HGP_OBJECT_TOTAL_NUM; i++)
    {
        if (HGP_THIS_OBJECT[i] != NULL)
            hgp_delete_object(HGP_THIS_OBJECT[i]->window_info);
    }
    HgLRemove(HGP_THIS_LAYER->lid[0]);
    HgLRemove(HGP_THIS_LAYER->lid[1]);
    free(HGP_THIS_WINDOW->Layer[layer]);
    HGP_THIS_LAYER = NULL;
    return 1;
}

int hgp_destroy_window(int window, int quit_flag)
{
    for (int i = HG_MAX_LAYERS - 1; i >= 0; i--)
    {
        if (HGP_THIS_WINDOW->Layer[i] != NULL)
        {
            if (quit_flag == 1)
            {
                hgp_delete_layer(window, i);
            }
        }
    }
    HgSleep(0.1);
    HgWClose(HGP_THIS_WINDOW->wid);
    HgSleep(0.1);
    free(HGP_THIS_WINDOW);
    HGP_THIS_WINDOW = NULL;
    return 1;
}

void breakpoint()
{
    printf("breakpoint:%d\n", breakpointcount);
    breakpointcount++;
}

int hgp_update(int flag) //TODO:add obj
{
    HGP_OBJECT_WINDOW_INFO windowinfo;
    for (int window = 0; window < HG_MAX_WINDOWS; window++)
    {
        if (HGP_THIS_WINDOW != NULL)
        {
            for (int layer = 0; layer < HG_MAX_LAYERS; layer++)
            {
                if (HGP_THIS_LAYER != NULL)
                {
                    HgLShow(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->nextnode->nextnode->layer_reverse_flag], 1);
                    HgLShow(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], 0);
                    HgLClear(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->nextnode->layer_reverse_flag]);
                    for (int obj_id = 0; obj_id < HGP_OBJECT_TOTAL_NUM; obj_id++)
                    {
                        if (HGP_THIS_OBJECT[obj_id] != NULL)
                        {
                            windowinfo.window_id = window;
                            windowinfo.layer_id = layer;
                            windowinfo.object_pointer_id = obj_id;
                            hgp_single_draw(windowinfo);
                        }
                    }
                    HGP_LAYER_FLAG_CURRENT_PTR = HGP_LAYER_FLAG_CURRENT_PTR->nextnode;
                    if (flag)
                    {
                        HgLShow(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->nextnode->nextnode->layer_reverse_flag], 1);
                        HgLShow(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], 0);
                        HgLClear(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->nextnode->layer_reverse_flag]);
                        HGP_LAYER_FLAG_CURRENT_PTR = HGP_LAYER_FLAG_CURRENT_PTR->nextnode;
                    }
                }
            }
        }
    } 
    return 1;
}

    int hgp_single_draw(HGP_OBJECT_WINDOW_INFO window_info)
    {
        int window = window_info.window_id;
        int layer = window_info.layer_id;
        int obj_id = window_info.object_pointer_id;

        switch (HGP_THIS_OBJECT[obj_id]->type)
        {
        case HGP_OBJECT_RECT_FLAG:
        {
            HGP_RECT *obj_pointer = ((HGP_RECT *)HGP_THIS_OBJECT[obj_id]->pointer);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->fill_color, HG_ColorFill);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->shell_color, HG_ColorDraw);
            HGCRectangle(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
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
            HGP_CIRCLE *obj_pointer = ((HGP_CIRCLE *)HGP_THIS_OBJECT[obj_id]->pointer);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->fill_color, HG_ColorFill);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->shell_color, HG_ColorDraw);
            HGCCircle(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
                      obj_pointer->x,
                      obj_pointer->y,
                      obj_pointer->r,
                      obj_pointer->fill_flag,
                      obj_pointer->stroke_lenth);
            break;
        }
        case HGP_OBJECT_ARC_FLAG:
        {
            HGP_ARC *obj_pointer = ((HGP_ARC *)HGP_THIS_OBJECT[obj_id]->pointer);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->shell_color, HG_ColorDraw);
            HGCFan(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
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
            HGP_FAN *obj_pointer = ((HGP_FAN *)HGP_THIS_OBJECT[obj_id]->pointer);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->shell_color, HG_ColorDraw);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->fill_color, HG_ColorFill);
            HGCFan(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
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
            HGP_LINE *obj_pointer = ((HGP_LINE *)HGP_THIS_OBJECT[obj_id]->pointer);
            HgWSetWidth(obj_pointer->window_info.window_id, obj_pointer->line_width);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->color, HG_ColorDraw);
            HgWLine(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->start_x,
                    obj_pointer->start_y, obj_pointer->end_x, obj_pointer->end_y);
            break;
        }
        case HGP_OBJECT_POLYGON_FLAG: //todo
        {
            HGP_POLYGON *obj_pointer = ((HGP_POLYGON *)HGP_THIS_OBJECT[obj_id]->pointer);
            HgWSetWidth(obj_pointer->window_info.window_id, obj_pointer->line_width);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->color, HG_ColorDraw);
            HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->fill_color, HG_ColorFill);
            HGCPolygon(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->counter,
                       obj_pointer->x, obj_pointer->y, obj_pointer->fill_flag, obj_pointer->stroke_lenth);
            break;
        }
        default:
            break;
        }
        return 1;
    }

    int hgp_direct_draw(HGP_OBJECT_WINDOW_INFO window_info)
    {
        HGP_LAYER_FLAG_CURRENT_PTR = HGP_LAYER_FLAG_CURRENT_PTR->nextnode->nextnode;
        hgp_single_draw(window_info);
        HGP_LAYER_FLAG_CURRENT_PTR = HGP_LAYER_FLAG_CURRENT_PTR->nextnode;
        return 0;
    }

    int hgp_erase_object(HGP_OBJECT_WINDOW_INFO window_info, unsigned long BackGroundColor)
    {
        {
            int window = window_info.window_id;
            int layer = window_info.layer_id;
            int obj_id = window_info.object_pointer_id;

            switch (HGP_THIS_OBJECT[obj_id]->type)
            {
            case HGP_OBJECT_RECT_FLAG:
            {
                HGP_RECT *obj_pointer = ((HGP_RECT *)HGP_THIS_OBJECT[obj_id]->pointer);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorFill);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorDraw);
                HGCRectangle(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
                             obj_pointer->x,
                             obj_pointer->y,
                             obj_pointer->width / 2 + 5,
                             obj_pointer->height / 2 + 5,
                             obj_pointer->rotate_arc,
                             obj_pointer->fill_flag,
                             obj_pointer->stroke_lenth);
                break;
            }
            case HGP_OBJECT_CIRCLE_FLAG:
            {
                HGP_CIRCLE *obj_pointer = ((HGP_CIRCLE *)HGP_THIS_OBJECT[obj_id]->pointer);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorFill);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorDraw);
                HGCCircle(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
                          obj_pointer->x,
                          obj_pointer->y,
                          obj_pointer->r + 5,
                          obj_pointer->fill_flag,
                          obj_pointer->stroke_lenth);
                break;
            }
            case HGP_OBJECT_ARC_FLAG:
            {
                HGP_ARC *obj_pointer = ((HGP_ARC *)HGP_THIS_OBJECT[obj_id]->pointer);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorDraw);
                HGCFan(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
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
                HGP_FAN *obj_pointer = ((HGP_FAN *)HGP_THIS_OBJECT[obj_id]->pointer);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorDraw);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorFill);
                HGCFan(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag],
                       obj_pointer->x,
                       obj_pointer->y,
                       obj_pointer->r + 5,
                       obj_pointer->arc_start,
                       obj_pointer->arc_start + obj_pointer->arc_value,
                       obj_pointer->fill_flag,
                       obj_pointer->stroke_lenth);
                break;
            }
            case HGP_OBJECT_LINE_FLAG:
            {
                HGP_LINE *obj_pointer = ((HGP_LINE *)HGP_THIS_OBJECT[obj_id]->pointer);
                HgWSetWidth(obj_pointer->window_info.window_id, obj_pointer->line_width + 5);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorDraw);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorFill);
                HGCRectangle(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], (obj_pointer->start_x + obj_pointer->end_x) / 2,
                             (obj_pointer->start_y + obj_pointer->end_y) / 2, fabs(obj_pointer->start_x - obj_pointer->end_x) + 2,
                             fabs(obj_pointer->start_y - obj_pointer->end_y) + 2, 0, 1, 1);
                break;
            }
            case HGP_OBJECT_POLYGON_FLAG:
            {
                HGP_POLYGON *obj_pointer = ((HGP_POLYGON *)HGP_THIS_OBJECT[obj_id]->pointer);
                HgWSetWidth(obj_pointer->window_info.window_id, obj_pointer->line_width);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->color, HG_ColorDraw);
                HGCSetColor(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], BackGroundColor, HG_ColorFill);
                HGCPolygon(HGP_THIS_LAYER->lid[HGP_LAYER_FLAG_CURRENT_PTR->layer_reverse_flag], obj_pointer->counter,
                           obj_pointer->x, obj_pointer->y, obj_pointer->fill_flag, obj_pointer->stroke_lenth);
                break;
            }
            default:
                break;
            }
        }
        return 1;
    }

    int hgp_quit() //disabled hgp_killHG
    {
        for (int i = HG_MAX_WINDOWS - 1; i >= 0; i--)
        {
            if (HGP_WINDOW_CONTAINER[i] != NULL)
            {
                hgp_destroy_window(i, 1);
            }
        }
        HgSleep(1);
        //hgp_killHG();
        return 1;
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

    int hgp_object_move(HGP_OBJECT_WINDOW_INFO window_info, double direct_arc, double distance) //TODO:add obj
    {
        int window = window_info.window_id;
        int layer = window_info.layer_id;
        int obj_id = window_info.object_pointer_id;
        int type = HGP_THIS_OBJECT[obj_id]->type;
        void *ptr = HGP_THIS_OBJECT[obj_id]->pointer;
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

    int hgp_object_zoom(HGP_OBJECT_WINDOW_INFO window_info, double zoom_rate, double zoom_center_x, double zoom_center_y) //TODO:add obj
    {
        int window = window_info.window_id;
        int layer = window_info.layer_id;
        int obj_id = window_info.object_pointer_id;
        int type = HGP_THIS_OBJECT[obj_id]->type;
        void *ptr = HGP_THIS_OBJECT[obj_id]->pointer;
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

    int hgp_object_rotate(HGP_OBJECT_WINDOW_INFO window_info, double rotate_arc, int need_angle_input_1) //TODO:add obj:line polygon
    {
        if (need_angle_input_1)
        {
            rotate_arc = rotate_arc * M_PI / 180;
            //printf("\nangle 2 arc:%lf\n",rotate_arc);
        }
        int window = window_info.window_id;
        int layer = window_info.layer_id;
        int obj_id = window_info.object_pointer_id;
        int type = HGP_THIS_OBJECT[obj_id]->type;
        void *ptr = HGP_THIS_OBJECT[obj_id]->pointer;
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
    HGP_RECT *hgp_rect_init(int window, int layer, double x, double y,
                            double width, double height,
                            unsigned long shell_color, unsigned long fill_color,
                            int fill_flag, double rotate_arc, int stroke_lenth, HGP_OBJECT **obj_pointer)
    {
        HGP_RECT *Operate_Rect_Pointer =
            (hgp_add_object(HGP_OBJECT_RECT_FLAG, window, layer));
        if (obj_pointer != NULL)
            *obj_pointer = HGP_THIS_OBJECT[Operate_Rect_Pointer->window_info.object_pointer_id];
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

    HGP_CIRCLE *hgp_circle_init(int window, int layer,
                                double x, double y, double r,
                                unsigned long shell_color,
                                unsigned long fill_color,
                                int fill_flag, int stroke_lenth, HGP_OBJECT **obj_pointer)
    {
        HGP_CIRCLE *Operate_Circle_Pointer =
            ((HGP_CIRCLE *)hgp_add_object(HGP_OBJECT_CIRCLE_FLAG, window, layer));
        HGP_OBJECT_WINDOW_INFO windowinfo = Operate_Circle_Pointer->window_info;
        if (obj_pointer != NULL)
            *obj_pointer = HGP_THIS_OBJECT[windowinfo.object_pointer_id];
        Operate_Circle_Pointer->x = x;
        Operate_Circle_Pointer->y = y;
        Operate_Circle_Pointer->r = r;
        Operate_Circle_Pointer->shell_color = shell_color;
        Operate_Circle_Pointer->fill_color = fill_color;
        Operate_Circle_Pointer->fill_flag = fill_flag;
        Operate_Circle_Pointer->stroke_lenth = stroke_lenth;
        return Operate_Circle_Pointer;
    }

    HGP_ARC *hgp_arc_init(int window, int layer,
                          double x, double y, double r,
                          unsigned long shell_color,
                          double arc_start, double arc_value, HGP_OBJECT **obj_pointer)
    {
        HGP_ARC *Operate_Arc_Pointer =
            ((HGP_ARC *)hgp_add_object(HGP_OBJECT_ARC_FLAG, window, layer));
        HGP_OBJECT_WINDOW_INFO windowinfo = Operate_Arc_Pointer->window_info;
        if (obj_pointer != NULL)
            *obj_pointer = HGP_THIS_OBJECT[windowinfo.object_pointer_id];
        Operate_Arc_Pointer->x = x;
        Operate_Arc_Pointer->y = y;
        Operate_Arc_Pointer->r = r;
        Operate_Arc_Pointer->shell_color = shell_color;
        Operate_Arc_Pointer->arc_start = arc_start;
        Operate_Arc_Pointer->arc_value = arc_value;
        return Operate_Arc_Pointer;
    }

    HGP_FAN *hgp_fan_init(int window, int layer,
                          double x, double y, double r,
                          unsigned long shell_color,
                          unsigned long fill_color,
                          int fill_flag, int stroke_lenth,
                          double arc_start, double arc_value, HGP_OBJECT **obj_pointer)
    {
        HGP_FAN *Operate_Fan_Pointer =
            ((HGP_FAN *)hgp_add_object(HGP_OBJECT_FAN_FLAG, window, layer));
        HGP_OBJECT_WINDOW_INFO windowinfo = Operate_Fan_Pointer->window_info;
        if (obj_pointer != NULL)
            *obj_pointer = HGP_THIS_OBJECT[windowinfo.object_pointer_id];
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

    HGP_LINE *hgp_line_init(int window, int layer, double start_x, double start_y, double end_x, double end_y,
                            unsigned long color, double line_width, HGP_OBJECT **obj_pointer)
    {
        HGP_LINE *Operate_Obj_Pointer =
            ((HGP_LINE *)hgp_add_object(HGP_OBJECT_LINE_FLAG, window, layer));
        HGP_OBJECT_WINDOW_INFO windowinfo = Operate_Obj_Pointer->window_info;
        if (obj_pointer != NULL)
            *obj_pointer = HGP_THIS_OBJECT[windowinfo.object_pointer_id];
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