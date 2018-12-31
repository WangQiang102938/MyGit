#include "final2.h"

FIGHTER *fighter_init(HGP_LAYER_INFO *layer, double size_rate, int fighter_identity, BULLET *bullet_info, unsigned long color)
{
    FIGHTER *fighter = malloc(sizeof(FIGHTER));
    int counter = 0;
    double x = fighter->center_x;
    double y = fighter->center_y;
    fighter->size_rate = size_rate;
    {
        fighter->left_gun = hgp_rect_init(layer, x - 9 * size_rate, y + 3 * size_rate, 3 * size_rate, 9 * size_rate, HG_BLACK, HG_BLACK, 1, 0, 1);
        fighter->right_gun = hgp_rect_init(layer, x + 9 * size_rate, y + 3 * size_rate, 3 * size_rate, 9 * size_rate, HG_BLACK, HG_BLACK, 1, 0, 1);

        fighter->shell = hgp_polygon_init(layer, 1, HG_BLACK, 1, fighter->color, 1);

        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 0 * size_rate, y + 27 * size_rate);
        counter++;

        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 3 * size_rate, y + 12 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 3 * size_rate, y + 7 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 15 * size_rate, y - 1 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 15 * size_rate, y - 5 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 5 * size_rate, y - 3 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 9 * size_rate, y - 7 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 9 * size_rate, y - 11 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 5 * size_rate, y - 10 * size_rate);
        counter++;

        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 3 * size_rate, y - 8 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 3 * size_rate, y - 8 * size_rate);
        counter++;

        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 5 * size_rate, y - 10 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 9 * size_rate, y - 11 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 9 * size_rate, y - 7 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 5 * size_rate, y - 3 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 15 * size_rate, y - 5 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 15 * size_rate, y - 1 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 3 * size_rate, y + 7 * size_rate);
        counter++;
        hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 3 * size_rate, y + 12 * size_rate);
        counter++;
    }
    fighter->layer_info = layer;
    fighter->fighter_identity = fighter_identity;
    fighter->path_current_node = NULL;
    fighter->path_head_node = NULL;
    fighter->path_file = NULL;
    fighter->bullet_info = bullet_info;
    fighter->color = color;
    fighter->shell->fill_color = color;
    return fighter;
}

int fighter_destroy(FIGHTER **fighter_ptr)
{
    FIGHTER *fighter = *fighter_ptr;
    hgp_delete_object(fighter->shell->obj_ptr);
    hgp_delete_object(fighter->left_gun->obj_ptr);
    hgp_delete_object(fighter->right_gun->obj_ptr);
    fighter->shell = NULL;
    fighter->left_gun = NULL;
    fighter->right_gun = NULL;
    *fighter_ptr = NULL;
    return 1;
}

FIGHTER *fighter_list_add(FIGHTER_LIST *fighter_list, FIGHTER *fighter)
{
    FIGHTER_LIST *tmp_list = malloc(sizeof(FIGHTER_LIST));
    tmp_list->next = fighter_list->next;
    fighter_list->next = tmp_list;
    tmp_list->fighter_ptr = fighter;
    tmp_list->delete_flag = 0;
    tmp_list->next = NULL;
    return fighter;
}

int fighter_list_del(FIGHTER_LIST **fighter_list_head_ptr) //delete flag setted node
{
    FIGHTER_LIST *fighter_list_head = *fighter_list_head_ptr;
    FIGHTER_LIST *tmp_list = fighter_list_head;
    FIGHTER_LIST *tmp_list_prev = fighter_list_head;
    while (tmp_list)
    {
        if (tmp_list->delete_flag)
        {
            if (tmp_list == *fighter_list_head_ptr)
            {
                *fighter_list_head_ptr = tmp_list->next;            //change head by pointer
                fighter_destroy(&(fighter_list_head->fighter_ptr)); //
                free(fighter_list_head);                            //free old head
                fighter_list_head = *fighter_list_head_ptr;         //reset tmp head
                tmp_list = fighter_list_head;                       //reset tmp_list
                goto CIRCLE_END;
            }
            else
            {
                tmp_list_prev->next = tmp_list->next; //cut tmp node from list
                fighter_destroy(&(tmp_list->fighter_ptr));
                free(tmp_list);
            }
        }
        tmp_list_prev = tmp_list;
        tmp_list = tmp_list->next;
    CIRCLE_END:;
    }
    return 0;
}

