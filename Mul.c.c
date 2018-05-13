//
//  Mul.c
//  
//
//  Created by Zhentao Xie on 9/10/16.
//
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

#define M 2
#define N 4
#define Q 5

int A[M][N];
int B[N][Q];
int C[M][Q];

void read_matrix();
void *mul_thread_matrix(void *ptr);
void *mul_thread_row(void *ptr);
void *mul_thread_element(void *ptr);
void print_matrix(int matrix[M][Q]);

typedef struct
{
    int start_row;
    int element_num;
    
}thread_data_t;

struct timeval start1, end1, start2, end2, start3, end3;

int main()
{
    pthread_t thread1;
    pthread_t thread2[M],thread3[(M*Q)];
    thread_data_t thread_data2[M],thread_data3[(M*Q)];
    unsigned long time1,time2,time3;
    read_matrix(); //read two matrix from file
    int i;
    int choice;
    printf("Type 1 for 1 thread; 2 for 2 threads; 3 for 10 threads\n");
    scanf("%d",&choice);
    switch (choice)
    {
        
        /////////////////////////////////1 thread start (1 matrix)////////////////////////////////////////////
  case 1:
        //clock_t start=clock(); //timer start to count 1 thread
        gettimeofday(&start1,NULL);
        pthread_create(&thread1,NULL,(void *)&mul_thread_matrix,NULL); //create 1 thread
        //wait the thread finish work
        pthread_join(thread1,NULL);
        //clock_t finish=clock();
        
        gettimeofday(&end1,NULL);
        time1=(end1.tv_sec-start1.tv_sec)*1000000+((end1.tv_usec-start1.tv_usec));
        printf("Time of calculation by 1 thread is %ld microsecond.\n",time1);

        printf("The Matrix C (1 threads) is\n");
        print_matrix(C);
    
        //printf("Time of calculation by 1 thread is %.8fsec\n",(long)(finish-start)/1E6);
        break;
      /////////////////////////////////////1 thread finish////////////////////////////////////////////
    
    
    
    
    //////////////////////////////////2 threads start (2 rows)///////////////////////////////////////////////
  case 2:
        //clock_t start=clock(); //timer start to count 2 threads
        gettimeofday(&start2,NULL);
        //Give 0 and 1 row number
        for (i=0; i<M; i++)
        {
          thread_data2[i].start_row=i;
        }
        for (i=0; i<M; i++) //create 2 threads
        {
          pthread_create(&thread2[i],NULL,(void *)&mul_thread_row,(void *)&thread_data2[i]);
        }
        //wait all threads finish work
        for (i=0; i<M; i++)
        {
          pthread_join(thread2[i],NULL);
        }
        //clock_t finish=clock();
        gettimeofday(&end2,NULL);
        time2=(end2.tv_sec-start2.tv_sec)*1000000+((end2.tv_usec-start2.tv_usec));
        printf("Time of calculation by 2 threads %ld microsecond.\n",time2);
        printf("The Matrix C (2 threads) is\n");
        print_matrix(C);
    
        //printf("Time of calculation by 2 threads is %.8fsec\n",(long)(finish-start)/1E6);
        break;
    /////////////////////////////////2 threads finish///////////////////////////////////////////////
    
    
    
    //////////////////////////////////10 threads start (10 elements)//////////////////////////////////////////
  case 3:
       //clock_t start=clock(); //timer start to count 10 threads
       gettimeofday(&start3,NULL);
       //Give 0~9 to element_num
       for (i=0; i<(M*Q); i++)  //M*Q=10
       {
         thread_data3[i].element_num=i;
       }
       for (i=0; i<(M*Q); i++) //create 10 threads
       {
          pthread_create(&thread3[i],NULL,(void *)&mul_thread_element,(void *)&thread_data3[i]);
       }
       //wait all threads finish work
       for (i=0; i<(M*Q); i++)
       {
           pthread_join(thread3[i],NULL);
       }
       //clock_t finish=clock();
       gettimeofday(&end3,NULL);
       time3=(end3.tv_sec-start3.tv_sec)*1000000+((end3.tv_usec-start3.tv_usec));
       printf("Time of calculation by 10 threada is %ld microsecond.\n",time3);
       printf("The Matrix C (10 threads)is\n");
       print_matrix(C);
    
       //printf("Time of calculation by 10 threads is %.8fsec\n",(long)(finish-start)/1E6);
       break;
         ///////////////////////////////////10 threads finish///////////////////////////////////////////
    }
    return 0;
}

// 1 thread
void *mul_thread_matrix(void *ptr)
{
    int i,j,k;
    for (i=0; i<M; i++)
    {
        for (j=0; j<Q; j++)
        {
            for (k=0; k<N; k++)
            {
                C[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}


//2 threads
void *mul_thread_row(void *ptr)
{
    int start_row;
    int i,j,k;
    thread_data_t *td = (thread_data_t *) ptr;
    start_row = td->start_row;
    int end_row = start_row+1;
    for (i=start_row; i<end_row; i++)
    {
        for (j=0; j<Q; j++)
        {
            for (k=0; k<N; k++)
            {
                C[i][j]+=A[i][k]*B[k][j];
            }
        }
    }
    pthread_exit(NULL);
}


//10 threads
void *mul_thread_element(void *ptr)
{
    int element_num;
    int row,col;
    thread_data_t *td = (thread_data_t *)ptr;
    element_num = td->element_num;
    if (element_num < Q) //element_num<5 then row=0, col=element_num
    {
        row = 0;
        col = element_num;
    }
    else
    {
        row = M-1;
        col = element_num-5;
    }
    int i,j,k;
    for (k=0; k<N; k++)
    {
        C[row][col]+=A[row][k]*B[k][col];
    }
    pthread_exit(NULL);
}


void read_matrix()
{
    FILE *fp;
    int i,j,a,b,c,d;
    fp = fopen("Multiplication.txt","r");
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
        for (i=0; i<N; i++)  //read matrix B
        {
            for (j=0; j<Q; j++)
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
        for (i=0; i<N; i++) //print matrix B
        {
            for (j=0; j<Q; j++)
            {
                printf("%d ",B[i][j]);
            }
            printf("\n");
        }
        
    }
    fclose(fp);
}

void print_matrix(int matrix[M][Q])
{
    int i,j;
    for ( i=0; i<M; i++)
    {
        for (j=0; j<Q; j++)
        {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }

}
