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

    // *top �� main�Լ��� �迭������ ���ÿ� �����ϴ� �ε��� ���� �����ε� ... �׷��� Infor stack[*top] �̷������ΰ�
    int a = 0;
    int *aa = &a;
    if (stack[*top].x == row-2 && stack[*top].y == col-2) // ����!!
    {


        if (*pcost < *pmincost) // ���� �� cost�� mincost���� ������ �ȿ� �ְ� ī��Ʈ����
        {
            *pmincost = *pcost;
            *pcount = 1;
        }
        else if (*pcost == *pmincost) // ���� �� cost�� mincost�� ������ ī��Ʈ ++
        {
            *pcount += 1;
        }


        pop (top, stack[*top-1].d, pcost) ; // ������ ���ܰ��  ������


        *top = *top + 1;
 stack[*top].x = stack[*top -1 ].x;
        stack[*top].y = stack[*top -1 ].y;
        stack[*top -1].d += 1; // �� ������ ���� ����

        push(stack,top, pcost); // push ���ְ�

        move (maze , mark , row, col, stack, top);  //��� ȣ��
    }



     else if (*aa == 0 && stack[*top].x == 1 && stack[*top].y == 1 && * top == 0 && stack[*top].d == N && mark[stack[*top].x][stack[*top].y] == 1 ) // ���ۺκ���. �ѹ� ����ǰ� �ٽ� ������� ����
     {

         *top = *top + 1;

         stack[1].x = stack[0].x;
         stack[1].y = stack[0].y;
         stack[1].d = 0;
         push(stack,top,pcost);  // top�� �ϳ� �ø��� N���� �о (1,0) ���� �̵�
         *aa += 1;


         move ( maze, mark, row, col ,stack ,top);
     }




    else if ( maze[stack[*top].x][stack[*top].y] == 0 &&  mark[stack[*top].x][stack[*top].y] == 0) // ���� ��ġ�� �ü� �ִ� ��ġ�̸�  �ѹ��� �� �� ��ġ�϶�
    {


        mark[stack[*top].x][stack[*top].y] =1 ; //mark���ٰ� 1�� ����


        *top  = *top +1; // ž�� �ø���

        stack[*top].x = stack[*top-1].x; // ���� ���� �迭�� �� ��ǥ �� ����
        stack[*top].y = stack[*top-1].y; // ,,
        stack[*top].d = 0; // N���� push �ϱ� ����
        push (stack,top, pcost); // N�������� �б�


        move (maze, mark, row , col ,stack, top); // move ���ȣ��
    }


    else // ���� ��ġ�� ������ �ѹ� �� ���̰ų� ������ ��ġ�϶�
    {

        pop(top, stack[*top-1].d, pcost); // �ϴ� pop ��Ű��

        if( stack[*top].d >= NW ) // ��� �����̵� �� �� �ִ� ������ ������
        {

           mark[stack[*top].x][stack[*top].y] = 0 ;  // ���� ���� ���ٴ� �ǹ�

  pop (top, stack[*top-1].d, pcost);     // ���࿡ ���� ������ �ϴ� ������ �������̾��ٸ�  ��Ʈ��ŷ


          if ( *top <= -1 )  // ��� ����� ���� �� ������ �Ա��� ������ ���϶�
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
        else // ���� ��� ������ Ž������ ����
        {
            *top = *top + 1;
            stack[*top].x = stack[*top-1].x;
            stack[*top].y = stack[*top-1].y;

             // ��������

            stack[*top-1].d += 1;

            push(stack,top, pcost);    // ���� �������� ��


            move (maze, mark , row , col, stack, top); // move ���ȣ��
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
                                                                          
