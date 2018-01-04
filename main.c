#include "dsplib/fft.h"
#include "dsplib/process.h"
#include "stdio.h"
#include "main.h"
/*varlue*/
/* function  */
void init_func();

//int main(int argc, char* argv[])
int main()
{  
    init_func();
    do_once_srp();
}


/* function  */
//*************************
// *init function */
//************************
//    
void init_func(){
    /*fft init start*/
    myFFT = (struct objFFT*) malloc(sizeof(struct objFFT));
    fftInit(myFFT , FrameSize);
    /*fft init end*/
}

