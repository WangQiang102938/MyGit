#include <stdlib.h>
#include <stdio.h>
#include <handy.h>
//Tools
//GetNodeLocation
void *HGP_Entry;
void *Window_Methods;

enum NodeLocationState
{
    Node_Normal,
    Node_Head,
    Node_Tail,
    Node_Only,
    Node_Null
};

void *checkInit1;
void *checkInit2;
int CheckNotInit();

//Window
int Window_Sync(void *Window); //Sync current
HGP_Layer *Window_CreateLayer(void *Window);
HGP_Layer *Window_FindLayer(void *Window, int index);
int Window_DeleteLayer(void *Window,int index);
int Window_ChangeTitle(void *Window, char *title);
