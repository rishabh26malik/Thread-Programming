#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

#define MAX_THREAD 3
#define ROW 3
#define COL 3

int matrix1[ROW][COL];
int matrix2[ROW][COL];
int matrixRes[ROW][COL] = {{0,0,0},{0,0,0},{0,0,0}};
int indices[]={0, 1, 2};
int ans[MAX_THREAD] = {0};

void *mat_mul(void *arg){
	//printf("\n");
	fflush(stdout);
	int thread_no = *(int*)arg, i, j, k;
	printf("<THREAD %d>", thread_no);
	for(i = 0; i < ROW; i++){
		for(j = 0; j < COL; j++){
			matrixRes[thread_no][i] = matrixRes[thread_no][i] +  (matrix1[thread_no][j]*matrix2[j][i]);
			//printf("%d %d\n",thread_no,i);  	
		}
	}
	return (NULL);
}

int main(int argc, char* argv[]){
	int i,j;
	srand(time(0));
	pthread_t threadId[MAX_THREAD];
	time_t t1, t2;
	
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			matrix1[i][j] = (rand()%10);
			matrix2[i][j] = (rand()%10);
		}
	}
	(void) time(&t1);
	
	for(i=0; i<MAX_THREAD; i++){
		pthread_create(&threadId[i], NULL, mat_mul, (void*)&indices[i]);
		sleep(0.5);
	}
	printf("Matrix 1:\n");
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			printf("%d ",matrix1[i][j]);
		}
		printf("\n");
	}
	printf("Matrix 2:\n");
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			printf("%d ",matrix2[i][j]);
		}
		printf("\n");
	}
	printf("Matrix Resultant:\n");
	for(i=0; i<ROW; i++){
		for(j=0; j<COL; j++){
			printf("%d ",matrixRes[i][j]);
		}
		printf("\n");
	}
	
	(void) time(&t2);
	
	printf("Total Time Taken %lf sec\n", (double)t2-(double)t1);
	return 0;
}
