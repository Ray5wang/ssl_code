#include "dsplib/fft.h"
#include "stdio.h"
#define FrameSize 4096
#define MIC 16
#define RAW_BUFFER_SIZE (SAMPLES_PER_FRAME * NB_MICROPHONES)
#define SAMPLES_PER_FRAME 512
struct objFFT* myFFT;
struct ParametersStruct* parament;

struct mic_Array{
    float loc_x;
    float loc_y;
    float loc_z;
    float mic_signal[FrameSize];
    float mic_real[FrameSize];
    float mic_Imag[FrameSize];
};

/*varlue*/
const char  binfileAddr[12]="./test.bin ";   //bin文件的路径
struct mic_Array mic[8];
struct mic_Array ss[8];

/* function  */
void showfft(float *real ,float *imag);
void caculate_gccphat(struct mic_Array  *mic);
void sprphat();
void point_multi(struct mic_Array data1,struct mic_Array data2,float *result_real,float *result_Imag,int N);
void init_func();
int data_read();
//int main(int argc, char* argv[])
int main()
{
    init_func();
    caculate_gccphat(mic);
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



//*************************
/*cacullate the gcc-phat */
//************************
void caculate_gccphat(struct mic_Array *mic){
    int i,j;
    float temp[FrameSize];
    printf("do fft ...\n");
    for(i=0;i<MIC/2;i++){                                               //do fft
        fftComputeTwice(myFFT,  mic[0].mic_signal, mic[1].mic_signal
                         , mic[0].mic_real,  mic[0].mic_Imag, 
                           mic[1].mic_real,  mic[1].mic_Imag);
    }
    //Calculate the cross-power spectrum: = fft(x1).*conj(fft(x2))
    for(i=0;i<MIC;i++){
        for(j=i+1;j<MIC;j++){
            point_multi(*(mic+i),*(mic+j),ss[j].mic_real,ss[j].mic_Imag,FrameSize); //ss = MIC[first_data]*MIC[j];
            ifftComputeOnce(myFFT, ss[j].mic_real,ss[j].mic_Imag ,temp);
            printf("%d%d  ",i,j);
        }
        printf("\n");
    }
}

//********************************
/*cacullate the point multi of */
//********************************
void point_multi(struct mic_Array data1,struct mic_Array data2,float *result_real,float *result_Imag,int N){
    int i;
    float phat;
    for(i=0;i<N;i++){
    result_real[i] =  data1.mic_real[i] * data2.mic_real[i] - data1.mic_Imag[i] * data2.mic_Imag[i];
    result_Imag[i] =  data1.mic_real[i] * data2.mic_Imag[i] + data1.mic_Imag[i] * data2.mic_real[i];
   // phat = sqrt( result_real[i]* result_real[i]+result_Imag[i]*result_Imag[i] );
    phat = 1;//debug 用
   // result_real[i] = result_real[i]/phat;
   // result_Imag[i] = result_Imag[i]/phat;
    }
}

//show the result of fft
void showfft(float *real ,float *imag){
    int i;
    for (i=0;i<FrameSize;i++){
        printf("%f+%fj \n",real[i],imag[i]);
    }
}


//***********************
/*caculate the spr-phat*/
//***********************
//p
//
void sprphat(){

}

//***********************
/*read the data 
 *micN   type:double      
 *tdoa                      */
//***********************
int  data_read(){
  double num=0;
  int i;
  FILE *fp;//判断命令行是否正确
            //按读方式打开由argv[1]指出的文件
  if((fp=fopen(binfileAddr,"r"))==NULL){
        printf("The file <%s> can not be opened.\n",binfileAddr);//打开操作不成功
            return -1;//结束程序的执行
                }
                    for(i=1;i<=256;i++){
       // fscanf(fp, "%f",num);
           fread( &num, sizeof(double), 1, fp ); 
               printf("The num %d is %lf\n",i,num);
    }
    fclose(fp); //关闭fp所指文件
    return 0;
 }
