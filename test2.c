#include "handyplus.h"

int main()
{
    int wid = hgp_window_init(400, 400);
    HGP_RECT *testrect = hgp_rect_init(wid, 0, 50, 50, 50, 50, HG_RED, HG_RED, 1, 0, 2,NULL);
    HGP_RECT *exitrect = hgp_rect_init(wid, 0, 400, 400, 50, 50, HG_RED, HG_RED, 1, 0, 2,NULL);
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