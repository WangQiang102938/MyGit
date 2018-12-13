#include "final2.h"

int main()
{
    HGP_LAYER_INFO *layer = hgp_add_layer(hgp_window_init(boarder_x, boarder_x, -1, -1));
    FIGHTER_1 fighter;
    fighter.center_x = 200;
    fighter.center_y = 200;
    fighter_init(&fighter, layer, 2);
    fighter_mirrow(&fighter);
    HgSetEventMask(HG_MOUSE_EVENT_MASK);
    hgp_update(1);

    hgevent *event;
    while (1)
    {
        event = HgEventNonBlocking();
        if (event != NULL)
        {
            fighter_move(&fighter, event->x, event->y, 1);
            hgp_update(1);
        }
    }
    HgGetChar();
    return 0;
}