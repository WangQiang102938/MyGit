#include "handyplus.h"
#define breakp breakpoint();
#define update hgp_update(1);

int main()
{
    HGP_WINDOW_INFO *wid = hgp_window_init(200, 200, -1, -1);
    hgp_add_layer(wid);
    HGP_RECT *rect1 = hgp_rect_init(wid->start_layer_node, 100, 100, 50, 50, HG_BLACK, HG_YELLOW, 1, 0, 2);
    HGP_CIRCLE *circle1 = hgp_circle_init(wid->start_layer_node, 100, 100, 30, HG_BLACK, HG_GREEN, 1, 1);
    HGP_ARC *arc1 = hgp_arc_init(wid->start_layer_node, 100, 100, 40, HG_RED, M_PI, M_PI / 2);

    hgp_update(1);
    HgSleep(1);
    for (int i = 0; i < 50; i++)
    {
        hgp_object_move(rect1->obj_ptr, HGP_DIRECT_UP, 1);
        hgp_object_move(circle1->obj_ptr, HGP_DIRECT_DOWN, 1);
        hgp_object_move(arc1->obj_ptr, HGP_DIRECT_LEFT, 1);
        hgp_update(1);
        HgSleep(0.03);
    }
    hgp_delete_object(rect1->obj_ptr);
    update
        HgSleep(2);
    HGP_WINDOW_INFO *wid2 = hgp_create_window(200, 200, -1, -1);
    hgp_add_layer(wid2);
    HGP_RECT *rect2 = hgp_rect_init(wid2->start_layer_node, 100, 100, 50, 50, HG_BLACK, HG_YELLOW, 1, 0, 0);
    hgp_update(1);
    for (int i = 0; i < 5; i++)
    {
        hgp_object_move(rect2->obj_ptr, HGP_DIRECT_UP, 10);
        hgp_update(1);
        HgSleep(0.05);
    }
    HgSleep(3);
    hgp_quit();
    return 0;
}

