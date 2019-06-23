#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
int c=0;
void *counter(void *arg){
	int i;
	for(i=0;i<10;i++){
		c++;
	}
	return(NULL);
}
void *printer(void *arg){
	printf("%d",c);
	return(NULL);
}
int main(int argc, char* argv[]){
	fflush(stdout);
	pthread_t thread1, thread2;
	pthread_create(&thread1, NULL, counter, NULL);
	pthread_create(&thread2, NULL, printer, NULL); 
	printf("Hello from process \n");
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	printf("both thread Completed! \n");
}
