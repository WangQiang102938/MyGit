#include "final2.h"

int main()
{
    HGP_LAYER_INFO *layer = hgp_add_layer(hgp_window_init(boarder_x, boarder_x, -1, -1));
    FIGHTER_1 fighter;
    fighter.center_x = 200;
    fighter.center_y = 200;
    fighter_init(&fighter, layer, 2);
    //fighter_mirrow(&fighter);
    HgSetEventMask(HG_MOUSE_EVENT_MASK | HG_KEY_EVENT_MASK);
    hgp_update(1);

    BULLET my_bullet;
    my_bullet.direct = BULLET_DIRECT_UP;
    my_bullet.color = HG_GREEN;
    my_bullet.head=NULL;
    BULLET enemy_bullet;
    enemy_bullet.color = HG_RED;
    enemy_bullet.direct = BULLET_DIRECT_UP;
    enemy_bullet.head=NULL;
    timeval BULLET_MOVE_TIME = {0, 0};
    timeval BULLET_CREATE_TIME = {0, 0};
    timeval BREAKER_TIMER = {0, 0};
    timeval REFRESH_TIMER = {0, 0};
    timeval MOUSE_CHECK_TIMER = {0, 0};

    hgevent *event;
    int breakflag = 1;
    while (breakflag)
    {
        if(check_timer(&MOUSE_CHECK_TIMER,10))
        {
            event=HgEventNonBlocking();
            if(event!=NULL)
            {
                fighter_move(&fighter,event->x,event->y,1);
            }
        }
        if (check_timer(&BULLET_CREATE_TIME, 500))
        {
            create_bullet(&fighter, my_bullet.direct, &my_bullet, layer);
        }
        if (check_timer(&BULLET_MOVE_TIME, 50))
        {
            bullet_move(&my_bullet, 10);
            // bullet_move(&enemy_bullet, enemy_bullet.direct);
        }
        if(check_timer(&BREAKER_TIMER,5000))
        {
            HgEventNonBlocking();
        }
        if(check_timer(&REFRESH_TIMER,100))
        {
            hgp_update(0);
        }
    }
    HgGetChar();
    return 0;
}
