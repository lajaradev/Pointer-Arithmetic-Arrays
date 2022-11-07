<h2 align="center"> Sequential image processing </h2>
***
<h6> 
An image in shades of gray stored in a binary file with a raw extension will be read, that is, the data they will be of type unsigned char and the size of the file will be height x width x 1byte. 

The read image will be stored in a two-dimensional array, the processed images will be stored in binary files in raw format.
Processed to implement (one will be chosen passing parameter):

Filtered by mean (3 x 3 elements)
Filtered by median (3 x 3 elements)
Edge detection (SOBEL)

In edge detection by SOBEL, those elements that do not have the 8 YES neighbors will be considered, performing symmetric extension for both rows (row -1 will be equal to 1, row N will be equal to N-2, where N is the number of rows) and for columns.
</h6>
