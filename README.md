# Thread-Programming
<div align="center" style="margin: 20px">
  <img src="https://res.cloudinary.com/practicaldev/image/fetch/s--I12SreaX--/c_imagga_scale,f_auto,fl_progressive,h_500,q_auto,w_1000/https://thepracticaldev.s3.amazonaws.com/i/d8xzk4f2ckytkexkv5cf.jpg" height="200" width="680">
</div>
## Image Processing
### Average and sharpen filter
3X3 average filter is used in the above implementation.
In the implementation a 1024 X 1024 bmp image is used.
This code works for a N x N square image. In order to used another image, modify the HEIGHT,
WIDTH and MAX in the code accordingly.

### row_wise_thread_filter.c 
This file has implementation of filter by using 8 threads.
Each thread works on a band of 128 rows and 1024 columns(entire width) of image.
eg. 1st thread works from 0 to 127th row, 2nd thread works for 128th to 255th row and so on.

### filter_2x2_partioned_image.cpp
This file has implementation of filter by using 4 threads.
Unlike previous code, this implementation has each thread working on one-forth of the image.
Image is divided into 4 equal parts.
1st thread works from (0,0) to (511,511) pixel  
2nd thread works from (0,512) to (511,1023) pixel  
3rd thread works from (512,0) to (1023,511) pixel  
4th thread works from (512,512) to (1023,1023) pixel  

### Rotate.c
This code provides the method to rotate an image 90 degree right / left and 180 degree.