int fighter_mirrow(FIGHTER *fighter)
{
    HGP_POLYGON_NODE *tmp_node = fighter->shell->head;
    while (tmp_node)
    {
        tmp_node->x = tmp_node->x - (tmp_node->x - fighter->center_x) * 2;
        tmp_node->y = tmp_node->y - (tmp_node->y - fighter->center_y) * 2;
        tmp_node = tmp_node->next_node;
    }
    fighter->left_gun->x = fighter->left_gun->x - (fighter->left_gun->x - fighter->center_x) * 2;
    fighter->left_gun->y = fighter->left_gun->y - (fighter->left_gun->y - fighter->center_y) * 2;
    fighter->right_gun->x = fighter->right_gun->x - (fighter->right_gun->x - fighter->center_x) * 2;
    fighter->right_gun->y = fighter->right_gun->y - (fighter->right_gun->y - fighter->center_y) * 2;
    return 0;
}

int fighter_move(FIGHTER *fighter, double x, double y, int absolute_flag)
{
    double move_x;
    double move_y;
    if (x == 0 && y == 0)
        return 0;
    if (absolute_flag)
    {
        move_x = x - fighter->center_x;
        move_y = y - fighter->center_y;
    }
    else
    {
        move_x = x;
        move_y = y;
    }
    fighter->center_x += move_x;
    fighter->center_y += move_y;
    HGP_POLYGON_NODE *tmp_node = fighter->shell->head;
    while (tmp_node)
    {
        // printf("\nprev_x=%lf,prev_y=%lf\n", tmp_node->x, tmp_node->y);
        tmp_node->x = tmp_node->x + move_x;
        tmp_node->y = tmp_node->y + move_y; //unknow error
        // printf("now__x=%lf,now__y=%lf\n\n\n", tmp_node->x, tmp_node->y);
        tmp_node = tmp_node->next_node;
        //printf("move\n");
    }
    fighter->left_gun->x += move_x;
    fighter->left_gun->y += move_y;
    fighter->right_gun->x += move_x;
    fighter->right_gun->y += move_y;
    //printf("%lf,%lf\n", move_x, move_y);
    return 0;
}

int create_bullet(FIGHTER *fighter)
{
    double left_x = fighter->left_gun->x;
    double left_y = fighter->left_gun->y;
    double r = fighter->left_gun->width / 2;
    double right_x = fighter->right_gun->x;
    double right_y = fighter->right_gun->y;
    BULLET *bullet_list = fighter->bullet_info;

    BULLET_NODE *tmp_node = malloc(sizeof(BULLET_NODE));
    tmp_node->next = bullet_list->head;
    bullet_list->head = tmp_node;
    tmp_node->obj_ptr = hgp_circle_init(fighter->layer_info, left_x, left_y, r, HG_WHITE, bullet_list->color, 1, 0);
    tmp_node = malloc(sizeof(BULLET_NODE));
    tmp_node->next = bullet_list->head;
    bullet_list->head = tmp_node;
    tmp_node->obj_ptr = hgp_circle_init(fighter->layer_info, right_x, right_y, r, HG_WHITE, bullet_list->color, 1, 0);
    return 1;
}

