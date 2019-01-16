#include "final3.h"

void *List_Delete(LIST **target_node_ptr)
{
    LIST *target_node = *target_node_ptr;
    if (target_node == NULL)
    {
        return NULL;
    }
    void *return_ptr = target_node->ptr;
    if (target_node->prev_node == NULL)
    {
        *target_node_ptr = target_node->next_node;
        free(target_node);
    }
    else if (target_node->next_node == NULL)
    {
        target_node->prev_node->next_node = NULL;
        free(target_node);
    }
    else
    {
        target_node->prev_node->next_node = target_node->next_node;
        target_node->next_node->prev_node = target_node->prev_node;
        free(target_node);
    }
    return return_ptr;
}
#define List_Delete(x) List_Delete(&(x))

size_t Get_Size_By_ID(int ID)
{
    switch (ID)
    {
    case LIST_TYPE_FIGHTER:
        return sizeof(FIGHTER);
    case LIST_TYPE_BULLET:
        return sizeof(BULLET);
    case LIST_TYPE_LOG:
        return sizeof(PATH);
    case LIST_TYPE_PATHFILE:
        return sizeof(PATHFILE);
    case LIST_TYPE_PATH:
        return sizeof(PATH);
    default:
        return 0;
    }
}

LIST *List_Add(int type, int location, size_t mem_size, LIST **head_ptr, LIST *target_node)
{
    LIST *tmp_node = ((LIST *)malloc(sizeof(LIST)));
    tmp_node->type = type;
    LIST *head = *head_ptr;
START:
    switch (location)
    {
    case LIST_INSERT_HEAD:
    {
        tmp_node->prev_node = NULL;
        tmp_node->next_node = head;
        *head_ptr = tmp_node;
        break;
    }
    case LIST_INSERT_FRONT:
    {
        if (target_node->prev_node == NULL)
        {
            location = LIST_INSERT_HEAD;
            goto START;
        }
        target_node->prev_node->next_node = tmp_node;
        tmp_node->prev_node = target_node->prev_node;
        target_node->prev_node = tmp_node;
        tmp_node->next_node = target_node;
        break;
    }
    case LIST_INSERT_BACK:
    {
        if (target_node->next_node)
        {
            target_node->next_node->prev_node = tmp_node;
            tmp_node->next_node = target_node->next_node;
        }
        else
        {
            tmp_node->next_node = NULL;
        }
        target_node->next_node = tmp_node;
        tmp_node->prev_node = target_node;
        break;
    }
    case LIST_INSERT_TAIL:
    {
        LIST *tmp_node2 = *head_ptr;
        while (tmp_node2->next_node)
        {
            tmp_node2 = tmp_node2->next_node;
        }
        target_node = tmp_node2;
        location = LIST_INSERT_BACK;
        goto START;
    }
    default:
    {
        free(tmp_node);
        return NULL;
    }
    }
    if (mem_size)
        tmp_node->ptr = malloc(mem_size);
    else
        tmp_node->ptr = malloc(Get_Size_By_ID(type));
    return tmp_node;
}

FIGHTER *fighter_init(HGP_LAYER_INFO *layer, double size_rate, int fighter_identity, LIST *bullet_info, unsigned long color)
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
    fighter->path_current = NULL;
    fighter->path_head = NULL;
    fighter->path_file = NULL;
    fighter->bullet = bullet_info;
    fighter->color = color;
    fighter->shell->fill_color = color;
    return fighter;
}

int fighter_destroy(FIGHTER *fighter)
{
    if (fighter == NULL)
        return 0;
    hgp_delete_object(fighter->shell->obj_ptr);
    hgp_delete_object(fighter->left_gun->obj_ptr);
    hgp_delete_object(fighter->right_gun->obj_ptr);
    free(List_Delete(fighter->list_info));
    return 1;
}

FIGHTER *fighter_add(LIST *fighter_list_head, FIGHTER *fighter)
{
    if (fighter == NULL)
        return 0;
    LIST *tmp_list = List_Add(LIST_TYPE_FIGHTER, LIST_INSERT_HEAD, Get_Size_By_ID(LIST_TYPE_FIGHTER), &fighter_list, NULL);
    tmp_list->ptr = fighter;
    return fighter;
}

