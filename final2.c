#include "final2.h"

int main()
{
    HGP_LAYER_INFO *layer = hgp_add_layer(hgp_window_init(boarder_x, boarder_x, -1, -1));
    int score = 0;

    BULLET my_bullet;
    my_bullet.direct = BULLET_DIRECT_UP;
    my_bullet.color = HG_GREEN;
    my_bullet.head = NULL;
    BULLET enemy_bullet;
    enemy_bullet.color = HG_RED;
    enemy_bullet.direct = BULLET_DIRECT_DOWN;
    enemy_bullet.head = NULL;

    fighter_list_head = malloc(sizeof(FIGHTER_LIST));
    fighter_list_head->fighter_ptr = fighter_init(layer, 2, FIGHTER_IDENTITY_ME, &my_bullet, HG_BLUE);
    control_fighter = fighter_list_head->fighter_ptr;
    FIGHTER *enemy_1 = fighter_list_add(fighter_list_head, fighter_init(layer, 1.5, FIGHTER_IDENTITY_ENEMY, &enemy_bullet, HG_RED));
    FIGHTER *enemy_2 = fighter_list_add(fighter_list_head, fighter_init(layer, 1.5, FIGHTER_IDENTITY_ENEMY, &enemy_bullet, HG_RED));

    HgSetEventMask(HG_MOUSE_EVENT_MASK | HG_KEY_EVENT_MASK);
    hgp_update(1);

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
                    fighter_move(control_fighter, event->x, event->y, 1);
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
                    if (event->ch == '3' && (event->type | HG_KEY_DOWN) == HG_KEY_DOWN)
                    {
                        path_clean();
                        FIGHTER_LIST *tmp_list = fighter_list_head;
                        while (tmp_list)
                        {
                            tmp_list->fighter_ptr->path_current_node = NULL;
                            tmp_list->fighter_ptr->path_file = NULL;
                            tmp_list->fighter_ptr->path_head_node = NULL;
                            tmp_list = tmp_list->next;
                        }
                        path_load();
                    }
                    if (event->ch == '4')
                    {
                        FIGHTER_LIST *tmp_list = fighter_list_head;
                        while (tmp_list)
                        {
                            if (tmp_list->fighter_ptr->fighter_identity == FIGHTER_IDENTITY_ENEMY &&
                                pathfile_list_head != NULL)
                            {
                                read_path_list *tmp_path_list = pathfile_list_head;
                                for (int i = 0; i < rand() % path_counter; i++)
                                {
                                    tmp_path_list = tmp_path_list->next;
                                }
                                tmp_list->fighter_ptr->path_file = tmp_path_list;
                            }
                            tmp_list = tmp_list->next;
                        }
                    }
                    if (event->ch == ' ')
                    {
                        hgp_update(0);
                        HgGetChar();
                    }
                }
            }
            if (logflag != 0)
            {
                path_log(loginfo, control_fighter->center_x, control_fighter->center_y);
            }
            fighter_list_del(&fighter_list_head);
        }
        if (check_timer(&BULLET_CREATE_TIME, 500))
        {
            FIGHTER_LIST *tmp_list = fighter_list_head;
            while (tmp_list)
            {
                create_bullet(tmp_list->fighter_ptr);
                tmp_list = tmp_list->next;
            }
        }
        if (check_timer(&BULLET_MOVE_TIME, 50))
        {
            bullet_move(&my_bullet, 10);
            bullet_move(&enemy_bullet, 10);
            FIGHTER_LIST *tmp_list = fighter_list_head;
            while (tmp_list)
            {
                if (tmp_list->fighter_ptr->fighter_identity == FIGHTER_IDENTITY_ENEMY)
                {
                    if (check_collapse(tmp_list->fighter_ptr, &my_bullet))
                    {
                        tmp_list->delete_flag = 1;
                        score++;
                        printf("score:%d\n", score);
                    }
                }
                if (tmp_list->fighter_ptr->fighter_identity == FIGHTER_IDENTITY_ME)
                {
                    if (check_collapse(tmp_list->fighter_ptr, &enemy_bullet))
                    {
                        score -= 10;
                        printf("score:%d\n", score);
                    }
                }
                tmp_list = tmp_list->next;
            }
        }
        if (check_timer(&BREAKER_TIMER, 1000))
        {
            FIGHTER *tmp_fighter =
                fighter_list_add(fighter_list_head, fighter_init(layer, 1.5, FIGHTER_IDENTITY_ENEMY, &enemy_bullet, HG_RED));
            fighter_mirrow(tmp_fighter);
            fighter_move(tmp_fighter, -100, -100, 1);
            if (pathfile_list_head != NULL)
            {
                read_path_list *tmp_path_list = pathfile_list_head;
                for (int i = 0; i < rand() % 5; i++)
                {
                    tmp_path_list = tmp_path_list->next;
                }
                tmp_fighter->path_file = tmp_path_list;
                tmp_fighter->path_head_node=tmp_path_list->path_list_head;
                tmp_fighter->path_current_node=tmp_path_list->path_list_head;
            }
        }
        if (check_timer(&REFRESH_TIMER, 200))
        {
            hgp_update(0);
        }
        if (check_timer(&ENEMY_READ_DEMO_TIMER, 10))
        {
            FIGHTER_LIST *tmp_list = fighter_list_head;
            while (tmp_list)
            {
                fighter_move_by_pathfile(tmp_list);
                tmp_list = tmp_list->next;
            }
        }
    }
    HgGetChar();
    return 0;
}
