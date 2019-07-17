#include<stdio.h>
#include<sys/types.h>
#include<pthread.h>
#include<stdlib.h>
//#include<bits/stdc++.h>
#include<unistd.h>
#define MAX 1024				// 1024 X 1024 image
#define HEIGHT 1024
#define WIDTH 1024
int step[4][2]={{0,0},{0,512},{512,0},{512,512}};		// store row and column value for each box
int index[4]={0,1,2,3};			// store index to be passed in thread for extracting row and column value in filter function
unsigned char buffer[HEIGHT][WIDTH];					//to store the image data
unsigned char out_buffer[HEIGHT][WIDTH];
unsigned char mask[HEIGHT][WIDTH];
pthread_t thread[4]; 		// make 4 threads
//AVERAGE FILTER
void *avg_filter(void *arg)
{
	int core = *((int *)arg);
	printf("----%d---%d---%d\n",core,step[core][0],step[core][1]);
	int row=step[core][0],col=step[core][1];
	int i,j,tmp,x,y,height=1024,width=1024,k;
	for(i=row ; i <= (row + 511); i++)
	{
		for(j=col;j<=col+511;j++)
		{	
			k++;
			tmp=0;
			for(x=-1;x<=1;x++)
				for(y=-1;y<=1;y++)
					tmp+=buffer[i+x][j+y];
			out_buffer[i][j]=tmp/9;
		}
	}
}

//SHARP FILTER
void *sharp_filter(void *arg)
{
	int core = *((int *)arg); 
	int i,j,height=1024,width=1024;
	int row=step[core][0],col=step[core][1];
	printf("*****%d---%d---%d\n",core,step[core][0],step[core][1]);
	for(i=row;i<=( row + 511 );i++)
	{
		for(j=col;j<=col+511 ;j++)
		{
			mask[i][j]=abs(out_buffer[i][j]-buffer[i][j]);  // compute mask / edges
		}
	}
	
	for(i=row;i<=row+511;i++)
	{
		for(j=col;j<=col+511;j++)
		{
			out_buffer[i][j]=buffer[i][j]+mask[i][j];   	// compute sharp image
		}
	}
	
}


//  MIN FILTER
void min_filter()
{
	int min;
	//int core = *((int *)arg);
	//printf("----%d---%d---%d\n",core,step[core][0],step[core][1]);
	//int row=step[core][0],col=step[core][1];
	//int i,j,tmp,x,y,height=1024,width=1024,k;
	for(int i=2 ; i <= (1021); i++)
	{
		for(int j=2;j<=1021;j++)
		{	
			//k++;
			//tmp=0;
			min=999;
			for(int x=-2;x<=2;x++)
				for(int y=-2;y<=2;y++)
					if(min > buffer[i+x][j+y])
						min = buffer[i+x][j+y];
					//tmp+=buffer[i+x][j+y];
			out_buffer[i][j]=min;
		}
	}
}


//  MAX FILTER
void max_filter()
{
	int max1;
	//int core = *((int *)arg);
	//printf("----%d---%d---%d\n",core,step[core][0],step[core][1]);
	//int row=step[core][0],col=step[core][1];
	//int i,j,tmp,x,y,height=1024,width=1024,k;
	for(int i=2 ; i <= (1021); i++)
	{
		for(int j=2;j<=1021;j++)
		{	
			//k++;
			//tmp=0;
			max1=-1;
			for(int x=-2;x<=2;x++)
				for(int y=-2;y<=2;y++)
					if(max1 < buffer[i+x][j+y])
						max1 = buffer[i+x][j+y];
					//tmp+=buffer[i+x][j+y];
			out_buffer[i][j]=max1;
		}
	}
}


int main()
{
	pthread_t thread[8]; 
	FILE *fIn = fopen("man.bmp","r");					//Input File name
	FILE *fOut = fopen("out.bmp","w+");		    		//Output File name

	int i,j,choice,x,y,tmp,k;
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

	int size = height*width,step2[2]={0,0};					//calculate image size

	if(bitDepth <= 8)										//if ColorTable present, extract it.
	{
		fread(colorTable,sizeof(unsigned char),1024,fIn);
		fwrite(colorTable,sizeof(unsigned char),1024,fOut);
	}
	int ch;
	fread(buffer,sizeof(unsigned char),size,fIn);			//read the image data
	printf("*******MENU*******\n");
	printf("1 - average filter\n");
	printf("2 - sharpen filter\n");
	printf("3 - max filter\n");
	printf("4 - min filter\n");
	printf("ENTER CHOICE : ");
	scanf("%d",&ch);
	clock_t t; 			// note starting time
    	t = clock();
	switch(ch)
    {
    	case 1:
    			for(i=0;i<4;i++)			// start threads to average filter
					pthread_create(&thread[i], NULL, &avg_filter, (void*)&index[i]); // pass index of step array to extract row and column value
    			for(j=0;j<4;j++)			// join threads from average filter
					pthread_join(thread[j], NULL);
				fwrite(out_buffer,sizeof(unsigned char),size,fOut);		//write back to the output image
    			break;
    	case 2:
    			for(i=0;i<4;i++)			// start threads to average filter
					pthread_create(&thread[i], NULL, &avg_filter, (void*)&index[i]); // pass index of step array to extract row and column value
    			for(j=0;j<4;j++)			// join threads from average filter
					pthread_join(thread[j], NULL);
					
				for (i = 0; i < 4; i++) 	// start threads to sharp filter
        			pthread_create(&thread[i], NULL, &sharp_filter, (void*)&index[i]); 

				for (i = 0; i < 4; i++) 	// join threads from sharp filter
					pthread_join(thread[i], NULL); 	
    			
    			fwrite(mask,sizeof(unsigned char),size,fOut);		//write back to the output image
    			
    			break;
    	case 3:
 				max_filter();
 				fwrite(out_buffer,sizeof(unsigned char),size,fOut);		//write back to the output image   	
    			break;
    	case 4:
 				min_filter();   
 				fwrite(out_buffer,sizeof(unsigned char),size,fOut);		//write back to the output image	
    			break;
    	default :
    			printf("wrong choice\n");
    			
    }
    
    		
	/*for(i=0;i<4;i++)			// start threads to average filter
		pthread_create(&thread[i], NULL, &avg_filter, (void*)&index[i]); // pass index of step array to extract row and column value

    for(j=0;j<4;j++)			// join threads from average filter
		pthread_join(thread[j], NULL); 
  
  	for (i = 0; i < 4; i++) 	// start threads to sharp filter
        pthread_create(&thread[i], NULL, &sharp_filter, (void*)&index[i]); 

	for (i = 0; i < 4; i++) 	// join threads from sharp filter
		pthread_join(thread[i], NULL); 
	*/
	t = clock() - t; 			// note ending time
    double time_taken = ((double)t);	// compute time taken in millisecond
	printf("time = %lf\n\n",time_taken);
	
	//fwrite(out_buffer,sizeof(unsigned char),size,fOut);		//write back to the output image

	fclose(fIn);
	fclose(fOut);

	return 0;
}
