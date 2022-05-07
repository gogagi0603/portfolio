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

int cost = 0;
int *pcost = &cost;
int mincost = 1000;
int *pmincost = &mincost;
int count = 0 ;
int *pcount = &count;

typedef struct _Infor{
    int x;
    int y;
    int d;
}Infor;



int move(int **maze,int**mark,int row, int col, Infor *stack, int *top);
void push(Infor *stack,int *top, int *pcost);
void pop(int *top, int dir, int *pcost);


int move(int **maze,int**mark,int row, int col, Infor *stack, int *top) // *top == 0
{

    // *top 이 main함수의 배열형태의 스택에 접근하는 인덱스 같은 느낌인듯 ... 그러면 Infor stack[*top] 이런형식인가
    int a = 0;
    int *aa = &a;
    if (stack[*top].x == row-2 && stack[*top].y == col-2) // 도착!!
    {


        if (*pcost < *pmincost) // 여태 든 cost가 mincost보다 작으면 안에 넣고 카운트시작
        {
            *pmincost = *pcost;
            *pcount = 1;
        }
        else if (*pcost == *pmincost) // 여태 든 cost가 mincost랑 같으면 카운트 ++
        {
            *pcount += 1;
        }


        pop (top, stack[*top-1].d, pcost) ; // 스택을 전단계로  돌리고


        *top = *top + 1;
 stack[*top].x = stack[*top -1 ].x;
        stack[*top].y = stack[*top -1 ].y;
        stack[*top -1].d += 1; // 그 스택의 다음 방향

        push(stack,top, pcost); // push 해주고

        move (maze , mark , row, col, stack, top);  //재귀 호출
    }



     else if (*aa == 0 && stack[*top].x == 1 && stack[*top].y == 1 && * top == 0 && stack[*top].d == N && mark[stack[*top].x][stack[*top].y] == 1 ) // 시작부분임. 한번 실행되고 다신 실행되지 않음
     {

         *top = *top + 1;

         stack[1].x = stack[0].x;
         stack[1].y = stack[0].y;
         stack[1].d = 0;
         push(stack,top,pcost);  // top을 하나 올리고 N으로 밀어서 (1,0) 으로 이동
         *aa += 1;


         move ( maze, mark, row, col ,stack ,top);
     }




    else if ( maze[stack[*top].x][stack[*top].y] == 0 &&  mark[stack[*top].x][stack[*top].y] == 0) // 지금 위치가 올수 있는 위치이며  한번도 안 온 위치일때
    {


        mark[stack[*top].x][stack[*top].y] =1 ; //mark에다가 1을 새김


        *top  = *top +1; // 탑을 올리고

        stack[*top].x = stack[*top-1].x; // 다음 스택 배열에 전 좌표 값 대입
        stack[*top].y = stack[*top-1].y; // ,,
        stack[*top].d = 0; // N으로 push 하기 위해
        push (stack,top, pcost); // N방향으로 밀기


        move (maze, mark, row , col ,stack, top); // move 재귀호출
    }


    else // 지금 위치가 예전에 한번 온 곳이거나 못오는 위치일때
    {

        pop(top, stack[*top-1].d, pcost); // 일단 pop 시키고

        if( stack[*top].d >= NW ) // 어느 방향이든 갈 수 있는 방향이 없을대
        {

           mark[stack[*top].x][stack[*top].y] = 0 ;  // 여기 온적 없다는 의미

  pop (top, stack[*top-1].d, pcost);     // 만약에 지금 가고자 하는 방향이 마지막이었다면  백트래킹


          if ( *top <= -1 )  // 모든 경우의 수를 다 가봐서 입구에 도착한 거일때
            {
                return *pcount;
            }

            * top = *top +1;

            stack[*top].x = stack[*top-1].x;
            stack[*top].y = stack[*top-1].y;
            stack[*top].d = 0;
            stack[*top-1].d +=1;

            push (stack, top, pcost);
            move (maze, mark, row, col ,stack, top);
        }
        else // 아직 모든 방향을 탐색하진 않음
        {
            *top = *top + 1;
            stack[*top].x = stack[*top-1].x;
            stack[*top].y = stack[*top-1].y;

             // 다음방향

            stack[*top-1].d += 1;

            push(stack,top, pcost);    // 다음 방향으로 밀


            move (maze, mark , row , col, stack, top); // move 재귀호출
        }
     }

}

void push(Infor *stack,int *top, int *pcost)
{
    if (stack[*top - 1].d == N)
    {
        stack[*top].y--;
        *pcost += 1;
    }
    else if (stack[*top -1 ].d == NE)
    {
        stack[*top].x++;
        stack[*top].y--;
        *pcost  +=2;
    }
    else if (stack[*top -1 ].d == E)
    {
        stack[*top].x++;
        *pcost +=1;
    }
    else if (stack[*top - 1].d == SE)
    {
        stack[*top].x++;
        stack[*top].y++;
        *pcost  +=2;
                                                                                                  }
    else if (stack[*top - 1].d == S)
    {
        stack[*top].y++;
        *pcost +=1;
    }
    else if (stack[*top - 1].d == SW)
    {
        stack[*top].y++;
        stack[*top].x--;
        *pcost  +=2;
    }
    else if (stack[*top - 1].d == W)
    {
        stack[*top].x--;
        *pcost +=1;
    }
    else if (stack[*top-1].d == NW)
    {
        stack[*top].x--;
        stack[*top].y--;
        *pcost  +=2;
    }
}

void pop(int *top, int dir, int *pcost)
{
    *top = *top - 1;
    if (dir == N || dir == E || dir == S || dir == W)
    {
        *pcost-=1;
    }
    else if (dir == NE || dir == SE || dir == SW || dir == NW)
    {
        *pcost -=2;
    }
}
                                                                          
