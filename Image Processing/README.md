# Average and sharpen filter
3X3 average filter is used in the above implementation.
In the implementation a 1024 X 1024 bmp image is used.
This code works for a N x N square image. In order to used another image, modify the HEIGHT,
WIDTH and MAX in the code accordingly.

### row_wise_thread_filter.c 
This file has implementation of filter by using 8 threads.
Each thread works on a band of 128 rows and 1024 columns(entire width) of image.
eg. 1st thread works from 0 to 127th row, 2nd thread works for 128th to 255th row and so on.