int bullet_move(BULLET *bullet_list, double move_distance)
{
    if (bullet_list == NULL)
        return 0;
    if (bullet_list->head == NULL)
        return 0;
    BULLET_NODE *tmp_node = bullet_list->head;
    BULLET_NODE *tmp_prev_node = NULL;
    double move_x = 0;
    double move_y = 0;
    switch (bullet_list->direct)
    {
    case BULLET_DIRECT_UP:
        move_y = move_distance;
        break;
    case BULLET_DIRECT_DOWN:
        move_y = -move_distance;
        break;
    case BULLET_DIRECT_LEFT:
        move_x = -move_distance;
        break;
    case BULLET_DIRECT_RIGHT:
        move_x = move_distance;
        break;
    default:
        return 0;
    }
    while (tmp_node)
    {
        tmp_node->obj_ptr->x += move_x;
        tmp_node->obj_ptr->y += move_y;
        if (tmp_node->obj_ptr->x < 0 ||
            tmp_node->obj_ptr->y < 0 ||
            tmp_node->obj_ptr->x > boarder_x ||
            tmp_node->obj_ptr->y > boarder_y)
        {
            if (tmp_prev_node != NULL)
            {
                BULLET_NODE *cachenode = tmp_node;
                tmp_prev_node->next = tmp_node->next; //cut this node from list
                tmp_node = tmp_node->next;            //go to next node
                hgp_delete_object(cachenode->obj_ptr->obj_ptr);
                free(cachenode);
                goto CIRCLE_END;
            }
        }
        tmp_prev_node = tmp_node;
        tmp_node = tmp_node->next;
    CIRCLE_END:;
    }
    return 0;
}

int check_timer(struct timeval *checked_time, int timer_ms)
{
    struct timeval current;
    gettimeofday(&current, NULL);
    if (current.tv_sec * 1000000 + current.tv_usec >
        checked_time->tv_sec * 1000000 + checked_time->tv_usec + 1000 * timer_ms)
    {
        *checked_time = current;
        return 1;
    }
    else
    {
        return 0;
    }
}

int check_collapse(FIGHTER *fighter, BULLET *bullet_list)
{
    BULLET_NODE *tmp_node = bullet_list->head;
    while (tmp_node)
    {
        if (pow(tmp_node->obj_ptr->x - fighter->center_x, 2) + pow(tmp_node->obj_ptr->y - fighter->center_y, 2) < (pow(13 * fighter->size_rate + tmp_node->obj_ptr->r, 2)))
        {
            tmp_node->obj_ptr->x = -1; //move to out of boarder,next check will remove from list
            return 1;
        }
        tmp_node = tmp_node->next;
    }
    return 0;
}

int fighter_move_by_pathfile(FIGHTER_LIST *fighter_list)
{
    FIGHTER *fighter = fighter_list->fighter_ptr;
    if (fighter->path_file == NULL || fighter->path_file->path_list_head == NULL)
    {
        return 0;
    }
    if (fighter->path_head_node == NULL)
    {
        fighter->path_head_node = fighter->path_file->path_list_head;
        fighter->path_current_node = fighter->path_head_node;
    }
    else if (fighter->path_current_node != NULL)
    {
        fighter_move(fighter, fighter->path_current_node->x, fighter->path_current_node->y, 1);
        fighter->path_current_node = fighter->path_current_node->next;
    }
    else
    {
        fighter_list->delete_flag = 1;
    }
    return 0;
}

log_prop *path_log_init(char *pathname)
{
    log_prop *loginfo = malloc(sizeof(log_prop));
    loginfo->pathname = malloc(sizeof(char) * strlen(pathname));
    strcpy(loginfo->pathname, pathname);
    loginfo->log_file_path_ptr = fopen(pathname, "w+");
    loginfo->head = malloc(sizeof(fighter_path_list));
    loginfo->current_node = loginfo->head;
    loginfo->node_counter = 0;
    return loginfo;
}

