#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#define  SIZE   4096
int main()  
{ 
    float data;
    float data_array[SIZE];
    int i;
    for (i=0;i<SIZE;i++)
    {   
        scanf("%f",&data);
        data_array[i];
    }
    for (i=0;i<SIZE;i++){
    printf("%f, ",data_array[i]);
    }
    return 0;
} 
