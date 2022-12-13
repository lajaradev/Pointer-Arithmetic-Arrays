<h2 align="center"> Sequential iterative system </h2>
***
<h6> 
Develop a program that implements the following iterative scheme for k=0..m. Taking into account that the vector x0 will be the unit vector (= 1,1,....1). The value of the number of iterations (m) will be specified as argument. The size of the vectors will be equal to N=15000.

Iteration 1: x1 = M x0

Iteration 2: x2 = M x1; Find the largest absolute value of x2, if it is greater than the literal value 25.0, convert x2 to values between [-1,1], dividing all elements by that value.

Iteration 3: x3 = M x2; Find the largest absolute value of x3, if it is greater than the literal value 25.0,
converting x3 to values between [-1,1], and dividing all elements by that value.

The matrix M (square of size NxN) will be stored in a binary file, if said file does not exist the values of M are generated, with elements equal to 1 on the diagonal and the rest will be generated randomly
or pseudorandomly and will be between -50.0 < x < 50.0, so that the elements do not diagonals of the lower triangular half will be positive and of the upper triangular negative.

The number of iterations (m) and filenames will be passed in the execution statement. M will be a row-by-row reserved array, i.e. double pointer. The elements of the matrix and the vectors are of type
double.

gcc iterativeSystem.c -o it && ./it matrix.bin 5

</h6>
