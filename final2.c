#include "final2.h"

int main()
{
    HGP_LAYER_INFO *layer = hgp_add_layer(hgp_window_init(boarder_x, boarder_x, -1, -1));
    FIGHTER fighter;
    FIGHTER enemy_fighter;
    fighter.center_x = 200;
    fighter.center_y = 200;
    fighter.color = HG_BLUE;
    enemy_fighter.center_x = 300;
    enemy_fighter.center_y = 300;
    enemy_fighter.color = HG_RED;
    fighter_init(&fighter, layer, 2);
    fighter_init(&enemy_fighter, layer, 1);
    fighter_mirrow(&enemy_fighter);
    HgSetEventMask(HG_MOUSE_EVENT_MASK | HG_KEY_EVENT_MASK);
    hgp_update(1);

    BULLET my_bullet;
    my_bullet.direct = BULLET_DIRECT_UP;
    my_bullet.color = HG_GREEN;
    my_bullet.head = NULL;
    BULLET enemy_bullet;
    enemy_bullet.color = HG_RED;
    enemy_bullet.direct = BULLET_DIRECT_DOWN;
    enemy_bullet.head = NULL;

    timeval BULLET_MOVE_TIME = {0, 0};
    timeval BULLET_CREATE_TIME = {0, 0};
    timeval BREAKER_TIMER = {0, 0};
    timeval REFRESH_TIMER = {0, 0};
    timeval MOUSE_CHECK_TIMER = {0, 0};
    timeval ENEMY_READ_DEMO_TIMER = {0, 0};

    logflag = 0;
    logpath_counter = 1;
    sprintf(logpathname, "path%d.dat", logpath_counter);
    loginfo = NULL;

    int loadflag = 0;
    int loadcounter = 1;
    int loaddemoflag = 0;
    read_path_list *path_list = NULL;
    read_path_list *path_list_head = NULL;
    FIGHTER cachefighter;
    cachefighter.path_current_node = NULL;
    cachefighter.path_head_node = NULL;

    hgevent *event;
    int breakflag = 1;
    while (breakflag)
    {
        if (check_timer(&MOUSE_CHECK_TIMER, 10))
        {
            event = HgEventNonBlocking();
            if (event != NULL)
            {
                if ((event->type | HG_MOUSE_EVENT_MASK) == HG_MOUSE_EVENT_MASK)
                    fighter_move(&enemy_fighter, event->x, event->y, 1);
                else
                {
                    if (event->ch == '1')
                    {
                        logflag = 1;
                        sprintf(logpathname, "path%d.dat", logpath_counter);
                        loginfo = path_log_init(logpathname);
                    }
                    if (event->ch == '2' && logflag != 0)
                    {
                        path_log_write(&loginfo);
                        logpath_counter++;
                        logflag = 0;
                    }
                    if (event->ch == '3' && loadflag == 0)
                    {
                        loadcounter = 1;
                        char pathcache[128] = {0};
                        sprintf(pathcache, "path%d.dat", loadcounter);
                        path_list = malloc(sizeof(read_path_list));
                        path_list_head = path_list;
                        while (1)
                        {
                            path_list->path = fopen(pathcache, "r");
                            if (path_list->path)
                            {
                                path_list->next = malloc(sizeof(read_path_list));
                                printf("%s;loaded\n", pathcache);
                                loadcounter++;
                                sprintf(pathcache, "path%d.dat", loadcounter);
                                path_list = path_list->next;
                            }
                            else
                            {
                                break;
                            }
                        }
                        loadflag = 1;
                    }
                }
            }
            if (logflag != 0)
            {
                path_log(loginfo, enemy_fighter.center_x, enemy_fighter.center_y);
            }
        }
        if (check_timer(&BULLET_CREATE_TIME, 500))
        {
            create_bullet(&fighter, my_bullet.direct, &my_bullet, layer);
            if (loaddemoflag)
                create_bullet(&cachefighter, enemy_bullet.direct, &enemy_bullet, layer);
        }
        if (check_timer(&BULLET_MOVE_TIME, 50))
        {
            bullet_move(&my_bullet, 10);
            if (check_collapse(&enemy_fighter, &my_bullet))
            {
                printf("shooted\n");
            }
            if (loaddemoflag)
            {
                bullet_move(&enemy_bullet, 10);
                if (check_collapse(&fighter, &enemy_bullet))
                {
                    printf("be shooted\n");
                }
            }
        }
        if (check_timer(&BREAKER_TIMER, 5000))
        {
            HgEventNonBlocking();
        }
        if (check_timer(&REFRESH_TIMER, 100))
        {
            hgp_update(0);
        }
        if (check_timer(&ENEMY_READ_DEMO_TIMER, 10) == 1 && loadflag != 0)
        {
            if (loaddemoflag)
            {
                if (fighter_move_by_pathfile(&cachefighter, NULL))
                {
                    loaddemoflag = 0;
                    fighter_destroy(&cachefighter);
                }
            }
            else
            {
                fighter_init(&cachefighter, layer, 1.5);
                fighter_mirrow(&cachefighter);
                path_list = path_list_head;
                for (int i = 1; i < rand() % loadcounter; i++)
                {
                    path_list = path_list->next;
                }
                fighter_move_by_pathfile(&cachefighter, path_list->path);
                loaddemoflag = 1;
            }
        }
    }
    HgGetChar();
    return 0;
}