int fighter_mirrow(FIGHTER *fighter)
{
    if (fighter == NULL)
        return 0;
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
    if (fighter == NULL)
        return 0;
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
    if (fighter == NULL)
        return 0;
    double left_x = fighter->left_gun->x;
    double left_y = fighter->left_gun->y;
    double r = fighter->left_gun->width / 2;
    double right_x = fighter->right_gun->x;
    double right_y = fighter->right_gun->y;
    LIST * tmp_list=List_Add(LIST_TYPE_BULLET, LIST_INSERT_HEAD, Get_Size_By_ID(LIST_TYPE_BULLET), &bullet_list, NULL);
    BULLET *tmp_list_ptr = tmp_list->ptr;
    tmp_list_ptr->bullet_obj_ptr = hgp_circle_init(fighter->layer_info, left_x, left_y, r, HG_WHITE, Get_BulletColor_By_FighterID(fighter), 1, 1);
    tmp_list_ptr->fighter=fighter;
    tmp_list_ptr->list_info=tmp_list;
    tmp_list=List_Add(LIST_TYPE_BULLET, LIST_INSERT_HEAD, Get_Size_By_ID(LIST_TYPE_BULLET), &bullet_list, NULL);
    tmp_list_ptr = tmp_list->ptr;
    tmp_list_ptr->bullet_obj_ptr = hgp_circle_init(fighter->layer_info, right_x, right_y, r, HG_WHITE, Get_BulletColor_By_FighterID(fighter), 1, 1);
    tmp_list_ptr->fighter=fighter;
    tmp_list_ptr->list_info=tmp_list;
    return 1;
}

unsigned long Get_BulletColor_By_FighterID(FIGHTER *fighter)
{
    if (fighter == NULL)
        return 0;
    if (fighter->fighter_identity == FIGHTER_IDENTITY_ME)
    {
        return HG_GREEN;
    }
    else if (fighter->fighter_identity == FIGHTER_IDENTITY_ENEMY)
    {
        return HG_RED;
    }
    else
    {
        return 1;
    }
}

