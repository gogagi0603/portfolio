#define N 0
#define NE 1
#define E 2
#define SE 3
#define S 4
#define SW 5
#define W 6
#define NW 7
#include <stdio.h>

// 이미 설명한 바와 같이 stack에 필요한 메모리 공간은 배열의 형태로 미리 할당되어 호출된다는 뜻입니다.
// stack내 좌표에 대한 접근은 강의내용에서 설명한 바와 같이 top 변수를 사용하면 됩니다.
// (과제에서는 *top 의 값이 stack 배열의 인덱스가 되겠지요.)


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
    // *top 이 main함수의 배열형태의 스택에 접근하는 인덱스 같은 느낌인듯
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

        // 도착!!
    }

    if (maze[stack->x][stack->y] == 0 && mark[stack->x][stack->y] == 0) // 지금 위치가 올수 있는 위치이며  한번도 안 온 위치일때
    {
        // cost ++;
    mark[stack->x][stack->y] =1 ; //mark에다가 1을 새김
        push (stack, dir[d]); // N방향으로 밀기
        move (maze , mark , row, col, stack, top); // move 재귀호출
    }

    else // 지금 위치가 예전에 한번 온 곳이거나 못오는 위치일때
    {
        pop(top, dir[d]); // 일단 pop 시키고

        if ( d == NW )
        {
            *top--;
            move (maze , mark , row, col, stack, top); // 만약에 지금 가고자 하는 방향이 마지막이었다면  백트래킹
        }
        else
        {
            *top ++;
            push(stack, dir[d+1]);    // 만약에 아니라면 다음 방향으로 밀고
            move (maze , mark , row, col, stack, top); // move 재귀호출
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
    if (*top == 0) // stack이 비었을때
    {
        printf("[Error] 스택이 이미 비어있습니다");
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

