#include "handyplus.h"
#define breakp breakpoint();
#define update hgp_update(1);

int main()
{
    int wid = hgp_window_init(200, 200);
    HGP_RECT *rect1 = hgp_rect_init(wid, 0, 100, 100, 50, 50, HG_BLACK, HG_YELLOW, 1, 0, 2, NULL);
    HGP_CIRCLE *circle1 = hgp_circle_init(wid, 0, 100, 100, 30, HG_BLACK, HG_GREEN, 1, 2, NULL);
    HGP_ARC *arc1 = hgp_arc_init(wid, 0, 100, 100, 40, HG_BLACK, M_PI, M_PI / 2, NULL);

    hgp_update(1);
    HgSleep(1);
    for (int i = 0; i < 5; i++)
    {
        hgp_object_move(rect1->window_info, HGP_DIRECT_UP, 10);
        hgp_object_move(circle1->window_info, HGP_DIRECT_DOWN, 10);
        hgp_object_move(arc1->window_info, HGP_DIRECT_LEFT, 10);
        hgp_update(1);
        //HgSleep(0.03);
    }
    hgp_delete_object(rect1->window_info);
    update
        HgSleep(2);
    int wid2 = hgp_create_window(200, 200);
    HGP_RECT *rect2 = hgp_rect_init(wid2, 0, 100, 100, 50, 50, HG_BLACK, HG_YELLOW, 1, 0, 2, NULL);
    hgp_update(1);
    for (int i = 0; i < 5; i++)
    {
        hgp_object_move(rect2->window_info, HGP_DIRECT_UP, 10);
        hgp_update(1);
        HgSleep(0.05);
    }
    HgSleep(3);
    hgp_quit();
    return 0;
}