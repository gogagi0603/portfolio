#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SW 5
#define W 6
#define NW 7
#include <stdio.h>

// �̹� ������ �ٿ� ���� stack�� �ʿ��� �޸� ������ �迭�� ���·� �̸� �Ҵ�Ǿ� ȣ��ȴٴ� ���Դϴ�.
// stack�� ��ǥ�� ���� ������ ���ǳ��뿡�� ������ �ٿ� ���� top ������ ����ϸ� �˴ϴ�.
// (���������� *top �� ���� stack �迭�� �ε����� �ǰ�����.)


// stack

int cost = 100000;
int *pcost = &cost;
int mincost = 0;
int *pmincost = &mincost;
int count = 0 ;
int *pcount = &count;

typedef struct _Infor{
    int x;
    int y;
}Infor;



int move(int **maze,int**mark,int row, int col, Infor *stack, int *top);
void push(Infor *stack, int dir);
void pop(int *top, int dir);


int move(int **maze,int**mark,int row, int col, Infor *stack, int *top) // *top == 0
{
    // *top �� main�Լ��� �迭������ ���ÿ� �����ϴ� �ε��� ���� �����ε�
    int d =0;
    int dir[8] = {N,NE,E,SE,S,SW,W,NW};


    if (stack->x == row-2 && stack->y == col-2)
    {
        if (*pcost < *pmincost)
        {
            *pmincost = *pcost;
            *pcount = 0;
        }
        else if (*pcost == *pmincost)
        {
            *pcount ++;
        }
        *pcost = 0 ;

        // ����!!
    }

    if (maze[stack->x][stack->y] == 0 && mark[stack->x][stack->y] == 0) // ���� ��ġ�� �ü� �ִ� ��ġ�̸�  �ѹ��� �� �� ��ġ�϶�
    {
        // cost ++;
    mark[stack->x][stack->y] =1 ; //mark���ٰ� 1�� ����
        push (stack, dir[d]); // N�������� �б�
        move (maze , mark , row, col, stack, top); // move ���ȣ��
    }

    else // ���� ��ġ�� ������ �ѹ� �� ���̰ų� ������ ��ġ�϶�
    {
        pop(top, dir[d]); // �ϴ� pop ��Ű��

        if ( d == NW )
        {
            *top--;
            move (maze , mark , row, col, stack, top); // ���࿡ ���� ������ �ϴ� ������ �������̾��ٸ�  ��Ʈ��ŷ
        }
        else
        {
            *top ++;
            push(stack, dir[d+1]);    // ���࿡ �ƴ϶�� ���� �������� �а�
            move (maze , mark , row, col, stack, top); // move ���ȣ��
        }
     }
    return *pcount;
}

void push(Infor *stack, int dir)
{
    if (dir == N)
    {
        stack->y--;
        *pcost ++;
    }
    else if (dir == NE)
    {
        stack->x++;
        stack->y--;
        *pcost  +=2;
    }
    else if (dir == E)
    {
        stack->x++;
        *pcost ++;
    }
    else if (dir == SE)
    {
        stack->x++;
        stack->y++;
        *pcost  +=2;
    }
    else if (dir == S)
    {
        stack->y++;
        *pcost ++;
    }
    else if (dir == SW)
    {
        stack->y++;
        stack->x--;
        *pcost  +=2;
    }
    else if (dir == W)
 {
        stack->x--;
        *pcost ++;
    }
    else if (dir == NW)
    {
        stack->x--;
        stack->y--;
        *pcost  +=2;
    }
}

void pop(int *top, int dir)
{
    if (*top == 0) // stack�� �������
    {
        printf("[Error] ������ �̹� ����ֽ��ϴ�");
     }
    else
    {
        *top--;
        if (dir == N || dir == E || dir == S || dir == W)
        {
            *pcost--;
        }
        else if (dir == NE || dir == SE || dir == SW || dir == NW)
        {
            *pcost -=2;
        }
    }

}

