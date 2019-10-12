//WindowControl's DataStructure
#include "./HGP_Private.h"
HGP_Window *HGP_Entry;

typedef struct HGP_Window
{
    void *PrivateProps;
    //props
    double x;
    double y;
    double width;
    double height;
    char *title;
    //method
    HGP_Window_Methods *Methods;
} HGP_Window;

typedef struct HGP_Window_Methods
{
    int (*Sync)(HGP_Window *Window); //Sync current
    HGP_Layer *(*CreateLayer)(HGP_Window *Window);
    HGP_Layer *(*FindLayer)(HGP_Window *Window, int index);
    int (*ChangeTitle)(HGP_Window *Window, char *title);
    int (*DeleteLayer)(HGP_Window *Window, int index);
} HGP_Window_Methods;

typedef struct HGP_Layer
{
    void *PrivateProps;
    //props
    int mode;
    int isVisible;
    //method
    int (*SwitchMode)();
    int (*Sync)();
    int (*MoveToLayer)(HGP_Layer *TargetLayer);
    int (*SetHide)(int status);
} HGP_Layer;
