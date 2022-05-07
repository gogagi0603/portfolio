#define QUEUE_SIZE 100

typedef struct _Queue{
    int queue[100];
    int head;
    int tail;
}Queue;

void mvqueue(Queue *Q,int bfhead, int bftail, int qt);
void rvmvqueue(Queue *Q,int bfhead, int bftail, int qt);
void relocate(Queue *Q, int which, int base, int reverse);


void relocate(Queue *Q, int which, int base, int reverse)
{
    int bfhead = Q->head;
    int bftail = Q->tail;

    int qt;

    if (Q->tail > Q->head)
    {
        qt = (Q->tail) - (Q->head);
    }
    else
    {
        qt = (Q->tail + 101) - Q->head;
    }

    if (which == 72)
    {
        Q->head = base;
        Q->tail = base + qt;

        if(Q->tail > 99)
        {
            Q->tail -= 100;
        }
    }

    if (which == 84)
    {
        Q->tail = base;
        Q->head = base - qt;

        if (Q->head > 99)
        {
            Q->head -= 100;
        }
        else if (Q->head < 0)
        {
            Q->head += 100;
        }
    }

    if (reverse == 0)
    {
        mvqueue (Q,bfhead,bftail,qt);
    }
    if (reverse)
{
        rvmvqueue(Q,bfhead,bftail,qt);
    }
}



void rvmvqueue(Queue *Q,int bfhead, int bftail, int qt)
{
    int tailtemp = Q->tail;
    int headtemp = Q->head;
    int i;
    int A[qt];

    for (i=0 ; i<=qt-1 ; i++)
    {
        A[i] = Q->queue[bfhead];
        Q->queue[bfhead] = '\0';
        if (bfhead == 99)
        {
            bfhead = 0;
        }
        else
        {
            bfhead ++;
        }
    }

    for (i=0 ; i<= qt-1 ; i++)
    {
        if (Q->tail == 0)
        {
            Q->tail = 100;
        }

        Q->queue[Q->tail-1] = A[i];

        if (Q->tail == 1)
        {
            Q->tail = 100;
        }
        else
        {
            Q->tail --;
        }
    }
    Q->tail = tailtemp;
    Q->head = headtemp;
}

void mvqueue(Queue *Q,int bfhead, int bftail, int qt)
{
    int i;
    int A[qt];
    int headtemp = Q->head;
    int tailtemp = Q->tail;
    for (i=0 ; i<=qt-1 ; i++)
    {
        A[i] = Q->queue[bfhead];
        Q->queue[bfhead] = '\0';
 if (bfhead == 99)
        {
            bfhead = 0;
        }
        else
        {
            bfhead ++;
        }
    }

    for (i=0 ; i<= qt-1 ; i++)
    {
        Q->queue[Q->head] = A[i];

        if (Q->head ==99 )
        {
            Q->head = 0;
        }
        else
        {
            Q->head ++;
        }
    }
    Q->head = headtemp;
    Q->tail = tailtemp-1;
}


