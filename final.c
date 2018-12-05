#include "final.h"

int main()
{
    HGP_WINDOW_INFO *window1 = hgp_window_init(500, 500, -1, -1);
    HGP_LAYER_INFO *layer1 = hgp_add_layer(window1);
    hgevent *event = NULL;
    HgSetEventMask(HG_KEY_EVENT_MASK);
    srand(time(NULL));
    int breakflag = 1;

    //--world-init/
    //create 2-dimention circle linklist
    //array use to create linklist

    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            world_array[j][i] = malloc(sizeof(world_rect));
            world_array[j][i]->this_rect = hgp_rect_init(layer1, j * 10 + 5, i * 10 + 5, 10, 10, HG_WHITE, HG_WHITE, 1, 0, 0);
            world_array[j][i]->this_rect->obj_ptr->change_flag = 1;//dirty zone
            world_array[j][i]->left = NULL;
            world_array[j][i]->right = NULL;
            world_array[j][i]->up = NULL;
            world_array[j][i]->down = NULL;
            world_array[j][i]->snake_node = NULL;
            world_array[j][i]->food = 0;
            if (j > 0)
            {
                world_array[j][i]->left = world_array[j - 1][i];
                world_array[j][i]->left->right = world_array[j][i];
            }
            if (i > 0)
            {
                world_array[j][i]->down = world_array[j][i - 1];
                world_array[j][i]->down->up = world_array[j][i];
            }
        }
        world_array[0][i]->left = world_array[49][i];
        world_array[0][i]->left->right = world_array[0][i];
    }
    for (int i = 0; i < 50; i++)
    {
        world_array[i][49]->up = world_array[i][0];
        world_array[i][49]->up->down = world_array[i][49];
    }
    //check NULLPTR,will cause undetected errors;
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            int flag = 0;
            if (world_array[j][i] == NULL)
                flag = 1;
            if (world_array[j][i]->down == NULL)
                flag = 2;
            if (world_array[j][i]->up == NULL)
                flag = 3;
            if (world_array[j][i]->left == NULL)
                flag = 4;
            if (world_array[j][i]->right == NULL)
                flag = 5;
            if (flag != 0)
            {
                printf("NULLPTR:x=%d,y=%d,type=%d\n", j, i, flag);
            }
        }
    }

    //world-init complete

    //set head and first food generate

    snake_direct = snake_down;
    snake_head_ptr = world_array[rand() % 25 + 25][rand() % 25 + 25];
    snake_head_ptr->snake_node = snake_head_ptr->up; //add a tail
    food_generate();
    int foodflag = 0;

    int timer = 1000 * 250;//maybe fastest
    struct timeval current_time;
    struct timeval prev_time;
    gettimeofday(&prev_time, NULL);
    //main circle

    while (breakflag)
    {
        if (snake_head_ptr == NULL)
        {
            printf("be fucked\n");
            getchar();
            getchar();
        }
        gettimeofday(&current_time, NULL);
        if (current_time.tv_sec * 1000000 + current_time.tv_usec <
            prev_time.tv_sec * 1000000 + prev_time.tv_usec + timer)
        {
            event = HgEventNonBlocking();
            //W A S D control
            if (event != NULL)
            {
                if (event->type == HG_KEY_DOWN)
                {
                    switch (event->ch)
                    {
                    case 'w':
                        if (snake_direct != snake_down)
                            snake_direct = snake_up;
                        break;
                    case 'a':
                        if (snake_direct != snake_right)
                            snake_direct = snake_left;
                        break;
                    case 's':
                        if (snake_direct != snake_up)
                            snake_direct = snake_down;
                        break;
                    case 'd':
                        if (snake_direct != snake_left)
                            snake_direct = snake_right;
                        break;
                    case 0x1b:
                        breakflag = 0;
                        goto JUMPUPDATE;
                    }
                }
            }
        }
        else
        {
            if (snake_move() == NULL)
            {
                printf("You Lose\n");
                breakflag = 0;
                printf("your score is:%d\n", score);
                goto JUMPUPDATE;
            }
            else
            {
                snake_update();
            }
            hgp_update(1);
            //HgSleep(0.2); //TODO:will block,
            prev_time = current_time;
        }
    JUMPUPDATE:
    {
    }
    }

    hgp_quit();
}

void food_generate()
{
    for (int i = 0; i < foodtotal-foodcount; i++)
    {
        do
        {
            food_x = rand() % 50;
            food_y = rand() % 50;
        } while (world_array[food_x][food_y]->snake_node != NULL || world_array[food_x][food_y]->food == 1);
        world_array[food_x][food_y]->food = 1;
        world_array[food_x][food_y]->this_rect->fill_color = HG_RED;
        world_array[food_x][food_y]->this_rect->obj_ptr->change_flag = -1;
    }
    foodcount = foodtotal;
}

void *snake_move()
{
    //move head
    switch (snake_direct)
    {
    case snake_up:
        snake_head_ptr = snake_head_ptr->up;
        if (snake_head_ptr->snake_node != NULL) //collapse with body
            return NULL;
        snake_head_ptr->snake_node = snake_head_ptr->down;
        break;
    case snake_down:
        snake_head_ptr = snake_head_ptr->down;
        if (snake_head_ptr->snake_node != NULL)
            return NULL;
        snake_head_ptr->snake_node = snake_head_ptr->up;
        break;
    case snake_left:
        snake_head_ptr = snake_head_ptr->left;
        if (snake_head_ptr->snake_node != NULL)
            return NULL;
        snake_head_ptr->snake_node = snake_head_ptr->right;
        break;
    case snake_right:
        snake_head_ptr = snake_head_ptr->right;
        if (snake_head_ptr->snake_node != NULL)
            return NULL;
        snake_head_ptr->snake_node = snake_head_ptr->left;
        break;
    }
    //feed
    if (snake_head_ptr->food == 0)
    {
        world_rect *tmp_ptr = snake_head_ptr;
        while (tmp_ptr->snake_node->snake_node != NULL)
        {
            tmp_ptr = tmp_ptr->snake_node;
        }
        tmp_ptr->snake_node = NULL;
    }
    else
    {
        snake_head_ptr->food = 0;
        foodcount--;
        if (foodcount == 5)//low food limit
            food_generate();
    }
    return snake_head_ptr;
}

void snake_update()
{
    //set update flag,fast update
    world_rect *tmp_rect_ptr = snake_head_ptr;
    tmp_rect_ptr->this_rect->fill_color = HG_BLUE;
    tmp_rect_ptr->this_rect->obj_ptr->change_flag = 1;
    tmp_rect_ptr = tmp_rect_ptr->snake_node;
    score = 1;
    while (1)
    {
        if (tmp_rect_ptr->snake_node == NULL)
        {
            break;
        }
        else
        {
            tmp_rect_ptr->this_rect->fill_color = HG_BLACK + 1;
            tmp_rect_ptr->this_rect->obj_ptr->change_flag = 1;
            tmp_rect_ptr = tmp_rect_ptr->snake_node;
            score++;
        }
    }
}