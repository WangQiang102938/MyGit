#include "handyplus.h"

typedef struct fish_container
{
    int x;
    int y;
    int size;
    int window;
    int layer;
    HGP_OBJECT *ptr[10];
} fish_container;

int fish_init(int window, int layer, int size, int x, int y, fish_container *container)
{
    int counter = 0;
    hgp_fan_init(window, layer, x, y, size, HG_RED, HG_RED, 1, 1, 1.25 * M_PI, 1.5 * M_PI, &(container->ptr[counter]));
    counter++; //head
    hgp_circle_init(window, layer, x + 0.33 * size, y + 0.5 * size, 0.25 * size, HG_WHITE, HG_WHITE, 1, 1, &(container->ptr[counter]));
    counter++; //eye
    hgp_line_init(window, layer, x + size, y, x + 3 * size, y, HG_RED, 1, &(container->ptr[counter]));
    counter++; //main bone
    hgp_line_init(window, layer, x + 1.33 * size, y - 0.5 * size, x + 1.33 * size, y + 0.5 * size, HG_RED, 1, &(container->ptr[counter]));
    counter++; //bone 1
    hgp_line_init(window, layer, x + 1.66 * size, y - 0.5 * size, x + 1.66 * size, y + 0.5 * size, HG_RED, 1, &(container->ptr[counter]));
    counter++; //bone 2
    hgp_line_init(window, layer, x + 2 * size, y - 0.5 * size, x + 2 * size, y + 0.5 * size, HG_RED, 1, &(container->ptr[counter]));
    counter++; //bone 3
    hgp_line_init(window, layer, x + 2.33 * size, y - 0.5 * size, x + 2.33 * size, y + 0.5 * size, HG_RED, 1, &(container->ptr[counter]));
    counter++; //bone 4
    hgp_line_init(window, layer, x + 2.66 * size, y - 0.5 * size, x + 2.66 * size, y + 0.5 * size, HG_RED, 1, &(container->ptr[counter]));
    counter++; //bone 5
    hgp_line_init(window, layer, x + 3 * size, y, x + 4 * size, y + size, HG_RED, 1, &(container->ptr[counter]));
    counter++; //tail 1
    hgp_line_init(window, layer, x + 3 * size, y, x + 4 * size, y - size, HG_RED, 1, &(container->ptr[counter]));
    counter++; //tail 2
    container->x = x;
    container->y = y;
    container->size = size;
    container->window = window;
    container->layer = layer;
    return 1;
}

int fish_mirror(fish_container *container)
{
    for (int i = 0; i < 10; i++)
    {
        switch (container->ptr[i]->type)
        {
        case HGP_OBJECT_FAN_FLAG:
        {
            HGP_FAN *ptr = (HGP_FAN *)container->ptr[i]->pointer;
            ptr->x = ptr->x - 2 * (ptr->x - container->x);
            ptr->arc_start += M_PI;
            break;
        }
        case HGP_OBJECT_CIRCLE_FLAG:
        {
            HGP_CIRCLE *ptr = (HGP_CIRCLE *)container->ptr[i]->pointer;
            ptr->x = ptr->x - 2 * (ptr->x - container->x);
            break;
        }
        case HGP_OBJECT_LINE_FLAG:
        {
            HGP_LINE *ptr = (HGP_LINE *)container->ptr[i]->pointer;
            ptr->start_x = ptr->start_x - 2 * (ptr->start_x - container->x);
            ptr->end_x = ptr->end_x - 2 * (ptr->end_x - container->x);
            break;
        }
        }
    }
    return 1;
}

int fish_move(fish_container *container, double distance, double direct)
{
    //following code have flicker, by disable HgSleep in main()
    // for (int i = 0; i < 10; i++)
    // {
    //     hgp_erase_object(container->ptr[i]->window_info, HG_WHITE);
    // }
    for (int i = 0; i < 10; i++)
    {
        hgp_object_move(container->ptr[i]->window_info, direct, distance);
    }
    // for (int i = 0; i < 10; i++)
    // {
    //     hgp_single_draw(container->ptr[i]->window_info);
    // }
    container->x = ((HGP_FAN *)(container->ptr[0]->pointer))->x;
    container->y = ((HGP_FAN *)(container->ptr[0]->pointer))->y;
    return 1;
}
int main()
{
    int fishnum = 500;
    int wid = hgp_window_init(400, 400);
    fish_container fish1[fishnum];
    HgSetEventMask(HG_KEY_EVENT_MASK);
    hgevent *event;
    srand(time(NULL));

    double direct = HGP_DIRECT_LEFT;
    for (int i = 0;  i < fishnum; i++)
    {
        fish_init(wid, 0, rand() % 30, rand() % 400, rand() % 400, &fish1[i]);
        if(rand()%2)
        {
            fish_mirror(&fish1[i]);
        }
    }
    printf("testpoint1\n");
    hgp_update(0);
    hgp_update(1);
    while (1)
    {
        event = HgEventNonBlocking();
        if (event != NULL)
        {
            if (event->ch == ' ')
            {
                break;
            }
        }
    }
    hgp_quit();
}