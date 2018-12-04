#include "handyplus.h"

int main()
{
    HGP_WINDOW_INFO* wid = hgp_window_init(400, 400,-1,-1);
    hgp_add_layer(wid);
    HGP_RECT *testrect = hgp_rect_init(wid->start_layer_node, 50, 50, 50, 50, HG_RED, HG_RED, 1, 0, 2);
    HGP_RECT *exitrect = hgp_rect_init(wid->start_layer_node, 400, 400, 50, 50, HG_RED, HG_RED, 1, 0, 2);
    HgSetEventMask(HG_MOUSE_EVENT_MASK);
    //hgp_update(1);
    hgevent *event;
    while (1)
    {
        event = HgEventNonBlocking();
        if (event != NULL)
        {
            //hgp_erase_object(testrect->window_info, HG_WHITE);
            testrect->x = event->x;
            testrect->y = event->y;
            //hgp_single_draw(testrect->window_info);
            hgp_update(1);
            if (event->x > 350 && event->y > 350)
            {
                break;
            }
        }
    }
    hgp_quit();
}