#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(){
    int SIZE = 3; 
    int i,j,k,sum=0;
    int A[SIZE][SIZE], B[SIZE][SIZE], M[SIZE][SIZE];
    srand(time(0)); //randomize seed
    //populate matrices A and B with random values from 0-9
    for (i=0;i<SIZE;i++){
        for (j=0;j<SIZE;j++){
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }
    //print matrix A
    printf("-----\n");
    printf("--A--\n");
    printf("-----\n");
    for (i=0;i<SIZE;i++){
        for (j=0;j<SIZE;j++){
            printf("%d  ",A[i][j]);
        }
        printf("\n");
    }
    //print matrix B
    printf("-----\n");
    printf("--B--\n");
    printf("-----\n");
    for (i=0;i<SIZE;i++){
        for (j=0;j<SIZE;j++){
            printf("%d  ",B[i][j]);
        }
        printf("\n");
    }
    //matrix multiplication
    for (i=0;i<SIZE;i++){
        for (j=0;j<SIZE;j++){
            //for each element multiply row vector A[i][] with column vector B[][j]
            for (k=0;k<SIZE;k++){
                sum = sum + A[i][k] * B[k][j];
            }
            M[i][j]=sum;
            sum=0;
        }
    }
    //print final matrix
    printf("-----\n");
    printf("--M--\n");
    printf("-----\n");
    for (i=0;i<SIZE;i++){
        for (j=0;j<SIZE;j++){
            printf("%3d  ",M[i][j]);
        }
        printf("\n");
    }
}