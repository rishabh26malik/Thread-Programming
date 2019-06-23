#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
void *hellofn(void *arg){
	//int value = *(int*) arg;
	//char *s = (char*)arg;
	int *b=(int*)arg;
	//printf("Value is %d\n",value);
	//printf("String is %s",s);
	printf("%d \n",b[2]);
	sleep(10);
	printf("I woke up \n");
	return(NULL);
}

int main(int argc, char* argv[]){
	//int i=42;
	//char *str="ABCD";
	fflush(stdout); //to flush the buffer out
	int a[] = {5,10,-5,0,7};
	pthread_t threads;
	pthread_attr_t pthread_custom_attr;
	
	pthread_attr_init(&pthread_custom_attr);
	
	/*strings and array are passed as variable name
	  threads is the name of thread, parameter passing i*/
	pthread_create(&threads, &pthread_custom_attr, hellofn, (void*)a); 
	printf("Hello from process \n");
	pthread_join(threads, NULL);  //so that process wait for thread to complete its execution
	printf("Thread Completed! \n");
}
