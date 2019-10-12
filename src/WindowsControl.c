/*
HGP WindowControl
Provide Windows Manage Funcs

Public:
    Add Window
    Delete Window
    Move Window
    Hide/Show Window(save data's)
    Get Window's Info

Private:
    Status Code:
        0:Base Window
        1:Added Window
        2:Hide Window(delete but save data)
    LinkedListInfo
    WindowID
    LayerEntry
*/

#include "./HGP_API.h"
#include "./HGP_Structs.h"
#include "./HGP_Private.h"

enum WindowStatus
{
    ShowWindow,
    HideWindow
};

typedef struct HGP_Window_List
{
    HGP_Window_List *Prev;
    HGP_Window_List *Next;
    HGP_Window_PrivateProps *This;
} HGP_Window_List;

int GetWindowListNodeLocation(HGP_Window_List *TargetNode)
{
    if (TargetNode == NULL)
        return Node_Null;
    if (TargetNode->Prev == NULL)
    {
        if (TargetNode->Next == NULL)
            return Node_Only;
        else
            return Node_Head;
    }
    else
    {
        if (TargetNode->Next == NULL)
            return Node_Tail;
        else
            return Node_Normal;
    }
}

typedef struct HGP_Window_PrivateProps
{
    int status;
    HGP_Window_List *List_MyNode;
    HGP_Window *ThisWindow;
    int WindowID;
    //TODO:Add HGP_LAYER_LIST head
} HGP_Window_PrivateProps;

HGP_Window *HGP_CreateWindow(double x, double y, double width, double height, int isCoorValid)
{
    if (CheckNotInit())
    {
        printf("HGP is not Initialed.");
        return NULL;
    }
    //build Window struct
    HGP_Window_List *tmpWindowList = malloc(sizeof(HGP_Window_List));
    HGP_Window_PrivateProps *tmpWindowPrivate = tmpWindowList->This = malloc(sizeof(HGP_Window_PrivateProps));
    HGP_Window *tmpWindow = tmpWindowPrivate->ThisWindow = malloc(sizeof(HGP_Window));
    tmpWindow->PrivateProps = tmpWindowPrivate;
    tmpWindowPrivate->List_MyNode = tmpWindowList;
    //WindowList
    if (HGP_Entry == NULL)
    {
        HGP_Entry = tmpWindowList;
        tmpWindowList->Next = tmpWindowList->Prev = NULL;
    }
    else
    {
        HGP_Window_List *tmpWindowList_FindTail = HGP_Entry;
        while (tmpWindowList_FindTail->Next != NULL)
        {
            tmpWindowList_FindTail = tmpWindowList_FindTail->Next;
        }
        tmpWindowList_FindTail->Next = tmpWindowList;
        tmpWindowList->Next = NULL;
        tmpWindowList->Prev = tmpWindowList_FindTail;
    }
    //WindowPrivate
    if (isCoorValid)
        tmpWindowPrivate->WindowID = HgOpen(width, height);
    else
        tmpWindowPrivate->WindowID = HgWOpen(x, y, width, height);
    tmpWindowPrivate->status = ShowWindow;
    //TODO:Add Layer Method;
    //Window
    tmpWindow->height = height;
    tmpWindow->width = width;
    tmpWindow->x = x;
    tmpWindow->y = y;
    tmpWindow->title = NULL;
    tmpWindow->Methods = Window_Methods;
}

int Window_Sync(void *Window)
{
    HgWGetSize(((HGP_Window_PrivateProps *)((HGP_Window *)Window)->PrivateProps)->WindowID,
               &(((HGP_Window *)Window)->width), &(((HGP_Window *)Window)->height));
}
HGP_Layer *Window_CreateLayer(void *Window);          //TODO
HGP_Layer *Window_FindLayer(void *Window, int index); //TODO
int Window_DeleteLayer(void *Window, int index);      //TODO
int Window_ChangeTitle(void *Window, char *title)
{
    HgWSetTitle(((HGP_Window_PrivateProps *)((HGP_Window *)Window)->PrivateProps)->WindowID, title);
    ((HGP_Window *)Window)->title = title;
}
