#include <stdio.h>

/*
1) Given a string array. Count how many capital letters are in the string array str1.
2) Given an signed integer array. Write a function that generates the running product of the array. E.g. given {1,2,-3,-4,5} the running product is {1,2,-6,24,120}. Pass the arrays as pointers to the function. 
3) Grades is an unsigned integer array containing grades out of 100. Process the grades and generate an array of letter grades. You must use the switch statement.
4) M and N are unsigned integer values. Write a function that returns the greatest common denominator for M and N.
5) N is an unsigned integer. Calculate the Fibonacci F(N) for any N. 
*/ 

// GROUP MEMBERS: Brandon Vo, Yanwen Qi, Brandon Marshall, Christian Crout, Douglas Goff

// Question 1
void count_capital_letters(char str[]) {
    int i, count = 0;

                                                        // Loop through each character in the string array
    for (i = 0; str[i] != '\0'; i++) {
                                                        // Check if the character is an uppercase letter using ASCII range
        if (str[i] >= 'A' && str[i] <= 'Z') {
            count++;
        }
    }
    printf("Task 1: Number of Capital Letters = %d\n", count);
}



// Question 2
void running_product(int *arr, int *result, int size) {
    int i;
    int product = 1;                                
    
    // Loop through each element in the array
    for (i = 0; i < size; i++) {
        product *= arr[i];                              // Update the running product
        result[i] = product;                            // Store the running product in the result array
    }   

    printf("Task 2: The running product is: [");
    for (i = 0; i < size; i++) {
        printf("%d", result[i]);  
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}



// Question 3
void grades_array(unsigned int *grades, char *letterGrades, int size) {
    int i;
    
    // Loop through each grade in the array, divide by 10 to round any single digits to a base 10 number
    for (i = 0; i < size; i++) {
        switch (grades[i] / 10) {
            case 10:
            case 9:
                letterGrades[i] = 'A';
                break;
            case 8:
                letterGrades[i] = 'B';
                break;
            case 7:
                letterGrades[i] = 'C';
                break;
            case 6:
                letterGrades[i] = 'D';
                break;
            default:
                letterGrades[i] = 'F';
                break;
        }
    }

    printf("Task 3: The array of letter grades is: [");
    for (i = 0; i < size; i++) {
        printf("'%c'", letterGrades[i]);  
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}



// Question 4
void GCD(unsigned int M, unsigned int N){
    unsigned int original_M = M;
    unsigned int original_N = N;
     while(M!=N)
    {
        if(M > N)
            M = M - N;
        else
            N = N - M;
    }
    
    printf("Task 4: GCD of %u and %u --> %u\n", original_M, original_N, M );
}



// Question 5
int fib(int N)
{
    if (N <= 1)
        return N;
    return fib(N - 1) + fib(N - 2);
}


// Driver Codes
int main(){
    
    // Question 1
    char str1[] = "Hello World";
    count_capital_letters(str1);


    // Question 2
    int arr[] = {1, 2, -3, -4, 5};
    int size = sizeof(arr) / sizeof(arr[0]);    // Calculate the size of the array
    int result[size];                           // Array to store the running product
    running_product(arr, result, size);
    
    
    // Question 3
    unsigned int grades[] = {90,91, 80,10, 0, 100, 85, 76, 60, 45};
    size = sizeof(grades) / sizeof(grades[0]);
    char letterGrades[size]; 
    grades_array(grades, letterGrades, size);
    
    
    // Question 4
    unsigned int M = 186;
    unsigned int N = 46;
    GCD(M, N);
    
    // Question 5
    N = 5;
    printf("Task 5: F(%d) = %d", N, fib(7));
    return 0;
}
