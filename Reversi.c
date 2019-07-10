#include "handyplus.h"

typedef struct Disc
{
    int status; //0:empty;-1:black;1:white;
    int valid;  //0:invalid,1:valid
    HGP_CIRCLE *graphic;
} Disc;

typedef struct ReversiStatus
{
    HGP_WINDOW_INFO *windowInfo;
    HGP_LAYER_INFO *layerInfo;
    Disc discs[8][8];
    int currentPlayer; //-1->black 1->white
    int giveUpFlag;    //1:no disk to operate
} ReversiStatus;
/*
findValid

white->white
return<-0

white->black->black->border
return<-0

white->black->black->empty
empty.valid=true;

return<-1
 */
int findValid(int currentX, int currentY, int upFlag, int rightFlag, ReversiStatus *status, int flag)
{
    if (currentX < 0 || currentX > 7 || currentY < 0 || currentY > 7)
    {
        return 0;
    }
    else if (status->discs[currentX][currentY].status == 0)
    {
        if (flag)
        {
            status->discs[currentX][currentY].valid = 1;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (status->discs[currentX][currentY].status == status->currentPlayer)
            return 0;
        else
            return findValid(currentX + upFlag, currentY + rightFlag, upFlag, rightFlag, status, 1);
    }
}
/*
setChanges

white->border/empty
<-0
white->white
<-1
white ->black->black->white
return<-1 set<-1 set<-1
white ->black->black->border
return<-0    <-0    <-0
 */
int setChanges(int currentX, int currentY, int upFlag, int rightFlag, ReversiStatus *status)
{
    if (currentX < 0 || currentX > 7 || currentY < 0 || currentY > 7 || status->discs[currentX][currentY].status == 0)
    {
        return 0;
    }
    else if (status->discs[currentX][currentY].status == status->currentPlayer)
    {
        return 1;
    }
    else
    {
        if (setChanges(currentX + upFlag, currentY + rightFlag, upFlag, rightFlag, status))
        {
            status->discs[currentX][currentY].status = status->currentPlayer;
            return 1;
        }
        else
        {
            return 0;
        }
    }
}

ReversiStatus *reversiInit()
{
    ReversiStatus *tmp = malloc(sizeof(ReversiStatus));
    tmp->windowInfo = hgp_window_init(800, 1000, -1, -1);
    tmp->layerInfo = hgp_add_layer(tmp->windowInfo);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            tmp->discs[i][j].graphic = hgp_circle_init(tmp->layerInfo, i * 100 + 50, j * 100 + 50, 40, HG_BLACK, HG_WHITE, 1, 1);
            tmp->discs[i][j].graphic->obj_ptr->change_flag = 0;
            tmp->discs[i][j].status = 0;
            tmp->discs[i][j].valid = 0;
            hgp_line_init(tmp->layerInfo, (i + 1) * 100, 0, (i + 1) * 100, 800, HG_BLACK, 1)->obj_ptr->change_flag = -1;
            hgp_line_init(tmp->layerInfo, 0, (j + 1) * 100, 800, (j + 1) * 100, HG_BLACK, 1)->obj_ptr->change_flag = -1;
        }
    }
    hgp_update(1);
    tmp->currentPlayer = -1;
    tmp->discs[3][3].status = 1;
    tmp->discs[3][4].status = -1;
    tmp->discs[4][3].status = -1;
    tmp->discs[4][4].status = 1;
    return tmp;
}

void colorSync(ReversiStatus *status)//set color
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (status->discs[i][j].status == 1)
            {
                status->discs[i][j].graphic->fill_color = HG_WHITE;
                status->discs[i][j].graphic->obj_ptr->change_flag = -1;
            }
            else if (status->discs[i][j].status == -1)
            {
                status->discs[i][j].graphic->fill_color = HG_BLACK;
                status->discs[i][j].graphic->obj_ptr->change_flag = -1;
            }
            else if (status->discs[i][j].valid == 1)
            {
                status->discs[i][j].graphic->obj_ptr->change_flag = -1;
                if (status->currentPlayer == 1)
                    status->discs[i][j].graphic->fill_color = HG_GREEN;
                else
                    status->discs[i][j].graphic->fill_color = HG_DGREEN;
            }
            else
            {
                status->discs[i][j].graphic->obj_ptr->change_flag = 0;
            }
        }
    }
}

int checkClickPosition(double x, double y, ReversiStatus *status)
{
    int discX = x / 100;
    int discY = y / 100;
    if (discX > 7 || discY > 7 || discX < 0 || discY < 0)//out of border
    {
        return 0;
    }
    else if (status->discs[discX][discY].status != 0 || status->discs[discX][discY].valid == 0)//position already have disc or not valid for operation
    {
        return 0;
    }
    else
    {
        status->discs[discX][discY].status = status->currentPlayer;
        for (int i = -1; i < 2; i++)//set in 8 direction
        {
            for (int j = -1; j < 2; j++)
            {
                if (i != 0 || j != 0)
                    setChanges(discX + i, discY + j, i, j, status);
            }
        }
        return 1;
    }
}

int markValid(ReversiStatus *status)
{
    for (int i = 0; i < 8; i++)//clean valid sign
    {
        for (int j = 0; j < 8; j++)
        {
            status->discs[i][j].valid = 0;
        }
    }
    int temp = 0;
    int flag = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (status->discs[i][j].status == status->currentPlayer)
            {
                for (int temp1 = -1; temp1 < 2; temp1++)//check in 8 direction
                {
                    for (int temp2 = -1; temp2 < 2; temp2++)
                    {
                        if (temp1 != 0 || temp2 != 0)
                            flag += findValid(i + temp1, j + temp2, temp1, temp2, status, 0);//check current player's valid disk, if 0 return 0,current player give up.
                    }
                }
                temp++;//check current player's disk num,if 0 return -1,current player lose
            }
        }
    }
    if (temp == 0)
        return -1; //no disc for current player
    else if (flag == 0)
        return 0; //have disc but no valid
    else
        return 1; //have disc and have valid position to put
}

int main(int argc, char const *argv[])
{
    ReversiStatus *status = reversiInit();
    hgevent *event;
    HgSetEventMask(HG_MOUSE_DOWN | HG_MOUSE_UP);
    markValid(status);
    colorSync(status);
    hgp_update(1);
    while (1)
    {
        event = HgEventNonBlocking();
        if (event != NULL)
        {
            if (checkClickPosition(event->x, event->y, status))
            {
                int loopCounter=2;
            markStart:
                if(loopCounter<1){//TODO:add winner info
                    return 0;//TimeToLive, reach 0 to jumpOut the infinity loop: 1 -> -1 -> 1 -> -1
                }
                status->currentPlayer = 0 - status->currentPlayer;
                switch (markValid(status))
                {
                case -1:
                    printf("winner code :%d\n", 0 - status->currentPlayer);//TODO:add winner info
                    return 0;
                    break;
                case 0:
                    printf("%d have no valid disk, switch back\n", status->currentPlayer);
                    loopCounter--;
                    goto markStart;
                    break;
                default:
                    break;
                }
                colorSync(status);
                hgp_update(1);
            }
        }
    }
}
