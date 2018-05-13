//
//  Add.c
//  
//
//  Created by Zhentao Xie on 9/10/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define M 20
#define N 20

int A[M][N];
int B[M][N];
int C[M][N];

void read_matrix();
void *Add_thread_matrix(void *ptr);
void *Add_thread_row(void *ptr);
void *Add_thread_element(void *ptr);
void print_matrix(int matrix[M][N]);

typedef struct
{
    int start_row;
    int element_num;
    
}thread_data_t;

struct timeval start1, end1, start2, end2, start3, end3;

int main()
{
    pthread_t thread1;
    pthread_t thread2[M],thread3[(M*N)];
    thread_data_t thread_data2[M],thread_data3[(M*N)];
    unsigned long time1,time2,time3;
    read_matrix(); //read two matrix from file
    int i;
    int choice;
    printf("Type 1 for 1 thread; 2 for 20 threads; 3 for 400 threads\n");
    scanf("%d",&choice);
    switch (choice)
    {
            
        ////////////////////////////////1 thread start (1 matrix)////////////////////////////////////////////
        case 1:
    
            gettimeofday(&start1,NULL); //timer start to count 1 thread
            pthread_create(&thread1,NULL,(void *)&Add_thread_matrix,NULL); //create 1 thread
            pthread_join(thread1,NULL);//wait the thread finish work
            gettimeofday(&end1,NULL); // timer stop
            time1=(end1.tv_sec-start1.tv_sec)*1000000+((end1.tv_usec-start1.tv_usec));
            printf("Time of calculation by 1 thread is %ld microsecond.\n",time1);
            printf("The Matrix C (1 threads) is\n");
            print_matrix(C);
        
            break;
            /////////////////////////////////////1 thread finish////////////////////////////////////////////
            
            
            
            
        ////////////////////////////////20 threads start (20 rows)//////////////////////////////////////////
       case 2:
            gettimeofday(&start2,NULL); //timer start to count 2 threads
            //Give 20 row number
            for (i=0; i<M; i++)
            {
                thread_data2[i].start_row=i;
            }
            for (i=0; i<M; i++) //create 20 threads
            {
                pthread_create(&thread2[i],NULL,(void *)&Add_thread_row,(void *)&thread_data2[i]);
            }
            //wait all threads finish work
            for (i=0; i<M; i++)
            {
                pthread_join(thread2[i],NULL);
            }
            //timer stop;
            gettimeofday(&end2,NULL);
            time2=(end2.tv_sec-start2.tv_sec)*1000000+((end2.tv_usec-start2.tv_usec));
            printf("Time of calculation by 20 threads %ld microsecond.\n",time2);
            printf("The Matrix C (20 threads) is\n");
            print_matrix(C);
    
           break;
            /////////////////////////////////20 threads finish///////////////////////////////////////////////
            
            
            
            ///////////////////////////////400 threads start (400 elements)////////////////////////////
       case 3:
            //timer start to count 400 threads
            gettimeofday(&start3,NULL);
            //Give 400 to element_num
            for (i=0; i<(M*N); i++)  //M*N=400
            {
                thread_data3[i].element_num=i;
            }
            for (i=0; i<(M*N); i++) //create 400 threads
            {
                pthread_create(&thread3[i],NULL,(void *)&Add_thread_element,(void *)&thread_data3[i]);
            }
            //wait all threads finish work
            for (i=0; i<(M*N); i++)
            {
                pthread_join(thread3[i],NULL);
            }
            //clock_t finish=clock();
            gettimeofday(&end3,NULL);
            time3=(end3.tv_sec-start3.tv_sec)*1000000+((end3.tv_usec-start3.tv_usec));
            printf("Time of calculation by 400 threada is %ld microsecond.\n",time3);
            printf("The Matrix C (400 threads)is\n");
            print_matrix(C);
    
            break;
            /////////////////////////////////400 threads finish///////////////////////////////////////////
    }
    return 0;
}

// 1 thread
void *Add_thread_matrix(void *ptr)
{
    int i,j;
    for (i=0; i<M; i++)
    {
        for (j=0; j<N; j++)
        {
            C[i][j]=A[i][j]+B[i][j];
        }
    }
    pthread_exit(NULL);
}


//20 threads
void *Add_thread_row(void *ptr)
{
    int start_row;
    int i,j;
    thread_data_t *td = (thread_data_t *) ptr;
    start_row = td->start_row;
    int end_row = start_row+1;
    for (i=start_row; i<end_row; i++)
    {
        for (j=0; j<N; j++)
        {
            C[i][j]=A[i][j]+B[i][j];
        }
    }
    pthread_exit(NULL);
}


//400 threads
void *Add_thread_element(void *ptr)
{
    int element_num;
    int row,col;
    thread_data_t *td = (thread_data_t *)ptr;
    element_num = td->element_num;
    row = element_num/20;
    col = element_num%20;
    C[row][col] = A[row][col]+B[row][col];
    pthread_exit(NULL);
}


void read_matrix()
{
    FILE *fp;
    int i,j,a,b,c,d;
    fp = fopen("addition.txt","r");
    if (fp==NULL)
    {
        printf("Fail to open file Multiplication.txt\n");
        exit (0);
    }
    else
    {
        fscanf(fp, "%d %d",&a,&b); //read matrix A dimension
        for (i=0; i<M; i++) //read matrix A
        {
            for (j=0; j<N; j++)
            {
                fscanf(fp, "%d",&A[i][j]);
            }
        }
        fscanf(fp, "%d %d",&c,&d); //read matrix B dimesion
        for (i=0; i<M; i++)  //read matrix B
        {
            for (j=0; j<N; j++)
            {
                fscanf(fp, "%d",&B[i][j]);
            }
        }
        
        //output two matrix
        printf("The matrix A is\n");
        for (i=0; i<M; i++) //print matrix A
        {
            for (j=0; j<N; j++)
            {
                printf("%d ",A[i][j]);
            }
            printf("\n");
        }
        
        printf("The matrix B is\n");
        for (i=0; i<M; i++) //print matrix B
        {
            for (j=0; j<N; j++)
            {
                printf("%d ",B[i][j]);
            }
            printf("\n");
        }
        
    }
    fclose(fp);
}

void print_matrix(int matrix[M][N])
{
    int i,j;
    for ( i=0; i<M; i++)
    {
        for (j=0; j<N; j++)
        {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
    
}

