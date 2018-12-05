#include "handyplus.h"

int main()
{
    HGP_WINDOW_INFO* wid = hgp_window_init(400, 400,-1,-1);
    HGP_LAYER_INFO* layer_1= hgp_add_layer(wid);
    HGP_RECT *testrect = hgp_rect_init(wid->start_layer_node, 50, 50, 50, 50, HG_RED, HG_RED, 1, 0, 2);
    HGP_RECT *exitrect = hgp_rect_init(wid->start_layer_node, 400, 400, 50, 50, HG_RED, HG_RED, 1, 0, 2);
    HgSetEventMask(HG_MOUSE_EVENT_MASK);
    //hgp_update(1);
    hgevent *event;
    while (1)
    {
        testrect->x=rand()%400;
        testrect->y=rand()%400;
        testrect->width=rand()%400;
        testrect->height=rand()%400;
        testrect->shell_color=rand()%HG_WHITE;
        testrect->fill_color=rand()%HG_WHITE;
        hgp_update(1);
        HgSleep(0.1);
    }
    hgp_quit();
}