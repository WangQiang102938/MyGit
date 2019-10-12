#include "./HGP_API.h"
#include "./HGP_Structs.h"
#include "./HGP_Private.h"

int HGP_Init()
{
    checkInit1 = malloc(sizeof(int));
    checkInit2 = checkInit1;

    //Methods Init
    Window_Methods = malloc(sizeof(HGP_Window_Methods));
    ((HGP_Window_Methods *)Window_Methods)->ChangeTitle = &Window_ChangeTitle;
    ((HGP_Window_Methods *)Window_Methods)->CreateLayer = &Window_CreateLayer;
    ((HGP_Window_Methods *)Window_Methods)->FindLayer = &Window_FindLayer;
    ((HGP_Window_Methods *)Window_Methods)->DeleteLayer=&Window_DeleteLayer;
}

int CheckNotInit()
{
    if (checkInit1 == NULL || checkInit2 == NULL || checkInit1 != checkInit2)
        return 1;
    return 0;
}

int HGP_Close()
{

    free(checkInit1);
    free(checkInit2);
}