int path_log(log_prop *loginfo, double x, double y)
{
    if (x != 0 && y != 0)
    {
        loginfo->current_node->next = malloc(sizeof(fighter_path_list));
        loginfo->current_node = loginfo->current_node->next;
        loginfo->current_node->x = x;
        loginfo->current_node->y = y;
        loginfo->current_node->next = NULL;
        printf("log:%d\n", loginfo->node_counter);
        loginfo->node_counter++;
    }
    return 0;
}

int path_log_write(log_prop **loginfo_ptr)
{
    log_prop *loginfo = *loginfo_ptr;
    fighter_path_list *tmp_node = loginfo->head;
    fighter_path_list *free_node = NULL;
    while (tmp_node)
    {
        fwrite(tmp_node, sizeof(fighter_path_list), 1, loginfo->log_file_path_ptr);
        free_node = tmp_node;
        tmp_node = tmp_node->next;
        free(free_node);
    }
    fclose(loginfo->log_file_path_ptr);
    free(loginfo->pathname);
    free(loginfo);
    *loginfo_ptr = NULL;
    return 1;
}

int path_load() //set global path(final2.h)
{
    int load_counter = 1;
    path_counter = 0;
    char path_name[128] = {0};
    pathfile_list_head = malloc(sizeof(read_path_list));
    read_path_list *tmp_pathfile_list = pathfile_list_head;
    FILE *tmp_file = NULL;
    while (1)
    {
        sprintf(path_name, "path%d.dat", load_counter);
        tmp_file = fopen(path_name, "r");
        if (tmp_file)
        {
            path_counter++;
            if (load_counter != 1)
            {
                tmp_pathfile_list->next = malloc(sizeof(read_path_list));
                tmp_pathfile_list = tmp_pathfile_list->next;
                tmp_pathfile_list->next = NULL;
            }
            tmp_pathfile_list->path = tmp_file;
            printf("%s loaded\n\n", path_name);
            load_counter++;
            fighter_path_list *tmp_list = malloc(sizeof(fighter_path_list));
            fighter_path_list *tmp_list_prev = tmp_list;
            tmp_list->next = NULL;
            tmp_pathfile_list->path_list_head = tmp_list;
            while (fread(tmp_list, sizeof(fighter_path_list), 1, tmp_file))
            {
                if (tmp_list->x > -(FIGHTER_SIZE_RATE_MAX * 30) && tmp_list->x < boarder_x + FIGHTER_SIZE_RATE_MAX * 30 &&
                    tmp_list->y > -(FIGHTER_SIZE_RATE_MAX * 30) && tmp_list->y < boarder_y + FIGHTER_SIZE_RATE_MAX * 30)
                {
                    printf("x:%lf,y:%lf\n",tmp_list->x,tmp_list->y);
                    tmp_list_prev->next = tmp_list; //set prev node next
                    tmp_list_prev = tmp_list;       //change prev node to current
                    if (tmp_list->next == NULL)     //path file end check
                    {
                        break;
                    }
                    else
                    {
                        tmp_list->next = malloc(sizeof(fighter_path_list));
                        tmp_list = tmp_list->next;
                    }
                }
            }
            fseek(tmp_file, 0L, SEEK_SET);
        }
        else
        {
            break;
        }
    }
    return load_counter;
}

int path_clean()
{
    if (pathfile_list_head)
    {
        read_path_list *tmp_pathfile = pathfile_list_head;
        read_path_list *tmp_pathfile2 = pathfile_list_head;
        fighter_path_list *tmp_path;
        fighter_path_list *tmp_path2;
        while (tmp_pathfile)
        {
            tmp_path = tmp_pathfile->path_list_head;
            while (tmp_path)
            {
                tmp_path2 = tmp_path;
                tmp_path = tmp_path->next;
                free(tmp_path2);
            }
            tmp_pathfile2 = tmp_pathfile;
            tmp_pathfile = tmp_pathfile->next;
            free(tmp_pathfile2);
        }
        pathfile_list_head = NULL;
        return 1;
    }
    else
    {
        return 0;
    }
}
