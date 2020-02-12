#include<stdio.h>

void attack()
{
    printf("**********attack**********");
    return;
}
void rest()
{
    printf("**********rest**********\n");
    return;
}
void blood()
{
    printf("**********blood**********\n");
    return;
}

int main()
{
    char orderChar;
    printf("**********GAME BEGIN**********\n");
    while (1)
    {
        orderChar = getchar();
        switch (orderChar)
        {
        case 'A':
            attack();
            break;
        case 'R':
            rest();
            break;
        case 'B':
            blood();
            break;
        case 'Q':
            printf("**********GAME OVER**********\n");
            return 0;
        }
    }

    return 0;
}