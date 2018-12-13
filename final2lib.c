#include "final2.h"

int fighter_init(FIGHTER_1 *fighter, HGP_LAYER_INFO *layer, double size_rate)
{
    HGP_POLYGON_NODE *node[20];
    int counter = 0;
    double x = fighter->center_x;
    double y = fighter->center_y;

    fighter->left_gun = hgp_rect_init(layer, x - 9 * size_rate, y + 3 * size_rate, 3 * size_rate, 9 * size_rate, HG_BLACK, HG_BLACK, 1, 0, 1);
    fighter->right_gun = hgp_rect_init(layer, x + 9 * size_rate, y + 3 * size_rate, 3 * size_rate, 9 * size_rate, HG_BLACK, HG_BLACK, 1, 0, 1);

    fighter->shell = hgp_polygon_init(layer, 1, HG_BLACK, 1, HG_BLUE, 1);

    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 0 * size_rate, y + 27 * size_rate);
    counter++;

    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 3 * size_rate, y + 12 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 3 * size_rate, y + 7 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 15 * size_rate, y - 1 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 15 * size_rate, y - 5 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 5 * size_rate, y - 3 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 9 * size_rate, y - 7 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 9 * size_rate, y - 11 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 5 * size_rate, y - 10 * size_rate);
    counter++;

    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x + 3 * size_rate, y - 8 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 3 * size_rate, y - 8 * size_rate);
    counter++;

    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 5 * size_rate, y - 10 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 9 * size_rate, y - 11 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 9 * size_rate, y - 7 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 5 * size_rate, y - 3 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 15 * size_rate, y - 5 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 15 * size_rate, y - 1 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 3 * size_rate, y + 7 * size_rate);
    counter++;
    node[counter] = hgp_polygon_add_point(HGP_POLYGON_INSERT_TAIL, NULL, fighter->shell, x - 3 * size_rate, y + 12 * size_rate);
    counter++;
    printf("counter:%d\n", counter);
}

int fighter_mirrow(FIGHTER_1 *fighter)
{
    HGP_POLYGON_NODE *tmp_node = fighter->shell->head;
    while (tmp_node)
    {
        tmp_node->x=tmp_node->x-(tmp_node->x-fighter->center_x)*2;
        tmp_node->y=tmp_node->y-(tmp_node->y-fighter->center_y)*2;
        tmp_node=tmp_node->next_node;
    }
    fighter->left_gun->x=fighter->left_gun->x-(fighter->left_gun->x-fighter->center_x)*2;
    fighter->left_gun->y=fighter->left_gun->y-(fighter->left_gun->y-fighter->center_y)*2;
    fighter->right_gun->x=fighter->right_gun->x-(fighter->right_gun->x-fighter->center_x)*2;
    fighter->right_gun->y=fighter->right_gun->y-(fighter->right_gun->y-fighter->center_y)*2;
}

int fighter_move(FIGHTER_1 *fighter, double x, double y, int absolute_flag)
{
    double move_x;
    double move_y;
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
        tmp_node->x += move_x;
        tmp_node->y += move_y;
        tmp_node = tmp_node->next_node;
        //printf("move\n");
    }
    fighter->left_gun->x += move_x;
    fighter->left_gun->y += move_y;
    fighter->right_gun->x += move_x;
    fighter->right_gun->y += move_y;
    printf("%lf,%lf\n", move_x, move_y);
}