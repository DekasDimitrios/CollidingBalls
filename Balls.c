#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <graphics.h>

struct BallChar
{
    double x;
    double y;
    int c;
    double m;
    double r;
    double sx;
    double sy;
};

int ValuesCheck(struct BallChar *array, int n)
{
    int k=0,l=n+1;
    if((array[n].x < 0) || (array[n].x > 600))
    {
        printf("Invalid starting position on x axes on the sphere number %d\n",l);
        k=1;
    }
    if((array[n].y < 0) || (array[n].y > 400))
    {
        printf("Invalid starting position on y axes on the sphere number %d\n",l);
        k=1;
    }
    if((array[n].c < 0) || (array[n].c > 15))
    {
        printf("Invalid color on the sphere number %d\n",l);
        k=1;
    }
    if((array[n].m < 0) || (array[n].m > 10000))
    {
        printf("Invalid mass on the sphere number %d\n",l);
        k=1;
    }
    if((array[n].r < 1) || (array[n].r > 100))
    {
        printf("Invalid radius on the sphere number %d\n",l);
        k=1;
    }
    if((array[n].sx < -100) || (array[n].sx > 100))
    {
        printf("Invalid starting speed on x axes on the sphere number %d\n",l);
        k=1;
    }
    if((array[n].sy < -100) || (array[n].sy > 100))
    {
        printf("Invalid starting speed on y axes on the sphere number %d\n",l);
        k=1;
    }
    return k;
}

struct BallChar *BallRead(int *K)
{
    struct BallChar *array;
    FILE *file;
    if((file=fopen("input.txt", "r")) == NULL)
        printf("Could not open file - aborting\n");
    else
    {
        int N,i;
        fscanf(file, "%d", &N);
        array=(BallChar*)malloc(N * sizeof(struct BallChar));
        *K=N;
        if(array == NULL)
            printf("Could not allocate memory\n");
        else
        {
            for(i=0;i<N;i++)
            {
                fscanf(file, "%lf", &array[i].x);
                fscanf(file, "%lf", &array[i].y);
                fscanf(file, "%d", &array[i].c);
                fscanf(file, "%lf", &array[i].m);
                fscanf(file, "%lf", &array[i].r);
                fscanf(file, "%lf", &array[i].sx);
                fscanf(file, "%lf", &array[i].sy);
                if(ValuesCheck(array, i))
                    break;
            }
        }
    }
    fclose(file);
    return array;
}

double BallsDistance(double X1,double Y1,double X2,double Y2)
{
    double res = (X2-X1)*(X2-X1)+(Y2-Y1)*(Y2-Y1);
    return sqrt(res);
}

int BallsCollide(struct BallChar *array, int i, int j)
{
    if(BallsDistance(array[i].x, array[i].y, array[j].x, array[j].y) <= (array[i].r + array[j].r))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void BallsCollision(struct BallChar *array, int i, int j)
{
    double SumMass = array[i].m + array[j].m;
    double ij_mass = array[i].m - array[j].m;
    double ji_mass = array[j].m - array[i].m;
    double sx1,sx2,sy1,sy2;
    sx1= (2.0 * array[j].sx * array[j].m) / SumMass + (array[i].sx * (ij_mass)) / SumMass;
    sy1= (2.0 * array[j].sy * array[j].m) / SumMass + (array[i].sy * (ij_mass)) / SumMass;
    sx2= (2.0 * array[i].sx * array[i].m) / SumMass + (array[j].sx * (ji_mass)) / SumMass;
    sy2= (2.0 * array[i].sy * array[i].m) / SumMass + (array[j].sy * (ji_mass)) / SumMass;
    array[i].sx=sx1;
    array[i].sy=sy1;
    array[j].sx=sx2;
    array[j].sy=sy2;
}

int main()
{
    clock_t start,finish,previous;
    int gdriver = DETECT,gmode,errorcode;
    initgraph(&gdriver,&gmode,"");
    errorcode = graphresult();
    if(errorcode!=grOk)
    {
        printf("Graphics error: %s\n",grapherrormsg(errorcode));
        system("pause");
        exit(1);
    }

    int i,j,K;
    struct BallChar *array = BallRead(&K);
    double step;
    start=clock();
    previous=start;
    do
    {
        finish=clock();
        step=(finish-previous)*1.0/CLOCKS_PER_SEC;
        if(step >= 0.01)
        {
            previous=finish;
            for(i=0;i<K;i++)
            {
                setfillstyle(SOLID_FILL,BLACK);
                setcolor(BLACK);
                fillellipse(array[i].x, array[i].y, array[i].r, array[i].r);

                array[i].x+= array[i].sx * step;
                array[i].y+= array[i].sy * step;

                if(array[i].x + array[i].r >= getmaxx() || array[i].x - array[i].r <= 0)
                    array[i].sx*=-1;

                if(array[i].y + array[i].r >= getmaxy() || array[i].y - array[i].r <= 0)
                    array[i].sy*=-1;

                for(j=i+1;j<K;j++)
                {
                    if(BallsCollide(array, i, j))
                        BallsCollision(array, i, j);
                }
                setfillstyle(SOLID_FILL, array[i].c);
                setcolor(array[i].c);
                fillellipse(array[i].x, array[i].y, array[i].r, array[i].r);
            }

        }
    }
    while(!kbhit());
    closegraph();
    return 0;
}