int bullet_move(LIST *bullet_list, double move_distance)
{
    if (bullet_list == NULL)
        return 0;
    if (bullet_list == NULL)
        return 0;
    LIST *tmp_node=bullet_list;
    BULLET *tmp_node_ptr = bullet_list->ptr;
    double move_x = 0;
    double move_y = 0;
    while (tmp_node)
    {
        switch (tmp_node_ptr->fighter->fighter_identity)
        {
        case FIGHTER_IDENTITY_ME:
            move_y = move_distance;
            break;
        case FIGHTER_IDENTITY_ENEMY:
            move_y = -move_distance;
            break;
        default:
            return 0;
        }
        tmp_node_ptr->bullet_obj_ptr->x += move_x;
        tmp_node_ptr->bullet_obj_ptr->y += move_y;
        if (tmp_node_ptr->bullet_obj_ptr->x < 0 ||
            tmp_node_ptr->bullet_obj_ptr->y < 0 ||
            tmp_node_ptr->bullet_obj_ptr->x > boarder_x ||
            tmp_node_ptr->bullet_obj_ptr->y > boarder_y)
        {
            hgp_delete_object(tmp_node_ptr->bullet_obj_ptr->obj_ptr);
            tmp_node = tmp_node->next_node;
            free(List_Delete(tmp_node_ptr->list_info));
        }
        else
        {
            tmp_node = tmp_node->next_node;
        }
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

int check_collapse(FIGHTER *fighter, LIST *bullet_list)
{
    if (fighter == NULL || bullet_list == NULL)
        return 0;
    LIST *tmp_node = bullet_list;
    BULLET *tmp_node_ptr = tmp_node->ptr;
    while (tmp_node)
    {
        tmp_node_ptr = tmp_node->ptr;
        if (pow(tmp_node_ptr->bullet_obj_ptr->x - fighter->center_x, 2) +
                    pow(tmp_node_ptr->bullet_obj_ptr->y - fighter->center_y, 2) <
                (pow(13 * fighter->size_rate + tmp_node_ptr->bullet_obj_ptr->r, 2)) &&
            fighter->fighter_identity != tmp_node_ptr->fighter->fighter_identity)
        {
            tmp_node_ptr->bullet_obj_ptr->x = -1; //move to out of boarder,next check will remove from list
            return 1;
        }
        tmp_node = tmp_node->next_node;
    }
    return 0;
}

int fighter_move_by_pathfile(LIST *fighter_list)
{
    if (fighter_list == NULL)
        return 0;
    FIGHTER *fighter = fighter_list->ptr;
    if (fighter->path_file == NULL || fighter->path_file->path_list_head == NULL)
    {
        return 0;
    }
    if (fighter->path_head == NULL)
    {
        fighter->path_head = fighter->path_current = fighter->path_file->path_list_head->ptr;
    }
    else if (fighter->path_current != NULL)
    {
        fighter_move(fighter, fighter->path_current->x, fighter->path_current->y, 1);
        fighter->path_current = fighter->path_current->list_info->next_node->ptr;
    }
    else
    {
        fighter->path_head = NULL; //reset path func
        return 1;                  //delete flag
    }
    return 0;
}

PATH_LOG_INFO *path_log_init(char *pathname)
{
    if (pathname == NULL)
        return NULL;
    PATH_LOG_INFO *tmp_loginfo = malloc(sizeof(PATH_LOG_INFO));
    tmp_loginfo->pathname = malloc(sizeof(char) * strlen(pathname));
    strcpy(tmp_loginfo->pathname, pathname);
    tmp_loginfo->pathfile_ptr = fopen(pathname, "w+");
    tmp_loginfo->path_current = tmp_loginfo->path_head = NULL;
    tmp_loginfo->node_counter = 0;
    return tmp_loginfo;
}

int path_log(PATH_LOG_INFO *loginfo, double x, double y)
{
    if (loginfo == NULL)
        return 0;
    if (x != 0 && y != 0)
    {
        loginfo->path_current = List_Add(LIST_TYPE_LOG, LIST_INSERT_HEAD, Get_Size_By_ID(LIST_TYPE_LOG), &(loginfo->path_head), loginfo->path_current); //need list reverse
        PATH *tmp_path = loginfo->path_current->ptr;
        tmp_path->x = x;
        tmp_path->y = y;
        loginfo->node_counter++;
    }
    return 0;
}

int path_log_write(PATH_LOG_INFO *loginfo)
{
    if (loginfo == NULL)
        return 0;
    LIST *tmp_list = loginfo->path_head;
    //reverse list
    while (tmp_list)
    {
        void *tmp_addr = tmp_list->next_node;
        tmp_list->next_node = tmp_list->prev_node;
        tmp_list->prev_node = tmp_addr;
        loginfo->path_head = tmp_list; //reset head
        tmp_list = tmp_list->prev_node;
    }
    PATH *tmp_list_ptr = tmp_list->ptr;
    while (tmp_list)
    {
        tmp_list_ptr = tmp_list->ptr;
        fwrite(tmp_list_ptr, sizeof(PATH), 1, loginfo->pathfile_ptr);
        tmp_list = tmp_list->next_node;
        free(List_Delete(tmp_list_ptr->list_info));
        tmp_list = tmp_list->next_node;
    }
    fclose(loginfo->pathfile_ptr);
    free(loginfo);
    return 1; //set loginfo to NULL;
}

int path_load()
{
    path_clean();
    //add pathclean
    int load_counter = 1;
    load_path_counter = 0;
    char path_name[128] = {0};
    FILE *tmp_file = NULL;
    LIST *tmp_list = NULL;
    PATHFILE *tmp_list_ptr = NULL;
    while (1)
    {
        sprintf(path_name, "path%d.dat", load_counter);
        tmp_file = fopen(path_name, "r");
        if (tmp_file)
        {
            load_path_counter++;
            tmp_list = List_Add(LIST_TYPE_PATHFILE, LIST_INSERT_HEAD, Get_Size_By_ID(LIST_TYPE_PATHFILE), &pathfile_list, NULL);
            tmp_list_ptr = tmp_list->ptr;
            tmp_list_ptr->path_file = tmp_file;
            printf("%s loaded\n\n", path_name);
            load_counter++;
            LIST *tmp_list2 = NULL;
            LIST *tmp_list2_head = NULL;
            PATH *tmp_list2_ptr = NULL;
            PATH *tmp_path = NULL;
            while (fread(tmp_path, sizeof(PATH), 1, tmp_file))
            {
                if (tmp_path->x > -(FIGHTER_SIZE_RATE_MAX * 30) && tmp_path->x < boarder_x + FIGHTER_SIZE_RATE_MAX * 30 &&
                    tmp_path->y > -(FIGHTER_SIZE_RATE_MAX * 30) && tmp_path->y < boarder_y + FIGHTER_SIZE_RATE_MAX * 30)
                {
                    tmp_list2 = List_Add(LIST_TYPE_PATH, LIST_INSERT_HEAD, Get_Size_By_ID(LIST_TYPE_PATH), &tmp_list2_head, NULL);
                    tmp_list2_ptr = tmp_list2->ptr;
                    tmp_list2_ptr->x = tmp_path->x;
                    tmp_list2_ptr->y = tmp_path->y;
                    tmp_list2_ptr->list_info = tmp_list2;
                    if (tmp_list->next_node == NULL)
                    {
                        break;
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
    while (1)
    {
        if (List_Delete(pathfile_list) == NULL)
        {
            break;
        }
    }
    LIST *tmp_list = fighter_list;
    FIGHTER *tmp_list_ptr = tmp_list->ptr;
    while (tmp_list)
    {
        tmp_list_ptr = tmp_list->ptr;
        tmp_list_ptr->path_current = NULL;
        tmp_list_ptr->path_file = NULL;
        tmp_list_ptr->path_head = NULL;
    }
    return 0;
}
