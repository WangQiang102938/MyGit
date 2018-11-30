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
    hgp_fan_init(window, layer, x, y, size, HG_RED, HG_RED, 1, 1, 1.25 * M_PI, 1.5 * M_PI, &container->ptr[counter]);
    counter++;
    hgp_circle_init(window, layer, x + 0.33 * size, y + 0.5 * size, 0.25 * size, HG_WHITE, HG_WHITE, 1, 1, &container->ptr[counter]);
    counter++;
    hgp_line_init(window, layer, x + size, y, x + 3 * size, y, HG_RED, 1, &container->ptr[counter]);
    counter++;
    hgp_line_init(window, layer, x + 1.33 * size, y - 0.5 * size, x + 1.33 * size, y + 0.5 * size, HG_RED, 1, &container->ptr[counter]);
    counter++;
    hgp_line_init(window, layer, x + 1.66 * size, y - 0.5 * size, x + 1.66 * size, y + 0.5 * size, HG_RED, 1, &container->ptr[counter]);
    counter++;
    hgp_line_init(window, layer, x + 2 * size, y - 0.5 * size, x + 2 * size, y + 0.5 * size, HG_RED, 1, &container->ptr[counter]);
    counter++;
    hgp_line_init(window, layer, x + 2.33 * size, y - 0.5 * size, x + 2.33 * size, y + 0.5 * size, HG_RED, 1, &container->ptr[counter]);
    counter++;
    hgp_line_init(window, layer, x + 2.66 * size, y - 0.5 * size, x + 2.66 * size, y + 0.5 * size, HG_RED, 1, &container->ptr[counter]);
    counter++;
    hgp_line_init(window, layer, x + 3 * size, y, x + 4 * size, y + size, HG_RED, 1, &container->ptr[counter]);
    counter++;
    hgp_line_init(window, layer, x + 3 * size, y, x + 4 * size, y - size, HG_RED, 1, &container->ptr[counter]);
    counter++;
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
    return 0;
}

int main()
{
    int wid = hgp_window_init(400, 400);
    fish_container fish1;
    fish_init(wid, 0, 50, 200, 200, &fish1);
    hgp_update(1);
    // HgSleep(5);
    // fish_mirror(&fish1);
    // hgp_update(1);
    HgGetChar();
    hgp_quit();
}