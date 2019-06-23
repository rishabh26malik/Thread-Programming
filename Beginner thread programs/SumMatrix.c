#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define MAX_THREAD 4
#define MAX_NO 4
int arr[MAX_NO][MAX_NO]={{7,0,4,2},{8,6,9,6},{7,0,4,2},{8,6,9,6}};
int nrow=4,ncol=4;
int sum[MAX_THREAD]={0};
void *array_sum(void *arg){
	fflush(stdout);
	int loop = *(int*)arg;
	int i = loop, j=0;
	printf("<THREAD %d> ",i);
	for(i=MAX_NO*(loop/4);i<(MAX_NO*((loop+1)/4));i++){
		for(j=0;j<MAX_NO;j++){
			printf("< %d >\n",arr[i][j]);
			sum[loop]+=arr[i][j];
		}
	}
	return(NULL);
}
int main(int argc, char* argv[]){
	int maxm=0;
	int i,orig_sum=0;
	fflush(stdout);
	pthread_t threads[MAX_THREAD];
	time_t t1,t2;
	
	(void) time(&t1);
	for(i=0;i<MAX_THREAD;++i){
		pthread_create(&threads[i], NULL, array_sum, (void*)&i);
		sleep(0.1);
	}
	for(i=0;i<MAX_THREAD;++i){
		pthread_join(threads[i], NULL);
		orig_sum+=sum[i];
	}
	(void)time(&t2);
	printf("both thread Completed! sum=%d time %lf seconds\n",orig_sum,(double)(t2-t1));
}
