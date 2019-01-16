#include "final3.h"

int main()
{
    HGP_LAYER_INFO *layer = hgp_add_layer(hgp_window_init(boarder_x, boarder_x, -1, -1));
    int score = 0;

    control_fighter = ((fighter_add(fighter_list, fighter_init(layer, 2, FIGHTER_IDENTITY_ME, bullet_list, HG_BLUE))));

    HgSetEventMask(HG_MOUSE_EVENT_MASK | HG_KEY_EVENT_MASK);
    hgp_update(1);

    timeval BULLET_MOVE_TIME = {0, 0};
    timeval BULLET_CREATE_TIME = {0, 0};
    timeval BREAKER_TIMER = {0, 0};
    timeval REFRESH_TIMER = {0, 0};
    timeval MOUSE_CHECK_TIMER = {0, 0};
    timeval ENEMY_READ_DEMO_TIMER = {0, 0};
    //public
    int logflag = 0;
    int logpath_counter = 1;
    char logpathname[128] = {0};
    PATH_LOG_INFO *loginfo = malloc(sizeof(PATH_LOG_INFO));
    int read_path_counter = 0;
    int enemy_fighter_generate = 0;
    //
    hgevent *event;
    int breakflag = 1;
    hgp_update(0);
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
                    if (event->ch == '1' && logflag != 1)
                    {
                        logflag = 1;
                        sprintf(logpathname, "path%d.dat", logpath_counter);
                        loginfo = path_log_init(logpathname);
                    }
                    if (event->ch == '2' && logflag != 0)
                    {
                        path_log_write(loginfo);
                        logpath_counter++;
                        logflag = 0;
                    }
                    if (event->ch == '3' && (event->type | HG_KEY_DOWN) == HG_KEY_DOWN)
                    {
                        read_path_counter = path_load();
                    }
                    if (event->ch == '4')
                    {
                        enemy_fighter_generate = 1;
                    }
                    if (event->ch == ' ') //pause
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
        }
        if (check_timer(&BULLET_CREATE_TIME, 500))
        {
            LIST *tmp_list = fighter_list;
            FIGHTER *tmp_list_ptr = tmp_list->ptr;
            while (tmp_list)
            {
                tmp_list_ptr = tmp_list->ptr;
                create_bullet(tmp_list_ptr);
                tmp_list = tmp_list->next_node;
            }
        }
        if (check_timer(&BULLET_MOVE_TIME, 50))
        {
            bullet_move(bullet_list, 10);
            LIST *tmp_list = fighter_list;
            FIGHTER *tmp_list_ptr = tmp_list->ptr;
            while (tmp_list)
            {
                tmp_list_ptr = tmp_list->ptr;
                if (check_collapse(tmp_list_ptr, bullet_list))
                {
                    if (tmp_list_ptr->fighter_identity == FIGHTER_IDENTITY_ENEMY)
                    {
                        score++;
                        printf("score:%d\n", score);
                    }
                    if (tmp_list_ptr->fighter_identity == FIGHTER_IDENTITY_ME)
                    {
                        score -= 10;
                        printf("score:%d\n", score);
                    }
                }
                tmp_list = tmp_list->next_node;
            }
        }
        if (check_timer(&BREAKER_TIMER, 1000) && enemy_fighter_generate)
        {
            FIGHTER *tmp_fighter =
                fighter_add(fighter_list, fighter_init(layer, 1.5, FIGHTER_IDENTITY_ENEMY, bullet_list, HG_RED));
            fighter_mirrow(tmp_fighter);
            fighter_move(tmp_fighter, -100, -100, 1);
            if (pathfile_list != NULL)
            {
                LIST *tmp_list = pathfile_list;
                PATHFILE *tmp_list_ptr = pathfile_list->ptr;
                for (int i = 0; i < rand() % read_path_counter; i++)
                {
                    tmp_list = tmp_list->next_node;
                }
                tmp_list_ptr = tmp_list->ptr;
                tmp_fighter->path_file = tmp_list_ptr;
                tmp_fighter->path_head = tmp_list_ptr->path_list_head->ptr;
                tmp_fighter->path_current = tmp_list_ptr->path_list_head->ptr;
            }
        }
        if (check_timer(&REFRESH_TIMER, 200))
        {
            hgp_update(0);
        }
        if (check_timer(&ENEMY_READ_DEMO_TIMER, 10))
        {
            LIST *tmp_list = fighter_list;
            while (tmp_list)
            {
                fighter_move_by_pathfile(tmp_list);
                tmp_list = tmp_list->next_node;
            }
        }
    }
}