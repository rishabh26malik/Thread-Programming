#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#define MAX 1024
#define HEIGHT 1024
#define WIDTH 1024
unsigned char buffer[HEIGHT][WIDTH];					//to store the image data
unsigned char out_buffer[HEIGHT][WIDTH];
unsigned char mask[HEIGHT][WIDTH];
pthread_t thread[8]; 
void *avg_filter(void *arg)
{
	int core = (int)arg; 
	//AVERAGE FILTER
	printf("<%d %d>\n",core,core+127);
	int i,j,tmp,x,y,height=1024,width=1024,k;
	for(i=core ; i <= (core + 127); i++)
	{
		for(j=1;j<width-1;j++)
		{	//printf("%d\n",k);
			k++;
			tmp=0;
			for(x=-1;x<=1;x++)
				for(y=-1;y<=1;y++)
					tmp+=buffer[i+x][j+y];
			out_buffer[i][j]=tmp/9;
			//printf("%d %d - %d\n",i,j,(int)out_buffer[i][j]);
		}
	}
	printf("<<<<<<<<%d %d>\n",i,j);
}


void *sharp_filter(void *arg)
{
	int core = (int)arg; 
	int i,j,height=1024,width=1024;
	for(i=core;i<=( core + 127 );i++)
	{
		for(j=0;j<width-1 ;j++)
		{
			mask[i][j]=abs(out_buffer[i][j]-buffer[i][j]);
		}
	}
	/*
	for(i=0;i<MAX;i++)
	{
		for(j=0;j<MAX;j++)
		{
			out_buffer[i][j]=buffer[i][j]+mask[i][j];
		}
	}*/
	
}


int main()
{
	pthread_t thread[8]; 
	FILE *fIn = fopen("man.bmp","r");					//Input File name
	FILE *fOut = fopen("123.bmp","w+");		    	//Output File name

	int i,j,choice,x,y,tmp;
	unsigned char byte[54],colorTable[1024];

	if(fIn==NULL)											// check if the input file has not been opened succesfully.
	{
		printf("File does not exist.\n");
	}

	for(i=0;i<54;i++)										//read the 54 byte header from fIn
	{
		byte[i] = getc(fIn);
	}

	fwrite(byte,sizeof(unsigned char),54,fOut);				//write the header back

	// extract image height, width and bitDepth from imageHeader
	int height = *(int*)&byte[18];
	int width = *(int*)&byte[22];
	int bitDepth = *(int*)&byte[28];

	printf("width: %d\n",width);
	printf("height: %d\n",height );

	int step=0,size = height*width;								//calculate image size

	if(bitDepth <= 8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}

	//unsigned char buffer[width][height];					//to store the image data
	//unsigned char out_buffer[width][height];
	//unsigned char mask[width][height];

	fread(buffer,sizeof(unsigned char),size,fIn);			//read the image data
	//AVERAGE FILTER
	/*int k=1;
	for(i=1;i<height-1;i++)
	{
		for(j=1;j<width-1;j++)
		{	//printf("%d\n",k);
			k++;
			tmp=0;
			for(x=-1;x<=1;x++)
				for(y=-1;y<=1;y++)
					tmp+=buffer[i+x][j+y];
			out_buffer[i][j]=tmp/9;
			printf("%d %d - %d\n",i,j,(int)out_buffer[i][j]);
		}
	}
	*/
	clock_t t; 
    	t = clock();
	for (i = 0; i < 8; i++) { 
  
        pthread_create(&thread[i], NULL, &avg_filter, (void*)step); 
        //pthread_create(&thread[i + CORE], NULL, &subtraction, (void*)step); 
        step+=128; 
    	} 
  
  for (i = 0; i < 8; i++) { 
	  
		pthread_join(thread[i], NULL); 
	    }
  
  
  
  step=0;
  	for (i = 0; i < 8; i++) { 
  
        pthread_create(&thread[i], NULL, &sharp_filter, (void*)step); 
        //pthread_create(&thread[i + CORE], NULL, &subtraction, (void*)step); 
        step+=128; 
    	} 
  
  
	    // Waiting for join threads after compute 
	    for (i = 0; i < 8; i++) { 
	  
		pthread_join(thread[i], NULL); 
	    } 
	t = clock() - t; 
    	double time_taken = ((double)t);
	printf("time = %lf\n\n",time_taken);
	
	
	//avg_filter();

	fwrite(mask,sizeof(unsigned char),size,fOut);		//write back to the output image

	fclose(fIn);
	fclose(fOut);

	return 0;
}
