#include <stdio.h>

int primecal(int num)
{
    for (int i = 2; i < num; i++)
    {
        if (num % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main()
{
    int num;
    scanf("%d", &num);
    if (primecal(num))
    {
        printf("prime!\n");
    }
    else
    {
        printf("not prime~\n");
    }
    //return 0;
}
